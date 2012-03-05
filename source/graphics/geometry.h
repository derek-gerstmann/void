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

#ifndef VD_GRAPHICS_GEOMETRY_INCLUDED
#define VD_GRAPHICS_GEOMETRY_INCLUDED

// ============================================================================================== //

#include "graphics/graphics.h"
#include "graphics/buffer.h"

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_BEGIN();

// ============================================================================================== //

class Geometry : public Object 
{
public:

    VD_DECLARE_ENUM(ScalarType,
        I8,
        U8,
        I16,
        U16,
        I32,
        U32,
        F16,
        F32,
        F64);

	VD_DECLARE_ENUM(AttributeSlot,
        Index,
		Position,
		Normal,
		Color,
		TexCoord,
		UserAttrib0,
		UserAttrib1,
		UserAttrib2,
		UserAttrib3,
		UserAttrib4,
		UserAttrib5,
		UserAttrib6,		
		UserAttrib7);

    VD_DECLARE_ENUM(PrimitiveType,
        Points,
        Lines,
        LineStrip,
        LineLoop,
        Quads,
        QuadStrip,
        Triangles,
        TriangleStrip,
        TriangleFan,
        Polygon);

    struct DataRegion 
    {
        vd::u32         Slot;
        vd::u8          Components;
        vd::bytesize    Stride;
        vd::bytesize    Offset;
        ScalarType      DataType;
        void*           Ptr;
    };

	struct Data 
	{
		bool            IsActive;
		vd::u32         Index;
        vd::u32         IndexCount;
        vd::u32         PrimitiveType;
		vd::u32         PrimitiveCount;
		vd::u32         Bindings[AttributeSlot::Count];
        vd::u32         Buffers[AttributeSlot::Count];
	};

    static vd::u32 InvalidBuffer;
    static vd::u32 InvalidSlot;
	
public:
	
    Geometry(Context* context);
    virtual ~Geometry();
    
    virtual vd::status Destroy();
    void Reset();
    void Setup(const Data& data);
    const Data& GetData() const;

    void SetActive(bool v);
    bool IsActive(void);

    vd::status Attach(AttributeSlot::Value attrib, vd::u32 buffer, vd::u32 slot=InvalidSlot);
    vd::status Detach(AttributeSlot::Value attrib);

    bool IsAttributeUsed(AttributeSlot::Value attrib);
    bool IsBufferUsed(AttributeSlot::Value attrib);

    vd::u32 GetBuffer(AttributeSlot::Value attrib);
    vd::u32 GetBinding(AttributeSlot::Value attrib);

	VD_DECLARE_OBJECT(Geometry);
	
protected:

	VD_DISABLE_COPY_CONSTRUCTORS(Geometry);

	Data 			m_Data;
	Context* 		m_Context;
};

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_END();

// ============================================================================================== //

#endif 
