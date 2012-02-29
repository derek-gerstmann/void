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

#include "gs.h"
#include "log.h"

#include <string.h>
#include <math.h>

/**************************************************************************************************/

gs_data_t*
gsAllocateData()
{
	gs_data_t* gs = (gs_data_t*)malloc(sizeof(gs_data_t));
    memset(gs, 0, sizeof(gs_data_t));
	return gs;
}

void 
gsDestroyData(gs_data_t* gs)
{
	int i;
	if(gs == NULL)
		return;
		
	for(i = 0; i < GADGET_IO_NBLOCKS; i++)
		gsDestroyBlockData(gs, i);

	free(gs);
}

size_t
gsAllocateBlockData(
	gs_data_t* gs, 
	gs_block_type_t block)
{
	void* ptr = gsGetBlockDataPtr(gs, block);
	size_t bytes = gsGetBlockEntrySize(block) * gs->TotalParticleCount;
	if(ptr == NULL)
	{
		ptr = (float*)malloc(bytes);
		if(ptr == NULL)
		{
			Error("Failed to allocate memory for reading '%s' data!  Aborting...\n", 
				gsGetBlockName(block));
	
			exit(0);
		}
		memset(ptr, 0, bytes);
	}
	
	gsSetBlockDataPtr(gs, block, ptr);
	
	return bytes;
}

void
gsDestroyBlockData(
	gs_data_t* gs, 
	gs_block_type_t block)
{
	void* ptr = gsGetBlockDataPtr(gs, block);
	if(ptr == NULL)
		return;
	
	free(ptr);
	gsSetBlockDataPtr(gs, block, NULL);
	return;
}

void*
gsGetBlockDataPtr(
	gs_data_t* gs,
	gs_block_type_t block)
{
	if(gs == NULL)
		return NULL;
		
	switch (block)
	{
		case GADGET_IO_POS:	
			return gs->Data.Position;
		case GADGET_IO_VEL:		
			return gs->Data.Velocity;
		case GADGET_IO_ACCEL:
			return gs->Data.Acceleration;
		case GADGET_IO_ID:
			return gs->Data.Id;
		case GADGET_IO_TYPE:
			return gs->Data.Type;
		case GADGET_IO_MASS:
			return gs->Data.Mass;
		case GADGET_IO_U:				
			return gs->Data.InternalEnergy;
		case GADGET_IO_RHO:				
			return gs->Data.Density;
		case GADGET_IO_HSML:			
			return gs->Data.SmoothingLength;
		case GADGET_IO_POT:				
			return gs->Data.Potential;
		case GADGET_IO_DTENTR:			
			return gs->Data.DtEntropy;
		case GADGET_IO_TSTP:			
			return gs->Data.TimeStep;
		default:						
			return NULL;
	}
	return NULL;
}

void
gsSetBlockDataPtr(
	gs_data_t* gs,
	gs_block_type_t block,
	void* ptr)
{
	if(gs == NULL)
		return;
		
	switch (block)
	{
		case GADGET_IO_POS:	
		{
			gs->Data.Position = (float*)ptr; 
			break;
		}
		case GADGET_IO_VEL:		
		{
			gs->Data.Velocity = (float*)ptr; 
			break;
		}
		case GADGET_IO_ACCEL:
		{
			gs->Data.Acceleration = (float*)ptr; 
			break;
		}
		case GADGET_IO_ID:
		{
			gs->Data.Id = (int*)ptr; 
			break;
		}
		case GADGET_IO_TYPE:
		{
			gs->Data.Type = (int*)ptr; 
			break;
		}
		case GADGET_IO_MASS:
		{
			gs->Data.Mass = (float*)ptr; 
			break;
		}
		case GADGET_IO_U:				
		{
			gs->Data.InternalEnergy = (float*)ptr; 
			break;
		}
		case GADGET_IO_RHO:				
		{
			gs->Data.Density = (float*)ptr; 
			break;
		}
		case GADGET_IO_HSML:			
		{
			gs->Data.SmoothingLength = (float*)ptr; 
			break;
		}
		case GADGET_IO_POT:				
		{
			gs->Data.Potential = (float*)ptr; 
			break;
		}
		case GADGET_IO_DTENTR:			
		{
			gs->Data.DtEntropy = (float*)ptr; 
			break;
		}
		case GADGET_IO_TSTP:			
		{
			gs->Data.TimeStep = (float*)ptr; 
			break;
		}
		default:						
			return;
	}
	return;
}

static int
gsReadTypeData(
	FILE* fd, gs_data_t* gs,
	int offset)
{
	int pt, pn;
	int pc = 0;

	gs_block_type_t block = GADGET_IO_TYPE;
	void* ptr = gsGetBlockDataPtr(gs, block);
	if(ptr == NULL)
		gsAllocateBlockData(gs, block);

	for(pt = 0; pt < GADGET_PT_NTYPES; pt++)
	{
		for(pn = 0; pn < gs->Header.Fields.ParticleCount[pt]; pn++)
		{
			gs->Data.Type[pc++] = pt;
		}
	}
	return pt;
}

size_t
gsGetBlockSeparatorSize(void)
{
	return sizeof(char) * 4; //	32-bit
}

static int
gsSkipToNextBlock(
	FILE* fh)
{
    int dummy;
    fread(&dummy, sizeof(dummy), 1, fh);
	return dummy;
}

#if 0
static size_t
gsFindBlockByName(
	FILE* fd, const char* label)
{
	size_t rd = 0;
    int blocksize=0;
    char blocklabel[5]= {"    "};

    while(!feof(fd) && blocksize == 0)
    {
		rd = fread(blocklabel, 4*sizeof(char), 1, fd);
		if(memcmp(label,blocklabel,4)==0)
			Info("Found Block <%s> ... \n",blocklabel);	
    }
    return(blocksize-8);
}

static size_t
gsFindBlock(
	FILE* fd, gs_block_type_t block)
{
	return gsFindBlockByName(fd, gsGetBlockLabel(block));
}
#endif

const char*
gsGetBlockLabel(
	gs_block_type_t block)
{
	switch (block)
	{
		case GADGET_IO_POS:				return "POS";
		case GADGET_IO_VEL:				return "VEL";
		case GADGET_IO_ID:				return "ID";
		case GADGET_IO_TYPE:			return "TYPE";
		case GADGET_IO_MASS:			return "MASS";
		case GADGET_IO_U:				return "U";
		case GADGET_IO_RHO:				return "RHO";
		case GADGET_IO_HSML:			return "HSML";
		case GADGET_IO_POT:				return "POT";
		case GADGET_IO_ACCEL:			return "ACCE";
		case GADGET_IO_DTENTR:			return "ENDT";
		case GADGET_IO_TSTP:			return "TSTP";
		default:						return "\0\0\0\0";
	}
	return "\0\0\0\0";
}

const char*
gsGetBlockName(
	gs_block_type_t block)
{
	switch (block)
	{
    case GADGET_IO_POS:		return "Position";
    case GADGET_IO_VEL:		return "Velocity";
    case GADGET_IO_ID:		return "ParticleID";
    case GADGET_IO_TYPE:	return "ParticleType";
    case GADGET_IO_MASS:	return "Mass";
    case GADGET_IO_U:		return "InternalEnergy";
    case GADGET_IO_RHO:		return "Density";
    case GADGET_IO_HSML:	return "SmoothingLength";
    case GADGET_IO_POT:		return "Potential";
    case GADGET_IO_ACCEL:	return "Acceleration";
    case GADGET_IO_DTENTR:	return "RateOfChangeOfEntropy";
    case GADGET_IO_TSTP:	return "TimeStep";
	default:
      break;
    };
    return " ";
}

size_t
gsGetBlockEntrySize(
	gs_block_type_t block)
{
	switch (block)
	{
		case GADGET_IO_POS:				
		case GADGET_IO_VEL:		
		case GADGET_IO_ACCEL:
			return 3 * sizeof(float);
		case GADGET_IO_ID:
			return 1 * sizeof(int); 	/* XXX - Could be sizeof(long long) if LONGID was defined for compiled sim code */
		case GADGET_IO_TYPE:
			return 1 * sizeof(int); 	
		case GADGET_IO_MASS:
		case GADGET_IO_U:				
		case GADGET_IO_RHO:				
		case GADGET_IO_HSML:			
		case GADGET_IO_POT:				
		case GADGET_IO_DTENTR:			
		case GADGET_IO_TSTP:			
			return sizeof(float);
		default:						
			return 0;
	}
	return 0;
}

size_t
gsGetBlockVectorSize(
	gs_block_type_t block)
{
	switch (block)
	{
		case GADGET_IO_POS:				
		case GADGET_IO_VEL:		
		case GADGET_IO_ACCEL:
			return 3;
		case GADGET_IO_ID:
		case GADGET_IO_TYPE:
			return 1; 	
		case GADGET_IO_MASS:
		case GADGET_IO_U:				
		case GADGET_IO_RHO:				
		case GADGET_IO_HSML:			
		case GADGET_IO_POT:				
		case GADGET_IO_DTENTR:			
		case GADGET_IO_TSTP:			
			return 1;
		default:						
			return 0;
	}
	return 0;
}

const char*
gsGetBlockSizeTypeSuffix(
	gs_block_type_t block)
{
	switch (block)
	{
		case GADGET_IO_POS:				
		case GADGET_IO_VEL:		
		case GADGET_IO_ACCEL:
			return "3f";
		case GADGET_IO_ID:
		case GADGET_IO_TYPE:
			return "1i"; 	
		case GADGET_IO_MASS:
		case GADGET_IO_U:				
		case GADGET_IO_RHO:				
		case GADGET_IO_HSML:			
		case GADGET_IO_POT:				
		case GADGET_IO_DTENTR:			
		case GADGET_IO_TSTP:			
			return "1f";
		default:						
			return " ";
	}
	return "";
}

int
gsIsBlockIntegerData(
	gs_block_type_t block)
{
	switch (block)
	{
		case GADGET_IO_POS:				
		case GADGET_IO_VEL:		
		case GADGET_IO_ACCEL:
			return 0;
		case GADGET_IO_ID:
		case GADGET_IO_TYPE:
			return 1; 	
		case GADGET_IO_MASS:
		case GADGET_IO_U:				
		case GADGET_IO_RHO:				
		case GADGET_IO_HSML:			
		case GADGET_IO_POT:				
		case GADGET_IO_DTENTR:			
		case GADGET_IO_TSTP:			
			return 0;
		default:						
			return 0;
	}
	return 0;
}

static int
gsReadBlockData(
	FILE* fd, 
	gs_data_t* gs, 
	gs_block_type_t block,
	int offset)

{
	size_t rd = 0;
	if(gs == NULL || fd == 0)
		return 0;
		
	void* ptr = gsGetBlockDataPtr(gs, block);
	if(ptr == NULL)
		gsAllocateBlockData(gs, block);
		
	ptr = gsGetBlockDataPtr(gs, block);
	size_t bytes = gsGetBlockEntrySize(block) * gs->TotalParticleCount;
	
	gsSkipToNextBlock(fd);
	rd = fread(ptr, bytes, 1, fd);
	gsSkipToNextBlock(fd);
	
	return rd;
}

static int
gsReadPositionData(
	FILE* fd, gs_data_t* gs, 
	int offset)
{
	return gsReadBlockData(fd, gs, GADGET_IO_POS, offset);
}

static int
gsReadVelocityData(
	FILE* fd, gs_data_t* gs, 
	int offset)
{
	return gsReadBlockData(fd, gs, GADGET_IO_VEL, offset);
}

static int
gsReadIdentifierData(
	FILE* fd, gs_data_t* gs, 
	int offset)
{
	return gsReadBlockData(fd, gs, GADGET_IO_ID, offset);
}

static int
gsReadMassData(
	FILE* fd, gs_data_t* gs, 
	int offset)
{
	int pt, pn;
	int pc = 0;
	size_t rd = 0;
	int per_particle_mass = 0;

	gs_block_type_t block = GADGET_IO_MASS;
	void* ptr = gsGetBlockDataPtr(gs, block);
	if(ptr == NULL)
		gsAllocateBlockData(gs, block);
		
	size_t bytes = gsGetBlockEntrySize(block) * gs->TotalParticleCount;
	for(pt = 0; pt < GADGET_PT_NTYPES; pt++)
	{
		if(gs->Header.Fields.Mass[pt] == 0.0f)
		{
			per_particle_mass += gs->Header.Fields.ParticleCount[pt];
		}
		else
		{
			for(pn = 0; pn < gs->Header.Fields.ParticleCount[pt]; pn++)
			{
				gs->Data.Mass[pc++] = gs->Header.Fields.Mass[pt];
			}
		}
	}
	
	if(per_particle_mass > 0)
	{
		gsSkipToNextBlock(fd);
		rd = fread(gs->Data.Mass, bytes, 1, fd);
		gsSkipToNextBlock(fd);	
		return rd;
	}

	return pc;
}

static int 
gsReadInternalEnergyData(
	FILE* fd, gs_data_t* gs, 
	int offset)
{
	return gsReadBlockData(fd, gs, GADGET_IO_U, offset);
}

static int
gsReadDensityData(
	FILE* fd, gs_data_t* gs, 
	int offset)
{
	if(gs->Header.Fields.ParticleCount[GADGET_PT_GAS] < 1)
		return 0;
	
	return gsReadBlockData(fd, gs, GADGET_IO_RHO, offset);
}

static int
gsReadSmoothingLengthData(
	FILE* fd, gs_data_t* gs, 
	int offset)
{
	if(gs->Header.Fields.ParticleCount[GADGET_PT_GAS] < 1)
		return 0;
	
	return gsReadBlockData(fd, gs, GADGET_IO_HSML, offset);
}

static int
gsReadPotentialData(
	FILE* fd, gs_data_t* gs, 
	int offset)
{
	return gsReadBlockData(fd, gs, GADGET_IO_POT, offset);
}

static int
gsReadAcceleration(
	FILE* fd, gs_data_t* gs, 
	int offset)
{
	return gsReadBlockData(fd, gs, GADGET_IO_ACCEL, offset);
}

static int
gsReadRateOfChangeOfEntropyData(
	FILE* fd, gs_data_t* gs, 
	int offset)
{
	return gsReadBlockData(fd, gs, GADGET_IO_DTENTR, offset);
}

static int
gsReadTimeStepData(
	FILE* fd, gs_data_t* gs, 
	int offset)
{
	int pt, pn;
	int pc = 0;
	size_t rd = 0;
	gs_block_type_t block = GADGET_IO_TSTP;

	void* ptr = gsGetBlockDataPtr(gs, block);
	if(ptr == NULL)
		gsAllocateBlockData(gs, block);
			
	size_t bytes = gsGetBlockEntrySize(block) * gs->TotalParticleCount;
	for(pt = 0; pt < GADGET_PT_NTYPES; pt++)
	{
		for(pn = 0; pn < gs->Header.Fields.ParticleCount[pt]; pn++)
		{
			gs->Data.TimeStep[pc++] = gs->Header.Fields.Time;
		}
	}
	
	gsSkipToNextBlock(fd);
	rd = fread(gs->Data.TimeStep, bytes, 1, fd);
	gsSkipToNextBlock(fd);	
	return rd;
}

int
gsSkipBlock(
	FILE* fd, gs_data_t* gs, 
	gs_block_type_t block,
	int start)
{
	int pt;
	size_t offset = 0;
	offset += gsGetBlockSeparatorSize();
	for(pt = 0; pt < GADGET_PT_NTYPES; pt++)
	{
		offset += gsGetBlockEntrySize(block) * gs->Header.Fields.ParticleCount[pt];
	}
	offset += gsGetBlockSeparatorSize();
	return fseek(fd,offset,1);
}

int
gsReadBlock(
	FILE* fd, gs_data_t* gs, 
	gs_block_type_t block,
	int offset)
{
	switch (block)
	{
		case GADGET_IO_POS:		
			return gsReadPositionData(fd, gs, offset);
		case GADGET_IO_VEL:				
			return gsReadVelocityData(fd, gs, offset);
		case GADGET_IO_ID:
			return gsReadIdentifierData(fd, gs, offset);
		case GADGET_IO_MASS:
			return gsReadMassData(fd, gs, offset);
		case GADGET_IO_U:				
			return gsReadInternalEnergyData(fd, gs, offset);
		case GADGET_IO_RHO:				
			return gsReadDensityData(fd, gs, offset);
		case GADGET_IO_HSML:			
			return gsReadSmoothingLengthData(fd, gs, offset);
		case GADGET_IO_POT:				
			return gsReadPotentialData(fd, gs, offset);
		case GADGET_IO_ACCEL:			
			return gsReadAcceleration(fd, gs, offset);
		case GADGET_IO_DTENTR:			
			return gsReadRateOfChangeOfEntropyData(fd, gs, offset);
		case GADGET_IO_TSTP:			
			return gsReadTimeStepData(fd, gs, offset);
		default:						
			return 0;
	}
	return 0;
}

float
gsPeriodic(
	float x, float l2)
{
    if(x > +l2)
		x -= l2 * 2.0f;
    if(x < -l2)
		x += l2 * 2.0f;
    return x;
}

static void
gsSwapFloatDataAt(
    float* ptr, int isrc, int idst)
{
    float t;
    float* src;
    float* dst;
    float* tmp;

    tmp = &t;
    src = &(ptr[isrc]);
    dst = &(ptr[idst]);

    memcpy(tmp, src, sizeof(float));
    memcpy(src, dst, sizeof(float));
    memcpy(dst, tmp, sizeof(float));
}

static void
gsSwapIntDataAt(
    int* ptr, int isrc, int idst)
{
    int t;
    int* src;
    int* dst;
    int* tmp;

    tmp = &t;
    src = &(ptr[isrc]);
    dst = &(ptr[idst]);

    memcpy(tmp, src, sizeof(int));
    memcpy(src, dst, sizeof(int));
    memcpy(dst, tmp, sizeof(int));
}

static void
gsSwapFloat3DataAt(
    float* ptr, int isrc, int idst)
{
    float t[3];
    float* src;
    float* dst;
    float* tmp;

    tmp = &t[0];
    src = &(ptr[isrc * 3]);
    dst = &(ptr[idst * 3]);

    memcpy(tmp, src, sizeof(float) * 3);
    memcpy(src, dst, sizeof(float) * 3);
    memcpy(dst, tmp, sizeof(float) * 3);
}

static void
gsSwapParticleAt(
    gs_data_t* gs, int isrc, int idst)
{
	if(gs->Data.Position) 			gsSwapFloat3DataAt(gs->Data.Position, isrc, idst);
	if(gs->Data.Velocity) 			gsSwapFloat3DataAt(gs->Data.Velocity, isrc, idst);
	if(gs->Data.Acceleration)		gsSwapFloat3DataAt(gs->Data.Acceleration, isrc, idst);
	if(gs->Data.Mass)				gsSwapFloatDataAt(gs->Data.Mass, isrc, idst);
	if(gs->Data.InternalEnergy)		gsSwapFloatDataAt(gs->Data.InternalEnergy, isrc, idst);
	if(gs->Data.Density)			gsSwapFloatDataAt(gs->Data.Density, isrc, idst);
	if(gs->Data.SmoothingLength)	gsSwapFloatDataAt(gs->Data.SmoothingLength, isrc, idst);
	if(gs->Data.Potential)			gsSwapFloatDataAt(gs->Data.Potential, isrc, idst);
	if(gs->Data.DtEntropy)			gsSwapFloatDataAt(gs->Data.DtEntropy, isrc, idst);
	if(gs->Data.Type)				gsSwapIntDataAt(gs->Data.Type, isrc, idst);
	if(gs->Data.Id)					gsSwapIntDataAt(gs->Data.Id, isrc, idst);	
}

int 
gsReadHeader(
	FILE* fd,
	gs_data_t* gs,
	int splits)
{
    int k;

	size_t rd;
	gsSkipToNextBlock(fd);
	rd = fread(&(gs->Header), sizeof(gs_header_t), 1, fd);
	gsSkipToNextBlock(fd);

	if(splits == 1)
	{
		for(k = 0, gs->TotalParticleCount = 0; k < GADGET_PT_NTYPES; k++)
			gs->TotalParticleCount += gs->Header.Fields.ParticleCount[k];

		gs->GasParticleCount = gs->Header.Fields.ParticleCount[0];
	}
	else
	{
		for(k = 0, gs->TotalParticleCount = 0; k < GADGET_PT_NTYPES; k++)
			gs->TotalParticleCount += gs->Header.Fields.ParticleCountTotal[k];

		gs->GasParticleCount = gs->Header.Fields.ParticleCountTotal[0];
	}
	return rd;
}

void
gsConvertUnits(gs_data_t* gs)
{
    double GRAVITY, BOLTZMANN, PROTONMASS;
    double UnitLength_in_cm, UnitMass_in_g, UnitVelocity_in_cm_per_s;
    double UnitTime_in_s, UnitDensity_in_cgs, UnitPressure_in_cgs, UnitEnergy_in_cgs;
    double G, Xh, HubbleParam, Ne;

    int i;
    double MeanWeight, u, gamma;

    /* physical constants in cgs units */
    GRAVITY = 6.672e-8;
    BOLTZMANN = 1.3806e-16;
    PROTONMASS = 1.6726e-24;

    /* internal unit system of the code */
    UnitLength_in_cm = 3.085678e21;	/*  code length unit in cm/h */
    UnitMass_in_g = 1.989e43;	/*  code mass unit in g/h */
    UnitVelocity_in_cm_per_s = 1.0e5;

    UnitTime_in_s = UnitLength_in_cm / UnitVelocity_in_cm_per_s;
    UnitDensity_in_cgs = UnitMass_in_g / pow(UnitLength_in_cm, 3);
    UnitPressure_in_cgs = UnitMass_in_g / UnitLength_in_cm / pow(UnitTime_in_s, 2);
    UnitEnergy_in_cgs = UnitMass_in_g * pow(UnitLength_in_cm, 2) / pow(UnitTime_in_s, 2);

    G = GRAVITY / pow(UnitLength_in_cm, 3) * UnitMass_in_g * pow(UnitTime_in_s, 2);
	Ne = 1.0;

    Xh = 0.76;			/* mass fraction of hydrogen */
    HubbleParam = 0.65;

	if(!gs->Data.Temp || !gs->Data.InternalEnergy)
		return;
		
    for(i = 0; i < gs->TotalParticleCount; i++)
    {
        if(gs->Data.Type[i] == GADGET_PT_GAS)
        {
            MeanWeight = 4.0 / (3 * Xh + 1 + 4 * Xh * Ne) * PROTONMASS;

            /* convert internal energy to cgs units */

            u = gs->Data.InternalEnergy[i] * UnitEnergy_in_cgs / UnitMass_in_g;

            gamma = 5.0 / 3;

            /* get temperature in Kelvin */

            gs->Data.Temp[i] = MeanWeight / BOLTZMANN * (gamma - 1) * u;
        }
    }
}

int
gsLoadSnapshot(
	gs_data_t* gs, 
	const char* prefix, 
	int splits,
	int* req_data,
	int* req_types)
{
    FILE* fd;
    int i, n, pc, pc_new;
    
    size_t length = strlen(prefix) * 2;
    char* filename = malloc(length);
	if(!filename)
	{
		Error("Failed to allocate buffer for filename!  Aborting.... \n");
		exit(0);
	}
	
    for(i = 0, pc = 1; i < splits; i++, pc = pc_new)
    {
        if(splits > 1)
            snprintf(filename, length, "%s.%d", prefix, i);
        else
            snprintf(filename, length, "%s", prefix);

        if(!(fd = fopen(filename, "r")))
        {
            Error("Failed to open Gadget snapshot file '%s'!  Aborting.... \n", filename);
            exit(0);
        }

        Info("Reading snapshot '%s' ...\n", filename);
        fflush(stdout);

		if(!gsReadHeader(fd, gs, splits))
		{
            Error("Failed to read Gadget header from file '%s'!  Aborting...\n", filename);
            exit(0);
        }

		gsReadTypeData(fd, gs, pc);
		
		for(n = 0; n < GADGET_IO_NBLOCKS; n++)
		{
			if(req_data[n])
				gsReadBlock(fd, gs, n, pc);
			else
				gsSkipBlock(fd, gs, n, pc);
		}

        fclose(fd);
    }
    
    free(filename);
    return 1;
}

void gsReorder(
    gs_data_t* gs)
{
    int i;
    int idst;

    Info("Reordering '%lu' particles ...\n", gs->TotalParticleCount);

    for(i = 0; i < gs->TotalParticleCount; i++)
    	gs->Data.Id[i]--;
        	
    for(i = 0; i < gs->TotalParticleCount; i++)
    {
        if(gs->Data.Id[i] != i)
        {
            idst = gs->Data.Id[i];
            if(idst > gs->TotalParticleCount)
                continue;

            do
            {
                gsSwapParticleAt(gs, idst, i);
                if(idst == i)
                    break;

                idst = gs->Data.Id[i];
            }
            while(1);
        }
    }
    
    for(i = 0; i < gs->TotalParticleCount; i++)
    	gs->Data.Id[i]++;
}

/**************************************************************************************************/

