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

#ifndef VD_CORE_ENUM_INCLUDED
#define VD_CORE_ENUM_INCLUDED

// ============================================================================================== //

#include "core/core.h"

// ============================================================================================== //

#define VD_DECLARE_ENUM(StructName, ...)    \
struct VD_API StructName \
{    \
  static const vd::u32 Count = VD_PP_VA_ARG_COUNT(__VA_ARGS__);    \
  enum Value    \
  {    \
      	VD_PP_PASS_ENUM_ARGS(VD_DECLARE_ONE_BASED_ENUM_VALUE,__VA_ARGS__)    \
        Invalid ## StructName, \
      	LastValue \
  };    \
  static const bool IsValid(StructName::Value value) \
  { \
		return ToInteger(value) < VD_U32_MAX; \
  } \
  static const char* ToString(vd::u32 value) \
  { \
		switch (value) \
		{ \
			VD_PP_PASS_ENUM_ARGS(VD_DECLARE_ENUM_VALUE_TO_STRING,__VA_ARGS__)    \
      case Invalid ## StructName: return "Invalid" VD_PP_STRINGIZE(StructName); \
			case LastValue: return "LastValue"; \
			default: \
			VD_NO_SWITCH_DEFAULT; \
		} \
		return NULL; \
  } \
  static const vd::u32 ToInteger(StructName::Value value) \
  { \
    switch (value) \
    { \
        VD_PP_PASS_ENUM_ARGS(VD_DECLARE_ONE_BASED_ENUM_VALUE_TO_INT,__VA_ARGS__) \
      case Invalid ## StructName: return VD_U32_MAX; \
      case LastValue: return VD_U32_MAX; \
      default: \
        VD_NO_SWITCH_DEFAULT; \
    } \
    return VD_U32_MAX; \
  } \
  static const StructName::Value FromInteger(vd::u32 value) \
  { \
    switch (value) \
    { \
        VD_PP_PASS_ENUM_ARGS(VD_DECLARE_ENUM_INT_TO_VALUE,__VA_ARGS__) \
      case VD_U32_MAX: return Invalid ## StructName; \
      default: \
        VD_NO_SWITCH_DEFAULT; \
    } \
    return Invalid ## StructName; \
  } \
}

#define VD_DECLARE_ENUM_START_FROM_ZERO(StructName, ...)    \
struct VD_API StructName \
{    \
  static const vd::u32 Count = VD_PP_VA_ARG_COUNT(__VA_ARGS__);    \
  enum Value    \
  {    \
        VD_PP_PASS_ENUM_ARGS(VD_DECLARE_ZERO_BASED_ENUM_VALUE,__VA_ARGS__)    \
        Invalid ## StructName, \
        LastValue \
  };    \
  static const bool IsValid(StructName::Value value) \
  { \
    	return ToInteger(value) >= 0 && ToInteger(value) < VD_U32_MAX; \
  } \
  static const char* ToString(vd::u32 value) \
  { \
    switch (value) \
    { \
        VD_PP_PASS_ENUM_ARGS(VD_DECLARE_ENUM_VALUE_TO_STRING,__VA_ARGS__)    \
      case Invalid ## StructName: return "Invalid" VD_PP_STRINGIZE(StructName); \
      case LastValue: return "LastValue"; \
      default: \
        VD_NO_SWITCH_DEFAULT; \
    } \
    return NULL; \
  } \
  static const vd::u32 ToInteger(StructName::Value value) \
  { \
    switch (value) \
    { \
        VD_PP_PASS_ENUM_ARGS(VD_DECLARE_ZERO_BASED_ENUM_VALUE_TO_INT,__VA_ARGS__) \
      case Invalid ## StructName: return VD_U32_MAX; \
      case LastValue: return VD_U32_MAX; \
      default: \
        VD_NO_SWITCH_DEFAULT; \
    } \
    return VD_U32_MAX; \
  } \
  static const StructName::Value FromInteger(vd::u32 value) \
  { \
    switch (value) \
    { \
        VD_PP_PASS_ENUM_ARGS(VD_DECLARE_ENUM_INT_TO_VALUE,__VA_ARGS__) \
      default: \
        VD_NO_SWITCH_DEFAULT; \
    } \
    return LastValue; \
  } \
}

// ============================================================================================== //

#define VD_DECLARE_ONE_BASED_ENUM_VALUE(v,n) \
  v = n,

#define VD_DECLARE_ONE_BASED_ENUM_VALUE_TO_INT(v,n) \
  case v: return n;

#define VD_DECLARE_ZERO_BASED_ENUM_VALUE(v,n) \
	v = n-1,

#define VD_DECLARE_ZERO_BASED_ENUM_VALUE_TO_INT(v,n) \
  case v: return n-1;

#define VD_DECLARE_ENUM_INT_TO_VALUE(v,n) \
  case n: return v;

#define VD_DECLARE_ENUM_VALUE_TO_STRING(v,n) \
	case v: return VD_PP_STRINGIZE(v);

// ============================================================================================== //

#endif // VD_CORE_ENUM_INCLUDED

