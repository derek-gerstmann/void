// ============================================================================================== //
//
// License:		The Lesser GNU Public License (LGPL) v3.0.
// 
// Author(s): 	Derek Gerstmann. The University of Western Australia (UWA). 
//				As well as the shoulders of many giants...
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

VD_IMPORT(Core, Mutex);
VD_IMPORT(Core, WorkItem);
VD_IMPORT(Core, WorkQueue);
VD_IMPORT(Core, AtomicCounter);
VD_IMPORT(Containers, LruCache);

// ============================================================================================== //

struct GadgetMetaData
{
	int ParticleCount[6];                  
	double Mass[6];               
	double Time;                         	
	double Redshift;                     	
	int HasStarFormation;                   
	int HasFeedback;                   	
	int ParticleCountTotal[6];    
	int HasCooling;                    	
	int FileCountPerSnapshot;              
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

// vdStaticAssert((sizeof(GadgetMetaData) == 256), InvalidGS2HeaderSize);

// ============================================================================================== //

struct GadgetParticleData
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

struct GadgetScalarStatistic
{
    float Minimum;
    float Maximum;
    float Mean;
    float Variance;
    float TotalSum;
    float SumSqr;
    size_t Count;
};

struct GadgetRangeData
{
	void*  Base;
	void*  StartRange;
	void*  EndRange;
	size_t Offset;
	size_t TotalBytes;		
};

struct GadgetBlockRangeData
{
	GadgetRangeData Id;
    GadgetRangeData Type;
    GadgetRangeData Position;
    GadgetRangeData Velocity;
    GadgetRangeData Mass;
    GadgetRangeData Density;
    GadgetRangeData InternalEnergy;
    GadgetRangeData Temp;
    GadgetRangeData Ne;
    GadgetRangeData SmoothingLength;
    GadgetRangeData Potential;
    GadgetRangeData Acceleration;
    GadgetRangeData DtEntropy;
    GadgetRangeData TimeStep;
};

// ============================================================================================== //

class GadgetSnapshot : public Object
{
public:

	// Order *must* match block order in snapshot file (eg v2)
	typedef struct ParticleType
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

        static const vd::u32 ToInteger(
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
    typedef struct Block
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

        static const vd::u32 ToInteger(
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
    };


	typedef vd::i32 BlockRequest[Block::Count];
	typedef vd::i32 TypeRequest[ParticleType::Count];
	
public:
		
    GadgetSnapshot();    
    virtual ~GadgetSnapshot();

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
    
    const GadgetMetaData& GetMetaData() const { return m_MetaData; }

    GadgetScalarStatistic& GetScalarStatistic(Block::Value v) 
    { 
        return m_StatisticsData[Block::ToInteger(v)]; 
    }

    void SetScalarStatistic(Block::Value v, const GadgetScalarStatistic& stats) 
    {
        m_StatisticsData[Block::ToInteger(v)] = stats; 
    }

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
    
    VD_DECLARE_OBJECT(GadgetSnapshot);
	
private:

	VD_DISABLE_COPY_CONSTRUCTORS(GadgetSnapshot);

	GadgetMetaData 			m_MetaData;
	GadgetParticleData 		m_ParticleData;
    GadgetScalarStatistic   m_StatisticsData[Block::Count];
	vd::u64 				m_TotalParticleCount;
	vd::u64 				m_FilteredParticleCount;
	vd::u64 				m_GasParticleCount;
    vd::u32 				m_FileIndex;
    AtomicCounter 			m_IsLoaded;
};

// ============================================================================================== //

class GadgetDataset;

// ============================================================================================== //

class GadgetWorkItem : public WorkItem
{
	friend class GadgetWorkQueue;
	
public:

	explicit GadgetWorkItem(
		GadgetSnapshot* snapshot,
        const vd::string& prefix,
		vd::i32 index,
		vd::i32 splits,
		vd::i32 padding,
		vd::i32* blocks,
		vd::i32* types,
        vd::i32* stats
	) :
		WorkItem(),
		m_Snapshot(snapshot),
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
	GadgetSnapshot* GetSnapshot(void) { return m_Snapshot; }
	const vd::string& GetFilePrefix(void) const { return m_FilePrefix; }
    vd::i32 GetFileIndex(void) const { return m_FileIndex; }
    vd::i32 GetFileNumberPadding(void) const { return m_FileNumberPadding; }
    vd::i32 GetFileSplits(void) const { return m_FileSplits; }

	VD_DECLARE_OBJECT(GadgetWorkItem);
	
private:

	VD_DISABLE_COPY_CONSTRUCTORS(GadgetWorkItem);
	
	GadgetSnapshot* m_Snapshot;	
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

class GadgetWorkQueue : public WorkQueue
{
public:

	GadgetWorkQueue() : WorkQueue() {} 
	virtual ~GadgetWorkQueue() { Destroy(); } 

    virtual void OnRun(WorkItem* item);

	VD_DECLARE_OBJECT(GadgetWorkQueue);

private:

	VD_DISABLE_COPY_CONSTRUCTORS(GadgetWorkQueue);

};

// ============================================================================================== //

struct GadgetResidentSizeFn 
{
    unsigned long operator()( const GadgetSnapshot* x ) 
    {
    	if(x != NULL)
    		return x->GetResidentMemorySize(); 
	    return 1; 
	}
};

class GadgetDataset : public Object
{
public:
	typedef LruCache< vd::i32, GadgetWorkItem* > WorkItemCache;
	typedef LruCache< vd::i32, GadgetSnapshot*, GadgetResidentSizeFn > DataCache;

    GadgetDataset(Runtime::Context* runtime);    
    virtual ~GadgetDataset();
	
    virtual vd::status 
    Destroy();
    	
	vd::status 
	Open(const vd::string& prefix, vd::i32 count=0, vd::i32 splits=1);
	
	vd::status 
	Close();
	
	static vd::string
	GetFilenameForSnapshot(const vd::string& prefix, vd::i32 index, vd::i32 padding);

	bool Request(vd::i32 index);

	void SetBlockRequest(GadgetSnapshot::Block::Value v, vd::i32 enable = 1);
	void SetTypeRequest(GadgetSnapshot::ParticleType::Value v, vd::i32 enable = 1); 
    void SetStatisticRequest(GadgetSnapshot::Block::Value v, vd::i32 enable = 1);
	
    vd::i32 GetFileCount() const;
    vd::string GetFilePrefix() const;
    vd::i32 GetStartFileIndex() const;
    vd::i32 GetEndFileIndex() const;
    vd::i32 GetCurrentFileIndex() const;
    
    void SetCurrentFileIndex(vd::i32 index);
    bool IsOpen() const;
    bool IsResident(vd::i32 index);
	bool IsPending(vd::i32 index);
	bool IsReady(vd::i32 index);
	bool IsValidIndex(vd::i32 index);
    bool IsValidBlock(GadgetSnapshot::Block::Value v) const;
    bool IsValidParticleType(GadgetSnapshot::ParticleType::Value v) const;
    void Retain(vd::i32 index, GadgetSnapshot* snapshot);

    GadgetSnapshot*
	Retrieve(const vd::i32& index);
    
	GadgetWorkItem* GetPendingWorkItem(vd::i32 index);
	
	VD_DECLARE_OBJECT(GadgetDataset);
	
private:

	VD_DISABLE_COPY_CONSTRUCTORS(GadgetDataset);

	GadgetSnapshot* 
	OnFetch(const vd::i32& index);
    
    void 
    OnEvict(GadgetSnapshot* snapshot);

	GadgetWorkItem*
	OnSubmit(const vd::i32& index);
    
    void 
    OnComplete(GadgetWorkItem* work);

    void Store(vd::i32 index, GadgetSnapshot* snapshot);
    void Release(vd::i32 index);
	void Evict();
	
	void RetainWorkItem(vd::i32 index, GadgetWorkItem* task);  
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

	vd::i32 m_RequestedTypes[GadgetSnapshot::ParticleType::Count];
	vd::i32 m_RequestedBlocks[GadgetSnapshot::Block::Count];
    vd::i32 m_RequestedStatistics[GadgetSnapshot::Block::Count];

	vd::i32            m_CacheSize;
	DataCache          m_DataCache;
	WorkItemCache      m_WorkCache;
	GadgetWorkQueue    m_WorkQueue;
	Mutex              m_Mutex;	
	Runtime::Context*  m_Runtime;
};

// ============================================================================================== //

VD_FORMATS_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_DATA_GS2_HEADER_INCLUDED

// ============================================================================================== //

#endif // VD_USE_GS2

// ============================================================================================== //
// END OF FILE

