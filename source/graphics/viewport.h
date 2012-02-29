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

#ifndef VD_GRAPHICS_VIEWPORT_INCLUDED
#define VD_GRAPHICS_VIEWPORT_INCLUDED

// ============================================================================================== //

#include "graphics/graphics.h"

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_BEGIN();

// ============================================================================================== //

struct Viewport
{
	Viewport(
		vd::u32 vx = 0, 
		vd::u32 vy = 0, 
		vd::u32 vw = 0, 
		vd::u32 vh = 0
	) : X(vx), 
		Y(vy), 
		Width(vw), 
		Height(vh)
	{
		// EMPTY!
	}

	Viewport(
		vd::u32 vw, 
		vd::u32 vh
	) : X(0), 
		Y(0), 
		Width(vw), 
		Height(vh)
	{
		// EMPTY!
	}
	
	Viewport(const Viewport & other)
	{
		*this = other;
	}
	
	~Viewport()
	{
		// EMPTY!
	}

	inline Viewport & operator=(const Viewport & other)
	{
		this->X = other.X;
		this->Y = other.Y;
		this->Width = other.Width;
		this->Height = other.Height;		
		return *this;
	}

	bool operator==(const Viewport& other)
	{
		return (this->X == other.X && 
				this->Y == other.Y &&
				this->Width == other.Width &&
				this->Height == other.Height );
	}
	
	inline bool operator!=(const Viewport& other)
	{
		return !(*this == other);
	}
	
	inline bool IsValid(void)
	{
		return (Width > 0 && Height > 0);
	}
	
	vd::u32 X;
	vd::u32 Y;
	vd::u32 Width;
	vd::u32 Height;
};

// ============================================================================================== //

inline float AspectRatio(const Viewport& vp)
{
	float aspect = float(vp.Width) / float(vp.Height);
	return aspect;
}

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_GRAPHICS_VIEWPORT_INCLUDED
