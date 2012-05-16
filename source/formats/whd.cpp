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

float* EquatorialToCelestialCartesian(
    float* ra, float* dec, float* distance,
    size_t count)
{
    float* pos = VD_NEW_ARRAY(float, count*3);

    for(size_t i = 0; i < count; i++)
    {
        float rav = ra[i] + VD_F32_TINY_VAL;
        float decv = dec[i] + VD_F32_TINY_VAL;
        float distv = (distance[i] + VD_F32_TINY_VAL) * 1000.0f;

        float vx = distv * Core::Cos(ToRadians(rav)) * Core::Cos(ToRadians(decv));
        float vy = distv * Core::Sin(ToRadians(rav)) * Core::Cos(ToRadians(decv));
        float vz = distv * Core::Sin(ToRadians(decv));

        pos[i * 3 + 0] = vx;
        pos[i * 3 + 1] = vy;
        pos[i * 3 + 2] = vz;
    }

    return pos;
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
            v[i] = -16.201031f;
            bad_sfr++;
            bad = true;
        }

        if(bad == false &&
           Core::IsWithinUlps(mstar[i], Constants::Zero, 2) || 
           Core::IsWithinUlps(mstar[i], Constants::Infinity, 2) ||
           Core::IsWithinUlps(mstar[i], Constants::NegInf, 2) ||
           Core::IsWithinUlps(mstar[i], Constants::NaN, 2))
        {
            v[i] = 0.0f;
            bad_mstar++;
            bad = true;
        }

        if(bad)
        {
            bad_ssfr++;
        }

        if(bad == false)
            v[i] = Core::Log10(sfr[i] / mstar[i]);
    }

    printf("BadSFR[%d => %f percent] BadMStar[%d => %f percent] BadSSFR[%d => %f percent]\n",
        (int)bad_sfr, bad_sfr/(float)count * 100.0, 
        (int)bad_mstar, bad_mstar/(float)count * 100.0,
        (int)bad_ssfr, bad_ssfr/(float)count * 100.0);

    printf("Blue[%d] Green[%d] Red[%d] DarkRed[%d]\n",
        (int)blue, (int) green, (int)red, (int)dark_red);

    return v;
}
class HiDistInputThread : public Thread
{

};

HiDistData::HiDistData(
    HiDistDataset* dataset) :
    Core::Object(),
    m_DataSet(dataset),
    m_TotalGalaxyCount(0),
    m_GasGalaxyCount(0),
    m_FileIndex(0)
{
    Memory::SetBytes(&m_MetaData,         0, sizeof(m_MetaData));
    Memory::SetBytes(&m_GalaxyData,       0, sizeof(m_GalaxyData));
    Memory::SetBytes(&m_StatisticsData,   0, sizeof(m_StatisticsData));
}
 
HiDistData::~HiDistData()
{
    Destroy();
}

vd::bytesize
HiDistData::GetResidentMemorySize() const
{
    vd::bytesize bytes = 0;
    for(vd::u32 i = 0; i < Block::Count; i++)
    {
        HiDistData::Block::Value block = HiDistData::Block::FromInteger(i);
        const char* ptr = (char*)GetBlockDataPtr(block);
        if(ptr != NULL)
        {
            bytes += GetBlockEntrySize(block) * m_TotalGalaxyCount;
        }
    }
    return bytes;
}

vd::status 
HiDistData::Destroy()
{
    vdLogInfo("Destroying HiDist '%d' ... ", m_FileIndex);
    
    for(vd::u32 i = 0; i < Block::Count; i++)
    {
        HiDistData::Block::Value block = HiDistData::Block::FromInteger(i);
        DestroyBlockData(block);
    }

    return Status::Code::Success;
}

vd::bytesize
HiDistData::GetBlockSize(
    Block::Value block) const
{
    return GetBlockEntrySize(block) * m_TotalGalaxyCount;
}

vd::bytesize
HiDistData::CreateBlockData(
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
HiDistData::DestroyBlockData(
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
HiDistData::GetBlockDataPtr(
    Block::Value block) const
{       
    if(Block::IsValid(block))
        return m_GalaxyData[Block::ToInteger(block)];

    return NULL;
}

void
HiDistData::SetBlockDataPtr(
    Block::Value block, void* ptr) 
{
    if(Block::IsValid(block))
        m_GalaxyData[Block::ToInteger(block)] = (float*)ptr;
}

vd::bytesize
HiDistData::GetBlockSeparatorSize(void) const
{
    return 0;
}

vd::i32
HiDistData::SkipToNextBlock(
    FILE* fh)
{
    return 0;
}

vd::bytesize
HiDistData::FindBlockByName(
    FILE* fd, const char* label)
{
    return 0;
}

vd::bytesize
HiDistData::FindBlock(
    FILE* fd, Block::Value block)
{
    return FindBlockByName(fd, GetBlockLabel(block));
}

const char*
HiDistData::GetBlockLabel(
    Block::Value block)  const
{
    return Block::ToString(block);
}

const char*
HiDistData::GetBlockName(
    Block::Value block)  const
{
    return Block::ToString(block);
}

vd::bytesize
HiDistData::GetBlockEntrySize(
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
HiDistData::GetBlockVectorLength(
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
HiDistData::GetBlockSizeTypeSuffix(
    Block::Value block) const
{
    if(Block::IsValid(block))
        return "1f";
    return "";
}

bool
HiDistData::IsBlockEntryIntegerValue(
    Block::Value block) const
{
    return false;
}

vd::f32
HiDistData::Periodic(
    vd::f32 x, vd::f32 l2)
{
    if(x > +l2)
        x -= l2 * 2.0f;
    if(x < -l2)
        x += l2 * 2.0f;
    return x;
}

void
HiDistData::SwapFloatDataAt(
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
HiDistData::SwapIntDataAt(
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
HiDistData::SwapFloat3DataAt(
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
HiDistData::SwapGalaxyAt(
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
HiDistData::ReadHeader(
    const char* filename, vd::i32 splits)
{
    struct stat statbuf;
    stat(filename, &statbuf);
    size_t bytes = statbuf.st_size;

    if(splits <= 1)
    {
        m_TotalGalaxyCount = 0;
        m_GasGalaxyCount = 0;
    }

    m_TotalGalaxyCount = GetLineCount(filename);
    m_MetaData.GalaxyCount[0] = m_TotalGalaxyCount;
    m_MetaData.BoxSize = 1000.0f;
    m_GasGalaxyCount = 0;
    vdLogInfo("DataSet size: %llu (%d galaxies)", bytes, m_TotalGalaxyCount);
    return 1;
}

vd::string
HiDistData::GetFilename(
    const vd::string& prefix, 
    vd::i32 index,
    vd::i32 padding)
{
    return HiDistDataset::GetFilenameForHiDist(prefix, index, padding);
}

vd::status
HiDistData::Load(
    const vd::string& prefix, 
    vd::i32 index,
    vd::i32 splits,
    vd::i32 padding,
    vd::i32* req_data,
    vd::i32* req_types)
{
    vdAssert(prefix.size() > 1);    

    vd::i32 pc = 1;
    vd::i32 total_pc = pc;

    int ssrs = HiDistData::Block::ToInteger(HiDistData::Block::SSRS);
    int pos = HiDistData::Block::ToInteger(HiDistData::Block::Position);
    if(req_data[pos] > 0)
    {
        int ra = HiDistData::Block::ToInteger(HiDistData::Block::RA);
        int dec = HiDistData::Block::ToInteger(HiDistData::Block::DEC);
        int rs = HiDistData::Block::ToInteger(HiDistData::Block::Redshift);
//        int rs = HiDistData::Block::ToInteger(HiDistData::Block::ObservedRedshift);

        req_data[ra] = 1;
        req_data[dec] = 1;
        req_data[rs] = 1;
    }

    if(req_data[pos] > 0)
    {
        int sm = HiDistData::Block::ToInteger(HiDistData::Block::StellarMass);
        int sfr = HiDistData::Block::ToInteger(HiDistData::Block::SFR);

        req_data[sm] = 1;
        req_data[sfr] = 1;
    }

    m_IsLoaded.Set(0);
    m_FilteredGalaxyCount = 0;
    splits = splits < 1 ? 1 : splits;

    vd::string filename = HiDistDataset::GetFilenameForHiDist(prefix, index, padding);
    for(vd::i32 i = 0; i < splits; i++)
    {
//        if(splits > 1)
//            filename += vd::string(".") + Core::Convert::ToString(i, 0, 0);

        if(!ReadHeader(filename.c_str(), splits))
        {
            vdLogWarning("Failed to open HiDist datafile file '%s'!", filename.c_str());
            return Status::Code::ReadError;
        }
    }

    for(vd::i32 i = 0; i < splits; i++)
    {
        if(splits > 1)
            filename += vd::string(".") + Core::Convert::ToString(i, 0, 0);

        size_t field_count;
        FILE* fd = OpenCsvFile(filename.c_str(), &field_count);        
        if(fd == NULL)
        {
            vdLogWarning("Failed to open HiDist datafile file '%s'!", filename.c_str());
            return Status::Code::ReadError;
        }

        vdLogDebug("Reading datafile '%s' ...", filename.c_str());

        for(vd::u32 n = 0; n < Block::Count; n++)
        {
            HiDistData::Block::Value block = HiDistData::Block::FromInteger(n);
            if(block == HiDistData::Block::Position)
                continue;

            if(block == HiDistData::Block::SSRS)
                continue;

            if(req_data[n] > 0)
            {
                CreateBlockData(block);
            }
        }

        if(i == 0)  
            m_TotalGalaxyCount = 0;

        bool use_bin = false;
        FILE* out_file = NULL;
        std::string out_filename = filename + ".bin";

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

        size_t index = 0;
        size_t bytes = 1;
        do {
            for(vd::u32 n = 0; n < Block::Count && bytes > 0; n++)
            {
                HiDistData::Block::Value block = HiDistData::Block::FromInteger(n);
                if(block == HiDistData::Block::Position)
                    continue;

                if(block == HiDistData::Block::SSRS)
                    continue;

                float v;
                if(use_bin)
                {
                    bytes = fread(&v, sizeof(float), 1, out_file);
                }
                else
                {
                    float v = ReadCsvFloatColumn(fd, &bytes);
                    fwrite(&v, sizeof(float), 1, out_file);
                }

                if(req_data[n] > 0)
                {
                    float* ptr = (float*)GetBlockDataPtr(block);
                    if(ptr) ptr[index] = v;
                    // printf("Read[%d] '%s' : %f\n", (int)index,  HiDistData::Block::ToString(block), v);
                }
            }        
            index++;
        } while(bytes > 0);

        m_TotalGalaxyCount += index;
        CloseCsvFile(fd);
        fclose(out_file);
        total_pc += pc;
    }

    m_FilteredGalaxyCount = m_TotalGalaxyCount;
    vdLogInfo("Loaded '%d'/'%d' galaxies!", m_FilteredGalaxyCount, m_TotalGalaxyCount);

    float* xyz = NULL;
    pos = HiDistData::Block::ToInteger(HiDistData::Block::Position);
    if(req_data[pos] >= 1)
    {
        float* ra = (float*)GetBlockDataPtr(HiDistData::Block::RA);
        float* dec = (float*)GetBlockDataPtr(HiDistData::Block::DEC);
        float* redshift = (float*)GetBlockDataPtr(HiDistData::Block::Redshift);
//        float* redshift = (float*)GetBlockDataPtr(HiDistData::Block::ObservedRedshift);

        // printf("Computing EquatorialToCelestialCartesian for '%d' galaxies!\n", m_TotalGalaxyCount);
        if(ra != NULL && dec != NULL && redshift != NULL)
        {
            xyz = EquatorialToCelestialCartesian(ra, dec, redshift, m_TotalGalaxyCount);
            SetBlockDataPtr(HiDistData::Block::Position, xyz);
        }
    }
 
    ssrs = HiDistData::Block::ToInteger(HiDistData::Block::SSRS);
    if(req_data[ssrs] >= 1)
    {
        float* sfr = (float*)GetBlockDataPtr(HiDistData::Block::SFR);
        float* sm = (float*)GetBlockDataPtr(HiDistData::Block::StellarMass);

        // printf("Computing SSFR for '%d' galaxies!\n", m_TotalGalaxyCount);
        if(sfr != NULL && sm != NULL)
        {
            float* ssfr = SpecificStarFormationRate(xyz, sfr, sm, m_TotalGalaxyCount);
            SetBlockDataPtr(HiDistData::Block::SSRS, ssfr);
        }
    }

    FILE* csv_file = NULL;
    std::string csv_filename = filename + ".csv";
    csv_file = fopen(csv_filename.c_str(), "w");
    printf("Exporting '%s' ...\n", csv_filename.c_str());
    for(vd::u32 n = 0; n < Block::Count && m_TotalGalaxyCount; n++)
    {
        HiDistData::Block::Value block = HiDistData::Block::FromInteger(n);
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
                fprintf(csv_file, "%s%s%s",  HiDistData::Block::ToString(block), suffix[v], vi ? ", " : (inner ? ", " : "\n"));
                printf("%s%s%s",  HiDistData::Block::ToString(block), suffix[v], vi ? ", " : (inner ? ", " : "\n"));
            }
        }
        else
        {
            fprintf(csv_file, "%s%s",  HiDistData::Block::ToString(block), inner ? ", " : "\n");
            printf("%s%s",  HiDistData::Block::ToString(block), inner ? ", " : "\n");
        }
    }        
    for(vd::u32 g = 0; g < m_TotalGalaxyCount; g++)
    {
        for(vd::u32 n = 0; n < Block::Count; n++)
        {
            HiDistData::Block::Value block = HiDistData::Block::FromInteger(n);
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

    // Compute derived quantity for Temperature from Internal Energy if it wasn't in the file
//    if(req_data[HiDistData::Block::Temperature] == VD_TRUE && 
//        GetBlockDataPtr(HiDistData::Block::Temperature) == NULL)
//    {
//        ComputeTempFromInternalEnergy();
//    }



    m_FileIndex = index;
    m_IsLoaded.Increment();
    return 1;
}

const HiDistMetaData& 
HiDistData::GetMetaData() const 
{ 
    return m_MetaData; 
}

HiDistScalarStatistic& 
HiDistData::GetScalarStatistic(Block::Value v) 
{ 
    return m_StatisticsData[Block::ToInteger(v)]; 
}

void 
HiDistData::SetScalarStatistic(Block::Value v, const HiDistScalarStatistic& stats) 
{
    m_StatisticsData[Block::ToInteger(v)] = stats; 
}

vd::u64
HiDistData::GetTotalGalaxyCount() const 
{ 
    return m_TotalGalaxyCount; 
}

vd::u64
HiDistData::GetFilteredGalaxyCount() const 
{ 
    return m_FilteredGalaxyCount; 
}

vd::u64
HiDistData::GetGalaxyCountForType(GalaxyType::Value v) const 
{ 
    return m_MetaData.GalaxyCount[GalaxyType::ToInteger(v)]; 
}

vd::f32
HiDistData::GetBoundaryBoxSize() const 
{ 
    return m_MetaData.BoxSize; 
}
    
vd::u32
HiDistData::GetFileIndex() const 
{ 
    return m_FileIndex; 
}
    
bool
HiDistData::IsLoaded() 
{ 
    return m_IsLoaded.Get() > 0 ? true : false; 
}
    
void 
HiDistData::Reorder()
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

HiDistDataset::HiDistDataset(
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

}

HiDistDataset::~HiDistDataset() 
{ 
    Destroy();
}

vd::status 
HiDistDataset::Destroy(void)
{
    vd::status result = Close();
    m_DataCache.Destroy();
    return result;
}

void 
HiDistDataset::SetBlockRequest(HiDistData::Block::Value v, vd::i32 enable) 
    { m_RequestedBlocks[HiDistData::Block::ToInteger(v)] = enable; }

void 
HiDistDataset::SetTypeRequest(HiDistData::GalaxyType::Value v, vd::i32 enable) 
    { m_RequestedTypes[HiDistData::GalaxyType::ToInteger(v)] = enable; }

void 
HiDistDataset::SetStatisticRequest(HiDistData::Block::Value v, vd::i32 enable) 
    { m_RequestedStatistics[HiDistData::Block::ToInteger(v)] = enable; }

vd::status 
HiDistDataset::Open(
    const vd::string& prefix, 
    vd::i32 count, 
    vd::i32 splits)
{
    m_Mutex.Lock();
    for(vd::u32 n = 0; n < HiDistData::Block::Count; n++)
        m_RequestedBlocks[n] = 0;

    m_FileNumberPadding = 0;
    vd::i32 length = (vd::i32)prefix.size();
    for(vd::i32 i = length-1; i >= 0 && isdigit(prefix[i]); --i)
        m_FileNumberPadding++;
        
    vd::string digits = prefix.substr(length - m_FileNumberPadding, length);
    m_FilePrefix = prefix.substr(0, length - m_FileNumberPadding);
    m_StartFileIndex = atoi(digits.c_str());
    if(count < 1)
    {
        int missed = 0;
        Core::FileSystem* fs = m_Runtime->GetFileSystem();
        int i = m_StartFileIndex;
        while(missed < 10)
        {
            if(fs->Exists(GetFilenameForHiDist(m_FilePrefix, i, m_FileNumberPadding)) == true)
            {
                m_EndFileIndex = i;
                missed = 0;
            }
            else
            {
                missed++;
            }
            i++;
        }
    }
    else
    {
        m_EndFileIndex = m_StartFileIndex + count;
    }

    m_FileSplits = splits;
    m_IsOpen = true;
    
    vdLogInfo("Opening dataset '%s' from '%d-%d' with padded index '%d' ...",
        m_FilePrefix.c_str(), m_StartFileIndex, m_EndFileIndex, m_FileNumberPadding);
    
    m_DataCache.Setup(2 * Constants::MemorySize::GB, 
        VD_BIND_MEMBER_FUNCTION(this, &HiDistDataset::OnFetch), 
        VD_BIND_MEMBER_FUNCTION(this, &HiDistDataset::OnEvict)
    );

    m_WorkCache.Setup(m_EndFileIndex - m_StartFileIndex + 1, 
        VD_BIND_MEMBER_FUNCTION(this, &HiDistDataset::OnSubmit), 
        VD_BIND_MEMBER_FUNCTION(this, &HiDistDataset::OnComplete)
    );
    m_WorkQueue.Start(1);   
    m_CurrentFileIndex = m_StartFileIndex;
    m_Mutex.Unlock();

    return Status::Code::Success;
}
    
vd::string
HiDistDataset::GetFilenameForHiDist(
    const vd::string& prefix, vd::i32 index, vd::i32 padding)
{
    vd::string result = prefix;
    result += ".dat";
/*
    char digits[1024] = {0};
    snprintf(digits, sizeof(digits), "%d", index);
    vd::i32 digit_len = (vd::i32) std::strlen(digits);
    
    for(vd::i32 p = 0; p < padding - digit_len; p++)
        result += vd::string("0");
    
    result += vd::string(digits);
*/
    return result;
}
    
vd::status 
HiDistDataset::Close()
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
HiDistDataset::IsReady(vd::i32 index)
{
    return IsResident(index);
}

bool
HiDistDataset::IsPending(vd::i32 index)
{
    return m_WorkCache.IsResident(index);
}

bool
HiDistDataset::IsResident(vd::i32 index)
{
    return m_DataCache.IsResident(index);
}

HiDistWorkItem*
HiDistDataset::GetPendingWorkItem(vd::i32 index)
{
    HiDistWorkItem* work = NULL;
    return work;
}

HiDistData*
HiDistDataset::Retrieve(
    const vd::i32& index)
{
    HiDistData* data = NULL;
    bool hit = m_DataCache.Fetch(index, data);
    hit = hit && data && data->IsLoaded();
//  vdLogInfo("Fetching datafile: Index[%d] Hit[%s] ...", index, hit ? "true" : "false");
    return hit ? data : NULL;
}
  
bool
HiDistDataset::Request(
    vd::i32 index)
{
    bool hit = m_DataCache.Load(index);
    vdLogInfo("Requesting datafile: Index[%d] Hit[%s] ...", index, hit ? "true" : "false");
    return hit;
}

HiDistData*
HiDistDataset::OnFetch(
    const vd::i32& index)
{
    vdLogInfo("Miss on datafile '%d' ...", index);

/*
    HiDistData* datafile = VD_NEW(HiDistData);
    datafile->Load(m_FilePrefix, 
        index, 
        m_FileSplits, 
        m_FileNumberPadding, 
        m_RequestedBlocks, 
        m_RequestedTypes);
*/

    HiDistData* datafile = NULL; 
    HiDistWorkItem* work = NULL;

    if(m_WorkCache.IsResident(index))
        return NULL;

    bool hit = m_WorkCache.Fetch(index, work);
    if(hit && work && work->IsReady())
    {
        vdLogInfo("OnFetch datafile: Index [%d] Ptr[%p] ...", 
            index, work);

        datafile = work->GetHiDist();
    }

    return datafile;
}

void 
HiDistDataset::OnEvict(
    HiDistData* datafile)
{
    if(datafile != NULL)
        vdLogInfo("Evicting datafile '%d' ...", datafile->GetFileIndex());
    VD_SAFE_DELETE(datafile);
}

HiDistWorkItem*
HiDistDataset::OnSubmit(
    const vd::i32& index)
{
    vdLogInfo("Adding load request for datafile '%d' ...", index);
    HiDistWorkItem* work = NULL;
    if(!IsPending(index))
    {
        HiDistData* datafile = VD_NEW(HiDistData, this);
        HiDistWorkItem* work = VD_NEW(HiDistWorkItem, 
            datafile, 
            m_FilePrefix, 
            index, 
            m_FileSplits, 
            m_FileNumberPadding, 
            m_RequestedBlocks, 
            m_RequestedTypes,
            m_RequestedStatistics);                                                           

        m_WorkQueue.Submit(work);
        m_DataCache.Insert(index, datafile);
    }
    return work;
}

void 
HiDistDataset::OnComplete(
    HiDistWorkItem* work)
{
    if(work != NULL)
        vdLogInfo("HiDist '%d' loaded!", work->GetFileIndex());
    VD_SAFE_DELETE(work);
}

void
HiDistDataset::Release(vd::i32 index)
{
    m_WorkCache.Remove(index);
    m_DataCache.Remove(index);
}

void 
HiDistDataset::Retain(vd::i32 index, HiDistData* datafile)
{
    m_DataCache.Touch(index);
}

void
HiDistDataset::Evict()
{

}

void
HiDistWorkQueue::OnRun(
    WorkItem* item)
{
    HiDistWorkItem* work = (HiDistWorkItem*)item;
    if(work == NULL)
        return;

    vd::f64 t0 = Process::GetTimeInSeconds();
    HiDistData* datafile = work->m_Data;

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

    HiDistMetaData m = datafile->GetMetaData();
    vd::f64 t1 = Process::GetTimeInSeconds();
    vdLogInfo("Loaded '%s' in '%f' sec.", filename.c_str(), t1 - t0);

    for(vd::u32 i = 0; i < HiDistData::Block::Count; i++)
    {
        HiDistData::Block::Value block = HiDistData::Block::FromInteger(i);
        if(work->m_Stats[i] < 1)
            continue;

        if(datafile->GetBlockVectorLength(block) > 1)
            continue;

        vd::f64 t0 = Process::GetTimeInSeconds();
        HiDistScalarStatistic& s = datafile->GetScalarStatistic(block);
        if(datafile->IsBlockEntryIntegerValue(block))
        {
            s.Minimum = +VD_F32_MAX;
            s.Maximum = -VD_F32_MAX;

            int* src = (int*)datafile->GetBlockDataPtr(block);
            if(src == NULL)
                continue;

            float* dst = (float*)datafile->GetBlockDataPtr(block);
            for(vd::u64 n = 0; n < datafile->GetFilteredGalaxyCount(); n++)
            {
                float value = src[n];
                dst[n] = float(value);

                s.TotalSum += value;
                s.SumSqr += (value * value);
                s.Count += 1.0f;

                if (value < s.Minimum)
                  s.Minimum = value;

                if (value > s.Maximum)
                  s.Maximum = value;
            }
            float inverse = 1.0f / s.Count;
            float first = s.SumSqr * inverse;
            float second = s.TotalSum * inverse;
            s.Variance = first - second * second;
        }
        else
        {
            s.Minimum = +VD_F32_MAX;
            s.Maximum = -VD_F32_MAX;

            float* src = (float*)datafile->GetBlockDataPtr(block);
            if(src == NULL)
                continue;

            for(vd::u64 n = 0; n < datafile->GetFilteredGalaxyCount(); n++)
            {
                float value = src[n];

                s.TotalSum += value;
                s.SumSqr += (value * value);
                s.Count += 1.0f;

                if (value < s.Minimum)
                  s.Minimum = value;

                if (value > s.Maximum)
                  s.Maximum = value;
            }

            float inverse = 1.0f / s.Count;
            float first = s.SumSqr * inverse;
            float second = s.TotalSum * inverse;
            s.Variance = first - second * second;
        }

        vd::f64 t1 = Process::GetTimeInSeconds();
        vdLogInfo("Computed stats for '%s' in '%f' sec:",
            HiDistData::Block::ToString(i), t1 - t0);

        vdLogInfo("-- Min[%f] Max[%f] Mean[%f] Sum[%f] Var[%f]", 
            s.Minimum, s.Maximum, s.TotalSum / s.Count, s.TotalSum, s.Variance);
    }
//  VD_DELETE(work);
}

// ============================================================================================== //

VD_IMPLEMENT_OBJECT(HiDistData, vd_sym(HiDistData), vd_sym(Object));
VD_IMPLEMENT_OBJECT(HiDistDataset, vd_sym(HiDistDataset), vd_sym(Object));
VD_IMPLEMENT_OBJECT(HiDistWorkQueue, vd_sym(HiDistWorkQueue), vd_sym(WorkQueue));
VD_IMPLEMENT_OBJECT(HiDistWorkItem, vd_sym(HiDistWorkItem), vd_sym(WorkItem));

// ============================================================================================== //

VD_FORMATS_NAMESPACE_END();

// ============================================================================================== //


