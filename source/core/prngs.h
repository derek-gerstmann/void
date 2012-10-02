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

#ifndef VD_CORE_PRNGS_INCLUDED
#define VD_CORE_PRNGS_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/memory.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

namespace Random {

// ============================================================================================== //

struct Hash 
{
    static inline vd::u32 
    Encode(vd::u32 seed)
    {
       vd::u32 i = (seed^0xA3C59AC3u)*2654435769u;
       i ^= (i>>16);
       i *= 2654435769u;
       i ^= (i>>16);
       i *= 2654435769u;
       return i;
    }

    static inline vd::u32 
    Decode(
        vd::u32 h)
    {
       h *= 340573321u;
       h ^= (h>>16);
       h *= 340573321u;
       h ^= (h>>16);
       h *= 340573321u;
       h ^= 0xA3C59AC3u;
       return h;
    }

    static inline vd::u32 NextU32(
        vd::u32 seed)
    { 
        return Encode(seed); 
    }

    static inline float NextF32(
        vd::u32 seed)
    { 
        return NextU32(seed)/(vd::f32)VD_U32_MAX; 
    }

    static inline vd::f64 NextF64(
        vd::u32 seed)
    { 
        return NextU32(seed)/(vd::f64)VD_U32_MAX; 
    }

    static inline vd::f64 RandHashF64(
        vd::u32 seed, vd::f64 a, vd::f64 b)
    { 
        return (b-a)*NextU32(seed)/(vd::f64)VD_U32_MAX + a; 
    }

    static inline vd::f32 RandHashF32(
        vd::u32 seed, vd::f32 a, vd::f32 b)
    { 
        return ( (b-a)*NextU32(seed)/(vd::f32)VD_U32_MAX + a); }
    }
};

// ============================================================================================== //

#define VD_RANDOM_MASKU64               0xffffffffUL
#define VD_RANDOM_VD_RANDOM_MASKF32     0x00ffffffUL

// A maximally equidistributed combined Tausworthe 64-bit random number generator 
// based on the implementation in the GSL (GNU Scientific Library).  
//
// The period of this generator is about 2^88.
//
template<int BufferSize = 2048>
class Tausworthe64 
{
public:
    static const vd::f32 InverseU32 = (1.f / (VD_RANDOM_VD_RANDOM_MASKF32 + 1UL));

    Tausworthe64(const vd::u64 seed)
    {
        m_Buffer = VD_NEW_ARRAY(vd::u64, BufferSize);
        m_Index = BufferSize;
        Populate(seed);
    }

    ~Tausworthe64() 
    {
        VD_DELETE_ARRAY(buf);
    }

    vd::u64 NextU64() 
    {
        if (m_Index == BufferSize) 
        {
            for (int i = 0; i < BufferSize; ++i)
                m_Buffer[i] = Generate();
            m_Index = 0;
        }

        return m_Buffer[m_Index++];
    }

    vd::f32 NextF32() 
    {
        return (Generate() & VD_RANDOM_VD_RANDOM_MASKU64F32) * InverseU32;
    }

private:

    vd::u64 LCG(const vd::u64 n) 
    {
        return 69069UL * n; 
        // The result is clamped to 32 bits (long)
    }

    void Populate(vd::u64 s) 
    {
        if (!s)
            s = 1UL; // default seed is 1

        z1 = LCG(s);
        if (z1 < 2UL)
            z1 += 2UL;
        z2 = LCG(z1);
        if (z2 < 8UL)
            z2 += 8UL;
        z3 = LCG(z2);
        if (z3 < 16UL)
            z3 += 16UL;
        z4 = LCG(z3);
        if (z4 < 128UL)
            z4 += 128UL;

        // Calling RNG ten times to satify recurrence condition
        for (int i = 0; i < 10; ++i)
            Generate();
    }

    vd::u64 Generate() 
    {
        const vd::u64 b1 = ((((z1 << 6UL) & VD_RANDOM_MASKU64) ^ z1) >> 13UL);
        z1 = ((((z1 & 4294967294UL) << 18UL) & VD_RANDOM_MASKU64) ^ b1);

        const vd::u64 b2 = ((((z2 << 2UL) & VD_RANDOM_MASKU64) ^ z2) >> 27UL);
        z2 = ((((z2 & 4294967288UL) << 2UL) & VD_RANDOM_MASKU64) ^ b2);

        const vd::u64 b3 = ((((z3 << 13UL) & VD_RANDOM_MASKU64) ^ z3) >> 21UL);
        z3 = ((((z3 & 4294967280UL) << 7UL) & VD_RANDOM_MASKU64) ^ b3);

        const vd::u64 b4 = ((((z4 << 3UL) & VD_RANDOM_MASKU64) ^ z4) >> 12UL);
        z4 = ((((z4 & 4294967168UL) << 13UL) & VD_RANDOM_MASKU64) ^ b4);

        return (z1 ^ z2 ^ z3 ^ z4);
    }

    vd::u64 z1;
    vd::u64 z2;
    vd::u64 z3;
    vd::u64 z4;
    vd::u64* m_Buffer;
    vd::u32 m_Index;
};

// A maximally equidistributed combined Tausworthe 32-bit random number generator 
// based on the implementation in the GSL (GNU Scientific Library).  
//
// The period of this generator is about 2^88.
//
class Tausworthe32 
{
public:
    static const vd::f32 InverseU32 = (1.f / (VD_RANDOM_VD_RANDOM_MASKF32 + 1UL));

    Tausworthe32(const vd::u32 seed) 
    {
        s1 = ValidateSeed(LCG(seed), 1);
        s2 = ValidateSeed(LCG(s1), 7);
        s3 = ValidateSeed(LCG(s2), 15);
    }
    ~Tausworthe32() { }

    vd::u64 NextU32() 
    {
        s1 = Generate(s1, 13, 19, 4294967294UL, 12);
        s2 = Generate(s2, 2, 25, 4294967288UL, 4);
        s3 = Generate(s3, 3, 11, 4294967280UL, 17);
        return (s1 ^ s2 ^ s3);
    }

    vd::f32 NextF32() 
    {
        return (NextU32() & VD_RANDOM_VD_RANDOM_MASKU64F32) * invUI;
    }

private:
    vd::u32 LCG(const vd::u32 x) const { return x * 69069; }

    vd::u32 ValidateSeed(const vd::u32 x, const vd::u32 m) const 
    {
        return (x < m) ? (x + m) : x;
    }

    vd::u32 Generate(
        const vd::u32 s, const vd::u32 a,
        const vd::u32 b, const vd::u32 c,
        const vd::u32 d) const 
    {
        return ((s&c)<<d) ^ (((s << a) ^ s) >> b);
    }

    vd::u32 s1, s2, s3;
};

template <typename T> void Shuffle(
    T *samp, size_t count, size_t dims, RandomGenerator &rng) 
{
    for (size_t i = 0; i < count; ++i) {
        size_t other = i + (rng.NextU32() % (count - i));
        for (size_t j = 0; j < dims; ++j)
            Swap(samp[dims * i + j], samp[dims * other + j]);
    }
}

// ============================================================================================== //

} // END NAMESPACE: Random

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif  // VD_CORE_PRNGS_INCLUDED
