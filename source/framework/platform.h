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

#ifndef VD_PLATFORM_INCLUDED
#define VD_PLATFORM_INCLUDED

// ============================================================================================== //

// See 'http://sourceforge.net/apps/mediawiki/predef/index.php' for a comprehensive list of
// known predefined macros for standard compilers

// ============================================================================================== //

#define VD_PRAGMA(x)                   _Pragma (#x)
#define VD_TODO(x)                     VD_PRAGMA(message ("TODO - " #x))
#define VD_INFO(x)                     VD_PRAGMA(message ("INFO - " #x))

// ============================================================================================== //

// Stringification macros
#define VD_STRINGIFY_HELP(x)           #x
#define VD_STRINGIFY(x)                VD_STRINGIFY_HELP(x)
#define VD_STR_JOIN(x, y)              VD_STR_JOIN_WRAP(x, y)
#define VD_STR_JOIN_WRAP(x, y)         VD_STR_JOIN_HELP(x, y)
#define VD_STR_JOIN_HELP(x, y)         x##y

// ============================================================================================== //
// Hardware architecture detection 
// ============================================================================================== //

#if (defined(__i386__) || defined(_M_IX86) || defined(__I86__) || defined(__INTEL__))
  #define VD_ARCH_NAME                   "x86"
  #define VD_ARCH_X86                    1
#elif (defined(__x86_64__) || defined(__x86_64) || defined(_M_X64))
  #define VD_ARCH_NAME                   "x86_64"
  #define VD_ARCH_X86                    1
  #define VD_ARCH_X86_64                 1
#elif (defined(__amd64__) || defined(__amd64))
  #define VD_ARCH_NAME                   "AMD64"
  #define VD_ARCH_X86                    1
  #define VD_ARCH_X86_64                 1
#elif (defined(__arm__) || defined(_ARM))
  #define VD_ARCH_NAME                   "ARM"
  #define VD_ARCH_ARM                    1
#elif (defined(__ppc__) || defined(_M_PPC) || defined(__powerpc__))
  #define VD_ARCH_NAME                   "PPC"
  #define VD_ARCH_PPC                    1
#elif (defined(__mips__) || defined(_MIPS_ISA))
  #define VD_ARCH_NAME                   "MIPS"
  #define VD_ARCH_MIPS                   1
#elif (defined(__ia64__) || defined(_IA64) || defined(__ia64) || defined(_M_IA64) || defined(__itanium__))
  #define VD_ARCH_NAME                   "IA64"
  #define VD_ARCH_IA64                   1
#else
  #error "Unknown CPU architecture detected!  Please update platform detection for this architecture!"
#endif

#if (defined(__SIZEOF_POINTER__) && (__SIZEOF_POINTER__ == 4))
  #define VD_32BIT                       1
  #define VD_ARCH_BITS                   32
  #define VD_ARCH_BITS_INFO              "32-bit"
  #if defined(VD_64BIT)
       #undef VD_64BIT
  #endif
#elif (defined(__SIZEOF_POINTER__) && (__SIZEOF_POINTER__ == 8))
  #define VD_64BIT                       1
  #define VD_ARCH_BITS                   64
  #define VD_ARCH_BITS_INFO              "64-bit"
  #if defined(VD_32BIT)
       #undef VD_32BIT
  #endif
#elif (defined(_WIN64) || defined(_M_X64))
  #define VD_64BIT                       1
  #define VD_ARCH_BITS                   64
  #define VD_ARCH_BITS_INFO              "64-bit"
  #if defined(VD_32BIT)
       #undef VD_32BIT
  #endif
#elif (defined(__LP64__) || defined(_LP64))
  #define VD_64BIT                       1
  #define VD_ARCH_BITS                   64
  #define VD_ARCH_BITS_INFO              "64-bit"
  #if defined(VD_32BIT)
       #undef VD_32BIT
  #endif
#elif defined(VD_ARCH_IA64)
  #define VD_64BIT                       1
  #define VD_ARCH_BITS                   64
  #define VD_ARCH_BITS_INFO              "64-bit"
  #if defined(VD_32BIT)
       #undef VD_32BIT
  #endif
#elif defined(VD_ARCH_X86_64)
  #define VD_64BIT                       1
  #define VD_ARCH_BITS                   64
  #define VD_ARCH_BITS_INFO              "64-bit"
  #if defined(VD_32BIT)
       #undef VD_32BIT
  #endif
#elif defined(VD_ARCH_X86)
  #define VD_32BIT                       1
  #define VD_ARCH_BITS                   32
  #define VD_ARCH_BITS_INFO              "32-bit"
  #if defined(VD_64BIT)
       #undef VD_64BIT
  #endif
#else
  #error "Failed to detect size of CPU address space!  Please update platform detection for this architecture!"
#endif

#define VD_ARCH_INFO                   VD_ARCH_NAME " (" VD_ARCH_BITS_INFO ")"

// ============================================================================================== //
// Operating system detection 
// ============================================================================================== //

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)) && !defined(__CYGWIN__)
  #if !defined(__WIN32__)
  #define __WIN32__
  #endif
#endif

#if (defined(__linux__) || defined(__linux) || defined(linux))
  #if !defined(__LINUX__)
  #define __LINUX__
  #endif
  #if !defined(__UNIX__)
  #define __UNIX__
  #endif
#endif

#if (defined(__FreeBSD__) || defined(__FREEBSD__))
  #if !defined(__UNIX__)
  #define __UNIX__
  #endif
#endif

#if (defined(__APPLE__) && defined(__APPLE_CC__))
  #if !defined(__MACOSX__)
  #define __MACOSX__
  #endif
  #if !defined(__UNIX__)
  #define __UNIX__
  #endif
#endif

#if defined(__unix__) || defined (unix) || defined(__unix) || defined(_unix)
  #if !defined(__UNIX__)
  #define __UNIX__
  #endif
#endif

#if defined(__WIN32__)
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  #define VD_TARGET_WINDOWS              1
  #define VD_TARGET_OS_NAME              "Windows"
#elif defined(__MACOSX__) 
  #include <AvailabilityMacros.h>
  #include <sys/sysctl.h>
  #include <mach-o/dyld.h>
  #define VD_USE_POSIX               	   1
  #define VD_TARGET_OSX		               1
  #define VD_TARGET_OS_NAME 		         "OSX"
#elif defined(__LINUX__)
  #define VD_USE_POSIX                   1
  #define VD_TARGET_LINUX	               1
  #define VD_TARGET_OS_NAME 		         "Linux"
#elif defined(__UNIX__)
  #include <unistd.h>
  #if defined(_POSIX_VERSION)
  #define VD_USE_POSIX                   1
  #endif
  #define VD_TARGET_UNIX		             1
  #define VD_TARGET_OS_NAME 		         "Unix"
#else
  #error "Failed to detect Operating System!  Please update platform detection for this OS!"
#endif

// ============================================================================================== //
// Compiler detection 
// ============================================================================================== //

#if defined(__clang__)
  #define VD_COMPILER_NAME               "clang"
  #define VD_COMPILER_CLANG               1
#elif defined(__llvm__)
  #define VD_COMPILER_NAME               "llvm-gcc"
  #define VD_COMPILER_LLVM               1
  #define VD_COMPILER_GCC                1
#elif defined(__GNUC__)
  #define VD_COMPILER_NAME               "gcc"
  #define VD_COMPILER_GCC                1
#elif defined(_MSC_VER)
  #define VD_COMPILER_NAME               "msvc"
  #define VD_COMPILER_MSVC               1
#elif defined(__MINGW32__)
  #define VD_COMPILER_NAME               "mingw32"
  #define VD_COMPILER_MINGW              1
#elif defined(__MINGW64__)
  #define VD_COMPILER_NAME               "mingw64"
  #define VD_COMPILER_MINGW              1
#elif (defined(__INTEL_COMPILER) || defined(__ICC))
  #define VD_COMPILER_NAME               "icc"
  #define VD_COMPILER_ICC                1
#else
  #error "Unknown compiler detected!  Please update platform detection for this compiler!"
#endif

#if defined(__cplusplus)
  #if (__cplusplus >= 201103L)
  #define VD_COMPILER_LANG               "C++11"
  #elif (__cplusplus >= 199707L)
  #define VD_COMPILER_LANG               "C++98"
  #else
  #define VD_COMPILER_LANG               "C++"
  #endif
#elif defined(__embedded_cplusplus)
  #define VD_COMPILER_LANG               "EC++"
#elif defined(__STDC__)
  #if   (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L))
  #define VD_COMPILER_LANG               "C11"
  #elif (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L))
  #define VD_COMPILER_LANG               "C99"
  #elif (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199409L))
  #define VD_COMPILER_LANG               "C94"
  #elif defined(__STDC_VERSION__)
  #define VD_COMPILER_LANG               "C90"
  #else
  #define VD_COMPILER_LANG               "C89"
  #endif
#elif defined(__OBJC__)
  #define VD_COMPILER_LANG               "OBJC"
#elif defined(__ASSEMBLER__)
  #define VD_COMPILER_LANG               "ASM"
#else
  #error "Unknown language dialect detected!  Please update platform detection for this language!"
#endif

#if defined(VD_COMPILER_GCC)
  #if (__GNUC__ >= 3)
  #define VD_COMPILER_INFO               VD_COMPILER_NAME "-" VD_STRINGIFY(__GNUC__) "." VD_STRINGIFY(__GNUC_MINOR__) "." VD_STRINGIFY(__GNUC_PATCHLEVEL__)
  #else
  #define VD_COMPILER_INFO               VD_COMPILER_NAME "-" VD_STRINGIFY(__GNUC__) "." VD_STRINGIFY(__GNUC_MINOR__)
  #endif
#elif (VD_COMPILER_CLANG)
  #define VD_COMPILER_INFO               VD_COMPILER_NAME "-" VD_STRINGIFY(__clang_major__) "." VD_STRINGIFY(__clang_minor__) "." VD_STRINGIFY(__clang_patchlevel__)
#elif (VD_COMPILER_MSVC)
  #if (_MSC_VER) >= 1800
  #define VD_COMPILER_INFO               VD_COMPILER_NAME "-12.0"
  #elif (_MSC_VER) >= 1700
  #define VD_COMPILER_INFO               VD_COMPILER_NAME "-11.0"
  #elif (_MSC_VER) >= 1600
  #define VD_COMPILER_INFO               VD_COMPILER_NAME "-10.0"
  #elif (_MSC_VER) >= 1500
  #define VD_COMPILER_INFO               VD_COMPILER_NAME "-9.0"
  #elif _MSC_VER >= 1400 
  #define VD_COMPILER_INFO               VD_COMPILER_NAME "-8.0"
  #elif _MSC_VER >= 1310
  #define VD_COMPILER_INFO               VD_COMPILER_NAME "-7.1"
  #elif _MSC_VER >= 1300
  #define VD_COMPILER_INFO               VD_COMPILER_NAME "-7.0"
  #elif _MSC_VER >= 1200
  #define VD_COMPILER_INFO               VD_COMPILER_NAME "-6.0"
  #pragma message ("Compiling with Visual C++ 6.0 is not recommended.")
  #else
  #define VD_COMPILER_INFO               VD_COMPILER_NAME "-5.0"
  #pragma message ("Compiling with Visual C++ < 6.0 is not recommended.")
  #endif
#else
  #define VD_COMPILER_INFO               VD_COMPILER_NAME
#endif

#define VD_TARGET_INFO                   VD_COMPILER_NAME " (" VD_COMPILER_LANG ") for " VD_TARGET_OS_NAME " " VD_ARCH_INFO
#define VD_BUILD_INFO                    VD_TARGET_INFO " built on " __DATE__ " at " __TIME__

// ============================================================================================== //

// Branch hint 
#if !defined(VD_COMPILER_GCC)
#define __builtin_expect(expr,b)         (expr)
#endif

#define VD_LIKELY(x)                     __builtin_expect (((x) ? 1 : 0), 1)
#define VD_UNLIKELY(x)                   __builtin_expect (((x) ? 1 : 0), 0)

// Various helper macros
#define VD_STATIC_ARRAY_SIZE(x)          (sizeof(x) / sizeof(x[0]))

#if defined(VD_TARGET_WINDOWS)
  #define VD_NO_SWITCH_DEFAULT           VD_ASSUME(0)
  #define VD_ASSUME(hint)                __assume(hint)
#else
  #if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5))
    #define VD_NO_SWITCH_DEFAULT         __builtin_unreachable()
  #else
    #define VD_NO_SWITCH_DEFAULT         VD_LIKELY(0)
  #endif
  #define VD_ASSUME(hint)                VD_LIKELY(hint)
#endif

// ============================================================================================== //

#if defined(VD_COMPILER_MSVC)

// ---------------------------------------------------------------------------------------------- //

#include <mbstring.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define VD_AVOID_INLINE                  __declspec(noinline)
#define VD_FORCE_INLINE          	       __forceinline
#define VD_INLINE					               inline
#define VD_RESTRICT             	       __restrict
#define VD_ALIGN_START(...)			         __declspec(align(__VA_ARGS__))
#define VD_ALIGN_END(...)
#define VD_ALIGN(...)       	           __declspec(align(__VA_ARGS__))
#define VD_ALIGNED(...)      		         __declspec(align(__VA_ARGS__))
#define VD_FUNC_ID		                   __FUNCTION__
#define VD_FILE_ID		                   __FILE__
#define VD_LINE_ID		                   __LINE__
#define VD_OPTIONAL

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
#define vd_char                          unsigned char
#define vd_txt(x)                        _T(x)

#if !defined(VD_HAVE_ICU)
#   define VD_HAVE_ICU                   0
#endif

#define vdDebugBreak()   	               __debugbreak()

// ---------------------------------------------------------------------------------------------- //

#else // VD_COMPILER_MSVC

// ---------------------------------------------------------------------------------------------- //

# include <sys/types.h>
# include <sys/stat.h>
# include <glob.h>
# include <limits.h>

#define VD_MAX_PATH                    PATH_MAX
#define vd_strchr                      ::strchr
#define vd_strrchr                     ::strrchr
#define vd_strlen                      ::strlen
#define vd_strcpy_s(a,n,b)             ::strcpy(a,b)
#define vd_strcmp                      ::strcmp
#define vd_strcasecmp                  ::strcasecmp
#define vd_char                        char
#define vd_txt(x)                      x
#define VD_AVOID_INLINE                __attribute__((noinline))
#define VD_FORCE_INLINE		             inline __attribute__((always_inline))
#define VD_INLINE					             inline
#define VD_RESTRICT		                 __restrict
#define VD_ALIGN_START(...)
#define VD_ALIGN_END(...) 			       __attribute__((aligned(__VA_ARGS__)))
#define VD_ALIGN(...)                  __attribute__((aligned(__VA_ARGS__)))
#define VD_ALIGNED(...)      		       __attribute__((aligned(__VA_ARGS__)))
#define VD_FUNC_ID		                 __PRETTY_FUNCTION__
#define VD_FILE_ID		                 __FILE__
#define VD_LINE_ID		                 __LINE__
#define VD_OPTIONAL 				           __attribute__((used))
#define vdDebugBreak()       		       asm ("int $3")

#if (__GNUC__ >= 4)
#define VD_USE_GCC_ATOMICS		         1
#endif

#if !defined(VD_HAVE_ICU)
#if defined(USTRING_H)
#   define VD_HAVE_ICU                 1
#else
#   define VD_HAVE_ICU                 0
#endif
#endif

// ============================================================================================== //

#endif // VD_COMPILER_MSVC

// ============================================================================================== //

#if !defined(VD_MAX_PATH_LENGTH)
#    define  VD_MAX_PATH_LENGTH        FILENAME_MAX
#endif

// ============================================================================================== //

#endif // VD_PLATFORM_INCLUDED

// ============================================================================================== //
// END FILE




