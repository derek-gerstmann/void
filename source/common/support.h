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

#ifndef VD_FRAMEWORK_SUPPORT_INCLUDED
#define VD_FRAMEWORK_SUPPORT_INCLUDED

// ============================================================================================== //
// Boost hints
// ============================================================================================== //

#define BOOST_ALL_NO_LIB 				 1

// ============================================================================================== //
// Branch hint
// ============================================================================================== //

#if !defined(VD_COMPILER_GCC)
#define __builtin_expect(expr,b)         (expr)
#endif

#define VD_LIKELY(x)                     __builtin_expect (((x) ? 1 : 0), 1)
#define VD_UNLIKELY(x)                   __builtin_expect (((x) ? 1 : 0), 0)

// ============================================================================================== //
// Various helper macros
// ============================================================================================== //

#define VD_STATIC_ARRAY_SIZE(x)          (sizeof(x) / sizeof(x[0]))

#if defined(VD_TARGET_WINDOWS)
#define VD_NO_SWITCH_DEFAULT             VD_ASSUME(0)
#define VD_ASSUME(hint)                  __assume(hint)
#else
#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5))
#define VD_NO_SWITCH_DEFAULT             __builtin_unreachable()
#else
#define VD_NO_SWITCH_DEFAULT             VD_LIKELY(0)
#endif
#define VD_ASSUME(hint)                  VD_LIKELY(hint)
#endif

// ============================================================================================== //
// Compiler specific builtins
// ============================================================================================== //

#if defined(VD_COMPILER_MSVC)

// ---------------------------------------------------------------------------------------------- //

#define BOOST_FILESYSTEM_NO_LIB 
#define BOOST_SYSTEM_NO_LIB 

// ---------------------------------------------------------------------------------------------- //

#include <mbstring.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#ifdef _SECURE_SCL_THROWS
#undef _SECURE_SCL_THROWS
#endif
#ifdef _SCL_SECURE_NO_WARNINGS
#undef _SCL_SECURE_NO_WARNINGS
#endif
#ifdef _HAS_ITERATOR_DEBUGGING
#undef _HAS_ITERATOR_DEBUGGING
#endif
#undef _STLP_DEBUG

#define _SECURE_SCL_THROWS 				0
#define _HAS_ITERATOR_DEBUGGING 		0		
#define _SCL_SECURE_NO_WARNINGS 		0
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE

// ---------------------------------------------------------------------------------------------- //

#define VD_AVOID_INLINE                  __declspec(noinline)
#define VD_FORCE_INLINE          	     __forceinline
#define VD_INLINE					     inline
#define VD_RESTRICT             	     __restrict
#define VD_ALIGN_START(...)			     __declspec(align(__VA_ARGS__))
#define VD_ALIGN_END(...)
#define VD_ALIGN(...)       	         __declspec(align(__VA_ARGS__))
#define VD_ALIGNED(...)      		     __declspec(align(__VA_ARGS__))
#define VD_FUNC_ID		                 __FUNCTION__
#define VD_FILE_ID		                 __FILE__
#define VD_LINE_ID		                 __LINE__
#define VD_OPTIONAL
#define VD_SZ_FMT                        "%Iu"

#define vd_strchr                        ::_mbschr
#define vd_strrchr                       ::_mbsrchr
#define vd_strlen                        ::_mbslen
#if __STDC_WANT_SECURE_LIB__
#define vd_strcpy_s(a,n,b)               ::_mbscpy_s(a,n,b)
#else
#define vd_strcpy_s(a,n,b)               ::_mbscpy(a,b)
#endif
#define vd_strcmp                        ::_mbscmp
#define vd_strcasecmp                    ::_mbsicmp
#define vd_txt(x)                        _T(x)

#if !defined(VD_HAVE_ICU)
#   define VD_HAVE_ICU                   0
#endif

#define vdDebugBreak()   	             __debugbreak()

// ---------------------------------------------------------------------------------------------- //

#else // VD_COMPILER_MSVC

// ---------------------------------------------------------------------------------------------- //

# include <sys/types.h>
# include <sys/stat.h>
# include <glob.h>
# include <limits.h>

// ---------------------------------------------------------------------------------------------- //

#define VD_AVOID_INLINE                  __attribute__((noinline))
#define VD_FORCE_INLINE		             inline __attribute__((always_inline))
#define VD_INLINE					     inline
#define VD_RESTRICT		                 __restrict
#define VD_ALIGN_START(...)
#define VD_ALIGN_END(...) 			     __attribute__((aligned(__VA_ARGS__)))
#define VD_ALIGN(...)                    __attribute__((aligned(__VA_ARGS__)))
#define VD_ALIGNED(...)      		     __attribute__((aligned(__VA_ARGS__)))
#define VD_FUNC_ID		                 __PRETTY_FUNCTION__
#define VD_FILE_ID		                 __FILE__
#define VD_LINE_ID		                 __LINE__
#define VD_OPTIONAL 				     __attribute__((used))
#define VD_SZ_FMT                        "%zd"
#define VD_MAX_PATH                      PATH_MAX

#define vd_strchr                        ::strchr
#define vd_strrchr                       ::strrchr
#define vd_strlen                        ::strlen
#define vd_strcpy_s(a,n,b)               ::strcpy(a,b)
#define vd_strcmp                        ::strcmp
#define vd_strcasecmp                    ::strcasecmp
#define vd_txt(x)                        x

#define vdDebugBreak()       		     asm ("int $3")

// ---------------------------------------------------------------------------------------------- //

#if (__GNUC__ >= 4)
#define VD_USE_GCC_ATOMICS		         1
#endif

#if !defined(VD_HAVE_ICU)
#if defined(USTRING_H)
#   define VD_HAVE_ICU                   1
#else
#   define VD_HAVE_ICU                   0
#endif
#endif

// ============================================================================================== //

#endif // VD_COMPILER_MSVC

// ============================================================================================== //

#if !defined(VD_MAX_PATH_LENGTH)
#    define  VD_MAX_PATH_LENGTH          FILENAME_MAX
#endif

// ============================================================================================== //

#define VD_TARGET_INFO    VD_COMPILER_NAME " (" VD_COMPILER_LANG ") for " VD_TARGET_OS_NAME " " VD_ARCH_INFO
#define VD_BUILD_INFO     VD_TARGET_INFO " built on " __DATE__ " at " __TIME__

// ============================================================================================== //

#endif // VD_FRAMEWORK_SUPPORT_INCLUDED

// ============================================================================================== //
// END FILE




