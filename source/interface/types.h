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

#ifndef VD_INTERFACE_TYPES_INCLUDED
#define VD_INTERFACE_TYPES_INCLUDED

// ============================================================================================== //

#include "interface/interface.h"
#include "core/object.h"

// ============================================================================================== //

VD_INTERFACE_NAMESPACE_BEGIN();

// ============================================================================================== //

using VD_CORE_SCOPE::Object;

class Event;
class Mouse;
class Keyboard;
class Window;

// ============================================================================================== //

VD_INTERFACE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_INTERFACE_TYPES_INCLUDED
