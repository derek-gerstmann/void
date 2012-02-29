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

#ifndef VD_CONSTANTS_TYPES_INCLUDED
#define VD_CONSTANTS_TYPES_INCLUDED

// ============================================================================================== //

#include "constants/constants.h"

// ============================================================================================== //

VD_CONSTANTS_NAMESPACE_BEGIN();

// ============================================================================================== //

static struct NullType 
{

} Null VD_OPTIONAL;

static struct TrueType
 {
    VD_FORCE_INLINE operator bool( ) const { return true; }

} True VD_OPTIONAL;

static struct FalseType 
{
    VD_FORCE_INLINE operator bool( ) const { return false; }

} False VD_OPTIONAL;

static struct ZeroType
{
    VD_FORCE_INLINE operator f64( ) const { return 0; }
    VD_FORCE_INLINE operator f32( ) const { return 0; }
    VD_FORCE_INLINE operator i64( ) const { return 0; }
    VD_FORCE_INLINE operator u64( ) const { return 0; }
    VD_FORCE_INLINE operator i32( ) const { return 0; }
    VD_FORCE_INLINE operator u32( ) const { return 0; }
    VD_FORCE_INLINE operator i16( ) const { return 0; }
    VD_FORCE_INLINE operator u16( ) const { return 0; }
    VD_FORCE_INLINE operator i8 ( ) const { return 0; }
    VD_FORCE_INLINE operator u8 ( ) const { return 0; }
#ifndef __MSVC__
    VD_FORCE_INLINE operator size_t( ) const { return 0; }
#endif

} Zero VD_OPTIONAL;

static struct OneType
{
    VD_FORCE_INLINE operator f64( ) const { return 1; }
    VD_FORCE_INLINE operator f32( ) const { return 1; }
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


static struct TwoType
{
    VD_FORCE_INLINE operator f64( ) const { return 2; }
    VD_FORCE_INLINE operator f32( ) const { return 2; }
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

static struct ThreeType
{
    VD_FORCE_INLINE operator f64( ) const { return 3; }
    VD_FORCE_INLINE operator f32( ) const { return 3; }
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

static struct FourType
{
    VD_FORCE_INLINE operator f64( ) const { return 4; }
    VD_FORCE_INLINE operator f32( ) const { return 4; }
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

static struct NegInfType
{
    VD_FORCE_INLINE operator f64( ) const { return -std::numeric_limits<f64>::infinity(); }
    VD_FORCE_INLINE operator f32( ) const { return -std::numeric_limits<f32>::infinity(); }
    VD_FORCE_INLINE operator i64( ) const { return std::numeric_limits<i64>::min(); }
    VD_FORCE_INLINE operator u64( ) const { return std::numeric_limits<u64>::min(); }
    VD_FORCE_INLINE operator i32( ) const { return std::numeric_limits<i32>::min(); }
    VD_FORCE_INLINE operator u32( ) const { return std::numeric_limits<u32>::min(); }
    VD_FORCE_INLINE operator i16( ) const { return std::numeric_limits<i16>::min(); }
    VD_FORCE_INLINE operator u16( ) const { return std::numeric_limits<u16>::min(); }
    VD_FORCE_INLINE operator i8 ( ) const { return std::numeric_limits<i8>::min(); }
    VD_FORCE_INLINE operator u8 ( ) const { return std::numeric_limits<u8>::min(); }
#ifndef __MSVC__
    VD_FORCE_INLINE operator size_t( ) const { return std::numeric_limits<size_t>::min(); }
#endif

} NegativeInfinity VD_OPTIONAL, NegInf VD_OPTIONAL;

static struct PosInfType
{
    VD_FORCE_INLINE operator f64( ) const { return std::numeric_limits<f64>::infinity(); }
    VD_FORCE_INLINE operator f32( ) const { return std::numeric_limits<f32>::infinity(); }
    VD_FORCE_INLINE operator i64( ) const { return std::numeric_limits<i64>::max(); }
    VD_FORCE_INLINE operator u64( ) const { return std::numeric_limits<u64>::max(); }
    VD_FORCE_INLINE operator i32( ) const { return std::numeric_limits<i32>::max(); }
    VD_FORCE_INLINE operator u32( ) const { return std::numeric_limits<u32>::max(); }
    VD_FORCE_INLINE operator i16( ) const { return std::numeric_limits<i16>::max(); }
    VD_FORCE_INLINE operator u16( ) const { return std::numeric_limits<u16>::max(); }
    VD_FORCE_INLINE operator i8 ( ) const { return std::numeric_limits<i8>::max(); }
    VD_FORCE_INLINE operator u8 ( ) const { return std::numeric_limits<u8>::max(); }
#ifndef _WIN32
    VD_FORCE_INLINE operator size_t( ) const { return std::numeric_limits<size_t>::max(); }
#endif

} Infinity VD_OPTIONAL, PosInf VD_OPTIONAL;

static struct NaNType
{
    VD_FORCE_INLINE operator f64( ) const { return std::numeric_limits<f64>::quiet_NaN(); }
    VD_FORCE_INLINE operator f32( ) const { return std::numeric_limits<f32>::quiet_NaN(); }
} NaN VD_OPTIONAL;

static struct UlpType
{
    VD_FORCE_INLINE operator f64( ) const { return std::numeric_limits<f64>::epsilon(); }
    VD_FORCE_INLINE operator f32 ( ) const { return std::numeric_limits<f32>::epsilon(); }
} Ulp VD_OPTIONAL;

static struct PiType
{
    VD_FORCE_INLINE operator f64( ) const { return 3.14159265358979323846; }
    VD_FORCE_INLINE operator f32 ( ) const { return 3.14159265358979323846f; }
} Pi VD_OPTIONAL;

static struct StepType {
} Step VD_OPTIONAL;

static struct EmptyType {
} Empty VD_OPTIONAL;

static struct FullType {
} Full VD_OPTIONAL;

static const size_t KB = 1024u;
static const size_t MB = KB*KB;
static const size_t GB = KB*MB;

// ============================================================================================== //

VD_CONSTANTS_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CONSTANTS_TYPES_INCLUDED

