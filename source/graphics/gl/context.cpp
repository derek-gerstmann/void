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

#include "graphics/gl/context.h"
#include "core/core.h"
#include "core/asserts.h"
#include "core/memory.h"

#if defined(VD_PLATFORM_OSX)        
#include <CGLCurrent.h>
#include <CGLTypes.h>
#endif

// ============================================================================================== //

VD_GRAPHICS_OPENGL_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_IMPORT(Core, Memory);

// ============================================================================================== //

template<typename T>
bool IsOdd(T x)
{
    return ((x) % 2);
}

static GLenum 
ConvertBufferTargetToGL(
	Buffer::TargetType::Value value)
{
	switch(value)
	{
		case Buffer::TargetType::ArrayBuffer: 		{ return GL_ARRAY_BUFFER; }
		case Buffer::TargetType::IndexBuffer: 		{ return GL_ELEMENT_ARRAY_BUFFER;  }
		case Buffer::TargetType::PixelBuffer: 		{ return GL_PIXEL_UNPACK_BUFFER; }
		case Buffer::TargetType::FrameBuffer: 		{ return GL_FRAMEBUFFER; }
		case Buffer::TargetType::RenderBuffer: 		{ return GL_RENDERBUFFER; }
		case Buffer::TargetType::TextureBuffer: 	{ return GL_TEXTURE_BUFFER; }
		case Buffer::TargetType::Invalid:	
		default:
		{
			vdLogGlobalError("Invalid buffer target '%s' specified for conversion!", 
				Buffer::TargetType::ToString(value));

			return GL_INVALID_ENUM;	
		}
	};
	return GL_INVALID_ENUM;
}

static GLenum 
ConvertGeometryPrimitiveTypeToGL(
    Geometry::PrimitiveType::Value value)
{
    switch(value)
    {
        case Geometry::PrimitiveType::Points:       { return GL_POINTS; }
        case Geometry::PrimitiveType::Lines:        { return GL_LINES; }
        case Geometry::PrimitiveType::LineStrip:    { return GL_LINE_STRIP; }
        case Geometry::PrimitiveType::LineLoop:     { return GL_LINE_LOOP; }
        case Geometry::PrimitiveType::Quads:        { return GL_QUADS; }
        case Geometry::PrimitiveType::QuadStrip:    { return GL_QUAD_STRIP; }
        case Geometry::PrimitiveType::Triangles:    { return GL_TRIANGLES; }
        case Geometry::PrimitiveType::TriangleStrip:{ return GL_TRIANGLE_STRIP; }
        case Geometry::PrimitiveType::TriangleFan:  { return GL_TRIANGLE_FAN; }
        case Geometry::PrimitiveType::Polygon:      { return GL_POLYGON; }
        case Geometry::PrimitiveType::Invalid:
        default:
        {
            vdLogGlobalError("Invalid geometry primitive type '%s' specified for conversion!", 
                Geometry::PrimitiveType::ToString(value));

            return GL_INVALID_ENUM; 
        }
    };
    return GL_INVALID_ENUM;
}
		
static GLenum 
ConvertBufferAttributeToGL(
	Buffer::AttributeType::Value value)
{
	switch(value)
	{
		case Buffer::AttributeType::Index:			{ return GL_ELEMENT_ARRAY_BUFFER; }
		case Buffer::AttributeType::Position:		{ return GL_VERTEX_ARRAY; }
		case Buffer::AttributeType::Normal:			{ return GL_NORMAL_ARRAY; }
		case Buffer::AttributeType::Color:			{ return GL_COLOR_ARRAY; }
		case Buffer::AttributeType::TexCoord:		{ return GL_TEXTURE_COORD_ARRAY; }
		case Buffer::AttributeType::UserAttrib:		{ return GL_COLOR_ARRAY; }
		case Buffer::AttributeType::Invalid:
		default:
		{
			vdLogGlobalError("Invalid buffer attribute '%s' specified for conversion!", 
				Buffer::AttributeType::ToString(value));

			return GL_INVALID_ENUM;	
		}
	};
	return GL_INVALID_ENUM;
}

static GLenum
ConvertBufferDataTypeToGL(
	Buffer::DataType::Value value)
{
	switch(value)
	{
		case Buffer::DataType::U8:					{ return GL_UNSIGNED_BYTE; }
		case Buffer::DataType::U16:					{ return GL_UNSIGNED_SHORT; }
		case Buffer::DataType::U32:					{ return GL_UNSIGNED_INT; }
		case Buffer::DataType::I8:					{ return GL_BYTE; }
		case Buffer::DataType::I16:					{ return GL_SHORT; }
		case Buffer::DataType::I32:					{ return GL_INT; }
		case Buffer::DataType::F16:					{ return GL_HALF_FLOAT; }
		case Buffer::DataType::F32:					{ return GL_FLOAT; }
		case Buffer::DataType::F64:					{ return GL_DOUBLE; }
		case Buffer::DataType::Invalid:
		default:
		{
			vdLogGlobalError("Invalid buffer datatype '%s' specified for conversion!", 
				Buffer::DataType::ToString(value));

			return GL_INVALID_ENUM;	
		}
	};
	return GL_INVALID_ENUM;
}

static GLenum
ConvertBufferAccessUsageModeToGL(
	Buffer::AccessMode::Value access,
	Buffer::UsageMode::Value usage)
{
/*
	STREAM
	The data store contents will be modified once and used at most a few times.
	
	STATIC
	The data store contents will be modified once and used many times.
	
	DYNAMIC
	The data store contents will be modified repeatedly and used many times.

	DRAW
	The data store contents are modified by the application, and used as the source for GL drawing and image specification commands.
	
	READ
	The data store contents are modified by reading data from the GL, and used to return that data when queried by the application.
	
	COPY
	The data store contents are modified by reading data from the GL, and used as the source for GL drawing and image specification commands.
*/
	switch(usage)
	{
		case Buffer::UsageMode::Static:
		{
			switch(access)
			{
				case Buffer::AccessMode::ReadOnly:	{ return GL_STATIC_DRAW; }
				case Buffer::AccessMode::WriteOnly: { return GL_STATIC_READ; }
				case Buffer::AccessMode::ReadWrite: { return GL_STATIC_COPY; }
				case Buffer::AccessMode::Invalid:
				default:
				{
					vdLogGlobalError("Invalid buffer usage '%s' and access '%s' modes specified for conversion!", 
						Buffer::UsageMode::ToString(usage), Buffer::AccessMode::ToString(access));
		
					return GL_INVALID_ENUM;	
				}
			};
		}
		case Buffer::UsageMode::Stream:
		{
			switch(access)
			{
				case Buffer::AccessMode::ReadOnly:	{ return GL_STREAM_DRAW; }
				case Buffer::AccessMode::WriteOnly: { return GL_STREAM_READ; }
				case Buffer::AccessMode::ReadWrite: { return GL_STREAM_COPY; }
				case Buffer::AccessMode::Invalid:
				default:
				{
					vdLogGlobalError("Invalid buffer usage '%s' and access '%s' modes specified for conversion!", 
						Buffer::UsageMode::ToString(usage), Buffer::AccessMode::ToString(access));
		
					return GL_INVALID_ENUM;	
				}
			};
		}
		case Buffer::UsageMode::Dynamic:
		{
			switch(access)
			{
				case Buffer::AccessMode::ReadOnly:	{ return GL_DYNAMIC_DRAW; }
				case Buffer::AccessMode::WriteOnly: { return GL_DYNAMIC_READ; }
				case Buffer::AccessMode::ReadWrite: { return GL_DYNAMIC_COPY; }
				case Buffer::AccessMode::Invalid:
				default:
				{
					vdLogGlobalError("Invalid buffer usage '%s' and access '%s' modes specified for conversion!", 
						Buffer::UsageMode::ToString(usage), Buffer::AccessMode::ToString(access));
		
					return GL_INVALID_ENUM;	
				}
			};
		}
		case Buffer::DataType::Invalid:
        default:
        {
            vdLogGlobalError("Invalid buffer usage '%s' and access '%s' modes specified for conversion!", 
                Buffer::UsageMode::ToString(usage), Buffer::AccessMode::ToString(access));

            return GL_INVALID_ENUM; 
        }
	};
	return GL_INVALID_ENUM;
}

static GLenum
ConvertBufferAccessModeToGL(
	Buffer::AccessMode::Value access)
{
	switch(access)
	{
		case Buffer::AccessMode::ReadOnly:	{ return GL_READ_ONLY; }
		case Buffer::AccessMode::WriteOnly:	{ return GL_WRITE_ONLY; }
		case Buffer::AccessMode::ReadWrite:	{ return GL_READ_WRITE; }
		case Buffer::AccessMode::Invalid:
		default:
		{
			vdLogGlobalError("Invalid buffer access '%s' mode specified for conversion!", 
				Buffer::AccessMode::ToString(access));

			return GL_INVALID_ENUM;	
		}
	};
	return GL_INVALID_ENUM;
}

static size_t
GetSizeOfDataTypeFromGL(
    GLenum eDataType)
{
    switch(eDataType)
    {
        case (GL_BYTE):                 
            return sizeof(GLbyte);
        case (GL_UNSIGNED_BYTE):                 
            return sizeof(GLubyte);
        case (GL_SHORT):                      
            return sizeof(GLshort);
        case (GL_UNSIGNED_SHORT):                
            return sizeof(GLushort);
        case (GL_INT):                 
            return sizeof(GLint);
        case (GL_UNSIGNED_INT):                   
            return sizeof(GLuint);
        case (GL_FLOAT):                          
            return sizeof(GLfloat);
        case (GL_DOUBLE):
            return sizeof(GLdouble);
        default:
        {
            vdLogGlobalError("Invalid data type request for conversion!");
            return GL_INVALID_ENUM; 
        }
    };
    
    return 0;
}


// ============================================================================================== //

Context::Context(
    Runtime::Context* runtime) :
    Graphics::Context(runtime),
	m_Native(NULL)
{
	// EMPTY!
}

Context::~Context()
{
//    Destroy();
}

vd::status
Context::Destroy()
{
    for(size_t i = 0; i < m_Buffers.size(); i++)
    {
        Buffer* b = m_Buffers[i];
        if(b != NULL) Release(b);
        m_Buffers[i] = NULL;
    }

    for(size_t i = 0; i < m_Geometry.size(); i++)
    {
        Geometry* geo = m_Geometry[i];
        if(geo != NULL) Release(geo);
        m_Geometry[i] = NULL;
    }

    for(size_t i = 0; i < m_Framebuffers.size(); i++)
    {
        Framebuffer* fb = m_Framebuffers[i];
        if(fb != NULL) Release(fb);
        m_Framebuffers[i] = NULL;
    }

    for(size_t i = 0; i < m_Shaders.size(); i++)
    {
        Shader* s = m_Shaders[i];
        if(s != NULL) Release(s);
        m_Shaders[i] = NULL;
    }

    m_Buffers.clear();
    m_Geometry.clear();
    m_Framebuffers.clear();
    m_Textures.clear();
    m_Shaders.clear();
    return Status::Code::Success;
}

vd::status 
Context::Clear(vd::f32 r, vd::f32 g, vd::f32 b, vd::f32 a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return Status::Code::Success;
}

vd::status 
Context::Flush()
{
    glFlush();   
    return Status::Code::Success;
}

vd::status 
Context::Finish()
{
    glFinish();
    return Status::Code::Success;
}

// ============================================================================================== //

void
Context::ReportErrors(
    const char* msg,
    const char *func, 
    const char *filename, 
    int linenum)
{
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        const char* str = (const char*)gluErrorString(err);
        vdLogWarning("%s %s(%d) : ", filename, func, linenum);
        vdLogWarning("%s '%s'", msg ? msg : "   ", str != NULL ? str : "<NULL>");
    }
}

void
Context::RenderToScreen(
    vd::u32 uiTextureId, const Graphics::Viewport& vp, 
    vd::f32 *afData, bool bClear)
{
    static Geometry* quad = NULL;
    if(quad == NULL)
    {
        quad = CreateQuad(
                    -1.0f, +1.0f, +1.0f, -1.0f,
                    +0.0f, +1.0f, +0.0f, +1.0f);
    }
        
    int iViewport[4];
    glDisable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glGetIntegerv(GL_VIEWPORT, iViewport);
    glViewport( vp.X, vp.Y, vp.Width, vp.Height );

    if(bClear)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D( -1.0, 1.0, -1.0, 1.0 );
    
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode( GL_TEXTURE );
    glPushMatrix();
    glLoadIdentity();

    glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );
    glDisable( GL_ALPHA_TEST );
    glDisable( GL_BLEND );
    
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, uiTextureId );

    if(afData)
    {
//        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, vp.Width, vp.Height, GL_RGBA, GL_FLOAT, afData);
        vdLogInfo("Uploading texture data [%x] %p\n", uiTextureId, afData);
    }
    
    glColor3f( 1.0f, 1.0f, 1.0f );

    Bind(quad);
    Submit(quad);
    Unbind(quad);

    glActiveTexture(GL_TEXTURE0);
    glDisable( GL_TEXTURE_2D );
    glMatrixMode( GL_TEXTURE );
    glPopMatrix();

    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();    

    glViewport(iViewport[0], iViewport[1], iViewport[2], iViewport[3]);
    glMatrixMode( GL_MODELVIEW );
}

Buffer*
Context::CreateBuffer(
	Buffer::TargetType::Value target, 
	Buffer::AttributeType::Value attrib,
	Buffer::AccessMode::Value access,
	Buffer::UsageMode::Value usage,
	Buffer::DataType::Value datatype,	
	vd::u8 components, vd::u32 count, 
    const void* ptr)
{
    vdLogOpenGLErrors("Start");

    GLuint gl_id = 0;
    glGenBuffers(1, &gl_id);
    if(!gl_id)
    {
    	vdLogGlobalError("Failed to create buffer object!");
		return NULL;
	}
	
    GLenum gl_target = ConvertBufferTargetToGL(target);
    GLenum gl_usage = ConvertBufferAccessUsageModeToGL(access, usage);
    GLenum gl_datatype = ConvertBufferDataTypeToGL(datatype);
    size_t gl_bytes = GetSizeOfDataTypeFromGL(gl_datatype) * components * count;

    glBindBuffer(gl_target, gl_id);
    glBufferData(gl_target, gl_bytes, ptr, gl_usage);
    glBindBuffer(gl_target, 0);

    vdGlobalAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error after creating buffer!");

    Buffer::Data data;
    Core::Memory::MemSet(&data, 0, sizeof(data));

    data.Id = gl_id;
    data.Target = target;
    data.Attribute = attrib;
    data.Access = access;
    data.Usage = usage;
    data.DataType = datatype;
    data.Components = components;
    data.Count = count;
    data.Bytes = gl_bytes;
    data.Index = m_Buffers.size();
    data.State = Buffer::State::Allocated;

    Buffer* buffer = VD_NEW(Buffer, this);
    buffer->Setup(data);
    m_Buffers.push_back(buffer);

    vdLogOpenGLErrors("End");
    return buffer;
}

vd::status
Context::Release(
	Buffer* buffer)
{
    vdLogOpenGLErrors("Start");

    vdAssert(buffer != NULL);
    Unbind(buffer);
    
    const Buffer::Data& data = buffer->GetData();
    vd::u32 index = data.Index;
    GLuint gl_id = (GLuint)data.Id;

    if(gl_id) glDeleteBuffers(1, &gl_id);

    buffer->Destroy();
    m_Buffers[index] = NULL;
    VD_DELETE(buffer);

    vdLogOpenGLErrors("End");

    return Status::Code::Success;
}

void* 
Context::Map(
    Buffer* buffer, 
    Buffer::AccessMode::Value access)
{
    vdLogOpenGLErrors("Start");

    vdAssert(buffer != NULL);
    const Buffer::Data& data = buffer->GetData();
    GLuint gl_id = (GLuint)data.Id;
    if(gl_id < 1) return NULL;

	GLenum gl_access = ConvertBufferAccessModeToGL(access);
    GLenum gl_target = ConvertBufferTargetToGL(data.Target);

    buffer->SetState(Buffer::State::Mapped);

    glBindBuffer(gl_target, gl_id);
    void* ptr = glMapBuffer(gl_target, gl_access);

    vdLogOpenGLErrors("End");
    return ptr;
}

vd::status 
Context::Unmap(
	Buffer* buffer)
{
    vdLogOpenGLErrors("Start");

    vdAssert(buffer != NULL);
    if(buffer->GetState() != Buffer::State::Mapped)
        return Status::Code::Reject;

    const Buffer::Data& data = buffer->GetData();
    GLenum gl_target = ConvertBufferTargetToGL(data.Target);

    glUnmapBuffer(gl_target);
    glBindBuffer(gl_target, 0);
    buffer->SetState(Buffer::State::Allocated);

    vdLogOpenGLErrors("End");

    return Status::Code::Success;
}

vd::status
Context::Bind(
	Buffer* buffer)
{
    vdLogOpenGLErrors("Start");

    vdAssert(buffer != NULL);
    const Buffer::Data& data = buffer->GetData();
    GLuint gl_id = (GLuint)data.Id;
    if(gl_id < 1) return Status::Code::Reject;

    GLenum gl_target = ConvertBufferTargetToGL(data.Target);
    GLenum gl_attrib = ConvertBufferAttributeToGL(data.Attribute);
	GLenum gl_datatype = ConvertBufferDataTypeToGL(data.DataType);
	GLuint gl_components = (GLuint)data.Components;
	GLuint gl_stride = 0;
	const GLvoid* gl_addr = 0;
                               	
    buffer->SetState(Buffer::State::Bound);
    glBindBuffer(gl_target, gl_id);
	switch(gl_attrib)
	{
		case(GL_VERTEX_ARRAY):
		{   
			glVertexPointer(gl_components, gl_datatype, gl_stride, gl_addr);
			glEnableClientState(GL_VERTEX_ARRAY);
			break;
		}
		case(GL_COLOR_ARRAY):
		{   
			glColorPointer(gl_components, gl_datatype, gl_stride, gl_addr);
			glEnableClientState(GL_COLOR_ARRAY);
			break;
		}
		case(GL_NORMAL_ARRAY):
		{
			glNormalPointer(gl_datatype, gl_stride, gl_addr);
			glEnableClientState(GL_NORMAL_ARRAY);
			break;
		}
		case(GL_TEXTURE_COORD_ARRAY):
		{
			glTexCoordPointer(gl_components, gl_datatype, gl_stride, gl_addr);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);        
			break;
		}
		case(GL_INDEX_ARRAY):
		case(GL_EDGE_FLAG_ARRAY):
		default:
			break;
	};

    vdLogOpenGLErrors("End");
    
    return Status::Code::Success;
}

vd::status 
Context::Submit(
    Buffer* buffer, 
    Geometry::PrimitiveType::Value primitives, 
    vd::u32 offset, vd::u32 count)
{
    vdLogOpenGLErrors("Start");

    vdAssert(buffer != NULL);
    if(buffer->GetState() != Buffer::State::Bound)
        return Status::Code::Reject;

    GLenum gl_prim = ConvertGeometryPrimitiveTypeToGL(primitives);
    glDrawArrays( gl_prim, offset, count );
    vdLogOpenGLErrors("End");
    return Status::Code::Success;
}

vd::status
Context::Unbind(
	Buffer* buffer)
{
    vdLogOpenGLErrors("Start");

    vdAssert(buffer != NULL);
    if(buffer->GetState() != Buffer::State::Bound)
        return Status::Code::Reject;

    const Buffer::Data& data = buffer->GetData();
    GLuint gl_id = (GLuint)data.Id;
    GLenum gl_target = ConvertBufferTargetToGL(data.Target);
    GLenum gl_attrib = ConvertBufferAttributeToGL(data.Attribute);

    glBindBuffer(gl_target, gl_id);
	switch(gl_attrib)
	{
		case(GL_VERTEX_ARRAY):
		{   
			glDisableClientState(GL_VERTEX_ARRAY);
			break;
		}
		case(GL_COLOR_ARRAY):
		{   
			glDisableClientState(GL_COLOR_ARRAY);
			break;
		}
		case(GL_NORMAL_ARRAY):
		{
			glDisableClientState(GL_NORMAL_ARRAY);
			break;
		}
		case(GL_TEXTURE_COORD_ARRAY):
		{
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);        
			break;
		}
		case(GL_INDEX_ARRAY):
		case(GL_EDGE_FLAG_ARRAY):
		default:
			break;
	};
    
    glBindBuffer(gl_target, 0);
    buffer->SetState(Buffer::State::Allocated);

    vdLogOpenGLErrors("End");
    return Status::Code::Success;
}

vd::status
Context::Release(
	Geometry* geo)
{
    vdLogOpenGLErrors("Start");

    const Geometry::Data& data = geo->GetData();
    for(vd::u32 i = 0; i < Geometry::AttributeSlot::Count; i++)
    {
        vd::u32 index = data.Buffers[i];
        if(m_Buffers.size() < index)
            continue;

        Buffer* buffer = m_Buffers[index];
        if(buffer == NULL)
            continue;

        Release(buffer);
    }

    m_Geometry[data.Index] = NULL;
    VD_DELETE(geo);

    vdLogOpenGLErrors("End");

    return Status::Code::Success;	
}

Geometry*
Context::CreatePointList(
	vd::f32* positions, vd::i32 components, vd::u64 count)
{
    vdLogOpenGLErrors("Start");

    Geometry::Data data;
    Core::Memory::MemSet(&data, 0, sizeof(data));

    data.IndexCount = 0;
    data.PrimitiveType = GL_POINTS;
    data.PrimitiveCount = count;
    data.Index = m_Geometry.size();

    Buffer* position_buffer = CreateBuffer(
        Buffer::TargetType::ArrayBuffer, 
        Buffer::AttributeType::Position,
        Buffer::AccessMode::ReadOnly,
        Buffer::UsageMode::Static,
        Buffer::DataType::F32,
        components, data.PrimitiveCount, positions
    );

    const Buffer::Data& pbd = position_buffer->GetData();

    Geometry* geo = VD_NEW(Geometry, this);
    geo->Setup(data);
    geo->Attach(Geometry::AttributeSlot::Position, pbd.Index);
    m_Geometry.push_back(geo);

    vdLogOpenGLErrors("End");

    return geo;
}

Geometry*
Context::CreatePoint(
	vd::f32 x, vd::f32 y, vd::f32 z)
{
    vdLogOpenGLErrors("Start");

    vd::f32 positions[] = {
    	x, y, z, 1.0f
    };
    
	unsigned int faces[] = { 0 };
    
    Geometry* geo = VD_NEW(Geometry, this);
    Geometry::Data data = geo->GetData();
    data.IndexCount = 1;
    data.PrimitiveCount = 1;
    data.PrimitiveType = GL_POINTS;
    data.Index = m_Geometry.size();
    
    Buffer* position_buffer = CreateBuffer(
        Buffer::TargetType::ArrayBuffer, 
        Buffer::AttributeType::Position,
        Buffer::AccessMode::ReadOnly,
        Buffer::UsageMode::Static,
        Buffer::DataType::F32,
        4, data.PrimitiveCount, positions
    );

    Buffer* index_buffer = CreateBuffer(
        Buffer::TargetType::IndexBuffer, 
        Buffer::AttributeType::Index,
        Buffer::AccessMode::ReadOnly,
        Buffer::UsageMode::Static,
        Buffer::DataType::U32,
        1, data.IndexCount, faces
    );

    const Buffer::Data& pbd = position_buffer->GetData();
    const Buffer::Data& ibd = index_buffer->GetData();
  
    geo->Setup(data);
    geo->Attach(Geometry::AttributeSlot::Position, pbd.Index);
    geo->Attach(Geometry::AttributeSlot::Index, ibd.Index);
    m_Geometry.push_back(geo);

    vdLogOpenGLErrors("End");
    return geo;
}

Geometry*
Context::CreateQuad(
	vd::f32 left, vd::f32 top, vd::f32 right, vd::f32 bottom,
    vd::f32 u0, vd::f32 u1, vd::f32 v0, vd::f32 v1)
{
    vdLogOpenGLErrors("Start");

    const vd::f32 positions[] = {
        left,   top,    0.0f, 1.0f,
        right,  top,    0.0f, 1.0f,
        right,  bottom, 0.0f, 1.0f,
        left,   bottom, 0.0f, 1.0f,
    };

    const vd::f32 texcoords[] = {
        u0, v0,
        u1, v0,
        u1, v1,
        u0, v1,
    };

    const unsigned int faces[] = { 3, 2, 1, 1, 0, 3 };

    Geometry::Data data;
    Core::Memory::MemSet(&data, 0, sizeof(data));
    data.Index = m_Geometry.size();
    data.IndexCount = 6;
    data.PrimitiveCount = 4;
    data.PrimitiveType = GL_TRIANGLES;

    Buffer* position_buffer = CreateBuffer(
        Buffer::TargetType::ArrayBuffer, 
        Buffer::AttributeType::Position,
        Buffer::AccessMode::ReadOnly,
        Buffer::UsageMode::Static,
        Buffer::DataType::F32,
        4, data.PrimitiveCount, positions
    );

    Buffer* texcoord_buffer = CreateBuffer(
        Buffer::TargetType::ArrayBuffer, 
        Buffer::AttributeType::TexCoord,
        Buffer::AccessMode::ReadOnly,
        Buffer::UsageMode::Static,
        Buffer::DataType::F32,
        2, data.PrimitiveCount, texcoords
    );

    Buffer* index_buffer = CreateBuffer(
        Buffer::TargetType::IndexBuffer, 
        Buffer::AttributeType::Index,
        Buffer::AccessMode::ReadOnly,
        Buffer::UsageMode::Static,
        Buffer::DataType::U32,
        1, data.IndexCount, faces
    );
    
    const Buffer::Data& pbd = position_buffer->GetData();
    const Buffer::Data& tbd = texcoord_buffer->GetData();
    const Buffer::Data& ibd = index_buffer->GetData();

    Geometry* geo = VD_NEW(Geometry, this);
    geo->Setup(data);
    geo->Attach(Geometry::AttributeSlot::Position, pbd.Index);
    geo->Attach(Geometry::AttributeSlot::TexCoord, tbd.Index);
    geo->Attach(Geometry::AttributeSlot::Index, ibd.Index);    
	m_Geometry.push_back(geo);

    vdLogOpenGLErrors("End");
    return geo;
}

Geometry*
Context::CreateQuad()
{
	return CreateQuad(-1.0f, +1.0f, +1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
}

Geometry*
Context::CreateWireGrid(
	int rows, int columns,
	vd::f32 left, vd::f32 right,
	vd::f32 bottom, vd::f32 top)
{
    vdLogOpenGLErrors("Start");

    vd::f32 fDX = (right - left) / (columns-1.0f);
    vd::f32 fDY = (top - bottom) / (rows-1.0f);
    size_t vertex_count = (rows+1) * 2 + (columns+1) * 2;

    Geometry::Data data;
    Core::Memory::MemSet(&data, 0, sizeof(data));

    data.Index = m_Geometry.size();
    data.IndexCount = vertex_count;
    data.PrimitiveCount = vertex_count;
    data.PrimitiveType = GL_LINES;

	vd::f32* positions = VD_NEW_ARRAY(vd::f32, vertex_count * 4);
	vd::f32* p = positions;
	for (int i=0; i<=rows; i++) 
	{
		vd::f32 fX = left + columns * fDX;
		vd::f32 fY = bottom + i * fDY;

		*p++ = 0.0f;
		*p++ = fY;
		*p++ = 0.0f;
		*p++ = 1.0f;

		*p++ = fX;
		*p++ = fY;
		*p++ = 0.0f;
		*p++ = 1.0f;

	}

	for (int i=0; i<=columns; i++) 
	{
		vd::f32 fX = left + i * fDX;
		vd::f32 fY = bottom + rows * fDY;

		*p++ = fX;
		*p++ = 0.0f;
		*p++ = 0.0f;
		*p++ = 1.0f;

		*p++ = fX;
		*p++ = fY;
		*p++ = 0.0f;
		*p++ = 1.0f;
	}

	vdLogGlobalDebug("Allocated Vertices: %d\n", vertex_count * 4);

    Buffer* position_buffer = CreateBuffer(
        Buffer::TargetType::ArrayBuffer, 
        Buffer::AttributeType::Position,
        Buffer::AccessMode::ReadOnly,
        Buffer::UsageMode::Static,
        Buffer::DataType::F32,
        4, data.PrimitiveCount, positions
    );
    VD_DELETE_ARRAY(positions);
    
    const Buffer::Data& pbd = position_buffer->GetData();

    Geometry* geo = VD_NEW(Geometry, this);
    geo->Setup(data);
    geo->Attach(Geometry::AttributeSlot::Position, pbd.Index);
    m_Geometry.push_back(geo);

    vdLogGlobalDebug("Created Wire Grid: PrimitiveCount: %d IndexCount: %d\n", data.PrimitiveCount, data.IndexCount); 
    vdLogOpenGLErrors("End");
    return geo;		
}

	
Geometry*
Context::CreateGrid(
	int uiCountX, int uiCountY,
	vd::f32 fX0, vd::f32 fX1, 
	vd::f32 fY0, vd::f32 fY1, 
	bool bVertical)
{
    vdLogOpenGLErrors("Start");

    vd::f32 fDX = (fX1 - fX0) / (uiCountX-1.0f);
    vd::f32 fDY = (fY1 - fY0) / (uiCountY-1.0f);

	vd::f32 fTU = (1.0f) / (uiCountX-1.0f);
	vd::f32 fTV = (1.0f) / (uiCountY-1.0f);

    size_t vertex_count = uiCountX * uiCountY;
    size_t index_count = 2 * (uiCountY - 1) * uiCountX;

    Geometry::Data data;
    Core::Memory::MemSet(&data, 0, sizeof(data));

    data.Index = m_Geometry.size();
    data.IndexCount = index_count;
    data.PrimitiveCount = vertex_count;
    data.PrimitiveType = GL_QUAD_STRIP;

    vd::u64 pi = 0;
	vd::f32* positions = VD_NEW_ARRAY(vd::f32, 4 * vertex_count);
	for(int y=0; y < uiCountY; y++)
	{
		for(int x=0; x < uiCountX; x++)
		{           
			vd::f32 fX = fX0 + x * fDX;
			vd::f32 fY = fY0 + y * fDY;
			
			if(bVertical)
			{
				positions[pi++] = fX;
				positions[pi++] = fY;
				positions[pi++] = 0.0f;
				positions[pi++] = 1.0f;
			}
			else
			{
				positions[pi++] = fX;
				positions[pi++] = 0.0f;
				positions[pi++] = fY;
				positions[pi++] = 1.0f;
			}	
		}
	}

    Buffer* position_buffer = CreateBuffer(
        Buffer::TargetType::ArrayBuffer, 
        Buffer::AttributeType::Position,
        Buffer::AccessMode::ReadOnly,
        Buffer::UsageMode::Static,
        Buffer::DataType::F32,
        4, data.PrimitiveCount, positions
    );

	VD_DELETE_ARRAY(positions);

	size_t ti = 0;
	vd::f32* texcoords = VD_NEW_ARRAY(vd::f32, 2 * vertex_count );
	for(int y=0; y < uiCountY; y++)
	{
		for(int x=0; x < uiCountX; x++)
		{           
			vd::f32 fU = x * fTU;
			vd::f32 fV = y * fTV;

			texcoords[ti++] = fU;
			texcoords[ti++] = fV;
		}
	}

    Buffer* texcoord_buffer = CreateBuffer(
        Buffer::TargetType::ArrayBuffer, 
        Buffer::AttributeType::TexCoord,
        Buffer::AccessMode::ReadOnly,
        Buffer::UsageMode::Static,
        Buffer::DataType::F32,
        2, data.PrimitiveCount, texcoords
    );

	VD_DELETE_ARRAY(texcoords);
	
	size_t fi = 0;	
	unsigned int* faces = VD_NEW_ARRAY(unsigned int, index_count);
	for(int y = 0; y < uiCountY - 1; y++)
	{
		if(IsOdd(y))
		{
			for(int x = uiCountX - 1; x >= 0; x--)
			{
				faces[fi++] = (uiCountX * (y + 1) + x);
				faces[fi++] = (uiCountX * (y + 0) + x);
			}
		}
		else
		{
			for(int x = 0; x < uiCountX; x++)
			{
				faces[fi++] = (uiCountX * (y + 0) + x);
				faces[fi++] = (uiCountX * (y + 1) + x);
			}
		}
	}

    Buffer* index_buffer = CreateBuffer(
        Buffer::TargetType::IndexBuffer, 
        Buffer::AttributeType::Index,
        Buffer::AccessMode::ReadOnly,
        Buffer::UsageMode::Static,
        Buffer::DataType::U32,
        1, data.IndexCount, faces
    );

	VD_DELETE_ARRAY(faces);

    const Buffer::Data& pbd = position_buffer->GetData();
    const Buffer::Data& tbd = texcoord_buffer->GetData();
    const Buffer::Data& ibd = index_buffer->GetData();

    Geometry* geo = VD_NEW(Geometry, this);
    geo->Setup(data);
    geo->Attach(Geometry::AttributeSlot::Position, pbd.Index);
    geo->Attach(Geometry::AttributeSlot::TexCoord, tbd.Index);
    geo->Attach(Geometry::AttributeSlot::Index, ibd.Index);    
    m_Geometry.push_back(geo);

    vdLogGlobalDebug("Created Grid: PrimitiveCount: %d IndexCount: %d\n", data.PrimitiveCount, data.IndexCount);
    vdLogOpenGLErrors("End");
    return geo;
}
    
vd::status 
Context::Bind(
    Geometry* geo)
{
    vdLogOpenGLErrors("Start");

    const Geometry::Data& data = geo->GetData();

    vdGlobalAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error prior to render geometry.");

    for(vd::u32 i = 0; i < Geometry::AttributeSlot::Count; i++)
    {
        if(data.Buffers[i] >= VD_U32_MAX)
            continue;
        
        vd::u32 index = data.Buffers[i];
        if(m_Buffers.size() < index)
            continue;
        
        Graphics::Buffer* buffer = m_Buffers[i];
        if(buffer == NULL)
            continue;

        Geometry::AttributeSlot::Value attrib =  Geometry::AttributeSlot::FromInteger(i);
        Geometry::AttributeSlot::Value binding = Geometry::AttributeSlot::FromInteger(data.Bindings[i]);
        vd::u32 slot = Geometry::AttributeSlot::ToInteger(binding);

        Bind(buffer);
        Bind(attrib, buffer, slot);
    }

    vdLogOpenGLErrors("End");

    return Status::Code::Success;
}

vd::status 
Context::Submit(
    Geometry* geo,
    vd::u32 start, 
    vd::u32 end, 
    vd::u32 stride)
{
    vdLogOpenGLErrors("Start");

    const Geometry::Data& data = geo->GetData();

    vdGlobalAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error prior to render geometry.");

    if(data.Buffers[Geometry::AttributeSlot::Index] < VD_U32_MAX)
    {
        vd::u32 index = data.Buffers[Geometry::AttributeSlot::Index];
        if(m_Buffers.size() < index)
            return Status::Code::Reject;
        
        Buffer* buffer = m_Buffers[index];
        if(buffer == NULL)
            return Status::Code::Reject;

        vdGlobalAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error pre submit index buffer.");
        Bind(buffer);

        vdGlobalAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error post submit index buffer.");
        GLenum gl_datatype = ConvertBufferDataTypeToGL(buffer->GetData().DataType);
        glDrawElements(data.PrimitiveType, data.IndexCount, gl_datatype, 0);
        vdGlobalAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error during draw elements.");

        Unbind(buffer);
    }
    else
    {
        vdGlobalAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error pre submit draw arrays.");
        glDrawArrays( data.PrimitiveType, 0, data.PrimitiveCount );
        vdGlobalAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error post submit draw arrays.");
    }

    vdLogOpenGLErrors("End");
    return Status::Code::Success;
}

vd::status 
Context::Unbind(
    Geometry* geo)
{
    vdLogOpenGLErrors("Start");

    vdAssert(geo != NULL);
    const Geometry::Data& data = geo->GetData();
    vdGlobalAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error prior to render geometry.");

    for(vd::u32 i = 0; i < Geometry::AttributeSlot::Count; i++)
    {
        vd::u32 index = data.Buffers[i];
        if(m_Buffers.size() < index)
            return Status::Code::Reject;
        
        Buffer* buffer = m_Buffers[i];
        if(buffer == NULL)
            return Status::Code::Reject;
        
        Geometry::AttributeSlot::Value attrib =  Geometry::AttributeSlot::FromInteger(i);
        Geometry::AttributeSlot::Value binding = Geometry::AttributeSlot::FromInteger(data.Bindings[i]);
        vd::u32 slot = Geometry::AttributeSlot::ToInteger(binding);

        Unbind(buffer);
        Unbind(attrib, buffer, slot);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    vdLogOpenGLErrors("End");
    return Status::Code::Success;
}

vd::status
Context::Attach(
    Geometry* geo, 
    Geometry::AttributeSlot::Value attrib, 
    vd::u32 buffer,
    vd::u32 slot)
{
    vdAssert(geo != NULL);
    return geo->Attach(attrib, buffer, slot);
}

vd::status
Context::Detach(
    Geometry* geo, 
    Geometry::AttributeSlot::Value attrib)
{
    vdAssert(geo != NULL);
    return geo->Detach(attrib);
}

vd::status
Context::Bind(
    Geometry::AttributeSlot::Value attrib,
    Buffer* buffer,
    vd::u32 slot)
{
    vdAssert(Geometry::AttributeSlot::IsValid(attrib) == true);
    vdAssert(buffer != NULL);

    const Buffer::Data& data = buffer->GetData();
    GLuint components = data.Components;
    GLenum datatype = ConvertBufferDataTypeToGL(data.DataType);
    GLsizei stride = GetSizeOfDataTypeFromGL(datatype) * components;
    const GLvoid* ptr = data.Ptr;

    vdAssert(components >= 1);

    if(slot < Geometry::InvalidSlot)
    {    
        glVertexAttribPointer(slot, components, datatype, GL_FALSE, stride, ptr);
        glEnableVertexAttribArray(slot);    
    }
    else if(attrib == Geometry::AttributeSlot::Position)    
    {
        glVertexPointer( components, datatype, stride, ptr);
        glEnableClientState( GL_VERTEX_ARRAY );
    }  
    else if(attrib == Geometry::AttributeSlot::Normal)
    {
        glNormalPointer( datatype, stride, ptr);
        glEnableClientState( GL_NORMAL_ARRAY );
    } 
    else if(attrib == Geometry::AttributeSlot::TexCoord)
    {
        glTexCoordPointer( components, datatype, stride, ptr );
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    }    
    else if(attrib == Geometry::AttributeSlot::Color)
    {
        glColorPointer( components, datatype, stride, ptr );
        glEnableClientState( GL_COLOR_ARRAY );
    }    

    vdGlobalAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error enabling vertex attribute array!");        
    return Status::Code::Success;
}

vd::status
Context::Unbind(
    Geometry::AttributeSlot::Value attrib,
    Buffer* buffer, vd::u32 slot)
{
    if(slot < Geometry::InvalidSlot)
    {    
        glDisableVertexAttribArray(slot);
    }
    else if(attrib == Geometry::AttributeSlot::Position)    
    {
        glDisableClientState( GL_VERTEX_ARRAY );
    }  
    else if(attrib == Geometry::AttributeSlot::Normal)
    {
        glDisableClientState( GL_NORMAL_ARRAY );
    } 
    else if(attrib == Geometry::AttributeSlot::TexCoord)
    {
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    }    
    else if(attrib == Geometry::AttributeSlot::Color)
    {
        glDisableClientState( GL_COLOR_ARRAY );
    }    

    vdGlobalAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error disabling vertex attribute array!");     
       
    return Status::Code::Success;
}

// ============================================================================================== //

Framebuffer* 
Context::CreateFramebuffer(
    vd::u32 width, vd::u32 height, 
    FramebufferFormat format, bool depth)
{
    GLuint handle = 0;
    GLuint renderbuffer = 0;
    GLuint color_texture = 0;
    GLuint depth_texture = 0;

    vdLogOpenGLErrors("Start");

    glGenFramebuffers(1, &handle);
    glBindFramebuffer(GL_FRAMEBUFFER, handle);

    if(depth)
    {
        glGenTextures(1, &depth_texture);
        glBindTexture(GL_TEXTURE_2D, depth_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, 0);
        vdGlobalAssertMsg(GL_NO_ERROR == glGetError(), "Unable to create depth texture");   
    }
    
    glGenTextures(1, &color_texture);
    glBindTexture(GL_TEXTURE_2D, color_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    switch(format)
    {
        case VD_FRAMEBUFFER_RGBA_F32:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
            break;
        case VD_FRAMEBUFFER_RGBA_F16:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_HALF_FLOAT, 0);
            break;
        case VD_FRAMEBUFFER_RGBA_U8:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
            break;
        case VD_FRAMEBUFFER_LUMINANCE_F32:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_FLOAT, 0);
            break;
        case VD_FRAMEBUFFER_INVALID:
        case VD_FRAMEBUFFER_COUNT:
        default:
        {
            vdException("Invalid Format", "Invalid framebuffer format specified!\n");
            return NULL;
        }
    }

    vdAssertMsg(GL_NO_ERROR == glGetError(), "Unable to create color texture");

    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture, 0);
    vdAssertMsg(GL_NO_ERROR == glGetError(), "Unable to attach color texture to frame buffer");
    if(depth_texture)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture, 0);
        vdGlobalAssertMsg(GL_NO_ERROR == glGetError(), "Unable to attach depth texture to frame buffer");
    }
    vdAssertMsg(GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus(GL_FRAMEBUFFER), "Unable to create framebuffer!\n");
    
    Framebuffer::Data data;
    Core::Memory::MemSet(&data, 0, sizeof(data));

    data.Width = width;
    data.Height = height;
    data.Format = format;
    data.Framebuffer = handle;
    data.Renderbuffer = renderbuffer;
    data.ColorTexture = color_texture;
    data.DepthTexture = depth_texture;
    data.Index = m_Framebuffers.size();

    Framebuffer* fb = VD_NEW(Framebuffer, this);
    fb->Setup(data);
    m_Framebuffers.push_back(fb);

    Bind(fb);
    Clear(fb, 0.0f, 0.0f, 0.0f, 0.0f);
    Unbind(fb);

    vdLogOpenGLErrors("End");
    return fb;
}

vd::status 
Context::Bind(
    Framebuffer* fb)
{
    vdLogOpenGLErrors("Start");

    vdAssert(fb != NULL);
    if(fb->IsActive())
        return Status::Code::Reject;

    fb->SetActive(true);
    const Framebuffer::Data& data = fb->GetData();
    glBindFramebuffer(GL_FRAMEBUFFER, data.Framebuffer);

    vdLogOpenGLErrors("End");

    return Status::Code::Success;
}

vd::status 
Context::Unbind(
    Framebuffer* fb)
{
    vdLogOpenGLErrors("Start");

    vdAssert(fb != NULL);
    if(fb->IsActive() == false)
        return Status::Code::Reject;

    vdGlobalAssertMsg(GL_NO_ERROR == glGetError(), "Error prior to unbinding frame buffer!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    fb->SetActive(false);

    vdLogOpenGLErrors("End");

    return Status::Code::Success;
}

vd::status 
Context::Release(
    Framebuffer* fb)
{
    vdLogOpenGLErrors("Start");

    vdAssert(fb != NULL);
    Unbind(fb);

    GLuint gl_id = 0;
    const Framebuffer::Data& data = fb->GetData();
    if(data.Framebuffer)
    {
        gl_id = (GLuint)data.Framebuffer;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &gl_id);
    }       
    
    if(data.ColorTexture)
    {
        gl_id = (GLuint)data.ColorTexture;
        glDeleteTextures(1, &gl_id);
    }

    if(data.DepthTexture)
    {
        gl_id = (GLuint)data.DepthTexture;
        glDeleteTextures(1, &gl_id);
    }

    m_Framebuffers[data.Index] = NULL;
    VD_DELETE(fb);

    vdLogOpenGLErrors("End");

    return Status::Code::Success;
}

vd::status 
Context::Clear(
    Framebuffer* fb,
    vd::f32 r, vd::f32 g, vd::f32 b, vd::f32 a)
{
    vdLogOpenGLErrors("Start");

    const Framebuffer::Data& data = fb->GetData();
    glViewport(0, 0, data.Width, data.Height);
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vdLogOpenGLErrors("End");

    return Status::Code::Success;
}

// ============================================================================================== //

Graphics::Shader*
Context::CompileShaderFromFile(
    const vd::string& name, 
    const vd::string& vertex, 
    const vd::string& geometry, 
    const vd::string& fragment)
{
    vdLogOpenGLErrors("Start");

    Graphics::Shader* shader = VD_NEW(Shader, this, name);
    shader->Load(vertex.c_str(), geometry.c_str(), fragment.c_str());
    m_Shaders.push_back(shader);

    vdLogOpenGLErrors("End");

    return shader;
}

vd::status 
Context::Release(
    Shader* s)
{
    vdAssert(s != NULL);
    // m_Shaders[data.Index] = NULL;
    // VD_DELETE(fb);
    return Status::Code::Success;
}

vd::status 
Context::Bind(
    Shader* s)
{
    vdLogOpenGLErrors("Start");
    vdAssert(s != NULL);
    s->Bind();
    vdLogOpenGLErrors("End");
    return Status::Code::Success;
}

vd::status 
Context::Unbind(
    Shader* s)
{
    vdLogOpenGLErrors("Start");
    vdAssert(s != NULL);
    s->Unbind();
    vdLogOpenGLErrors("End");
    return Status::Code::Success;
}

// ============================================================================================== //

vd::status
Context::SetViewport(
    const Viewport& vp)
{
    vdLogOpenGLErrors("Start");

    glViewport(vp.X, vp.Y, vp.Width, vp.Height);
    m_Viewport = vp;

    vdLogOpenGLErrors("End");

    return Status::Code::Success;
}

const Viewport&
Context::GetViewport()
{
    return m_Viewport;
}

vd::status
Context::BindOrthographicView()
{
    vdLogOpenGLErrors("Start");

    Viewport vp = m_Viewport;
    SetViewport(vp);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, vp.Width, vp.Height, 0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.375, 0.375, 0);

    vdLogOpenGLErrors("End");

    return Status::Code::Success;
}

vd::status
Context::UnbindOrthographicView()
{
    vdLogOpenGLErrors("Start");

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    vdLogOpenGLErrors("End");

    return Status::Code::Success;    
}

vd::status
Context::SetActive(
    bool v)
{
    vdLogOpenGLErrors("Start");

    if(v)
    {
#if defined(VD_PLATFORM_OSX)        
        if(m_Native == NULL)
        {
            CGLContextObj cgl = CGLGetCurrentContext();
            m_Native = (void*)cgl;
        }
        else
        {
            CGLError error = CGLSetCurrentContext(m_Native);
            if(error != kCGLNoError)
            {
                const char* str = CGLErrorString(error);
                vdLogError("Failed to set current CGL Context: %s", str);
                return Status::Code::InvalidContext;
            }
        }
        return Status::Code::Success;
#endif
    }

    return Status::Code::Success;

}

bool
Context::IsActive()
{
    vdLogOpenGLErrors("Start");

#if defined(VD_PLATFORM_OSX)        
    CGLContextObj cgl = CGLGetCurrentContext();
    return (m_Native == ((void*)cgl);
#else
    return false;
#endif
}


// ============================================================================================== //

VD_IMPLEMENT_OBJECT(Context, vd_sym(OpenGLContext), vd_sym(GraphicsContext));

// ============================================================================================== //

VD_GRAPHICS_OPENGL_NAMESPACE_END();

// ============================================================================================== //
