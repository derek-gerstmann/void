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

#ifndef VD_FRAMEWORK_TYPES_INCLUDED
#define VD_FRAMEWORK_TYPES_INCLUDED

// ============================================================================================== //

#include "vd.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

// ============================================================================================== //

VD_NAMESPACE_BEGIN();

#if defined(VD_USE_HALF)
#include <half.h>
#endif

// ============================================================================================== //

#if (defined(VD_TARGET_WINDOWS) && defined(_MSC_VER))

// ============================================================================================== //

typedef signed   __int8    	                 flag;
typedef signed   __int8    	                 i8;
typedef unsigned __int8    	                 u8;
typedef signed   __int16   	                 i16;
typedef unsigned __int16   	                 u16;
typedef signed   __int32   	                 i32;
typedef unsigned __int32                     u32;
typedef signed   __int64                     i64;
typedef unsigned __int64                     u64;
#if defined(VD_USE_HALF)
typedef half                                 f16;
#else
typedef unsigned __int16                     f16;
#endif
typedef float                                f32;
typedef double                               f64;
typedef wchar_t                              wchar;
typedef size_t                               bytesize;
typedef u32 				                 tag;
typedef intptr_t                             address;
typedef void*                                ptr;

// ============================================================================================== //

#else // Unix 

// ============================================================================================== //

#include <stdint.h>

#if defined(VD_USE_ALIGNED_TYPES)
typedef int8_t          	                 flag    __attribute__((aligned(1)));
typedef int8_t          	                 i8	  	 __attribute__((aligned(1)));
typedef uint8_t         	                 u8		 __attribute__((aligned(1)));
typedef int16_t         	                 i16     __attribute__((aligned(2)));
typedef uint16_t        	                 u16     __attribute__((aligned(2)));
typedef int32_t         	                 i32     __attribute__((aligned(4)));
typedef uint32_t        	                 u32     __attribute__((aligned(4)));
typedef int64_t         	                 i64     __attribute__((aligned(8)));
typedef uint64_t        	                 u64     __attribute__((aligned(8)));
#if defined(VD_USE_HALF)
typedef half                                 f16     __attribute__((aligned(2)));
#else
typedef u16                                  f16     __attribute__((aligned(2)));
#endif
typedef float           	                 f32     __attribute__((aligned(4)));
typedef double          	                 f64	 __attribute__((aligned(8)));
typedef wchar_t                              wchar_t __attribute__((aligned(sizeof(wchar_t))));
typedef size_t                               bytesize;
typedef u32 				                 tag;
#else
typedef int8_t                               flag;
typedef int8_t                               i8;
typedef uint8_t                              u8;
typedef int16_t                              i16;
typedef uint16_t                             u16;
typedef int32_t                              i32;
typedef uint32_t                             u32;
typedef int64_t                              i64;
typedef uint64_t                             u64;
#if defined(VD_USE_HALF)
typedef half                                 f16;
#else
typedef uint16_t                             f16;
#endif
typedef float                                f32;
typedef double                               f64;
typedef wchar_t                              wchar;
typedef size_t                               bytesize;
typedef u32                                  tag;
typedef intptr_t                             address;
typedef void*                                ptr;
#endif
#endif

// ============================================================================================== //

#if     defined(VD_USE_DOUBLE_PRECISION)
typedef f64                                  real;
#ifdef VD_USE_SINGLE_PRECISION
#undef VD_USE_SINGLE_PRECISION            
#endif
#elif   defined(VD_USE_SINGLE_PRECISION)
typedef f32                                  real;
#else
#define VD_USE_SINGLE_PRECISION              1
typedef f32                                  real;
#warning "No numeric precision flag defined!  Using single precision floating-point for numeric values!"
#endif

// ============================================================================================== //


template<bool> 
class CompileTimeCheck
{
public:
    CompileTimeCheck(...) {}
};

template<> 
class CompileTimeCheck<false>
{

};

// ============================================================================================== //

#define vdStaticAssert(test, msg)       	                    \
    do {                                                        \
        struct VD_ERROR_##msg {};              		            \
        typedef CompileTimeCheck< (test) != 0 > check;          \
        check c = check(VD_ERROR_##msg());              		\
        size_t s = sizeof(c);                                 	\
        (void)(s);                            					\
    } while (0)

// ============================================================================================== //

template<typename T>
T AsType(void* v){ return reinterpret_cast<T*>(v); }

#define VD_TYPE_CHECK(T, S)                                    \
  while (false) {                                              \
    *(static_cast<T* volatile*>(0)) = static_cast<S*>(0);      \
  }

// ============================================================================================== //

typedef wchar*                		  	       cwstr;
typedef wchar                 		  	       fwstr[VD_FSTR_LENGTH];
typedef std::wstring              		       vwstr;

typedef char*                   		  	   cstr;
typedef char                    		  	   fstr[VD_FSTR_LENGTH];
typedef std::string               		       vstr;

typedef std::string 		          	       string;
typedef std::ostringstream        		       stringstream;

// ============================================================================================== //

struct Status
{
    VD_DECLARE_ENUM(Code,
        Success,
        FatalError,
        UncaughtException,
        UnknownError,
        RuntimeError,
        ExecError,
        StartupError,
        ShutdownError,
        Cancel,
        Abort,
        Queued,
        Starting,
        Running,
        Exiting,
        Defer,
        Reject,
        Accept,
        Bypass,
        OutOfMemory,
        InvalidRequest,
        InvalidContext,
        InvalidValue,
        InvalidEnum,
        InvalidFile,
        InvalidFileName,
        InvalidDirectoryName,
        InvalidResource,
        InvalidSlot,
        CreateError,
        DestroyError,
        OpenError,
        CloseError,
        LoadError,
        ReadError,
        WriteError,
        SaveError,
        UserStatus);
};

// ============================================================================================== //

typedef Status::Code status;

// ============================================================================================== //

class uid
{
public:

    uid(const u64 low = 0) 
    {
        m_Data[1] = 0;
        m_Data[0] = low;
    }

    explicit uid(const u64 high, const u64 low)
    {
        m_Data[1] = high;
        m_Data[0] = low;
    }

    explicit uid(const char* str)
    {
        *this = vd::string(str);
    }

    uid& operator = (const uid& rhs)
    {
        m_Data[1] = rhs.m_Data[1];
        m_Data[0] = rhs.m_Data[0];
        return *this;
    }

    uid& operator = (const u64 rhs)
    {
        m_Data[1] = 0;
        m_Data[0] = rhs;
        return *this;
    }

    uid& operator = (const vd::string& from);

    operator bool() const
    {
        return m_Data[0] != 0;        
    }
    
    bool operator == (const uid& rhs) const
    { 
        return m_Data[1] == rhs.m_Data[1] && m_Data[0] == rhs.m_Data[0]; 
    }

    bool operator != (const uid& rhs) const
    { 
        return m_Data[1] != rhs.m_Data[1] || m_Data[0] != rhs.m_Data[0]; 
    }

    bool operator < (const uid& rhs) const
    {
        if(m_Data[1] < rhs.m_Data[1])
            return true;

        if(m_Data[1] > rhs.m_Data[1])
            return false;

        return m_Data[0] < rhs.m_Data[0];
    }

    bool operator > (const uid& rhs) const
    {
        if(m_Data[1] > rhs.m_Data[1])
            return true;

        if(m_Data[1] < rhs.m_Data[1])
            return false;

        return m_Data[0] > rhs.m_Data[0];
    }

    bool operator <= (const uid& rhs) const
    {
        if(m_Data[1] < rhs.m_Data[1])
            return true;

        if(m_Data[1] > rhs.m_Data[1])
            return false;

        return m_Data[0] <= rhs.m_Data[0];
    }

    bool operator >= (const uid& rhs) const
    {
        if(m_Data[1] > rhs.m_Data[1])
            return true;

        if(m_Data[1] < rhs.m_Data[1])
            return false;

        return m_Data[0] >= rhs.m_Data[0];
    }

    uid& operator ++()
    {
        ++m_Data[0];

        if(!m_Data[0])
            ++m_Data[1];

        return *this;
    }

    uid& operator --()
    {
        if(!m_Data[0])
            --m_Data[1];

        --m_Data[0];
        return *this;
    }

    const u64& GetLower() const { return m_Data[0]; }
    const u64& GetUpper() const { return m_Data[1]; }

    u64& GetLower() { return m_Data[0]; }
    u64& GetUpper() { return m_Data[1]; }

private:
    u64 m_Data[2];
};

inline std::ostream& operator << (std::ostream& os, const uid& id)
{
    if(id.GetUpper() == 0)
        os << std::hex << id.GetLower() << std::dec;
    else
        os << std::hex << id.GetUpper() << ':' << id.GetLower() << std::dec;

    return os;
}

inline uid operator+ (const uid& a, const u64& b)
{
    uid result = a;
    result.GetLower() += b;

    if(result.GetLower() < a.GetLower())
        ++result.GetUpper();

    return result;
};

inline uid operator- (const uid& a, const u64& b)
{
    uid result = a;
    result.GetLower() -= b;

    if(result.GetLower() > a.GetLower())
        --result.GetUpper();

    return result;
};

inline uid operator & (const uid& a, const uid& b)
{
    uid result = a;
    result.GetLower() &= b.GetLower();
    result.GetUpper() &= b.GetUpper();
    return result;
}

inline uid operator | (const uid& a, const uid& b)
{
    uid result = a;
    result.GetLower() |= b.GetLower();
    result.GetUpper() |= b.GetUpper();
    return result;
}

struct UidEqualTo : std::binary_function<vd::uid, vd::uid, bool>
{
    bool operator()(vd::uid const& x, vd::uid const& y) const
    {
        return x == y;
    }
};

struct UidHash : std::unary_function<vd::uid, std::size_t>
{
    std::size_t operator()(vd::uid const& x) const
    {
        return x.GetLower();
    }
};

// ============================================================================================== //

#define vdSetTag(a, b, c, d)                   (((a) << 24) | ((b) << 16) | ((c) << 8) | (d))

// ============================================================================================== //

VD_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_FRAMEWORK_TYPES_INCLUDED

