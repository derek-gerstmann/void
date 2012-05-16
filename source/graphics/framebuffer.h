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
#include "graphics/image.h"
#include "graphics/viewport.h"

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_BEGIN();

// ============================================================================================== //

class Framebuffer : public Object
{
public:

	struct Data 
	{
        vd::u32                     Id;
		vd::u32                     Index;
        vd::u32                     Usage;
		vd::u32                     ColorTexture;
		vd::u32                     DepthTexture;
		vd::u32                     Renderbuffer;
		ChannelLayout::Value        ChannelLayout;	
        ScalarTypeId::Value         ChannelDataType;	
        DepthFormat::Value          DepthFormat; 
        ScalarTypeId::Value         DepthDataType;    
        Graphics::Viewport          Viewport;
	};

public:

    Framebuffer(Context* context);
    virtual ~Framebuffer();

    virtual vd::status Acquire();
    virtual vd::status Release();
    virtual vd::status Destroy();

    void Reset();
    void Setup(const Data& data);
    void Bind();
    void Unbind();

    const Data& GetData() const;
    const Data* GetPtr() const;

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

