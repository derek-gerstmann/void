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

#ifndef VD_INTERFACE_CONTROLS_INCLUDED
#define VD_INTERFACE_CONTROLS_INCLUDED

// ============================================================================================== //

#include "interface/interface.h"
#include "graphics/graphics.h"
#include "graphics/viewport.h"

// ============================================================================================== //

VD_INTERFACE_NAMESPACE_BEGIN();

// ============================================================================================== //

struct CameraControlInterface 
{
public:
	enum Mode {OrbitMode, FlyMode};
	
	CameraControlInterface() {}
	virtual ~CameraControlInterface() {}

	virtual vd::flag SetMode(Mode mode) = 0;
	virtual Mode GetMode(void) const = 0;
	virtual vd::flag SetViewport(const VD_GRAPHICS_SCOPE::Viewport& vp) = 0;
    virtual vd::flag OnMouseDown(vd::i32 x, vd::i32 y, vd::flag ctrl, vd::flag alt, vd::flag sys) = 0;
    virtual vd::flag OnMouseUp(vd::i32 x, vd::i32 y, vd::flag ctrl, vd::flag alt, vd::flag sys) = 0;
    virtual vd::flag OnMouseMove(vd::i32 x, vd::i32 y, vd::flag ctrl, vd::flag alt, vd::flag sys) = 0;
    virtual vd::flag ReturnHome(void) = 0;
    virtual vd::flag Reset(void) = 0;
    virtual vd::flag Goto(const vd::q4f32& q) = 0;
    virtual vd::q4f32 GetRotation() const = 0;
    virtual float GetZoom() const = 0;
    virtual vd::flag Update(vd::u64 microseconds) = 0;
    virtual vd::flag IsDone() const = 0;
    virtual vd::flag IsActive() const = 0;
};

// ============================================================================================== //

struct KeyControlInterface 
{
public:
	
	KeyControlInterface() {}
	virtual ~KeyControlInterface() {}

    virtual vd::flag OnKeyDown(vd::i32 x, vd::i32 y, vd::flag ctrl, vd::flag alt, vd::flag sys) = 0;
    virtual vd::flag OnKeyUp(vd::i32 x, vd::i32 y, vd::flag ctrl, vd::flag alt, vd::flag sys) = 0;
    virtual vd::flag Update(vd::u64 microseconds) = 0;
    virtual vd::flag Resize(vd::u32 width, vd::u32 height) = 0;
    virtual vd::flag Reset() = 0;
    virtual vd::flag IsDone() const = 0;
};

// ============================================================================================== //

VD_INTERFACE_NAMESPACE_END();

// ============================================================================================== //

#endif	// VD_INTERFACE_CONTROLS_INCLUDED

// ============================================================================================== //
// end file
