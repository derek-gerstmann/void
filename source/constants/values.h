// ============================================================================================== //
//
// License:     The Lesser GNU Public License (LGPL) v3.0.
// 
// Author(s):   Derek Gerstmann. The University of Western Australia (UWA). 
//              As well as the shoulders of many giants...
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

#ifndef VD_CONSTANTS_VALUES_INCLUDED
#define VD_CONSTANTS_VALUES_INCLUDED

// ============================================================================================== //

#include "vd.h"
#include "constants/constants.h"

// ============================================================================================== //

VD_CONSTANTS_NAMESPACE_BEGIN();

// ============================================================================================== //

static struct NullValue 
{

} Null VD_OPTIONAL;

static struct TrueValue
 {
    VD_FORCE_INLINE operator bool( ) const { return true; }

} True VD_OPTIONAL;

static struct FalseValue 
{
    VD_FORCE_INLINE operator bool( ) const { return false; }

} False VD_OPTIONAL;

static struct ZeroValue
{
    VD_FORCE_INLINE operator f64( ) const { return 0; }
    VD_FORCE_INLINE operator f32( ) const { return 0; }
    VD_FORCE_INLINE operator f16( ) const { return 0; }
    VD_FORCE_INLINE operator i64( ) const { return 0; }
    VD_FORCE_INLINE operator u64( ) const { return 0; }
    VD_FORCE_INLINE operator i32( ) const { return 0; }
    VD_FORCE_INLINE operator u32( ) const { return 0; }
    VD_FORCE_INLINE operator i16( ) const { return 0; }
    VD_FORCE_INLINE operator u16( ) const { return 0; }
    VD_FORCE_INLINE operator i8 ( ) const { return 0; }
    VD_FORCE_INLINE operator u8 ( ) const { return 0; }
    VD_FORCE_INLINE operator uid ( ) const { static const uid v; return v; }
#ifndef __MSVC__
    VD_FORCE_INLINE operator size_t( ) const { return 0; }
#endif

} Zero VD_OPTIONAL;

static struct OneValue
{
    VD_FORCE_INLINE operator f64( ) const { return 1; }
    VD_FORCE_INLINE operator f32( ) const { return 1; }
    VD_FORCE_INLINE operator f16( ) const { return 1; }
    VD_FORCE_INLINE operator i64( ) const { return 1; }
    VD_FORCE_INLINE operator u64( ) const { return 1; }
    VD_FORCE_INLINE operator i32( ) const { return 1; }
    VD_FORCE_INLINE operator u32( ) const { return 1; }
    VD_FORCE_INLINE operator i16( ) const { return 1; }
    VD_FORCE_INLINE operator u16( ) const { return 1; }
    VD_FORCE_INLINE operator i8 ( ) const { return 1; }
    VD_FORCE_INLINE operator u8 ( ) const { return 1; }
#ifndef __MSVC__
    VD_FORCE_INLINE operator size_t( ) const { return 1; }
#endif
} One VD_OPTIONAL;


static struct TwoValue
{
    VD_FORCE_INLINE operator f64( ) const { return 2; }
    VD_FORCE_INLINE operator f32( ) const { return 2; }
    VD_FORCE_INLINE operator f16( ) const { return 2; }
    VD_FORCE_INLINE operator i64( ) const { return 2; }
    VD_FORCE_INLINE operator u64( ) const { return 2; }
    VD_FORCE_INLINE operator i32( ) const { return 2; }
    VD_FORCE_INLINE operator u32( ) const { return 2; }
    VD_FORCE_INLINE operator i16( ) const { return 2; }
    VD_FORCE_INLINE operator u16( ) const { return 2; }
    VD_FORCE_INLINE operator i8 ( ) const { return 2; }
    VD_FORCE_INLINE operator u8 ( ) const { return 2; }
#ifndef __MSVC__
    VD_FORCE_INLINE operator size_t( ) const { return 2; }
#endif

} Two VD_OPTIONAL;

static struct ThreeValue
{
    VD_FORCE_INLINE operator f64( ) const { return 3; }
    VD_FORCE_INLINE operator f32( ) const { return 3; }
    VD_FORCE_INLINE operator f16( ) const { return 3; }
    VD_FORCE_INLINE operator i64( ) const { return 3; }
    VD_FORCE_INLINE operator u64( ) const { return 3; }
    VD_FORCE_INLINE operator i32( ) const { return 3; }
    VD_FORCE_INLINE operator u32( ) const { return 3; }
    VD_FORCE_INLINE operator i16( ) const { return 3; }
    VD_FORCE_INLINE operator u16( ) const { return 3; }
    VD_FORCE_INLINE operator i8 ( ) const { return 3; }
    VD_FORCE_INLINE operator u8 ( ) const { return 3; }
#ifndef __MSVC__
    VD_FORCE_INLINE operator size_t( ) const { return 3; }
#endif

} Three VD_OPTIONAL;

static struct FourValue
{
    VD_FORCE_INLINE operator f64( ) const { return 4; }
    VD_FORCE_INLINE operator f32( ) const { return 4; }
    VD_FORCE_INLINE operator f16( ) const { return 4; }
    VD_FORCE_INLINE operator i64( ) const { return 4; }
    VD_FORCE_INLINE operator u64( ) const { return 4; }
    VD_FORCE_INLINE operator i32( ) const { return 4; }
    VD_FORCE_INLINE operator u32( ) const { return 4; }
    VD_FORCE_INLINE operator i16( ) const { return 4; }
    VD_FORCE_INLINE operator u16( ) const { return 4; }
    VD_FORCE_INLINE operator i8 ( ) const { return 4; }
    VD_FORCE_INLINE operator u8 ( ) const { return 4; }
#ifndef __MSVC__
    VD_FORCE_INLINE operator size_t( ) const { return 4; }
#endif

} Four VD_OPTIONAL;

static struct InvalidIndexValue 
{
    VD_FORCE_INLINE operator f64( ) const { return VD_F64_MAX; }
    VD_FORCE_INLINE operator f32( ) const { return VD_F32_MAX; }
    VD_FORCE_INLINE operator f16( ) const { return VD_F16_MAX; }
    VD_FORCE_INLINE operator i64( ) const { return VD_I64_MAX; }
    VD_FORCE_INLINE operator u64( ) const { return VD_U64_MAX; }
    VD_FORCE_INLINE operator i32( ) const { return VD_I32_MAX; }
    VD_FORCE_INLINE operator u32( ) const { return VD_U32_MAX; }
    VD_FORCE_INLINE operator i16( ) const { return VD_I16_MAX; }
    VD_FORCE_INLINE operator u16( ) const { return VD_U16_MAX; }
    VD_FORCE_INLINE operator i8 ( ) const { return VD_I8_MAX; }
    VD_FORCE_INLINE operator u8 ( ) const { return VD_U8_MAX; }
    VD_FORCE_INLINE operator uid ( ) const { static const uid v; return v; }
#ifndef __MSVC__
    VD_FORCE_INLINE operator size_t( ) const { return std::numeric_limits<size_t>::max(); }
#endif

} InvalidIndex VD_OPTIONAL;

static struct NegInfValue
{
    VD_FORCE_INLINE operator f64( ) const { return -VD_F64_MAX; }
    VD_FORCE_INLINE operator f32( ) const { return -VD_F32_MAX; }
    VD_FORCE_INLINE operator f16( ) const { return -VD_F16_MAX; }
    VD_FORCE_INLINE operator i64( ) const { return +VD_I64_MIN; }
    VD_FORCE_INLINE operator u64( ) const { return +VD_U64_MIN; }
    VD_FORCE_INLINE operator i32( ) const { return +VD_I32_MIN; }
    VD_FORCE_INLINE operator u32( ) const { return +VD_U32_MIN; }
    VD_FORCE_INLINE operator i16( ) const { return +VD_I16_MIN; }
    VD_FORCE_INLINE operator u16( ) const { return +VD_U16_MIN; }
    VD_FORCE_INLINE operator i8 ( ) const { return +VD_I8_MIN; }
    VD_FORCE_INLINE operator u8 ( ) const { return +VD_U8_MIN; }
#ifndef __MSVC__
    VD_FORCE_INLINE operator size_t( ) const { return std::numeric_limits<size_t>::min(); }
#endif

} NegativeInfinity VD_OPTIONAL, NegInf VD_OPTIONAL;

static struct PosInfValue
{
    VD_FORCE_INLINE operator f64( ) const { return +VD_F64_MAX; }
    VD_FORCE_INLINE operator f32( ) const { return +VD_F32_MAX; }
    VD_FORCE_INLINE operator f16( ) const { return +VD_F16_MAX; }
    VD_FORCE_INLINE operator i64( ) const { return +VD_I64_MAX; }
    VD_FORCE_INLINE operator u64( ) const { return +VD_U64_MAX; }
    VD_FORCE_INLINE operator i32( ) const { return +VD_I32_MAX; }
    VD_FORCE_INLINE operator u32( ) const { return +VD_U32_MAX; }
    VD_FORCE_INLINE operator i16( ) const { return +VD_I16_MAX; }
    VD_FORCE_INLINE operator u16( ) const { return +VD_U16_MAX; }
    VD_FORCE_INLINE operator i8 ( ) const { return +VD_I8_MAX; }
    VD_FORCE_INLINE operator u8 ( ) const { return +VD_U8_MAX; }
#ifndef __MSVC__
    VD_FORCE_INLINE operator size_t( ) const { return std::numeric_limits<size_t>::max(); }
#endif

} Infinity VD_OPTIONAL, PosInf VD_OPTIONAL;

static struct NaNValue
{
    VD_FORCE_INLINE operator f64( ) const { return std::numeric_limits<f64>::quiet_NaN(); }
    VD_FORCE_INLINE operator f32( ) const { return std::numeric_limits<f32>::quiet_NaN(); }
} NaN VD_OPTIONAL;

static struct UlpValue
{
    VD_FORCE_INLINE operator f64( ) const { return VD_F64_EPSILON; }
    VD_FORCE_INLINE operator f32( ) const { return VD_F32_EPSILON; }
    VD_FORCE_INLINE operator f16( ) const { return VD_F16_EPSILON; }
} Ulp VD_OPTIONAL;

static struct PiValue
{
    VD_FORCE_INLINE operator f64( ) const { return 3.14159265358979323846; }
    VD_FORCE_INLINE operator f32( ) const { return 3.14159265358979323846f; }
} Pi VD_OPTIONAL;

static struct StepValue {
} Step VD_OPTIONAL;

static struct EmptyValue {
} Empty VD_OPTIONAL;

static struct FullValue {
} Full VD_OPTIONAL;

struct MemorySize {
    static const size_t KB = 1024u;
    static const size_t MB = 1024u * 1024u;
    static const size_t GB = 1024u * 1024u * 1024u;
};

// ============================================================================================== //

VD_CONSTANTS_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CONSTANTS_VALUES_INCLUDED

