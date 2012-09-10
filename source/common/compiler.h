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

#ifndef VD_FRAMEWORK_COMPILER_INCLUDED
#define VD_FRAMEWORK_COMPILER_INCLUDED

// ============================================================================================== //

// See 'http://sourceforge.net/apps/mediawiki/predef/index.php' for a comprehensive list of
// known predefined macros for standard compilers

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

// ============================================================================================== //

#endif // VD_FRAMEWORK_COMPILER_INCLUDED

// ============================================================================================== //
// END FILE




