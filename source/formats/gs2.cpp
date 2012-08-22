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

#if defined(VD_USE_GS2)

// ============================================================================================== //

// Enable the following directive for verbose output to help debugging
// #define VD_DEBUG_GS2 (1)

// ============================================================================================== //

#include "formats/gs2.h"
#include "core/asserts.h"
#include "core/utilities.h"
#include "core/process.h"
#include "constants/constants.h"

#include <string.h>
#include <math.h>

// ============================================================================================== //

#define VD_DEBUG_GS2 (1)

// ============================================================================================== //

VD_FORMATS_NAMESPACE_BEGIN();

// ============================================================================================== //

namespace Gadget {

// ============================================================================================== //

VD_IMPORT(Core, Thread);
VD_IMPORT(Core, Memory);
VD_IMPORT(Core, Process);

// ============================================================================================== //

class InputThread : public Thread
{

};

Snapshot::Snapshot(
	Dataset* dataset) :
	Core::Object(),
	m_DataSet(dataset),
	m_TotalParticleCount(0),
	m_FilteredParticleCount(0),
	m_GasParticleCount(0),
	m_FileIndex(0)
{
    Memory::SetBytes(&m_MetaData, 		0, sizeof(m_MetaData));
    Memory::SetBytes(&m_ParticleData, 	0, sizeof(m_ParticleData));
    Memory::SetBytes(&m_StatisticsData,	0, sizeof(m_StatisticsData));
}
 
Snapshot::~Snapshot()
{
	Destroy();
}

size_t
Snapshot::GetResidentMemorySize() const
{
	size_t bytes = 0;
	for(SubBlockList::const_iterator it = m_SubBlockList.begin(); it != m_SubBlockList.end(); it++)
	{
		const void* ptr = GetSubBlockDataPtr(it->BlockType, it->StartIndex, it->EndIndex);
		if(ptr != NULL)
		{
			bytes += GetBlockEntrySize(it->BlockType) * GetFilteredParticleCount(it->StartIndex, it->EndIndex);
		}
	}
	vdLogInfo("Snapshot resident memory size: %d MB", bytes / 1024 / 1024);
	return bytes;
}

Statistic& 
Snapshot::GetStatistic(Block::Value v) 
{ 
    return m_StatisticsData[Block::ToInteger(v)]; 
}

void 
Snapshot::SetStatistic(Block::Value v, const Statistic& stats) 
{
    m_StatisticsData[Block::ToInteger(v)] = stats; 
}

vd::status 
Snapshot::Destroy()
{
	vdLogInfo("Destroying Snapshot '%d' ... ", m_FileIndex);
	
	for(SubBlockList::const_iterator it = m_SubBlockList.begin(); it != m_SubBlockList.end(); it++)
	{
		Block::Value block_type = it->BlockType;
		size_t start_index = it->StartIndex;
		size_t end_index = it->EndIndex;
		DestroySubBlockData(block_type, start_index, end_index);
	}

	m_SubBlockList.clear();
	return Status::Code::Success;
}

vd::u64
Snapshot::GetFilteredParticleCount(
	size_t start_index, size_t end_index) const
{
	size_t valid_start = 0;
	size_t valid_end = m_FilteredParticleCount;

	if(start_index > 0 && 
	   start_index != VD_INVALID_INDEX && 
	   start_index < m_FilteredParticleCount)
	{
		valid_start = start_index;
	}

	if(end_index > valid_start && 
	   end_index != VD_INVALID_INDEX &&
	   end_index < m_FilteredParticleCount)
	{
		valid_end = end_index;
	}

	if(valid_start >= valid_end)
		return 0;
		
	return valid_end - valid_start;
}

size_t
Snapshot::GetBlockSize(
	Block::Value block) const
{
	return GetBlockEntrySize(block) * m_FilteredParticleCount;
}

size_t
Snapshot::GetSubBlockDataSize(
	Block::Value block_type,
	size_t start_index,
	size_t end_index) const
{
	size_t bytes = GetBlockEntrySize(block_type) * GetFilteredParticleCount(start_index, end_index);
	return bytes;
}

size_t
Snapshot::CreateSubBlockData(
	Block::Value block_type, 
	size_t start_index, 
	size_t end_index)
{
	void* ptr = GetSubBlockDataPtr(block_type, start_index, end_index);
	size_t count = GetFilteredParticleCount(start_index, end_index);
	size_t bytes = GetBlockEntrySize(block_type) * count;
	if(ptr == NULL)
	{
		char* new_ptr = VD_NEW_ARRAY(char, bytes);
		if(new_ptr == NULL)
		{
			vdLogError("Failed to allocate memory for snapshot block '%s' data!", 
				GetBlockName(block_type));

			return 0;
		}
		Memory::SetBytes(new_ptr, 0, bytes);
		SetSubBlockDataPtr(new_ptr, bytes, block_type, start_index, end_index);
	}
	return bytes;
}

void
Snapshot::DestroySubBlockData(
	Block::Value block_type, 
	size_t start_index, 
	size_t end_index)
{
	char* ptr = (char*)GetSubBlockDataPtr(block_type, start_index, end_index);
	if(ptr == NULL)
		return;
	
	VD_SAFE_DELETE_ARRAY(ptr);
	SetSubBlockDataPtr(NULL, 0, block_type, start_index, end_index);
	return;
}

void*
Snapshot::GetSubBlockDataPtr(
	Block::Value block_type, 
	size_t start_index, 
	size_t end_index) const
{
	SubBlockList::const_iterator it;
	for(it = m_SubBlockList.begin(); it != m_SubBlockList.end(); it++)
	{
		if(it->BlockType == block_type &&
		   it->StartIndex == start_index &&
		   it->EndIndex == end_index )
		{
			return it->Ptr;
		}
	}
	return NULL;
}

void
Snapshot::SetSubBlockDataPtr(
	void* ptr, size_t bytes, 
	Block::Value block_type, 
	size_t start_index, 
	size_t end_index)
{
	void* old_ptr = GetSubBlockDataPtr(block_type, start_index, end_index);

	size_t slot = 0;
	bool found = false;
	SubBlockList::iterator it;
	for(it = m_SubBlockList.begin(); it != m_SubBlockList.end(); it++)
	{
		slot++;
		if(it->BlockType == block_type &&
		   it->StartIndex == start_index &&
		   it->EndIndex == end_index )
		{
			found = true;
			it->Ptr = ptr;
			break;
		}
	}

	if(found == false)
	{
		slot = m_SubBlockList.size();
		SubBlockIndex sbi(ptr, block_type, start_index, end_index);
		m_SubBlockList.push_back(sbi);
	}

	if(old_ptr) 
	{
		char* byte_array = (char*)old_ptr;
		VD_DELETE_ARRAY(byte_array);
	}
	return;
}

size_t
Snapshot::ReadTypeData(
	FILE* fd, 
	size_t start_index, 
	size_t end_index)
{
	Block::Value block = Block::Type;
	
	void* ptr = GetSubBlockDataPtr(block, start_index, end_index);
	if(ptr == NULL) CreateSubBlockData(block, start_index, end_index);

	vd::i32 pt = 0;
	vd::i64 pc = 0;
	for(pt = 0; pt < (vd::i32)ParticleType::Count; pt++)
	{
		for(vd::i64 pn = 0; pn < m_MetaData.ParticleCount[pt]; pn++)
		{
			m_ParticleData.Type[pc++] = pt;
		}
	}
	return pt;
}

size_t
Snapshot::GetBlockSeparatorSize(void) const
{
	return size_t(sizeof(char) * 4); //	32-bit
}

vd::i32
Snapshot::SkipToNextBlock(
	FILE* fh)
{
    vd::i32 dummy = 1;
    int result = fseek(fh, sizeof(dummy), SEEK_CUR);
//    size_t bytes = fread(&dummy, sizeof(dummy), 1, fh);
    vdAssert(result == 0);
	return result ? dummy : 0;
}

size_t
Snapshot::FindBlockByName(
	FILE* fd, const char* label)
{
	size_t rd = 0;
    int blocksize=0;
    char blocklabel[5]= {"    "};

    while(!feof(fd) && blocksize == 0)
    {
		rd = fread(blocklabel, 4*sizeof(char), 1, fd);
		if(memcmp(label,blocklabel,4)==0)
			vdLogInfo("Found Block <%s> ... ",blocklabel);	

//		if(rd < 1)
//			continue;

//		Info("Found Block <%s> with %d bytes",blocklabel,blocksize);	
//		rd = fread(&blocksize, sizeof(int), 1, fd);
//		if(rd < 1)
//			continue;

/*			
		if(blocksize)
		{
			Info("Found Block <%s> with %d bytes",blocklabel,blocksize);	
			if(strcmp(label,blocklabel)!=0)
			{
				fseek(fd,blocksize,SEEK_CUR);
				blocksize=0;
			}
		}
*/
    }
    return(blocksize-8);
}

size_t
Snapshot::FindBlock(
	FILE* fd, Block::Value block)
{
	return FindBlockByName(fd, GetBlockLabel(block));
}

const char*
Snapshot::GetBlockLabel(
	Block::Value block)  const
{
	switch (block)
	{
		case Block::Position:				return "POS";
		case Block::Velocity:				return "VEL";
		case Block::Id:						return "ID";
		case Block::Type:					return "TYPE";
		case Block::Mass:					return "MASS";
		case Block::InternalEnergy:			return "U";
		case Block::Density:				return "RHO";
		case Block::SmoothingLength:		return "HSML";
		case Block::Potential:				return "POT";
		case Block::Acceleration:			return "ACCE";
		case Block::DtEntropy:				return "ENDT";
		case Block::TimeStep:				return "TSTP";
		default:							return "\0\0\0\0";
	}
	return "\0\0\0\0";
}

const char*
Snapshot::GetBlockName(
	Block::Value block)  const
{
	switch (block)
	{
    case Block::Position:			return "Position";
    case Block::Velocity:			return "Velocity";
    case Block::Id:					return "ParticleID";
    case Block::Type:				return "ParticleType";
    case Block::Mass:				return "Mass";
    case Block::InternalEnergy:		return "InternalEnergy";
    case Block::Density:			return "Density";
    case Block::SmoothingLength:	return "SmoothingLength";
    case Block::Potential:			return "Potential";
    case Block::Acceleration:		return "Acceleration";
    case Block::DtEntropy:			return "RateOfChangeOfEntropy";
    case Block::TimeStep:			return "TimeStep";
	default:
      break;
    };
    return " ";
}

size_t
Snapshot::GetBlockEntrySize(
	Block::Value block)  const
{
	switch (block)
	{
		case Block::Position:				
		case Block::Velocity:		
		case Block::Acceleration:
			return 3 * sizeof(vd::f32);
		case Block::Id:
			return 1 * sizeof(vd::i32); 	/* XXX - Could be sizeof(long long) if LONGID was defined for compiled sim code */
		case Block::Type:
			return 1 * sizeof(vd::i32); 	
		case Block::Mass:
		case Block::InternalEnergy:				
		case Block::Density:				
		case Block::SmoothingLength:			
		case Block::Potential:				
		case Block::DtEntropy:			
		case Block::TimeStep:			
			return sizeof(float);
		default:						
			return 0;
	}
	return 0;
}

vd::i32
Snapshot::GetBlockVectorLength(
	Block::Value block) const
{
	switch (block)
	{
		case Block::Position:				
		case Block::Velocity:		
		case Block::Acceleration:
			return 3;
		case Block::Id:
		case Block::Type:
			return 1; 	
		case Block::Mass:
		case Block::InternalEnergy:				
		case Block::Density:				
		case Block::SmoothingLength:			
		case Block::Potential:				
		case Block::DtEntropy:			
		case Block::TimeStep:			
			return 1;
		default:						
			return 0;
	}
	return 0;
}

const char*
Snapshot::GetBlockSizeTypeSuffix(
	Block::Value block) const
{
	switch (block)
	{
		case Block::Position:				
		case Block::Velocity:		
		case Block::Acceleration:
			return "3f";
		case Block::Id:
		case Block::Type:
			return "1i"; 	
		case Block::Mass:
		case Block::InternalEnergy:				
		case Block::Density:				
		case Block::SmoothingLength:			
		case Block::Potential:				
		case Block::DtEntropy:			
		case Block::TimeStep:			
			return "1f";
		default:						
			return " ";
	}
	return "";
}

bool
Snapshot::IsBlockEntryIntegerValue(
	Block::Value block) const
{
	switch (block)
	{
		case Block::Position:				
		case Block::Velocity:		
		case Block::Acceleration:
			return false;
		case Block::Id:
		case Block::Type:
			return true; 	
		case Block::Mass:
		case Block::InternalEnergy:				
		case Block::Density:				
		case Block::SmoothingLength:			
		case Block::Potential:				
		case Block::DtEntropy:			
		case Block::TimeStep:			
			return false;
		default:						
			return false;
	}
	return false;
}

size_t
Snapshot::ReadSubBlockData(
	FILE* fd, 
	Block::Value block,
	size_t start_index,
	size_t end_index,
	vd::i32* types)
{
	size_t rd = 0;
	size_t dx = 0;
	void* ptr = GetSubBlockDataPtr(block, start_index, end_index);
	if(ptr == NULL)
	{
		CreateSubBlockData(block, start_index, end_index);
		ptr = GetSubBlockDataPtr(block, start_index, end_index);
	}

#if defined(VD_DEBUG_GS2)
	vdLogInfo("Reading block [%d = %s] (raw %d)...", 
		Block::ToInteger(block), 
		Block::ToString(block), 
		(vd::i32)block);
#endif

	SkipToNextBlock(fd);
	for(vd::i32 k = 0; k < (vd::i32)ParticleType::Count; k++)
	{
		size_t load = (types && types[k]) ? 1 : 0;
		size_t end = m_MetaData.ParticleCountTotal[k];
		if(end_index && end_index <= end)
			end = end_index;

		size_t start_range = load * GetBlockEntrySize(block) * start_index;
		size_t end_range = load * GetBlockEntrySize(block) * end_index;
		if(end_range < 1)
			continue;

		ptr = Memory::Increment(ptr, dx);

		if(start_index)
			rd += fseek(fd, start_range, SEEK_CUR);

		if(types == NULL)
			rd += fread(ptr, end_range, 1, fd);
		else if(types[k] > 0)
			rd += fread(ptr, end_range, 1, fd);
		else
			rd += fseek(fd, end_range, SEEK_CUR);

		dx += end_range;
	}	
	SkipToNextBlock(fd);	
	return rd;
}

size_t
Snapshot::ReadPositionData(
	FILE* fd, 
	size_t start_index,
	size_t end_index,
	vd::i32* types)
{
	return ReadSubBlockData(fd, Block::Position, start_index, end_index, types);
}

size_t
Snapshot::ReadVelocityData(
	FILE* fd, 
	size_t start_index,
	size_t end_index,
	vd::i32* types)
{
	return ReadSubBlockData(fd, Block::Velocity, start_index, end_index, types);
}

size_t
Snapshot::ReadIdentifierData(
	FILE* fd, 
	size_t start_index,
	size_t end_index,
	vd::i32* types)
{
	return ReadSubBlockData(fd, Block::Id, start_index, end_index, types);
}

size_t
Snapshot::ReadMassData(
	FILE* fd, 
	size_t start_index,
	size_t end_index,
	vd::i32* types)
{
	size_t rd = 0;
	vd::i64 pc = start_index;
	vd::i64 per_particle_mass = 0;
	for(vd::i32 pt = 0; pt < (vd::i32)ParticleType::Count; pt++)
	{
		if(m_MetaData.Mass[pt] <= 0.0f)
		{
			per_particle_mass += m_MetaData.ParticleCount[pt];
		}
		else
		{
			for(vd::i64 pn = start_index; pn < (vd::i64)end_index && pn < (vd::i64)m_MetaData.ParticleCount[pt]; pn++)
			{
				m_ParticleData.Mass[pc++] = m_MetaData.Mass[pt];
			}
		}
	}
	
	Block::Value block = Block::Mass;		
	if(per_particle_mass > 0)
	{
#if defined(VD_DEBUG_GS2)
		vdLogInfo("Reading block [%d = %s] (raw %d)...", 
			Block::ToInteger(block), 
			Block::ToString(block), 
			(vd::i32)block);
#endif
		rd = ReadSubBlockData(fd, block, start_index, end_index, types);
		return rd;
	}

	return rd;
}

size_t
Snapshot::ReadInternalEnergyData(
	FILE* fd, 
	size_t start_index,
	size_t end_index, 
	vd::i32* types)
{
	return ReadSubBlockData(fd, Block::InternalEnergy, start_index, end_index, types);
}

size_t
Snapshot::ReadDensityData(
	FILE* fd, 
	size_t start_index,
	size_t end_index, 
	vd::i32* types)
{
	return ReadSubBlockData(fd, Block::Density, start_index, end_index, types);
}

size_t
Snapshot::ReadSmoothingLengthData(
	FILE* fd, 
	size_t start_index,
	size_t end_index, 
	vd::i32* types)
{
	return ReadSubBlockData(fd, Block::SmoothingLength, start_index, end_index, types);
}

size_t
Snapshot::ReadPotentialData(
	FILE* fd, 
	size_t start_index,
	size_t end_index, 
	vd::i32* types)
{
	return ReadSubBlockData(fd, Block::Potential, start_index, end_index, types);
}

size_t
Snapshot::ReadAcceleration(
	FILE* fd, 
	size_t start_index,
	size_t end_index, 
	vd::i32* types)
{
	return ReadSubBlockData(fd, Block::Acceleration, start_index, end_index, types);
}

size_t
Snapshot::ReadRateOfChangeOfEntropyData(
	FILE* fd, 
	size_t start_index,
	size_t end_index, 
	vd::i32* types)
{
	return ReadSubBlockData(fd, Block::DtEntropy, start_index, end_index, types);
}

size_t
Snapshot::ReadTimeStepData(
	FILE* fd, 
	size_t start_index,
	size_t end_index, 
	vd::i32* types)
{
	return ReadSubBlockData(fd, Block::TimeStep, start_index, end_index, types);

/*
	Block::Value block = Block::TimeStep;

	void* ptr = GetSubBlockDataPtr(block, start_index, end_index);
	if(ptr == NULL)
	{
		CreateSubBlockData(block, start_index, end_index);
		ptr = GetSubBlockDataPtr(block, start_index, end_index);
	}

	vd::u64 pc = 0;
	for(vd::i32 k = 0; k < (vd::i32)ParticleType::Count; k++)
	{
		size_t load = (types && types[k]) ? 1 : 0;
		size_t bytes = load * GetBlockEntrySize(block) * m_MetaData.ParticleCountTotal[k];
		if(bytes < 1)
			continue;

		for(vd::i32 n = 0; n < m_MetaData.ParticleCount[k]; n++)
		{
			m_ParticleData.TimeStep[pc++] = m_MetaData.Time;
		}
	}
	
	size_t dx = 0;
	size_t rd = 0;
	SkipToNextBlock(fd);
	for(vd::i32 k = 0; k < (vd::i32)ParticleType::Count; k++)
	{
		size_t load = (types && types[k]) ? 1 : 0;
		size_t bytes = load * GetBlockEntrySize(block) * m_MetaData.ParticleCountTotal[k];
		if(bytes < 1)
			continue;

		ptr = Memory::Increment(ptr, dx);

		if(types == NULL)
			rd += fread(ptr, bytes, 1, fd);
		else if(types[k] > 0)
			rd += fread(ptr, bytes, 1, fd);
		else
			rd += fseek(fd, bytes, SEEK_CUR);

		dx += bytes;
	}	
	SkipToNextBlock(fd);	
	return rd;
*/
}

size_t
Snapshot::SkipSubBlock(
	FILE* fd, 
	Block::Value block, 
	size_t start_index,
	size_t end_index)
{
	size_t offset = 0;
	SkipToNextBlock(fd);
	for(vd::i32 pt = 0; pt < (vd::i32)ParticleType::Count; pt++)
	{
		offset += GetBlockEntrySize(block) * m_MetaData.ParticleCount[pt];
	}
	fseek(fd, offset, SEEK_CUR);
	SkipToNextBlock(fd);

//	offset += GetBlockSeparatorSize();
	return offset;
}

size_t
Snapshot::ReadSubBlock(
	FILE* fd, 
	Block::Value block, 
	size_t start_index,
	size_t end_index, 
	vd::i32* types)
{
	switch (block)
	{
		case Block::Position:		
			return ReadPositionData(fd, start_index, end_index, types);
		case Block::Velocity:				
			return ReadVelocityData(fd, start_index, end_index, types);
		case Block::Id:
			return ReadIdentifierData(fd, start_index, end_index, types);
		case Block::Mass:
			return ReadMassData(fd, start_index, end_index, types);
		case Block::InternalEnergy:				
			return ReadInternalEnergyData(fd, start_index, end_index, types);
		case Block::Density:				
			return ReadDensityData(fd, start_index, end_index, types);
		case Block::SmoothingLength:			
			return ReadSmoothingLengthData(fd, start_index, end_index, types);
		case Block::Potential:				
			return ReadPotentialData(fd, start_index, end_index, types);
		case Block::Acceleration:			
			return ReadAcceleration(fd, start_index, end_index, types);
		case Block::DtEntropy:			
			return ReadRateOfChangeOfEntropyData(fd, start_index, end_index, types);
		case Block::TimeStep:			
			return ReadTimeStepData(fd, start_index, end_index, types);
		default:						
			return 0;
	}
	return 0;
}

vd::f32
Snapshot::Periodic(
	vd::f32 x, vd::f32 l2)
{
    if(x > +l2)
		x -= l2 * 2.0f;
    if(x < -l2)
		x += l2 * 2.0f;
    return x;
}

void
Snapshot::SwapFloatDataAt(
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
Snapshot::SwapIntDataAt(
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
Snapshot::SwapFloat3DataAt(
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
Snapshot::SwapParticleAt(
    int isrc, int idst)
{
	if(m_ParticleData.Position) 			SwapFloat3DataAt(m_ParticleData.Position, isrc, idst);
	if(m_ParticleData.Velocity) 			SwapFloat3DataAt(m_ParticleData.Velocity, isrc, idst);
	if(m_ParticleData.Acceleration)			SwapFloat3DataAt(m_ParticleData.Acceleration, isrc, idst);
	if(m_ParticleData.Mass)					SwapFloatDataAt(m_ParticleData.Mass, isrc, idst);
	if(m_ParticleData.InternalEnergy)		SwapFloatDataAt(m_ParticleData.InternalEnergy, isrc, idst);
	if(m_ParticleData.Density)				SwapFloatDataAt(m_ParticleData.Density, isrc, idst);
	if(m_ParticleData.SmoothingLength)		SwapFloatDataAt(m_ParticleData.SmoothingLength, isrc, idst);
	if(m_ParticleData.Potential)			SwapFloatDataAt(m_ParticleData.Potential, isrc, idst);
	if(m_ParticleData.DtEntropy)			SwapFloatDataAt(m_ParticleData.DtEntropy, isrc, idst);
	if(m_ParticleData.Type)					SwapIntDataAt(m_ParticleData.Type, isrc, idst);
	if(m_ParticleData.Id)					SwapIntDataAt(m_ParticleData.Id, isrc, idst);	
}

size_t 
Snapshot::ReadHeader(
	FILE* fd, vd::i32 splits, 
	vd::i32* req_types)
{
	SkipToNextBlock(fd);
	size_t rd = fread(&(m_MetaData), sizeof(m_MetaData), 1, fd);
	SkipToNextBlock(fd);

	if(splits <= 1)
	{
		m_FilteredParticleCount = 0;
		m_TotalParticleCount = 0;
		m_GasParticleCount = 0;
	}

	for(vd::i32 k = 0; k < (vd::i32)ParticleType::Count; k++)
		m_TotalParticleCount += m_MetaData.ParticleCountTotal[k];

	bool filter_types = false;
	for(vd::i32 k = 0; k < (vd::i32)ParticleType::Count; k++)
	{
		if(req_types && req_types[k] > 0)
			filter_types = true;
	}

	if(filter_types)
	{
		for(vd::i32 k = 0; k < (vd::i32)ParticleType::Count; k++)
		{
			if(req_types && req_types[k] > 0)
				m_FilteredParticleCount += m_MetaData.ParticleCountTotal[k];
		}
	}
	else
	{
		m_FilteredParticleCount += m_TotalParticleCount;
	}

	m_GasParticleCount += m_MetaData.ParticleCountTotal[0];
	return rd;
}

void
Snapshot::ComputeTempFromInternalEnergy(
	size_t start_index, size_t end_index)
{
    // physical constants in cgs units
//    static const vd::f64 GravityCGS = 6.672e-8;
    static const vd::f64 BoltzmannCGS = 1.3806e-16;
    static const vd::f64 ProtonMassCGS = 1.6726e-24;

    // internal unit system of the code 
    vd::f64 unit_length_in_cm = 3.085678e21;			//  code length unit in cm/h 
    vd::f64 unit_mass_in_g = 1.989e43;				//  code mass unit in g/h
    vd::f64 unit_velocity_in_cm_per_s = 1.0e5;

    vd::f64 unit_time_in_s = unit_length_in_cm / unit_velocity_in_cm_per_s;
    vd::f64 unit_energy_in_cgs = unit_mass_in_g * pow(unit_length_in_cm, 2) / pow(unit_time_in_s, 2);

//    vd::f64 unit_density_in_cgs = unit_mass_in_g / pow(unit_length_in_cm, 3);
//    vd::f64 unit_pressure_in_cgs = unit_mass_in_g / unit_length_in_cm / pow(unit_time_in_s, 2);
//    vd::f64 gravity = GravityCGS / pow(unit_length_in_cm, 3) * unit_mass_in_g * pow(unit_time_in_s, 2);
//    vd::f64 hubble = 0.65;			// hubble constant param

	vd::f64 Ne = 1.0;
    vd::f64 Xh = 0.76;				// mass fraction of hydrogen

	if(!m_ParticleData.InternalEnergy)
		return;

	Snapshot::Block::Value block = Snapshot::Block::Temperature;
	void* ptr = GetSubBlockDataPtr(block, start_index, end_index);
	if(ptr == NULL)
		CreateSubBlockData(block, start_index, end_index);
		
	ptr = GetSubBlockDataPtr(block, start_index, end_index);
	if(ptr == NULL)
		return;

    for(vd::i64 i = 0; i < (vd::i64)GetFilteredParticleCount(start_index, end_index); i++)
    {
        if(m_ParticleData.Type[i] == ParticleType::Gas)
        {
            vd::f64 mean_weight = 4.0 / (3 * Xh + 1 + 4 * Xh * Ne) * ProtonMassCGS;

            // Convert internal energy to cgs units
            vd::f64 u = m_ParticleData.InternalEnergy[i] * unit_energy_in_cgs / unit_mass_in_g;
            vd::f64 gamma = 5.0 / 3;

            // Convert temperature to Kelvin
            m_ParticleData.Temp[i] = mean_weight / BoltzmannCGS * (gamma - 1) * u;
        }
    }
}

vd::string
Snapshot::GetFilename(
	const vd::string& prefix, 
	vd::i32 index,
	vd::i32 padding)
{
	return m_DataSet->GetFilenameForSnapshot(prefix, index, padding);
}

vd::status
Snapshot::Load(
	const vd::string& prefix, 
	vd::i32 index,
	vd::i32 start_index,
	vd::i32 end_index,
	vd::i32 split_count,
	vd::i32 padding,
	vd::i32* req_data,
	vd::i32* req_types)
{
	vdAssert(prefix.size() > 1);    

	vd::i32 pc = 1;
	vd::i32 total_pc = pc;

    for(vd::i32 i = 0; i < split_count; i++)
    {
	    vd::string filename = m_DataSet->GetFilenameForSnapshot(prefix, index, padding);
    	if(split_count > 1) filename += vd::string(".") + Core::Convert::ToString(i, 0, 0);

	    FILE* fd = fopen(filename.c_str(), "r");	    
	    if(fd == NULL)
        {
            vdLogWarning("Failed to open snapshot file '%s'!", filename.c_str());
			return Status::Code::ReadError;
        }

#if defined(VD_DEBUG_GS2)
        vdLogDebug("Reading snapshot '%s' ...", filename.c_str());
#endif
        m_SplitStartIndex = m_TotalParticleCount;

		if(!ReadHeader(fd, split_count, req_types))
		{
            vdLogWarning("Failed to open snapshot file '%s'!", filename.c_str());
			return Status::Code::ReadError;
        }

        m_SplitEndIndex = m_TotalParticleCount;
	}

    for(vd::i32 i = 0; i < split_count; i++)
    {
	    vd::string filename = m_DataSet->GetFilenameForSnapshot(prefix, index, padding);
    	if(split_count > 1) filename += vd::string(".") + Core::Convert::ToString(i, 0, 0);

	    FILE* fd = fopen(filename.c_str(), "r");	    
	    if(fd == NULL)
        {
            vdLogWarning("Failed to open snapshot file '%s'!", filename.c_str());
			return Status::Code::ReadError;
        }

#if defined(VD_DEBUG_GS2)
        vdLogDebug("Reading snapshot '%s' ...", filename.c_str());
#endif

		if(!ReadHeader(fd, split_count, req_types))
		{
            vdLogWarning("Failed to open snapshot file '%s'!", filename.c_str());
			return Status::Code::ReadError;
        }

#if defined(VD_DEBUG_GS2)
	    vdLogInfo("Reading block 'ParticleType' from '%s'!", filename.c_str());
#endif
		ReadTypeData(fd, start_index, end_index);

	#if defined(VD_DEBUG_GS2)
		    vdLogInfo("Loading '%d' / '%d' particles!", 
		    	m_FilteredParticleCount, m_TotalParticleCount);
	#endif

		for(vd::u32 n = 0; n < Block::Count; n++)
		{
			Snapshot::Block::Value block = Snapshot::Block::FromInteger(n);
			if(req_data[n] > 0)
			{
#if defined(VD_DEBUG_GS2)
	            vdLogInfo("Reading block '%s' from '%s'!", 
	            	Snapshot::Block::ToString(block), 
	            	filename.c_str());
#endif
				ReadSubBlock(fd, block, start_index, end_index, req_types);
 			}
			else
            {
				SkipSubBlock(fd, block, start_index, end_index);

            }
        }
        
        fclose(fd);
        total_pc += pc;
    }

    // Compute derived quantity for Temperature from Internal Energy if it wasn't in the file
    if(req_data[Snapshot::Block::Temperature] == VD_TRUE && 
    	GetSubBlockDataPtr(Snapshot::Block::Temperature, start_index, end_index) == NULL)
    {
    	ComputeTempFromInternalEnergy(start_index, end_index);
    }

#if defined(VD_DEBUG_GS2)
    vdLogInfo("Loaded '%d'/'%d' particles!", m_FilteredParticleCount, m_TotalParticleCount);
//    vdLogInfo("Filtering by type for '%s'!", filename.c_str());
#endif

    m_FileIndex = index;
    return 1;
}

void 
Snapshot::Reorder()
{
/*	
    vd::i64 idst;
    for(vd::i64 i = 0; i < (vd::i64)GetFilteredParticleCount(start_index, end_index); i++)
    	m_ParticleData.Id[i]--;
        	
    for(vd::i64 i = 0; i < (vd::i64)GetFilteredParticleCount(start_index, end_index); i++)
    {
        if(m_ParticleData.Id[i] != i)
        {
            idst = m_ParticleData.Id[i];
            if(idst > (vd::i64)GetFilteredParticleCount(start_index, end_index))
                continue;

            do
            {
                SwapParticleAt(idst, i);
                if(idst == i)
                    break;

                idst = m_ParticleData.Id[i];
            }
            while(1);
        }
    }
    
    for(vd::i64 i = 0; i < (vd::i64)GetFilteredParticleCount(start_index, end_index); i++)
    	m_ParticleData.Id[i]++;
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
    m_CacheSize(2*1024),
    m_Runtime(runtime)
{
	Memory::SetBytes(m_RequestedTypes,  0, sizeof(m_RequestedTypes));
	Memory::SetBytes(m_RequestedBlocks, 0, sizeof(m_RequestedBlocks));
	Memory::SetBytes(m_RequestedStatistics, 0, sizeof(m_RequestedStatistics));
}

Dataset::~Dataset() 
{ 
	Destroy();
}

vd::status
Dataset::Destroy(void)
{
	Close();
	m_DataCache.Destroy();
	return Status::Code::Success;
}

vd::status 
Dataset::Open(
	const vd::string& prefix, 
	vd::i32 file_count, 
	vd::i32 split_count)
{
	m_Mutex.Lock();

	m_FileNumberPadding = 0;
	vd::i32 length = (vd::i32)prefix.size();
	for(vd::i32 i = length-1; i >= 0 && isdigit(prefix[i]); --i)
		m_FileNumberPadding++;
		
	vd::string digits = prefix.substr(length - m_FileNumberPadding, length);
	m_FilePrefix = prefix.substr(0, length - m_FileNumberPadding);
	m_StartFileIndex = atoi(digits.c_str());
	if(file_count < 1)
	{
		int missed = 0;
		Core::FileSystem* fs = m_Runtime->GetFileSystem();
		int i = m_StartFileIndex;
		while(missed < 10)
		{
			if(fs->Exists(GetFilenameForSnapshot(m_FilePrefix, i, m_FileNumberPadding)) == true)
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
		m_EndFileIndex = m_StartFileIndex + file_count;
	}

	m_FileSplits = split_count;
	m_IsOpen = true;
	
	vdLogInfo("Opening dataset '%s' from '%d-%d' with padded index '%d' ...",
		m_FilePrefix.c_str(), m_StartFileIndex, m_EndFileIndex, m_FileNumberPadding);
	
	m_DataCache.Setup(2 * Constants::MemorySize::GB, 
		VD_BIND_MEMBER_FUNCTION(this, &Dataset::OnFetch), 
		VD_BIND_MEMBER_FUNCTION(this, &Dataset::OnEvict)
	);

	m_WorkCache.Setup(m_EndFileIndex - m_StartFileIndex + 1, 
		VD_BIND_MEMBER_FUNCTION(this, &Dataset::OnSubmit), 
		VD_BIND_MEMBER_FUNCTION(this, &Dataset::OnComplete)
	);
	
	m_WorkQueue.Start(1);	
	m_CurrentFileIndex = m_StartFileIndex;
	m_Mutex.Unlock();

	return Status::Code::Success;
}
	
vd::string
Dataset::GetFilenameForSnapshot(
    const vd::string& prefix, vd::i32 index, vd::i32 padding)
{
	char digits[1024] = {0};
	Core::FileSystem* fs = m_Runtime->GetFileSystem();

	snprintf(digits, sizeof(digits), "%d", index);
	vd::i32 digit_len = (vd::i32) std::strlen(digits);

	for(vd::i32 i = digit_len; i < digit_len + 8; i++)
	{		
		vd::string result = prefix;
		for(vd::i32 p = 0; p < i - digit_len; p++)
			result += vd::string("0");
		
		result += vd::string(digits);
		if(fs->Exists(result) == true)
			return result;
	}

	return vd::string();
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
Dataset::IsReady(
	DataRequest request)
{
	return IsResident(request);
}

bool
Dataset::IsPending(
	DataRequest request)
{
	return m_WorkCache.IsResident(request);
}

bool
Dataset::IsResident(
	DataRequest request)
{
	return m_DataCache.IsResident(request);
}

WorkItem*
Dataset::GetPendingWorkItem(
	DataRequest request)
{
	WorkItem* work = NULL;
	return work;
}

Snapshot*
Dataset::Retrieve(
	DataRequest request)
{
	if(IsValidRequest(request) == false)
		return NULL;
	
	Snapshot* data = NULL;
	bool hit = m_DataCache.Fetch(request, data);
	hit = hit && data && data->IsResident();

	vdLogInfo("Fetching snapshot data '%d' %s [%d -> %d]...", 
		request.SnapshotIndex,
		Snapshot::Block::ToString(request.BlockType),
		request.StartIndex,
		request.EndIndex);

//	vdLogInfo("Fetching snapshot: Index[%d] Hit[%s] ...", index, hit ? "true" : "false");
	return hit ? data : NULL;
}
  
bool
Dataset::Request(
	DataRequest request)
{
	if(IsValidRequest(request) == false)
		return false;

	bool hit = m_DataCache.Load(request);

#if defined(VD_DEBUG_GS2)
	vdLogInfo("Requesting snapshot data '%d' %s [%d -> %d]...", 
		request.SnapshotIndex,
		Snapshot::Block::ToString(request.BlockType),
		request.StartIndex,
		request.EndIndex);
#endif

	return hit;
}

Snapshot*
Dataset::OnFetch(
	const DataRequest& request)
{
	if(IsValidRequest(request) == false)
		return NULL;
		
#if defined(VD_DEBUG_GS2)
	vdLogInfo("Miss for snapshot '%d' %s [%d -> %d]...", 
		request.SnapshotIndex,
		Snapshot::Block::ToString(request.BlockType),
		request.StartIndex,
		request.EndIndex);
#endif

	Snapshot* snapshot = NULL; 
	WorkItem* work = NULL;

	bool hit = m_WorkCache.Fetch(request, work);
	if(hit && work && work->IsReady())
	{
#if defined(VD_DEBUG_GS2)
		vdLogInfo("OnFetch() for snapshot '%d' %s [%d -> %d]...", 
			request.SnapshotIndex,
			Snapshot::Block::ToString(request.BlockType),
			request.StartIndex,
			request.EndIndex);
#endif
		snapshot = work->GetSnapshot();
	}

	return snapshot;
}

void 
Dataset::OnEvict(
	Snapshot* snapshot)
{
	if(snapshot != NULL)
		vdLogInfo("Evicting snapshot '%d' ...", snapshot->GetFileIndex());

	VD_DELETE(snapshot);
}

WorkItem*
Dataset::OnSubmit(
	const DataRequest& request)
{
	if(IsValidRequest(request) == false)
		return NULL;
		
#if defined(VD_DEBUG_GS2)
	vdLogInfo("Adding load request for snapshot '%d' %s [%d -> %d]...", 
		request.SnapshotIndex,
		Snapshot::Block::ToString(request.BlockType),
		request.StartIndex,
		request.EndIndex);
#endif

	Snapshot* snapshot = VD_NEW(Snapshot, this);

	snapshot->SetResident(false);

	WorkItem* work = VD_NEW(WorkItem, 
		request,
		snapshot, 
		m_FilePrefix, 
		request.SnapshotIndex, 
		m_FileSplits, 
		m_FileNumberPadding, 
		m_RequestedBlocks, 
		m_RequestedTypes,
		m_RequestedStatistics);															  

	m_WorkQueue.Submit(work);
	m_DataCache.Insert(request, snapshot);
	return work;
}

void 
Dataset::OnComplete(
	WorkItem* work)
{
#if defined(VD_DEBUG_GS2)
	if(work != NULL)
		vdLogInfo("Snapshot '%d' loaded!", work->GetFileIndex());
#endif

	VD_DELETE(work);
}

void
Dataset::Release(
	DataRequest request)
{
	m_WorkCache.Remove(request);
	m_DataCache.Remove(request);
}

void 
Dataset::Retain(
	DataRequest request, Snapshot* snapshot)
{
	m_DataCache.Touch(request);
}

void Dataset::SetBlockRequest(
	Snapshot::Block::Value v, vd::i32 enable) 
{ 
	m_RequestedBlocks[Snapshot::Block::ToInteger(v)] = enable; 
}

void Dataset::SetTypeRequest(
	Snapshot::ParticleType::Value v, vd::i32 enable) 
{ 
	m_RequestedTypes[Snapshot::ParticleType::ToInteger(v)] = enable; 
}

void Dataset::SetStatisticRequest(Snapshot::Block::Value v, vd::i32 enable) 
{ 
	m_RequestedStatistics[Snapshot::Block::ToInteger(v)] = enable; 
}

vd::i32 Dataset::GetFileCount() const 
{ 
	return m_FileCount; 
}

vd::string Dataset::GetFilePrefix() const 
{ 
	return m_FilePrefix; 
}

vd::i32 Dataset::GetStartFileIndex() const 
{ 
	return m_StartFileIndex; 
}

vd::i32 Dataset::GetEndFileIndex() const 
{ 
	return m_EndFileIndex; 
}

vd::i32 Dataset::GetCurrentFileIndex() const 
{ 
	return m_CurrentFileIndex; 
}

void Dataset::SetCurrentFileIndex(vd::i32 index) 
{ 
	m_CurrentFileIndex = index; 
}

bool Dataset::IsOpen() const 
{ 
	return m_IsOpen; 
}

bool Dataset::IsValidIndex(
	vd::i32 index) const
{ 
	return (index >= m_StartFileIndex && index <= m_EndFileIndex); 
}

bool Dataset::IsValidBlock(
	Snapshot::Block::Value v) const 
{ 
	return v != Snapshot::Block::LastValue; 
}

bool Dataset::IsValidParticleType(
	Snapshot::ParticleType::Value v) const 
{ 
	return v != Snapshot::ParticleType::LastValue; 
}

bool Dataset::IsValidRequest(
	DataRequest request) const
{
	if(IsValidIndex(request.SnapshotIndex) == false)
		return false;

	if(IsValidBlock(request.BlockType) == false)
		return false;

	if(request.StartIndex >= request.EndIndex)
		return false;

	return true;
}

void
Dataset::Evict()
{

}

void
WorkQueue::OnRun(
	WorkItem* item)
{
	WorkItem* work = (WorkItem*)item;
    if(work == NULL)
        return;

    Snapshot* snapshot = work->m_Snapshot;

#if defined(VD_DEBUG_GS2)
	vd::f64 t0 = Process::GetTimeInSeconds();
	vdLogInfo("[%d] %s Index[%d] Splits[%d] Padding[%d]", 
		(int)work->GetSlotId(), work->m_FilePrefix.c_str(), 
		work->m_FileIndex, work->m_FileSplits, work->m_FileNumberPadding);
#endif
		
	if(work->IsAborted())
	{
//		VD_DELETE(work);
		return;
	}	
	
	snapshot->Load(
		work->m_FilePrefix, 
		work->m_FileIndex, 
		work->m_Request.StartIndex,
		work->m_Request.EndIndex,
		work->m_FileSplits, 
		work->m_FileNumberPadding, 
		work->m_Blocks, 
		work->m_Types);

	vd::string filename = snapshot->GetFilename(work->m_FilePrefix, 
												work->m_FileIndex, 
												work->m_FileNumberPadding);

#if defined(VD_DEBUG_GS2)
	MetaData m = snapshot->GetMetaData();
	vd::f64 t1 = Process::GetTimeInSeconds();
	vdLogInfo("Loaded '%s' in '%f' sec.", filename.c_str(), t1 - t0);
	vdLogInfo("-- '%s' = [%s=%d %s=%d %s=%d %s=%d %s=%d %s=%d]", "ParticleCount", 
		Snapshot::ParticleType::ToString(0),
		m.ParticleCount[0], 
		Snapshot::ParticleType::ToString(1),
		m.ParticleCount[1],
		Snapshot::ParticleType::ToString(2),
		m.ParticleCount[2], 
		Snapshot::ParticleType::ToString(3),
		m.ParticleCount[3], 
		Snapshot::ParticleType::ToString(4),
		m.ParticleCount[4], 
		Snapshot::ParticleType::ToString(5),
		m.ParticleCount[5]);
	vdLogInfo("-- '%s' = [%d %d %d %d %d %d]", "ParticleCountTotal", 
		m.ParticleCountTotal[0], m.ParticleCountTotal[1], m.ParticleCountTotal[2], 
		m.ParticleCountTotal[3], m.ParticleCountTotal[4], m.ParticleCountTotal[5]);
	vdLogInfo("-- '%s' = [%f %f %f %f %f %f]", "Mass", 
		m.Mass[0], m.Mass[1], m.Mass[2], 
		m.Mass[3], m.Mass[4], m.Mass[5]);
	vdLogInfo("-- '%s' = %s", "HasStarFormation", m.HasStarFormation ? "true" : "false");
	vdLogInfo("-- '%s' = %s", "HasFeedback", m.HasFeedback ? "true" : "false");
	vdLogInfo("-- '%s' = %s", "HasStellarAge", m.HasStellarAge ? "true" : "false");
	vdLogInfo("-- '%s' = %s", "HasMetals", m.HasMetals ? "true" : "false");
	vdLogInfo("-- '%s' = %s", "HasEntropyInsteadofU", m.HasEntropyInsteadofU ? "true" : "false");
	vdLogInfo("-- '%s' = %s", "HasCooling", m.HasCooling ? "true" : "false");
	vdLogInfo("-- '%s' = %f", "Redshift", m.Redshift);
	vdLogInfo("-- '%s' = %f", "BoxSize", m.BoxSize);
	vdLogInfo("-- '%s' = %f", "Omega0", m.Omega0);
	vdLogInfo("-- '%s' = %f", "OmegaLambda", m.OmegaLambda);
	vdLogInfo("-- '%s' = %f", "HubbleParam", m.HubbleParam);
#endif
	
	size_t start_index = work->m_Request.StartIndex;
	size_t end_index = work->m_Request.EndIndex;
	for(vd::i32 i = 0; i < (vd::i32)Snapshot::Block::Count; i++)
	{
		Snapshot::Block::Value block = Snapshot::Block::FromInteger(i);
		if(work->m_Stats[i] < 1)
			continue;

		vd::f64 t0 = Process::GetTimeInSeconds();
		Statistic& s = snapshot->GetStatistic(block);
		s.Components = snapshot->GetBlockVectorLength(block);
		if(snapshot->IsBlockEntryIntegerValue(block))
		{
			int* src = (int*)snapshot->GetSubBlockDataPtr(block, start_index, end_index);
			if(src == NULL)
				continue;

			for(vd::i8 vc = 0; vc < s.Components; vc++)
			{
				s.Minimum[vc] = +VD_F32_MAX;
				s.Maximum[vc] = -VD_F32_MAX;
			}

			float* dst = (float*)snapshot->GetSubBlockDataPtr(block, start_index, end_index);
			for(vd::u64 n = 0; n < snapshot->GetFilteredParticleCount(start_index, end_index); n++)
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
			float* src = (float*)snapshot->GetSubBlockDataPtr(block, start_index, end_index);
			if(src == NULL)
				continue;

			for(vd::i8 vc = 0; vc < s.Components; vc++)
			{
				s.Minimum[vc] = +VD_F32_MAX;
				s.Maximum[vc] = -VD_F32_MAX;
			}

			for(vd::u64 n = 0; n < snapshot->GetFilteredParticleCount(start_index, end_index); n++)
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
			Snapshot::Block::ToString(i), t1 - t0);

		for(vd::i8 vc = 0; vc < s.Components; vc++)
		{
			vdLogInfo("-- Min[%f] Max[%f] Sum[%f] Var[%f]", 
				s.Minimum[vc], s.Maximum[vc], s.TotalSum[vc], s.Variance[vc]);
		}

		snapshot->SetResident(true);
	}
//	VD_DELETE(work);
}

// ============================================================================================== //

VD_IMPLEMENT_OBJECT(Snapshot, vd_sym(GadgetSnapshot), vd_sym(Object));
VD_IMPLEMENT_OBJECT(Dataset, vd_sym(GadgetDataset), vd_sym(Object));
VD_IMPLEMENT_OBJECT(WorkQueue, vd_sym(GadgetWorkQueue), vd_sym(WorkQueue));
VD_IMPLEMENT_OBJECT(WorkItem, vd_sym(GadgetWorkItem), vd_sym(WorkItem));

// ============================================================================================== //

} // end namespace: Gadget

// ============================================================================================== //

VD_FORMATS_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_USE_GS2

// ============================================================================================== //

