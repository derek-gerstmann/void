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
#include "core/asserts.h"

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

    template <typename T> VD_INLINE
    T FromString(const vd::string& str)
    {

    }

    template <> VD_INLINE
    vd::uid FromString( const vd::string& str )
    {
        char* next = 0;
        vd::u64 upper = 0;
        vd::u64 lower = 0;

#if defined(VD_COMPILER_MSVC)
        upper = ::_strtoui64( str.c_str(), &next, 16 );
#else
        upper = ::strtoull( str.c_str(), &next, 16 );
#endif
        assert( next != str.c_str() );
        if( *next == '\0' ) 
        {
            lower = upper;
            upper = 0;
        }
        else
        {
            assert( *next == ':' );
            ++next;
#if defined(VD_COMPILER_MSVC)
            lower = ::_strtoui64( next, 0, 16 );
#else
            lower = ::strtoull( next, 0, 16 );
#endif
        }
        return vd::uid(upper, lower);
    }

    template <> VD_INLINE
    vd::string ToString(const Object& obj)
    {
        return obj.ToString();
    }

    template <> VD_INLINE
    vd::string ToString(const vd::uid& uid)
    {
        vd::stringstream stream;
        if(uid.GetUpper() == 0)
            stream << std::hex << uid.GetLower() << std::dec;
        else
            stream << std::hex << uid.GetUpper() << ':' << uid.GetLower() << std::dec;

        const vd::string str = stream.str();
        return str;
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

    vd::string
    FormatString(const char* fmt, ...);
};

class Bitwise 
{

public:

    vd::i32 Value;

    Bitwise(const vd::i32 value) {
        Value = value;
    }

    Bitwise(const char* s) {
        Value = Parse(s, 10);
    }

public://int functionality
    VD_INLINE char ToChar() {
        return (char)Value;
    }

    VD_INLINE short ToShort() {
        return (short)Value;
    }

    VD_INLINE int ToInteger() {
        return Value;
    }

    VD_INLINE vd::u64 ToU64() {
        return (vd::u64)Value;
    }

    VD_INLINE vd::f32 ToF32() {
        return (vd::f32)Value;
    }

    VD_INLINE vd::f64 ToF64() 
    {
        return (vd::f64)Value;
    }

    VD_INLINE vd::string ToString() 
    {
        vd::stringstream lStream; 
        lStream << Value;
        return lStream.str();
    }

    VD_INLINE vd::uid ToKey() 
    {
        return vd::uid(Value);
    }

    VD_INLINE 
    vd::i32 CompareTo(const Bitwise& rhs) 
    {
        vd::i32 us = Value;
        vd::i32 them = rhs.Value;
        return (us < them ? -1 : (us == them ? 0 : 1));
    }

public:

    VD_INLINE static 
    vd::string ToString(int i, int radix) {
        char tmp_buf[256*1];
        ToAscii (i, tmp_buf,radix);
        return vd::string(tmp_buf);
    }

    VD_INLINE static 
    vd::string ToHexString(const int i) {
        vd::stringstream lStream; 
        lStream.flags ( std::ios::hex );
        lStream << i;
        return lStream.str();
    }

    VD_INLINE static 
    vd::string ToOctalString(const int i) {
        vd::stringstream lStream; 
        lStream.flags ( std::ios::oct );
        lStream << i;
        return lStream.str();
    }

    VD_INLINE static 
    vd::string ToBinaryString(const int i) {
        char tmp_buf[256*1];
        ToAscii (i,tmp_buf,2);
        return vd::string(tmp_buf);
    }

    VD_INLINE static 
    vd::string ToString(const int i) {
        vd::stringstream lStream;
        lStream << i;
        return lStream.str();
    }

    VD_INLINE static 
    vd::i32 Parse(const char* s, const int radix) {
        return strtol(s,NULL,radix);
    }

    VD_INLINE static 
    vd::i32 Parse(const char* s) {
        return Parse(s,10);
    }

    VD_INLINE static 
    vd::i32 GetHighestOneBit(int i) 
    {
        i |= (i >>  1);
        i |= (i >>  2);
        i |= (i >>  4);
        i |= (i >>  8);
        i |= (i >> 16);
        return i - (i >> 1);
    }

    VD_INLINE static 
    vd::i32 GetLowestOneBit(const int i) 
    {
        return i & -i;
    }

    VD_INLINE static 
    vd::i32 GetNumberOfLeadingZeros(int i) 
    {
        if (i == 0)
            return 32;
        int n = 1;
        if (i >> 16 == 0) { n += 16; i <<= 16; }
        if (i >> 24 == 0) { n +=  8; i <<=  8; }
        if (i >> 28 == 0) { n +=  4; i <<=  4; }
        if (i >> 30 == 0) { n +=  2; i <<=  2; }
        n -= i >> 31;
        return n;
    }

    VD_INLINE static 
    vd::i32 GetNumberOfTrailingZeros(int i) 
    {
        int y;
        if (i == 0) return 32;
        int n = 31;
        y = i <<16; if (y != 0) { n = n -16; i = y; }
        y = i << 8; if (y != 0) { n = n - 8; i = y; }
        y = i << 4; if (y != 0) { n = n - 4; i = y; }
        y = i << 2; if (y != 0) { n = n - 2; i = y; }
        return n - ((i << 1) >> 31);
    }

    VD_INLINE static 
    vd::i32 RotateLeft(const vd::i32 i, const vd::i32 distance) 
    {
        return (i << distance) | (i >> -distance);
    }

    VD_INLINE static 
    vd::i32 RotateRight(const vd::i32 i, const vd::i32 distance) 
    {
        return (i >> distance) | (i << -distance);
    }

    VD_INLINE static 
    vd::i32 Reverse(vd::i32 i) 
    {
        i = ((i & 0x55555555) << 1) | ((i >> 1) & 0x55555555);
        i = ((i & 0x33333333) << 2) | ((i >> 2) & 0x33333333);
        i = ((i & 0x0f0f0f0f) << 4) | ((i >> 4) & 0x0f0f0f0f);
        i = (i << 24) | ((i & 0xff00) << 8) |
            ((i >> 8) & 0xff00) | (i >> 24);
        return i;
    }

    VD_INLINE static 
    vd::i32 Signum(const vd::i32 i) 
    {
        return (i >> 31) | (-i >> 31);
    }

    VD_INLINE static 
    vd::i32 ReverseBytes(const vd::i32 i) 
    {
        return ((i >> 24)           )  |
            ((i >>   8) &   0xFF00)  |
            ((i <<   8) & 0xFF0000)  |
            ((i << 24));
    }


    VD_INLINE static 
    vd::u32 GetNearestPowerOfTwo(const vd::u32 i) 
    {
        vd::u32 rc( 1 );
        while (rc < i) {
            rc <<= 1;
        }
        return rc;
    }

    VD_INLINE static 
    vd::u32 Log2(const vd::u32 i) 
    {
        vd::u32 mask = 1;
        vd::u32 count=0;
        while(mask < i) 
        {
            mask <<= 1;
            ++count;
        }
        return count;
    }

    VD_INLINE static 
    vd::u32 GetBitCount(vd::u32 i) {
        return vd_bit_count(i);
    }

    VD_INLINE static vd::u32 GetLsbBitIndex(vd::u32 x) {
        return vd_first_lsb_bit_index(x);
    }

    VD_INLINE static vd::u32 GetMsbBitIndex(vd::u32 x) {
        return vd_first_msb_bit_index(x);
    }

private:
    VD_INLINE static char* ToAscii(int value, char* str, int radix) 
    {
        static char dig[] = "0123456789abcdefghijklmnopqrstuvwxyz";
        int n = 0, neg = 0;
        unsigned int v;
        char* p, *q;
        char c;

        if (radix == 10 && value < 0) {
            value = -value;
            neg = 1;
        }
        v = value;
        do {
            str[n++] = dig[v%radix];
            v /= radix;
        } while (v);
        if (neg)
            str[n++] = '-';
        str[n] = '\0';
        for (p = str, q = p + n/2; p != q; ++p, --q)
            c = *p, *p = *q, *q = c;
        return str;
    }
};

// ============================================================================================== //


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

