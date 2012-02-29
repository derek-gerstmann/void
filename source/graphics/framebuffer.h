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

#ifndef VD_GRAPHICS_FRAMEBUFFER_INCLUDED
#define VD_GRAPHICS_FRAMEBUFFER_INCLUDED

// ============================================================================================== //

#include "graphics/graphics.h"

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_BEGIN();

// ============================================================================================== //

enum FramebufferFormat
{
	VD_FRAMEBUFFER_INVALID,
	VD_FRAMEBUFFER_LUMINANCE_F32,
	VD_FRAMEBUFFER_RGBA_F32,
	VD_FRAMEBUFFER_RGBA_F16,
	VD_FRAMEBUFFER_RGBA_U8,
	VD_FRAMEBUFFER_COUNT
};

class Framebuffer : public Object
{
public:

	struct Data 
	{
		bool IsActive;
		vd::u32 Index;
		vd::i32 Width;
		vd::i32 Height;
		vd::u32 ColorTexture;
		vd::u32 DepthTexture;
		vd::u32 Framebuffer;
		vd::u32 Renderbuffer;
		FramebufferFormat Format;		
	};

public:

    Framebuffer(Context* context);
    virtual ~Framebuffer();

    virtual vd::status Destroy();
    void Reset();
    void Setup(const Data& data);
    const Data& GetData() const;

    void SetActive(bool v);
    bool IsActive(void);
    
	VD_DECLARE_OBJECT(Framebuffer);
	
private:

	VD_DISABLE_COPY_CONSTRUCTORS(Framebuffer);

	Data 		m_Data;
	Context* 	m_Context;
};

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_GRAPHICS_FRAMEBUFFER_INCLUDED 

