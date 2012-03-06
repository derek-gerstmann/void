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

#ifndef VD_CORE_TYPES_INCLUDED
#define VD_CORE_TYPES_INCLUDED

// ---------------------------------------------------------------------------------------------- //

#include "core/core.h"
#include "constants/constants.h"
#include "core/numerics.h"

// ---------------------------------------------------------------------------------------------- //

VD_NAMESPACE_BEGIN();

// ============================================================================================== //
// Typedefs Promoting Vector Types to Framework Namespace
// ============================================================================================== //


typedef VD_CORE_NAMESPACE::v1<bool>      v1b;
typedef VD_CORE_NAMESPACE::v1<vd::i8>    v1i8;
typedef VD_CORE_NAMESPACE::v1<vd::i16>   v1i16;
typedef VD_CORE_NAMESPACE::v1<vd::i32>   v1i32;
typedef VD_CORE_NAMESPACE::v1<vd::i64>   v1i64;
typedef VD_CORE_NAMESPACE::v1<vd::u8>    v1u8;
typedef VD_CORE_NAMESPACE::v1<vd::u16>   v1u16;
typedef VD_CORE_NAMESPACE::v1<vd::u32>   v1u32;
typedef VD_CORE_NAMESPACE::v1<vd::u64>   v1u64;
typedef VD_CORE_NAMESPACE::v1<vd::f16>   v1f16;
typedef VD_CORE_NAMESPACE::v1<vd::f32>   v1f32;
typedef VD_CORE_NAMESPACE::v1<vd::f64>   v1f64;

typedef VD_CORE_NAMESPACE::v2<bool>      v2b;
typedef VD_CORE_NAMESPACE::v2<vd::i8>    v2i8;
typedef VD_CORE_NAMESPACE::v2<vd::i16>   v2i16;
typedef VD_CORE_NAMESPACE::v2<vd::i32>   v2i32;
typedef VD_CORE_NAMESPACE::v2<vd::i64>   v2i64;
typedef VD_CORE_NAMESPACE::v2<vd::u8>    v2u8;
typedef VD_CORE_NAMESPACE::v2<vd::u16>   v2u16;
typedef VD_CORE_NAMESPACE::v2<vd::u32>   v2u32;
typedef VD_CORE_NAMESPACE::v2<vd::u64>   v2u64;
typedef VD_CORE_NAMESPACE::v2<vd::f16>   v2f16;
typedef VD_CORE_NAMESPACE::v2<vd::f32>   v2f32;
typedef VD_CORE_NAMESPACE::v2<vd::f64>   v2f64;

typedef VD_CORE_NAMESPACE::v3<bool>      v3b;
typedef VD_CORE_NAMESPACE::v3<vd::i8>    v3i8;
typedef VD_CORE_NAMESPACE::v3<vd::i16>   v3i16;
typedef VD_CORE_NAMESPACE::v3<vd::i32>   v3i32;
typedef VD_CORE_NAMESPACE::v3<vd::i64>   v3i64;
typedef VD_CORE_NAMESPACE::v3<vd::u8>    v3u8;
typedef VD_CORE_NAMESPACE::v3<vd::u16>   v3u16;
typedef VD_CORE_NAMESPACE::v3<vd::u32>   v3u32;
typedef VD_CORE_NAMESPACE::v3<vd::u64>   v3u64;
typedef VD_CORE_NAMESPACE::v3<vd::f16>   v3f16;
typedef VD_CORE_NAMESPACE::v3<vd::f32>   v3f32;
typedef VD_CORE_NAMESPACE::v3<vd::f64>   v3f64;

typedef VD_CORE_NAMESPACE::v4<bool>      v4b;
typedef VD_CORE_NAMESPACE::v4<vd::i8>    v4i8;
typedef VD_CORE_NAMESPACE::v4<vd::i16>   v4i16;
typedef VD_CORE_NAMESPACE::v4<vd::i32>   v4i32;
typedef VD_CORE_NAMESPACE::v4<vd::i64>   v4i64;
typedef VD_CORE_NAMESPACE::v4<vd::u8>    v4u8;
typedef VD_CORE_NAMESPACE::v4<vd::u16>   v4u16;
typedef VD_CORE_NAMESPACE::v4<vd::u32>   v4u32;
typedef VD_CORE_NAMESPACE::v4<vd::u64>   v4u64;
typedef VD_CORE_NAMESPACE::v4<vd::f16>   v4f16;
typedef VD_CORE_NAMESPACE::v4<vd::f32>   v4f32;
typedef VD_CORE_NAMESPACE::v4<vd::f64>   v4f64;

typedef VD_CORE_NAMESPACE::q4<vd::f16>   q4f16;     //! Quaternion 4-component floating-point 16-bit 
typedef VD_CORE_NAMESPACE::q4<vd::f32>   q4f32;     //! Quaternion 4-component floating-point 32-bit 
typedef VD_CORE_NAMESPACE::q4<vd::f64>   q4f64;     //! Quaternion 4-component floating-point 64-bit 

typedef VD_CORE_NAMESPACE::m3<vd::f16>   m3f16;     //! Matrix 3x3 dense floating-point 16-bit 
typedef VD_CORE_NAMESPACE::m3<vd::f32>   m3f32;     //! Matrix 3x3 dense floating-point 32-bit 
typedef VD_CORE_NAMESPACE::m3<vd::f64>   m3f64;     //! Matrix 3x3 dense floating-point 64-bit 

typedef VD_CORE_NAMESPACE::a4<vd::m3f16> a4f16;     //! Affine 4x3 dense floating-point 16-bit 
typedef VD_CORE_NAMESPACE::a4<vd::m3f32> a4f32;     //! Affine 4x3 dense floating-point 32-bit 
typedef VD_CORE_NAMESPACE::a4<vd::m3f64> a4f64;     //! Affine 4x3 dense floating-point 64-bit 

typedef VD_CORE_NAMESPACE::m4<vd::f16>   m4f16;     //! Matrix 4x4 dense floating-point 16-bit 
typedef VD_CORE_NAMESPACE::m4<vd::f32>   m4f32;     //! Matrix 4x4 dense floating-point 16-bit 
typedef VD_CORE_NAMESPACE::m4<vd::f64>   m4f64;     //! Matrix 4x4 dense floating-point 16-bit 

// ---------------------------------------------------------------------------------------------- //

VD_NAMESPACE_END();

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //
// Enums
// ============================================================================================== //

VD_DECLARE_ENUM(TypeId,
    B,
    I8,
    I16,
    I32,
    I64,
    U8,
    U16,
    U32,
    U64,
    F16,
    F32,
    F64,
    V2B,
    V2I8,
    V2I16,
    V2I32,
    V2I64,
    V2U8,
    V2U16,
    V2U32,
    V2U64,
    V2F16,
    V2F32,
    V2F64,
    V3B,
    V3I8,
    V3I16,
    V3I32,
    V3I64,
    V3U8,
    V3U16,
    V3U32,
    V3U64,
    V3F16,
    V3F32,
    V3F64,
    V4B,
    V4I8,
    V4I16,
    V4I32,
    V4I64,
    V4U8,
    V4U16,
    V4U32,
    V4U64,
    V4F16,
    V4F32,
    V4F64,
    M3F16,
    M3F32,
    M3F64,
    A3F16,
    A3F32,
    A3F64,
    Q4F16,
    Q4F32,
    Q4F64,
    M4F16,
    M4F32,
    M4F64);

// ---------------------------------------------------------------------------------------------- //

template <typename T>
struct TypeTraits 
{  
    static vd::cstr GetName() 
    {
        return TypeId::ToString(TypeId::Invalid);
    }

    static TypeId::Value GetTypeId() 
    {
        return TypeId::Invalid;
    }

    static vd::TypeId::Value GetBaseTypeId() 
    {
        return 0;
    }

    static int GetByteSize() 
    {
        return 0;
    }

    static int GetBitSize() 
    {
        return 0;
    }

    static int GetComponents()
    {
        return 0;
    }
};

// ============================================================================================== //
// Template specializations
// ============================================================================================== // 

template <> inline vd::cstr TypeTraits<vd::i8>::GetName()       { return TypeId::ToString(TypeId::I8);  }
template <> inline vd::cstr TypeTraits<vd::i16>::GetName()      { return TypeId::ToString(TypeId::I16); }
template <> inline vd::cstr TypeTraits<vd::i32>::GetName()      { return TypeId::ToString(TypeId::I32); }
template <> inline vd::cstr TypeTraits<vd::i64>::GetName()      { return TypeId::ToString(TypeId::I64); }
template <> inline vd::cstr TypeTraits<vd::u8>::GetName()       { return TypeId::ToString(TypeId::U8);  }
template <> inline vd::cstr TypeTraits<vd::u16>::GetName()      { return TypeId::ToString(TypeId::U16); }
template <> inline vd::cstr TypeTraits<vd::u32>::GetName()      { return TypeId::ToString(TypeId::U32); }
template <> inline vd::cstr TypeTraits<vd::u64>::GetName()      { return TypeId::ToString(TypeId::U64); }
template <> inline vd::cstr TypeTraits<vd::f16>::GetName()      { return TypeId::ToString(TypeId::F16); }
template <> inline vd::cstr TypeTraits<vd::f32>::GetName()      { return TypeId::ToString(TypeId::F32); }
template <> inline vd::cstr TypeTraits<vd::f64>::GetName()      { return TypeId::ToString(TypeId::F64); } 

template <> inline vd::cstr TypeTraits<vd::v2i8>::GetName()     { return TypeId::ToString(TypeId::V2I8);  }
template <> inline vd::cstr TypeTraits<vd::v2i16>::GetName()    { return TypeId::ToString(TypeId::V2I16); }
template <> inline vd::cstr TypeTraits<vd::v2i32>::GetName()    { return TypeId::ToString(TypeId::V2I32); }
template <> inline vd::cstr TypeTraits<vd::v2i64>::GetName()    { return TypeId::ToString(TypeId::V2I64); }
template <> inline vd::cstr TypeTraits<vd::v2u8>::GetName()     { return TypeId::ToString(TypeId::V2U8);  }
template <> inline vd::cstr TypeTraits<vd::v2u16>::GetName()    { return TypeId::ToString(TypeId::V2U16); }
template <> inline vd::cstr TypeTraits<vd::v2u32>::GetName()    { return TypeId::ToString(TypeId::V2U32); }
template <> inline vd::cstr TypeTraits<vd::v2u64>::GetName()    { return TypeId::ToString(TypeId::V2U64); }
template <> inline vd::cstr TypeTraits<vd::v2f16>::GetName()    { return TypeId::ToString(TypeId::V2F16); }
template <> inline vd::cstr TypeTraits<vd::v2f32>::GetName()    { return TypeId::ToString(TypeId::V2F32); }
template <> inline vd::cstr TypeTraits<vd::v2f64>::GetName()    { return TypeId::ToString(TypeId::V2F64); }

template <> inline vd::cstr TypeTraits<vd::v3i8>::GetName()     { return TypeId::ToString(TypeId::V3I8);  }
template <> inline vd::cstr TypeTraits<vd::v3i16>::GetName()    { return TypeId::ToString(TypeId::V3I16); }
template <> inline vd::cstr TypeTraits<vd::v3i32>::GetName()    { return TypeId::ToString(TypeId::V3I32); }
template <> inline vd::cstr TypeTraits<vd::v3i64>::GetName()    { return TypeId::ToString(TypeId::V3I64); }
template <> inline vd::cstr TypeTraits<vd::v3u8>::GetName()     { return TypeId::ToString(TypeId::V3U8);  }
template <> inline vd::cstr TypeTraits<vd::v3u16>::GetName()    { return TypeId::ToString(TypeId::V3U16); }
template <> inline vd::cstr TypeTraits<vd::v3u32>::GetName()    { return TypeId::ToString(TypeId::V3U32); }
template <> inline vd::cstr TypeTraits<vd::v3u64>::GetName()    { return TypeId::ToString(TypeId::V3U64); }
template <> inline vd::cstr TypeTraits<vd::v3f16>::GetName()    { return TypeId::ToString(TypeId::V3F16); }
template <> inline vd::cstr TypeTraits<vd::v3f32>::GetName()    { return TypeId::ToString(TypeId::V3F32); }
template <> inline vd::cstr TypeTraits<vd::v3f64>::GetName()    { return TypeId::ToString(TypeId::V3F64); } 

template <> inline vd::cstr TypeTraits<vd::v4i8>::GetName()     { return TypeId::ToString(TypeId::V4I8);  }
template <> inline vd::cstr TypeTraits<vd::v4i16>::GetName()    { return TypeId::ToString(TypeId::V4I16); }
template <> inline vd::cstr TypeTraits<vd::v4i32>::GetName()    { return TypeId::ToString(TypeId::V4I32); }
template <> inline vd::cstr TypeTraits<vd::v4i64>::GetName()    { return TypeId::ToString(TypeId::V4I64); }
template <> inline vd::cstr TypeTraits<vd::v4u8>::GetName()     { return TypeId::ToString(TypeId::V4U8);  }
template <> inline vd::cstr TypeTraits<vd::v4u16>::GetName()    { return TypeId::ToString(TypeId::V4U16); }
template <> inline vd::cstr TypeTraits<vd::v4u32>::GetName()    { return TypeId::ToString(TypeId::V4U32); }
template <> inline vd::cstr TypeTraits<vd::v4u64>::GetName()    { return TypeId::ToString(TypeId::V4U64); }
template <> inline vd::cstr TypeTraits<vd::v4f16>::GetName()    { return TypeId::ToString(TypeId::V4F16); }
template <> inline vd::cstr TypeTraits<vd::v4f32>::GetName()    { return TypeId::ToString(TypeId::V4F32); }
template <> inline vd::cstr TypeTraits<vd::v4f64>::GetName()    { return TypeId::ToString(TypeId::V4F64); }

// ---------------------------------------------------------------------------------------------- //

template <> inline vd::TypeId::Value TypeTraits<vd::i8>::GetBaseTypeId()       {  return vd::TypeId::Char; }
template <> inline vd::TypeId::Value TypeTraits<vd::i16>::GetBaseTypeId()      {  return vd::TypeId::Short; }
template <> inline vd::TypeId::Value TypeTraits<vd::i32>::GetBaseTypeId()      {  return vd::TypeId::Integer; }
template <> inline vd::TypeId::Value TypeTraits<vd::i64>::GetBaseTypeId()      {  return vd::TypeId::Long; }
template <> inline vd::TypeId::Value TypeTraits<vd::u8>::GetBaseTypeId()       {  return vd::TypeId::UnsignedChar; }
template <> inline vd::TypeId::Value TypeTraits<vd::u16>::GetBaseTypeId()      {  return vd::TypeId::UnsignedShort; }
template <> inline vd::TypeId::Value TypeTraits<vd::u32>::GetBaseTypeId()      {  return vd::TypeId::UnsignedInteger; }
template <> inline vd::TypeId::Value TypeTraits<vd::u64>::GetBaseTypeId()      {  return vd::TypeId::UnsignedLong; }
template <> inline vd::TypeId::Value TypeTraits<vd::f16>::GetBaseTypeId()      {  return vd::TypeId::Half; }
template <> inline vd::TypeId::Value TypeTraits<vd::f32>::GetBaseTypeId()      {  return vd::TypeId::Float; }
template <> inline vd::TypeId::Value TypeTraits<vd::f64>::GetBaseTypeId()      {  return vd::TypeId::Double; }

template <> inline vd::TypeId::Value TypeTraits<vd::v2i8>::GetBaseTypeId()     {  return vd::TypeId::Char; }
template <> inline vd::TypeId::Value TypeTraits<vd::v2i16>::GetBaseTypeId()    {  return vd::TypeId::Short; }
template <> inline vd::TypeId::Value TypeTraits<vd::v2i32>::GetBaseTypeId()    {  return vd::TypeId::Integer; }
template <> inline vd::TypeId::Value TypeTraits<vd::v2i64>::GetBaseTypeId()    {  return vd::TypeId::Long; }
template <> inline vd::TypeId::Value TypeTraits<vd::v2u8>::GetBaseTypeId()     {  return vd::TypeId::UnsignedChar; }
template <> inline vd::TypeId::Value TypeTraits<vd::v2u16>::GetBaseTypeId()    {  return vd::TypeId::UnsignedShort; }
template <> inline vd::TypeId::Value TypeTraits<vd::v2u32>::GetBaseTypeId()    {  return vd::TypeId::UnsignedInteger; }
template <> inline vd::TypeId::Value TypeTraits<vd::v2u64>::GetBaseTypeId()    {  return vd::TypeId::UnsignedLong; }
template <> inline vd::TypeId::Value TypeTraits<vd::v2f16>::GetBaseTypeId()    {  return vd::TypeId::Half; }
template <> inline vd::TypeId::Value TypeTraits<vd::v2f32>::GetBaseTypeId()    {  return vd::TypeId::Float; }
template <> inline vd::TypeId::Value TypeTraits<vd::v2f64>::GetBaseTypeId()    {  return vd::TypeId::Double; }

template <> inline vd::TypeId::Value TypeTraits<vd::v3i8>::GetBaseTypeId()     {  return vd::TypeId::Char; }
template <> inline vd::TypeId::Value TypeTraits<vd::v3i16>::GetBaseTypeId()    {  return vd::TypeId::Short; }
template <> inline vd::TypeId::Value TypeTraits<vd::v3i32>::GetBaseTypeId()    {  return vd::TypeId::Integer; }
template <> inline vd::TypeId::Value TypeTraits<vd::v3i64>::GetBaseTypeId()    {  return vd::TypeId::Long; }
template <> inline vd::TypeId::Value TypeTraits<vd::v3u8>::GetBaseTypeId()     {  return vd::TypeId::UnsignedChar; }
template <> inline vd::TypeId::Value TypeTraits<vd::v3u16>::GetBaseTypeId()    {  return vd::TypeId::UnsignedShort; }
template <> inline vd::TypeId::Value TypeTraits<vd::v3u32>::GetBaseTypeId()    {  return vd::TypeId::UnsignedInteger; }
template <> inline vd::TypeId::Value TypeTraits<vd::v3u64>::GetBaseTypeId()    {  return vd::TypeId::UnsignedLong; }
template <> inline vd::TypeId::Value TypeTraits<vd::v3f16>::GetBaseTypeId()    {  return vd::TypeId::Half; }
template <> inline vd::TypeId::Value TypeTraits<vd::v3f32>::GetBaseTypeId()    {  return vd::TypeId::Float; }
template <> inline vd::TypeId::Value TypeTraits<vd::v3f64>::GetBaseTypeId()    {  return vd::TypeId::Double; }

template <> inline vd::TypeId::Value TypeTraits<vd::v4i8>::GetBaseTypeId()     {  return vd::TypeId::Char; }
template <> inline vd::TypeId::Value TypeTraits<vd::v4i16>::GetBaseTypeId()    {  return vd::TypeId::Short; }
template <> inline vd::TypeId::Value TypeTraits<vd::v4i32>::GetBaseTypeId()    {  return vd::TypeId::Integer; }
template <> inline vd::TypeId::Value TypeTraits<vd::v4i64>::GetBaseTypeId()    {  return vd::TypeId::Long; }
template <> inline vd::TypeId::Value TypeTraits<vd::v4u8>::GetBaseTypeId()     {  return vd::TypeId::UnsignedChar; }
template <> inline vd::TypeId::Value TypeTraits<vd::v4u16>::GetBaseTypeId()    {  return vd::TypeId::UnsignedShort; }
template <> inline vd::TypeId::Value TypeTraits<vd::v4u32>::GetBaseTypeId()    {  return vd::TypeId::UnsignedInteger; }
template <> inline vd::TypeId::Value TypeTraits<vd::v4u64>::GetBaseTypeId()    {  return vd::TypeId::UnsignedLong; }
template <> inline vd::TypeId::Value TypeTraits<vd::v4f16>::GetBaseTypeId()    {  return vd::TypeId::Half; }
template <> inline vd::TypeId::Value TypeTraits<vd::v4f32>::GetBaseTypeId()    {  return vd::TypeId::Float; }
template <> inline vd::TypeId::Value TypeTraits<vd::v4f64>::GetBaseTypeId()    {  return vd::TypeId::Double; }

// ---------------------------------------------------------------------------------------------- //

template <> inline TypeId::Value TypeTraits<vd::i8>::GetTypeId()       {  return TypeId::I8; }
template <> inline TypeId::Value TypeTraits<vd::i16>::GetTypeId()      {  return TypeId::I16; }
template <> inline TypeId::Value TypeTraits<vd::i32>::GetTypeId()      {  return TypeId::I32; }
template <> inline TypeId::Value TypeTraits<vd::i64>::GetTypeId()      {  return TypeId::I64; }
template <> inline TypeId::Value TypeTraits<vd::u8>::GetTypeId()       {  return TypeId::U8; }
template <> inline TypeId::Value TypeTraits<vd::u16>::GetTypeId()      {  return TypeId::U16; }
template <> inline TypeId::Value TypeTraits<vd::u32>::GetTypeId()      {  return TypeId::U32; }
template <> inline TypeId::Value TypeTraits<vd::u64>::GetTypeId()      {  return TypeId::U64; }
template <> inline TypeId::Value TypeTraits<vd::f16>::GetTypeId()      {  return TypeId::F16; }
template <> inline TypeId::Value TypeTraits<vd::f32>::GetTypeId()      {  return TypeId::F32; }
template <> inline TypeId::Value TypeTraits<vd::f64>::GetTypeId()      {  return TypeId::F64; }

template <> inline TypeId::Value TypeTraits<vd::v2i8>::GetTypeId()     {  return TypeId::V2I8; }
template <> inline TypeId::Value TypeTraits<vd::v2i16>::GetTypeId()    {  return TypeId::V2I16; }
template <> inline TypeId::Value TypeTraits<vd::v2i32>::GetTypeId()    {  return TypeId::V2I32; }
template <> inline TypeId::Value TypeTraits<vd::v2i64>::GetTypeId()    {  return TypeId::V2I64; }
template <> inline TypeId::Value TypeTraits<vd::v2u8>::GetTypeId()     {  return TypeId::V2U8; }
template <> inline TypeId::Value TypeTraits<vd::v2u16>::GetTypeId()    {  return TypeId::V2U16; }
template <> inline TypeId::Value TypeTraits<vd::v2u32>::GetTypeId()    {  return TypeId::V2U32; }
template <> inline TypeId::Value TypeTraits<vd::v2u64>::GetTypeId()    {  return TypeId::V2U64; }
template <> inline TypeId::Value TypeTraits<vd::v2f16>::GetTypeId()    {  return TypeId::V2F16; }
template <> inline TypeId::Value TypeTraits<vd::v2f32>::GetTypeId()    {  return TypeId::V2F32; }
template <> inline TypeId::Value TypeTraits<vd::v2f64>::GetTypeId()    {  return TypeId::V2F64; }

template <> inline TypeId::Value TypeTraits<vd::v3i8>::GetTypeId()     {  return TypeId::V3I8; }
template <> inline TypeId::Value TypeTraits<vd::v3i16>::GetTypeId()    {  return TypeId::V3I16; }
template <> inline TypeId::Value TypeTraits<vd::v3i32>::GetTypeId()    {  return TypeId::V3I32; }
template <> inline TypeId::Value TypeTraits<vd::v3i64>::GetTypeId()    {  return TypeId::V3I64; }
template <> inline TypeId::Value TypeTraits<vd::v3u8>::GetTypeId()     {  return TypeId::V3U8; }
template <> inline TypeId::Value TypeTraits<vd::v3u16>::GetTypeId()    {  return TypeId::V3U16; }
template <> inline TypeId::Value TypeTraits<vd::v3u32>::GetTypeId()    {  return TypeId::V3U32; }
template <> inline TypeId::Value TypeTraits<vd::v3u64>::GetTypeId()    {  return TypeId::V3U64; }
template <> inline TypeId::Value TypeTraits<vd::v3f16>::GetTypeId()    {  return TypeId::V3F16; }
template <> inline TypeId::Value TypeTraits<vd::v3f32>::GetTypeId()    {  return TypeId::V3F32; }
template <> inline TypeId::Value TypeTraits<vd::v3f64>::GetTypeId()    {  return TypeId::V3F64; }

template <> inline TypeId::Value TypeTraits<vd::v4i8>::GetTypeId()     {  return TypeId::V4I8; }
template <> inline TypeId::Value TypeTraits<vd::v4i16>::GetTypeId()    {  return TypeId::V4I16; }
template <> inline TypeId::Value TypeTraits<vd::v4i32>::GetTypeId()    {  return TypeId::V4I32; }
template <> inline TypeId::Value TypeTraits<vd::v4i64>::GetTypeId()    {  return TypeId::V4I64; }
template <> inline TypeId::Value TypeTraits<vd::v4u8>::GetTypeId()     {  return TypeId::V4U8; }
template <> inline TypeId::Value TypeTraits<vd::v4u16>::GetTypeId()    {  return TypeId::V4U16; }
template <> inline TypeId::Value TypeTraits<vd::v4u32>::GetTypeId()    {  return TypeId::V4U32; }
template <> inline TypeId::Value TypeTraits<vd::v4u64>::GetTypeId()    {  return TypeId::V4U64; }
template <> inline TypeId::Value TypeTraits<vd::v4f16>::GetTypeId()    {  return TypeId::V4F16; }
template <> inline TypeId::Value TypeTraits<vd::v4f32>::GetTypeId()    {  return TypeId::V4F32; }
template <> inline TypeId::Value TypeTraits<vd::v4f64>::GetTypeId()    {  return TypeId::V4F64; }

// ---------------------------------------------------------------------------------------------- //

template <> inline int TypeTraits<vd::i8>::GetByteSize()       {  return 1; }
template <> inline int TypeTraits<vd::i16>::GetByteSize()      {  return 2; }
template <> inline int TypeTraits<vd::i32>::GetByteSize()      {  return 4; }
template <> inline int TypeTraits<vd::i64>::GetByteSize()      {  return 8; }
template <> inline int TypeTraits<vd::u8>::GetByteSize()       {  return 1; }
template <> inline int TypeTraits<vd::u16>::GetByteSize()      {  return 2; }
template <> inline int TypeTraits<vd::u32>::GetByteSize()      {  return 4; }
template <> inline int TypeTraits<vd::u64>::GetByteSize()      {  return 8; }
template <> inline int TypeTraits<vd::f16>::GetByteSize()      {  return 2; }
template <> inline int TypeTraits<vd::f32>::GetByteSize()      {  return 4; }
template <> inline int TypeTraits<vd::f64>::GetByteSize()      {  return 8; }

template <> inline int TypeTraits<vd::v2i8>::GetByteSize()       {  return 1; }
template <> inline int TypeTraits<vd::v2i16>::GetByteSize()      {  return 2; }
template <> inline int TypeTraits<vd::v2i32>::GetByteSize()      {  return 4; }
template <> inline int TypeTraits<vd::v2i64>::GetByteSize()      {  return 8; }
template <> inline int TypeTraits<vd::v2u8>::GetByteSize()       {  return 1; }
template <> inline int TypeTraits<vd::v2u16>::GetByteSize()      {  return 2; }
template <> inline int TypeTraits<vd::v2u32>::GetByteSize()      {  return 4; }
template <> inline int TypeTraits<vd::v2u64>::GetByteSize()      {  return 8; }
template <> inline int TypeTraits<vd::v2f16>::GetByteSize()      {  return 2; }
template <> inline int TypeTraits<vd::v2f32>::GetByteSize()      {  return 4; }
template <> inline int TypeTraits<vd::v2f64>::GetByteSize()      {  return 8; }

template <> inline int TypeTraits<vd::v3i8>::GetByteSize()       {  return 1; }
template <> inline int TypeTraits<vd::v3i16>::GetByteSize()      {  return 2; }
template <> inline int TypeTraits<vd::v3i32>::GetByteSize()      {  return 4; }
template <> inline int TypeTraits<vd::v3i64>::GetByteSize()      {  return 8; }
template <> inline int TypeTraits<vd::v3u8>::GetByteSize()       {  return 1; }
template <> inline int TypeTraits<vd::v3u16>::GetByteSize()      {  return 2; }
template <> inline int TypeTraits<vd::v3u32>::GetByteSize()      {  return 4; }
template <> inline int TypeTraits<vd::v3u64>::GetByteSize()      {  return 8; }
template <> inline int TypeTraits<vd::v3f16>::GetByteSize()      {  return 2; }
template <> inline int TypeTraits<vd::v3f32>::GetByteSize()      {  return 4; }
template <> inline int TypeTraits<vd::v3f64>::GetByteSize()      {  return 8; }

template <> inline int TypeTraits<vd::v4i8>::GetByteSize()       {  return 1; }
template <> inline int TypeTraits<vd::v4i16>::GetByteSize()      {  return 2; }
template <> inline int TypeTraits<vd::v4i32>::GetByteSize()      {  return 4; }
template <> inline int TypeTraits<vd::v4i64>::GetByteSize()      {  return 8; }
template <> inline int TypeTraits<vd::v4u8>::GetByteSize()       {  return 1; }
template <> inline int TypeTraits<vd::v4u16>::GetByteSize()      {  return 2; }
template <> inline int TypeTraits<vd::v4u32>::GetByteSize()      {  return 4; }
template <> inline int TypeTraits<vd::v4u64>::GetByteSize()      {  return 8; }
template <> inline int TypeTraits<vd::v4f16>::GetByteSize()      {  return 2; }
template <> inline int TypeTraits<vd::v4f32>::GetByteSize()      {  return 4; }
template <> inline int TypeTraits<vd::v4f64>::GetByteSize()      {  return 8; }

// ---------------------------------------------------------------------------------------------- //

template <> inline int TypeTraits<bool>::GetBitSize()         {  return 8; }
template <> inline int TypeTraits<vd::i8>::GetBitSize()       {  return 8; }
template <> inline int TypeTraits<vd::i16>::GetBitSize()      {  return 16; }
template <> inline int TypeTraits<vd::i32>::GetBitSize()      {  return 32; }
template <> inline int TypeTraits<vd::i64>::GetBitSize()      {  return 64; }
template <> inline int TypeTraits<vd::u8>::GetBitSize()       {  return 8; }
template <> inline int TypeTraits<vd::u16>::GetBitSize()      {  return 16; }
template <> inline int TypeTraits<vd::u32>::GetBitSize()      {  return 32; }
template <> inline int TypeTraits<vd::u64>::GetBitSize()      {  return 64; }
template <> inline int TypeTraits<vd::f16>::GetBitSize()      {  return 16; }
template <> inline int TypeTraits<vd::f32>::GetBitSize()      {  return 32; }
template <> inline int TypeTraits<vd::f64>::GetBitSize()      {  return 64; }

template <> inline int TypeTraits<vd::v2b>::GetBitSize()      {  return 8; }
template <> inline int TypeTraits<vd::v2i8>::GetBitSize()     {  return 8; }
template <> inline int TypeTraits<vd::v2i16>::GetBitSize()    {  return 16; }
template <> inline int TypeTraits<vd::v2i32>::GetBitSize()    {  return 32; }
template <> inline int TypeTraits<vd::v2i64>::GetBitSize()    {  return 64; }
template <> inline int TypeTraits<vd::v2u8>::GetBitSize()     {  return 8; }
template <> inline int TypeTraits<vd::v2u16>::GetBitSize()    {  return 16; }
template <> inline int TypeTraits<vd::v2u32>::GetBitSize()    {  return 32; }
template <> inline int TypeTraits<vd::v2u64>::GetBitSize()    {  return 64; }
template <> inline int TypeTraits<vd::v2f16>::GetBitSize()    {  return 16; }
template <> inline int TypeTraits<vd::v2f32>::GetBitSize()    {  return 32; }
template <> inline int TypeTraits<vd::v2f64>::GetBitSize()    {  return 64; }

template <> inline int TypeTraits<vd::v3b>::GetBitSize()      {  return 8; }
template <> inline int TypeTraits<vd::v3i8>::GetBitSize()     {  return 8; }
template <> inline int TypeTraits<vd::v3i16>::GetBitSize()    {  return 16; }
template <> inline int TypeTraits<vd::v3i32>::GetBitSize()    {  return 32; }
template <> inline int TypeTraits<vd::v3i64>::GetBitSize()    {  return 64; }
template <> inline int TypeTraits<vd::v3u8>::GetBitSize()     {  return 8; }
template <> inline int TypeTraits<vd::v3u16>::GetBitSize()    {  return 16; }
template <> inline int TypeTraits<vd::v3u32>::GetBitSize()    {  return 32; }
template <> inline int TypeTraits<vd::v3u64>::GetBitSize()    {  return 64; }
template <> inline int TypeTraits<vd::v3f16>::GetBitSize()    {  return 16; }
template <> inline int TypeTraits<vd::v3f32>::GetBitSize()    {  return 32; }
template <> inline int TypeTraits<vd::v3f64>::GetBitSize()    {  return 64; }

template <> inline int TypeTraits<vd::v4b>::GetBitSize()      {  return 8; }
template <> inline int TypeTraits<vd::v4i8>::GetBitSize()     {  return 8; }
template <> inline int TypeTraits<vd::v4i16>::GetBitSize()    {  return 16; }
template <> inline int TypeTraits<vd::v4i32>::GetBitSize()    {  return 32; }
template <> inline int TypeTraits<vd::v4i64>::GetBitSize()    {  return 64; }
template <> inline int TypeTraits<vd::v4u8>::GetBitSize()     {  return 8; }
template <> inline int TypeTraits<vd::v4u16>::GetBitSize()    {  return 16; }
template <> inline int TypeTraits<vd::v4u32>::GetBitSize()    {  return 32; }
template <> inline int TypeTraits<vd::v4u64>::GetBitSize()    {  return 64; }
template <> inline int TypeTraits<vd::v4f16>::GetBitSize()    {  return 16; }
template <> inline int TypeTraits<vd::v4f32>::GetBitSize()    {  return 32; }
template <> inline int TypeTraits<vd::v4f64>::GetBitSize()    {  return 64; }

// ---------------------------------------------------------------------------------------------- //

template <> inline int TypeTraits<bool>::GetComponents()         {  return 1; }
template <> inline int TypeTraits<vd::i8>::GetComponents()       {  return 1; }
template <> inline int TypeTraits<vd::i16>::GetComponents()      {  return 1; }
template <> inline int TypeTraits<vd::i32>::GetComponents()      {  return 1; }
template <> inline int TypeTraits<vd::i64>::GetComponents()      {  return 1; }
template <> inline int TypeTraits<vd::u8>::GetComponents()       {  return 1; }
template <> inline int TypeTraits<vd::u16>::GetComponents()      {  return 1; }
template <> inline int TypeTraits<vd::u32>::GetComponents()      {  return 1; }
template <> inline int TypeTraits<vd::u64>::GetComponents()      {  return 1; }
template <> inline int TypeTraits<vd::f16>::GetComponents()      {  return 1; }
template <> inline int TypeTraits<vd::f32>::GetComponents()      {  return 1; }
template <> inline int TypeTraits<vd::f64>::GetComponents()      {  return 1; }

template <> inline int TypeTraits<vd::v2b>::GetComponents()      {  return 2; }
template <> inline int TypeTraits<vd::v2i8>::GetComponents()     {  return 2; }
template <> inline int TypeTraits<vd::v2i16>::GetComponents()    {  return 2; }
template <> inline int TypeTraits<vd::v2i32>::GetComponents()    {  return 2; }
template <> inline int TypeTraits<vd::v2i64>::GetComponents()    {  return 2; }
template <> inline int TypeTraits<vd::v2u8>::GetComponents()     {  return 2; }
template <> inline int TypeTraits<vd::v2u16>::GetComponents()    {  return 2; }
template <> inline int TypeTraits<vd::v2u32>::GetComponents()    {  return 2; }
template <> inline int TypeTraits<vd::v2u64>::GetComponents()    {  return 2; }
template <> inline int TypeTraits<vd::v2f16>::GetComponents()    {  return 2; }
template <> inline int TypeTraits<vd::v2f32>::GetComponents()    {  return 2; }
template <> inline int TypeTraits<vd::v2f64>::GetComponents()    {  return 2; }

template <> inline int TypeTraits<vd::v3b>::GetComponents()      {  return 3; }
template <> inline int TypeTraits<vd::v3i8>::GetComponents()     {  return 3; }
template <> inline int TypeTraits<vd::v3i16>::GetComponents()    {  return 3; }
template <> inline int TypeTraits<vd::v3i32>::GetComponents()    {  return 3; }
template <> inline int TypeTraits<vd::v3i64>::GetComponents()    {  return 3; }
template <> inline int TypeTraits<vd::v3u8>::GetComponents()     {  return 3; }
template <> inline int TypeTraits<vd::v3u16>::GetComponents()    {  return 3; }
template <> inline int TypeTraits<vd::v3u32>::GetComponents()    {  return 3; }
template <> inline int TypeTraits<vd::v3u64>::GetComponents()    {  return 3; }
template <> inline int TypeTraits<vd::v3f16>::GetComponents()    {  return 3; }
template <> inline int TypeTraits<vd::v3f32>::GetComponents()    {  return 3; }
template <> inline int TypeTraits<vd::v3f64>::GetComponents()    {  return 3; }

template <> inline int TypeTraits<vd::v4b>::GetComponents()      {  return 4; }
template <> inline int TypeTraits<vd::v4i8>::GetComponents()     {  return 4; }
template <> inline int TypeTraits<vd::v4i16>::GetComponents()    {  return 4; }
template <> inline int TypeTraits<vd::v4i32>::GetComponents()    {  return 4; }
template <> inline int TypeTraits<vd::v4i64>::GetComponents()    {  return 4; }
template <> inline int TypeTraits<vd::v4u8>::GetComponents()     {  return 4; }
template <> inline int TypeTraits<vd::v4u16>::GetComponents()    {  return 4; }
template <> inline int TypeTraits<vd::v4u32>::GetComponents()    {  return 4; }
template <> inline int TypeTraits<vd::v4u64>::GetComponents()    {  return 4; }
template <> inline int TypeTraits<vd::v4f16>::GetComponents()    {  return 4; }
template <> inline int TypeTraits<vd::v4f32>::GetComponents()    {  return 4; }
template <> inline int TypeTraits<vd::v4f64>::GetComponents()    {  return 4; }

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_TYPES_INCLUDED

