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

#ifndef VD_CORE_INTERPOLATE_INCLUDED
#define VD_CORE_INTERPOLATE_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "constants/values.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

template<typename Real>
Real Normalize(
    const Real& value, const Real& low, const Real& high)
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
Real LinearRemap(
    const Real& x,
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

// ============================================================================================== //

template<class T>
inline void FindBarycentric(
    T x, int& i, T& f, int i_low, int i_high)
{
    T s = Floor(x);
    i = (int)s;

    if(i < i_low)
    {
        i=i_low;
        f=0;
    }
    else if(i > i_high-2)
    {
        i = i_high-2;
        f = 1;
    }
    else
    {
        f=(T)(x-s);
    }
}

template<class T, class Real>
inline T Lerp(
    const T& v0, const T& v1, Real f)
{
    const Real one_minus_f = Real(Constants::One) - f;
    return (one_minus_f * v0) + (f * v1);
}

template<class S, class T>
inline S Bilerp(
    const S& v00, const S& v10,
    const S& v01, const S& v11,
    T fx, T fy)
{
    return Lerp(
        Lerp(v00, v10, fx),
        Lerp(v01, v11, fx),
        fy
    );
}

template<class S, class T>
inline S Trilerp(
    const S& v000, const S& v100,
    const S& v010, const S& v110,
    const S& v001, const S& v101,
    const S& v011, const S& v111,
    T fx, T fy, T fz)
{
    return Lerp(
       Bilerp(v000, v100, v010, v110, fx, fy),
       Bilerp(v001, v101, v011, v111, fx, fy),
       fz
   );
}

template<class S, class T>
inline S Quadlerp(
    const S& v0000, const S& v1000,
    const S& v0100, const S& v1100,
    const S& v0010, const S& v1010,
    const S& v0110, const S& v1110,
    const S& v0001, const S& v1001,
    const S& v0101, const S& v1101,
    const S& v0011, const S& v1011,
    const S& v0111, const S& v1111,
    T fx, T fy, T fz, T ft)
{
    return Lerp(
       Trilerp(v0000, v1000, v0100, v1100, v0010, v1010, v0110, v1110, fx, fy, fz),
       Trilerp(v0001, v1001, v0101, v1101, v0011, v1011, v0111, v1111, fx, fy, fz),
       ft
    );
}

template<class S, class Real>
inline S CubicLerp(
    const S& vn1, const S& v0, const S& v1, const S& v2,
    Real f)
{
    v4<Real> w = CubicWeights(f);
    Real n1 = w[0];
    Real w0 = w[1];
    Real w1 = w[2];
    Real w2 = w[3];
    return n1 * vn1 + w0 * v0 + w1 * v1 + w2 * v2;
}

template<class T>
inline T 
SmoothStep(T r)
{
    static const T ten(10);
    static const T nft(-15);
    static const T six(6);
    static const T zero(0);
    static const T one(1);

    if(r < zero) return zero;
    else if(r > one) return one;
    return r * r * r * ( ten + r * (nft + r * six));
}

template<class T>
inline T 
SmoothStep(
    T r, T r_lower, T r_upper, T value_lower, T value_upper)
{
    return value_lower + SmoothStep( (r - r_lower)/(r_upper-r_lower)) * (value_upper-value_lower);
}

template<typename Real>
inline Real 
Ramp(Real r)
{
    return SmoothStep(
        (r + Real(Constants::One)) / Real(Constants::Two)) * 
        Real(Constants::Two) - Real(Constants::One);
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_INTERPOLATE_INCLUDED


