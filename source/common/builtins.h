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

#ifndef VD_FRAMEWORK_BUILTINS_INCLUDED
#define VD_FRAMEWORK_BUILTINS_INCLUDED

// ============================================================================================== //

#include "vd.h"

// ============================================================================================== //

VD_NAMESPACE_BEGIN();

// ============================================================================================== //
// Architecture specific macros and builtin methods
// ============================================================================================== //

#if (defined(VD_ARCH_X86) || defined(VD_ARCH_X86_64))

// ---------------------------------------------------------------------------------------------- //

#define vd_get_marked_ref(_p)          ((void *)(((vd::u64)(_p)) | 1UL))
#define vd_get_unmarked_ref(_p)        ((void *)(((vd::u64)(_p)) & ~1UL))
#define vd_is_marked_ref(_p)           (((vd::u64)(_p)) & 1UL)

#define vd_bit_count(x)                __builtin_popcount (x)
#define vd_bit_count64(x)              __builtin_popcountll (x)

// ---------------------------------------------------------------------------------------------- //

inline
int vd_first_lsb_bit_index(vd::u32 x)
{
    if(0 == x) return -1;
    return __builtin_ffs(x) - 1;
}

inline
int vd_first_lsb_bit_index64(vd::u64 x)
{
    if(0 == x) return -1;

    return __builtin_ffsll(x) - 1;
}

inline
int vd_first_msb_bit_index(vd::u32 x)
{
    if(0 == x) return -1;

    return  __builtin_clz(x) - 1;
}

inline
int vd_first_msb_bit_index64(vd::u64 x)
{
    if(0 == x) return -1;

    return  __builtin_clzll(x) - 1;
}

#elif defined(VD_64BIT)

inline
vd::u32 vd_bit_count(const vd::u32 x)
{
    vd::u32 i = x;
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

inline
vd::u32 vd_bit_count64(const vd::u64 x)
{
    return vd_bit_count((_u32)x) + vd_bit_count((_u32)(x >> 32));
}

inline
int vd_fls(vd::u32 x)
{
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return vd_bit_count(x);
}

inline
int vd_first_lsb_bit_index(vd::u32 x)
{
    if(0 == x) return -1;

    return vd_ffs(x) - 1;
}

inline
int vd_first_msb_bit_index(vd::u32 x)
{
    if(0 == x)
        return -1;

    return fls(x) - 1;
}

inline
int vd_first_lsb_bit_index64(vd::u64 x)
{
    if(0 == x)
        return -1;
    else if(0 == (x & 0xFFFFFFFFULL))
    {
        x >>= 32;
        register unsigned long i;
        i = ffs((vd::u32)x);
        return (int)i + 31;
    }
    else
    {
        register unsigned long i;
        i = ffs((vd: u32)x);
        return (int)i - 1;
    }
}

inline
int vd_first_msb_bit_index64(vd::u64 x)
{
    if(0 == x)
        return -1;
    else if(0 == (x & 0xFFFFFFFFULL))
    {
        x >>= 32;
        register unsigned long i;
        i = vd_fls((vd::u32)x);
        return (int)i + 31;
    } e
    else
    {
        register unsigned long i;
        i = vd_fls((vd::u32)x);
        return (int)i - 1;
    }

}

// ---------------------------------------------------------------------------------------------- //

#else
#error "Unknown CPU features for bitwise ops!  Please update platform detection for this architecture!"
#endif

// ============================================================================================== //

VD_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_FRAMEWORK_BUILTINS_INCLUDED

// ============================================================================================== //
// END FILE



