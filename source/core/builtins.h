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

#ifndef VD_CORE_BUILTINS_INCLUDED
#define VD_CORE_BUILTINS_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "constants/values.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

vd::f32
FastSqrt(const vd::f32 fV);

vd::f32
FastRsqrt(const vd::f32 fV);

vd::i32
DivideUp(vd::i32 a, vd::i32 b);

vd::u32
NearestPowerOfTwo(vd::u32 x);

// --------------------------------------------------------------------------------------------- //

int
DilateEven(const int iX);

int
DilateOdd(const int iX);

// --------------------------------------------------------------------------------------------- //

int
MortonIndex2D(const int row, const int col);

int
MortonIndex2DPadded(const int row, const int col, const int size);

// --------------------------------------------------------------------------------------------- //

template <typename T>
int Compare(const T& a, const T& b)
{
    if (a == b)
        return 0;
    else if (a < b)
        return -1;
    else
        return 1;
}

template <typename T>
int PointerValueCompare(const T* a, const T* b)
{
    return Compare<T>(*a, *b);
}

inline vd::u32 RoundUpToPowerOf2(vd::u32 x)
{
//    vdGlobalAssert(x <= 0x80000000u);
    x = x - 1;
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
    return x + 1;
}

inline vd::u32 RoundDownToPowerOf2(vd::u32 x)
{
    vd::u32 rounded_up = RoundUpToPowerOf2(x);
    if (rounded_up > x) return rounded_up >> 1;
    return rounded_up;
}

// ============================================================================================== //

#if 0
template<typename Real>
Real PoissonDistribution(Real rate)
{
    const Real one = Real(Constants::One);
    return -Log(one - ((Real) random() / (RAND_MAX + 1)) / rate;
}
#endif

// ============================================================================================== //

template<typename Integer>
Integer RoundToMultipleOf2( Integer x )
{
    return (((x) + 1) >> 1 << 1);
}

template<typename Integer>
Integer RoundToMultipleOf4( Integer x )
{
    return (((x) + 3) >> 2 << 2);
}

// ============================================================================================== //

template<class T>
inline void MinMax(
    T a1, T a2, T& amin, T& amax)
{
    if(a1<a2)
    {
        amin=a1;
        amax=a2;
    }
    else
    {
        amin=a2;
        amax=a1;
    }
}

template<class T>
inline void MinMax(
    T a1, T a2, T a3, T& amin, T& amax)
{
    if(a1<a2)
    {
        if(a1<a3)
        {
            amin=a1;
            if(a2<a3) amax=a3;
            else amax=a2;
        }
        else
        {
            amin=a3;
            if(a1<a2) amax=a2;
            else amax=a1;
        }
    }
    else
    {
        if(a2<a3)
        {
            amin=a2;
            if(a1<a3) amax=a3;
            else amax=a1;
        }
        else
        {
            amin=a3;
            amax=a1;
        }
    }
}

template<class T>
inline void MinMax(
    T a1, T a2, T a3, T a4, T& amin, T& amax)
{
    if(a1<a2)
    {
        if(a3<a4)
        {
            amin = Min(a1,a3);
            amax = Max(a2,a4);
        }
        else
        {
            amin = Min(a1,a4);
            amax = Max(a2,a3);
        }
    }
    else
    {
        if(a3<a4)
        {
            amin = Min(a2,a3);
            amax = Max(a1,a4);
        }
        else
        {
            amin = Min(a2,a4);
            amax = Max(a1,a3);
        }
    }
}

template<class T>
inline void MinMmax(
    T a1, T a2, T a3, T a4, T a5,
    T& amin, T& amax)
{
    amin = Min(a1,a2,a3,a4,a5);
    amax = Max(a1,a2,a3,a4,a5);
}

template<class T>
inline void MinMax(
    T a1, T a2, T a3, T a4, T a5, T a6,
    T& amin, T& amax)
{
    amin = Min(a1,a2,a3,a4,a5,a6);
    amax = Max(a1,a2,a3,a4,a5,a6);
}

template<class T>
inline void UpdateMinMax(
    T a1, T& amin, T& amax)
{
    if(a1 < amin) amin = a1;
    else if(a1 > amax) amax = a1;
}

template<class T>
inline void Sort(T &a, T &b, T &c)
{
    T temp;
    if(a<b)
    {
        if(a<c)
        {
            if(c<b)
            {
                // a<c<b
                temp=c;
                c=b;
                b=temp;
            } // else: a<b<c
        }
        else
        {
            // c<a<b
            temp=c;
            c=b;
            b=a;
            a=temp;
        }
    }
    else
    {
        if(b<c)
        {
            if(a<c)
            {
                //b<a<c
                temp=b;
                b=a;
                a=temp;
            }
            else
            {
                // b<c<a
                temp=b;
                b=c;
                c=a;
                a=temp;
            }
        }
        else
        {
            // c<b<a
            temp=c;
            c=a;
            a=temp;
        }
    }
}

#if defined(VD_TARGET_WINDOWS)
inline int 
lround(double x)
{
    if(x>0)
        return (x - Floor(x) < 0.5) ? (int)Floor(x) : (int)Ceil(x);
    else
        return (x - Floor(x) <= 0.5) ? (int)Floor(x) : (int)Ceil(x);
}

inline double 
remainder(double x, double y)
{
    return x - Floor(x/y+0.5)*y;
}
#endif

inline vd::u32
RoundUpPow2(vd::u32 n)
{
    vd::i32 exponent=0;
    --n;
    while(n)
    {
        ++exponent;
        n >>= 1;
    }
    return 1 << exponent;
}

inline vd::u32
RoundDownPow2(vd::u32 n)
{
    vd::i32 exponent=0;
    while(n > 1)
    {
        ++exponent;
        n >>= 1;
    }
    return 1 << exponent;
}

inline vd::i32 
IntLog2(vd::i32 x)
{
    vd::i32 exp = -1;
    while(x)
    {
        x >>= 1;
        ++exp;
    }
    return exp;
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_BUILTINS_INCLUDED


