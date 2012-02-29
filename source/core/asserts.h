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

#ifndef VD_CORE_ASSERTS_INCLUDED
#define VD_CORE_ASSERTS_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/logging.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

#if defined(VD_NDEBUG) || defined(VD_RELEASE_BUILD)

// ============================================================================================== //

#define vdAssert(cond) ((void) 0)
#define vdAssertMsg(cond, explanation) ((void) 0)
#define vdGlobalAssert(cond) ((void) 0)
#define vdGlobalAssertMsg(cond, explanation) ((void) 0)
#define vdException(cond, explanation) ((void) 0)
#define vdGlobalException(cond, explanation) ((void) 0)

// ============================================================================================== //

#else

// ============================================================================================== //

/// Object specific assertion with a custom error message
#define vdAssert(cond) do { \
	if (((cond) ? false : true)) { \
		vdLogError("Assertion \"%s\" failed in %s:%i", \
					#cond, __FILE__, __LINE__); \
		} \
	} while (0)

/// Global assertion with a custom error message
#define vdGlobalAssert(cond) do { \
	if (((cond) ? false : true)) { \
		vdLogGlobalError("Assertion \"%s\" failed in %s:%i", \
			#cond, __FILE__, __LINE__); \
		} \
	} while (0)

/// Object specific assertion with a custom error message
#define vdAssertMsg(cond, explanation) do { \
	if (((cond) ? false : true)) { \
		vdLogError("Assertion \"%s\" failed in %s:%i (" explanation ")", \
			#cond, __FILE__, __LINE__); \
		} \
	} while (0)

/// Global assertion with a custom error message
#define vdGlobalAssertMsg(cond, explanation) do { \
	if (((cond) ? false : true)) { \
		vdLogGlobalError("Assertion \"%s\" failed in %s:%i (" explanation ")", \
			#cond, __FILE__, __LINE__); \
		} \
	} while (0)
	
/// Object specific exception with a custom error message
#define vdException(exception, explanation)  do { \
	vdLogError("Exception \"%s\" caught in %s:%i (" explanation ")", \
		#exception, __FILE__, __LINE__); \
	} while (0)
	
/// Global exception with a custom error message
#define vdGlobalException(exception, explanation)  do { \
	vdLogGlobalError("Exception \"%s\" caught in %s:%i (" explanation ")", \
		#exception, __FILE__, __LINE__); \
	} while (0)
	
#endif

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif	// VD_CORE_ASSERTS_INCLUDED

