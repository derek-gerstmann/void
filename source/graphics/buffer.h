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
		ArrayBuffer,
		IndexBuffer,
		PixelBuffer,
		FrameBuffer,
		RenderBuffer,
		TextureBuffer);
		
	VD_DECLARE_ENUM(AttributeType,
		Index,
		Position,
		Normal,
		Color,
		TexCoord,
		UserAttrib);

	VD_DECLARE_ENUM(TypeId,
		U8,
		U16,
		U32,
		I8,
		I16,
		I32,
		F16,
		F32,
		F64);

	VD_DECLARE_ENUM(UpdateMode,
		Static,
		Stream,
		Dynamic);

	VD_DECLARE_ENUM(AccessMode,
		ReadOnly,
		WriteOnly,
		ReadWrite);

    VD_DECLARE_ENUM(StateId,
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
        vd::u32                 Usage;
        vd::u64                 Count;
        vd::u8                  Components;
        vd::u32                 Slot;
        vd::bytesize            Bytes;
        vd::bytesize            Stride;
        vd::bytesize            Offset;
        TargetType::Value       Target;
        AttributeType::Value    Attribute;
        TypeId::Value         	DataType;
        UpdateMode::Value       UpdateMode;
        AccessMode::Value       AccessMode;
        StateId::Value          State;
        const void*             Ptr;
    };

public:

    Buffer(Context* context);
    virtual ~Buffer();
    
    virtual vd::status Acquire();
    virtual vd::status Release();
    virtual vd::status Destroy();

    void Reset();
    void Setup(const Data& data);
    void Bind();
    void Unbind();

    const Data& GetData() const;
    const Data* GetPtr() const;

    void SetState(StateId::Value v);
    StateId::Value GetState(void) const;
    bool IsActive(void) const;

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
