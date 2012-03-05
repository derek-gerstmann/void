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

#if 0
template <typename T>
struct VD_API Enumerate
{   
    static vd::u32 ToId(const T& v)
    {
        return 0;
    }

    static T FromId(const vd::u32 v)
    {
        return 0;
    }

    static vd::string ToString(const T& v)
    {
        return 0;
    }

    static T FromString(const vd::string& str)
    {
        return T();
    }
};
#endif

// ============================================================================================== //

#define VD_DECLARE_ENUM_STARTING_FROM(StructName, StartFrom, ...)                                 \
struct VD_API StructName                                                                          \
{                                                                                                 \
    static const vd::u32 Count = VD_PP_VA_ARG_COUNT(__VA_ARGS__);                                 \
    static const vd::u32 StartIndex = StartFrom;                                                  \
    static const vd::u32 EndIndex = StructName::StartIndex + StructName::Count + 1;               \
    enum Value                                                                                    \
    {                                                                                             \
        VD_PP_PASS_ENUM_ARGS(VD_PP_EXPAND_ENUM_ENTRY,__VA_ARGS__)                                 \
        Invalid                                                                                   \
    };                                                                                            \
    static const bool IsValid(StructName::Value value)                                            \
    {                                                                                             \
  		  return ToInteger(value) < StructName::EndIndex;                                         \
    }                                                                                             \
    static vd::cstr ToString(StructName::Value value)                                             \
    {                                                                                             \
    		switch (value)                                                                        \
    		{                                                                                     \
    			  VD_PP_PASS_ENUM_ARGS(VD_PP_EXPAND_ENUM_VALUE_TO_STRING,__VA_ARGS__)             \
            case StructName::Invalid: return (vd::cstr)(VD_PP_STRINGIZE(StructName) "::Invalid"); \
    			  default: VD_NO_SWITCH_DEFAULT;                                                  \
        }                                                                                         \
    		return NULL;                                                                          \
    }                                                                                             \
    static const vd::u32 ToInteger(StructName::Value value)                                       \
    {                                                                                             \
        switch (value)                                                                            \
        {                                                                                         \
            VD_PP_PASS_ENUM_ARGS(VD_PP_EXPAND_ENUM_VALUE_TO_U32,__VA_ARGS__)                      \
            case StructName::Invalid: return StructName::EndIndex;                                \
            default: VD_NO_SWITCH_DEFAULT;                                                        \
        }                                                                                         \
        return StructName::EndIndex;                                                              \
    }                                                                                             \
    static const StructName::Value FromInteger(vd::u32 value)                                     \
    {                                                                                             \
        switch (value)                                                                            \
        {                                                                                         \
            VD_PP_PASS_ENUM_ARGS(VD_PP_EXPAND_ENUM_U32_TO_VALUE,__VA_ARGS__)                      \
            case StructName::EndIndex: return StructName::Invalid;                                \
            default: VD_NO_SWITCH_DEFAULT;                                                        \
        }                                                                                         \
        return StructName::Invalid;                                                               \
    }                                                                                             \
}


// ============================================================================================== //

#define VD_DECLARE_ENUM(StructName, ...)                                                          \
  VD_DECLARE_ENUM_STARTING_FROM(StructName, 0, __VA_ARGS__)

// ============================================================================================== //

#define VD_PP_EXPAND_ENUM_ENTRY(v,n) \
  v = (n + StartIndex - 1),

#define VD_PP_EXPAND_ENUM_VALUE_TO_U32(v,n) \
  case v: return (n + StartIndex - 1);

#define VD_PP_EXPAND_ENUM_U32_TO_VALUE(v,n) \
  case n: return v;

#define VD_PP_EXPAND_ENUM_VALUE_TO_STRING(v,n) \
	case v: return VD_PP_STRINGIZE(v);

// ============================================================================================== //

#endif // VD_CORE_ENUM_INCLUDED

