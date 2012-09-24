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

#ifndef VD_STD_HEADERS_INCLUDED
#define VD_STD_HEADERS_INCLUDED

// ============================================================================================== //
// Standard headers
// ============================================================================================== //

#include <climits>
#include <cmath>
#include <cstring>
#include <cassert>
#include <cfloat>

#include <limits>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <typeinfo>

// ============================================================================================== //

#define VD_INVALID_INDEX 			 (0xFFFFFFFFU)

#ifndef vdLogGlobalError
#define vdLogGlobalError(...)        fprintf(stderr, "ERROR: " __VA_ARGS__)
#endif

// ============================================================================================== //

#endif // VD_STD_HEADERS_INCLUDED

// ============================================================================================== //
// END FILE