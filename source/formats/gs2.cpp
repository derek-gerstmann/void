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

#include "formats/gs2.h"
#include "core/asserts.h"
#include "core/utilities.h"
#include "core/process.h"
#include "constants/constants.h"

#include <string.h>
#include <math.h>

// ============================================================================================== //

VD_FORMATS_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_USING(Core, Thread);
VD_USING(Core, Memory);
VD_USING(Core, Process);

// ============================================================================================== //

namespace Gadget {

// ============================================================================================== //

class InputThread : public Core::Thread
{

};

DataFile::DataFile() :
	Core::Object(),
	m_TotalParticleCount(0),
	m_GasParticleCount(0),
	m_FileIndex(0)
{
    Memory::SetBytes(&m_MetaData, 		0, sizeof(m_MetaData));
    Memory::SetBytes(&m_ParticleData, 	0, sizeof(m_ParticleData));
    Memory::SetBytes(&m_SummaryData,	0, sizeof(m_SummaryData));
    Memory::SetBytes(&m_BlockOffsets,	0, sizeof(m_BlockOffsets));
}
 
DataFile::~DataFile()
{
	Destroy();
}

vd::bytesize
DataFile::GetResidentMemorySize() const
{
	vd::bytesize bytes = 0;
	for(vd::u32 i = 0; i < Block::Count; i++)
	{
		DataFile::Block::Value block = DataFile::Block::FromInteger(i);
		const char* ptr = (char*)GetBlockDataPtr(block);
		if(ptr != NULL)
		{
			bytes += GetBlockEntrySize(block) * m_TotalParticleCount;
		}
	}
	return bytes;
}

vd::status 
DataFile::Destroy()
{
	vdLogInfo("Destroying DataFile '%d' ... ", m_FileIndex);
	
	for(vd::u32 i = 0; i < Block::Count; i++)
	{
		DataFile::Block::Value block = DataFile::Block::FromInteger(i);
		DestroyBlockData(block);
	}
	return Status::Code::Success;
}

vd::bytesize
DataFile::GetBlockSize(
	Block::Value block) const
{
	return GetBlockEntrySize(block) * m_TotalParticleCount;
}

vd::bytesize
DataFile::CreateBlockData(
	Block::Value block)
{
	char* ptr = (char*)GetBlockDataPtr(block);
	vd::bytesize bytes = GetBlockEntrySize(block) * m_TotalParticleCount;
	if(ptr == NULL)
	{
		ptr = VD_NEW_ARRAY(char, bytes);
		if(ptr == NULL)
		{
			vdLogError("Failed to allocate memory for snapshot block '%s' data!", 
				GetBlockName(block));

			return 0;
		}
		Memory::SetBytes(ptr, 0, bytes);
	}
	
	SetBlockDataPtr(block, ptr);
	return bytes;
}

void
DataFile::DestroyBlockData(
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
DataFile::GetBlockDataPtr(
	Block::Value block) const
{		
	switch (block)
	{
		case Block::Position:			{ return m_ParticleData.Position; }
		case Block::Velocity:			{ return m_ParticleData.Velocity; }
		case Block::Acceleration:		{ return m_ParticleData.Acceleration; }
		case Block::Id:					{ return m_ParticleData.Id; }
		case Block::Type:				{ return m_ParticleData.Type; }
		case Block::Mass:				{ return m_ParticleData.Mass; }
		case Block::InternalEnergy:		{ return m_ParticleData.InternalEnergy; }
		case Block::Density:			{ return m_ParticleData.Density; }
		case Block::SmoothingLength:	{ return m_ParticleData.SmoothingLength; }
		case Block::Potential:			{ return m_ParticleData.Potential; }
		case Block::DtEntropy:			{ return m_ParticleData.DtEntropy; }
		case Block::TimeStep:			{ return m_ParticleData.TimeStep; }
		default:						{ return NULL; }
	}
	return NULL;
}

void
DataFile::SetBlockDataPtr(
	Block::Value block, void* ptr) 
{
	switch (block)
	{
		case Block::Position:			{ m_ParticleData.Position = (float*)ptr; break; }
		case Block::Velocity:			{ m_ParticleData.Velocity = (float*)ptr; break; }
		case Block::Acceleration:		{ m_ParticleData.Acceleration = (float*)ptr; break;	}
		case Block::Id:					{ m_ParticleData.Id = (int*)ptr; break; }
		case Block::Type:				{ m_ParticleData.Type = (int*)ptr; break; }
		case Block::Mass:				{ m_ParticleData.Mass = (float*)ptr; break;	}
		case Block::InternalEnergy:		{ m_ParticleData.InternalEnergy = (float*)ptr; break; }
		case Block::Density:			{ m_ParticleData.Density = (float*)ptr; break; }
		case Block::SmoothingLength:	{ m_ParticleData.SmoothingLength = (float*)ptr; break; }
		case Block::Potential:			{ m_ParticleData.Potential = (float*)ptr; break; }
		case Block::DtEntropy:			{ m_ParticleData.DtEntropy = (float*)ptr; break; }
		case Block::TimeStep:			{ m_ParticleData.TimeStep = (float*)ptr; break; }
		default:						
			return;
	}
	return;
}

vd::bytesize
DataFile::ReadTypeData(
	FILE* fd, vd::bytesize offset)
{
	Block::Value block = Block::Type;
	
	void* ptr = GetBlockDataPtr(block);
	if(ptr == NULL)
		CreateBlockData(block);

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

vd::bytesize
DataFile::GetBlockSeparatorSize(void) const
{
	return vd::bytesize(sizeof(char) * 4); //	32-bit
}

vd::i32
DataFile::SkipToNextBlock(
	FILE* fh)
{
    vd::i32 dummy;
    fread(&dummy, sizeof(dummy), 1, fh);
	return dummy;
}

vd::bytesize
DataFile::FindBlockByName(
	FILE* fd, const char* label)
{
//	size_t rd = 0;
    int blocksize=0;
    char blocklabel[5]= {"    "};

    while(!feof(fd) && blocksize == 0)
    {
		fread(blocklabel, 4*sizeof(char), 1, fd);
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

vd::bytesize
DataFile::FindBlock(
	FILE* fd, Block::Value block)
{
	return FindBlockByName(fd, GetBlockLabel(block));
}

const char*
DataFile::GetBlockLabel(
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
DataFile::GetBlockName(
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

vd::bytesize
DataFile::GetBlockEntrySize(
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
DataFile::GetBlockVectorLength(
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
DataFile::GetBlockSizeTypeSuffix(
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
DataFile::IsBlockEntryIntegerValue(
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

BlockSummary& 
DataFile::GetBlockSummary(
	Block::Value v) 
{ 
    return m_SummaryData[Block::ToInteger(v)]; 
}

void 
DataFile::SetBlockSummary(
	Block::Value v, 
	const BlockSummary& summary) 
{
    m_SummaryData[Block::ToInteger(v)] = summary; 
}


vd::bytesize
DataFile::ReadBlockData(
	FILE* fd, 
	Block::Value block,
	vd::bytesize offset,
	vd::i32* types)
{
	size_t rd = 0;
	size_t dx = 0;
	char* ptr = (char*)GetBlockDataPtr(block);
	if(ptr == NULL)
		CreateBlockData(block);
		
	ptr = (char*)GetBlockDataPtr(block);
//	size_t bytes = GetBlockEntrySize(block) * m_TotalParticleCount;

	vdLogInfo("Reading block [%d = %s] (raw %d)...", Block::ToInteger(block), Block::ToString(block), (vd::i32)block);

	SkipToNextBlock(fd);
	for(vd::i32 k = 0; k < (vd::i32)ParticleType::Count; k++)
	{
		size_t bytes = GetBlockEntrySize(block) * m_MetaData.ParticleCountTotal[k];
		if(bytes < 1)
			continue;

		if(types == NULL)
			rd += fread(ptr+dx, bytes, 1, fd);
		else if(types[k] > 0)
			rd += fread(ptr+dx, bytes, 1, fd);
		else
			rd += fseek(fd, bytes, SEEK_CUR);
		dx += bytes;
	}	
	SkipToNextBlock(fd);	
	return rd;
}

vd::bytesize
DataFile::ReadPositionData(
	FILE* fd, vd::bytesize offset, vd::i32* types)
{
	return ReadBlockData(fd, Block::Position, offset, types);
}

vd::bytesize
DataFile::ReadVelocityData(
	FILE* fd, vd::bytesize offset, vd::i32* types)
{
	return ReadBlockData(fd, Block::Velocity, offset, types);
}

vd::bytesize
DataFile::ReadIdentifierData(
	FILE* fd, vd::bytesize offset, vd::i32* types)
{
	return ReadBlockData(fd, Block::Id, offset, types);
}

vd::bytesize
DataFile::ReadMassData(
	FILE* fd, vd::bytesize offset, vd::i32* types)
{
	size_t rd = 0;
	vd::i64 per_particle_mass = 0;

	Block::Value block = Block::Mass;

	char* ptr = (char*)GetBlockDataPtr(block);
	if(ptr == NULL)
		CreateBlockData(block);
		
	vd::i64 pc = 0;
	for(vd::i32 pt = 0; pt < (vd::i32)ParticleType::Count; pt++)
	{
		if(m_MetaData.Mass[pt] <= 0.0f)
		{
			per_particle_mass += m_MetaData.ParticleCount[pt];
		}
		else
		{
			for(vd::i64 pn = 0; pn < (vd::i64)m_MetaData.ParticleCount[pt]; pn++)
			{
				m_ParticleData.Mass[pc++] = m_MetaData.Mass[pt];
			}
		}
	}
	
	if(per_particle_mass > 0)
	{
		size_t dx = 0;
		ptr = (char*)GetBlockDataPtr(block);

		vdLogInfo("Reading block [%d = %s] (raw %d)...", Block::ToInteger(block), Block::ToString(block), (vd::i32)block);

		SkipToNextBlock(fd);
		for(vd::i32 k = 0; k < (vd::i32)ParticleType::Count; k++)
		{
			size_t bytes = GetBlockEntrySize(block) * m_MetaData.ParticleCountTotal[k];
			if(bytes < 1)
				continue;
			if(types == NULL)
				rd += fread(ptr+dx, bytes, 1, fd);
			else if(types[k] > 0)
				rd += fread(ptr+dx, bytes, 1, fd);
			else
				rd += fseek(fd, bytes, SEEK_CUR);
			dx += bytes;
		}	
		SkipToNextBlock(fd);	

//		SkipToNextBlock(fd);
//		rd = fread(m_ParticleData.Mass, bytes, 1, fd);
//		SkipToNextBlock(fd);	
		return rd;
	}

	return pc;
}

vd::bytesize
DataFile::ReadInternalEnergyData(
	FILE* fd, vd::bytesize offset, vd::i32* types)
{
	return ReadBlockData(fd, Block::InternalEnergy, offset, types);
}

vd::bytesize
DataFile::ReadDensityData(
	FILE* fd, vd::bytesize offset, vd::i32* types)
{
//	if(m_MetaData.ParticleCount[ParticleType::Gas] < 1)
//		return 0;
	
	return ReadBlockData(fd, Block::Density, offset, types);
}

vd::bytesize
DataFile::ReadSmoothingLengthData(
	FILE* fd, vd::bytesize offset, vd::i32* types)
{
//	if(m_MetaData.ParticleCount[ParticleType::Gas] < 1)
//		return 0;
	
	return ReadBlockData(fd, Block::SmoothingLength, offset, types);
}

vd::bytesize
DataFile::ReadPotentialData(
	FILE* fd, vd::bytesize offset, vd::i32* types)
{
	return ReadBlockData(fd, Block::Potential, offset, types);
}

vd::bytesize
DataFile::ReadAcceleration(
	FILE* fd, vd::bytesize offset, vd::i32* types)
{
	return ReadBlockData(fd, Block::Acceleration, offset, types);
}

vd::bytesize
DataFile::ReadRateOfChangeOfEntropyData(
	FILE* fd, vd::bytesize offset, vd::i32* types)
{
	return ReadBlockData(fd, Block::DtEntropy, offset, types);
}

vd::bytesize
DataFile::ReadTimeStepData(
	FILE* fd, vd::bytesize offset, vd::i32* types)
{
	Block::Value block = Block::TimeStep;

	void* ptr = GetBlockDataPtr(block);
	if(ptr == NULL)
		CreateBlockData(block);
			
	vd::u64 pc = 0;
	size_t bytes = GetBlockEntrySize(block) * m_TotalParticleCount;
	for(vd::i32 pt = 0; pt < (vd::i32)ParticleType::Count; pt++)
	{
		for(vd::i32 pn = 0; pn < m_MetaData.ParticleCount[pt]; pn++)
		{
			m_ParticleData.TimeStep[pc++] = m_MetaData.Time;
		}
	}
	
	SkipToNextBlock(fd);
	vd::bytesize rd = fread(m_ParticleData.TimeStep, bytes, 1, fd);
	SkipToNextBlock(fd);	
	return rd;
}

vd::bytesize
DataFile::SkipBlock(
	FILE* fd, Block::Value block, vd::bytesize start)
{
	size_t offset = 0;
	SkipToNextBlock(fd);
	for(vd::i32 pt = 0; pt < (vd::i32)ParticleType::Count; pt++)
	{
		offset += GetBlockEntrySize(block) * m_MetaData.ParticleCount[pt];
	}
	fseek(fd,offset,SEEK_CUR);
	SkipToNextBlock(fd);
	return offset;
}

vd::bytesize
DataFile::ReadBlock(
	FILE* fd, Block::Value block, vd::bytesize offset, vd::i32* types)
{
	switch (block)
	{
		case Block::Position:		
			return ReadPositionData(fd, offset, types);
		case Block::Velocity:				
			return ReadVelocityData(fd, offset, types);
		case Block::Id:
			return ReadIdentifierData(fd, offset, types);
		case Block::Mass:
			return ReadMassData(fd, offset, types);
		case Block::InternalEnergy:				
			return ReadInternalEnergyData(fd, offset, types);
		case Block::Density:				
			return ReadDensityData(fd, offset, types);
		case Block::SmoothingLength:			
			return ReadSmoothingLengthData(fd, offset, types);
		case Block::Potential:				
			return ReadPotentialData(fd, offset, types);
		case Block::Acceleration:			
			return ReadAcceleration(fd, offset, types);
		case Block::DtEntropy:			
			return ReadRateOfChangeOfEntropyData(fd, offset, types);
		case Block::TimeStep:			
			return ReadTimeStepData(fd, offset, types);
		default:						
			return 0;
	}
	return 0;
}

vd::bytesize
DataFile::ReadPartialBlock(
	FILE* fd, Block::Value block, 
    vd::bytesize start, vd::bytesize end, 
    vd::i32* types)
{
	size_t offset = 0;
#if 0
	fseek(fd, m_BlockOffsets[b], SEEK_SET);
	SkipToNextBlock(fd);

	for(vd::i32 k = 0; k < (vd::i32)ParticleType::Count; k++)
	{
		offset += fseek(fd, start, SEEK_CUR);

		size_t req_size = end - start;
		size_t sub_size = GetBlockEntrySize(block) * m_MetaData.ParticleCountTotal[k];
		if(sub_size < 1)
			continue;

		if(req_size > sub_size)
			req_size = sub_size;

		if(types == NULL)
			rd += fread(ptr+dx, req_size, 1, fd);
		else if(types[k] > 0)
			rd += fread(ptr+dx, req_size, 1, fd);
		else
			rd += fseek(fd, sub_size - start, SEEK_CUR);

		dx += (sub_size - start);
	}	
	SkipToNextBlock(fd);
#endif
	return offset;

}

vd::f32
DataFile::Periodic(
	vd::f32 x, vd::f32 l2)
{
    if(x > +l2)
		x -= l2 * 2.0f;
    if(x < -l2)
		x += l2 * 2.0f;
    return x;
}

void
DataFile::SwapFloatDataAt(
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
DataFile::SwapIntDataAt(
    int* ptr, int isrc, int idst)
{
    int t;
    int* src;
    int* dst;
    int* tmp;

    tmp = &t;
    src = &(ptr[isrc]);
    dst = &(ptr[idst]);

    Memory::CopyBytes(tmp, src, sizeof(int) * 1);
    Memory::CopyBytes(src, dst, sizeof(int) * 1);
    Memory::CopyBytes(dst, tmp, sizeof(int) * 1);
}

void
DataFile::SwapFloat3DataAt(
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
DataFile::SwapParticleAt(
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

vd::bytesize 
DataFile::ReadHeader(
	FILE* fd, vd::i32 splits)
{
	SkipToNextBlock(fd);
	vd::bytesize rd = fread(&(m_MetaData), sizeof(m_MetaData), 1, fd);
	SkipToNextBlock(fd);

	if(splits <= 1)
	{
		m_TotalParticleCount = 0;
		m_GasParticleCount = 0;
	}

	for(vd::i32 k = 0; k < (vd::i32)ParticleType::Count; k++)
		m_TotalParticleCount += m_MetaData.ParticleCountTotal[k];

	m_GasParticleCount += m_MetaData.ParticleCountTotal[0];
	return rd;
}

void
DataFile::ComputeTempFromInternalEnergy()
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

	DataFile::Block::Value block = DataFile::Block::Temperature;
	void* ptr = GetBlockDataPtr(block);
	if(ptr == NULL)
		CreateBlockData(block);
		
	ptr = GetBlockDataPtr(block);		
	if(ptr == NULL)
		return;

    for(vd::i64 i = 0; i < (vd::i64)m_TotalParticleCount; i++)
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
DataFile::GetFilename(
	const vd::string& prefix, 
	vd::i32 index,
	vd::i32 padding)
{
	return DataSet::GetFilenameForDataFile(prefix, index, padding);
}

vd::status
DataFile::Load(
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

    m_IsLoaded.Set(0);
	m_FilteredParticleCount = 0;
    vd::string filename = DataSet::GetFilenameForDataFile(prefix, index, padding);
    for(vd::i32 i = 0; i < splits; i++)
    {
    	if(splits > 1)
    		filename += vd::string(".") + Core::Convert::ToString(i, 0, 0);

	    FILE* fd = fopen(filename.c_str(), "r");	    
	    if(fd == NULL)
        {
            vdLogWarning("Failed to open Gadget snapshot file '%s'!", filename.c_str());
			return Status::Code::ReadError;
        }

        vdLogDebug("Reading snapshot '%s' ...", filename.c_str());

		if(!ReadHeader(fd, splits))
		{
            vdLogWarning("Failed to open Gadget snapshot file '%s'!", filename.c_str());
			return Status::Code::ReadError;
        }

	    vdLogInfo("Reading block 'ParticleType' from '%s'!", filename.c_str());
		ReadTypeData(fd, pc);

		m_FilteredParticleCount = 0;
		for(vd::i32 k = 0; k < (vd::i32)ParticleType::Count; k++)
		{
			if(req_types && req_types[k] > 0)
				m_FilteredParticleCount += m_MetaData.ParticleCountTotal[k];
		}
	    vdLogInfo("Loading only '%d' / '%d' particles!", m_FilteredParticleCount, m_TotalParticleCount);

		for(vd::u32 n = 0; n < Block::Count; n++)
		{
			DataFile::Block::Value block = DataFile::Block::FromInteger(n);
			if(req_data[n] > 0)
			{
	            vdLogInfo("Reading block '%s' from '%s'!", DataFile::Block::ToString(block), filename.c_str());
				m_BlockOffsets[n] = ftell(fd);
				ReadBlock(fd, block, pc, req_types);
 			}
			else
            {
				m_BlockOffsets[n] = ftell(fd);
				SkipBlock(fd, block, pc);
            }
        }
        
        fclose(fd);
        total_pc += pc;
    }

    // Compute derived quantity for Temperature from Internal Energy if it wasn't in the file
    if(req_data[DataFile::Block::Temperature] == VD_TRUE && 
    	GetBlockDataPtr(DataFile::Block::Temperature) == NULL)
    {
    	ComputeTempFromInternalEnergy();
    }

    vdLogInfo("Loaded only '%d'/'%d' particles!", m_FilteredParticleCount, m_TotalParticleCount);

    vdLogInfo("Filtering by type for '%s'!", filename.c_str());
    m_FilteredParticleCount = 0;
	for(vd::i64 i = 0; i < (vd::i64)m_TotalParticleCount; i++)
    {
        if(req_types[m_ParticleData.Type[i]] > 0)
        {
		     m_FilteredParticleCount++;
	    }
	}
    vdLogInfo("Validating '%d'/'%d' particles!", m_FilteredParticleCount, m_TotalParticleCount);

    m_FileIndex = index;
    m_IsLoaded.Increment();
    return 1;
}

void 
DataFile::Reorder()
{
    vd::i64 idst;
    for(vd::i64 i = 0; i < (vd::i64)m_TotalParticleCount; i++)
    	m_ParticleData.Id[i]--;
        	
    for(vd::i64 i = 0; i < (vd::i64)m_TotalParticleCount; i++)
    {
        if(m_ParticleData.Id[i] != i)
        {
            idst = m_ParticleData.Id[i];
            if(idst > (vd::i64)m_TotalParticleCount)
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
    
    for(vd::i64 i = 0; i < (vd::i64)m_TotalParticleCount; i++)
    	m_ParticleData.Id[i]++;
}

// ============================================================================================== //

DataSet::DataSet(
	Runtime::Context* runtime) :
	Core::Object(),
	m_IsOpen(false),
	m_FileCount(0),
	m_LastResidentFileIndex(0),
	m_CurrentFileIndex(0),
	m_StartFileIndex(0),
	m_EndFileIndex(0),
	m_FileSplits(0),
    m_CacheSize(1*1024),
    m_Runtime(runtime)
{
	Memory::SetBytes(m_TypeRequests,  0, sizeof(m_TypeRequests));
	Memory::SetBytes(m_BlockRequests, 0, sizeof(m_BlockRequests));
	Memory::SetBytes(m_SummaryRequests, 0, sizeof(m_SummaryRequests));
}

DataSet::~DataSet() 
{ 
	Destroy();
}

vd::status
DataSet::Destroy(void)
{
	Close();
	m_DataCache.Destroy();
	return Status::Code::Success;
}

vd::status 
DataSet::Open(
	const vd::string& prefix, 
	vd::i32 count, 
	vd::i32 splits)
{
	m_Mutex.Lock();

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
			if(fs->Exists(GetFilenameForDataFile(m_FilePrefix, i, m_FileNumberPadding)) == true)
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
		VD_BIND_MEMBER_FUNCTION(this, &DataSet::OnFetch), 
		VD_BIND_MEMBER_FUNCTION(this, &DataSet::OnEvict)
	);

	m_WorkCache.Setup(m_EndFileIndex - m_StartFileIndex + 1, 
		VD_BIND_MEMBER_FUNCTION(this, &DataSet::OnSubmit), 
		VD_BIND_MEMBER_FUNCTION(this, &DataSet::OnComplete)
	);
	
	m_WorkQueue.Start(1);	
	m_CurrentFileIndex = m_StartFileIndex;
	m_Mutex.Unlock();

	return Status::Code::Success;
}
	
vd::string
DataSet::GetFilenameForDataFile(
    const vd::string& prefix, vd::i32 index, vd::i32 padding)
{
	char digits[1024] = {0};
	snprintf(digits, sizeof(digits), "%d", index);
	vd::i32 digit_len = (vd::i32) std::strlen(digits);
	
	vd::string result = prefix;
	for(vd::i32 p = 0; p < padding - digit_len; p++)
		result += vd::string("0");
	
	result += vd::string(digits);
	return result;
}
	
vd::status 
DataSet::Close()
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
DataSet::IsReady(vd::i32 index)
{
	return IsResident(index);
}

bool
DataSet::IsPending(vd::i32 index)
{
	return m_WorkCache.IsResident(index);
}

bool
DataSet::IsResident(vd::i32 index)
{
	return m_DataCache.IsResident(index);
}

WorkItem*
DataSet::GetPendingWorkItem(vd::i32 index)
{
	WorkItem* work = NULL;
	return work;
}

DataFile*
DataSet::Retrieve(
	const vd::i32& index)
{
	DataFile* data = NULL;
	bool hit = m_DataCache.Fetch(index, data);
	hit = hit && data && data->IsLoaded();
//	vdLogInfo("Fetching snapshot: Index[%d] Hit[%s] ...", index, hit ? "true" : "false");
	return hit ? data : NULL;
}
  
bool
DataSet::Request(
	vd::i32 index)
{
	bool hit = m_DataCache.Load(index);
	vdLogInfo("Requesting snapshot: Index[%d] Hit[%s] ...", index, hit ? "true" : "false");
	return hit;
}

DataFile*
DataSet::OnFetch(
	const vd::i32& index)
{
	vdLogInfo("Miss on snapshot '%d' ...", index);

	DataFile* snapshot = NULL; 
	WorkItem* work = NULL;

	bool hit = m_WorkCache.Fetch(index, work);
	if(hit && work && work->IsReady())
	{
		vdLogInfo("OnFetch snapshot: Index [%d] Ptr[%p] ...", 
			index, work);

		snapshot = work->GetDataFile();
	}

	return snapshot;
}

void 
DataSet::OnEvict(
	DataFile* snapshot)
{
	if(snapshot != NULL)
		vdLogInfo("Evicting snapshot '%d' ...", snapshot->GetFileIndex());
	VD_DELETE(snapshot);
}

WorkItem*
DataSet::OnSubmit(
	const vd::i32& index)
{
	vdLogInfo("Adding load request for snapshot '%d' ...", index);

	DataFile* snapshot = VD_NEW(DataFile);
	WorkItem* work = VD_NEW(WorkItem, 
		snapshot, 
		m_FilePrefix, 
		index, 
		m_FileSplits, 
		m_FileNumberPadding, 
		m_BlockRequests, 
		m_TypeRequests,
		m_SummaryRequests);															  

	m_WorkQueue.Submit(work);
	m_DataCache.Insert(index, snapshot);
	return work;
}

void 
DataSet::OnComplete(
	WorkItem* work)
{
	if(work != NULL)
		vdLogInfo("DataFile '%d' loaded!", work->GetFileIndex());
	VD_DELETE(work);
}

void
DataSet::Release(vd::i32 index)
{
	m_WorkCache.Remove(index);
	m_DataCache.Remove(index);
}

void 
DataSet::Retain(vd::i32 index, DataFile* snapshot)
{
	m_DataCache.Touch(index);
}

void
DataSet::Evict()
{

}

void
WorkQueue::OnRun(
	Core::WorkItem* item)
{
	Gadget::WorkItem* work = (Gadget::WorkItem*)item;
    if(work == NULL)
        return;

	vd::f64 t0 = Process::GetTimeInSeconds();
    DataFile* snapshot = work->m_DataFile;

	vdLogInfo("[%d] %s Index[%d] Splits[%d] Padding[%d]", 
		(int)work->GetSlotId(), work->m_FilePrefix.c_str(), 
		work->m_FileIndex, work->m_FileSplits, work->m_FileNumberPadding);
		
	if(work->IsAborted())
	{
//		VD_DELETE(work);
		return;
	}	
	
	snapshot->Load(work->m_FilePrefix, work->m_FileIndex, 
				   work->m_FileSplits, work->m_FileNumberPadding, 
				   work->m_Blocks, work->m_Types);

	vd::string filename = snapshot->GetFilename(work->m_FilePrefix, 
												work->m_FileIndex, 
												work->m_FileNumberPadding);

	MetaData m = snapshot->GetMetaData();
	vd::f64 t1 = Process::GetTimeInSeconds();
	vdLogInfo("Loaded '%s' in '%f' sec.", filename.c_str(), t1 - t0);
	vdLogInfo("-- '%s' = [%s=%d %s=%d %s=%d %s=%d %s=%d %s=%d]", "ParticleCount", 
		DataFile::ParticleType::ToString(0),
		m.ParticleCount[0], 
		DataFile::ParticleType::ToString(1),
		m.ParticleCount[1],
		DataFile::ParticleType::ToString(2),
		m.ParticleCount[2], 
		DataFile::ParticleType::ToString(3),
		m.ParticleCount[3], 
		DataFile::ParticleType::ToString(4),
		m.ParticleCount[4], 
		DataFile::ParticleType::ToString(5),
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

	for(vd::i32 i = 0; i < (vd::i32)DataFile::Block::Count; i++)
	{
		DataFile::Block::Value block = DataFile::Block::FromInteger(i);
		if(work->m_Stats[i] < 1)
			continue;

		vd::f64 t0 = Process::GetTimeInSeconds();
		BlockSummary& s = snapshot->GetBlockSummary(block);
		s.Components = snapshot->GetBlockVectorLength(block);
		if(snapshot->IsBlockEntryIntegerValue(block))
		{
//			int* src = (int*)snapshot->GetSubBlockDataPtr(block, start_index, end_index);
			int* src = (int*)snapshot->GetBlockDataPtr(block);
			if(src == NULL)
				continue;

			for(vd::i8 vc = 0; vc < s.Components; vc++)
			{
				s.Minimum[vc] = +VD_F32_MAX;
				s.Maximum[vc] = -VD_F32_MAX;
			}

//			float* dst = (float*)snapshot->GetSubBlockDataPtr(block, start_index, end_index);
			float* dst = (float*)snapshot->GetBlockDataPtr(block);
//			for(vd::u64 n = 0; n < snapshot->GetFilteredParticleCount(start_index, end_index); n++)
			for(vd::u64 n = 0; n < snapshot->GetFilteredParticleCount(); n++)
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
//			float* src = (float*)snapshot->GetSubBlockDataPtr(block, start_index, end_index);
			float* src = (float*)snapshot->GetBlockDataPtr(block);
			if(src == NULL)
				continue;

			for(vd::i8 vc = 0; vc < s.Components; vc++)
			{
				s.Minimum[vc] = +VD_F32_MAX;
				s.Maximum[vc] = -VD_F32_MAX;
			}

//			for(vd::u64 n = 0; n < snapshot->GetFilteredParticleCount(start_index, end_index); n++)
			for(vd::u64 n = 0; n < snapshot->GetFilteredParticleCount(); n++)
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
			DataFile::Block::ToString(i), t1 - t0);

		for(vd::i8 vc = 0; vc < s.Components; vc++)
		{
			vdLogInfo("-- Min[%f] Max[%f] Sum[%f] Var[%f]", 
				s.Minimum[vc], s.Maximum[vc], s.TotalSum[vc], s.Variance[vc]);
		}

//		snapshot->SetResident(true);
	}
//	VD_DELETE(work);
}

// ============================================================================================== //

VD_IMPLEMENT_OBJECT(DataFile, vd_sym(GadgetDataFile), vd_sym(Object));
VD_IMPLEMENT_OBJECT(DataSet, vd_sym(GadgetDataSet), vd_sym(Object));
VD_IMPLEMENT_OBJECT(WorkQueue, vd_sym(GadgetWorkQueue), vd_sym(WorkQueue));
VD_IMPLEMENT_OBJECT(WorkItem, vd_sym(GadgetWorkItem), vd_sym(WorkItem));

// ============================================================================================== //

// ############################################################################################## //
// FIXME: Sub-block indexing borked. FIX!
// ############################################################################################## //
#if 0


namespace Gadget {

// ============================================================================================== //

VD_USING(Core, Thread);
VD_USING(Core, Memory);
VD_USING(Core, Process);

// ============================================================================================== //

class InputThread : public Thread
{

};

DataFile::DataFile(
	DataSet* dataset) :
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
 
DataFile::~DataFile()
{
	Destroy();
}

size_t
DataFile::GetResidentMemorySize() const
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
	vdLogInfo("DataFile resident memory size: %d MB", bytes / 1024 / 1024);
	return bytes;
}

Statistic& 
DataFile::GetStatistic(Block::Value v) 
{ 
    return m_StatisticsData[Block::ToInteger(v)]; 
}

void 
DataFile::SetStatistic(Block::Value v, const Statistic& stats) 
{
    m_StatisticsData[Block::ToInteger(v)] = stats; 
}

vd::status 
DataFile::Destroy()
{
	vdLogInfo("Destroying DataFile '%d' ... ", m_FileIndex);
	
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
DataFile::GetFilteredParticleCount(
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
DataFile::GetBlockSize(
	Block::Value block) const
{
	return GetBlockEntrySize(block) * m_FilteredParticleCount;
}

size_t
DataFile::GetSubBlockDataSize(
	Block::Value block_type,
	size_t start_index,
	size_t end_index) const
{
	size_t bytes = GetBlockEntrySize(block_type) * GetFilteredParticleCount(start_index, end_index);
	return bytes;
}

size_t
DataFile::CreateSubBlockData(
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
DataFile::DestroySubBlockData(
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
DataFile::GetSubBlockDataPtr(
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
DataFile::SetSubBlockDataPtr(
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
DataFile::ReadTypeData(
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
DataFile::GetBlockSeparatorSize(void) const
{
	return size_t(sizeof(char) * 4); //	32-bit
}

vd::i32
DataFile::SkipToNextBlock(
	FILE* fh)
{
    vd::i32 dummy = 1;
    int result = fseek(fh, sizeof(dummy), SEEK_CUR);
//    size_t bytes = fread(&dummy, sizeof(dummy), 1, fh);
    vdAssert(result == 0);
	return result ? dummy : 0;
}

size_t
DataFile::FindBlockByName(
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
DataFile::FindBlock(
	FILE* fd, Block::Value block)
{
	return FindBlockByName(fd, GetBlockLabel(block));
}

const char*
DataFile::GetBlockLabel(
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
DataFile::GetBlockName(
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
DataFile::GetBlockEntrySize(
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
DataFile::GetBlockVectorLength(
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
DataFile::GetBlockSizeTypeSuffix(
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
DataFile::IsBlockEntryIntegerValue(
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
DataFile::ReadSubBlockData(
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
DataFile::ReadPositionData(
	FILE* fd, 
	size_t start_index,
	size_t end_index,
	vd::i32* types)
{
	return ReadSubBlockData(fd, Block::Position, start_index, end_index, types);
}

size_t
DataFile::ReadVelocityData(
	FILE* fd, 
	size_t start_index,
	size_t end_index,
	vd::i32* types)
{
	return ReadSubBlockData(fd, Block::Velocity, start_index, end_index, types);
}

size_t
DataFile::ReadIdentifierData(
	FILE* fd, 
	size_t start_index,
	size_t end_index,
	vd::i32* types)
{
	return ReadSubBlockData(fd, Block::Id, start_index, end_index, types);
}

size_t
DataFile::ReadMassData(
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
DataFile::ReadInternalEnergyData(
	FILE* fd, 
	size_t start_index,
	size_t end_index, 
	vd::i32* types)
{
	return ReadSubBlockData(fd, Block::InternalEnergy, start_index, end_index, types);
}

size_t
DataFile::ReadDensityData(
	FILE* fd, 
	size_t start_index,
	size_t end_index, 
	vd::i32* types)
{
	return ReadSubBlockData(fd, Block::Density, start_index, end_index, types);
}

size_t
DataFile::ReadSmoothingLengthData(
	FILE* fd, 
	size_t start_index,
	size_t end_index, 
	vd::i32* types)
{
	return ReadSubBlockData(fd, Block::SmoothingLength, start_index, end_index, types);
}

size_t
DataFile::ReadPotentialData(
	FILE* fd, 
	size_t start_index,
	size_t end_index, 
	vd::i32* types)
{
	return ReadSubBlockData(fd, Block::Potential, start_index, end_index, types);
}

size_t
DataFile::ReadAcceleration(
	FILE* fd, 
	size_t start_index,
	size_t end_index, 
	vd::i32* types)
{
	return ReadSubBlockData(fd, Block::Acceleration, start_index, end_index, types);
}

size_t
DataFile::ReadRateOfChangeOfEntropyData(
	FILE* fd, 
	size_t start_index,
	size_t end_index, 
	vd::i32* types)
{
	return ReadSubBlockData(fd, Block::DtEntropy, start_index, end_index, types);
}

size_t
DataFile::ReadTimeStepData(
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
DataFile::SkipSubBlock(
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
DataFile::ReadSubBlock(
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
DataFile::Periodic(
	vd::f32 x, vd::f32 l2)
{
    if(x > +l2)
		x -= l2 * 2.0f;
    if(x < -l2)
		x += l2 * 2.0f;
    return x;
}

void
DataFile::SwapFloatDataAt(
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
DataFile::SwapIntDataAt(
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
DataFile::SwapFloat3DataAt(
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
DataFile::SwapParticleAt(
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
DataFile::ReadHeader(
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
DataFile::ComputeTempFromInternalEnergy(
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

	DataFile::Block::Value block = DataFile::Block::Temperature;
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
DataFile::GetFilename(
	const vd::string& prefix, 
	vd::i32 index,
	vd::i32 padding)
{
	return m_DataSet->GetFilenameForDataFile(prefix, index, padding);
}

vd::status
DataFile::Load(
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
	    vd::string filename = m_DataSet->GetFilenameForDataFile(prefix, index, padding);
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
	    vd::string filename = m_DataSet->GetFilenameForDataFile(prefix, index, padding);
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
			DataFile::Block::Value block = DataFile::Block::FromInteger(n);
			if(req_data[n] > 0)
			{
#if defined(VD_DEBUG_GS2)
	            vdLogInfo("Reading block '%s' from '%s'!", 
	            	DataFile::Block::ToString(block), 
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
    if(req_data[DataFile::Block::Temperature] == VD_TRUE && 
    	GetSubBlockDataPtr(DataFile::Block::Temperature, start_index, end_index) == NULL)
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
DataFile::Reorder()
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

DataSet::DataSet(
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
	Memory::SetBytes(m_TypeRequests,  0, sizeof(m_TypeRequests));
	Memory::SetBytes(m_BlockRequests, 0, sizeof(m_BlockRequests));
	Memory::SetBytes(m_SummaryRequests, 0, sizeof(m_SummaryRequests));
}

DataSet::~DataSet() 
{ 
	Destroy();
}

vd::status
DataSet::Destroy(void)
{
	Close();
	m_DataCache.Destroy();
	return Status::Code::Success;
}

vd::status 
DataSet::Open(
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
			if(fs->Exists(GetFilenameForDataFile(m_FilePrefix, i, m_FileNumberPadding)) == true)
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
		VD_BIND_MEMBER_FUNCTION(this, &DataSet::OnFetch), 
		VD_BIND_MEMBER_FUNCTION(this, &DataSet::OnEvict)
	);

	m_WorkCache.Setup(m_EndFileIndex - m_StartFileIndex + 1, 
		VD_BIND_MEMBER_FUNCTION(this, &DataSet::OnSubmit), 
		VD_BIND_MEMBER_FUNCTION(this, &DataSet::OnComplete)
	);
	
	m_WorkQueue.Start(1);	
	m_CurrentFileIndex = m_StartFileIndex;
	m_Mutex.Unlock();

	return Status::Code::Success;
}
	
vd::string
DataSet::GetFilenameForDataFile(
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
DataSet::Close()
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
DataSet::IsReady(
	DataRequest request)
{
	return IsResident(request);
}

bool
DataSet::IsPending(
	DataRequest request)
{
	return m_WorkCache.IsResident(request);
}

bool
DataSet::IsResident(
	DataRequest request)
{
	return m_DataCache.IsResident(request);
}

WorkItem*
DataSet::GetPendingWorkItem(
	DataRequest request)
{
	WorkItem* work = NULL;
	return work;
}

DataFile*
DataSet::Retrieve(
	DataRequest request)
{
	if(IsValidRequest(request) == false)
		return NULL;
	
	DataFile* data = NULL;
	bool hit = m_DataCache.Fetch(request, data);
	hit = hit && data && data->IsResident();

	vdLogInfo("Fetching snapshot data '%d' %s [%d -> %d]...", 
		request.DataFileIndex,
		DataFile::Block::ToString(request.BlockType),
		request.StartIndex,
		request.EndIndex);

//	vdLogInfo("Fetching snapshot: Index[%d] Hit[%s] ...", index, hit ? "true" : "false");
	return hit ? data : NULL;
}
  
bool
DataSet::Request(
	DataRequest request)
{
	if(IsValidRequest(request) == false)
		return false;

	bool hit = m_DataCache.Load(request);

#if defined(VD_DEBUG_GS2)
	vdLogInfo("Requesting snapshot data '%d' %s [%d -> %d]...", 
		request.DataFileIndex,
		DataFile::Block::ToString(request.BlockType),
		request.StartIndex,
		request.EndIndex);
#endif

	return hit;
}

DataFile*
DataSet::OnFetch(
	const DataRequest& request)
{
	if(IsValidRequest(request) == false)
		return NULL;
		
#if defined(VD_DEBUG_GS2)
	vdLogInfo("Miss for snapshot '%d' %s [%d -> %d]...", 
		request.DataFileIndex,
		DataFile::Block::ToString(request.BlockType),
		request.StartIndex,
		request.EndIndex);
#endif

	DataFile* snapshot = NULL; 
	WorkItem* work = NULL;

	bool hit = m_WorkCache.Fetch(request, work);
	if(hit && work && work->IsReady())
	{
#if defined(VD_DEBUG_GS2)
		vdLogInfo("OnFetch() for snapshot '%d' %s [%d -> %d]...", 
			request.DataFileIndex,
			DataFile::Block::ToString(request.BlockType),
			request.StartIndex,
			request.EndIndex);
#endif
		snapshot = work->GetDataFile();
	}

	return snapshot;
}

void 
DataSet::OnEvict(
	DataFile* snapshot)
{
	if(snapshot != NULL)
		vdLogInfo("Evicting snapshot '%d' ...", snapshot->GetFileIndex());

	VD_DELETE(snapshot);
}

WorkItem*
DataSet::OnSubmit(
	const DataRequest& request)
{
	if(IsValidRequest(request) == false)
		return NULL;
		
#if defined(VD_DEBUG_GS2)
	vdLogInfo("Adding load request for snapshot '%d' %s [%d -> %d]...", 
		request.DataFileIndex,
		DataFile::Block::ToString(request.BlockType),
		request.StartIndex,
		request.EndIndex);
#endif

	DataFile* snapshot = VD_NEW(DataFile, this);

	snapshot->SetResident(false);

	WorkItem* work = VD_NEW(WorkItem, 
		request,
		snapshot, 
		m_FilePrefix, 
		request.DataFileIndex, 
		m_FileSplits, 
		m_FileNumberPadding, 
		m_BlockRequests, 
		m_TypeRequests,
		m_SummaryRequests);															  

	m_WorkQueue.Submit(work);
	m_DataCache.Insert(request, snapshot);
	return work;
}

void 
DataSet::OnComplete(
	WorkItem* work)
{
#if defined(VD_DEBUG_GS2)
	if(work != NULL)
		vdLogInfo("DataFile '%d' loaded!", work->GetFileIndex());
#endif

	VD_DELETE(work);
}

void
DataSet::Release(
	DataRequest request)
{
	m_WorkCache.Remove(request);
	m_DataCache.Remove(request);
}

void 
DataSet::Retain(
	DataRequest request, DataFile* snapshot)
{
	m_DataCache.Touch(request);
}

void DataSet::SetBlockRequest(
	DataFile::Block::Value v, vd::i32 enable) 
{ 
	m_BlockRequests[DataFile::Block::ToInteger(v)] = enable; 
}

void DataSet::SetTypeRequest(
	DataFile::ParticleType::Value v, vd::i32 enable) 
{ 
	m_TypeRequests[DataFile::ParticleType::ToInteger(v)] = enable; 
}

void DataSet::SetStatisticRequest(DataFile::Block::Value v, vd::i32 enable) 
{ 
	m_SummaryRequests[DataFile::Block::ToInteger(v)] = enable; 
}

vd::i32 DataSet::GetFileCount() const 
{ 
	return m_FileCount; 
}

vd::string DataSet::GetFilePrefix() const 
{ 
	return m_FilePrefix; 
}

vd::i32 DataSet::GetStartFileIndex() const 
{ 
	return m_StartFileIndex; 
}

vd::i32 DataSet::GetEndFileIndex() const 
{ 
	return m_EndFileIndex; 
}

vd::i32 DataSet::GetCurrentFileIndex() const 
{ 
	return m_CurrentFileIndex; 
}

void DataSet::SetCurrentFileIndex(vd::i32 index) 
{ 
	m_CurrentFileIndex = index; 
}

bool DataSet::IsOpen() const 
{ 
	return m_IsOpen; 
}

bool DataSet::IsValidIndex(
	vd::i32 index) const
{ 
	return (index >= m_StartFileIndex && index <= m_EndFileIndex); 
}

bool DataSet::IsValidBlock(
	DataFile::Block::Value v) const 
{ 
	return v != DataFile::Block::LastValue; 
}

bool DataSet::IsValidParticleType(
	DataFile::ParticleType::Value v) const 
{ 
	return v != DataFile::ParticleType::LastValue; 
}

bool DataSet::IsValidRequest(
	DataRequest request) const
{
	if(IsValidIndex(request.DataFileIndex) == false)
		return false;

	if(IsValidBlock(request.BlockType) == false)
		return false;

	if(request.StartIndex >= request.EndIndex)
		return false;

	return true;
}

void
DataSet::Evict()
{

}

void
WorkQueue::OnRun(
	WorkItem* item)
{
	WorkItem* work = (WorkItem*)item;
    if(work == NULL)
        return;

    DataFile* snapshot = work->m_DataFile;

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
		DataFile::ParticleType::ToString(0),
		m.ParticleCount[0], 
		DataFile::ParticleType::ToString(1),
		m.ParticleCount[1],
		DataFile::ParticleType::ToString(2),
		m.ParticleCount[2], 
		DataFile::ParticleType::ToString(3),
		m.ParticleCount[3], 
		DataFile::ParticleType::ToString(4),
		m.ParticleCount[4], 
		DataFile::ParticleType::ToString(5),
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
	for(vd::i32 i = 0; i < (vd::i32)DataFile::Block::Count; i++)
	{
		DataFile::Block::Value block = DataFile::Block::FromInteger(i);
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
			DataFile::Block::ToString(i), t1 - t0);

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

VD_IMPLEMENT_OBJECT(DataFile, vd_sym(GadgetDataFile), vd_sym(Object));
VD_IMPLEMENT_OBJECT(DataSet, vd_sym(GadgetDataSet), vd_sym(Object));
VD_IMPLEMENT_OBJECT(WorkQueue, vd_sym(GadgetWorkQueue), vd_sym(WorkQueue));
VD_IMPLEMENT_OBJECT(WorkItem, vd_sym(GadgetWorkItem), vd_sym(WorkItem));

// ============================================================================================== //

// ############################################################################################## //
#endif
// ############################################################################################## //

} // end namespace: Gadget

// ============================================================================================== //

VD_FORMATS_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_USE_GS2

// ============================================================================================== //

