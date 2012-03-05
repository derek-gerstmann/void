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

#include "core/builtins.h"
#include <cmath>

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

vd::f32 
FastSqrt(vd::f32 r) 
{
	vd::f32 x,y;
	vd::f32 v = r;
	unsigned int *up = ((unsigned int *)&v)+1;

	(*up) = (0xbfcd4600-(*up))>>1;
	x=v;
	y=r*0.5f; 
	x*=1.5f-x*x*y; 
	x*=1.5f-x*x*y;
	x*=1.5f-x*x*y; 
	x*=1.5f-x*x*y; 
	return x*r;
}

vd::f32 
FastRsqrt(vd::f32 v)
{
    vd::f32 h = 0.5f * v;
    int i = *(int*)&v;
    i = 0x5f3759df - (i >> 1);
    v = *(vd::f32*)&i;
    v = v*(1.5f - h * v * v);
    return v;
}

// ============================================================================================== //

unsigned int
NearestPowerOfTwo(unsigned int x)
{
    int i, k;

    k = x;
    i = -1;
    while (k != 0)
    {
        k >>= 1;
        i++;
    }
    return 1 << (i + ((x >> (i - 1)) & 1));
}

int 
DilateEven(const int x)
{
	int u = ((x & 0x0000ff00) << 8) | (x & 0x000000ff);
	int v = ((u & 0x00f000f0) << 4) | (u & 0x000f000f);
	int w = ((v & 0x0c0c0c0c) << 2) | (v & 0x03030303);
	int r = ((w & 0x22222222) << 1) | (w & 0x11111111);
	return r;
}

int
DilateOdd(const int x)
{
    return (DilateEven(x) << 1);
}

int
MortonIndex2D(
    const int row, const int col)
{
    return (DilateEven(row) | DilateOdd(col));
}

int
MortonIndex2DPadded(
    const int row, const int col, const int size)
{
    return ((row < size) ? ((col < size) ? MortonIndex2D(row, col) : (size) * (size) + row) : (size) * (size + 1) + col);
}

// ============================================================================================== //

int 
DivideUp(int a, int b) 
{
    return ((a % b) != 0) ? (a / b + 1) : (a / b);
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

