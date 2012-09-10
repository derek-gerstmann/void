// ============================================================================================== //
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
// ============================================================================================== //

#include "formats/whd.h"
#include "formats/csv.h"
#include "core/asserts.h"
#include "core/utilities.h"
#include "core/process.h"
#include "core/numerics.h"
#include "constants/constants.h"

#include <string.h>
#include <math.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

// ============================================================================================== //

VD_FORMATS_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_IMPORT(Core, Thread);
VD_IMPORT(Core, Memory);
VD_IMPORT(Core, Process);

// ============================================================================================== //

namespace HiDist {

// ============================================================================================== //

unsigned int FileRead( 
    std::istream & is, 
    std::vector <char> & buff ) 
{
    is.read( &buff[0], buff.size() );
    return is.gcount();
}

unsigned int CountLines( 
    const std::vector <char> & buff, int sz ) 
{
    int newlines = 0;
    const char * p = &buff[0];
    for ( int i = 0; i < sz; i++ ) {
        if ( p[i] == '\n' ) {
                newlines++;
        }
    }
    return newlines;
}

size_t GetLineCount(const char* filename)
{
    const int SZ = 16 * 1024 * 1024;
    std::vector <char> buff( SZ );
    std::ifstream ifs(filename);

    size_t n = 0;
    while( int cc = FileRead( ifs, buff ) ) 
    {
        n += CountLines( buff, cc );
    }
    return n;
}

float ToRadians(float v)
{
    static const float pi(Constants::Pi);
    static const float one_eighty(180.0);
    return v * (pi / one_eighty);
}

VD_DECLARE_ENUM(CosmoDistType,
    AgeOfUniverseNow,
    AgeOfUniverseThen,
    ComovingHorizonOfUniverseNow,
    ComovingHorizonOfUniverseThen,
    ComovingDistanceOfSourceNow,
    ComovingDistanceOfSourceThen,
    AngularDiameterDistance,
    LuminosityDistance,
    LightTravelTimeDistance);

template<int CD>
struct CosmologicalDistance 
{
    static float Compute(float z)
    {
        /*
            cosmodis - A Cosmological Distances Program - version 1.1
            by Richard Powell - http://www.atlasoftheuniverse.com/

            This is a simple piece of code to provide comoving, angular diameter,
            luminosity, and light travel distances for any given redshift.  The
            Hubble constant (H0), Omega_matter (OM) and Omega_lambda (OL) are defined
            within the body of the program and can be adjusted to your favourite
            values.

            For a summary of the formulae used in this program, see:
            David Hogg, Distance Measures in Cosmology, (2000), astro-ph/9905116.
            http://arxiv.org/abs/astro-ph/9905116

            This program is in the Public Domain.  There is no copyright.
        */

        double c = 299792.458;          // Speed of Light (C)

        double H0 = 70;                // Hubble constant (km/s/Mpc) - adjust according to taste
        double OM = 0.27;              // Omega(matter) - adjust according to taste
        double OL = 0.73;              // Omega(lambda) - adjust according to taste
        double OR = 0.42/(H0*H0);      // Omega(radiation) - this is the usual textbook value
        long i;
        long n=10000;                 // Number of steps in the integral
        double OK = 1-OM-OR-OL;        // Omega(k) defined as 1-OM-OR-OL
        double HD = 3.2616*c/H0/1000;  // Hubble distance (billions of light years).  See section 2 of Hogg
        double a, adot;                // Scale Factor "a", and its derivative "adot"
        double DC, DCC=0, DT, DTT=0, DA, DL, DM;
        double age, size;              // The age and size of the universe

        return HD * z;

        for(i=n; i>=1; i--) {         // This loop is the numerical integration
            a = (i-0.5)/n;              // Steadily decrease the scale factor
                                        // Comoving formula (See section 4 of Hogg, but I've added a radiation term too):
            adot = a*sqrt(OM*pow(1/a,3)+OK*pow(1/a,2)+OL+OR*pow(1/a,4));    // Note that "a" is equivalent to 1/(1+z)
            DCC = DCC + 1/(a*adot)/n;   // Running total of the comoving distance
            DTT = DTT + 1/adot/n;       // Running total of the light travel time (see section 10 of Hogg)
            if (a>=1/(1+z)) {           // Collect DC and DT until the correct scale is reached
              DC = DCC;                 // Comoving distance DC
              DT = DTT;                 // Light travel time DT
            }
        }
        
        // Transverse comoving distance DM from section 5 of Hogg:
        if (OK>0.0001) DM=(1/sqrt(OK))*sinh(sqrt(OK)*DC);
        else if (OK<-0.0001) DM=(1/sqrt(fabs(OK)))*sin(sqrt(fabs(OK))*DC);
        else DM=DC;

        age = HD*DTT;                 // Age of the universe (billions of years)
        size = HD*DCC;                // Comoving radius of the observable universe

        DC = HD*DC;                   // Comoving distance
        DA = HD*DM/(1+z);             // Angular diameter distance (section 6 of Hogg)
        DL = HD*DM*(1+z);             // Luminosity distance (section 7 of Hogg)
        DT = HD*DT;                   // Light travel distance

        switch (CosmoDistType::FromInteger(CD))
        {
            case CosmoDistType::AgeOfUniverseNow:              { return age; }         // (Gyr) Giga Years 
            case CosmoDistType::AgeOfUniverseThen:             { return age-DT; }      // (Gyr) Giga Years 
            case CosmoDistType::ComovingHorizonOfUniverseNow:  { return size; }        // (Gly) Giga Light-Years 
            case CosmoDistType::ComovingHorizonOfUniverseThen: { return size/(1+z); }  // (Gly) Giga Light-Years 
            case CosmoDistType::ComovingDistanceOfSourceNow:   { return DC; }          // (Gly) Giga Light-Years 
            case CosmoDistType::ComovingDistanceOfSourceThen:  { return DC/(1+z); }    // (Gly) Giga Light-Years
            case CosmoDistType::AngularDiameterDistance:       { return DA; }          // (Gly) Giga Light-Years
            case CosmoDistType::LuminosityDistance:            { return DL; }          // (Gly) Giga Light-Years
            case CosmoDistType::LightTravelTimeDistance:       { return DT; }          // (Gly) Giga Light-Years
            default:                                           { return DL; }
        }
        return DL;

    #if 0
          printf("-------------------------------------------------------------------\n");
          printf("For Redshift %.3f, (Ho=%.1fkm/s/Mpc, Omega_M=%.2f, Omega_L=%.2f):\n",z,H0,OM,OL);
          printf("-------------------------------------------------------------------\n");
          printf("* Age of the universe now               = %.3f Gyr\n",age);
          printf("* Age of the universe then              = %.6f Gyr\n",age-DT);
          printf("* Comoving horizon of the universe now  = %.3f Gyr\n",size);
          printf("* Comoving horizon of the universe then = %.3f Gyr\n",size/(1+z));
          printf("* Comoving distance of the source now   = %.3f Gly\n",DC);
          printf("* Comoving distance of the source then  = %.3f Gly\n",DC/(1+z));  // In a flat universe, this is equal to DA
          printf("* Angular Diameter distance             = %.3f Gly\n",DA);
          printf("* Luminosity distance                   = %.3f Gly\n",DL);
          printf("* Light Travel Time Distance            = %.3f Gly\n",DT);
          printf("-------------------------------------------------------------------\n");
    #endif
    }
};

float* EquatorialToCelestialCartesian(
    float* ra, float* dec, float* distance,
    size_t count)
{
//    static const double EarthRadiusKm  = 6367.0; //radius in km
    static const double EarthRadiusGly = 6.7345338474424e-19; // radius in Gly

    float* pos = VD_NEW_ARRAY(float, count*3);

    for(size_t i = 0; i < count; i++)
    {
        float rav = ra[i];// + VD_F32_TINY_VAL;
        float decv = dec[i];// + VD_F32_TINY_VAL;
        float distv = distance[i];// + VD_F32_TINY_VAL;

        double theta = ToRadians(rav);
        double phi = ToRadians(decv);

        double d = EarthRadiusGly + CosmologicalDistance<CosmoDistType::LuminosityDistance>::Compute(distv);
        d *= 0.306601352201;

        float vx = d * Core::Cos(theta) * Core::Cos(phi);
        float vy = d * Core::Sin(theta) * Core::Cos(phi);
        float vz = d * Core::Sin(phi);

        pos[i * 3 + 0] = vx;
        pos[i * 3 + 1] = vy;
        pos[i * 3 + 2] = vz;

#if 0
        if((i % 1000) == 0)
            printf("Coords: %d / %d [%f, %f @ %f => %f %f %f => %f Gpc]\n", 
                (int)i, (int)count, 
                rav, decv, distv,
                vx, vy, vz, d);
#endif                
    }

    return pos;
}

float LinearRemap(float x, 
    float a, float b,
    float c, float d)
{
    float xp = ((d - c) * (x - a) / (b - a)) + c;
    return xp;
}
    
float* SpecificStarFormationRate(
    float* xyz, float* sfr, float* mstar, size_t count)
{
    size_t bad_sfr = 0;
    size_t bad_mstar = 0;
    size_t bad_ssfr = 0;
    size_t blue = 0;
    size_t green = 0;
    size_t red = 0;
    size_t dark_red = 0;
    
    float* v = VD_NEW_ARRAY(float, count);

    printf("SpecificStarFormationRate: %d\n", int(count));

    for(size_t i = 0; i < count; i++)
    {
        bool bad = false;

        if(Core::IsWithinUlps(sfr[i], Constants::Zero, 2) || 
           Core::IsWithinUlps(sfr[i], Constants::Infinity, 2) ||
           Core::IsWithinUlps(sfr[i], Constants::NegInf, 2) ||
           Core::IsWithinUlps(sfr[i], Constants::NaN, 2))
        {
            v[i] = -18.0f; // -16.201031f;
            bad_sfr++;
            bad = true;
        }

        if(bad == false && (
           Core::IsWithinUlps(mstar[i], Constants::Zero, 2) || 
           Core::IsWithinUlps(mstar[i], Constants::Infinity, 2) ||
           Core::IsWithinUlps(mstar[i], Constants::NegInf, 2) ||
           Core::IsWithinUlps(mstar[i], Constants::NaN, 2)))
        {
            v[i] = -18.0f;
            bad_mstar++;
            bad = true;
        }

        if(bad)
        {
            bad_ssfr++;
        }
        else
        {
//            v[i] = Core::Log10(sfr[i] / mstar[i]);
            v[i] = Core::Log10(sfr[i] / mstar[i]);
        }

        v[i] = -v[i];
    }

    // EG:    Min[-16.349771] Max[-8.277701]

    printf("BadSFR[%d => %f percent] BadMStar[%d => %f percent] BadSSFR[%d => %f percent]\n",
        (int)bad_sfr, bad_sfr/(float)count * 100.0, 
        (int)bad_mstar, bad_mstar/(float)count * 100.0,
        (int)bad_ssfr, bad_ssfr/(float)count * 100.0);

    printf("Blue[%d] Green[%d] Red[%d] DarkRed[%d]\n",
        (int)blue, (int) green, (int)red, (int)dark_red);

    return v;
}

Data::Data(
    Dataset* dataset) :
    Core::Object(),
    m_DataSet(dataset),
    m_TotalGalaxyCount(0),
    m_FileIndex(0)
{
    Memory::SetBytes(&m_MetaData,         0, sizeof(m_MetaData));
    Memory::SetBytes(&m_GalaxyData,       0, sizeof(m_GalaxyData));
    Memory::SetBytes(&m_StatisticsData,   0, sizeof(m_StatisticsData));
}
 
Data::~Data()
{
    Destroy();
}

vd::bytesize
Data::GetResidentMemorySize() const
{
    vd::bytesize bytes = 0;
    for(vd::u32 i = 0; i < Block::Count; i++)
    {
        Data::Block::Value block = Data::Block::FromInteger(i);
        const char* ptr = (char*)GetBlockDataPtr(block);
        if(ptr != NULL)
        {
            bytes += GetBlockEntrySize(block) * m_TotalGalaxyCount;
        }
    }
    return bytes;
}

vd::status 
Data::Destroy()
{
    vdLogInfo("Destroying  '%d' ... ", m_FileIndex);
    
    for(vd::u32 i = 0; i < Block::Count; i++)
    {
        Data::Block::Value block = Data::Block::FromInteger(i);
        DestroyBlockData(block);
    }

    return Status::Code::Success;
}

vd::bytesize
Data::GetBlockSize(
    Block::Value block) const
{
    return GetBlockEntrySize(block) * m_TotalGalaxyCount;
}

vd::bytesize
Data::CreateBlockData(
    Block::Value block)
{
    float* ptr = (float*)GetBlockDataPtr(block);
    vd::bytesize count = GetBlockVectorLength(block) * m_TotalGalaxyCount;
    if(ptr != NULL)
        VD_SAFE_DELETE_ARRAY(ptr);

//    printf("Allocated[%s] %d\n", Block::ToString(block), (int)count);
    ptr = NULL;
    ptr = VD_NEW_ARRAY(float, count+1);
    if(ptr == NULL)
    {
        vdLogError("Failed to allocate memory for datafile block '%s' data!", 
            GetBlockName(block));

        return 0;
    }

    SetBlockDataPtr(block, ptr);
    return count * sizeof(vd::f32);
}

void
Data::DestroyBlockData(
    Block::Value block)
{
    char* ptr = (char*)GetBlockDataPtr(block);
    if(ptr == NULL)
        return;
    
    VD_SAFE_DELETE_ARRAY(ptr);
    SetBlockDataPtr(block, NULL);
    return;
}

void*
Data::GetBlockDataPtr(
    Block::Value block) const
{       
    if(Block::IsValid(block))
        return m_GalaxyData[Block::ToInteger(block)];

    return NULL;
}

void
Data::SetBlockDataPtr(
    Block::Value block, void* ptr) 
{
    if(Block::IsValid(block))
        m_GalaxyData[Block::ToInteger(block)] = (float*)ptr;
}

vd::bytesize
Data::GetBlockSeparatorSize(void) const
{
    return 0;
}

vd::i32
Data::SkipToNextBlock(
    FILE* fh)
{
    return 0;
}

vd::bytesize
Data::FindBlockByName(
    FILE* fd, const char* label)
{
    return 0;
}

vd::bytesize
Data::FindBlock(
    FILE* fd, Block::Value block)
{
    return FindBlockByName(fd, GetBlockLabel(block));
}

const char*
Data::GetBlockLabel(
    Block::Value block)  const
{
    return Block::ToString(block);
}

const char*
Data::GetBlockName(
    Block::Value block)  const
{
    return Block::ToString(block);
}

vd::bytesize
Data::GetBlockEntrySize(
    Block::Value block)  const
{
    if(Block::IsValid(block))
    {
        if(Block::Position == block)
            return 3 * sizeof(vd::f32);
        else
            return 1 * sizeof(vd::f32);
    }
    return 0;
}

vd::i32
Data::GetBlockVectorLength(
    Block::Value block) const
{
    if(Block::IsValid(block))
    {
        if(Block::Position == block)
            return 3;
        else
            return 1;
    }
    return 0;
}

const char*
Data::GetBlockSizeTypeSuffix(
    Block::Value block) const
{
    if(Block::IsValid(block))
        return "1f";
    return "";
}

bool
Data::IsBlockEntryIntegerValue(
    Block::Value block) const
{
    return false;
}

vd::f32
Data::Periodic(
    vd::f32 x, vd::f32 l2)
{
    if(x > +l2)
        x -= l2 * 2.0f;
    if(x < -l2)
        x += l2 * 2.0f;
    return x;
}

void
Data::SwapFloatDataAt(
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

void
Data::SwapIntDataAt(
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

void
Data::SwapFloat3DataAt(
    float* ptr, int isrc, int idst)
{
    float t[3];
    float* src;
    float* dst;
    float* tmp;

    tmp = &t[0];
    src = &(ptr[isrc * 3]);
    dst = &(ptr[idst * 3]);

    Memory::CopyBytes(tmp, src, sizeof(float) * 3);
    Memory::CopyBytes(src, dst, sizeof(float) * 3);
    Memory::CopyBytes(dst, tmp, sizeof(float) * 3);
}

void
Data::SwapGalaxyAt(
    int isrc, int idst)
{
    for(int i = 0; i < (int)Block::Count; i++)
    {
        float* ptr = (float*)GetBlockDataPtr(Block::FromInteger(i));
        if(ptr == NULL)
            continue;

        SwapFloatDataAt(ptr, isrc, idst);    
    }
}

vd::bytesize 
Data::ReadHeader(
    const char* filename, vd::i32 splits)
{
    struct stat statbuf;
    stat(filename, &statbuf);
    size_t bytes = statbuf.st_size;

    if(splits <= 1)
    {
        m_TotalGalaxyCount = 0;
    }

    m_TotalGalaxyCount = GetLineCount(filename);
    m_MetaData.GalaxyCount = m_TotalGalaxyCount;
    m_MetaData.BoxSize = 1000.0f;
    vdLogInfo("DataSet size: %llu (%d galaxies)", bytes, m_TotalGalaxyCount);
    return 1;
}

vd::string
Data::GetFilename(
    const vd::string& prefix, 
    vd::i32 index,
    vd::i32 padding)
{
    return Dataset::GetFilenameFor(prefix, index, padding);
}

vd::status
Data::Load(
    const vd::string& prefix, 
    vd::i32 index,
    vd::i32 splits,
    vd::i32 padding,
    vd::i32* requested_data,
    vd::i32* requsted_types)
{
    vdAssert(prefix.size() > 1);    

    vd::i32 pc = 1;
    vd::i32 total_pc = pc;

    vd::i32 req_data[Data::Block::Count] = {0};
    if(requested_data != NULL)
    {
        for(vd::u32 rdi = 0; rdi < Data::Block::Count; rdi++)
        {
            req_data[rdi] = requested_data[rdi];
        }
    }

    if(req_data[Data::Block::ToInteger(Data::Block::Position)] > 0)
    {
        req_data[Data::Block::ToInteger(Data::Block::RA)] = 1;
        req_data[Data::Block::ToInteger(Data::Block::DEC)] = 1;
        req_data[Data::Block::ToInteger(Data::Block::Redshift)] = 1;
    }

    if(req_data[Data::Block::ToInteger(Data::Block::SSRS)] > 0)
    {
        req_data[Data::Block::ToInteger(Data::Block::StellarMass)] = 1;
        req_data[Data::Block::ToInteger(Data::Block::SFR)] = 1;
    }

    m_IsLoaded.Set(0);
    m_FilteredGalaxyCount = 0;

    vd::string filename = Dataset::GetFilenameFor(prefix, index, padding);
    if(!ReadHeader(filename.c_str(), 0))
    {
        vdLogWarning("Failed to open  datafile file '%s'!", filename.c_str());
        return Status::Code::ReadError;
    }

    size_t field_count;
    FILE* fd = OpenCsvFile(filename.c_str(), &field_count);        
    if(fd == NULL)
    {
        vdLogWarning("Failed to open  datafile file '%s'!", filename.c_str());
        return Status::Code::ReadError;
    }

    vdLogInfo("Reading datafile '%s' with '%d' fields ...", 
        filename.c_str(), (int)field_count);

    for(vd::u32 ln = 0; ln < Data::Block::Count; ln++)
    {
        Data::Block::Value block = Data::Block::FromInteger(ln);
        if(block == Data::Block::Position)
            continue;

        if(block == Data::Block::SSRS)
            continue;

        if(req_data[ln] > 0)
        {
            vdLogInfo("Creating Block for '%s' with '%d' entries ...", 
                Data::Block::ToString(block), (int)GetTotalGalaxyCount());

            CreateBlockData(block);
        }
    }

    bool use_bin = false;
    FILE* out_file = NULL;
    std::string out_filename = filename + ".bin";

#if 1
    Core::FileSystem* fs = m_DataSet->GetRuntime()->GetFileSystem();
    if(fs->Exists(out_filename) == true)
    {
        use_bin = true;
        vdLogInfo("Using '%s'", out_filename.c_str());
        out_file = fopen(out_filename.c_str(), "rb");
    }
    else
    {
        vdLogInfo("Saving '%s'", out_filename.c_str());
        out_file = fopen(out_filename.c_str(), "wb");
    }
#endif

    size_t valid = 0;
    int misses = 0;
    bool more = true;
    while(more)
    {
        int hit = 0;
        for(vd::u32 bn = 0; bn < Block::Count; bn++)
        {
            Data::Block::Value block = Data::Block::FromInteger(bn);
            if(block == Data::Block::Position)
                continue;

            if(block == Data::Block::SSRS)
                continue;

            float v;
            bool miss = false;
            size_t bytes = 0;
            if(use_bin)
            {
                bytes = fread(&v, sizeof(float), 1, out_file);
                if(bytes < 1) miss = true;
            }
            else
            {
                v = ReadCsvFloatColumn(fd, &bytes);
                if(bytes < 1) miss = true;
                if(out_file) fwrite(&v, sizeof(float), 1, out_file);
            }

            if(miss == false && req_data[bn] > 0)
            {
                float* ptr = (float*)GetBlockDataPtr(block);
                if(ptr) ptr[valid] = v;
//                printf("Read[%d] '%s' : %f\n", (int)valid,  Data::Block::ToString(block), v);
            }

            if(miss)
                hit = 0;
            else
                hit = 1;

        }        
        
        if(misses > 100)
            more = false;

        if(hit)
            valid++;
        else
            misses++;
    }

    CloseCsvFile(fd);
    if(out_file) fclose(out_file);
    total_pc += pc;

    m_FilteredGalaxyCount = m_TotalGalaxyCount;
    vdLogInfo("Loaded '%d'/'%d' galaxies!", m_FilteredGalaxyCount, m_TotalGalaxyCount);

    float* xyz = NULL;
    if(req_data[Data::Block::ToInteger(Data::Block::Position)] >= 1)
    {
        float* ra = (float*)GetBlockDataPtr(Data::Block::RA);
        float* dec = (float*)GetBlockDataPtr(Data::Block::DEC);
        float* redshift = (float*)GetBlockDataPtr(Data::Block::Redshift);
//        float* redshift = (float*)GetBlockDataPtr(Data::Block::ObservedRedshift);

        printf("Computing EquatorialToCelestialCartesian for '%d' galaxies!\n", (int)m_TotalGalaxyCount);
        if(ra != NULL && dec != NULL && redshift != NULL)
        {
            xyz = EquatorialToCelestialCartesian(ra, dec, redshift, m_TotalGalaxyCount);
            SetBlockDataPtr(Data::Block::Position, xyz);
        }
    }
 
    if(req_data[Data::Block::ToInteger(Data::Block::SSRS)] >= 1)
    {
        float* sfr = (float*)GetBlockDataPtr(Data::Block::SFR);
        float* sm = (float*)GetBlockDataPtr(Data::Block::StellarMass);

        printf("Computing SSFR for '%d' galaxies!\n", (int)m_TotalGalaxyCount);
        if(sfr != NULL && sm != NULL)
        {
            float* ssfr = SpecificStarFormationRate(xyz, sfr, sm, m_TotalGalaxyCount);
            SetBlockDataPtr(Data::Block::SSRS, ssfr);
        }
    }

#if 0
    FILE* csv_file = NULL;
    std::string csv_filename = filename + ".csv";
    csv_file = fopen(csv_filename.c_str(), "w");
    printf("Exporting '%s' ...\n", csv_filename.c_str());
    for(vd::u32 n = 0; n < Block::Count && m_TotalGalaxyCount; n++)
    {
        Data::Block::Value block = Data::Block::FromInteger(n);
        float* ptr = (float*)GetBlockDataPtr(block);
        if(!ptr) continue;
        bool inner = (n != Block::Count - 1);
        vd::u32 vl = GetBlockVectorLength(block);
        if(vl > 1)
        {
            for(vd::u32 v = 0; v < vl; v++)
            {
                bool vi = (v != vl - 1);
                static const char* suffix[] = { "X", "Y", "Z", "W" };
                fprintf(csv_file, "%s%s%s",  Data::Block::ToString(block), suffix[v], vi ? ", " : (inner ? ", " : "\n"));
                printf("%s%s%s",  Data::Block::ToString(block), suffix[v], vi ? ", " : (inner ? ", " : "\n"));
            }
        }
        else
        {
            fprintf(csv_file, "%s%s",  Data::Block::ToString(block), inner ? ", " : "\n");
            printf("%s%s",  Data::Block::ToString(block), inner ? ", " : "\n");
        }
    }

    for(vd::u32 g = 0; g < m_TotalGalaxyCount; g++)
    {
        for(vd::u32 n = 0; n < Block::Count; n++)
        {
            Data::Block::Value block = Data::Block::FromInteger(n);
            float* ptr = (float*)GetBlockDataPtr(block);
            if(!ptr) continue;

            bool inner = (n != Block::Count - 1);
            vd::u32 vl = GetBlockVectorLength(block);
            if(vl > 1)
            {
                for(vd::u32 v = 0; v < vl; v++)
                {
                    bool vi = (v != vl - 1);
                    fprintf(csv_file, "%f%s", ptr[g * vl + v], vi ? ", " : (inner ? ", " : "\n"));
//                    printf("\"%E\"", ptr[g * vl + v], vi ? ", " : (inner ? ", " : "\n"));
                }
            }
            else
            {
                fprintf(csv_file, "%f%s", ptr[g], inner ? ", " : "\n");
            }
        }        
    }
    fclose(csv_file);
#endif

    m_FileIndex = index;
    m_IsLoaded.Increment();
    return 1;
}

const MetaData& 
Data::GetMetaData() const 
{ 
    return m_MetaData; 
}

Statistic& 
Data::GetStatistic(Block::Value v) 
{ 
    return m_StatisticsData[Block::ToInteger(v)]; 
}

void 
Data::SetStatistic(Block::Value v, const Statistic& stats) 
{
    m_StatisticsData[Block::ToInteger(v)] = stats; 
}

vd::u64
Data::GetTotalGalaxyCount() const 
{ 
    return m_TotalGalaxyCount; 
}

vd::u64
Data::GetFilteredGalaxyCount() const 
{ 
    return m_FilteredGalaxyCount; 
}

vd::u64
Data::GetGalaxyCountForType(GalaxyType::Value) const 
{ 
    return m_MetaData.GalaxyCount; 
}

vd::f32
Data::GetBoundaryBoxSize() const 
{ 
    return m_MetaData.BoxSize; 
}
    
vd::u32
Data::GetFileIndex() const 
{ 
    return m_FileIndex; 
}
    
bool
Data::IsLoaded() 
{ 
    return m_IsLoaded.Get() > 0 ? true : false; 
}
    
void 
Data::Reorder()
{
/*
    vd::i64 idst;
    for(vd::i64 i = 0; i < (vd::i64)m_TotalGalaxyCount; i++)
        m_GalaxyData.Id[i]--;
            
    for(vd::i64 i = 0; i < (vd::i64)m_TotalGalaxyCount; i++)
    {
        if(m_GalaxyData.Id[i] != i)
        {
            idst = m_GalaxyData.Id[i];
            if(idst > (vd::i64)m_TotalGalaxyCount)
                continue;

            do
            {
                SwapGalaxyAt(idst, i);
                if(idst == i)
                    break;

                idst = m_GalaxyData.Id[i];
            }
            while(1);
        }
    }
    
    for(vd::i64 i = 0; i < (vd::i64)m_TotalGalaxyCount; i++)
        m_GalaxyData.Id[i]++;
*/
}

// ============================================================================================== //

Dataset::Dataset(
    Runtime::Context* runtime) :
    Core::Object(),
    m_IsOpen(false),
    m_FileCount(0),
    m_LastResidentFileIndex(0),
    m_CurrentFileIndex(0),
    m_StartFileIndex(0),
    m_EndFileIndex(0),
    m_FileSplits(0),
    m_CacheSize(2*1024*1024*1024),
    m_Runtime(runtime)
{
    Memory::SetBytes(m_RequestedBlocks, 0, sizeof(m_RequestedBlocks));
    Memory::SetBytes(m_RequestedTypes, 0, sizeof(m_RequestedTypes));
    Memory::SetBytes(m_RequestedStatistics, 0, sizeof(m_RequestedStatistics));
}

Dataset::~Dataset() 
{ 
    Destroy();
}

vd::status 
Dataset::Destroy(void)
{
    vd::status result = Close();
    m_DataCache.Destroy();
    return result;
}

void 
Dataset::SetBlockRequest(Data::Block::Value v, vd::i32 enable) 
    { m_RequestedBlocks[Data::Block::ToInteger(v)] = enable; }

void 
Dataset::SetTypeRequest(Data::GalaxyType::Value v, vd::i32 enable) 
    { m_RequestedTypes[Data::GalaxyType::ToInteger(v)] = enable; }

void 
Dataset::SetStatisticRequest(Data::Block::Value v, vd::i32 enable) 
    { m_RequestedStatistics[Data::Block::ToInteger(v)] = enable; }

vd::status 
Dataset::Open(
    const vd::string& prefix, 
    vd::i32 count, 
    vd::i32 splits)
{
    m_Mutex.Lock();
    Memory::SetBytes(m_RequestedBlocks, 0, sizeof(m_RequestedBlocks));
    Memory::SetBytes(m_RequestedTypes, 0, sizeof(m_RequestedTypes));
    Memory::SetBytes(m_RequestedStatistics, 0, sizeof(m_RequestedStatistics));

    m_FilePrefix = prefix;
    m_FileNumberPadding = 0;
    m_StartFileIndex = 0;
    m_EndFileIndex = 1;
    m_FileSplits = 1; // splits;
    m_IsOpen = true;

    if(m_FilePrefix.find(".dat") == vd::string::npos)
        m_FilePrefix += ".dat";
    
    vdLogInfo("Opening dataset '%s' from '%d-%d' with padded index '%d' ...",
        m_FilePrefix.c_str(), m_StartFileIndex, m_EndFileIndex, m_FileNumberPadding);
    
    m_DataCache.Setup(2 * Constants::MemorySize::GB, 
        VD_BIND_MEMBER_FUNCTION(this, &Dataset::OnFetch), 
        VD_BIND_MEMBER_FUNCTION(this, &Dataset::OnEvict)
    );

    m_WorkCache.Setup(4, 
        VD_BIND_MEMBER_FUNCTION(this, &Dataset::OnSubmit), 
        VD_BIND_MEMBER_FUNCTION(this, &Dataset::OnComplete)
    );

    m_WorkQueue.Start(1);   
    m_CurrentFileIndex = m_StartFileIndex;
    m_Mutex.Unlock();

    return Status::Code::Success;
}
    
vd::string
Dataset::GetFilenameFor(
    const vd::string& prefix, vd::i32 index, vd::i32 padding)
{
    vd::string result = prefix;
    return result;
}
    
vd::status 
Dataset::Close()
{
    if(m_IsOpen)
    {
        m_WorkQueue.Finish();
        m_WorkCache.Destroy();
    }
    m_IsOpen = false;
    return Status::Code::Success;
}

bool
Dataset::IsReady(vd::i32 index)
{
    return IsResident(index);
}

bool
Dataset::IsPending(vd::i32 index)
{
    return m_WorkCache.IsResident(index);
}

bool
Dataset::IsResident(vd::i32 index)
{
    return m_DataCache.IsResident(index);
}

WorkItem*
Dataset::GetPendingWorkItem(vd::i32 index)
{
    WorkItem* work = NULL;
    return work;
}

Data*
Dataset::Retrieve(
    const vd::i32& index)
{
    Data* data = NULL;
    bool hit = m_DataCache.Fetch(index, data);
    hit = hit && data && data->IsLoaded();
    vdLogInfo("Fetching datafile: Index[%d] Hit[%s] ...", index, hit ? "true" : "false");
    return hit ? data : NULL;
}
  
bool
Dataset::Request(
    vd::i32 index)
{
    bool hit = m_DataCache.Load(index);
    vdLogInfo("Requesting datafile: Index[%d] Hit[%s] ...", index, hit ? "true" : "false");
    return hit;
}

Data*
Dataset::OnFetch(
    const vd::i32& index)
{
    vdLogInfo("Miss on datafile '%d' ...", index);

#if 1
    Data* datafile = VD_NEW(Data, this);
    WorkItem* work = VD_NEW(WorkItem, 
        datafile, 
        m_FilePrefix, 
        index, 
        m_FileSplits, 
        m_FileNumberPadding, 
        m_RequestedBlocks, 
        m_RequestedTypes,
        m_RequestedStatistics); 

    work->OnRun();
    VD_DELETE(work);

    return datafile;
#else
    Data* datafile = NULL; 
    WorkItem* work = NULL;
    Thread::Yield();
    bool hit = m_WorkCache.Fetch(index, work);
    Thread::Yield();
    if(hit && work && work->IsReady())
    {
        vdLogInfo("OnFetch datafile: Index [%d] Ptr[%p] ...", 
            index, work);

        datafile = work->Get();
    }

    return datafile;
#endif
}

void 
Dataset::OnEvict(
    Data* datafile)
{
    if(datafile != NULL)
        vdLogInfo("Evicting datafile '%d' ...", datafile->GetFileIndex());
    VD_SAFE_DELETE(datafile);
}

WorkItem*
Dataset::OnSubmit(
    const vd::i32& index)
{
    WorkItem* work = NULL;
    if(!IsPending(index))
    {
        vdLogInfo("Adding load request for datafile '%d' ...", index);
        Formats::HiDist::Data* datafile = VD_NEW(Formats::HiDist::Data, this);
        Formats::HiDist::WorkItem* work = VD_NEW(Formats::HiDist::WorkItem, 
            datafile, 
            m_FilePrefix, 
            index, 
            m_FileSplits, 
            m_FileNumberPadding, 
            m_RequestedBlocks, 
            m_RequestedTypes,
            m_RequestedStatistics);                                                           

        m_DataCache.Insert(index, datafile);
        m_WorkQueue.Submit(work);
    }
    else
    {
        vdLogInfo("Rejecting load request -- pending request exists for datafile '%d'", index);
    }
    return work;
}

void 
Dataset::OnComplete(
    WorkItem* work)
{
    if(work != NULL)
        vdLogInfo(" '%d' loaded!", work->GetFileIndex());
    VD_SAFE_DELETE(work);
}

void
Dataset::Release(vd::i32 index)
{
    m_WorkCache.Remove(index);
    m_DataCache.Remove(index);
}

void 
Dataset::Retain(vd::i32 index, Data* datafile)
{
    m_DataCache.Touch(index);
}

void
Dataset::Evict()
{

}

void
WorkItem::OnRun()
{
    WorkItem* work = this;
    vd::f64 t0 = Process::GetTimeInSeconds();
    Data* datafile = work->m_Data;

    vdLogInfo("[%d] %s Index[%d] Splits[%d] Padding[%d]", 
        (int)work->GetSlotId(), work->m_FilePrefix.c_str(), 
        work->m_FileIndex, work->m_FileSplits, work->m_FileNumberPadding);
        
    if(work->IsAborted())
    {
//      VD_DELETE(work);
        return;
    }   
    
    datafile->Load(work->m_FilePrefix, work->m_FileIndex, 
                   work->m_FileSplits, work->m_FileNumberPadding, 
                   work->m_Blocks, work->m_Types);

    vd::string filename = datafile->GetFilename(work->m_FilePrefix, 
                                                work->m_FileIndex, 
                                                work->m_FileNumberPadding);

//    MetaData m = datafile->GetMetaData();
    vd::f64 t1 = Process::GetTimeInSeconds();
    vdLogInfo("Loaded '%s' in '%f' sec.", filename.c_str(), t1 - t0);

    for(vd::i32 i = 0; i < (vd::i32)Data::Block::Count; i++)
    {
        Data::Block::Value block = Data::Block::FromInteger(i);
        if(work->m_Stats[i] < 1)
            continue;

        vd::f64 t0 = Process::GetTimeInSeconds();
        Statistic& s = datafile->GetStatistic(block);
        s.Components = datafile->GetBlockVectorLength(block);
        if(datafile->IsBlockEntryIntegerValue(block))
        {
            int* src = (int*)datafile->GetBlockDataPtr(block);
            if(src == NULL)
                continue;

            for(vd::i8 vc = 0; vc < s.Components; vc++)
            {
                s.Minimum[vc] = +VD_F32_MAX;
                s.Maximum[vc] = -VD_F32_MAX;
            }

            float* dst = (float*)datafile->GetBlockDataPtr(block);
            for(vd::u64 n = 0; n < datafile->GetFilteredGalaxyCount(); n++)
            {
                for(vd::i8 vc = 0; vc < s.Components; vc++)
                {
                    float value = src[n * s.Components + vc];
                    dst[n * s.Components + vc] = float(value);

                    s.TotalSum[vc] += value;
                    s.SumSqr[vc] += (value * value);
                    s.Count[vc] += 1.0f;

                    if (value < s.Minimum[vc])
                      s.Minimum[vc] = value;

                    if (value > s.Maximum[vc])
                      s.Maximum[vc] = value;
                }
            }
    
            for(vd::i8 vc = 0; vc < s.Components; vc++)
            {
                float inverse = 1.0f / s.Count[vc];
                float first = s.SumSqr[vc] * inverse;
                float second = s.TotalSum[vc] * inverse;
                s.Variance[vc] = first - second * second;
            }
        }
        else
        {
            float* src = (float*)datafile->GetBlockDataPtr(block);
            if(src == NULL)
                continue;

            for(vd::i8 vc = 0; vc < s.Components; vc++)
            {
                s.Minimum[vc] = +VD_F32_MAX;
                s.Maximum[vc] = -VD_F32_MAX;
            }

            for(vd::u64 n = 0; n < datafile->GetFilteredGalaxyCount(); n++)
            {
                for(vd::i8 vc = 0; vc < s.Components; vc++)
                {
                    float value = src[n * s.Components + vc];

                    s.TotalSum[vc] += value;
                    s.SumSqr[vc] += (value * value);
                    s.Count[vc] += 1.0f;

                    if (value < s.Minimum[vc])
                      s.Minimum[vc] = value;

                    if (value > s.Maximum[vc])
                      s.Maximum[vc] = value;
                }
            }
    
            for(vd::i8 vc = 0; vc < s.Components; vc++)
            {
                float inverse = 1.0f / s.Count[vc];
                float first = s.SumSqr[vc] * inverse;
                float second = s.TotalSum[vc] * inverse;
                s.Variance[vc] = first - second * second;
            }
        }

        vd::f64 t1 = Process::GetTimeInSeconds();
        vdLogInfo("Computed stats for '%s' in '%f' sec:",
            Data::Block::ToString(i), t1 - t0);

        for(vd::i8 vc = 0; vc < s.Components; vc++)
        {
            vdLogInfo("-- Min[%f] Max[%f] Sum[%f] Var[%f]", 
                s.Minimum[vc], s.Maximum[vc], s.TotalSum[vc], s.Variance[vc]);
        }
    }
//  VD_DELETE(work);
}

void
WorkQueue::OnRun(
    Core::WorkItem* item)
{
    Formats::HiDist::WorkItem* work = (Formats::HiDist::WorkItem*)item;
    if(work == NULL)
        return;

    work->OnRun();
}

// ============================================================================================== //

VD_IMPLEMENT_OBJECT(Data, vd_sym(HiDistData), vd_sym(Object));
VD_IMPLEMENT_OBJECT(Dataset, vd_sym(HiDistDataset), vd_sym(Object));
VD_IMPLEMENT_OBJECT(WorkQueue, vd_sym(HiDistWorkQueue), vd_sym(WorkQueue));
VD_IMPLEMENT_OBJECT(WorkItem, vd_sym(HiDistWorkItem), vd_sym(WorkItem));

// ============================================================================================== //

} // end namespace: HiDist

// ============================================================================================== //

VD_FORMATS_NAMESPACE_END();

// ============================================================================================== //


