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

#ifndef VD_CORE_UTILITIES_INCLUDED
#define VD_CORE_UTILITIES_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/object.h"

// ============================================================================================== //

#include <string>
#include <sstream>
#include <iomanip>

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

namespace Convert
{

    template <typename T> VD_INLINE
    vd::string ToString(const T& t) 
    { 
    	std::ostringstream os; 
    	os << t; 
    	return os.str(); 
    }

    template <> VD_INLINE
    vd::string ToString(const Object& obj)
    {
        return obj.ToString();
    }

    vd::string 
    ToString(vd::i32 v, vd::i32 padding, char fill);

    vd::string 
    ToString(vd::f32 v, vd::i32 precision); 

    vd::string 
    ToString(const char* fmt, ...);

    vd::string 
    ToLowerCase(const vd::string& str);

    vd::string 
    ToUpperCase(const vd::string& str);

};

template <typename T>
inline intptr_t OffsetFrom(T x) {
  return x - static_cast<T>(0);
}

template <typename T>
inline T AddressFrom(intptr_t x) {
  return static_cast<T>(static_cast<T>(0) + x);
}

template <typename T, typename U>
inline bool IsAligned(T value, U alignment) 
{
    return (value & (alignment - 1)) == 0;
}

inline bool IsAddressAligned(
    vd::address addr, intptr_t alignment, int offset = 0) 
{
    intptr_t offs = OffsetFrom(addr + offset);
    return IsAligned(offs, alignment);
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_UTILITIES_INCLUDED

