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

#ifndef VD_CORE_FLAGS_INCLUDED
#define VD_CORE_FLAGS_INCLUDED

// ============================================================================================== //

#include "core/core.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

template <class T>
class Flags
{
    typedef typename T::Enum Enum;
    typedef typename T::Bits Bits;

public:
    typedef char Description[512];

    inline Flags(void)
        : m_Flags(0)
    {
    }

    inline explicit Flags(Enum flag)
        : m_Flags(flag)
    {
    }

    inline void Set(Enum flag)
    {
        m_Flags |= flag;
    }

    inline void Remove(Enum flag)
    {
        m_Flags &= ~flag;
    }

    inline void Clear(void)
    {
        m_Flags = 0;
    }

    inline bool IsSet(Enum flag) const
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

    const char* ToString(Description& description) const
    {
        int offset = 0;

        for(size_t i = 0; i < 32; ++i)
        {
            if((m_Flags & (1u << i)) != 0)
            {
#if defined(VD_TARGET_WINDOWS)
                offset += _snprintf_s(description + offset, sizeof(description) - offset, _TRUNCATE, "%s, ", T::ToString(1u << i));
#else
                offset += snprintf(description + offset, sizeof(description) - offset, "%s, ", T::ToString(1u << i));
#endif
            }
        }

        description[offset - 2] = 0;
        return description;
    }

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
    }

    union
    {
        vd::u32 m_Flags;
        Bits m_Bits;
    };
};

// ============================================================================================== //

#define VD_DECLARE_FLAGS_ENUM(name, n)                    name = (1u << n),
#define VD_DECLARE_FLAGS_BITS(name, n)                    vd::u32 name : 1;
#define VD_DECLARE_FLAGS_TO_STRING(name, n)               case name: return VD_PP_STRINGIZE(name);

#define VD_DECLARE_FLAGS(name, ...)    \
struct name    \
{    \
  static const size_t Count = VD_PP_NUM_ARGS(__VA_ARGS__);    \
  enum Enum    \
  {    \
    VD_PP_EXPAND_ARGS VD_PP_PASS_ARGS(VD_DECLARE_FLAGS_ENUM, __VA_ARGS__)    \
  };    \
  struct Bits    \
  {    \
    VD_PP_EXPAND_ARGS VD_PP_PASS_ARGS(VD_DECLARE_FLAGS_BITS, __VA_ARGS__)    \
  };    \
  static const char* ToString(size_t value) \
  { \
    switch (value) \
    { \
      VD_PP_EXPAND_ARGS VD_PP_PASS_ARGS(VD_DECLARE_FLAGS_TO_STRING, __VA_ARGS__)    \
      default: \
        VD_PP_NO_SWITCH_DEFAULT; \
    } \
  } \
}; \
\
inline Flags operator|(name::Enum lhs, name::Enum rhs)    \
{    \
  return (Flags(lhs) | Flags(rhs));    \
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_FLAGS_INCLUDED
