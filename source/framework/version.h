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

#ifndef VD_FRAMEWORK_VERSION_INCLUDED
#define VD_FRAMEWORK_VERSION_INCLUDED

// ============================================================================================== //

#ifndef VD_VERSION_MAJOR
#define VD_VERSION_MAJOR 				1
#define VD_VERSION_MINOR 				0
#define VD_VERSION_PATCH 				0
#define VD_VERSION_NAMESPACE			v1p0
#endif
#ifndef VD_VERSION_SUFFIX
#define VD_VERSION_SUFFIX				"DevBuild"
#endif

#define VD_VERSION 						(10000 * VD_VERSION_MAJOR + \
                      					   100 * VD_VERSION_MINOR + \
                           						 VD_VERSION_PATCH)
#ifndef VD_VERSION_STRING
#define VD_MAKE_VERSION_STRING2(a,b,c) 	#a "." #b "." #c
#define VD_MAKE_VERSION_STRING(a,b,c) 	VD_MAKE_VERSION_STRING2(a,b,c)
#define VD_VERSION_STRING 				VD_MAKE_VERSION_STRING(VD_VERSION_MAJOR, VD_VERSION_MINOR, VD_VERSION_PATCH) "-" VD_VERSION_SUFFIX
#endif

#define VD_INFO_STRING 					"Void Framework " VD_VERSION_STRING " http://www.voidcycles.com"

// ============================================================================================== //

#endif // VD_FRAMEWORK_VERSION_INCLUDED

// ============================================================================================== //
// END FILE
