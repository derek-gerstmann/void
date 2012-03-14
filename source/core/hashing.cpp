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

#include "core/hashing.h"

// ============================================================================================== //

// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

// Note - The x86 and x64 versions do _not_ produce the same results, as the
// algorithms are optimized for their respective platforms. You can still
// compile and run any of them on any platform, but your performance with the
// non-native version will be less than optimal.

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //
//
// Platform-specific functions and macros
//
// ============================================================================================== //

#if defined(VD_TARGET_WINDOWS)

#define VD_ROTL32(x,y)	_rotl(x,y)
#define VD_ROTL64(x,y)	_rotl64(x,y)
#define VD_BIG_CONSTANT(x) (x)

#else

inline vd::u32 rotl32(vd::u32 x, int8_t r)
{
    return (x << r) | (x >> (32 - r));
}

inline vd::u64 rotl64(vd::u64 x, int8_t r)
{
    return (x << r) | (x >> (64 - r));
}

#define	VD_ROTL32(x,y)	rotl32(x,y)
#define VD_ROTL64(x,y)	rotl64(x,y)
#define VD_BIG_CONSTANT(x) (x##LLU)

#endif

// ============================================================================================== //
// Block read - if your platform needs to do endian-swapping or can only
// handle aligned reads, do the conversion here
// ============================================================================================== //

VD_FORCE_INLINE
vd::u32 getblock(const vd::u32* p, int i)
{
    return p[i];
}

VD_FORCE_INLINE
vd::u64 getblock(const vd::u64* p, int i)
{
    return p[i];
}

// ============================================================================================== //
// Finalization mix - force all bits of a hash block to avalanche
// ============================================================================================== //

VD_FORCE_INLINE
vd::u32 fmix(vd::u32 h)
{
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
}

// ============================================================================================== //

VD_FORCE_INLINE
vd::u64 fmix(vd::u64 k)
{
    k ^= k >> 33;
    k *= VD_BIG_CONSTANT(0xff51afd7ed558ccd);
    k ^= k >> 33;
    k *= VD_BIG_CONSTANT(0xc4ceb9fe1a85ec53);
    k ^= k >> 33;
    return k;
}

// ============================================================================================== //

void 
Hashing::MurmurHash3_x86_32(
    const char* key, int len,
    vd::u32 seed, vd::uid* out)
{
    const uint8_t* data = (const uint8_t*)key;
    const int nblocks = len / 4;

    vd::u32 h1 = seed;
    vd::u32 c1 = 0xcc9e2d51;
    vd::u32 c2 = 0x1b873593;

    // body
    const vd::u32* blocks = (const vd::u32*)(data + nblocks * 4);
    for(int i = -nblocks; i; i++)
    {
        vd::u32 k1 = getblock(blocks, i);

        k1 *= c1;
        k1 = VD_ROTL32(k1, 15);
        k1 *= c2;

        h1 ^= k1;
        h1 = VD_ROTL32(h1, 13);
        h1 = h1 * 5 + 0xe6546b64;
    }

    // tail
    const uint8_t* tail = (const uint8_t*)(data + nblocks * 4);
    vd::u32 k1 = 0;
    switch(len & 3)
    {
        case 3: k1 ^= tail[2] << 16;
        case 2: k1 ^= tail[1] << 8;
        case 1: k1 ^= tail[0];
            k1 *= c1; k1 = VD_ROTL32(k1, 15); k1 *= c2; h1 ^= k1;
    };

    // finalization
    h1 ^= len;
    h1 = fmix(h1);

    *(vd::u32*)out = h1;
}

// ============================================================================================== //

void 
Hashing::MurmurHash3_x86_128(
    const char* key, const int len,
    vd::u32 seed, vd::uid* out)
{
    const uint8_t* data = (const uint8_t*)key;
    const int nblocks = len / 16;

    vd::u32 h1 = seed;
    vd::u32 h2 = seed;
    vd::u32 h3 = seed;
    vd::u32 h4 = seed;

    vd::u32 c1 = 0x239b961b;
    vd::u32 c2 = 0xab0e9789;
    vd::u32 c3 = 0x38b34ae5;
    vd::u32 c4 = 0xa1e38b93;

    // body
    const vd::u32* blocks = (const vd::u32*)(data + nblocks * 16);
    for(int i = -nblocks; i; i++)
    {
        vd::u32 k1 = getblock(blocks, i * 4 + 0);
        vd::u32 k2 = getblock(blocks, i * 4 + 1);
        vd::u32 k3 = getblock(blocks, i * 4 + 2);
        vd::u32 k4 = getblock(blocks, i * 4 + 3);

        k1 *= c1; 
        k1  = VD_ROTL32(k1, 15); 
        k1 *= c2; h1 ^= k1;

        h1 = VD_ROTL32(h1, 19); 
        h1 += h2; 
        h1 = h1 * 5 + 0x561ccd1b;

        k2 *= c2; 
        k2  = VD_ROTL32(k2, 16); 
        k2 *= c3; h2 ^= k2;

        h2 = VD_ROTL32(h2, 17); 
        h2 += h3; 
        h2 = h2 * 5 + 0x0bcaa747;

        k3 *= c3; 
        k3  = VD_ROTL32(k3, 17); 
        k3 *= c4; h3 ^= k3;

        h3 = VD_ROTL32(h3, 15); 
        h3 += h4; 
        h3 = h3 * 5 + 0x96cd1c35;

        k4 *= c4; 
        k4  = VD_ROTL32(k4, 18); 
        k4 *= c1; 
        
        h4 ^= k4;
        h4 = VD_ROTL32(h4, 13); 
        h4 += h1; 
        h4 = h4 * 5 + 0x32ac3b17;
    }

    // tail
    const uint8_t* tail = (const uint8_t*)(data + nblocks * 16);
    vd::u32 k1 = 0;
    vd::u32 k2 = 0;
    vd::u32 k3 = 0;
    vd::u32 k4 = 0;
    switch(len & 15)
    {
        case 15: k4 ^= tail[14] << 16;
        case 14: k4 ^= tail[13] << 8;
        case 13: k4 ^= tail[12] << 0;
            k4 *= c4; k4  = VD_ROTL32(k4, 18); k4 *= c1; h4 ^= k4;

        case 12: k3 ^= tail[11] << 24;
        case 11: k3 ^= tail[10] << 16;
        case 10: k3 ^= tail[ 9] << 8;
        case  9: k3 ^= tail[ 8] << 0;
            k3 *= c3; k3  = VD_ROTL32(k3, 17); k3 *= c4; h3 ^= k3;

        case  8: k2 ^= tail[ 7] << 24;
        case  7: k2 ^= tail[ 6] << 16;
        case  6: k2 ^= tail[ 5] << 8;
        case  5: k2 ^= tail[ 4] << 0;
            k2 *= c2; k2  = VD_ROTL32(k2, 16); k2 *= c3; h2 ^= k2;

        case  4: k1 ^= tail[ 3] << 24;
        case  3: k1 ^= tail[ 2] << 16;
        case  2: k1 ^= tail[ 1] << 8;
        case  1: k1 ^= tail[ 0] << 0;
            k1 *= c1; k1  = VD_ROTL32(k1, 15); k1 *= c2; h1 ^= k1;
    };

    // finalization
    h1 ^= len; h2 ^= len; h3 ^= len; h4 ^= len;

    h1 += h2; h1 += h3; h1 += h4;
    h2 += h1; h3 += h1; h4 += h1;

    h1 = fmix(h1);
    h2 = fmix(h2);
    h3 = fmix(h3);
    h4 = fmix(h4);

    h1 += h2; h1 += h3; h1 += h4;
    h2 += h1; h3 += h1; h4 += h1;

    ((vd::u32*)out)[0] = h1;
    ((vd::u32*)out)[1] = h2;
    ((vd::u32*)out)[2] = h3;
    ((vd::u32*)out)[3] = h4;
}

// ============================================================================================== //

void 
Hashing::MurmurHash3_x64_128(
    const char* key, const int len,
    const vd::u32 seed, vd::uid* out)
{
    const uint8_t* data = (const uint8_t*)key;
    const int nblocks = len / 16;

    vd::u64 h1 = seed;
    vd::u64 h2 = seed;

    vd::u64 c1 = VD_BIG_CONSTANT(0x87c37b91114253d5);
    vd::u64 c2 = VD_BIG_CONSTANT(0x4cf5ad432745937f);

    // body
    const vd::u64* blocks = (const vd::u64*)(data);
    for(int i = 0; i < nblocks; i++)
    {
        vd::u64 k1 = getblock(blocks, i * 2 + 0);
        vd::u64 k2 = getblock(blocks, i * 2 + 1);

        k1 *= c1;
        k1  = VD_ROTL64(k1, 31); 
        k1 *= c2; 
        
        h1 ^= k1;
        h1 = VD_ROTL64(h1, 27); 
        h1 += h2; 
        h1 = h1 * 5 + 0x52dce729;

        k2 *= c2; 
        k2  = VD_ROTL64(k2, 33); 
        k2 *= c1; 
        
        h2 ^= k2;
        h2 = VD_ROTL64(h2, 31); 
        h2 += h1; 
        h2 = h2 * 5 + 0x38495ab5;
    }

    // tail
    const uint8_t* tail = (const uint8_t*)(data + nblocks * 16);
    vd::u64 k1 = 0;
    vd::u64 k2 = 0;
    switch(len & 15)
    {
        case 15: k2 ^= vd::u64(tail[14]) << 48;
        case 14: k2 ^= vd::u64(tail[13]) << 40;
        case 13: k2 ^= vd::u64(tail[12]) << 32;
        case 12: k2 ^= vd::u64(tail[11]) << 24;
        case 11: k2 ^= vd::u64(tail[10]) << 16;
        case 10: k2 ^= vd::u64(tail[ 9]) << 8;
        case  9: k2 ^= vd::u64(tail[ 8]) << 0;
            k2 *= c2; k2  = VD_ROTL64(k2, 33); k2 *= c1; h2 ^= k2;

        case  8: k1 ^= vd::u64(tail[ 7]) << 56;
        case  7: k1 ^= vd::u64(tail[ 6]) << 48;
        case  6: k1 ^= vd::u64(tail[ 5]) << 40;
        case  5: k1 ^= vd::u64(tail[ 4]) << 32;
        case  4: k1 ^= vd::u64(tail[ 3]) << 24;
        case  3: k1 ^= vd::u64(tail[ 2]) << 16;
        case  2: k1 ^= vd::u64(tail[ 1]) << 8;
        case  1: k1 ^= vd::u64(tail[ 0]) << 0;
            k1 *= c1; k1  = VD_ROTL64(k1, 31); k1 *= c2; h1 ^= k1;
    };

    // finalization
    h1 ^= len; 
    h2 ^= len;
    
    h1 += h2;
    h2 += h1;

    h1 = fmix(h1);
    h2 = fmix(h2);

    h1 += h2;
    h2 += h1;

    ((vd::u64*)out)[0] = h1;
    ((vd::u64*)out)[1] = h2;
}

// ============================================================================================== //

vd::u32 
Hashing::Whiz(const char *str, size_t wrdlen)
{
    const vd::u32 WhizPrime = 1607UL;
 
    vd::u32 hash32 = 2166136261UL;
    const char *p = str;
 
    for(; wrdlen >= sizeof(vd::u32); wrdlen -= sizeof(vd::u32), p += sizeof(vd::u32)) {
        hash32 = (hash32 ^ *(vd::u32 *)p) * WhizPrime;
    }
    if (wrdlen & sizeof(vd::u16)) {
        hash32 = (hash32 ^ *(vd::u16*)p) * WhizPrime;
        p += sizeof(vd::u16);
    }
    if (wrdlen & 1) 
        hash32 = (hash32 ^ *p) * WhizPrime;
 
    return hash32 ^ (hash32 >> 16);
}

// ============================================================================================== //

vd::u64 
Tiny::Encode(
    vd::u64 value, vd::i32 length)
{
    vd::u32 sum = 0;
    vd::u32 delta = 0x9e3779b9;
    vd::u32 v[2] = { ((vd::u32*)value)[0], ((vd::u32*)value)[1] };
    vd::u32 k[4] = { 0xA341316C, 0xC8013EA4, 0xAD90777D, 0x7E95761E };
    for(vd::i32 i = 0; i < length; ++i)
    {
        sum += delta;
        v[0] += ((v[1] << 4) + k[0]) ^ (v[1] + sum) ^ ((v[1] >> 5) + k[1]);
        v[1] += ((v[0] << 4) + k[2]) ^ (v[0] + sum) ^ ((v[0] >> 5) + k[3]);
    }
    vd::u64 out = 0;
    ((vd::u32*)out)[0] = v[0];
    ((vd::u32*)out)[1] = v[1];
    return out;
}

vd::u64 
Tiny::Decode(
    vd::u64 value, vd::i32 length)
{
    u32 sum = 0xC6EF3720;
    u32 delta = 0x9e3779b9;
    vd::u32 v[2] = { ((vd::u32*)value)[0], ((vd::u32*)value)[1] };
    vd::u32 k[4] = { 0xA341316C, 0xC8013EA4, 0xAD90777D, 0x7E95761E };
    for(vd::i32 i = 0; i < length; ++i)
    {
        v[1] -= ((v[0] << 4) + k[2]) ^ (v[0] + sum) ^ ((v[0] >> 5) + k[3]);
        v[0] -= ((v[1] << 4) + k[0]) ^ (v[1] + sum) ^ ((v[1] >> 5) + k[1]);
        sum -= delta;
    }
    vd::u64 out = 0;
    ((vd::u32*)out)[0] = v[0];
    ((vd::u32*)out)[1] = v[1];
    return out;
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //
