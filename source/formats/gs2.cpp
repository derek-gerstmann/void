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

VD_FORMATS_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_IMPORT(Core, Thread);
VD_IMPORT(Core, Memory);
VD_IMPORT(Core, Process);

// ============================================================================================== //

class GadgetInputThread : public Thread
{

};

GadgetSnapshot::GadgetSnapshot(
	GadgetDataset* dataset) :
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
 
GadgetSnapshot::~GadgetSnapshot()
{
	Destroy();
}

size_t
GadgetSnapshot::GetResidentMemorySize() const
{
	size_t bytes = 0;
	for(vd::i32 i = 0; i < (int)Block::Count; i++)
	{
		GadgetSnapshot::Block::Value block = GadgetSnapshot::Block::FromInteger(i);
		const char* ptr = (char*)GetBlockDataPtr(block);
		if(ptr != NULL)
		{
			bytes += GetBlockEntrySize(block) * GetFilteredParticleCount();
		}
	}
	return bytes;
}

vd::status 
GadgetSnapshot::Destroy()
{
	vdLogInfo("Destroying Snapshot '%d' ... ", m_FileIndex);
	
	for(vd::u32 i = 0; i < Block::Count; i++)
	{
		GadgetSnapshot::Block::Value block = GadgetSnapshot::Block::FromInteger(i);
		DestroyBlockData(block);
	}

	return Status::Code::Success;
}

size_t
GadgetSnapshot::GetBlockSize(
	Block::Value block) const
{
	return GetBlockEntrySize(block) * GetFilteredParticleCount();
}

size_t
GadgetSnapshot::CreateBlockData(
	Block::Value block)
{
	char* ptr = (char*)GetBlockDataPtr(block);
	size_t bytes = GetBlockEntrySize(block) * GetFilteredParticleCount();
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
GadgetSnapshot::DestroyBlockData(
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
GadgetSnapshot::GetBlockDataPtr(
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
GadgetSnapshot::SetBlockDataPtr(
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

size_t
GadgetSnapshot::ReadTypeData(
	FILE* fd, size_t offset)
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

size_t
GadgetSnapshot::GetBlockSeparatorSize(void) const
{
	return size_t(sizeof(char) * 4); //	32-bit
}

vd::i32
GadgetSnapshot::SkipToNextBlock(
	FILE* fh)
{
    vd::i32 dummy = 1;
    int result = fseek(fh, sizeof(dummy), SEEK_CUR);
//    size_t bytes = fread(&dummy, sizeof(dummy), 1, fh);
    vdAssert(result == 0);
	return dummy;
}

size_t
GadgetSnapshot::FindBlockByName(
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
GadgetSnapshot::FindBlock(
	FILE* fd, Block::Value block)
{
	return FindBlockByName(fd, GetBlockLabel(block));
}

const char*
GadgetSnapshot::GetBlockLabel(
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
GadgetSnapshot::GetBlockName(
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
GadgetSnapshot::GetBlockEntrySize(
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
GadgetSnapshot::GetBlockVectorLength(
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
GadgetSnapshot::GetBlockSizeTypeSuffix(
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
GadgetSnapshot::IsBlockEntryIntegerValue(
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
GadgetSnapshot::ReadBlockData(
	FILE* fd, 
	Block::Value block,
	size_t offset,
	vd::i32* types)
{
	size_t rd = 0;
	size_t dx = 0;
	void* ptr = GetBlockDataPtr(block);
	if(ptr == NULL)
	{
		CreateBlockData(block);
		ptr = GetBlockDataPtr(block);
	}

#if defined(VD_DEBUG_GS2)
	vdLogInfo("Reading block [%d = %s] (raw %d)...", Block::ToInteger(block), Block::ToString(block), (vd::i32)block);
#endif

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
}

size_t
GadgetSnapshot::ReadPositionData(
	FILE* fd, size_t offset, vd::i32* types)
{
	return ReadBlockData(fd, Block::Position, offset, types);
}

size_t
GadgetSnapshot::ReadVelocityData(
	FILE* fd, size_t offset, vd::i32* types)
{
	return ReadBlockData(fd, Block::Velocity, offset, types);
}

size_t
GadgetSnapshot::ReadIdentifierData(
	FILE* fd, size_t offset, vd::i32* types)
{
	return ReadBlockData(fd, Block::Id, offset, types);
}

size_t
GadgetSnapshot::ReadMassData(
	FILE* fd, size_t offset, vd::i32* types)
{
	size_t rd = 0;
	vd::i64 per_particle_mass = 0;

	Block::Value block = Block::Mass;

	void* ptr = GetBlockDataPtr(block);
	if(ptr == NULL)
	{
		CreateBlockData(block);
		ptr = GetBlockDataPtr(block);
	}
		
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

#if defined(VD_DEBUG_GS2)
		vdLogInfo("Reading block [%d = %s] (raw %d)...", Block::ToInteger(block), Block::ToString(block), (vd::i32)block);
#endif

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
	}

	return pc;
}

size_t
GadgetSnapshot::ReadInternalEnergyData(
	FILE* fd, size_t offset, vd::i32* types)
{
	return ReadBlockData(fd, Block::InternalEnergy, offset, types);
}

size_t
GadgetSnapshot::ReadDensityData(
	FILE* fd, size_t offset, vd::i32* types)
{
//	if(m_MetaData.ParticleCount[ParticleType::Gas] < 1)
//		return 0;
	
	return ReadBlockData(fd, Block::Density, offset, types);
}

size_t
GadgetSnapshot::ReadSmoothingLengthData(
	FILE* fd, size_t offset, vd::i32* types)
{
//	if(m_MetaData.ParticleCount[ParticleType::Gas] < 1)
//		return 0;
	
	return ReadBlockData(fd, Block::SmoothingLength, offset, types);
}

size_t
GadgetSnapshot::ReadPotentialData(
	FILE* fd, size_t offset, vd::i32* types)
{
	return ReadBlockData(fd, Block::Potential, offset, types);
}

size_t
GadgetSnapshot::ReadAcceleration(
	FILE* fd, size_t offset, vd::i32* types)
{
	return ReadBlockData(fd, Block::Acceleration, offset, types);
}

size_t
GadgetSnapshot::ReadRateOfChangeOfEntropyData(
	FILE* fd, size_t offset, vd::i32* types)
{
	return ReadBlockData(fd, Block::DtEntropy, offset, types);
}

size_t
GadgetSnapshot::ReadTimeStepData(
	FILE* fd, size_t offset, vd::i32* types)
{
	Block::Value block = Block::TimeStep;

	void* ptr = GetBlockDataPtr(block);
	if(ptr == NULL)
	{
		CreateBlockData(block);
		ptr = GetBlockDataPtr(block);
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
}

size_t
GadgetSnapshot::SkipBlock(
	FILE* fd, Block::Value block, size_t start)
{
	size_t offset = 0;
//	offset += GetBlockSeparatorSize();
	SkipToNextBlock(fd);
	for(vd::i32 pt = 0; pt < (vd::i32)ParticleType::Count; pt++)
	{
		offset += GetBlockEntrySize(block) * m_MetaData.ParticleCount[pt];
	}
	fseek(fd,offset,SEEK_CUR);
	SkipToNextBlock(fd);

//	offset += GetBlockSeparatorSize();
	return offset;
}

size_t
GadgetSnapshot::ReadBlock(
	FILE* fd, Block::Value block, size_t offset, vd::i32* types)
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

vd::f32
GadgetSnapshot::Periodic(
	vd::f32 x, vd::f32 l2)
{
    if(x > +l2)
		x -= l2 * 2.0f;
    if(x < -l2)
		x += l2 * 2.0f;
    return x;
}

void
GadgetSnapshot::SwapFloatDataAt(
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
GadgetSnapshot::SwapIntDataAt(
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
GadgetSnapshot::SwapFloat3DataAt(
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
GadgetSnapshot::SwapParticleAt(
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
GadgetSnapshot::ReadHeader(
	FILE* fd, vd::i32 splits)
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

	m_FilteredParticleCount = m_TotalParticleCount;
	m_GasParticleCount += m_MetaData.ParticleCountTotal[0];
	return rd;
}

void
GadgetSnapshot::ComputeTempFromInternalEnergy()
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

	GadgetSnapshot::Block::Value block = GadgetSnapshot::Block::Temperature;
	void* ptr = GetBlockDataPtr(block);
	if(ptr == NULL)
		CreateBlockData(block);
		
	ptr = GetBlockDataPtr(block);		
	if(ptr == NULL)
		return;

    for(vd::i64 i = 0; i < (vd::i64)GetFilteredParticleCount(); i++)
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
GadgetSnapshot::GetFilename(
	const vd::string& prefix, 
	vd::i32 index,
	vd::i32 padding)
{
	return m_DataSet->GetFilenameForSnapshot(prefix, index, padding);
}

void
GadgetSnapshot::ProcessParticleTypesRequest(
	vd::i32* req_types)
{
	bool filter_types = false;
	for(vd::i32 k = 0; k < (vd::i32)ParticleType::Count; k++)
	{
		if(req_types && req_types[k] > 0)
			filter_types = true;
	}

	if(filter_types)
	{
		m_FilteredParticleCount = 0;
		for(vd::i32 k = 0; k < (vd::i32)ParticleType::Count; k++)
		{
			if(req_types && req_types[k] > 0)
				m_FilteredParticleCount += m_MetaData.ParticleCountTotal[k];
		}
	}
	else
	{
		m_FilteredParticleCount = m_TotalParticleCount;
	}
}

vd::status
GadgetSnapshot::Load(
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

    vd::string filename = m_DataSet->GetFilenameForSnapshot(prefix, index, padding);
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

#if defined(VD_DEBUG_GS2)
        vdLogDebug("Reading snapshot '%s' ...", filename.c_str());
#endif

		if(!ReadHeader(fd, splits))
		{
            vdLogWarning("Failed to open Gadget snapshot file '%s'!", filename.c_str());
			return Status::Code::ReadError;
        }

#if defined(VD_DEBUG_GS2)
	    vdLogInfo("Reading block 'ParticleType' from '%s'!", filename.c_str());
#endif
		ReadTypeData(fd, pc);
		ProcessParticleTypesRequest(req_types);

	#if defined(VD_DEBUG_GS2)
		    vdLogInfo("Loading '%d' / '%d' particles!", 
		    	m_FilteredParticleCount, m_TotalParticleCount);
	#endif

		for(vd::u32 n = 0; n < Block::Count; n++)
		{
			GadgetSnapshot::Block::Value block = GadgetSnapshot::Block::FromInteger(n);
			if(req_data[n] > 0)
			{
#if defined(VD_DEBUG_GS2)
	            vdLogInfo("Reading block '%s' from '%s'!", 
	            	GadgetSnapshot::Block::ToString(block), 
	            	filename.c_str());
#endif
				ReadBlock(fd, block, pc, req_types);
 			}
			else
            {
				SkipBlock(fd, block, pc);

            }
        }
        
        fclose(fd);
        total_pc += pc;
    }

    // Compute derived quantity for Temperature from Internal Energy if it wasn't in the file
    if(req_data[GadgetSnapshot::Block::Temperature] == VD_TRUE && 
    	GetBlockDataPtr(GadgetSnapshot::Block::Temperature) == NULL)
    {
    	ComputeTempFromInternalEnergy();
    }

#if defined(VD_DEBUG_GS2)
    vdLogInfo("Loaded '%d'/'%d' particles!", m_FilteredParticleCount, m_TotalParticleCount);
    vdLogInfo("Filtering by type for '%s'!", filename.c_str());
#endif

#if defined(VD_DEBUG_GS2)
#if 0
    m_FilteredParticleCount = 0;
	for(vd::i64 i = 0; i < (vd::i64)m_TotalParticleCount; i++)
    {
        if(req_types[m_ParticleData.Type[i]] > 0)
        {
		     m_FilteredParticleCount++;
	    }
	}
    vdLogInfo("Validating '%d'/'%d' particles!", m_FilteredParticleCount, m_TotalParticleCount);
#endif
#endif

    m_FileIndex = index;
    return 1;
}

void 
GadgetSnapshot::Reorder()
{
    vd::i64 idst;
    for(vd::i64 i = 0; i < (vd::i64)GetFilteredParticleCount(); i++)
    	m_ParticleData.Id[i]--;
        	
    for(vd::i64 i = 0; i < (vd::i64)GetFilteredParticleCount(); i++)
    {
        if(m_ParticleData.Id[i] != i)
        {
            idst = m_ParticleData.Id[i];
            if(idst > (vd::i64)GetFilteredParticleCount())
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
    
    for(vd::i64 i = 0; i < (vd::i64)GetFilteredParticleCount(); i++)
    	m_ParticleData.Id[i]++;
}

// ============================================================================================== //

GadgetDataset::GadgetDataset(
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

GadgetDataset::~GadgetDataset() 
{ 
	Destroy();
}

vd::status
GadgetDataset::Destroy(void)
{
	Close();
	m_DataCache.Destroy();
	return Status::Code::Success;
}

vd::status 
GadgetDataset::Open(
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
		m_EndFileIndex = m_StartFileIndex + count;
	}

	m_FileSplits = splits;
	m_IsOpen = true;
	
	vdLogInfo("Opening dataset '%s' from '%d-%d' with padded index '%d' ...",
		m_FilePrefix.c_str(), m_StartFileIndex, m_EndFileIndex, m_FileNumberPadding);
	
	m_DataCache.Setup(2 * Constants::MemorySize::GB, 
		VD_BIND_MEMBER_FUNCTION(this, &GadgetDataset::OnFetch), 
		VD_BIND_MEMBER_FUNCTION(this, &GadgetDataset::OnEvict)
	);

	m_WorkCache.Setup(m_EndFileIndex - m_StartFileIndex + 1, 
		VD_BIND_MEMBER_FUNCTION(this, &GadgetDataset::OnSubmit), 
		VD_BIND_MEMBER_FUNCTION(this, &GadgetDataset::OnComplete)
	);
	
	m_WorkQueue.Start(1);	
	m_CurrentFileIndex = m_StartFileIndex;
	m_Mutex.Unlock();

	return Status::Code::Success;
}
	
vd::string
GadgetDataset::GetFilenameForSnapshot(
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
GadgetDataset::Close()
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
GadgetDataset::IsReady(vd::i32 index)
{
	return IsResident(index);
}

bool
GadgetDataset::IsPending(vd::i32 index)
{
	return m_WorkCache.IsResident(index);
}

bool
GadgetDataset::IsResident(vd::i32 index)
{
	return m_DataCache.IsResident(index);
}

GadgetWorkItem*
GadgetDataset::GetPendingWorkItem(vd::i32 index)
{
	GadgetWorkItem* work = NULL;
	return work;
}

GadgetSnapshot*
GadgetDataset::Retrieve(
	const vd::i32& index)
{
	if(IsValidIndex(index) == false)
		return false;
	
	GadgetSnapshot* data = NULL;
	bool hit = m_DataCache.Fetch(index, data);
	hit = hit && data && data->IsResident();
//	vdLogInfo("Fetching snapshot: Index[%d] Hit[%s] ...", index, hit ? "true" : "false");
	return hit ? data : NULL;
}
  
bool
GadgetDataset::Request(
	vd::i32 index)
{
	if(IsValidIndex(index) == false)
		return false;

	bool hit = m_DataCache.Load(index);

#if defined(VD_DEBUG_GS2)
	vdLogInfo("Requesting snapshot: Index[%d] Hit[%s] ...", index, hit ? "true" : "false");
#endif

	return hit;
}

GadgetSnapshot*
GadgetDataset::OnFetch(
	const vd::i32& index)
{
	if(IsValidIndex(index) == false)
		return NULL;
		
#if defined(VD_DEBUG_GS2)
	vdLogInfo("Miss on snapshot '%d' ...", index);
#endif

	GadgetSnapshot* snapshot = NULL; 
	GadgetWorkItem* work = NULL;

	bool hit = m_WorkCache.Fetch(index, work);
	if(hit && work && work->IsReady())
	{
#if defined(VD_DEBUG_GS2)
		vdLogInfo("OnFetch snapshot: Index [%d] Ptr[%p] ...", 
			index, work);
#endif
		snapshot = work->GetSnapshot();
	}

	return snapshot;
}

void 
GadgetDataset::OnEvict(
	GadgetSnapshot* snapshot)
{
	if(snapshot != NULL)
		vdLogInfo("Evicting snapshot '%d' ...", snapshot->GetFileIndex());

	VD_DELETE(snapshot);
}

GadgetWorkItem*
GadgetDataset::OnSubmit(
	const vd::i32& index)
{
	if(IsValidIndex(index) == false)
		return NULL;
		
#if defined(VD_DEBUG_GS2)
	vdLogInfo("Adding load request for snapshot '%d' ...", index);
#endif

	GadgetSnapshot* snapshot = VD_NEW(GadgetSnapshot, this);

	snapshot->SetResident(false);

	GadgetWorkItem* work = VD_NEW(GadgetWorkItem, 
		snapshot, 
		m_FilePrefix, 
		index, 
		m_FileSplits, 
		m_FileNumberPadding, 
		m_RequestedBlocks, 
		m_RequestedTypes,
		m_RequestedStatistics);															  

	m_WorkQueue.Submit(work);
	m_DataCache.Insert(index, snapshot);
	return work;
}

void 
GadgetDataset::OnComplete(
	GadgetWorkItem* work)
{
#if defined(VD_DEBUG_GS2)
	if(work != NULL)
		vdLogInfo("Snapshot '%d' loaded!", work->GetFileIndex());
#endif

	VD_DELETE(work);
}

void
GadgetDataset::Release(vd::i32 index)
{
	m_WorkCache.Remove(index);
	m_DataCache.Remove(index);
}

void 
GadgetDataset::Retain(vd::i32 index, GadgetSnapshot* snapshot)
{
	m_DataCache.Touch(index);
}

void GadgetDataset::SetBlockRequest(GadgetSnapshot::Block::Value v, vd::i32 enable) 
	{ m_RequestedBlocks[GadgetSnapshot::Block::ToInteger(v)] = enable; }

void GadgetDataset::SetTypeRequest(GadgetSnapshot::ParticleType::Value v, vd::i32 enable) 
	{ m_RequestedTypes[GadgetSnapshot::ParticleType::ToInteger(v)] = enable; }

void GadgetDataset::SetStatisticRequest(GadgetSnapshot::Block::Value v, vd::i32 enable) 
    { m_RequestedStatistics[GadgetSnapshot::Block::ToInteger(v)] = enable; }

vd::i32 GadgetDataset::GetFileCount() const 
{ return m_FileCount; }

vd::string GadgetDataset::GetFilePrefix() const 
{ return m_FilePrefix; }

vd::i32 GadgetDataset::GetStartFileIndex() const 
{ return m_StartFileIndex; }

vd::i32 GadgetDataset::GetEndFileIndex() const 
{ return m_EndFileIndex; }

vd::i32 GadgetDataset::GetCurrentFileIndex() const 
{ return m_CurrentFileIndex; }

void GadgetDataset::SetCurrentFileIndex(vd::i32 index) 
{ m_CurrentFileIndex = index; }

bool GadgetDataset::IsOpen() const 
{ return m_IsOpen; }

bool GadgetDataset::IsValidIndex(vd::i32 index) 
{ return (index >= m_StartFileIndex && index <= m_EndFileIndex); }

bool GadgetDataset::IsValidBlock(GadgetSnapshot::Block::Value v) const 
{ return v != GadgetSnapshot::Block::LastValue; }

bool GadgetDataset::IsValidParticleType(GadgetSnapshot::ParticleType::Value v) const 
{ 
	return v != GadgetSnapshot::ParticleType::LastValue; 
}

void
GadgetDataset::Evict()
{

}

void
GadgetWorkQueue::OnRun(
	WorkItem* item)
{
	GadgetWorkItem* work = (GadgetWorkItem*)item;
    if(work == NULL)
        return;

    GadgetSnapshot* snapshot = work->m_Snapshot;

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
	
	snapshot->Load(work->m_FilePrefix, work->m_FileIndex, 
				   work->m_FileSplits, work->m_FileNumberPadding, 
				   work->m_Blocks, work->m_Types);

	vd::string filename = snapshot->GetFilename(work->m_FilePrefix, 
												work->m_FileIndex, 
												work->m_FileNumberPadding);

#if defined(VD_DEBUG_GS2)
	GadgetMetaData m = snapshot->GetMetaData();
	vd::f64 t1 = Process::GetTimeInSeconds();
	vdLogInfo("Loaded '%s' in '%f' sec.", filename.c_str(), t1 - t0);
	vdLogInfo("-- '%s' = [%s=%d %s=%d %s=%d %s=%d %s=%d %s=%d]", "ParticleCount", 
		GadgetSnapshot::ParticleType::ToString(0),
		m.ParticleCount[0], 
		GadgetSnapshot::ParticleType::ToString(1),
		m.ParticleCount[1],
		GadgetSnapshot::ParticleType::ToString(2),
		m.ParticleCount[2], 
		GadgetSnapshot::ParticleType::ToString(3),
		m.ParticleCount[3], 
		GadgetSnapshot::ParticleType::ToString(4),
		m.ParticleCount[4], 
		GadgetSnapshot::ParticleType::ToString(5),
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
	
	for(vd::i32 i = 0; i < (vd::i32)GadgetSnapshot::Block::Count; i++)
	{
		GadgetSnapshot::Block::Value block = GadgetSnapshot::Block::FromInteger(i);
		if(work->m_Stats[i] < 1)
			continue;

		vd::f64 t0 = Process::GetTimeInSeconds();
		GadgetStatistic& s = snapshot->GetStatistic(block);
		s.Components = snapshot->GetBlockVectorLength(block);
		if(snapshot->IsBlockEntryIntegerValue(block))
		{
			int* src = (int*)snapshot->GetBlockDataPtr(block);
			if(src == NULL)
				continue;

			for(vd::i8 vc = 0; vc < s.Components; vc++)
			{
				s.Minimum[vc] = +VD_F32_MAX;
				s.Maximum[vc] = -VD_F32_MAX;
			}

			float* dst = (float*)snapshot->GetBlockDataPtr(block);
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
			float* src = (float*)snapshot->GetBlockDataPtr(block);
			if(src == NULL)
				continue;

			for(vd::i8 vc = 0; vc < s.Components; vc++)
			{
				s.Minimum[vc] = +VD_F32_MAX;
				s.Maximum[vc] = -VD_F32_MAX;
			}

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
			GadgetSnapshot::Block::ToString(i), t1 - t0);

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

VD_IMPLEMENT_OBJECT(GadgetSnapshot, vd_sym(GadgetSnapshot), vd_sym(Object));
VD_IMPLEMENT_OBJECT(GadgetDataset, vd_sym(GadgetDataset), vd_sym(Object));
VD_IMPLEMENT_OBJECT(GadgetWorkQueue, vd_sym(GadgetWorkQueue), vd_sym(WorkQueue));
VD_IMPLEMENT_OBJECT(GadgetWorkItem, vd_sym(GadgetWorkItem), vd_sym(WorkItem));

// ============================================================================================== //

VD_FORMATS_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_USE_GS2

// ============================================================================================== //

