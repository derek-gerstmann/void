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

#ifndef VD_FRAMEWORK_SYSTEM_INCLUDED
#define VD_FRAMEWORK_SYSTEM_INCLUDED

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

// ============================================================================================== //

#ifdef VD_TARGET_WINDOWS
#undef VD_TARGET_WINDOWS
#endif

#ifdef VD_TARGET_OSX
#undef VD_TARGET_OSX
#endif

#ifdef VD_TARGET_LINUX
#undef VD_TARGET_LINUX
#endif

#ifdef VD_TARGET_UNIX
#undef VD_TARGET_UNIX
#endif

// ============================================================================================== //

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
  #define VD_TARGET_UNIX                 1
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

#endif // VD_FRAMEWORK_SYSTEM_INCLUDED

// ============================================================================================== //
// END FILE




