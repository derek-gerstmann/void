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

#ifndef VD_ARCHITECTURE_INCLUDED
#define VD_ARCHITECTURE_INCLUDED

// ============================================================================================== //
// Hardware architecture detection
// ============================================================================================== //

#if (defined(__i386__) || defined(_M_IX86) || defined(__I86__) || defined(__INTEL__))
#define VD_ARCH_NAME                   "x86"
#define VD_ARCH_X86                    1
#define VD_ARCH_VENDOR_INTEL           1
#elif (defined(__x86_64__) || defined(__x86_64) || defined(_M_X64))
#define VD_ARCH_NAME                   "x86_64"
#define VD_ARCH_X86                    1
#define VD_ARCH_X86_64                 1
#define VD_ARCH_VENDOR_INTEL           1
#elif (defined(__amd64__) || defined(__amd64))
#define VD_ARCH_NAME                   "AMD64"
#define VD_ARCH_X86                    1
#define VD_ARCH_X86_64                 1
#define VD_ARCH_VENDOR_AMD             1
#elif (defined(__arm__) || defined(_ARM))
#define VD_ARCH_NAME                   "ARM"
#define VD_ARCH_ARM                    1
#define VD_ARCH_VENDOR_ARM             1
#elif (defined(__ppc__) || defined(_M_PPC) || defined(__powerpc__))
#define VD_ARCH_NAME                   "PPC"
#define VD_ARCH_PPC                    1
#define VD_ARCH_VENDOR_IBM             1
#elif (defined(__mips__) || defined(_MIPS_ISA))
#define VD_ARCH_NAME                   "MIPS"
#define VD_ARCH_MIPS                   1
#define VD_ARCH_VENDOR_MIPS            1
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
#define VD_ARCH_CACHE_LINE_SIZE        64
#if defined(VD_64BIT)
#undef VD_64BIT
#endif
#elif (defined(__SIZEOF_POINTER__) && (__SIZEOF_POINTER__ == 8))
#define VD_64BIT                       1
#define VD_ARCH_BITS                   64
#define VD_ARCH_BITS_INFO              "64-bit"
#define VD_ARCH_CACHE_LINE_SIZE        64
#if defined(VD_32BIT)
#undef VD_32BIT
#endif
#elif (defined(_WIN64) || defined(_M_X64))
#define VD_64BIT                       1
#define VD_ARCH_BITS                   64
#define VD_ARCH_BITS_INFO              "64-bit"
#define VD_ARCH_CACHE_LINE_SIZE        64
#if defined(VD_32BIT)
#undef VD_32BIT
#endif
#elif (defined(__LP64__) || defined(_LP64))
#define VD_64BIT                       1
#define VD_ARCH_BITS                   64
#define VD_ARCH_BITS_INFO              "64-bit"
#define VD_ARCH_CACHE_LINE_SIZE        64
#if defined(VD_32BIT)
#undef VD_32BIT
#endif
#elif defined(VD_ARCH_IA64)
#define VD_64BIT                       1
#define VD_ARCH_BITS                   64
#define VD_ARCH_BITS_INFO              "64-bit"
#define VD_ARCH_CACHE_LINE_SIZE        64
#if defined(VD_32BIT)
#undef VD_32BIT
#endif
#elif defined(VD_ARCH_X86_64)
#define VD_64BIT                       1
#define VD_ARCH_BITS                   64
#define VD_ARCH_BITS_INFO              "64-bit"
#define VD_ARCH_CACHE_LINE_SIZE        64
#if defined(VD_32BIT)
#undef VD_32BIT
#endif
#elif defined(VD_ARCH_X86)
#define VD_32BIT                       1
#define VD_ARCH_BITS                   32
#define VD_ARCH_BITS_INFO              "32-bit"
#define VD_ARCH_CACHE_LINE_SIZE        64
#if defined(VD_64BIT)
#undef VD_64BIT
#endif
#else
#error "Failed to detect size of CPU address space!  Please update platform detection for this architecture!"
#endif

#define VD_ARCH_INFO                   VD_ARCH_NAME " (" VD_ARCH_BITS_INFO ")"

// ============================================================================================== //

#endif // VD_ARCHITECTURE_INCLUDED

// ============================================================================================== //
// END FILE




