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

#include <cassert>
#include <string>
#include <vector>
#include <sstream>

// ============================================================================================== //

VD_NAMESPACE_BEGIN();

// ============================================================================================== //

#if (defined(VD_TARGET_WINDOWS) && defined(_MSC_VER))

// ============================================================================================== //

typedef signed   __int8    	flag;
typedef signed   __int8    	i8;
typedef unsigned __int8    	u8;
typedef signed   __int16   	i16;
typedef unsigned __int16   	u16;
typedef signed   __int32   	i32;
typedef unsigned __int32    u32;
typedef signed   __int64    i64;
typedef unsigned __int64    u64;
typedef unsigned __int16    f16;
typedef float               f32;
typedef double              f64;
typedef wchar_t             wchar;
typedef size_t              bytesize;
typedef u32 				tag;

#define VD_I8_BIT         	8
#define VD_I8_MAX        	127
#define VD_I8_MIN        	(-127-1)
#define VD_U8_MAX           255
#define VD_I16_MAX          32767
#define VD_I16_MIN          (-32767-1)
#define VD_U16_MAX          65535
#define VD_I32_MAX          2147483647
#define VD_I32_MIN          (-2147483647-1)
#define VD_U32_MAX          0xFFFFFFFFU
#define VD_I64_MAX          ((i64) 0x7FFFFFFFFFFFFFFFLL)
#define VD_I64_MIN          ((i64) -0x7FFFFFFFFFFFFFFFLL - 1LL)
#define VD_U64_MAX          ((u64) 0xFFFFFFFFFFFFFFFFULL)

#define VD_F32_DIG          6
#define VD_F32_MANT_DIG     24
#define VD_F32_MAX_10_EXP   +38
#define VD_F32_MAX_EXP      +128
#define VD_F32_MIN_10_EXP   -37
#define VD_F32_MIN_EXP      -125
#define VD_F32_RADIX        2
#define VD_F32_MAX          340282346638528859811704183484516925440.0f
#define VD_F32_MIN          1.175494350822287507969e-38f
#define VD_F32_EPSILON      0x1.0p-23f
#define VD_F32_E          	2.71828174591064f
#define VD_F32_LOG2E      	1.44269502162933f
#define VD_F32_LOG10E     	0.43429449200630f
#define VD_F32_LN2        	0.69314718246460f
#define VD_F32_LN10       	2.30258512496948f
#define VD_F32_PI         	3.14159274101257f
#define VD_F32_PI_2       	1.57079637050629f
#define VD_F32_PI_4       	0.78539818525314f
#define VD_F32_1_PI       	0.31830987334251f
#define VD_F32_2_PI       	0.63661974668503f
#define VD_F32_2_SQRTPI   	1.12837922573090f
#define VD_F32_SQRT2      	1.41421353816986f
#define VD_F32_SQRT1_2    	0.70710676908493f
#define VD_F32_HUGE_VAL     ((f32) 1e50)
#define VD_F32_TINY_VAL     ((f32) 1.0e-37f)

#define VD_F64_DIG          15
#define VD_F64_MANT_DIG     53
#define VD_F64_MAX_10_EXP   +308
#define VD_F64_MAX_EXP      +1024
#define VD_F64_MIN_10_EXP   -307
#define VD_F64_MIN_EXP      -1021
#define VD_F64_RADIX        2
#define VD_F64_MAX          179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0
#define VD_F64_MIN          2.225073858507201383090e-308
#define VD_F64_EPSILON      2.220446049250313080847e-16
#define VD_F64_E         	  2.718281828459045090796
#define VD_F64_LOG2E      	1.442695040888963387005
#define VD_F64_LOG10E     	0.434294481903251816668
#define VD_F64_LN2        	0.693147180559945286227
#define VD_F64_LN10       	2.302585092994045901094
#define VD_F64_PI         	3.141592653589793115998
#define VD_F64_PI_2       	1.570796326794896557999
#define VD_F64_PI_4       	0.785398163397448278999
#define VD_F64_1_PI       	0.318309886183790691216
#define VD_F64_2_PI       	0.636619772367581382433
#define VD_F64_2_SQRTPI   	1.128379167095512558561
#define VD_F64_SQRT2      	1.414213562373095145475
#define VD_F64_SQRT1_2    	0.707106781186547572737	
#define VD_F64_HUGE_VAL     ((f64) 1e500)
#define VD_F64_TINY_VAL     ((f64) 1.0e-307)

#define VD_INFINITY         VD_HUGE_VAL_F32
#define VD_NAN              (VD_INFINITY - VD_INFINITY)

// ============================================================================================== //

#else // Unix 

// ============================================================================================== //

#include <stdint.h>

/* scalar types  */
#if defined(VD_USE_ALIGNED_TYPES)
typedef int8_t          	  flag	__attribute__((aligned(1)));
typedef int8_t          	  i8		__attribute__((aligned(1)));
typedef uint8_t         	  u8		__attribute__((aligned(1)));
typedef int16_t         	  i16   __attribute__((aligned(2)));
typedef uint16_t        	  u16   __attribute__((aligned(2)));
typedef int32_t         	  i32   __attribute__((aligned(4)));
typedef uint32_t        	  u32   __attribute__((aligned(4)));
typedef int64_t         	  i64   __attribute__((aligned(8)));
typedef uint64_t        	  u64   __attribute__((aligned(8)));
typedef uint16_t        	  f16   __attribute__((aligned(2)));
typedef float           	  f32   __attribute__((aligned(4)));
typedef double          	  f64		__attribute__((aligned(8)));
typedef wchar_t           	wchar __attribute__((aligned(sizeof(wchar_t))));
typedef size_t            	bytesize;
typedef u32 				        tag;
#else
typedef int8_t              flag;
typedef int8_t              i8;
typedef uint8_t             u8;
typedef int16_t             i16;
typedef uint16_t            u16;
typedef int32_t             i32;
typedef uint32_t            u32;
typedef int64_t             i64;
typedef uint64_t            u64;
typedef uint16_t            f16;
typedef float               f32;
typedef double              f64;
typedef wchar_t             wchar;
typedef size_t              bytesize;
typedef u32                 tag;
#endif

#define VD_I8_BIT         	8
#define VD_I8_MAX        	  127
#define VD_I8_MIN        	  (-127-1)
#define VD_U8_MAX        	  255
#define VD_I16_MAX         	32767
#define VD_I16_MIN         	(-32767-1)
#define VD_U16_MAX        	65535
#define VD_I32_MAX          2147483647
#define VD_I32_MIN          (-2147483647-1)
#define VD_U32_MAX         	0xFFFFFFFFU
#define VD_I64_MAX         	((i64) 0x7FFFFFFFFFFFFFFFLL)
#define VD_I64_MIN         	((i64) -0x7FFFFFFFFFFFFFFFLL - 1LL)
#define VD_U64_MAX        	((u64) 0xFFFFFFFFFFFFFFFFULL)

#define VD_F32_DIG          6
#define VD_F32_MANT_DIG     24
#define VD_F32_MAX_10_EXP   +38
#define VD_F32_MAX_EXP      +128
#define VD_F32_MIN_10_EXP   -37
#define VD_F32_MIN_EXP      -125
#define VD_F32_RADIX        2
#define VD_F32_MAX          0x1.FFFFFep127f
#define VD_F32_MIN          0x1.0p-126f
#define VD_F32_EPSILON      0x1.0p-23f
#define VD_F32_E          	2.71828174591064f
#define VD_F32_LOG2E      	1.44269502162933f
#define VD_F32_LOG10E     	0.43429449200630f
#define VD_F32_LN2        	0.69314718246460f
#define VD_F32_LN10       	2.30258512496948f
#define VD_F32_PI         	3.14159274101257f
#define VD_F32_PI_2       	1.57079637050629f
#define VD_F32_PI_4       	0.78539818525314f
#define VD_F32_1_PI       	0.31830987334251f
#define VD_F32_2_PI       	0.63661974668503f
#define VD_F32_2_SQRTPI   	1.12837922573090f
#define VD_F32_SQRT2      	1.41421353816986f
#define VD_F32_SQRT1_2    	0.70710676908493f

#define VD_F64_DIG          15
#define VD_F64_MANT_DIG     53
#define VD_F64_MAX_10_EXP   +308
#define VD_F64_MAX_EXP      +1024
#define VD_F64_MIN_10_EXP   -307
#define VD_F64_MIN_EXP      -1021
#define VD_F64_RADIX        2
#define VD_F64_MAX          0x1.FFFFFFFFFFFFFp1023
#define VD_F64_MIN          0x1.0p-1022
#define VD_F64_EPSILON      0x1.0p-52
#define VD_F64_E         	  2.718281828459045090796
#define VD_F64_LOG2E      	1.442695040888963387005
#define VD_F64_LOG10E     	0.434294481903251816668
#define VD_F64_LN2        	0.693147180559945286227
#define VD_F64_LN10       	2.302585092994045901094
#define VD_F64_PI         	3.141592653589793115998
#define VD_F64_PI_2       	1.570796326794896557999
#define VD_F64_PI_4       	0.785398163397448278999
#define VD_F64_1_PI       	0.318309886183790691216
#define VD_F64_2_PI       	0.636619772367581382433
#define VD_F64_2_SQRTPI   	1.128379167095512558561
#define VD_F64_SQRT2      	1.414213562373095145475
#define VD_F64_SQRT1_2    	0.707106781186547572737	

// ============================================================================================== //

#if defined( __GNUC__ )
   #define VD_F32_TINY_VAL  ((f32) 1e-37)
   #define VD_F32_HUGE_VAL  __builtin_huge_valf()
   #define VD_F64_TINY_VAL  ((f64) 1e-307)
   #define VD_F64_HUGE_VAL  __builtin_huge_val()
   #define VD_NAN           __builtin_nanf( "" )
#else
   #define VD_F32_TINY_VAL  ((f32) 1e-37)
   #define VD_F32_HUGE_VAL  ((f32) 1e50)
   #define VD_F64_TINY_VAL  ((f64) 1e-307)
   #define VD_F64_HUGE_VAL  ((f64) 1e500)
   float nanf( const char * );
   #define VD_NAN           nanf( "" )  
#endif

#define VD_INFINITY         VD_F32_HUGE_VAL
#endif

// ============================================================================================== //

#define VD_TRUE				          ((i8)(1))
#define VD_FALSE			          ((i8)(0))
#define VD_FIXED_STRING_LENGTH  (256u)

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

#if !defined(VD_RELEASE_BUILD)

#define vdStaticAssert(test, msg)       	                      \
    do {                                                        \
        struct VD_ERROR_##msg {};              		              \
        typedef CompileTimeCheck< (test) != 0 > check;          \
        check c = check(VD_ERROR_##msg());              				\
        size_t s = sizeof(c);                                 	\
        (void)(s);                            									\
    } while (0)

#else

#define vdStaticAssert(test, errormsg)                          \
    do {} while (0)

#endif

// ============================================================================================== //

template<typename T>
T AsType(void* v){ return reinterpret_cast<T*>(v); }

#define VD_TYPE_CHECK(T, S)                                    \
  while (false) {                                              \
    *(static_cast<T* volatile*>(0)) = static_cast<S*>(0);      \
  }

// ============================================================================================== //

typedef wchar*                		  	  cwstr;
typedef wchar                 		  	  fwstr[VD_FIXED_STRING_LENGTH];
typedef std::wstring              		  vwstr;

typedef u8*                   		  	  cstr;
typedef u8                    		  	  fstr[VD_FIXED_STRING_LENGTH];
typedef std::string               		  vstr;

typedef std::string 		          	    string;
typedef std::ostringstream        		  stringstream;
typedef u32 						  	            status;

// ============================================================================================== //

#if 	defined(VD_USE_DOUBLE_PRECISION)
typedef f64					                    real;
#ifdef VD_USE_SINGLE_PRECISION
#undef VD_USE_SINGLE_PRECISION   		  
#endif
#elif 	defined(VD_USE_SINGLE_PRECISION)
typedef f32 				                    real;
#else
#define VD_USE_SINGLE_PRECISION   		  VD_ON
typedef f32 				                    real;
#warning "No precision flag was defined!  Using single precision as default!"
#endif

// ============================================================================================== //

#define vdSetTag(a, b, c, d)    (((a) << 24) | ((b) << 16) | ((c) << 8) | (d))

// ============================================================================================== //

VD_DECLARE_ENUM(BasicType,
    Void,
    ByteSize,
    U8,
    I8,
    U16,
    I16,
    U32,
    I32,
    U64,
    I64,
    F16,
    F32,
    F64);

// ============================================================================================== //

class VD_API TypeInfo
{
public:
    TypeInfo();
    TypeInfo(BasicType base, vd::u32 length = 1);
    bool IsIntegral() const;       
    bool IsSigned() const;         
    vd::string GetName() const;
    vd::bytesize GetTotalSize() const;
    vd::bytesize GetBasicSize() const;  
    BasicType GetBasicType() const;
    vd::u32 GetLength() const;

private:
    BasicType    ValueType;    
    vd::u32      Length;
};

// ============================================================================================== //

VD_NAMESPACE_END();

// ============================================================================================== //

#include "framework/uid.h"

// ============================================================================================== //

#endif // VD_FRAMEWORK_TYPES_INCLUDED

