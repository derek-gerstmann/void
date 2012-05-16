// ============================================================================================== //
//
// License:     The Lesser GNU Public License (LGPL) v3.0.
// 
// Author(s):   Derek Gerstmann. The University of Western Australia (UWA). 
//              As well as the shoulders of many giants...
//
// This file is part of the Void framework.
//
// The Void framework is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// The Void framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with the Void framework.  If not, see <http://www.gnu.org/licenses/>.
//
// ============================================================================================== //

#ifndef VD_FORMATS_WHD_HEADER_INCLUDED
#define VD_FORMATS_WHD_HEADER_INCLUDED

// ============================================================================================== //

#include "formats/formats.h"
#include "core/core.h"
#include "core/object.h"
#include "core/memory.h"
#include "core/atomics.h"
#include "core/workqueue.h"
#include "runtime/runtime.h"
#include "runtime/context.h"
#include "containers/cache.h"

// ============================================================================================== //

VD_FORMATS_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_IMPORT(Core, Mutex);
VD_IMPORT(Core, WorkItem);
VD_IMPORT(Core, WorkQueue);
VD_IMPORT(Core, AtomicCounter);
VD_IMPORT(Containers, LruCache);

// ============================================================================================== //

struct HiDistMetaData
{
    int GalaxyCount[6];                  
    double Mass[6];               
    double Time;                            
    double Redshift;                        
    int HasStarFormation;                   
    int HasFeedback;                    
    int GalaxyCountTotal[6];    
    int HasCooling;                     
    int FileCountPerSnapshot;              
    double BoxSize;                         
    double Omega0;                          
    double OmegaLambda;                     
    double HubbleParam;                     
    int HasStellarAge;                      
    int HasMetals;                          
    unsigned int GalaxyCountTotalHighWord[6];  
    int  HasEntropyInsteadofU;              
    char Fill[56];                          //!< Fills to 256 Bytes 
};

// vdStaticAssert((sizeof(HiDistMetaData) == 256), InvalidGS2HeaderSize);

// ============================================================================================== //

typedef float* HiDistGalaxyData[16]; // 16 columns of 32-bit float data per HiDist dat file

struct HiDistScalarStatistic
{
    float Minimum;
    float Maximum;
    float Mean;
    float Variance;
    float TotalSum;
    float SumSqr;
    size_t Count;
};

struct HiDistRangeData
{
    void*  Base;
    void*  StartRange;
    void*  EndRange;
    size_t Offset;
    size_t TotalBytes;      
};

struct HiDistBlockRangeData
{
    HiDistRangeData Id;
    HiDistRangeData Type;
    HiDistRangeData Position;
    HiDistRangeData Velocity;
    HiDistRangeData Mass;
    HiDistRangeData Density;
    HiDistRangeData InternalEnergy;
    HiDistRangeData Temp;
    HiDistRangeData Ne;
    HiDistRangeData SmoothingLength;
    HiDistRangeData Potential;
    HiDistRangeData Acceleration;
    HiDistRangeData DtEntropy;
    HiDistRangeData TimeStep;
};

// ============================================================================================== //

class HiDistDataset;

class HiDistData : public Object
{
public:

    // Order *must* match block order in dat file (eg v2)
    typedef struct GalaxyType
    {
        enum Value 
        {
            All           = 0,
            LastValue     = 1
        };

        static const size_t Count = LastValue;

        static const char* ToString(vd::u32 value) 
        { 
            switch (value)
            {
            case All:       return "All";
            default:        return "<INVALID>";
            }
            return "<INVALID>";
        }

        static GalaxyType::Value FromInteger(
            vd::u32 value) 
        { 
            switch (value)
            {
            case 0: return All;
            default: return LastValue; 
            }
            return LastValue;
        }

        static const vd::u32 ToInteger(
            GalaxyType::Value value) 
        { 
            switch (value)
            {
            case All:       return 0;
            case LastValue: return Count;
            default:        return Count; 
            }
            return Count;
        }
        static bool IsValid(GalaxyType::Value v)
        {
            return ToInteger(v) < Count;
        }
    };


    // Order *must* match block order in dat file (for HiDist)
    typedef struct Block
    {

        // ## Total Mass [Msol]  
        // ## HI Mass [Msol] 
        // ## Stellar Mass [Msol] 
        // ## Gas Mass [Msol] 
        // ## Real z  
        // ## Observed z  
        // ## RA [deg]  
        // ## DEC [deg] 
        // ## Rest-frame LoS Vel Width [km/s] 
        // ## Angle of inclination [Radians] 
        // ## SFR [Msol/yr] 
        // ## HI Diameter [kpc] 
        // ## Angular extent [arcsec] 
        // ## Observed integrated flux [mJy km/s] 
        // ## Observed flux per beam/per channel [mJy km/s] 
        // ## Beams Resolved by

        enum Value 
        {
            VirialMass = 0,             // 0 - Virial mass of halo [Msol]
            HiMass = 1,                 // 1 - HI mass of halo [Msol]
            StellarMass = 2,            // 2 - Stellar mass of system [Msol]
            ColdGasMass = 3,            // 3 - Cold Gas mass of system [Msol]
            Redshift = 4,               // 4 - Redshift (distance from origin) (0.0 - 2.6?)
            ObservedRedshift = 5,       // 5 - Observed redshift
            RA = 6,                     // 6 - RA [degrees]
            DEC = 7,                    // 7 - DEC [degrees]
            LoS= 8,                     // 8 - LoS rotational width [km/s in the frame of galaxy]
            Inclination = 9,            // 9 - Angle of inclination [radians]
            SFR = 10,                   // 10 - SFR (star formation rate) [Msol/yr]
            HiDiameter = 11,            // 11 - HI diameter [kpc]
            AngleInSky = 12,            // 12 - Angle in the sky [arcseconds]
            ObservedFlux = 13,          // 13 - Observed integrated flux [mJy]
            ObservedFluxPerBeam = 14,   // 14 - Observed flux per beam/per channel [mJy km/s]
            BeamCount = 15,             // 15 - Number of beams
            Position = 16,              // 16 - Cartesian coordinates
            SSRS = 17,                  // 17 - Stellar Star formation rate
            LastValue = 18
        };

        static const size_t Count = LastValue;

        static const char* ToString(vd::u32 value) 
        { 
            switch (value)
            {
            case VirialMass:            return "VirialMass";
            case HiMass:                return "HiMass";
            case StellarMass:           return "StellarMass";
            case ColdGasMass:           return "ColdGasMass";
            case Redshift:              return "Redshift";
            case ObservedRedshift:      return "ObservedRedshift";
            case RA:                    return "RA"; 
            case DEC:                   return "DEC"; 
            case LoS:                   return "LoS"; 
            case Inclination:           return "Inclination"; 
            case SFR:                   return "SFR"; 
            case HiDiameter:            return "HiDiameter"; 
            case AngleInSky:            return "AngleInSky"; 
            case ObservedFlux:          return "ObservedFlux"; 
            case ObservedFluxPerBeam:   return "ObservedFluxPerBeam"; 
            case BeamCount:             return "BeamCount"; 
            case Position:              return "Position"; 
            case SSRS:                  return "SSRS"; 
            default:                    return "<INVALID>";
            }
            return "<INVALID>";
        }

        static Block::Value FromInteger(
            vd::u32 value) 
        { 
            switch (value)
            {
            case 0: return VirialMass;
            case 1: return HiMass;
            case 2: return StellarMass;
            case 3: return ColdGasMass;
            case 4: return Redshift;
            case 5: return ObservedRedshift;
            case 6: return RA;
            case 7: return DEC;
            case 8: return LoS;
            case 9: return Inclination;
            case 10: return SFR;
            case 11: return HiDiameter;
            case 12: return AngleInSky;
            case 13: return ObservedFlux;
            case 14: return ObservedFluxPerBeam;
            case 15: return BeamCount;
            case 16: return Position;
            case 17: return SSRS;
            default: return LastValue; 
            }
            return LastValue;
        }

        static const vd::u32 ToInteger(
            Block::Value value) 
        { 
            switch (value)
            {
            case VirialMass:            return 0;
            case HiMass:                return 1;
            case StellarMass:           return 2;
            case ColdGasMass:           return 3;
            case Redshift:              return 4;
            case ObservedRedshift:      return 5;
            case RA:                    return 6;
            case DEC:                   return 7;
            case LoS:                   return 8;
            case Inclination:           return 9;
            case SFR:                   return 10;
            case HiDiameter:            return 11;
            case AngleInSky:            return 12;
            case ObservedFlux:          return 13;
            case ObservedFluxPerBeam:   return 14;
            case BeamCount:             return 15;
            case Position:              return 16;
            case SSRS:                  return 17;
            default:                    return Count;
            }
            return Count;
        }

        static bool IsValid(Block::Value v)
        {
            return ToInteger(v) < Count;
        }
    };


    typedef vd::i32 BlockRequest[Block::Count];
    typedef vd::i32 TypeRequest[GalaxyType::Count];
    
public:
        
    HiDistData(HiDistDataset* dataset);    
    virtual ~HiDistData();

    vd::status
    Load(
        const vd::string& prefix, 
        vd::i32 index,
        vd::i32 splits=1,
        vd::i32 padding=3,
        vd::i32* req_data=0,
        vd::i32* req_types=0);

    vd::status 
    Destroy();
    
    vd::status 
    Close();
    
    vd::bytesize
    CreateBlockData(Block::Value block);
    
    void 
    DestroyBlockData(Block::Value block);
    
    void*   
    GetBlockDataPtr(Block::Value block) const;

    void 
    SetBlockDataPtr(Block::Value block, void* ptr);

    vd::bytesize 
    ReadHeader(const char* filename, vd::i32 splits);
    
    vd::bytesize
    ReadTypeData(FILE* fd, vd::bytesize offset);
    
    vd::bytesize
    GetBlockSeparatorSize(void) const;
    
    vd::i32
    SkipToNextBlock(FILE* fh);
    
    vd::bytesize
    FindBlockByName(FILE* fd, const char* label);
    
    vd::bytesize
    FindBlock(FILE* fd, Block::Value block);

    const char* 
    GetBlockLabel(Block::Value block) const;
    
    const char* 
    GetBlockName(Block::Value block) const;
    
    vd::bytesize
    GetBlockSize(Block::Value block) const;
    
    vd::bytesize 
    GetBlockEntrySize(Block::Value block) const;
    
    vd::i32 
    GetBlockVectorLength(Block::Value block) const;
    
    const char* 
    GetBlockSizeTypeSuffix(Block::Value block) const;
    
    bool 
    IsBlockEntryIntegerValue(Block::Value block) const;

    vd::bytesize 
    ReadColumnData(FILE* fd, Block::Value block, vd::bytesize offset, vd::i32* types);

    vd::f32
    Periodic(vd::f32 x, vd::f32 l2);
    
    void
    SwapFloatDataAt(float* ptr, int isrc, int idst);
    
    void
    SwapIntDataAt(int* ptr, int isrc, int idst);
    
    void
    SwapFloat3DataAt(float* ptr, int isrc, int idst);
    
    void
    SwapGalaxyAt(int isrc, int idst);
    
    void
    Reorder(void);
    
    const HiDistMetaData& GetMetaData() const;

    HiDistScalarStatistic& GetScalarStatistic(Block::Value v);

    void SetScalarStatistic(Block::Value v, const HiDistScalarStatistic& stats);

    vd::u64
    GetTotalGalaxyCount() const;

    vd::u64
    GetFilteredGalaxyCount() const;

    vd::u64
    GetGalaxyCountForType(GalaxyType::Value v) const;

    vd::f32
    GetBoundaryBoxSize() const;

    vd::u32
    GetFileIndex() const;

    vd::bytesize
    GetResidentMemorySize() const;
    
    bool
    IsLoaded();

    vd::string
    GetFilename(const vd::string& prefix, vd::i32 index, vd::i32 padding);
    
    VD_DECLARE_OBJECT(HiDistData);
    
private:

    VD_DISABLE_COPY_CONSTRUCTORS(HiDistData);

    HiDistDataset*         m_DataSet;
    HiDistMetaData         m_MetaData;
    HiDistGalaxyData       m_GalaxyData;
    HiDistScalarStatistic  m_StatisticsData[Block::Count];
    vd::u64                m_TotalGalaxyCount;
    vd::u64                m_FilteredGalaxyCount;
    vd::u64                m_GasGalaxyCount;
    vd::u32                m_FileIndex;
    AtomicCounter          m_IsLoaded;
};

// ============================================================================================== //

class HiDistDataset;

// ============================================================================================== //

class HiDistWorkItem : public WorkItem
{
    friend class HiDistWorkQueue;
    
public:

    explicit HiDistWorkItem(
        HiDistData* data,
        const vd::string& prefix,
        vd::i32 index,
        vd::i32 splits,
        vd::i32 padding,
        vd::i32* blocks,
        vd::i32* types,
        vd::i32* stats
    ) :
        WorkItem(),
        m_Data(data),
        m_FilePrefix(prefix),
        m_FileIndex(index),
        m_FileSplits(splits),
        m_FileNumberPadding(padding),
        m_Blocks(blocks),
        m_Types(types),
        m_Stats(stats)
    { 
        // EMPTY!
    }

    bool IsReady(void) { return m_IsReady.Get() > 0; }
    HiDistData* GetHiDist(void) { return m_Data; }
    const vd::string& GetFilePrefix(void) const { return m_FilePrefix; }
    vd::i32 GetFileIndex(void) const { return m_FileIndex; }
    vd::i32 GetFileNumberPadding(void) const { return m_FileNumberPadding; }
    vd::i32 GetFileSplits(void) const { return m_FileSplits; }

    VD_DECLARE_OBJECT(HiDistWorkItem);
    
private:

    VD_DISABLE_COPY_CONSTRUCTORS(HiDistWorkItem);
    
    HiDistData* m_Data; 
    vd::string m_FilePrefix;    
    vd::i32 m_FileIndex;
    vd::i32 m_FileSplits;
    vd::i32 m_FileNumberPadding;
    vd::i32* m_Blocks;
    vd::i32* m_Types;       
    vd::i32* m_Stats;       
    AtomicCounter m_IsReady;
};

// ============================================================================================== //

class HiDistWorkQueue : public WorkQueue
{
public:

    HiDistWorkQueue() : WorkQueue() {} 
    virtual ~HiDistWorkQueue() { Destroy(); } 

    virtual void OnRun(WorkItem* item);

    VD_DECLARE_OBJECT(HiDistWorkQueue);

private:

    VD_DISABLE_COPY_CONSTRUCTORS(HiDistWorkQueue);

};

// ============================================================================================== //

struct HiDistResidentSizeFn 
{
    unsigned long operator()( const HiDistData* x ) 
    {
        if(x != NULL)
            return x->GetResidentMemorySize(); 
        return 1; 
    }
};

class HiDistDataset : public Object
{
public:
    typedef LruCache< vd::i32, HiDistWorkItem* > WorkItemCache;
    typedef LruCache< vd::i32, HiDistData*, HiDistResidentSizeFn > DataCache;

    HiDistDataset(Runtime::Context* runtime);    
    virtual ~HiDistDataset();
    
    vd::status
    Destroy();
    
    vd::status 
    Open(const vd::string& prefix, vd::i32 count=0, vd::i32 splits=1);
    
    vd::status 
    Close();
    
    static vd::string
    GetFilenameForHiDist(const vd::string& prefix, vd::i32 index, vd::i32 padding);

    bool Request(vd::i32 index);

    void SetBlockRequest(HiDistData::Block::Value v, vd::i32 enable = 1);
    void SetTypeRequest(HiDistData::GalaxyType::Value v, vd::i32 enable = 1);
    void SetStatisticRequest(HiDistData::Block::Value v, vd::i32 enable = 1); 

    vd::i32 GetFileCount() const { return m_FileCount; }
    vd::string GetFilePrefix() const { return m_FilePrefix; }
    vd::i32 GetStartFileIndex() const { return m_StartFileIndex; }
    vd::i32 GetEndFileIndex() const { return m_EndFileIndex; }
    vd::i32 GetCurrentFileIndex() const { return m_CurrentFileIndex; }
    void SetCurrentFileIndex(vd::i32 index) { m_CurrentFileIndex = index; }

    bool IsOpen() const { return m_IsOpen; }
    bool IsResident(vd::i32 index);
    bool IsPending(vd::i32 index);
    bool IsReady(vd::i32 index);
    bool IsValidIndex(vd::i32 index) { return (index >= m_StartFileIndex && index <= m_EndFileIndex); }
    bool IsValidBlock(HiDistData::Block::Value v) const { return HiDistData::Block::IsValid(v); }
    bool IsValidGalaxyType(HiDistData::GalaxyType::Value v) const { return HiDistData::GalaxyType::IsValid(v); }
    void Retain(vd::i32 index, HiDistData* dat);

    HiDistData*
    Retrieve(const vd::i32& index);
    
    HiDistWorkItem* GetPendingWorkItem(vd::i32 index);
    Runtime::Context* GetRuntime() { return m_Runtime; }
    
    VD_DECLARE_OBJECT(HiDistDataset);
    
private:

    VD_DISABLE_COPY_CONSTRUCTORS(HiDistDataset);

    HiDistData* 
    OnFetch(const vd::i32& index);
    
    void 
    OnEvict(HiDistData* dat);

    HiDistWorkItem*
    OnSubmit(const vd::i32& index);
    
    void 
    OnComplete(HiDistWorkItem* work);

    void Store(vd::i32 index, HiDistData* dat);
    void Release(vd::i32 index);
    void Evict();
    
    void RetainWorkItem(vd::i32 index, HiDistWorkItem* task);  
    void ReleaseWorkItem(vd::i32 index);

    bool m_IsOpen;
    vd::string m_FilePrefix;
    vd::i32 m_FileNumberPadding;
    vd::i32 m_FileCount;
    vd::i32 m_LastResidentFileIndex;
    vd::i32 m_CurrentFileIndex;
    vd::i32 m_StartFileIndex;
    vd::i32 m_EndFileIndex;
    vd::i32 m_FileSplits;

    vd::i32 m_RequestedTypes[32];
    vd::i32 m_RequestedBlocks[32];
    vd::i32 m_RequestedStatistics[32];

    vd::i32            m_CacheSize;
    DataCache          m_DataCache;
    WorkItemCache      m_WorkCache;
    HiDistWorkQueue    m_WorkQueue;
    Mutex              m_Mutex; 
    Runtime::Context*  m_Runtime;
};

// ============================================================================================== //

VD_FORMATS_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_FORMATS_WHD_HEADER_INCLUDED

// ============================================================================================== //
// END OF FILE

