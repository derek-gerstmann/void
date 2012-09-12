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

#ifndef VD_COMMON_FLAGS_INCLUDED
#define VD_COMMON_FLAGS_INCLUDED

// ============================================================================================== //

#include "common/types.h"

// ============================================================================================== //

VD_NAMESPACE_BEGIN();

// ============================================================================================== //

template <class T>
class VD_API Flags
{
    typedef typename T::Value Value;
    typedef typename T::Bits  Bits;

public:
    inline Flags(void)
        : m_Flags(0)
    {
        // EMPTY!
    }

    inline explicit Flags(Value flag)
        : m_Flags(flag)
    {
        // EMPTY!
    }

    inline void Set(Value flag)
    {
        m_Flags |= flag;
    }

    inline void Remove(Value flag)
    {
        m_Flags &= ~flag;
    }

    inline void Clear(void)
    {
        m_Flags = 0;
    }

    inline bool IsSet(Value flag) const
    {
        return ((m_Flags & flag) != 0);
    }

    inline bool IsAnySet(void) const
    {
        return (m_Flags != 0);
    }

    inline bool AreAllSet(void) const
    {
        return (m_Flags == ((1ull << T::Count) - 1u));
    }


#if 0
    const char* ToString(Description& description) const
    {
        int offset = 0;

        for(size_t i = 0; i < 32; ++i)
        {
            if((m_Flags & (1u << i)) != 0)
            {
#if defined(VD_TARGET_WINDOWS)
                offset += _snprintf_s(
                    description + offset, 
                    sizeof(description) - offset, 
                    _TRUNCATE, "%s, ", 
                    T::ToString(1u << i)
                );
#else
                offset += snprintf(
                    description + offset, 
                    sizeof(description) - offset, 
                    "%s, ", T::ToString(1u << i)
                );
#endif
            }
        }

        description[offset - 2] = 0;
        return description;
    }
#endif
    inline Flags operator|(Flags other) const
    {
        return Flags(m_Flags | other.m_Flags);
    }

    inline Flags& operator|=(Flags other)
    {
        m_Flags |= other.m_Flags;
        return *this;
    }

private:
    inline explicit Flags(vd::u32 flags)
        : m_Flags(flags)
    {
        // EMPTY!
    }

    union
    {
        vd::u32 m_Flags;
        Bits    m_Bits;
    };
};

// ============================================================================================== //

VD_NAMESPACE_END();

// ============================================================================================== //

#define VD_DECLARE_FLAGS(FlagsName, ...)                                                          \
struct VD_API FlagsName                                                                           \
{                                                                                                 \
    static const vd::u32 Count = VD_PP_VA_ARG_COUNT(__VA_ARGS__);                                 \
    static const vd::u32 StartIndex = 0;                                                          \
    static const vd::u32 EndIndex = FlagsName::StartIndex + FlagsName::Count + 1;                 \
    enum Value                                                                                    \
    {                                                                                             \
        VD_PP_PASS_ENUM_ARGS(VD_PP_EXPAND_FLAGS_VALUE_ENTRY,__VA_ARGS__)                          \
        Invalid                                                                                   \
    };                                                                                            \
    struct Bits                                                                                   \
    {                                                                                             \
        VD_PP_PASS_ENUM_ARGS(VD_PP_EXPAND_FLAGS_BITS_ENTRY,__VA_ARGS__)                           \
    };                                                                                            \
    static bool IsValid(const FlagsName::Value& value)                                            \
    {                                                                                             \
  		  return ToInteger(value) < FlagsName::EndIndex;                                          \
    }                                                                                             \
    static vd::cstr ToString(const FlagsName::Value& value)                                       \
    {                                                                                             \
    		switch (value)                                                                        \
    		{                                                                                     \
    	        VD_PP_PASS_ENUM_ARGS(VD_PP_EXPAND_FLAGS_VALUE_TO_STRING,__VA_ARGS__)              \
                case FlagsName::Invalid: return vd::cstr(VD_PP_STRINGIZE(FlagsName) "::Invalid"); \
    		    default: return vd::cstr(VD_PP_STRINGIZE(FlagsName) "::Invalid");                 \
            }                                                                                     \
    		return vd::cstr(VD_PP_STRINGIZE(FlagsName) "::Invalid");                              \
    }                                                                                             \
    static vd::u32 ToInteger(const FlagsName::Value& value)                                       \
    {                                                                                             \
        switch (value)                                                                            \
        {                                                                                         \
            VD_PP_PASS_ENUM_ARGS(VD_PP_EXPAND_FLAGS_VALUE_TO_U32,__VA_ARGS__)                     \
            case FlagsName::Invalid: return FlagsName::EndIndex;                                  \
            default: return FlagsName::EndIndex;                                                  \
        }                                                                                         \
        return FlagsName::EndIndex;                                                               \
    }                                                                                             \
    static FlagsName::Value FromInteger(const vd::u32 value)                                      \
    {                                                                                             \
        switch (value)                                                                            \
        {                                                                                         \
            VD_PP_PASS_ENUM_ARGS(VD_PP_EXPAND_FLAGS_U32_TO_VALUE,__VA_ARGS__)                     \
            case FlagsName::EndIndex: return FlagsName::Invalid;                                  \
            default: return FlagsName::Invalid;                                                   \
        }                                                                                         \
        return FlagsName::Invalid;                                                                \
    }                                                                                             \
    FlagsName::Value Data;                                                                        \
    VD_FORCE_INLINE FlagsName() { Data = FlagsName::Invalid; }                                    \
    VD_FORCE_INLINE FlagsName(const vd::u32 i) { Data = FromInteger(i); }                         \
    VD_FORCE_INLINE FlagsName(const FlagsName::Value& value) { Data = value; }                    \
    VD_FORCE_INLINE operator bool() const { return IsValid(Data); }                               \
    VD_FORCE_INLINE operator u32( ) const { return ToInteger(Data); }                             \
    VD_FORCE_INLINE operator FlagsName::Value( ) const { return Data; }                           \
    VD_FORCE_INLINE operator vd::cstr( ) const { return ToString(Data); }                         \
};                                                                                                \
                                                                                                  \
inline Flags operator|(FlagsName::Value lhs, FlagsName::Value rhs)                                \
{                                                                                                 \
     return (Flags(lhs) | Flags(rhs));                                                            \
}

// ============================================================================================== //

#define VD_PP_EXPAND_FLAGS_VALUE_ENTRY(v,n) \
  v = (1u << n),

#define VD_PP_EXPAND_FLAGS_BITS_ENTRY(v,n) \
    vd::u32 n : 1;

#define VD_PP_EXPAND_FLAGS_VALUE_TO_U32(v,n) \
  case v: return (1u << n);

#define VD_PP_EXPAND_FLAGS_U32_TO_VALUE(v,n) \
  case (1u << n): return v;

#define VD_PP_EXPAND_FLAGS_VALUE_TO_STRING(v,n) \
	case v: return vd::cstr(VD_PP_STRINGIZE(v));

// ============================================================================================== //

#endif // VD_COMMON_FLAGS_INCLUDED

// ============================================================================================== //
// END FILE


