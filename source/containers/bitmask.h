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

#ifndef VD_CONTAINERS_BITMASK_INCLUDED
#define VD_CONTAINERS_BITMASK_INCLUDED

// ============================================================================================== //

#include "containers/containers.h"
#include "constants/constants.h"

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_BEGIN();

// ============================================================================================== //

template <vd::u32 WordCount>
struct BitMask
{
    BitMask() { for (vd::i32 i = 0; i < WordCount; ++i) bits[i] = 0; }

    void Set(const vd::i32 i)
    {
        const vd::i32 word_index = i >> 5;
        const vd::i32 bit_index  = i & 31;
        bits[ word_index ] |= (1 << bit_index);
    }
    void Clear(const vd::i32 i)
    {
        const vd::i32 word_index = i >> 5;
        const vd::i32 bit_index  = i & 31;
        bits[ word_index ] &= ~(1 << bit_index);
    }
    bool Get(const vd::i32 i) const
    {
        const vd::i32 word_index = i >> 5;
        const vd::i32 bit_index  = i & 31;
        return bits[ word_index ] & (1 << bit_index);
    }

    vd::i32 GetWord(const vd::i32 i) const { return bits[i]; }

    vd::i32 bits[WordCount];
};

template <>
struct BitMask<1>
{
    BitMask() : bits(0) {}

    void Set(const vd::i32 i)       { bits |= (1 << i); }
    void Clear(const vd::i32 i)     { bits &= ~(1 << i); }
    bool Get(const vd::i32 i) const { return bits & (1 << i); }

    vd::i32 GetWord(const vd::i32 i) const { return bits; }
    vd::i32 GetBits(const vd::i32 offset) const { return bits >> offset; }

    vd::i32 bits;
};

template <>
struct BitMask<2>
{
    BitMask() { bits0 = bits1 = 0; }

    void Set(const vd::i32 i)
    {
        const vd::i32 word_index = i >> 5;
        const vd::i32 bit_index  = i & 31;
        if (word_index == 0) bits0 |= (1 << bit_index);
        if (word_index != 0) bits1 |= (1 << bit_index);
    }
    void Clear(const vd::i32 i)
    {
        const vd::i32 word_index = i >> 5;
        const vd::i32 bit_index  = i & 31;
        if (word_index == 0) bits0 &= ~(1 << bit_index);
        if (word_index != 0) bits1 &= ~(1 << bit_index);
    }
    bool Get(const vd::i32 i) const
    {
        const vd::i32 word_index = i >> 5;
        const vd::i32 bit_index  = i & 31;
        return (word_index ? bits1 : bits0) & (1 << bit_index);
    }

    vd::i32 GetWord(const vd::i32 i) const { return i ? bits1 : bits0; }

    vd::i32 GetBits(const vd::i32 offset) const
    {
        const vd::i32 word_index = offset >> 5;
        const vd::i32 bit_offset = offset & 31;
        return (word_index ? bits1 : bits0) >> bit_offset;
    }

    vd::i32 bits0;
    vd::i32 bits1;
};

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_END();

// ============================================================================================== //

#endif
