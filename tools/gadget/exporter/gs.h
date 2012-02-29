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

#ifndef __GS_H__
#define __GS_H__

/**************************************************************************************************/

#include "gscr.h"

#include <stdlib.h>
#include <stdio.h>

/**************************************************************************************************/

typedef struct _gs_header_fields_t 
{
    int ParticleCount[6];                           //!< Number of particles of each type in this file 
    double Mass[6];                                 //!< Mass of particles of each type. If 0, then the masses are explicitly
                                                    //   stored in the mass-block of the snapshot file, otherwise they are omitted 
    double Time;                                    //!< Time of snapshot file 
    double Redshift;                                //!< Redshift of snapshot file 
    int HasStarFormation;                           //!< Flags whether the simulation was including star formation 
    int HasFeedback;                                //!< Flags whether feedback was included (obsolete) 
    unsigned int ParticleCountTotal[6];             //!< Total number of particles of each type in this snapshot. This can be
                                                    //   different from ParticleCount if one is dealing with a multi-file snapshot. 
    int HasCooling;                                 //!< Flags whether cooling was included  
    int FileCountPerSnapshot;                       //!< Number of files in multi-file snapshot 
    double BoxSize;                                 //!< Box-size of simulation in case periodic boundaries were used 
    double Omega0;                                  //!< Matter density in units of critical density 
    double OmegaLambda;                             //!< Cosmological constant parameter 
    double HubbleParam;                             //!< Hubble parameter in units of 100 km/sec/Mpc 
    int HasStellarAge;                              //!< Flags whether the file contains formation times of star particles 
    int HasMetals;                                  //!< Flags whether the file contains metallicity values for gas and star particles 
    unsigned int ParticleCountTotalHighWord[6];     //!< High word of the total number of particles of each type 
    int  HasEntropyInsteadofU;                      //!< Flags that IC-file contains entropy instead of u 

} gs_header_fields_t;

typedef union _gs_header_t
{
    gs_header_fields_t Fields;              //!< Header field data
	char Padding[256];	               	    //!< Padding fills to 256 Bytes 
} gs_header_t;

typedef struct _gs_particle_data_t
{
	int Id;
    int Type;
    float Pos[3];
    float Vel[3];
    float Mass;
    float Rho;
    float U;
    float Temp;
    float Ne;
    float Hsml;
    float Potential;
    float Acceleration;
    float DtEntropy;
    float TimeStep;
} gs_particle_data_t;

typedef struct _gs_particle_soa_t
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
} gs_particle_soa_t;

typedef struct _gs_data_t
{
	gs_header_t Header;
	gs_particle_soa_t Data;
	unsigned long TotalParticleCount;
	unsigned long GasParticleCount;
} gs_data_t;

typedef enum _gs_block_type_t
{
	GADGET_IO_POS		=  0,
	GADGET_IO_VEL		=  1,
	GADGET_IO_ID		=  2,
	GADGET_IO_MASS		=  3,
	GADGET_IO_U			=  4,
	GADGET_IO_RHO		=  5,
	GADGET_IO_HSML		=  6,
	GADGET_IO_POT		=  7,
	GADGET_IO_ACCEL		=  8,
	GADGET_IO_DTENTR	=  9,
	GADGET_IO_TSTP		= 10,
	GADGET_IO_NBLOCKS	= 11,
	GADGET_IO_TYPE		= 12,		// not stored as separate block, but inferred from file
} gs_block_type_t;

typedef enum _gs_particle_type_t
{
	GADGET_PT_GAS		=  0,
	GADGET_PT_HALO		=  1,
	GADGET_PT_DISK		=  2,
	GADGET_PT_BULGE		=  3,
	GADGET_PT_STARS		=  4,
	GADGET_PT_BNDRY		=  5,
	GADGET_PT_NTYPES	=  6
} gs_particle_type_t;

/**************************************************************************************************/

extern gs_data_t*
gsAllocateData();

extern void 
gsDestroyData(gs_data_t* gs);

extern int
gsLoadSnapshot(gs_data_t* gs, const char* prefix, int splits, int* req_blocks, int* req_types);

extern size_t 
gsAllocateBlockData( gs_data_t* gs, gs_block_type_t block);

extern void 
gsDestroyBlockData( gs_data_t* gs, gs_block_type_t block);

extern void*
gsGetBlockDataPtr(gs_data_t* gs, gs_block_type_t block);

extern void
gsSetBlockDataPtr(gs_data_t* gs, gs_block_type_t block, void* ptr);
	
extern int 
gsReadHeader(FILE* file, gs_data_t* gs, int splits);

extern int 
gsReadBlock(FILE* file, gs_data_t* gs, gs_block_type_t block, int start);

extern int 
gsSkipBlock(FILE* file, gs_data_t* gs, gs_block_type_t block, int start);

extern const char*
gsGetBlockLabel(gs_block_type_t block);

extern const char*
gsGetBlockName(gs_block_type_t block);

extern size_t
gsGetBlockEntrySize(gs_block_type_t block);

extern size_t
gsGetBlockVectorSize(gs_block_type_t block);

extern int
gsIsBlockIntegerData(gs_block_type_t block);

const char*
gsGetBlockSizeTypeSuffix(gs_block_type_t block);

extern void 
gsConvertUnits(gs_data_t* gs);
	
extern float
gsPeriodic(float x, float l2);

extern void
gsFindExtents(gs_data_t* gs, float minbounds[3], float maxbounds[3]);

extern void 
gsReorder(gs_data_t* gs);

/**************************************************************************************************/

#endif /* __GS_H__ */

