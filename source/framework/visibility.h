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

#ifndef VD_VISIBILITY_INCLUDED
#define VD_VISIBILITY_INCLUDED

// ============================================================================================== //
//
// See http://gcc.gnu.org/wiki/Visibility
//
// ============================================================================================== //

#if defined(_WIN32) || defined(__CYGWIN__)
#define VD_DLL_IMPORT   __declspec(dllimport)
#define VD_DLL_EXPORT   __declspec(dllexport)
#define VD_DLL_INTERNAL
#else
#if __GNUC__ >= 4
#define VD_DLL_DO_PUSH_POP
#define VD_DLL_IMPORT   __attribute__((visibility("default")))
#define VD_DLL_EXPORT   __attribute__((visibility("default")))
#define VD_DLL_INTERNAL __attribute__((visibility("hidden")))
#else
#define VD_DLL_IMPORT
#define VD_DLL_EXPORT
#define VD_DLL_INTERNAL
#endif
#endif

// ============================================================================================== //

#if defined(VD_BUILD_SHARED_LIBRARY)
#define VD_API          VD_DLL_EXPORT
#define VD_API_C        extern "C" VD_DLL_EXPORT
#else
#define VD_API          VD_DLL_IMPORT
#define VD_API_C        extern "C" VD_DLL_IMPORT
#endif
#define VD_INTERNAL     VD_DLL_INTERNAL
#define VD_INTERNAL_C   extern "C" VD_DLL_INTERNAL

// ============================================================================================== //

#endif // VD_VISIBILITY_INCLUDED

// ============================================================================================== //
// END FILE


