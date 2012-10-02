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
//
// Based on Intel math primitives contained in Embree -- License follows:
//
// ============================================================================================== //
// Copyright 2009-2011 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ============================================================================================== //

#ifndef VD_CORE_NUMERICS_INCLUDED
#define VD_CORE_NUMERICS_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/builtins.h"
#include "constants/types.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

#if defined(__WIN32__)
#undef min
#undef max
#if defined(__MSVC__)
VD_FORCE_INLINE bool finite(vd::f32 x) { return _finite(x) != 0; }
#endif
#endif

// ---------------------------------------------------------------------------------------------- //

VD_FORCE_INLINE
vd::i32 Abs(vd::i32 x)
{
    return (x > vd::i32(Constants::Zero)) ? x : -x;
}

VD_FORCE_INLINE
vd::i64 Abs(vd::i64 x)
{
    return (x > vd::i64(Constants::Zero)) ? x : -x;
}

// ---------------------------------------------------------------------------------------------- //

VD_FORCE_INLINE
vd::f32 Sign(vd::f32 x)
{
    return x < 0 ? -1.0f : 1.0f;
}

VD_FORCE_INLINE
vd::f32 Sqr(vd::f32 x)
{
    return x * x;
}

VD_FORCE_INLINE
vd::f32 Rcp(vd::f32 x)
{
    return 1.0f / (VD_F32_TINY_VAL + x);
}

VD_FORCE_INLINE
vd::f32 RcpSqrt(vd::f32 x)
{
    return 1.0f / (VD_F32_TINY_VAL + ::sqrtf(x));
}

VD_FORCE_INLINE
vd::f32 Abs(vd::f32 x)
{
    return ::fabsf(x);
}

VD_FORCE_INLINE
vd::f32 ArcCos(vd::f32 x)
{
    return ::acosf(x);
}

VD_FORCE_INLINE
vd::f32 ArcSin(vd::f32 x) 
{ 
    return ::asinf(x); 
}

VD_FORCE_INLINE
vd::f32 ArcTan(vd::f32 x)
{
    return ::atanf(x);
}

VD_FORCE_INLINE
vd::f32 Cos(vd::f32 x)
{
    return ::cosf(x);
}

VD_FORCE_INLINE
vd::f32 Cosh(vd::f32 x)
{
    return ::coshf(x);
}

VD_FORCE_INLINE
vd::f32 Exp(vd::f32 x)
{
    return ::expf(x);
}

VD_FORCE_INLINE
vd::f32 Log(vd::f32 x)
{
    return ::logf(x);
}

VD_FORCE_INLINE
vd::f32 Log2(vd::f32 x)
{
    return ::log(x) / 0.69314718055994530941723212145818f;
}

VD_FORCE_INLINE
vd::f32 Log10(vd::f32 x)
{
    return ::log10f(x);
}

VD_FORCE_INLINE
vd::f32 Sin(vd::f32 x)
{
    return ::sinf(x);
}

VD_FORCE_INLINE
vd::f32 Sinh(vd::f32 x)
{
    return ::sinhf(x);
}

VD_FORCE_INLINE
vd::f32 Sqrt(vd::f32 x)
{
    return ::sqrtf(x);
}

VD_FORCE_INLINE
vd::f32 Tan(vd::f32 x)
{
    return ::tanf(x);
}

VD_FORCE_INLINE
vd::f32 Tanh(vd::f32 x)
{
    return ::tanhf(x);
}

VD_FORCE_INLINE
vd::f32 Floor(vd::f32 x)
{
    return ::floorf(x);
}

VD_FORCE_INLINE
vd::f32 Ceil(vd::f32 x)
{
    return ::ceilf(x);
}

VD_FORCE_INLINE
vd::f32 ArcTan2(vd::f32 y, vd::f32 x)
{
    return ::atan2f(y, x);
}

VD_FORCE_INLINE
vd::f32 FMod(vd::f32 x, vd::f32 y)
{
    return ::fmodf(x, y);
}

VD_FORCE_INLINE
vd::f32 Pow(vd::f32 x, vd::f32 y)
{
    return ::powf(x, y);
}

VD_FORCE_INLINE
bool IsWithinUlps(
    vd::f32 x, vd::f32 y, vd::f32 ulps)
{
    union value {
        vd::f32 f;
        vd::i32 i;
    };

    vdStaticAssert( sizeof(vd::f32) == sizeof(vd::i32), TypeSizeMisMatch);

    if (x == y)
        return true;

    value vx;
    value vy;
    
    vx.f = x;
    vy.f = y;

    vd::i32 delta = Abs(vx.i - vy.i);
    if (delta <= ulps)
        return true;
    return false;
}

// ---------------------------------------------------------------------------------------------- //

VD_FORCE_INLINE
vd::f64 Abs(vd::f64 x)
{
    return ::fabs(x);
}

VD_FORCE_INLINE
vd::f64 Sign(vd::f64 x)
{
    return x < 0 ? -1.0 : 1.0;
}

VD_FORCE_INLINE
vd::f64 ArcCos(vd::f64 x)
{
    return ::acos(x);
}

VD_FORCE_INLINE
vd::f64 ArcSin(vd::f64 x)
{
    return ::asin(x);
}

VD_FORCE_INLINE
vd::f64 ArcTan(vd::f64 x)
{
    return ::atan(x);
}

VD_FORCE_INLINE
vd::f64 Cos(vd::f64 x)
{
    return ::cos(x);
}

VD_FORCE_INLINE
vd::f64 Cosh(vd::f64 x)
{
    return ::cosh(x);
}

VD_FORCE_INLINE
vd::f64 Exp(vd::f64 x)
{
    return ::exp(x);
}

VD_FORCE_INLINE
vd::f64 Log(vd::f64 x)
{
    return ::log(x);
}

VD_FORCE_INLINE
vd::f64 Log2(vd::f64 x)
{
    return ::log(x) / 0.69314718055994530941723212145818;
}

VD_FORCE_INLINE
vd::f64 Log10(vd::f64 x)
{
    return ::log10(x);
}

VD_FORCE_INLINE
vd::f64 Rcp(vd::f64 x)
{
    return 1.0 / (VD_F64_TINY_VAL + x);
}

VD_FORCE_INLINE
vd::f64 RcpSqrt(vd::f64 x)
{
    return 1.0 / (VD_F64_TINY_VAL + ::sqrt(x));
}

VD_FORCE_INLINE
vd::f64 Sin(vd::f64 x)
{
    return ::sin(x);
}

VD_FORCE_INLINE
vd::f64 Sinh(vd::f64 x)
{
    return ::sinh(x);
}

VD_FORCE_INLINE
vd::f64 Sqr(vd::f64 x)
{
    return x * x;
}

VD_FORCE_INLINE
vd::f64 Sqrt(vd::f64 x)
{
    return ::sqrt(x);
}

VD_FORCE_INLINE
vd::f64 Tan(vd::f64 x)
{
    return ::tan(x);
}

VD_FORCE_INLINE
vd::f64 Tanh(vd::f64 x)
{
    return ::tanh(x);
}

VD_FORCE_INLINE
vd::f64 Floor(vd::f64 x)
{
    return ::floor(x);
}

VD_FORCE_INLINE
vd::f64 Ceil(vd::f64 x)
{
    return ::ceil(x);
}

VD_FORCE_INLINE
vd::f64 ArcTan2(vd::f64 y, vd::f64 x)
{
    return ::atan2(y, x);
}

VD_FORCE_INLINE
vd::f64 FMod(vd::f64 x, vd::f64 y)
{
    return ::fmod(x, y);
}

VD_FORCE_INLINE
vd::f64 Pow(vd::f64 x, vd::f64 y)
{
    return ::pow(x, y);
}

VD_FORCE_INLINE
bool IsWithinUlps(
    vd::f64 x, vd::f64 y, vd::f64 ulps)
{

    union value {
        vd::f64 f;
        vd::i64 i;
    };

    vdStaticAssert( sizeof(vd::f32) == sizeof(vd::i32), TypeSizeMisMatch);

    if (x == y)
        return true;

    value vx;
    value vy;
    
    vx.f = x;
    vy.f = y;

    vd::i64 delta = Abs(vx.i - vy.i);
    if (delta <= ulps)
        return true;

    return false;
}

// ---------------------------------------------------------------------------------------------- //

template <typename T> VD_FORCE_INLINE
T Max(T a, T b)
{
    return a < b ? b : a;
}

template <typename T> VD_FORCE_INLINE
T Min(T a, T b)
{
    return a < b ? a : b;
}

template <typename T> VD_FORCE_INLINE
T Min(T a, T b, T c)
{
    return Min(Min(a, b), c);
}

template <typename T> VD_FORCE_INLINE
T Max(T a, T b, T c)
{
    return Max(Max(a, b), c);
}

template <typename T> VD_FORCE_INLINE
T Max(T a, T b, T c, T d)
{
    return Max(Max(a, b), Max(c, d));
}

template <typename T> VD_FORCE_INLINE
T Min(T a, T b, T c, T d)
{
    return Min(Min(a, b), Min(c, d));
}

template <typename T> VD_FORCE_INLINE
T Min(T a, T b, T c, T d, T e)
{
    return Min(Min(Min(a, b), Min(c, d)), e);
}

template <typename T> VD_FORCE_INLINE
T Max(T a, T b, T c, T d, T e)
{
    return Max(Max(Max(a, b), Max(c, d)), e);
}

template <typename T> VD_FORCE_INLINE
T Clamp(T x, T lower = T(Constants::Zero), T upper = T(Constants::One))
{
    return Max(lower, Min(x, upper));
}

template <typename T> VD_FORCE_INLINE
T ToRadians(T x)
{
    return x * T(1.74532925199432957692e-2f);
}

template <typename T> VD_FORCE_INLINE
T Deg2Rad(T x)
{
    return x * T(1.74532925199432957692e-2f);
}

template <typename T> VD_FORCE_INLINE
T ToDegrees(T x)
{
    return x * T(5.72957795130823208768e1f);
}

template <typename T> VD_FORCE_INLINE
T Rad2Deg(T x)
{
    return x * T(5.72957795130823208768e1f);
}

template <typename T> VD_FORCE_INLINE
T Sin2Cos(T x)
{
    return Sqrt(Max( T(Constants::Zero), T(Constants::One) - x * x));
}

template <typename T> VD_FORCE_INLINE
T Cos2Sin(T x)
{
    return Sin2Cos(x);
}

// ============================================================================================== //

template<typename T>
struct v1
{
    T x, y;
    typedef T ScalarType;
    enum {N = 1};

    VD_FORCE_INLINE
    v1() {}

    VD_FORCE_INLINE
    v1(const v1& v) { x = v.x; }

    template<typename T1> VD_FORCE_INLINE
    v1(const v1<T1>& a) : x(T(a.x)) {}

    template<typename T1> VD_FORCE_INLINE
    v1& operator= (const v1<T1>& v) {x = v.x; return *this; }

    VD_FORCE_INLINE explicit
    v1(const T& a) : x(a), y(a) {}

    VD_FORCE_INLINE explicit
    v1(const T& x, const T& y) : x(x), y(y) {}

    VD_FORCE_INLINE explicit
    v1(const T* a, vd::u32 stride = 1) : x(a[0]), y(a[stride]) {}

    VD_FORCE_INLINE
    v1(Constants::ZeroValue) : x(Constants::Zero), y(Constants::Zero) {}

    VD_FORCE_INLINE
    v1(Constants::OneValue) : x(Constants::One), y(Constants::One) {}

    VD_FORCE_INLINE
    v1(Constants::PosInfValue) : x(Constants::PosInf), y(Constants::PosInf) {}

    VD_FORCE_INLINE
    v1(Constants::NegInfValue) : x(Constants::NegInf), y(Constants::NegInf) {}

    VD_FORCE_INLINE const T&
    operator [](size_t axis) const
    {
        assert(axis < 2);
        return (&x)[axis];
    }

    VD_FORCE_INLINE T&
    operator [](size_t axis)       
    {
        assert(axis < 2); 
        return (&x)[axis]; 
    }
};

// ---------------------------------------------------------------------------------------------- //

template <typename T> VD_FORCE_INLINE
v1<T> operator+ (const v1<T>& a)
{
    return v1<T>(+a.x);
}

template <typename T> VD_FORCE_INLINE
v1<T> operator- (const v1<T>& a)
{
    return v1<T>(-a.x);
}

template <typename T> VD_FORCE_INLINE
v1<T> Abs(const v1<T>& a)
{
    return v1<T>(Abs(a.x));
}

template <typename T> VD_FORCE_INLINE
v1<T> Rcp(const v1<T>& a)
{
    return v1<T>(Rcp(a.x));
}

template <typename T> VD_FORCE_INLINE
v1<T> Sqrt(const v1<T>& a)
{
    return v1<T>(Sqrt(a.x));
}

template <typename T> VD_FORCE_INLINE
v1<T> RcpSqrt(const v1<T>& a)
{
    return v1<T>(RcpSqrt(a.x));
}

template <typename T> VD_FORCE_INLINE
v1<T> operator+ (const v1<T>& a, const v1<T>& b)
{
    return v1<T>(a.x + b.x);
}

template <typename T> VD_FORCE_INLINE
v1<T> operator- (const v1<T>& a, const v1<T>& b)
{
    return v1<T>(a.x - b.x);
}

template <typename T> VD_FORCE_INLINE
v1<T> operator* (const v1<T>& a, const v1<T>& b)
{
    return v1<T>(a.x * b.x);
}

template <typename T> VD_FORCE_INLINE
v1<T> operator* (const T& a, const v1<T>& b)
{
    return v1<T>(a * b.x);
}

template <typename T> VD_FORCE_INLINE
v1<T> operator* (const v1<T>& a, const T& b)
{
    return v1<T>(a.x * b);
}

template <typename T> VD_FORCE_INLINE
v1<T> operator/ (const v1<T>& a, const v1<T>& b)
{
    return v1<T>(a.x / b.x);
}

template <typename T> VD_FORCE_INLINE
v1<T> operator/ (const v1<T>& a, const T& b)
{
    return v1<T>(a.x / b.x);
}

template <typename T> VD_FORCE_INLINE
v1<T> operator/ (const T& a, const v1<T>& b)
{
    return v1<T>(a / b.x);
}

template <typename T> VD_FORCE_INLINE
v1<T> Min(const v1<T>& a, const v1<T>& b)
{
    return v1<T>(Min(a.x, b.x));
}

template <typename T> VD_FORCE_INLINE
v1<T> Max(const v1<T>& a, const v1<T>& b)
{
    return v1<T>(Max(a.x, b.x));
}

template <typename T> VD_FORCE_INLINE
T Dot(const v1<T>& a, const v1<T>& b)
{
    return a.x * b.x;
}

template <typename T> VD_FORCE_INLINE
T Length(const v1<T>& a)
{
    return Sqrt(Dot(a, a));
}

template <typename T> VD_FORCE_INLINE
T LengthSqr(const v1<T>& a)
{
    return (Dot(a, a));
}

template <typename T> VD_FORCE_INLINE
v1<T> Normalise(const v1<T>& a)
{
    return a * RcpSqrt(Dot(a, a));
}

template <typename T> VD_FORCE_INLINE
T Distance(const v1<T>& a, const v1<T>& b)
{
    return Length(a - b);
}

template <typename T> VD_FORCE_INLINE
T DistanceSqr(const v1<T>& a, const v1<T>& b)
{
    return LengthSqr(a - b);
}

template <typename T> VD_FORCE_INLINE
v1<T>& operator+= (v1<T>& a, const v1<T>& b)
{
    a.x += b.x;
    return a;
}

template <typename T> VD_FORCE_INLINE
v1<T>& operator-= (v1<T>& a, const v1<T>& b)
{
    a.x -= b.x; 
    return a;
}

template <typename T> VD_FORCE_INLINE
v1<T>& operator*= (v1<T>& a, const T& b)
{
    a.x *= b; 
    return a;
}

template <typename T> VD_FORCE_INLINE
v1<T>& operator/= (v1<T>& a, const T& b)
{
    a.x /= b;
    return a;
}

template <typename T> VD_FORCE_INLINE
T ReduceAdd(const v1<T>& a)
{
    return a.x;
}

template <typename T> VD_FORCE_INLINE
T ReduceMultiply(const v1<T>& a)
{
    return a.x;
}

template <typename T> VD_FORCE_INLINE
T ReduceMin(const v1<T>& a)
{
    return a.x;
}

template <typename T> VD_FORCE_INLINE
T ReduceMax(const v1<T>& a)
{
    return a.x;
}

template <typename T> VD_FORCE_INLINE
bool operator== (const v1<T>& a, const v1<T>& b)
{
    return a.x == b.x;
}

template <typename T> VD_FORCE_INLINE
bool operator!= (const v1<T>& a, const v1<T>& b)
{
    return a.x != b.x;
}

template <typename T> VD_FORCE_INLINE
bool operator< (const v1<T>& a, const v1<T>& b)
{
    if(a.x != b.x) 
        return a.x < b.x;

    return false;
}

template <typename T> VD_FORCE_INLINE
v1<T> Select(bool s, const v1<T>& t, const v1<T>& f)
{
    return v1<T>(Select(s, t.x, f.x));
}

template <typename T> VD_FORCE_INLINE
std::ostream& operator<<(std::ostream& cout, const v1<T>& a)
{
    return cout << "(" << a.x << ")";
}

// ============================================================================================== //

template<typename T>
struct v2
{
    T x, y;
    typedef T ScalarValue;
    enum {N = 2};

    VD_FORCE_INLINE
    v2() {}

    VD_FORCE_INLINE
    v2(const v2& v) {x = v.x; y = v.y; }

    template<typename T1> VD_FORCE_INLINE
    v2(const v2<T1>& a) : x(T(a.x)), y(T(a.y)) {}

    template<typename T1> VD_FORCE_INLINE
    v2& operator= (const v2<T1>& v) {x = v.x; y = v.y; return *this; }

    VD_FORCE_INLINE explicit
    v2(const T& a) : x(a), y(a) {}

    VD_FORCE_INLINE explicit
    v2(const T& x, const T& y) : x(x), y(y) {}

    VD_FORCE_INLINE explicit
    v2(const T* a, vd::u32 stride = 1) : x(a[0]), y(a[stride]) {}

    VD_FORCE_INLINE
    v2(Constants::ZeroValue) : x(Constants::Zero), y(Constants::Zero) {}

    VD_FORCE_INLINE
    v2(Constants::OneValue) : x(Constants::One), y(Constants::One) {}

    VD_FORCE_INLINE
    v2(Constants::PosInfValue) : x(Constants::PosInf), y(Constants::PosInf) {}

    VD_FORCE_INLINE
    v2(Constants::NegInfValue) : x(Constants::NegInf), y(Constants::NegInf) {}

    VD_FORCE_INLINE const T&
    operator [](size_t axis) const
    {
        assert(axis < 2);
        return (&x)[axis];
    }

    VD_FORCE_INLINE T&
    operator [](size_t axis)       
    {
        assert(axis < 2); 
        return (&x)[axis]; 
    }
};

// ============================================================================================== //

template <typename T> VD_FORCE_INLINE
v2<T> operator+ (const v2<T>& a)
{
    return v2<T>(+a.x, +a.y);
}

template <typename T> VD_FORCE_INLINE
v2<T> operator- (const v2<T>& a)
{
    return v2<T>(-a.x, -a.y);
}

template <typename T> VD_FORCE_INLINE
v2<T> Abs(const v2<T>& a)
{
    return v2<T>(Abs(a.x), Abs(a.y));
}

template <typename T> VD_FORCE_INLINE
v2<T> Rcp(const v2<T>& a)
{
    return v2<T>(Rcp(a.x), Rcp(a.y));
}

template <typename T> VD_FORCE_INLINE
v2<T> Sqrt(const v2<T>& a)
{
    return v2<T>(Sqrt(a.x), Sqrt(a.y));
}

template <typename T> VD_FORCE_INLINE
v2<T> RcpSqrt(const v2<T>& a)
{
    return v2<T>(RcpSqrt(a.x), RcpSqrt(a.y));
}

template <typename T> VD_FORCE_INLINE
v2<T> operator+ (const v2<T>& a, const v2<T>& b)
{
    return v2<T>(a.x + b.x, a.y + b.y);
}

template <typename T> VD_FORCE_INLINE
v2<T> operator- (const v2<T>& a, const v2<T>& b)
{
    return v2<T>(a.x - b.x, a.y - b.y);
}

template <typename T> VD_FORCE_INLINE
v2<T> operator* (const v2<T>& a, const v2<T>& b)
{
    return v2<T>(a.x * b.x, a.y * b.y);
}

template <typename T> VD_FORCE_INLINE
v2<T> operator* (const T& a, const v2<T>& b)
{
    return v2<T>(a * b.x, a * b.y);
}

template <typename T> VD_FORCE_INLINE
v2<T> operator* (const v2<T>& a, const T& b)
{
    return v2<T>(a.x * b, a.y * b);
}

template <typename T> VD_FORCE_INLINE
v2<T> operator/ (const v2<T>& a, const v2<T>& b)
{
    return v2<T>(a.x / b.x, a.y / b.y);
}

template <typename T> VD_FORCE_INLINE
v2<T> operator/ (const v2<T>& a, const T& b)
{
    return v2<T>(a.x / b, a.y / b);
}

template <typename T> VD_FORCE_INLINE
v2<T> operator/ (const T& a, const v2<T>& b)
{
    return v2<T>(a / b.x, a / b.y);
}

template <typename T> VD_FORCE_INLINE
v2<T> Min(const v2<T>& a, const v2<T>& b)
{
    return v2<T>(Min(a.x, b.x), Min(a.y, b.y));
}

template <typename T> VD_FORCE_INLINE
v2<T> Max(const v2<T>& a, const v2<T>& b)
{
    return v2<T>(Max(a.x, b.x), Max(a.y, b.y));
}

template <typename T> VD_FORCE_INLINE
T Dot(const v2<T>& a, const v2<T>& b)
{
    return a.x * b.x + a.y * b.y;
}

template <typename T> VD_FORCE_INLINE
T Length(const v2<T>& a)
{
    return Sqrt(Dot(a, a));
}

template <typename T> VD_FORCE_INLINE
T LengthSqr(const v2<T>& a)
{
    return (Dot(a, a));
}

template <typename T> VD_FORCE_INLINE
v2<T> Normalise(const v2<T>& a)
{
    return a * RcpSqrt(Dot(a, a));
}

template <typename T> VD_FORCE_INLINE
T Distance(const v2<T>& a, const v2<T>& b)
{
    return Length(a - b);
}

template <typename T> VD_FORCE_INLINE
T DistanceSqr(const v2<T>& a, const v2<T>& b)
{
    return LengthSqr(a - b);
}

template <typename T> VD_FORCE_INLINE
v2<T>& operator+= (v2<T>& a, const v2<T>& b)
{
    a.x += b.x; a.y += b.y;
    return a;
}

template <typename T> VD_FORCE_INLINE
v2<T>& operator-= (v2<T>& a, const v2<T>& b)
{
    a.x -= b.x; a.y -= b.y;
    return a;
}

template <typename T> VD_FORCE_INLINE
v2<T>& operator*= (v2<T>& a, const T& b)
{
    a.x *= b; a.y *= b;
    return a;
}

template <typename T> VD_FORCE_INLINE
v2<T>& operator/= (v2<T>& a, const T& b)
{
    a.x /= b; a.y /= b;
    return a;
}

template <typename T> VD_FORCE_INLINE
T ReduceAdd(const v2<T>& a)
{
    return a.x + a.y;
}

template <typename T> VD_FORCE_INLINE
T ReduceMultiply(const v2<T>& a)
{
    return a.x * a.y;
}

template <typename T> VD_FORCE_INLINE
T ReduceMin(const v2<T>& a)
{
    return Min(a.x, a.y);
}

template <typename T> VD_FORCE_INLINE
T ReduceMax(const v2<T>& a)
{
    return Max(a.x, a.y);
}

template <typename T> VD_FORCE_INLINE
bool operator== (const v2<T>& a, const v2<T>& b)
{
    return a.x == b.x && a.y == b.y;
}

template <typename T> VD_FORCE_INLINE
bool operator!= (const v2<T>& a, const v2<T>& b)
{
    return a.x != b.x || a.y != b.y;
}

template <typename T> VD_FORCE_INLINE
bool operator< (const v2<T>& a, const v2<T>& b)
{
    if(a.x != b.x) 
        return a.x < b.x;

    if(a.y != b.y) 
        return a.y < b.y;

    return false;
}

template <typename T> VD_FORCE_INLINE
v2<T> Select(bool s, const v2<T>& t, const v2<T>& f)
{
    return v2<T>(Select(s, t.x, f.x), Select(s, t.y, f.y));
}

template <typename T> VD_FORCE_INLINE
std::ostream& operator<<(std::ostream& cout, const v2<T>& a)
{
    return cout << "(" << a.x << ", " << a.y << ")";
}

// ============================================================================================== //

template<typename T>
struct v3
{
    T x, y, z;
    typedef T ScalarType;
    enum {N = 3};

    VD_FORCE_INLINE
    v3() {}

    VD_FORCE_INLINE v3(const v3& v)
    {x = v.x; y = v.y; z = v.z; }

    template<typename T1> VD_FORCE_INLINE
    v3(const v3<T1>& a) : x(T(a.x)), y(T(a.y)), z(T(a.z))
    {
        // EMPTY!
    }

    template<typename T1> VD_FORCE_INLINE
    v3& operator= (const v3<T1>& v)
    {
        x = v.x; y = v.y; z = v.z;
        return *this;
    }

    VD_FORCE_INLINE explicit
    v3(const T& a) : x(a), y(a), z(a)
    {
        // EMPTY!
    }

    VD_FORCE_INLINE explicit
    v3(const T& x, const T& y, const T& z) : x(x), y(y), z(z)
    {
        // EMPTY!
    }

    VD_FORCE_INLINE explicit
    v3(const T* a, vd::u32 stride = 1) : x(a[0]), y(a[stride]), z(a[2 * stride])
    {
        // EMPTY!
    }

    VD_FORCE_INLINE
    v3(Constants::ZeroValue)   : x(Constants::Zero), y(Constants::Zero), z(Constants::Zero)
    {
        // EMPTY!
    }

    VD_FORCE_INLINE
    v3(Constants::OneValue)    : x(Constants::One), y(Constants::One), z(Constants::One)
    {
        // EMPTY!
    }

    VD_FORCE_INLINE
    v3(Constants::PosInfValue) : x(Constants::PosInf), y(Constants::PosInf), z(Constants::PosInf)
    {
        // EMPTY!
    }

    VD_FORCE_INLINE
    v3(Constants::NegInfValue) : x(Constants::NegInf), y(Constants::NegInf), z(Constants::NegInf)
    {
        // EMPTY!
    }

    VD_FORCE_INLINE const T&
    operator [](size_t axis) const
    {
        assert(axis < 3); return (&x)[axis];
    }

    VD_FORCE_INLINE T&
    operator [](size_t axis)
    {
        assert(axis < 3); return (&x)[axis];
    }
};

// ============================================================================================== //

template <typename T> VD_FORCE_INLINE
v3<T> Abs(const v3<T>& a)
{
    return v3<T>(Abs(a.x), Abs(a.y), Abs(a.z));
}

template <typename T> VD_FORCE_INLINE
v3<T> Rcp(const v3<T>& a)
{
    return v3<T>(Rcp(a.x), Rcp(a.y), Rcp(a.z));
}

template <typename T> VD_FORCE_INLINE
v3<T> Sqrt(const v3<T>& a)
{
    return v3<T>(Sqrt(a.x), Sqrt(a.y), Sqrt(a.z));
}

template <typename T> VD_FORCE_INLINE
v3<T> RcpSqrt(const v3<T>& a)
{
    return v3<T>(RcpSqrt(a.x), RcpSqrt(a.y), RcpSqrt(a.z));
}

template <typename T> VD_FORCE_INLINE
v3<T> operator+ (const v3<T>& a)
{
    return v3<T>(+a.x, +a.y, +a.z);
}

template <typename T> VD_FORCE_INLINE
v3<T> operator- (const v3<T>& a)
{
    return v3<T>(-a.x, -a.y, -a.z);
}

template <typename T> VD_FORCE_INLINE
v3<T> operator+ (const v3<T>& a, const v3<T>& b)
{
    return v3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template <typename T> VD_FORCE_INLINE
v3<T> operator- (const v3<T>& a, const v3<T>& b)
{
    return v3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template <typename T> VD_FORCE_INLINE
v3<T> operator* (const v3<T>& a, const v3<T>& b)
{
    return v3<T>(a.x * b.x, a.y * b.y, a.z * b.z);
}

template <typename T> VD_FORCE_INLINE
v3<T> operator/ (const v3<T>& a, const v3<T>& b)
{
    return v3<T>(a.x / b.x, a.y / b.y, a.z / b.z);
}

template <typename T> VD_FORCE_INLINE
v3<T> operator* (const v3<T>& a, const T& b)
{
    return v3<T>(a.x * b, a.y * b, a.z * b);
}

template <typename T> VD_FORCE_INLINE
v3<T> operator/ (const v3<T>& a, const T& b)
{
    return v3<T>(a.x / b, a.y / b, a.z / b);
}

template <typename T> VD_FORCE_INLINE
v3<T> operator* (const T& a, const v3<T>& b)
{
    return v3<T>(a * b.x, a * b.y, a * b.z);
}

template <typename T> VD_FORCE_INLINE
v3<T> operator/ (const T& a, const v3<T>& b)
{
    return v3<T>(a / b.x, a / b.y, a / b.z);
}

template <typename T> VD_FORCE_INLINE
v3<T> operator+= (v3<T>& a, const v3<T>& b)
{
    a.x += b.x; a.y += b.y; a.z += b.z; return a;
}

template <typename T> VD_FORCE_INLINE
v3<T> operator-= (v3<T>& a, const v3<T>& b)
{
    a.x -= b.x; a.y -= b.y; a.z -= b.z; return a;
}

template <typename T> VD_FORCE_INLINE
v3<T> operator*= (v3<T>& a, const T& b)
{
    a.x *= b; a.y *= b; a.z *= b; return a;
}

template <typename T> VD_FORCE_INLINE
v3<T> operator/= (v3<T>& a, const T& b)
{
    a.x /= b; a.y /= b; a.z /= b; return a;
}

template <typename T> VD_FORCE_INLINE
bool operator== (const v3<T>& a, const v3<T>& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

template <typename T> VD_FORCE_INLINE
bool operator!= (const v3<T>& a, const v3<T>& b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z;
}

template <typename T> VD_FORCE_INLINE
bool operator< (const v3<T>& a, const v3<T>& b)
{
    if(a.x != b.x) 
        return a.x < b.x;

    if(a.y != b.y) 
        return a.y < b.y;

    if(a.z != b.z) 
        return a.z < b.z;

    return false;
}

template <typename T> VD_FORCE_INLINE
T ReduceAdd(const v3<T>& a)
{
    return a.x + a.y + a.z;
}

template <typename T> VD_FORCE_INLINE
T ReduceMultiply(const v3<T>& a)
{
    return a.x * a.y * a.z;
}

template <typename T> VD_FORCE_INLINE
T ReduceMin(const v3<T>& a)
{
    return Min(a.x, a.y, a.z);
}

template <typename T> VD_FORCE_INLINE
T ReduceMax(const v3<T>& a)
{
    return Max(a.x, a.y, a.z);
}

template <typename T> VD_FORCE_INLINE
v3<T> Min(const v3<T>& a, const v3<T>& b)
{
    return v3<T>(Min(a.x, b.x), Min(a.y, b.y), Min(a.z, b.z));
}

template <typename T> VD_FORCE_INLINE
v3<T> Max(const v3<T>& a, const v3<T>& b)
{
    return v3<T>(Max(a.x, b.x), Max(a.y, b.y), Max(a.z, b.z));
}

template <typename T> VD_FORCE_INLINE
v3<T> Select(bool s, const v3<T>& t, const v3<T>& f)
{
    return v3<T>(Select(s, t.x, f.x),
                 Select(s, t.y, f.y),
                 Select(s, t.z, f.z));
}

template <typename T> VD_FORCE_INLINE
T Length(const v3<T>& a)
{
    return Sqrt(Dot(a, a));
}

template <typename T> VD_FORCE_INLINE
T LengthSqr(const v3<T>& a)
{
    return (Dot(a, a));
}

template <typename T> VD_FORCE_INLINE
T Dot(const v3<T>& a, const v3<T>& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T> VD_FORCE_INLINE
T Distance(const v3<T>& a, const v3<T>& b)
{
    return Length(a - b);
}

template <typename T> VD_FORCE_INLINE
T DistanceSqr(const v3<T>& a, const v3<T>& b)
{
    return LengthSqr(a - b);
}

template <typename T> VD_FORCE_INLINE
v3<T> Normalise(const v3<T>& a)
{
    return a * RcpSqrt(Dot(a, a));
}

template <typename T> VD_FORCE_INLINE
v3<T> Cross(const v3<T>& a, const v3<T>& b)
{
    return v3<T>(a.y * b.z - a.z * b.y,
                 a.z * b.x - a.x * b.z,
                 a.x * b.y - a.y * b.x);
}

template <typename T> VD_FORCE_INLINE
std::ostream& operator<< (std::ostream& cout, const v3<T>& a)
{
    return cout << "(" << a.x << ", " << a.y << ", " << a.z << ")";
}

// ============================================================================================== //

template<typename T>
struct v4
{
    T x, y, z, w;
    typedef T ScalarType;
    enum {N = 4};

    VD_FORCE_INLINE
    v4() { }

    VD_FORCE_INLINE
    v4(const v4& v) : x(v.x), y(v.y), z(v.z), w(v.w) { }

    template<typename T1> VD_FORCE_INLINE
    v4(const v4<T1>& a) : x(T(a.x)), y(T(a.y)), z(T(a.z)), w(T(a.w)) { }

    template<typename T1> VD_FORCE_INLINE
    v4& operator= (const v4<T1>& v)
    {
        x = v.x; y = v.y; z = v.z; w = v.w;
        return *this;
    }

    VD_FORCE_INLINE explicit
    v4(const T& a) : x(a), y(a), z(a), w(a) 
    {
        // EMPTY!
    }

    VD_FORCE_INLINE explicit
    v4(const T& x, const T& y, const T& z, const T& w)
        : x(x), y(y), z(z), w(w)
    {
        // EMPTY!
    }

    VD_FORCE_INLINE explicit
    v4(const T* a, vd::u32 stride = 1)
        : x(a[0]), y(a[stride]), z(a[2 * stride]), w(a[3 * stride])
    {
        // EMPTY!
    }

    VD_FORCE_INLINE
    v4(Constants::ZeroValue)   : x(Constants::Zero), y(Constants::Zero), z(Constants::Zero), w(Constants::Zero)
    {
        // EMPTY!
    }

    VD_FORCE_INLINE
    v4(Constants::OneValue)    : x(Constants::One),  y(Constants::One),  z(Constants::One),  w(Constants::One)
    {
        // EMPTY!
    }

    VD_FORCE_INLINE
    v4(Constants::PosInfValue) : x(Constants::PosInf), y(Constants::PosInf), z(Constants::PosInf), w(Constants::PosInf)
    {
        // EMPTY!
    }

    VD_FORCE_INLINE
    v4(Constants::NegInfValue) : x(Constants::NegInf), y(Constants::NegInf), z(Constants::NegInf), w(Constants::NegInf)
    {
        // EMPTY!
    }

    VD_FORCE_INLINE const T&
    operator [](size_t axis) const
    {
        assert(axis < 4);
        return (&x)[axis];
    }

    VD_FORCE_INLINE T&
    operator [](size_t axis)
    {
        assert(axis < 4);
        return (&x)[axis];
    }
};

// ============================================================================================== //

template <typename T> VD_FORCE_INLINE
v4<T> operator+ (const v4<T>& a)
{
    return v4<T>(+a.x, +a.y, +a.z, +a.w);
}

template <typename T> VD_FORCE_INLINE
v4<T> operator- (const v4<T>& a)
{
    return v4<T>(-a.x, -a.y, -a.z, -a.w);
}

template <typename T> VD_FORCE_INLINE
v4<T> operator+ (const v4<T>& a, const v4<T>& b)
{
    return v4<T>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

template <typename T> VD_FORCE_INLINE
v4<T> operator- (const v4<T>& a, const v4<T>& b)
{
    return v4<T>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

template <typename T> VD_FORCE_INLINE
v4<T> operator* (const v4<T>& a, const v4<T>& b)
{
    return v4<T>(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

template <typename T> VD_FORCE_INLINE
v4<T> operator/ (const v4<T>& a, const v4<T>& b)
{
    return v4<T>(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

template <typename T> VD_FORCE_INLINE
v4<T> operator* (const v4<T>& a, const T& b)
{
    return v4<T>(a.x * b, a.y * b, a.z * b, a.w * b);
}

template <typename T> VD_FORCE_INLINE
v4<T> operator/ (const v4<T>& a, const T& b)
{
    return v4<T>(a.x / b, a.y / b, a.z / b, a.w / b);
}

template <typename T> VD_FORCE_INLINE
v4<T> operator* (const T& a, const v4<T>& b)
{
    return v4<T>(a * b.x, a * b.y, a * b.z, a * b.w);
}

template <typename T> VD_FORCE_INLINE
v4<T> operator/ (const T& a, const v4<T>& b)
{
    return v4<T>(a / b.x, a / b.y, a / b.z, a / b.w);
}

template <typename T> VD_FORCE_INLINE
v4<T>& operator+= (v4<T>& a, const v4<T>& b)
{
    a.x += b.x; a.y += b.y; a.z += b.z; a.w += b.w; return a;
}

template <typename T> VD_FORCE_INLINE
v4<T>& operator-= (v4<T>& a, const v4<T>& b)
{
    a.x -= b.x; a.y -= b.y; a.z -= b.z; a.w -= b.w; return a;
}

template <typename T> VD_FORCE_INLINE
v4<T>& operator*= (v4<T>& a, const T& b)
{
    a.x *= b; a.y *= b; a.z *= b; a.w *= b; return a;
}

template <typename T> VD_FORCE_INLINE
v4<T>& operator/= (v4<T>& a, const T& b)
{
    a.x /= b; a.y /= b; a.z /= b; a.w /= b; return a;
}

template <typename T> VD_FORCE_INLINE
bool operator== (const v4<T>& a, const v4<T>& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

template <typename T> VD_FORCE_INLINE
bool operator!= (const v4<T>& a, const v4<T>& b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
}

template <typename T> VD_FORCE_INLINE
bool operator< (const v4<T>& a, const v4<T>& b)
{
    if(a.x != b.x) 
        return a.x < b.x;

    if(a.y != b.y) 
        return a.y < b.y;

    if(a.z != b.z) 
        return a.z < b.z;

    if(a.w != b.w) 
        return a.w < b.w;

    return false;
}

template <typename T> VD_FORCE_INLINE
T ReduceAdd(const v4<T>& a)
{
    return a.x + a.y + a.z + a.w;
}

template <typename T> VD_FORCE_INLINE
T ReduceMultiply(const v4<T>& a)
{
    return a.x * a.y * a.z * a.w;
}

template <typename T> VD_FORCE_INLINE
T ReduceMin(const v4<T>& a)
{
    return Min(a.x, a.y, a.z, a.w);
}

template <typename T> VD_FORCE_INLINE
T ReduceMax(const v4<T>& a)
{
    return Max(a.x, a.y, a.z, a.w);
}

template <typename T> VD_FORCE_INLINE
v4<T> Min(const v4<T>& a, const v4<T>& b)
{
    return v4<T>(Min(a.x, b.x), Min(a.y, b.y), Min(a.z, b.z), Min(a.w, b.w));
}

template <typename T> VD_FORCE_INLINE
v4<T> Max(const v4<T>& a, const v4<T>& b)
{
    return v4<T>(Max(a.x, b.x), Max(a.y, b.y), Max(a.z, b.z), Max(a.w, b.w));
}

template <typename T> VD_FORCE_INLINE
v4<T> Abs(const v4<T>& a)
{
    return v4<T>(Abs(a.x), Abs(a.y), Abs(a.z), Abs(a.w));
}

template <typename T> VD_FORCE_INLINE
v4<T> Rcp(const v4<T>& a)
{
    return v4<T>(Rcp(a.x), Rcp(a.y), Rcp(a.z), Rcp(a.w));
}

template <typename T> VD_FORCE_INLINE
v4<T> Sqrt(const v4<T>& a)
{
    return v4<T>(Sqrt(a.x), Sqrt(a.y), Sqrt(a.z), Sqrt(a.w));
}

template <typename T> VD_FORCE_INLINE
v4<T> RcpSqrt(const v4<T>& a)
{
    return v4<T>(RcpSqrt(a.x), RcpSqrt(a.y), RcpSqrt(a.z), RcpSqrt(a.w));
}

template <typename T> VD_FORCE_INLINE
T Length(const v4<T>& a)
{
    return Sqrt(Dot(a, a));
}

template <typename T> VD_FORCE_INLINE
T LengthSqr(const v4<T>& a)
{
    return (Dot(a, a));
}

template <typename T> VD_FORCE_INLINE
T Dot(const v4<T>& a, const v4<T>& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

template <typename T> VD_FORCE_INLINE
v4<T> Normalise(const v4<T>& a)
{
    return a * RcpSqrt(Dot(a, a));
}

template <typename T> VD_FORCE_INLINE
T Distance(const v4<T>& a, const v4<T>& b)
{
    return Length(a - b);
}

template <typename T> VD_FORCE_INLINE
T DistanceSqr(const v4<T>& a, const v4<T>& b)
{
    return LengthSqr(a - b);
}

template <typename T> VD_FORCE_INLINE
v4<T> Select(bool s, const v4<T>& t, const v4<T>& f)
{
    return v4<T>(Select(s, t.x, f.x),
                 Select(s, t.y, f.y),
                 Select(s, t.z, f.z),
                 Select(s, t.w, f.w));
}

template<typename T> VD_FORCE_INLINE
v3<T> BSplineWeights(T f)
{
    v3<T> w(
        T(0.50) * Sqr(f - T(Constants::One)),
        T(0.75) - Sqr(f - T(0.50)),
        T(0.50) * Sqr(f)
    );
    return w;
}

template<typename T> VD_FORCE_INLINE
v4<T> CubicWeights(T f)
{
    T f2(f*f);
    T f3(f2*f);

    T n1 = -T(1.0/3.0) * f + T(1.0/2.0) * f2 - T(1.0/6.0) * f3;
    T w0 = T(1) - f2 + T(1.0/2.0) * (f3-f);
    T w1 = f + T(1.0/2.0) * (f2-f3);
    T w2 = T(1.0/6.0) * (f3-f);

    return v4<T>(n1, w0, w1, w2);
}

template <typename T> VD_FORCE_INLINE
std::ostream& operator<<(std::ostream& cout, const v4<T>& a)
{
    return cout << "(" << a.x << ", " << a.y << ", " << a.z << ", " << a.w << ")";
}

// ============================================================================================== //

template<typename T>
struct q4
{
    typedef v3<T> VectorType;

    VD_FORCE_INLINE
    q4(void)                     { }

    VD_FORCE_INLINE
    q4(const q4& other) { r = other.r; i = other.i; j = other.j; k = other.k; }

    VD_FORCE_INLINE
    q4& operator=(const q4& other) { r = other.r; i = other.i; j = other.j; k = other.k; return *this; }

    VD_FORCE_INLINE
    q4(const T& r) : r(r), i(Constants::Zero), j(Constants::Zero), k(Constants::Zero) {}

    VD_FORCE_INLINE explicit
    q4(const v3<T>& v) : r(Constants::Zero), i(v.x), j(v.y), k(v.z) {}

    VD_FORCE_INLINE
    q4(const T& r, const T& i, const T& j , const T& k) : r(r), i(i), j(j), k(k) {}

    VD_FORCE_INLINE
    q4(const T& r, const v3<T>& v) : r(r), i(v.x), j(v.y), k(v.z) {}

    VD_FORCE_INLINE
    q4(const v3<T>& vx, const v3<T>& vy, const v3<T>& vz);

    VD_FORCE_INLINE
    q4(const T& yaw, const T& pitch, const T& roll);

    VD_FORCE_INLINE
    q4(Constants::ZeroValue) : r(Constants::Zero), i(Constants::Zero), j(Constants::Zero), k(Constants::Zero) {}

    VD_FORCE_INLINE
    q4(Constants::OneValue) : r(Constants::One), i(Constants::Zero), j(Constants::Zero), k(Constants::Zero) {}

    static VD_FORCE_INLINE q4
    Rotate(const v3<T>& u, const T& r)
    {
        return q4<T>(Cos(T(0.5) * r), Sin(T(0.5) * r) * Normalise(u));
    }

    static VD_FORCE_INLINE q4
    AxisAngle(const v3<T>& u, const T& r)
    {
        return Rotate(u, r);
    }

    VD_FORCE_INLINE const
    v3<T> v() const { return v3<T>(i, j, k); }

public:
    T r, i, j, k;
};

// ============================================================================================== //

template<typename T> VD_FORCE_INLINE
q4<T> operator *(const T&              a, const q4<T>& b)
{
    return q4<T>(a * b.r, a * b.i, a * b.j, a * b.k);
}

template<typename T> VD_FORCE_INLINE
q4<T> operator *(const q4<T>& a, const T&              b)
{
    return q4<T>(a.r * b, a.i * b, a.j * b, a.k * b);
}

template<typename T> VD_FORCE_INLINE
q4<T> operator +(const q4<T>& a)
{
    return q4<T>(+a.r, +a.i, +a.j, +a.k);
}

template<typename T> VD_FORCE_INLINE
q4<T> operator -(const q4<T>& a)
{
    return q4<T>(-a.r, -a.i, -a.j, -a.k);
}

template<typename T> VD_FORCE_INLINE
q4<T> Conjugate(const q4<T>& a)
{
    return q4<T>(a.r, -a.i, -a.j, -a.k);
}

template<typename T> VD_FORCE_INLINE
T Abs(const q4<T>& a)
{
    return Sqrt(a.r * a.r + a.i * a.i + a.j * a.j + a.k * a.k);
}

template<typename T> VD_FORCE_INLINE
q4<T> Rcp(const q4<T>& a)
{
    return Conjugate(a) * Rcp(a.r * a.r + a.i * a.i + a.j * a.j + a.k * a.k);
}

template<typename T> VD_FORCE_INLINE
q4<T> Normalise(const q4<T>& a)
{
    return a * RcpSqrt(a.r * a.r + a.i * a.i + a.j * a.j + a.k * a.k);
}

template<typename T> VD_FORCE_INLINE
q4<T> operator +(const T&              a, const q4<T>& b)
{
    return q4<T>(a + b.r,  b.i,  b.j,  b.k);
}

template<typename T> VD_FORCE_INLINE
q4<T> operator +(const q4<T>& a, const T&              b)
{
    return q4<T>(a.r + b, a.i, a.j, a.k);
}

template<typename T> VD_FORCE_INLINE
q4<T> operator +(const q4<T>& a, const q4<T>& b)
{
    return q4<T>(a.r + b.r, a.i + b.i, a.j + b.j, a.k + b.k);
}

template<typename T> VD_FORCE_INLINE
q4<T> operator -(const T&              a, const q4<T>& b)
{
    return q4<T>(a - b.r, -b.i, -b.j, -b.k);
}

template<typename T> VD_FORCE_INLINE
q4<T> operator -(const q4<T>& a, const T&              b)
{
    return q4<T>(a.r - b, a.i, a.j, a.k);
}

template<typename T> VD_FORCE_INLINE
q4<T> operator -(const q4<T>& a, const q4<T>& b)
{
    return q4<T>(a.r - b.r, a.i - b.i, a.j - b.j, a.k - b.k);
}

template<typename T> VD_FORCE_INLINE
v3<T> operator *(const q4<T>& a, const v3<T>      & b)
{
    return (a * q4<T>(b) * Conjugate(a)).v();
}

template<typename T> VD_FORCE_INLINE
q4<T> operator *(const q4<T>& a, const q4<T>& b)
{
    return q4<T>(a.r * b.r - a.i * b.i - a.j * b.j - a.k * b.k,
                 a.r * b.i + a.i * b.r + a.j * b.k - a.k * b.j,
                 a.r * b.j - a.i * b.k + a.j * b.r + a.k * b.i,
                 a.r * b.k + a.i * b.j - a.j * b.i + a.k * b.r);
}

template<typename T> VD_FORCE_INLINE
q4<T> operator /(const T&              a, const q4<T>& b)
{
    return a * Rcp(b);
}

template<typename T> VD_FORCE_INLINE
q4<T> operator /(const q4<T>& a, const T&              b)
{
    return a * Rcp(b);
}

template<typename T> VD_FORCE_INLINE
q4<T> operator /(const q4<T>& a, const q4<T>& b)
{
    return a * Rcp(b);
}

template<typename T> VD_FORCE_INLINE
q4<T>& operator +=(q4<T>& a, const T&              b)
{
    return a = a + b;
}

template<typename T> VD_FORCE_INLINE
q4<T>& operator +=(q4<T>& a, const q4<T>& b)
{
    return a = a + b;
}

template<typename T> VD_FORCE_INLINE
q4<T>& operator -=(q4<T>& a, const T&              b)
{
    return a = a - b;
}

template<typename T> VD_FORCE_INLINE
q4<T>& operator -=(q4<T>& a, const q4<T>& b)
{
    return a = a - b;
}

template<typename T> VD_FORCE_INLINE
q4<T>& operator *=(q4<T>& a, const T&              b)
{
    return a = a * b;
}

template<typename T> VD_FORCE_INLINE
q4<T>& operator *=(q4<T>& a, const q4<T>& b)
{
    return a = a * b;
}

template<typename T> VD_FORCE_INLINE
q4<T>& operator /=(q4<T>& a, const T&              b)
{
    return a = a * Rcp(b);
}

template<typename T> VD_FORCE_INLINE
q4<T>& operator /=(q4<T>& a, const q4<T>& b)
{
    return a = a * Rcp(b);
}

template<typename T> VD_FORCE_INLINE
v3<T> TransformPoint(const q4<T>& a, const v3<T>&       b)
{
    return (a * q4<T>(b) * Conjugate(a)).v();
}

template<typename T> VD_FORCE_INLINE
v3<T> TransformVector(const q4<T>& a, const v3<T>&       b)
{
    return (a * q4<T>(b) * Conjugate(a)).v();
}

template<typename T> VD_FORCE_INLINE
v3<T> TransformNormal(const q4<T>& a, const v3<T>&       b)
{
    return (a * q4<T>(b) * Conjugate(a)).v();
}

template<typename T> VD_FORCE_INLINE bool
operator ==(const q4<T>& a, const q4<T>& b)
{
    return a.r == b.r && a.i == b.i && a.j == b.j && a.k == b.k;
}

template<typename T> VD_FORCE_INLINE bool
operator !=(const q4<T>& a, const q4<T>& b)
{
    return a.r != b.r || a.i != b.i || a.j != b.j || a.k != b.k;
}

// ============================================================================================== //

template<typename T>
q4<T>::q4(const v3<T>& vx, const v3<T>& vy, const v3<T>& vz)
{
    if(vx.x + vy.y + vz.z >= T(Constants::Zero))
    {
        const T t = T(Constants::One) + (vx.x + vy.y + vz.z);
        const T s = RcpSqrt(t) * T(0.5f);
        r = t * s;
        i = (vy.z - vz.y) * s;
        j = (vz.x - vx.z) * s;
        k = (vx.y - vy.x) * s;
    }
    else if(vx.x >= Max(vy.y, vz.z))
    {
        const T t = (T(Constants::One) + vx.x) - (vy.y + vz.z);
        const T s = RcpSqrt(t) * T(0.5f);
        r = (vy.z - vz.y) * s;
        i = t * s;
        j = (vx.y + vy.x) * s;
        k = (vz.x + vx.z) * s;
    }
    else if(vy.y >= vz.z)    // if ( vy.y >= Max(vz.z, vx.x) )
    {
        const T t = (T(Constants::One) + vy.y) - (vz.z + vx.x);
        const T s = RcpSqrt(t) * T(0.5f);
        r = (vz.x - vx.z) * s;
        i = (vx.y + vy.x) * s;
        j = t * s;
        k = (vy.z + vz.y) * s;
    }
    else //if ( vz.z >= Max(vy.y, vx.x) )
    {
        const T t = (T(Constants::One) + vz.z) - (vx.x + vy.y);
        const T s = RcpSqrt(t) * T(0.5f);
        r = (vx.y - vy.x) * s;
        i = (vz.x + vx.z) * s;
        j = (vy.z + vz.y) * s;
        k = t * s;
    }
}

template<typename T>
q4<T>::q4(const T& yaw, const T& pitch, const T& roll)
{
    const T cya = Cos(yaw  * T(0.5f));
    const T cpi = Cos(pitch * T(0.5f));
    const T cro = Cos(roll * T(0.5f));
    const T sya = Sin(yaw  * T(0.5f));
    const T spi = Sin(pitch * T(0.5f));
    const T sro = Sin(roll * T(0.5f));
    r = cro * cya * cpi + sro * sya * spi;
    i = cro * cya * spi + sro * sya * cpi;
    j = cro * sya * cpi - sro * cya * spi;
    k = sro * cya * cpi - cro * sya * spi;
}

// ============================================================================================== //

template<typename T> static 
std::ostream& operator<<(std::ostream& cout, const q4<T>& q)
{
    return cout << "{ r = " << q.r << ", i = " << q.i << ", j = " << q.j << ", k = " << q.k << " }";
}

// ============================================================================================== //

template<typename T>
struct m3
{
    typedef v3<T> VectorType;
    v3<T> c[3];

    VD_FORCE_INLINE
    m3() {}

    VD_FORCE_INLINE
    m3(const m3& m)
    {
        c[0] = m.c[0]; 
        c[1] = m.c[1]; 
        c[2] = m.c[2];
    }

    VD_FORCE_INLINE
    m3& operator= (const m3& m)
    {
        c[0] = m.c[0]; 
        c[1] = m.c[1]; 
        c[2] = m.c[2];
        return *this;
    }

    VD_FORCE_INLINE
    m3(v3<T> n)
    {
        c[1] = n;

        if(Abs(n.x) >= Abs(n.y))
        {
            const T invlen = Rcp(Sqrt(n.x * n.x + n.z * n.z));
            c[0] = v3<T>(-n.z * invlen, T(Constants::Zero), n.x * invlen);
        }
        else
        {
            const T invlen = Rcp(Sqrt(n.y * n.y + n.z * n.z));
            c[0] = v3<T>(T(Constants::Zero), n.z * invlen, -n.y * invlen);
        }

        c[0] = Normalise(c[0]);
        c[1] = Normalise(c[1]);
        c[2] = Cross(c[1], c[0]);
    }

    template<typename L1> VD_FORCE_INLINE explicit
    m3(m3<L1> s)
    {
        c[0] = s.c[0];
        c[1] = s.c[1];
        c[2] = s.c[2];
    }

    VD_FORCE_INLINE 
    m3(v3<T> vx, v3<T> vy, v3<T> vz)
    {
    	c[0] = vx;
    	c[1] = vy;
    	c[2] = vz;
    }

    VD_FORCE_INLINE 
    m3(q4<T> q)
    {
        c[0] = v3<T>(	(q.r* q.r + q.i* q.i - q.j* q.j - q.k* q.k),
           				2.0f * (q.i* q.j + q.r* q.k),
           				2.0f * (q.i* q.k - q.r* q.j));
           				
        c[1] = v3<T>(	2.0f * (q.i* q.j - q.r* q.k),
           				(q.r* q.r - q.i* q.i + q.j* q.j - q.k* q.k),
           				2.0f * (q.j* q.k + q.r* q.i));
           
        c[2] = v3<T>(	2.0f * (q.i* q.k + q.r* q.j),
           				2.0f * (q.j* q.k - q.r* q.i),
           				(q.r* q.r - q.i* q.i - q.j* q.j + q.k* q.k));
    }

    VD_FORCE_INLINE
    m3(T m00, T m01, T m02,
       T m10, T m11, T m12,
       T m20, T m21, T m22)
    {
        c[0] = v3<T>(m00, m10, m20);
        c[1] = v3<T>(m01, m11, m21);
        c[2] = v3<T>(m02, m12, m22); 
    }

    VD_FORCE_INLINE
    m3(Constants::ZeroValue)
    {
        c[0] = v3<T>(Constants::Zero);
        c[1] = v3<T>(Constants::Zero);
        c[2] = v3<T>(Constants::Zero);
    }

    VD_FORCE_INLINE
    m3(Constants::OneValue)
    {
        c[0] = v3<T>(Constants::One, Constants::Zero, Constants::Zero);
        c[1] = v3<T>(Constants::Zero, Constants::One, Constants::Zero);
        c[2] = v3<T>(Constants::Zero, Constants::Zero, Constants::One);
    }

    VD_FORCE_INLINE m3
    Adjoint(void) const
    {
        return m3(Cross(c[1], c[2]),
                  Cross(c[2], c[0]),
                  Cross(c[0], c[1])).Transposed();
    }

    VD_FORCE_INLINE m3
    Transposed(void) const
    {
        return m3(c[0].x, c[0].y, c[0].z,
                  c[1].x, c[1].y, c[1].z,
                  c[2].x, c[2].y, c[2].z);
    }

    VD_FORCE_INLINE 
    m3 Inverse(void) const
    {
    	return Rcp(Determinant()) * Adjoint();
    }

    VD_FORCE_INLINE 
    T Determinant(void) const
    {
    	return Dot(c[0], Cross(c[1], c[2]));
    }

    static VD_FORCE_INLINE
    m3 Scale(v3<T> &s)
    {
        return m3(s.x, 0, 0, 0, s.y, 0, 0, 0, s.z);
    }
    
    static VD_FORCE_INLINE
    m3 Rotate(v3<T> _u, T r)
    {
        v3<T> u = Normalise(_u);
        T s = Sin(r);
        T c = Cos(r);
        return m3(u.x * u.x + (1 - u.x * u.x) * c, u.x * u.y * (1 - c) - u.z * s,  u.x * u.z * (1 - c) + u.y * s,
                  u.x * u.y * (1 - c) + u.z * s,   u.y * u.y + (1 - u.y * u.y) * c, u.y * u.z * (1 - c) - u.x * s,
                  u.x * u.z * (1 - c) - u.y * s,   u.y * u.z * (1 - c) + u.x * s,  u.z * u.z + (1 - u.z * u.z) * c);
    }

    VD_FORCE_INLINE 
    v3<T>& operator[](int i)
    {
        return this->c[i];
    }

    VD_FORCE_INLINE 
    const v3<T>&  operator[](int i) const
    {
        return this->c[i];
    }
};

// ============================================================================================== //

template <typename T> VD_FORCE_INLINE
m3<T> operator- (m3<T> a)
{
    return m3<T>(-a.c[0], -a.c[1], -a.c[2]);
}

template <typename T> VD_FORCE_INLINE
m3<T> operator+ (m3<T> a)
{
    return m3<T>(+a.c[0], +a.c[1], +a.c[2]);
}

template <typename T> VD_FORCE_INLINE
m3<T> Rcp(m3<T> a)
{
    return a.Inverse();
}

template <typename T> VD_FORCE_INLINE
m3<T> operator+ (m3<T> a, m3<T> b)
{
    return m3<T>(a.c[0] + b.c[0], a.c[1] + b.c[1], a.c[2] + b.c[2]);
}

template <typename T> VD_FORCE_INLINE
m3<T> operator- (m3<T> a, m3<T> b)
{
    return m3<T>(a.c[0] - b.c[0], a.c[1] - b.c[1], a.c[2] - b.c[2]);
}

template <typename T> VD_FORCE_INLINE
m3<T> operator* (m3<T> a, m3<T> b)
{
    return m3<T>(a * b.c[0], a * b.c[1], a * b.c[2]);
}

template <typename T> VD_FORCE_INLINE
m3<T> operator/ (m3<T> a, m3<T> b)
{
    return a * Rcp(b);
}

template <typename T> VD_FORCE_INLINE
v3<T>  operator* (m3<T> a, v3<T> b)
{
    return b.x * a.c[0] + b.y * a.c[1] + b.z * a.c[2];
}

template <typename T> VD_FORCE_INLINE
m3<T> operator* (T a, m3<T> b)
{
    return m3<T>(a * b.c[0], a * b.c[1], a * b.c[2]);
}

template <typename T> VD_FORCE_INLINE
m3<T> operator/ (m3<T> a, T b)
{
    return a * Rcp(b);
}

template <typename T> VD_FORCE_INLINE
m3<T>& operator/= (m3<T>& a, T b)
{
    return a = a / b;
}

template <typename T> VD_FORCE_INLINE
m3<T>& operator*= (m3<T>& a, T b)
{
    return a = a * b;
}

template <typename T> VD_FORCE_INLINE
m3<T>& operator*= (m3<T>& a, m3<T> b)
{
    return a = a * b;
}

template <typename T> VD_FORCE_INLINE
m3<T>& operator/= (m3<T>& a, m3<T> b)
{
    return a = a / b;
}

template <typename T> VD_FORCE_INLINE
bool operator== (m3<T> a, m3<T> b)
{
    return (a.c[0] == b.c[0]) && (a.c[1] == b.c[1]) && (a.c[2] == b.c[2]);
}

template <typename T> VD_FORCE_INLINE
bool operator!= (m3<T> a, m3<T> b)
{
    return (a.c[0] != b.c[0]) || (a.c[1] != b.c[1]) || (a.c[2] != b.c[2]);
}

template <typename T> VD_FORCE_INLINE
v3<T> TransformPoint(m3<T> s, v3<T> a)
{
    return a.x * s.c[0] + a.y * s.c[1] + a.z * s.c[2];
}

template <typename T> VD_FORCE_INLINE
v3<T> TransformVector(m3<T> s, v3<T> a)
{
    return a.x * s.c[0] + a.y * s.c[1] + a.z * s.c[2];
}

template <typename T> VD_FORCE_INLINE
v3<T> TransformNormal(m3<T> s, v3<T> a)
{
    return TransformVector(s.Inverse().Transposed(), a);
}

template <typename T> VD_FORCE_INLINE
m3<T> Adjoint(m3<T> m)
{
    return m.Adjoint();
}

template <typename T> VD_FORCE_INLINE
m3<T> Transpose(m3<T> m)
{
    return m.Transposed();
}

template <typename T> VD_FORCE_INLINE
m3<T> Frame(v3<T> N)
{
    v3<T> dx0 = Cross(v3<T>(T(Constants::One), T(Constants::Zero), T(Constants::Zero)), N);
    v3<T> dx1 = Cross(v3<T>(T(Constants::Zero), T(Constants::One), T(Constants::Zero)), N);
    v3<T> dx = Normalise(Select(Dot(dx0, dx0) > Dot(dx1, dx1), dx0, dx1));
    v3<T> dy = Normalise(Cross(N, dx));
    return m3<T>(dx, dy, N);
}

template<typename T> static 
std::ostream& operator<<(std::ostream& cout, const m3<T>& m)
{
    return cout << "{c0 = " << m.c[0] << ", c1 = " << m.c[1] << ", c2 = " << m.c[2] << "}";
}

// ============================================================================================== //

template<typename T>
struct a4
{
    typedef T ScalarType;

    m3<T> l;
    v3<T> p;

    VD_FORCE_INLINE
    a4() {}

    VD_FORCE_INLINE
    a4(const a4& m) {l = m.l; p = m.p;}

    VD_FORCE_INLINE
    a4(v3<T> vx, v3<T> vy, v3<T> vz, v3<T> p) : l(vx, vy, vz), p(p) {}

    VD_FORCE_INLINE
    a4(m3<T> l, v3<T> p) : l(l), p(p) {}

    VD_FORCE_INLINE a4&
    operator= (const a4& m)
    {
        l = m.l; p = m.p; return *this;
    }

    template<typename L1>
    VD_FORCE_INLINE explicit a4(const a4<L1>& s) : l(s.l), p(s.p) {}

    VD_FORCE_INLINE a4(Constants::ZeroValue) : l(Constants::Zero), p(Constants::Zero) {}

    VD_FORCE_INLINE
    a4(Constants::OneValue)  : l(Constants::One),  p(Constants::Zero) {}

    /*! Static methods */
    static VD_FORCE_INLINE
    a4 Scale(v3<T> s)
    {
        return a4(m3<T>::Scale(s), Constants::Zero);
    }

    static VD_FORCE_INLINE
    a4 Translate(v3<T> p)
    {
        return a4(Constants::One, p);
    }

    static VD_FORCE_INLINE
    a4 Rotate(v3<T> u, T r)
    {
        return a4(m3<T>::Rotate(u, r), Constants::Zero);
    }

    static VD_FORCE_INLINE
    a4 Rotate(v3<T> p, v3<T> u, T r)
    {
        return Translate(p) * Rotate(u, r) * Translate(-p);
    }

    static VD_FORCE_INLINE
    a4 LookAtPoint(v3<T> eye, v3<T> point, v3<T> up)
    {
        v3<T> z = Normalise(point - eye);
        v3<T> u = Normalise(Cross(up, z));
        v3<T> v = Normalise(Cross(z, u));
        return a4(m3<T>(u, v, z), eye);
    }
};

// ============================================================================================== //

template <typename T> VD_FORCE_INLINE
a4<T> operator- (a4<T> a)
{
    return a4<T>(-a.l, -a.p);
}

template <typename T> VD_FORCE_INLINE
a4<T> operator+ (a4<T> a)
{
    return a4<T>(+a.l, +a.p);
}

template <typename T> VD_FORCE_INLINE
a4<T> Rcp(a4<T> a)
{
    m3<T> il = Rcp(a.l);
    return a4<T>(il, -il * a.p);
}

template <typename T> VD_FORCE_INLINE
a4<T> operator+ (a4<T> a, a4<T> b)
{
    return a4<T>(a.l + b.l, a.p + b.p);
}

template <typename T> VD_FORCE_INLINE
a4<T> operator- (a4<T> a, a4<T> b)
{
    return a4<T>(a.l - b.l, a.p - b.p);
}

template <typename T> VD_FORCE_INLINE
a4<T> operator* (T a,   a4<T> b)
{
    return a4<T>(a * b.l, a * b.p);
}

template <typename T> VD_FORCE_INLINE
a4<T> operator* (a4<T> a, a4<T> b)
{
    return a4<T>(a.l * b.l, a.l * b.p + a.p);
}

template <typename T> VD_FORCE_INLINE
a4<T> operator/ (a4<T> a, a4<T> b)
{
    return a * Rcp(b);
}

template <typename T> VD_FORCE_INLINE
a4<T> operator/ (a4<T> a, T b)
{
    return a * Rcp(b);
}

template <typename T> VD_FORCE_INLINE
a4<T>& operator*= (a4<T>& a, T b)
{
    return a = a * b;
}

template <typename T> VD_FORCE_INLINE
a4<T>& operator/= (a4<T>& a, T b)
{
    return a = a / b;
}

template <typename T> VD_FORCE_INLINE
a4<T>& operator*= (a4<T>& a, a4<T> b)
{
    return a = a * b;
}

template <typename T> VD_FORCE_INLINE
a4<T>& operator/= (a4<T>& a, a4<T> b)
{
    return a = a / b;
}

template <typename T> VD_FORCE_INLINE
bool operator== (a4<T> a, a4<T> b)
{
    return (a.l == b.l) && (a.p == b.p);
}

template <typename T> VD_FORCE_INLINE
bool operator!= (a4<T> a, a4<T> b)
{
    return (a.l != b.l) || (a.p != b.p);
}

// ============================================================================================== //

template <typename T> VD_FORCE_INLINE
v3<T> TransformPoint(a4<T> m, v3<T> p)
{
    return TransformPoint(m.l, p) + m.p;
}

template <typename T> VD_FORCE_INLINE
v3<T> TransformVector(a4<T> m, v3<T> v)
{
    return TransformVector(m.l, v);
}

template <typename T> VD_FORCE_INLINE
v3<T> TransformNormal(a4<T> m, v3<T> n)
{
    return TransformNormal(m.l, n);
}

template<typename T> static 
std::ostream& operator<<(std::ostream& cout, a4<T> m)
{
    return cout << "{l = " << m.l << ", p = " << m.p << "}";
}

// ============================================================================================== //

template<typename T>
struct m4
{
    typedef T ScalarType;

    v4<T> c[4];

    VD_FORCE_INLINE m4(void) {}

    VD_FORCE_INLINE m4(Constants::OneValue)
    {
        c[0] = v4<T>(Constants::One, Constants::Zero, Constants::Zero, Constants::Zero);
        c[1] = v4<T>(Constants::Zero, Constants::One, Constants::Zero, Constants::Zero);
        c[2] = v4<T>(Constants::Zero, Constants::Zero, Constants::One, Constants::Zero);
        c[3] = v4<T>(Constants::Zero, Constants::Zero, Constants::Zero, Constants::One);
    }

    VD_FORCE_INLINE
    m4(Constants::ZeroValue) 
    {
    	c[0] = c[1] = c[2] = c[3] = Constants::Zero;
    }

    VD_FORCE_INLINE
    m4(const m4<T> &m) 
    { 
    	c[0] = m.c[0]; 
    	c[1] = m.c[1]; 
    	c[2] = m.c[2]; 
    	c[3] = m.c[3]; 
    }

    VD_FORCE_INLINE
    m4(T s)
    {
        c[0] = v4<T>(s, Constants::Zero, Constants::Zero, Constants::Zero);
        c[1] = v4<T>(Constants::Zero, s, Constants::Zero, Constants::Zero);
        c[2] = v4<T>(Constants::Zero, Constants::Zero, s, Constants::Zero);
        c[3] = v4<T>(Constants::Zero, Constants::Zero, Constants::Zero, s);
    }

    VD_FORCE_INLINE
    m4(T x0, T y0, T z0, T w0,
       T x1, T y1, T z1, T w1,
       T x2, T y2, T z2, T w2,
       T x3, T y3, T z3, T w3)
    {
        c[0] = v4<T>(x0, y0, z0, w0);
        c[1] = v4<T>(x1, y1, z1, w1);
        c[2] = v4<T>(x2, y2, z2, w2);
        c[3] = v4<T>(x3, y3, z3, w3);
    }

    VD_FORCE_INLINE
    m4(v4<T> v0, v4<T> v1, v4<T> v2, v4<T> v3)
    {
        c[0] = v0; c[1] = v1; c[2] = v2; c[3] = v3;
    }

    template <typename U> VD_FORCE_INLINE
    m4(const m4<U> &m)
    {
        c[0] = v4<T>(m[0]);
        c[1] = v4<T>(m[1]);
        c[2] = v4<T>(m[2]);
        c[3] = v4<T>(m[3]);
    }

    template <typename U> VD_FORCE_INLINE
    m4(U s)
    {
        c[0] = v4<T>(T(s), Constants::Zero, Constants::Zero, Constants::Zero);
        c[1] = v4<T>(Constants::Zero, T(s), Constants::Zero, Constants::Zero);
        c[2] = v4<T>(Constants::Zero, Constants::Zero, T(s), Constants::Zero);
        c[3] = v4<T>(Constants::Zero, Constants::Zero, Constants::Zero, T(s));
    }

    VD_FORCE_INLINE
    m4(m3<T> m)
    {
        c[0] = v4<T>(m[0], Constants::Zero);
        c[1] = v4<T>(m[1], Constants::Zero);
        c[2] = v4<T>(m[2], Constants::Zero);
        c[3] = v4<T>(Constants::Zero, Constants::Zero, Constants::Zero, Constants::One);
    }

    VD_FORCE_INLINE
    m4(a4<T> m)
    {
        c[0] = v4<T>(m[0], Constants::Zero);
        c[1] = v4<T>(m[1], Constants::Zero);
        c[2] = v4<T>(m[2], Constants::Zero);
        c[3] = v4<T>(m[3], Constants::One);
    }

    template <typename U> VD_FORCE_INLINE
    m4<T>& operator+= (U s)
    {
        c[0] += s; c[1] += s; c[2] += s; c[3] += s; return *this;
    }

    template <typename U> VD_FORCE_INLINE
    m4<T>& operator-= (U s)
    {
        c[0] -= s; c[1] -= s; c[2] -= s; c[3] -= s; return *this;
    }

    template <typename U> VD_FORCE_INLINE
    m4<T>& operator*= (U s)
    {
        c[0] *= s; c[1] *= s; c[2] *= s; c[3] *= s; return *this;
    }

    template <typename U> VD_FORCE_INLINE
    m4<T>& operator/= (U s)
    {
        c[0] /= s; c[1] /= s; c[2] /= s; c[3] /= s; return *this;
    }

    template <typename U> VD_FORCE_INLINE
    m4<T>& operator= (m4<U> m)
    {
        c[0] = m[0];  c[1] = m[1];  c[2] = m[2];  c[3] = m[3]; return *this;
    }

    template <typename U> VD_FORCE_INLINE
    m4<T>& operator+= (m4<U> m)
    {
        c[0] += m[0]; c[1] += m[1]; c[2] += m[2]; c[3] += m[3]; return *this;
    }

    template <typename U> VD_FORCE_INLINE
    m4<T>& operator-= (m4<U> m)
    {
        c[0] -= m[0]; c[1] -= m[1]; c[2] -= m[2]; c[3] -= m[3]; return *this;
    }

    template <typename U> VD_FORCE_INLINE
    m4<T>& operator*= (m4<U> m)
    {
        c[0] *= m[0]; c[1] *= m[1]; c[2] *= m[2]; c[3] *= m[3]; return *this;
    }

    template <typename U> VD_FORCE_INLINE
    m4<T>& operator/= (m4<U> m)
    {
        c[0] /= m[0]; c[1] /= m[1]; c[2] /= m[2]; c[3] /= m[3]; return *this;
    }

    template <typename U> VD_FORCE_INLINE
    m4<T>& operator= (const m4<U> &m)
    {
        c[0] = m[0]; c[1] = m[1]; c[2] = m[2]; c[3] = m[3]; return *this;
    }

    VD_FORCE_INLINE 
    m4<T> Inverse() const
    {
        const T f00 = c[2][2] * c[3][3] - c[3][2] * c[2][3];
        const T f01 = c[2][1] * c[3][3] - c[3][1] * c[2][3];
        const T f02 = c[2][1] * c[3][2] - c[3][1] * c[2][2];
        const T f03 = c[2][0] * c[3][3] - c[3][0] * c[2][3];
        const T f04 = c[2][0] * c[3][2] - c[3][0] * c[2][2];
        const T f05 = c[2][0] * c[3][1] - c[3][0] * c[2][1];
        const T f06 = c[1][2] * c[3][3] - c[3][2] * c[1][3];
        const T f07 = c[1][1] * c[3][3] - c[3][1] * c[1][3];
        const T f08 = c[1][1] * c[3][2] - c[3][1] * c[1][2];
        const T f09 = c[1][0] * c[3][3] - c[3][0] * c[1][3];
        const T f10 = c[1][0] * c[3][2] - c[3][0] * c[1][2];
        const T f11 = c[1][1] * c[3][3] - c[3][1] * c[1][3];
        const T f12 = c[1][0] * c[3][1] - c[3][0] * c[1][1];
        const T f13 = c[1][2] * c[2][3] - c[2][2] * c[1][3];
        const T f14 = c[1][1] * c[2][3] - c[2][1] * c[1][3];
        const T f15 = c[1][1] * c[2][2] - c[2][1] * c[1][2];
        const T f16 = c[1][0] * c[2][3] - c[2][0] * c[1][3];
        const T f17 = c[1][0] * c[2][2] - c[2][0] * c[1][2];
        const T f18 = c[1][0] * c[2][1] - c[2][0] * c[1][1];

        m4<T> inv(+c[1][1]*f00 - c[1][2]*f01 + c[1][3]*f02,
                  -c[1][0]*f00 + c[1][2]*f03 - c[1][3]*f04,
                  +c[1][0]*f01 - c[1][1]*f03 + c[1][3]*f05,
                  -c[1][0]*f02 + c[1][1]*f04 - c[1][2]*f05,
                  -c[0][1]*f00 + c[0][2]*f01 - c[0][3]*f02,
                  +c[0][0]*f00 - c[0][2]*f03 + c[0][3]*f04,
                  -c[0][0]*f01 + c[0][1]*f03 - c[0][3]*f05,
                  +c[0][0]*f02 - c[0][1]*f04 + c[0][2]*f05,
                  +c[0][1]*f06 - c[0][2]*f07 + c[0][3]*f08,
                  -c[0][0]*f06 + c[0][2]*f09 - c[0][3]*f10,
                  +c[0][0]*f11 - c[0][1]*f09 + c[0][3]*f12,
                  -c[0][0]*f08 + c[0][1]*f10 - c[0][2]*f12,
                  -c[0][1]*f13 + c[0][2]*f14 - c[0][3]*f15,
                  +c[0][0]*f13 - c[0][2]*f16 + c[0][3]*f17,
                  -c[0][0]*f14 + c[0][1]*f16 - c[0][3]*f18,
                  +c[0][0]*f15 - c[0][1]*f17 + c[0][2]*f18);

        const T determinant = c[0][0] * inv[0][0] + c[0][1] * inv[1][0] + c[0][2] * inv[2][0] + c[0][3] * inv[3][0];
        inv /= determinant;
        return inv;
    }

    VD_FORCE_INLINE 
    v4<T>& operator[](int i)
    {
        return this->c[i];
    }

    VD_FORCE_INLINE 
    const v4<T>&  operator[](int i) const
    {
        return this->c[i];
    }
};

// ============================================================================================== //

template <typename T> VD_FORCE_INLINE
m4<T> operator+ (m4<T> m, T s)
{
    return m4<T>(m[0] + s, m[1] + s, m[2] + s, m[3] + s);
}

template <typename T> VD_FORCE_INLINE
m4<T> operator+ (T s, m4<T> m)
{
    return m4<T>(m[0] + s, m[1] + s, m[2] + s, m[3] + s);
}

template <typename T> VD_FORCE_INLINE
m4<T> operator- (m4<T> m, T s)
{
    return m4<T>(m[0] - s, m[1] - s, m[2] - s, m[3] - s);
}

template <typename T> VD_FORCE_INLINE
m4<T> operator- (T s, m4<T> m)
{
    return m4<T>(m[0] - s, m[1] - s, m[2] - s, m[3] - s);
}

template <typename T> VD_FORCE_INLINE
m4<T> operator* (m4<T> m, T s)
{
    return m4<T>(m[0] * s, m[1] * s, m[2] * s, m[3] * s);
}

template <typename T> VD_FORCE_INLINE
m4<T> operator* (T s, m4<T> m)
{
    return m4<T>(m[0] * s, m[1] * s, m[2] * s, m[3] * s);
}

template <typename T> VD_FORCE_INLINE
m4<T> operator/ (m4<T> m, T s)
{
    return m4<T>(m[0] / s, m[1] / s, m[2] / s, m[3] / s);
}

template <typename T> VD_FORCE_INLINE
m4<T> operator+ (m4<T> m1, m4<T> m2)
{
    return m4<T>(m1[0] + m2[0], m1[1] + m2[1], m1[2] + m2[2], m1[3] + m2[3]);
}

template <typename T> VD_FORCE_INLINE
m4<T> operator- (m4<T> m1, m4<T> m2)
{
    return m4<T>(m1[0] - m2[0], m1[1] - m2[1], m1[2] - m2[2], m1[3] - m2[3]);
}

template <typename T> VD_FORCE_INLINE
v4<T>  operator* (m4<T> m, v4<T> v)
{
    return V(m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] * v.w,
             m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] * v.w,
             m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] * v.w,
             m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3] * v.w);
}

template <typename T> VD_FORCE_INLINE
v4<T> operator* (v4<T> v, m4<T> m)
{
    return V(m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
             m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
             m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
             m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w);
}

template <typename T> VD_FORCE_INLINE
m4<T> operator* (m4<T> m1, m4<T> m2)
{
    const v4<T> a0 = m1[0], a1 = m1[1], a2 = m1[2], a3 = m1[3];
    const v4<T> b0 = m2[0], b1 = m2[1], b2 = m2[2], b3 = m2[3];
    m4<T> m;
    m[0] = a0 * b0[0] + a1 * b0[1] + a2 * b0[2] + a3 * b0[3];
    m[1] = a0 * b1[0] + a1 * b1[1] + a2 * b1[2] + a3 * b1[3];
    m[2] = a0 * b2[0] + a1 * b2[1] + a2 * b2[2] + a3 * b2[3];
    m[3] = a0 * b3[0] + a1 * b3[1] + a2 * b3[2] + a3 * b3[3];
    return m;
}

template <typename T> VD_FORCE_INLINE
v4<T> operator/ (m4<T> m, v4<T> v)
{
    return m.Inverse() * v;
}

template <typename T> VD_FORCE_INLINE
v4<T> operator/ (v4<T> v, m4<T> m)
{
    return v * m.Inverse();
}

template <typename T> VD_FORCE_INLINE
m4<T> operator/ (m4<T> m1, m4<T> m2)
{
    return m1 * m2.Inverse();
}

template <typename T> VD_FORCE_INLINE
m4<T> operator- (m4<T> m)
{
    return m4<T>(-m[0], -m[1], -m[2], -m[3]);
}

template <typename T> VD_FORCE_INLINE
bool operator== (m4<T> m, m4<T> n)
{
    return (m[0] == n[0]) && (m[1] == n[1]) && (m[2] == n[2]) && (m[3] == n[3]);
}

template <typename T> VD_FORCE_INLINE
bool operator!= (m4<T> m, m4<T> n)
{
    return (m[0] != n[0]) || (m[1] != n[1]) || (m[2] != n[2]) || (m[3] != n[3]);
}

// ============================================================================================== //

template <typename T> VD_FORCE_INLINE
m4<T> Translate(m4<T> m, v3<T> v)
{
    m4<T> rv(m);
    rv[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
    return rv;
}

template <typename T> VD_FORCE_INLINE
m4<T> LookAt(v3<T> eye, v3<T> center, v3<T> up)
{
    m4<T> rv(Constants::One);
    v3<T> f = Normalise(center - eye);
    v3<T> u = Normalise(up);
    v3<T> s = Normalise(Cross(f, u));
    u = Cross(s, f);
    rv[0][0] = s.x; rv[1][0] = s.y; rv[2][0] = s.z;
    rv[0][1] = u.x; rv[1][1] = u.y; rv[2][1] = u.z;
    rv[0][2] = -f.x; rv[1][2] = -f.y; rv[2][2] = -f.z;
    return Translate(rv, -eye);
}

template <typename T> VD_FORCE_INLINE
m4<T> Perspective(T fovy, T aspect, T zNear, T zFar)
{
    m4<T> rv(Constants::Zero);
    T range = Tan(Deg2Rad(fovy / T(Constants::Two))) * zNear;
    T left = -range * aspect;
    T right = range * aspect;
    T bottom = -range;
    T top = range;
    rv[0][0] = (T(Constants::Two) * zNear) / (right - left);
    rv[1][1] = (T(Constants::Two) * zNear) / (top - bottom);
    rv[2][2] = - (zFar + zNear) / (zFar - zNear);
    rv[2][3] = - T(Constants::One);
    rv[3][2] = - (T(Constants::Two) * zFar * zNear) / (zFar - zNear);
    return rv;
}

template <typename T> VD_FORCE_INLINE
m4<T> Rotate(m4<T> m, T angle, v3<T> v)
{
    m4<T> rot(Constants::Zero), rv(Constants::Zero);
    const T a = Deg2Rad(angle);
    const T c = Cos(a);
    const T s = Sin(a);
    const v3<T> axis = Normalise(v);
    const v3<T> temp = (T(Constants::One) - c) * axis;
    rot[0][0] = c + temp[0] * axis[0];
    rot[0][1] = 0 + temp[0] * axis[1] + s * axis[2];
    rot[0][2] = 0 + temp[0] * axis[2] - s * axis[1];
    rot[1][0] = 0 + temp[1] * axis[0] - s * axis[2];
    rot[1][1] = c + temp[1] * axis[1];
    rot[1][2] = 0 + temp[1] * axis[2] + s * axis[0];
    rot[2][0] = 0 + temp[2] * axis[0] + s * axis[1];
    rot[2][1] = 0 + temp[2] * axis[1] - s * axis[0];
    rot[2][2] = c + temp[2] * axis[2];
    rv[0] = m[0] * rot[0][0] + m[1] * rot[0][1] + m[2] * rot[0][2];
    rv[1] = m[0] * rot[1][0] + m[1] * rot[1][1] + m[2] * rot[1][2];
    rv[2] = m[0] * rot[2][0] + m[1] * rot[2][1] + m[2] * rot[2][2];
    rv[3] = m[3];
    return rv;
}

template<typename T> static 
std::ostream& operator<<(std::ostream& cout, m4<T> m)
{
    return cout << "{ c0 = " << m.c[0] <<
           ", c1 = " << m.c[1] <<
           ", c2 = " << m.c[2] <<
           ", c3 = " << m.c[3] << " };";
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_NUMERICS_INCLUDED

