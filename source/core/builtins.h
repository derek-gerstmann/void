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
#include "core/numerics.h"
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
int Compare(const T& a, const T& b) {
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

template<typename Real> 
Real Normalize(const Real& value, const Real& low, const Real& high)
{
	Real range(high - low);
	Real result = Clamp(value, low, high);
    return (result - low) / range;
}

// ============================================================================================== //
//
// Remaps X in [A,B] to X' in [C,D]
//
//       (D-C)*(X-A)
// X' =   -----------  + C
//          (B-A)
//
// ============================================================================================== //
       
template<typename Real> 
Real LinearRemap(const Real& x, 
    const Real& a, const Real& b,
    const Real& c, const Real& d)
{
    Real xp = ((d - c) * (x - a) / (b - a)) + c;
    return xp;
}
          
template<typename Real> 
Real Clamp(const Real& value, const Real& low, const Real& high)
{
	Real result(value);
    if (result < low) 	result = low;
    if (result > high) 	result = high;
    return result;
}

template<typename Real, typename T> T 
Lerp(Real t, const T& a, const T& b)
{
	Real ct = Clamp(t, Real(Constants::Zero), Real(Constants::One));
	return a * (Real(Constants::One) - ct) + b * ct;
}

template<typename Real>
Real SmoothIdentity( Real x, Real m, Real n )
{
    if( x > m ) return x;

    const Real one = Real(Constants::One);
    const Real two = Real(Constants::Two);
    const Real three = Real(Constants::Three);
    const Real a = two * n - m;
    const Real b = two * m - three * n;
    const Real t = x / m;

    return (a * t + b) * t * t + n;
}

// ============================================================================================== //

template<typename Real>
Real Impulse( Real k, Real x )
{
    const Real one = Real(Constants::One);
    const Real h = k * x;
    return h * Exp(one - h);
}

template<typename Real>
Real CubicPulse( Real c, Real w, Real x )
{
	const Real zero = Real(Constants::Zero);
    const Real one = Real(Constants::One);
    const Real two = Real(Constants::Two);
    const Real three = Real(Constants::Three);

    x = Abs(x - c);
    if( x > w ) return zero;
    x /= w;
    return one - x * x * (three - two * x);
}

template<typename Real>
Real ExpPulse( Real x, Real k, Real n )
{
    return Exp( -k * Pow(x,n) );
}

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

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_BUILTINS_INCLUDED


