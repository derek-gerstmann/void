/***************************************************************************************************
//
// License:    The MIT License
//
// Copyright (c) 2011. Derek Gerstmann, The University of Western Australia.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
***************************************************************************************************/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <time.h>
#include <ctype.h>
#include <xlocale.h>
#include <sys/time.h>

/**************************************************************************************************/

#include "raw.h"
#include "gs.h"
#include "log.h"
#include "timer.h"

/**************************************************************************************************/

static void
Usage(const char* progName)
{
    Info("Usage: %s [options]\n", progName);
    Info("options:\n");
    Info("  -h, --help                  This message\n");
    Info("  -q, --quiet                 Disable diagnostic output\n");
    Info("  -r, --reorder               Reorder and sort particles based on their Ids\n");
    Info("  -u, --unwrap                Unwrap positions using periodic boundary based on header info.\n");
    Info("  -i, --input <str>           Path and basename of Gadget v2.0 snapshot file to use for input\n");
    Info("  -s, --snapshots <int>       Number of snapshots to process (starting from the given input file) [default=1].\n");
    Info("  -n, --nfiles <int>          Number of files per snapshot [default=1].\n");
    Info("  -o, --output <str>          Path and basename of file to use for exporting the particle data [default=input]\n");
    Info("  -f, --format <str>          File extension identifiying the format of output file to generate (either CSV, ASC or RAW) [default=raw}\n");
    Info("  -t, --type <str>            Name of the type of particles to examine (either 'all', 'gas', 'halo', 'disk', 'bulge', 'stars', or 'bndry') [default=all]\n");
    Info("  -d, --data <str>            Block or field name of data to output.  Field must be one of the following:\n");
   	Info("                                'hdr'  - header information (exported as JSON text)\n");
   	Info("                                'pos'  - position\n");
   	Info("                                'vel'  - velocity\n");
   	Info("                                'id'   - particle id\n");
   	Info("                                'mass' - mass (only for variable masses)\n");
   	Info("                                'u'    - internal energy per unit mass (only avail for SPH)\n");
   	Info("                                'rho'  - density (only avail for SPH)\n");
   	Info("                                'hsml' - smoothing length (only avail for SPH)\n");
   	Info("                                'pot'  - gravitational potential (if avail)\n");
   	Info("                                'acce' - acceleration (if avail)\n");
   	Info("                                'endt' - rate of change of entropic function (only avail for SPH)\n");
   	Info("                                'type' - particle type identifier (values outputted are integers: 0=gas, 1=halo, 2=disk, 3=bulge, 4=stars, 5=bndry)\n");
   	Info("                                'tstp' - time step of particles (if avail)\n");
    exit(1);
}

static void
ParseOptions(gscr_options_t* opts, int argc, char* argv[])
{
    int opt;

    static struct option long_options[] =
    {
        {"help",         0, 0, 'h'},
        {"quiet",        0, 0, 'q'},
        {"reorder",      0, 0, 'r'},
        {"unwrap", 	 	 0, 0, 'u'},
        {"input",        required_argument, 0, 'i'},
        {"snapshots",    0, 0, 's'},
        {"nfiles",       0, 0, 'n'},
        {"output",       0, 0, 'o'},
        {"format",       0, 0, 'f'},
        {"type",   	 	 0, 0, 't'},
        {"data",     	 required_argument, 0, 'd'},
    };
	
    while((opt = getopt_long(argc, argv, "hqrus:n:i:o:f:t:d:", long_options, NULL)) != EOF)
    {
        switch(opt)
        {
            case 'q': opts->quiet = 1; break;
            case 'r': opts->reorder = 1; break;
            case 'u': opts->unwrap = 1; break;
            case 's': opts->snapshots = atoi(optarg); break;
            case 'n': opts->splits = atoi(optarg); break;
            case 'i': opts->input = strdup(optarg); break;
            case 'o': opts->output = strdup(optarg); break;
            case 'f': opts->format = strdup(optarg); break;
            case 't': opts->type = strdup(optarg); break;
            case 'd': opts->data = strdup(optarg); break;
            case 'h':
            default:
                Usage(argv[0]);
                break;
        }
    }

    if(!opts->input)
    {
        Warning("ERROR: Input path and basename is required!\n");
        exit(-1);
    }

    if(!opts->snapshots)
        opts->snapshots = 1;

    if(!opts->splits)
        opts->splits = 1;

    if(!opts->reorder)
        opts->reorder = 0;

    if(!opts->output)
    {
        opts->output = strdup(opts->input);
    }

    if(!opts->format)
    {
        opts->format = strdup("raw");
    }

    if(!opts->type)
    {
        opts->type = strdup("all");
    }

    if(!opts->data)
    {
        opts->data = strdup("all");
    }

    return;
}

void 
DestroyOptions(gscr_options_t* opts)
{
	if(opts == NULL)
		return;
		
	if(opts->input)
		free(opts->input);

	if(opts->output)
		free(opts->output);

	if(opts->format)
		free(opts->format);

	if(opts->type)
		free(opts->type);

	if(opts->data)
		free(opts->data);

	memset(opts, 0, sizeof(gscr_options_t));
}

/**************************************************************************************************/

void ExportRawFloatFile(
    const char* filename, size_t count, float* vd, int vn)
{
    FILE* fh;
    size_t written;

    fh = OpenWriteOnlyRawFile(filename);
    written = WriteRawFloats(fh, vd, count * vn);
    CloseRawFile(fh);
}

void ExportRawIntFile(
    const char* filename, size_t count, int* vd, int vn)
{
    FILE* fh;
    size_t written;
    
    fh = OpenWriteOnlyRawFile(filename);
    written = WriteRawInts(fh, vd, count * vn);
    CloseRawFile(fh);
}

void ExportRawByteFile(
    const char* filename, size_t count, char* vd, int vn)
{
    FILE* fh;
    size_t written;

    fh = OpenWriteOnlyRawFile(filename);
    written = WriteRawBytes(fh, vd, count * vn);
    CloseRawFile(fh);
}

void ExportAscFloatFile(
    const char* filename, size_t count, float* vd, int vn)
{
    size_t i = 0;
    int e = 0;
    int en = vn - 1;
    FILE* fh;

    fh = fopen(filename, "w+");
    for(i = 0; i < count; i++)
    {
        if((i % (1024 * 1024)) == 0)
            Info("%f ...\n", (double)i / (double)count * 100.0);

        for(e = 0; e < vn; e++)
            fprintf(fh, "%f%c", vd[i * vn + e], (e < en) ? ',' : '\n');
    }

    fclose(fh);
}

void ExportAscIntFile(
    const char* filename, size_t count, int* vd, int vn)
{
    size_t i = 0;
    int e = 0;
    int en = vn - 1;
    FILE* fh;

    fh = fopen(filename, "w+");
    for(i = 0; i < count; i++)
    {
        if((i % (1024 * 1024)) == 0)
            Info("%f ...\n", (double)i / (double)count * 100.0);

        for(e = 0; e < vn; e++)
            fprintf(fh, "%d%c", vd[i * vn + e], (e < en) ? ',' : '\n');
    }

    fclose(fh);
}

void ExportAscByteFile(
    const char* filename, size_t count, char* vd, int vn)
{
    size_t i = 0;
    int e = 0;
    int en = vn - 1;
    FILE* fh;

    fh = fopen(filename, "w+");
    for(i = 0; i < count; i++)
    {
        if((i % (1024 * 1024)) == 0)
            Info("%f ...\n", (double)i / (double)count * 100.0);

        for(e = 0; e < vn; e++)
            fprintf(fh, "%d%c", vd[i * vn + e], (e < en) ? ',' : '\n');
    }

    fclose(fh);
}

void ExportDataBlock(
	gs_block_type_t block_type, const char* prefix,
	size_t count, void* vd,  int ascii)
{
	int i = 0;
	char filename[4096] = {0};
	char lowercase[4096] = {0};

	const char* suffix = gsGetBlockSizeTypeSuffix(block_type);
	const char* label = gsGetBlockLabel(block_type);
	size_t vn = gsGetBlockVectorSize(block_type);
	
	size_t length = strlen(label);
	for(i = 0; i < length; i++)
		lowercase[i] = tolower(label[i]);
	
	if(ascii)
	{
		sprintf(filename, "%s_%s_%lu_%s.asc", prefix, lowercase, count, suffix);
    	Info("Exporting '%s' values to '%s' ... \n", gsGetBlockName(block_type), filename);
		ExportAscFloatFile(filename, count, vd, vn);
	}
	else
	{
		sprintf(filename, "%s_%s_%lu_%s.raw", prefix, lowercase, count, suffix);
		Info("Exporting '%s' values to '%s' ... \n", gsGetBlockName(block_type), filename);
		ExportRawFloatFile(filename, count, vd, vn);
	}	
}

void ExportColumns(
    gscr_options_t* opt,
    gs_data_t* gs,
    const char* prefix)
{
    FILE* fh;
    size_t i = 0;
    size_t p = 0;
    size_t e = 0;
    size_t c = 0;
    static const char* comp = "XYZW0123456789";
    char filename[4096] = {0};
    int columns = 0;

    size_t total_count = gs->TotalParticleCount;

    sprintf(filename, "%s.csv", prefix);

    Info("Exporting column data '%s'...\n", filename);

    fh = fopen(filename, "w+");

    for(i = 0; i < GADGET_IO_NBLOCKS; i++)
    {
        gs_block_type_t block_type =  (gs_block_type_t)i; 
        void* ptr = gsGetBlockDataPtr(gs, block_type);
        if(ptr == NULL)
            continue;

        columns++;
    }

    // Export all blocks that were requested    
    c = 0;
    for(i = 0; i < GADGET_IO_NBLOCKS; i++, c++)
    {
        gs_block_type_t block_type =  (gs_block_type_t)i; 
        if(block_type == GADGET_IO_ID)
            continue;

        block_type = (i == 0) ? GADGET_IO_ID : block_type;
        void* ptr = gsGetBlockDataPtr(gs, block_type);
        if(ptr == NULL)
            continue;
            
        const char* label = gsGetBlockName(block_type);
        int vn = gsGetBlockVectorSize(block_type);
        fprintf(fh, "%s", (c > 0) ? ", " : "");

        if(vn > 1)
        {
            for(e = 0; e < vn; e++)
                fprintf(fh, "%s\"%s%c\"", (e > 0) ? ", " : "", label, comp[(int)e]);
        }
        else
        {
            fprintf(fh, "\"%s\"", label);
        }
    }

    fprintf(fh, "\n");

    // Export all blocks that were requested    
    for(p = 0; p < total_count; p++)
    {
        if((p % (1024 * 1024)) == 0)
            Info("%f ...\n", (double)p / (double)total_count * 100.0);

        c = 0;
        for(i = 0; i < GADGET_IO_NBLOCKS; i++, c++)
        {
            gs_block_type_t block_type =  (gs_block_type_t)i; 
            if(block_type == GADGET_IO_ID)
                continue;

            block_type = (i == 0) ? GADGET_IO_ID : block_type;
            void* ptr = gsGetBlockDataPtr(gs, block_type);
            if(ptr == NULL)
                continue;
                
            size_t vn = gsGetBlockVectorSize(block_type);

            fprintf(fh, "%s", (c > 0) ? ", " : "");
            if(gsIsBlockIntegerData(block_type))
            {
                int* vi = (int*)ptr;
                for(e = 0; e < vn; e++)
                    fprintf(fh, "%s%d", (e > 0) ? ", " : "",  vi[p * vn + e]);
            }
            else
            {
                float* vf = (float*)ptr;
                for(e = 0; e < vn; e++)
                    fprintf(fh, "%s%f", (e > 0) ? ", " : "", vf[p * vn + e]);
            }
        }
        fprintf(fh, "\n");
    }

    fclose(fh);
}

void gsExportHeader(
    gscr_options_t* opt,
    gs_data_t* gs,
    const char* prefix)
{
    FILE* fh;
    char filename[4096] = {0};

    sprintf(filename, "%s_header.json", prefix);

    Info("Exporting header '%s'...\n", filename);

    fh = fopen(filename, "w+");
    fprintf(fh, "{ \"%s\": {\n", opt->input);
    fprintf(fh, "\t\"ParticleCount\": [ %d, %d, %d, %d, %d, %d ],\n",
            gs->Header.Fields.ParticleCount[0], gs->Header.Fields.ParticleCount[1], gs->Header.Fields.ParticleCount[2],
            gs->Header.Fields.ParticleCount[3], gs->Header.Fields.ParticleCount[4], gs->Header.Fields.ParticleCount[5]);
    fprintf(fh, "\t\"Mass\": [ %f, %f, %f, %f, %f, %f ],\n",
            gs->Header.Fields.Mass[0], gs->Header.Fields.Mass[1], gs->Header.Fields.Mass[2],
            gs->Header.Fields.Mass[3], gs->Header.Fields.Mass[4], gs->Header.Fields.Mass[5]);
    fprintf(fh, "\t\"Time\": %f,\n",
            gs->Header.Fields.Time);
    fprintf(fh, "\t\"Redshift\": %f,\n",
            gs->Header.Fields.Redshift);
    fprintf(fh, "\t\"HasStarFormation\": %d,\n",
            gs->Header.Fields.HasStarFormation);
    fprintf(fh, "\t\"HasFeedback\": %d,\n",
            gs->Header.Fields.HasFeedback);
    fprintf(fh, "\t\"ParticleCountTotal\": [ %d, %d, %d, %d, %d, %d ],\n",
            gs->Header.Fields.ParticleCountTotal[0], gs->Header.Fields.ParticleCountTotal[1], gs->Header.Fields.ParticleCountTotal[2],
            gs->Header.Fields.ParticleCountTotal[3], gs->Header.Fields.ParticleCountTotal[4], gs->Header.Fields.ParticleCountTotal[5]);
    fprintf(fh, "\t\"HasCooling\": %d,\n",
            gs->Header.Fields.HasCooling);
    fprintf(fh, "\t\"FileCountPerSnapshot\": %d,\n",
            gs->Header.Fields.FileCountPerSnapshot);
    fprintf(fh, "\t\"BoxSize\": %f,\n",
            gs->Header.Fields.BoxSize);
    fprintf(fh, "\t\"Omega0\": %f,\n",
            gs->Header.Fields.Omega0);
    fprintf(fh, "\t\"OmegaLambda\": %f,\n",
            gs->Header.Fields.OmegaLambda);
    fprintf(fh, "\t\"HubbleParam\": %f\n",
            gs->Header.Fields.HubbleParam);
    fprintf(fh, "\t}\n");
    fprintf(fh, "}\n");
    fclose(fh);
}

void ExportBlocks(
    gscr_options_t* opt,
    gs_data_t* gs,
    const char* prefix)
{
    int i;
    int ascii = strcasestr(opt->format, "asc") != NULL;
    int all = strcasestr (opt->data, "all") != NULL;
    
    // Type will always be loaded so we only export if requested
    if(gs->Data.Type && (all || strcasestr(opt->data, "type") != NULL))
    {
        ExportDataBlock( GADGET_IO_TYPE, prefix, gs->TotalParticleCount, gs->Data.Type, ascii);
        gsDestroyBlockData(gs, GADGET_IO_TYPE);
    }   

    // Export all other blocks that were requested    
    for(i = 0; i < GADGET_IO_NBLOCKS; i++)
    {
        void* ptr = gsGetBlockDataPtr(gs, i);
        if(ptr == NULL)
            continue;
            
        ExportDataBlock( i, prefix, gs->TotalParticleCount, ptr, ascii);
        gsDestroyBlockData(gs, i);
    }
    
}

void gsExport(
    gscr_options_t* opt,
    gs_data_t* gs,
    const char* prefix)
{
	int all = strcasestr (opt->data, "all") != NULL;
    int csv = strcasestr (opt->format, "csv") != NULL;
	
	// Export the header if requested
	if(all || strcasestr(opt->data, "hdr") != NULL )
	{
	    gsExportHeader(opt, gs, prefix);
	}

    if(csv)
    {
        ExportColumns( opt, gs, prefix );
    }
    else
    {
        ExportBlocks( opt, gs, prefix );
    }
}

int
gsLoadSnapshotData(
	gscr_options_t* opt,
	gs_data_t* gs,
	const char* filename)
{
    int req_data[GADGET_IO_NBLOCKS] = {0};
    int req_types[GADGET_PT_NTYPES] = {0};
    int all = 0;    
	int i;
	
	all = strcasestr(opt->data, "all") != NULL;
    for(i = 0; i < GADGET_IO_NBLOCKS; i++)
    	if(all || strcasestr(opt->data, gsGetBlockLabel(i)))
    		req_data[i] = 1;

	if(opt->reorder)
		req_data[GADGET_IO_ID] = 1;
		
	all = strcasestr(opt->type, "all") != NULL;
    for(i = 0; i < GADGET_PT_NTYPES; i++)
    	if(all || strcasestr(opt->type, gsGetBlockLabel(i)))
    		req_types[i] = 1;
    		
    return gsLoadSnapshot(gs, filename, opt->splits, req_data, req_types);
}


int main(int argc, char** argv)
{
    frequency_t frequency = {0};
    time_delta_t t0 = {0};
    time_delta_t t1 = {0};
	char* prefix = 0;
	int start = 0;
	int padding = 0;
    int i, k, s, p;
    double dt;
	
    gscr_options_t opt = {0};
    ParseOptions(&opt, argc, argv);

    if(!opt.quiet)
    {
        Info(SEPARATOR);
        Info("Loading input data from file '%s'\n", opt.input);
        Info(SEPARATOR);
    }

    frequency = GetTimerFrequencyForHost();
    t0 = t1 = GetCurrentTimeForHost();

	size_t length = strlen(opt.input);
	for(i = length-1; i >= 0 && isdigit(opt.input[i]); --i)
		padding++;
		
	prefix = malloc(length);
	snprintf(prefix, length - padding, "%s", opt.input);
	start = atoi(opt.input + (length - padding));
	
	Info("Starting prefix  '%s' at '%d' with padded index '%d' ...\n", prefix, start, padding);
	
    for(s = start; s <= opt.snapshots + start; s++)
    {
    	char digits[1024] = {0};
    	snprintf(digits, sizeof(digits), "%d", s);
    	size_t digit_len = strlen(digits);
    	
		char filename[4096] = {0};
        snprintf(filename, length + padding + digit_len, "%s_", prefix);
		for(p = 0; p < padding - digit_len; p++)
			strcat(filename, "0");
		strcat(filename, digits);

	    gs_data_t* gs = gsAllocateData();

		t0 = GetCurrentTimeForHost();
		gsLoadSnapshotData(&opt, gs, filename);
		t1 = GetCurrentTimeForHost();
	
		if(!opt.quiet)
		{
			dt = SubtractTimeInSecondsForHost(t1, t0, frequency);
			Info("Loaded snapshot  '%s' in '%.2f' ms\n", filename, 1000.0 * dt);
		}
	
		if(opt.reorder)
		{
            Info(SEPARATOR);
			t0 = GetCurrentTimeForHost();
			gsReorder(gs);
			t1 = GetCurrentTimeForHost();
	
			if(!opt.quiet)
			{
				dt = SubtractTimeInSecondsForHost(t1, t0, frequency);
				Info("Reordered '%lu' particles in '%.2f' ms\n", gs->TotalParticleCount, 1000.0 * dt);
                Info(SEPARATOR);
			}
		}
	
		if(opt.unwrap && gs->Data.Position)
		{
			t0 = GetCurrentTimeForHost();
			for(i = 0; i < gs->TotalParticleCount; i++)
			{
				for(k = 0; k < 3; k++)
					gs->Data.Position[i * 3 + k] = gsPeriodic(gs->Data.Position[i * 3 + k], gs->Header.Fields.BoxSize * 0.5f);
			}
			t1 = GetCurrentTimeForHost();
	
			if(!opt.quiet)
			{
				dt = SubtractTimeInSecondsForHost(t1, t0, frequency);
				Info("Unwrapped '%lu' particles in '%.2f' ms\n", gs->TotalParticleCount, 1000.0 * dt);
			}
		}
	
		t0 = GetCurrentTimeForHost();
		gsConvertUnits(gs);
		t1 = GetCurrentTimeForHost();
		
		t0 = GetCurrentTimeForHost();
		if(!opt.quiet) Info(SEPARATOR);
		
        gsExport(&opt, gs, filename);

		if(!opt.quiet) Info(SEPARATOR);
		t1 = GetCurrentTimeForHost();
	
		if(!opt.quiet)
		{
			dt = SubtractTimeInSecondsForHost(t1, t0, frequency);
			Info("Exported data for '%lu' particles in '%.2f' ms\n", gs->TotalParticleCount,  1000.0 * dt);
			Info(SEPARATOR);
		}
	
		gsDestroyData(gs);
	}
	
	DestroyOptions(&opt);
	free(prefix);
	
    Info("DONE!");
    exit(0);
}

/**************************************************************************************************/

