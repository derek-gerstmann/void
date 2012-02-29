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

#ifndef VD_GRAPHICS_BUFFER_INCLUDED
#define VD_GRAPHICS_BUFFER_INCLUDED

// ============================================================================================== //

#include "graphics/graphics.h"

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_BEGIN();

// ============================================================================================== //

class Buffer : public Object
{
	friend class Context;

public:

	VD_DECLARE_ENUM(TargetType,
		Invalid,
		ArrayBuffer,
		IndexBuffer,
		PixelBuffer,
		FrameBuffer,
		RenderBuffer,
		TextureBuffer);
		
	VD_DECLARE_ENUM(AttributeType,
		Invalid,
		Index,
		Position,
		Normal,
		Color,
		TexCoord,
		UserAttrib);

	VD_DECLARE_ENUM(DataType,
		Invalid,
		U8,
		U16,
		U32,
		I8,
		I16,
		I32,
		F16,
		F32,
		F64);

	VD_DECLARE_ENUM(UsageMode,
		Invalid,
		Static,
		Stream,
		Dynamic);

	VD_DECLARE_ENUM(AccessMode,
		Invalid,
		ReadOnly,
		WriteOnly,
		ReadWrite);

    VD_DECLARE_ENUM(State,
        Invalid,
        Allocated,
        Orphaned,
        Bound,
        Mapped,
        Streaming);
    
public:

    struct Data 
    {
        vd::u32                 Id;
        vd::u32                 Index;
        vd::u64                 Count;
        vd::u8                  Components;
        vd::u32                 Slot;
        vd::bytesize            Bytes;
        vd::bytesize            Stride;
        vd::bytesize            Offset;
        TargetType::Value       Target;
        AttributeType::Value    Attribute;
        DataType::Value         DataType;
        UsageMode::Value        Usage;
        AccessMode::Value       Access;
        State::Value            State;
        const void*             Ptr;
    };

public:

    Buffer(Context* context);
    virtual ~Buffer();
    
    virtual vd::status Destroy();
    void Reset();
    void Setup(const Data& data);
    const Data& GetData() const;

    void SetState(State::Value v);
    State::Value GetState(void) const;

	VD_DECLARE_OBJECT(Buffer);

private:
	
	VD_DISABLE_COPY_CONSTRUCTORS(Buffer);
	
    Data        m_Data;
    Context*    m_Context;
};


// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_END();

// ============================================================================================== //

#endif 
