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

#if defined(VD_USE_GS2)

// ============================================================================================== //

#ifndef VD_DATA_GS2_HEADER_INCLUDED
#define VD_DATA_GS2_HEADER_INCLUDED

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

VD_USING(Core, Mutex);
VD_USING(Core, WorkItem);
VD_USING(Core, WorkQueue);
VD_USING(Core, AtomicCounter);
VD_USING(Containers, LruCache);

// ============================================================================================== //

namespace Gadget {

// ============================================================================================== //

class DataSet;

// ============================================================================================== //

struct MetaData
{
    int ParticleCount[6];                  
    double Mass[6];               
    double Time;                            
    double Redshift;                        
    int HasStarFormation;                   
    int HasFeedback;                    
    int ParticleCountTotal[6];    
    int HasCooling;                     
    int FileCountPerDataFile;              
    double BoxSize;                         
    double Omega0;                          
    double OmegaLambda;                     
    double HubbleParam;                     
    int HasStellarAge;                      
    int HasMetals;                          
    unsigned int ParticleCountTotalHighWord[6];  
    int  HasEntropyInsteadofU;              
    char Fill[56];                          //!< Fills to 256 Bytes 
};

// vdStaticAssert((sizeof(GadgetMetaData) == 256), InvalidGS2HeaderSize);

// ============================================================================================== //

struct ParticleData
{
    int* Id;
    int* Type;
    float* Position;
    float* Velocity;
    float* Mass;
    float* Density;
    float* InternalEnergy;
    float* Temp;
    float* Ne;
    float* SmoothingLength;
    float* Potential;
    float* Acceleration;
    float* DtEntropy;
    float* TimeStep;
};

struct BlockSummary
{
    float Minimum[3];
    float Maximum[3];
    float Mean[3];
    float Variance[3];
    float TotalSum[3];
    float SumSqr[3];
    size_t Count[3];
    int    Components;
};

struct RangeData
{
    void*  Base;
    void*  StartRange;
    void*  EndRange;
    size_t Offset;
    size_t TotalBytes;      
};

struct BlockRangeData
{
    RangeData Id;
    RangeData Type;
    RangeData Position;
    RangeData Velocity;
    RangeData Mass;
    RangeData Density;
    RangeData InternalEnergy;
    RangeData Temp;
    RangeData Ne;
    RangeData SmoothingLength;
    RangeData Potential;
    RangeData Acceleration;
    RangeData DtEntropy;
    RangeData TimeStep;
};

// ============================================================================================== //

class DataFile : public Object
{
public:

    // Order *must* match block order in snapshot file (eg v2)
    struct ParticleType
    {
        enum Value 
        {
            Gas           = 0,
            Halo          = 1,
            Disk          = 2,
            Bulge         = 3,
            Star          = 4,
            Boundary      = 5,
            LastValue     = 6
        };

        static const size_t Count = LastValue;

        static const char* ToString(vd::u32 value) 
        { 
            switch (value)
            {
            case Gas:       return "Gas";
            case Halo:      return "Halo";
            case Disk:      return "Disk";
            case Bulge:     return "Bulge";
            case Star:      return "Star";
            case Boundary:  return "Boundary";
            case LastValue: return "LastValue"; 
            default:        return "<INVALID>";
            }
            return "<INVALID>";
        }

        static ParticleType::Value FromInteger(
            vd::u32 value) 
        { 
            switch (value)
            {
            case 0: return Gas;
            case 1: return Halo;
            case 2: return Disk;
            case 3: return Bulge;
            case 4: return Star;
            case 5: return Boundary;
            case 6: return LastValue;
            default: return LastValue; 
            }
            return LastValue;
        }

        static vd::u32 ToInteger(
            ParticleType::Value value) 
        { 
            switch (value)
            {
            case Gas:       return 0;
            case Halo:      return 1;
            case Disk:      return 2;
            case Bulge:     return 3;
            case Star:      return 4;
            case Boundary:  return 5;
            case LastValue: return Count;
            default:        return Count; 
            }
            return Count;
        }

        static bool IsValid(
            ParticleType::Value value)             
        {
            return ToInteger(value) < ParticleType::Count;
        }
    };


    // Order *must* match block order in snapshot file (eg v2)
    //  IO_POS,
    //  IO_VEL,
    //  IO_ID,
    //  IO_MASS,
    //  IO_U,
    //  IO_RHO,
    //  IO_HSML,
    //  IO_POT,
    //  IO_ACCEL,
    //  IO_DTENTR,
    //  IO_TSTP,

    // Order *must* match block order in snapshot file (eg v2)
    struct Block
    {
        enum Value 
        {
            Position            = 0,
            Velocity            = 1,
            Id                  = 2,
            Mass                = 3,
            InternalEnergy      = 4,
            Density             = 5,
            SmoothingLength     = 6,
            Potential           = 7,
            Acceleration        = 8,
            DtEntropy           = 9,
            TimeStep            = 10,
            Temperature         = 11,
            Type                = 12,
            LastValue           = 13
        };

        static const size_t Count = LastValue;

        static const char* ToString(vd::u32 value) 
        { 
            switch (value)
            {
            case Position:              return "Position";
            case Velocity:              return "Velocity";
            case Id:                    return "Id";
            case Mass:                  return "Mass";
            case InternalEnergy:        return "InternalEnergy";
            case Density:               return "Density";
            case SmoothingLength:       return "SmoothingLength"; 
            case Potential:             return "Potential"; 
            case Acceleration:          return "Acceleration"; 
            case DtEntropy:             return "DtEntropy"; 
            case TimeStep:              return "TimeStep"; 
            case Temperature:           return "Temperature"; 
            case Type:                  return "Type"; 
            case LastValue:             return "LastValue"; 
            default:                    return "<INVALID>";
            }
            return "<INVALID>";
        }

        static Block::Value FromInteger(
            vd::u32 value) 
        { 
            switch (value)
            {
            case 0: return Position;
            case 1: return Velocity;
            case 2: return Id;
            case 3: return Mass;
            case 4: return InternalEnergy;
            case 5: return Density;
            case 6: return SmoothingLength;
            case 7: return Potential;
            case 8: return Acceleration;
            case 9: return DtEntropy;
            case 10: return TimeStep;
            case 11: return Temperature;
            case 12: return Type;
            case 13: return LastValue;
            default: return LastValue; 
            }
            return LastValue;
        }

        static vd::u32 ToInteger(
            Block::Value value) 
        { 
            switch (value)
            {
            case Position:          return 0;
            case Velocity:          return 1;
            case Id:                return 2;
            case Mass:              return 3;
            case InternalEnergy:    return 4;
            case Density:           return 5;
            case SmoothingLength:   return 6;
            case Potential:         return 7;
            case Acceleration:      return 8;
            case DtEntropy:         return 9;
            case TimeStep:          return 10;
            case Temperature:       return 11;
            case Type:              return 12;
            case LastValue:         return Count;
            default:                return Count; 
            }
            return Count;
        }
        static bool IsValid(
            Block::Value value)             
        {
            return ToInteger(value) < Block::Count;
        }
    };


    typedef vd::i32 BlockRequest[Block::Count];
    typedef vd::i32 TypeRequest[ParticleType::Count];
    
public:
        
    DataFile();    
    virtual ~DataFile();

    vd::status
    Load(
        const vd::string& prefix, 
        vd::i32 index,
        vd::i32 splits=1,
        vd::i32 padding=3,
        vd::i32* req_data=0,
        vd::i32* req_types=0);
    
    virtual vd::status 
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
    ReadHeader(FILE* fd, vd::i32 splits);
    
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
    ReadBlockData(FILE* fd, Block::Value block, vd::bytesize offset, vd::i32* types);
    
    vd::bytesize 
    ReadPositionData(FILE* fd, vd::bytesize offset, vd::i32* types);

    vd::bytesize
    ReadVelocityData(FILE* fd, vd::bytesize offset, vd::i32* types);

    vd::bytesize
    ReadIdentifierData(FILE* fd, vd::bytesize offset, vd::i32* types);

    vd::bytesize
    ReadMassData(FILE* fd, vd::bytesize offset, vd::i32* types);

    vd::bytesize
    ReadInternalEnergyData(FILE* fd, vd::bytesize offset, vd::i32* types);

    vd::bytesize
    ReadDensityData(FILE* fd, vd::bytesize offset, vd::i32* types);

    vd::bytesize
    ReadSmoothingLengthData(FILE* fd, vd::bytesize offset, vd::i32* types);

    vd::bytesize
    ReadPotentialData(FILE* fd, vd::bytesize offset, vd::i32* types);

    vd::bytesize
    ReadAcceleration(FILE* fd, vd::bytesize offset, vd::i32* types);

    vd::bytesize
    ReadRateOfChangeOfEntropyData(FILE* fd, vd::bytesize offset, vd::i32* types);

    vd::bytesize
    ReadTimeStepData(FILE* fd, vd::bytesize offset, vd::i32* types);

    vd::bytesize
    SkipBlock(FILE* fd, Block::Value block, vd::bytesize start);
    
    vd::bytesize
    ReadBlock(FILE* fd, Block::Value block, vd::bytesize offset, vd::i32* types);

    vd::f32
    Periodic(vd::f32 x, vd::f32 l2);
    
    void
    SwapFloatDataAt(float* ptr, int isrc, int idst);
    
    void
    SwapIntDataAt(int* ptr, int isrc, int idst);
    
    void
    SwapFloat3DataAt(float* ptr, int isrc, int idst);
    
    void
    SwapParticleAt(int isrc, int idst);
    
    void
    ComputeTempFromInternalEnergy(void);

    void
    Reorder(void);
    
    const MetaData& GetMetaData() const { return m_MetaData; }

    BlockSummary& GetBlockSummary(Block::Value v);
    void SetBlockSummary(Block::Value v, const BlockSummary& summary);

    vd::u64
    GetTotalParticleCount() const { return m_TotalParticleCount; }

    vd::u64
    GetFilteredParticleCount() const { return m_FilteredParticleCount; }

    vd::u64
    GetParticleCountForType(ParticleType::Value v) const 
    { 
        return m_MetaData.ParticleCount[ParticleType::ToInteger(v)]; 
    }

    vd::f32
    GetBoundaryBoxSize() const { return m_MetaData.BoxSize; }
    
    vd::u32
    GetFileIndex() const { return m_FileIndex; }

    vd::bytesize
    GetResidentMemorySize() const;
    
    bool
    IsLoaded() { return m_IsLoaded.Get() > 0 ? true : false; }
    
    vd::string
    GetFilename(const vd::string& prefix, vd::i32 index, vd::i32 padding);
    
    VD_DECLARE_OBJECT(DataFile);
    
private:

    VD_DISABLE_COPY_CONSTRUCTORS(DataFile);

    MetaData          m_MetaData;
    ParticleData      m_ParticleData;
    BlockSummary      m_SummaryData[Block::Count];
    vd::u64           m_TotalParticleCount;
    vd::u64           m_FilteredParticleCount;
    vd::u64           m_GasParticleCount;
    vd::u32           m_FileIndex;
    AtomicCounter     m_IsLoaded;
};

// ============================================================================================== //

class WorkQueue : public Core::WorkQueue
{
public:

    WorkQueue() : Core::WorkQueue() {} 
    virtual ~WorkQueue() { Destroy(); } 

    virtual void OnRun(Core::WorkItem* item);

    VD_DECLARE_OBJECT(WorkQueue);

private:

    VD_DISABLE_COPY_CONSTRUCTORS(WorkQueue);

};

// ============================================================================================== //

class WorkItem : public Core::WorkItem
{
    friend class Gadget::WorkQueue;
    
public:

    explicit WorkItem(
        DataFile* snapshot,
        const vd::string& prefix,
        vd::i32 index,
        vd::i32 splits,
        vd::i32 padding,
        vd::i32* blocks,
        vd::i32* types,
        vd::i32* stats
    ) :
        Core::WorkItem(),
        m_DataFile(snapshot),
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
    DataFile* GetDataFile(void) { return m_DataFile; }
    const vd::string& GetFilePrefix(void) const { return m_FilePrefix; }
    vd::i32 GetFileIndex(void) const { return m_FileIndex; }
    vd::i32 GetFileNumberPadding(void) const { return m_FileNumberPadding; }
    vd::i32 GetFileSplits(void) const { return m_FileSplits; }

    VD_DECLARE_OBJECT(WorkItem);
    
private:

    VD_DISABLE_COPY_CONSTRUCTORS(WorkItem);
    
    DataFile* m_DataFile; 
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

struct ResidentSizeFn 
{
    unsigned long operator()( const DataFile* x ) 
    {
        if(x != NULL)
            return x->GetResidentMemorySize(); 
        return 1; 
    }
};

class DataSet : public Object
{
public:
    typedef LruCache< vd::i32, WorkItem* > WorkItemCache;
    typedef LruCache< vd::i32, DataFile*, ResidentSizeFn > DataCache;

    DataSet(Runtime::Context* runtime);    
    virtual ~DataSet();
    
    virtual vd::status 
    Destroy();
    
    vd::status 
    Open(const vd::string& prefix, vd::i32 count=0, vd::i32 splits=1);
    
    vd::status 
    Close();
    
    static vd::string
    GetFilenameForDataFile(const vd::string& prefix, vd::i32 index, vd::i32 padding);

    bool Request(vd::i32 index);

    void SetBlockRequest(DataFile::Block::Value v, vd::i32 enable = 1) 
        { m_BlockRequests[DataFile::Block::ToInteger(v)] = enable; }

    void SetTypeRequest(DataFile::ParticleType::Value v, vd::i32 enable = 1) 
        { m_TypeRequests[DataFile::ParticleType::ToInteger(v)] = enable; }
    
    void SetSummaryRequest(DataFile::Block::Value v, vd::i32 enable = 1) 
        { m_SummaryRequests[DataFile::Block::ToInteger(v)] = enable; }

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
    bool IsValidBlock(DataFile::Block::Value v) const { return v != DataFile::Block::LastValue; }
    bool IsValidParticleType(DataFile::ParticleType::Value v) const { return v != DataFile::ParticleType::LastValue; }
    void Retain(vd::i32 index, DataFile* snapshot);

    DataFile*
    Retrieve(const vd::i32& index);
    
    WorkItem* GetPendingWorkItem(vd::i32 index);
    
    VD_DECLARE_OBJECT(DataSet);
    
private:

    VD_DISABLE_COPY_CONSTRUCTORS(DataSet);

    DataFile* 
    OnFetch(const vd::i32& index);
    
    void 
    OnEvict(DataFile* snapshot);

    WorkItem*
    OnSubmit(const vd::i32& index);
    
    void 
    OnComplete(WorkItem* work);

    void Store(vd::i32 index, DataFile* snapshot);
    void Release(vd::i32 index);
    void Evict();
    
    void RetainWorkItem(vd::i32 index, WorkItem* task);  
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

    vd::i32 m_TypeRequests[DataFile::ParticleType::Count];
    vd::i32 m_BlockRequests[DataFile::Block::Count];
    vd::i32 m_SummaryRequests[DataFile::Block::Count];

    vd::i32            m_CacheSize;
    DataCache          m_DataCache;
    WorkItemCache      m_WorkCache;
    WorkQueue          m_WorkQueue;
    Mutex              m_Mutex; 
    Runtime::Context*  m_Runtime;
};

// ############################################################################################## //
// FIXME: Subblock indexing borked - fix!
// ############################################################################################## //

#if 0

// ============================================================================================== //

VD_USING(Core, Mutex);
VD_USING(Core, WorkItem);
VD_USING(Core, WorkQueue);
VD_USING(Core, AtomicCounter);
VD_USING(Containers, LruCache);
VD_USING(Containers, Map);

// ============================================================================================== //

namespace Gadget {

// ============================================================================================== //

class DataSet;

// ============================================================================================== //

struct MetaData
{
	int ParticleCount[6];                  
	double Mass[6];               
	double Time;                         	
	double Redshift;                     	
	int HasStarFormation;                   
	int HasFeedback;                   	
	int ParticleCountTotal[6];    
	int HasCooling;                    	
	int FileCountPerDataFile;              
	double BoxSize;                      	
	double Omega0;                       	
	double OmegaLambda;                  	
	double HubbleParam;                  	
	int HasStellarAge;                 		
	int HasMetals;                     		
	unsigned int ParticleCountTotalHighWord[6];  
	int  HasEntropyInsteadofU;	         	
	char Fill[56];	               			//!< Fills to 256 Bytes 
};

// vdStaticAssert((sizeof(MetaData) == 256), InvalidGS2HeaderSize);

// ============================================================================================== //

struct ParticleData
{
	int* Id;
    int* Type;
    float* Position;
    float* Velocity;
    float* Mass;
    float* Density;
    float* InternalEnergy;
    float* Temp;
    float* Ne;
    float* SmoothingLength;
    float* Potential;
    float* Acceleration;
    float* DtEntropy;
    float* TimeStep;
};

struct Statistic
{
    float Minimum[3];
    float Maximum[3];
    float Mean[3];
    float Variance[3];
    float TotalSum[3];
    float SumSqr[3];
    size_t Count[3];
    int    Components;
};

struct ParticleRange
{
	size_t StartIndex;
	size_t EndIndex;		
};

// ============================================================================================== //

class DataFile : public Object
{
public:

	// Order *must* match block order in snapshot file (eg v2)
    struct ParticleType
    {
        enum Value 
        {
    		Gas           = 0,
    		Halo          = 1,
    		Disk          = 2,
    		Bulge         = 3,
    		Star          = 4,
    		Boundary      = 5,
            LastValue     = 6
        };

        static const size_t Count = LastValue;

        static const char* ToString(vd::u32 value) 
        { 
            switch (value)
            {
            case ParticleType::Gas:       return "Gas";
            case ParticleType::Halo:      return "Halo";
            case ParticleType::Disk:      return "Disk";
            case ParticleType::Bulge:     return "Bulge";
            case ParticleType::Star:      return "Star";
            case ParticleType::Boundary:  return "Boundary";
            case ParticleType::LastValue: return "LastValue"; 
            default:                      return "<INVALID>";
            }
            return "<INVALID>";
        }

        static ParticleType::Value FromInteger(
            vd::u32 value) 
        { 
            switch (value)
            {
            case 0: return ParticleType::Gas;
            case 1: return ParticleType::Halo;
            case 2: return ParticleType::Disk;
            case 3: return ParticleType::Bulge;
            case 4: return ParticleType::Star;
            case 5: return ParticleType::Boundary;
            case 6: return ParticleType::LastValue;
            default: return ParticleType::LastValue; 
            }
            return LastValue;
        }

        static vd::u32 ToInteger(
            ParticleType::Value value) 
        { 
            switch (value)
            {
            case ParticleType::Gas:       return 0;
            case ParticleType::Halo:      return 1;
            case ParticleType::Disk:      return 2;
            case ParticleType::Bulge:     return 3;
            case ParticleType::Star:      return 4;
            case ParticleType::Boundary:  return 5;
            case ParticleType::LastValue: return ParticleType::Count;
            default:                      return ParticleType::Count; 
            }
            return Count;
        }

        static bool IsValid(
            ParticleType::Value value)             
        {
            return ToInteger(value) < ParticleType::Count;
        }
    };


    // Order *must* match block order in snapshot file (eg v2)
    //  IO_POS,
    //  IO_VEL,
    //  IO_ID,
    //  IO_MASS,
    //  IO_U,
    //  IO_RHO,
    //  IO_HSML,
    //  IO_POT,
    //  IO_ACCEL,
    //  IO_DTENTR,
    //  IO_TSTP,

    // Order *must* match block order in snapshot file (eg v2)
    struct Block
    {
        enum Value 
        {
            Position            = 0,
            Velocity            = 1,
            Id                  = 2,
            Mass                = 3,
            InternalEnergy      = 4,
            Density             = 5,
            SmoothingLength     = 6,
            Potential           = 7,
            Acceleration        = 8,
            DtEntropy           = 9,
            TimeStep            = 10,
            Temperature         = 11,
            Type                = 12,
            LastValue           = 13
        };

        static const size_t Count = LastValue;

        static const char* ToString(vd::u32 value) 
        { 
            switch (value)
            {
            case Position:              return "Position";
            case Velocity:              return "Velocity";
            case Id:                    return "Id";
            case Mass:                  return "Mass";
            case InternalEnergy:        return "InternalEnergy";
            case Density:               return "Density";
            case SmoothingLength:       return "SmoothingLength"; 
            case Potential:             return "Potential"; 
            case Acceleration:          return "Acceleration"; 
            case DtEntropy:             return "DtEntropy"; 
            case TimeStep:              return "TimeStep"; 
            case Temperature:           return "Temperature"; 
            case Type:                  return "Type"; 
            case LastValue:             return "LastValue"; 
            default:                    return "<INVALID>";
            }
            return "<INVALID>";
        }

        static Block::Value FromInteger(
            vd::u32 value) 
        { 
            switch (value)
            {
            case 0: return Position;
            case 1: return Velocity;
            case 2: return Id;
            case 3: return Mass;
            case 4: return InternalEnergy;
            case 5: return Density;
            case 6: return SmoothingLength;
            case 7: return Potential;
            case 8: return Acceleration;
            case 9: return DtEntropy;
            case 10: return TimeStep;
            case 11: return Temperature;
            case 12: return Type;
            case 13: return LastValue;
            default: return LastValue; 
            }
            return LastValue;
        }

        static vd::u32 ToInteger(
            Block::Value value) 
        { 
            switch (value)
            {
            case Position:          return 0;
            case Velocity:          return 1;
            case Id:                return 2;
            case Mass:              return 3;
            case InternalEnergy:    return 4;
            case Density:           return 5;
            case SmoothingLength:   return 6;
            case Potential:         return 7;
            case Acceleration:      return 8;
            case DtEntropy:         return 9;
            case TimeStep:          return 10;
            case Temperature:       return 11;
            case Type:              return 12;
            case LastValue:         return Block::Count;
            default:                return Block::Count; 
            }
            return Count;
        }

        static bool IsValid(
            Block::Value value)             
        {
            return ToInteger(value) < Block::Count;
        }
    };


	typedef vd::i32 BlockRequest[Block::Count];
	typedef vd::i32 TypeRequest[ParticleType::Count];
	
public:
		
    DataFile(DataSet* dataset);    
    virtual ~DataFile();

	vd::status
	Load(
		const vd::string& file_prefix, 
		vd::i32 snapshot_index,
        vd::i32 start_index=0,
        vd::i32 end_index=0,
		vd::i32 splits=1,
		vd::i32 padding=4,
		vd::i32* req_data=0,
		vd::i32* req_types=0);
	
	virtual vd::status 
    Destroy();
	
	vd::status 
	Close();

    // --

    size_t
    CreateSubBlockData(Block::Value block, size_t start_index, size_t end_index);
    
    void 
    DestroySubBlockData(Block::Value block, size_t start_index, size_t end_index);

    size_t   
    GetSubBlockDataSize(Block::Value block, size_t start_index, size_t end_index) const;
    
    void*   
    GetSubBlockDataPtr(Block::Value block, size_t start_index, size_t end_index) const;

    void 
    SetSubBlockDataPtr(void* ptr, size_t bytes, Block::Value block, size_t start_index, size_t end_index);

    size_t
    GetSubBlockParticleCount(Block::Value block, size_t start_index, size_t end_index) const;

    // --	
	
	size_t 
	ReadHeader(FILE* fd, vd::i32 splits, vd::i32* req_types);
	
	size_t
	ReadTypeData(FILE* fd, size_t start_index, size_t end_index);
	
	size_t
	GetBlockSeparatorSize(void) const;
	
	vd::i32
	SkipToNextBlock(FILE* fh);
	
	size_t
	FindBlockByName(FILE* fd, const char* label);
	
	size_t
	FindBlock(FILE* fd, Block::Value block);

	const char* 
	GetBlockLabel(Block::Value block) const;
	
	const char* 
	GetBlockName(Block::Value block) const;
	
	size_t
	GetBlockSize(Block::Value block) const;
	
	size_t 
	GetBlockEntrySize(Block::Value block) const;
	
	vd::i32 
	GetBlockVectorLength(Block::Value block) const;
	
	const char* 
	GetBlockSizeTypeSuffix(Block::Value block) const;
	
	bool 
	IsBlockEntryIntegerValue(Block::Value block) const;

	size_t 
	ReadSubBlockData(FILE* fd, Block::Value block, size_t start_index, size_t end_index, vd::i32* types);
	
	size_t 
	ReadPositionData(FILE* fd, size_t start_index, size_t end_index, vd::i32* types);

	size_t
	ReadVelocityData(FILE* fd, size_t start_index, size_t end_index, vd::i32* types);

	size_t
	ReadIdentifierData(FILE* fd, size_t start_index, size_t end_index, vd::i32* types);

	size_t
	ReadMassData(FILE* fd, size_t start_index, size_t end_index, vd::i32* types);

	size_t
	ReadInternalEnergyData(FILE* fd, size_t start_index, size_t end_index, vd::i32* types);

	size_t
	ReadDensityData(FILE* fd, size_t start_index, size_t end_index, vd::i32* types);

	size_t
	ReadSmoothingLengthData(FILE* fd, size_t start_index, size_t end_index, vd::i32* types);

	size_t
	ReadPotentialData(FILE* fd, size_t start_index, size_t end_index, vd::i32* types);

	size_t
	ReadAcceleration(FILE* fd, size_t start_index, size_t end_index, vd::i32* types);

	size_t
	ReadRateOfChangeOfEntropyData(FILE* fd, size_t start_index, size_t end_index, vd::i32* types);

	size_t
	ReadTimeStepData(FILE* fd, size_t start_index, size_t end_index, vd::i32* types);

	size_t
	SkipSubBlock(FILE* fd, Block::Value block, size_t start_index, size_t end_index);
	
	size_t
	ReadSubBlock(FILE* fd, Block::Value block, size_t start_index, size_t end_index, vd::i32* types);

	vd::f32
	Periodic(vd::f32 x, vd::f32 l2);
	
	void
	SwapFloatDataAt(float* ptr, int isrc, int idst);
	
	void
	SwapIntDataAt(int* ptr, int isrc, int idst);
	
	void
	SwapFloat3DataAt(float* ptr, int isrc, int idst);
    
	void
	SwapParticleAt(int isrc, int idst);
    
    void
    ComputeTempFromInternalEnergy(size_t start_index, size_t end_index);

    void
    Reorder(void);
    
    const MetaData& GetMetaData() const { return m_MetaData; }

    Statistic& GetStatistic(Block::Value v);

    void SetStatistic(Block::Value v, const Statistic& stats);

	vd::u64
	GetTotalParticleCount() const { return m_TotalParticleCount; }

    vd::u64
    GetFilteredParticleCount(size_t start_index, size_t end_index) const; 

	vd::u64
	GetParticleCountForType(ParticleType::Value v) const 
    { 
        return m_MetaData.ParticleCount[ParticleType::ToInteger(v)]; 
    }

	vd::f32
	GetBoundaryBoxSize() const { return m_MetaData.BoxSize; }
	
    vd::u32
    GetFileIndex() const { return m_FileIndex; }

    size_t
    GetResidentMemorySize() const;

    bool
    IsResident() { return m_IsLoaded.Get() > 0 ? true : false; }
    
    void
    SetResident(bool enable) { m_IsLoaded.Set(enable ? 1 : 0); }

    vd::string
    GetFilename(const vd::string& prefix, vd::i32 index, vd::i32 padding);
    
    VD_DECLARE_OBJECT(DataFile);
	
private:

    struct SubBlockIndex 
    {
        void*        Ptr;
        Block::Value BlockType;
        size_t       StartIndex;
        size_t       EndIndex;

        SubBlockIndex(
            void* ptr = NULL,
            Block::Value block_type = Block::LastValue,
            size_t start_index = VD_INVALID_INDEX,
            size_t end_index = VD_INVALID_INDEX
        ) :
            Ptr(ptr),
            BlockType(block_type),
            StartIndex(start_index),
            EndIndex(end_index)
        {
            // EMPTY!
        }

        SubBlockIndex(const SubBlockIndex& other)
        {
            this->Ptr = other.Ptr;
            this->BlockType = other.BlockType;
            this->StartIndex = other.StartIndex;
            this->EndIndex = other.EndIndex;
        }

        bool operator==(const SubBlockIndex& other)
        {
            return this->Ptr == other.Ptr &&
                   this->BlockType == other.BlockType &&
                   this->StartIndex == other.StartIndex &&
                   this->EndIndex == other.EndIndex;
        }

        SubBlockIndex& operator=(const SubBlockIndex& other)
        {
            *this = SubBlockIndex(other);
            return *this;
        }
    };

    typedef Containers::Vector<SubBlockIndex>::type             SubBlockList;

	VD_DISABLE_COPY_CONSTRUCTORS(DataFile);

    DataSet*            m_DataSet;
	MetaData 		    m_MetaData;
	ParticleData 		m_ParticleData;
    SubBlockList        m_SubBlockList;
    Statistic           m_StatisticsData[Block::Count];
    vd::u64             m_SplitStartIndex;
    vd::u64             m_SplitEndIndex;
	vd::u64 			m_TotalParticleCount;
	vd::u64 			m_FilteredParticleCount;
	vd::u64 			m_GasParticleCount;
    vd::u32 			m_FileIndex;
    AtomicCounter 		m_IsLoaded;
};

typedef DataFile Data;
// ============================================================================================== //

class DataSet;

// ============================================================================================== //

struct ResidentSizeFn 
{
    unsigned long operator()( const DataFile* x ) 
    {
        if(x != NULL)
            return x->GetResidentMemorySize() / 1024 / 1024; 
        return 1; 
    }
};

// ============================================================================================== //

struct DataRequest
{
    vd::i32                             DataFileIndex;
    DataFile::Block::Value              BlockType;
    size_t                              StartIndex;
    size_t                              EndIndex;

    DataRequest() :
        DataFileIndex(VD_INVALID_INDEX),
        BlockType(DataFile::Block::LastValue),
        StartIndex(VD_INVALID_INDEX),
        EndIndex(VD_INVALID_INDEX)
    {
        // EMPTY!
    }

    void Reset() 
    {
        this->DataFileIndex = VD_INVALID_INDEX;
        this->BlockType = DataFile::Block::LastValue;
        this->StartIndex = VD_INVALID_INDEX;
        this->EndIndex = VD_INVALID_INDEX;
    }

    bool operator==(const DataRequest& other)
    {
        return (this->DataFileIndex == other.DataFileIndex &&
                this->BlockType == other.BlockType && 
                this->StartIndex == other.StartIndex &&
                this->EndIndex == other.EndIndex);
    }
};

VD_FORCE_INLINE
bool operator<(const DataRequest& x, const DataRequest& y)
{
    return x.DataFileIndex < y.DataFileIndex &&
           x.BlockType < y.BlockType &&
           x.StartIndex < y.StartIndex;
}

// ============================================================================================== //

class WorkQueue;

class WorkItem : public Core::WorkItem
{
	friend class ::WorkQueue;
	
public:

	explicit WorkItem(
        DataRequest request,
		DataFile* snapshot,
        const vd::string& file_prefix,
		vd::i32 file_index,
		vd::i32 split_count,
		vd::i32 file_num_pad,
		vd::i32* block_request,
		vd::i32* type_request,
        vd::i32* stat_request
	) :
		Core::WorkItem(),
		m_DataFile(snapshot),
		m_FilePrefix(file_prefix),
        m_FileIndex(file_index),
		m_FileSplits(split_count),
		m_FileNumberPadding(file_num_pad),
		m_Blocks(block_request),
		m_Types(type_request),
        m_Stats(stat_request)
	{ 
		// EMPTY!
	}

	bool IsReady(void) { return m_IsReady.Get() > 0; }
    DataRequest GetDataRequest(void) { return m_Request; }
	DataFile* GetDataFile(void) { return m_DataFile; }
	const vd::string& GetFilePrefix(void) const { return m_FilePrefix; }
    vd::i32 GetFileIndex(void) const { return m_FileIndex; }
    vd::i32 GetFileNumberPadding(void) const { return m_FileNumberPadding; }
    vd::i32 GetFileSplits(void) const { return m_FileSplits; }

	VD_DECLARE_OBJECT(WorkItem);
	
private:

	VD_DISABLE_COPY_CONSTRUCTORS(WorkItem);
	
    DataRequest m_Request;
	DataFile* m_DataFile;	
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

class WorkQueue : public Core::WorkQueue
{
public:

	WorkQueue() : Core::WorkQueue() {} 
	virtual ~WorkQueue() { Destroy(); } 

    virtual void OnRun(WorkItem* item);

	VD_DECLARE_OBJECT(WorkQueue);

private:

	VD_DISABLE_COPY_CONSTRUCTORS(WorkQueue);

};

// ============================================================================================== //

class DataSet : public Object
{
public:
	typedef LruCache< DataRequest, WorkItem* >                 WorkItemCache;
	typedef LruCache< DataRequest, DataFile*, ResidentSizeFn > DataCache;

    DataSet(Runtime::Context* runtime);    
    virtual ~DataSet();
	
    virtual vd::status 
    Destroy();
    	
	vd::status 
	Open(const vd::string& prefix, vd::i32 count=0, vd::i32 splits=1);
	
	vd::status 
	Close();
	
	vd::string
	GetFilenameForDataFile(const vd::string& prefix, vd::i32 index, vd::i32 padding);

    bool Request(DataRequest rq);

	void SetBlockRequest(DataFile::Block::Value v, vd::i32 enable = 1);
	void SetTypeRequest(DataFile::ParticleType::Value v, vd::i32 enable = 1); 
    void SetStatisticRequest(DataFile::Block::Value v, vd::i32 enable = 1);
	
    vd::i32 GetFileCount() const;
    vd::string GetFilePrefix() const;
    vd::i32 GetStartFileIndex() const;
    vd::i32 GetEndFileIndex() const;
    vd::i32 GetCurrentFileIndex() const;
    void SetCurrentFileIndex(vd::i32 index);

    bool IsOpen() const;
    bool IsResident(DataRequest request);
	bool IsPending(DataRequest request);
	bool IsReady(DataRequest request);
	bool IsValidIndex(vd::i32 index) const;
    bool IsValidRequest(DataRequest req) const;
    bool IsValidBlock(DataFile::Block::Value v) const;
    bool IsValidParticleType(DataFile::ParticleType::Value v) const;
    void Retain(DataRequest request, DataFile* snapshot);

    DataFile*
	Retrieve(DataRequest request);
    
	WorkItem* 
    GetPendingWorkItem(DataRequest request);
	
	VD_DECLARE_OBJECT(DataSet);
	
private:

	VD_DISABLE_COPY_CONSTRUCTORS(DataSet);

	DataFile* 
	OnFetch(const DataRequest& index);
    
    void 
    OnEvict(DataFile* snapshot);

	WorkItem*
	OnSubmit(const DataRequest& request);
    
    void 
    OnComplete(WorkItem* work);

    void Store(DataRequest request, DataFile* snapshot);
    void Release(DataRequest request);
	void Evict();
	
	void RetainWorkItem(DataRequest request, WorkItem* task);  
	void ReleaseWorkItem(DataRequest request);

	bool m_IsOpen;
	vd::string m_FilePrefix;
	vd::i32 m_FileNumberPadding;
	vd::i32 m_FileCount;
	vd::i32 m_LastResidentFileIndex;
	vd::i32 m_CurrentFileIndex;
	vd::i32 m_StartFileIndex;
	vd::i32 m_EndFileIndex;
	vd::i32 m_FileSplits;

	vd::i32 m_RequestedTypes[DataFile::ParticleType::Count];
	vd::i32 m_RequestedBlocks[DataFile::Block::Count];
    vd::i32 m_RequestedStatistics[DataFile::Block::Count];

	vd::u64            m_CacheSize;
	DataCache          m_DataCache;
	WorkItemCache      m_WorkCache;
	WorkQueue          m_WorkQueue;
	Mutex              m_Mutex;	
	Runtime::Context*  m_Runtime;
};
// ============================================================================================== //

// ############################################################################################## //
#endif
// ############################################################################################## //

} // end namespace: Gadget

// ============================================================================================== //

VD_FORMATS_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_DATA_GS2_HEADER_INCLUDED

// ============================================================================================== //

#endif // VD_USE_GS2

// ============================================================================================== //
// END OF FILE

