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

#ifndef VD_GRAPHICS_GL_CONTEXT_INCLUDED
#define VD_GRAPHICS_GL_CONTEXT_INCLUDED

// ============================================================================================== //

#include "graphics/gl/opengl.h"
#include "graphics/graphics.h"
#include "graphics/context.h"
#include "core/symbol.h"
#include "containers/containers.h"

// ============================================================================================== //

VD_GRAPHICS_OPENGL_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_IMPORT(Containers, Vector);
VD_IMPORT(Core, Symbol);

// ============================================================================================== //

#define vdLogOpenGLErrors(x) ReportErrors(x, VD_FUNC_ID, VD_FILE_ID, VD_LINE_ID)

// ============================================================================================== //

class Context : public Graphics::Context 
{
public:
    
    typedef Vector< Graphics::Buffer* >::type         BufferData;
    typedef Vector< Graphics::Texture* >::type        TextureData;
    typedef Vector< Graphics::Framebuffer* >::type    FramebufferData;
    typedef Vector< Graphics::Geometry* >::type       GeometryData;
    typedef Vector< Graphics::Shader* >::type         ShaderData;
	
public:
	
	Context(Runtime::Context* runtime);
	virtual ~Context();
    
    virtual vd::status Destroy();

    virtual vd::status SetActive(bool v);
    virtual bool IsActive();

    virtual vd::status Clear(vd::f32 r=0.0f, vd::f32 g=0.0f, vd::f32 b=0.0f, vd::f32 a=0.0f);
    virtual vd::status Flush();
    virtual vd::status Finish();

	virtual vd::status SetViewport(const Viewport& vp);
    virtual const Viewport& GetViewport();

    virtual vd::status BindOrthographicView();
    virtual vd::status UnbindOrthographicView();

    virtual Graphics::Geometry* CreateQuad();
    virtual Graphics::Geometry* CreateQuad(vd::f32 left, vd::f32 top, vd::f32 right, vd::f32 bottom, vd::f32 u0, vd::f32 u1, vd::f32 v0, vd::f32 v1 );
    virtual Graphics::Geometry* CreateGrid(int xres, int yres, vd::f32 fX0, vd::f32 fX1, vd::f32 fY0, vd::f32 fY1, bool vertical);
    virtual Graphics::Geometry* CreateWireGrid(int xres, int yres, vd::f32 left, vd::f32 right, vd::f32 bottom, vd::f32 top);
    virtual Graphics::Geometry* CreatePointList(vd::f32* coordinates, vd::i32 components, vd::u64 count);
    virtual Graphics::Geometry* CreatePoint(vd::f32 x, vd::f32 y, vd::f32 z);

    virtual vd::status Attach(Geometry* geo, Geometry::AttributeSlot::Value attrib, vd::u32 buffer, vd::u32 slot);
    virtual vd::status Detach(Geometry* geo, Geometry::AttributeSlot::Value attrib);
    
    virtual vd::status Attach(Geometry* geo, Shader* shader);
    virtual vd::status Detach(Geometry* geo, Shader* shader);

    virtual vd::status Bind(Geometry* geo);
    virtual vd::status Submit(Geometry* geo, vd::u32 start=0, vd::u32 end=VD_U32_MAX, vd::u32 count=VD_U32_MAX);
    virtual vd::status Unbind(Geometry* geo);
    virtual vd::status Release(Geometry* geo);
        
    virtual Graphics::Buffer* CreateBuffer(
        Buffer::TargetType::Value target, 
        Buffer::AttributeType::Value attrib,
        Buffer::AccessMode::Value access,
        Buffer::UsageMode::Value usage,
        Buffer::TypeId::Value datatype,
        vd::u8 components, 
        vd::u32 count, 
        const void* ptr=NULL);

    virtual vd::status Bind(Buffer* buffer);
    virtual vd::status Unbind(Buffer* buffer);
    virtual vd::status Submit(Buffer* buffer, Geometry::PrimitiveType::Value primitives, vd::u32 offset, vd::u32 count);
    virtual void* Map(Buffer* buffer, Buffer::AccessMode::Value access);
    virtual vd::status Unmap(Buffer* buffer);
    virtual vd::status Release(Buffer* buffer);

    virtual Framebuffer* CreateFramebuffer(
        vd::u32 width, vd::u32 height, 
        Graphics::ChannelFormat::Order::Value format = Graphics::ChannelFormat::Order::RGBA, 
        Graphics::ScalarTypeId::Value datatype = Graphics::ScalarTypeId::U8, 
        bool depth=false);

    virtual vd::status Bind(Framebuffer* fb);
    virtual vd::status Unbind(Framebuffer* fb);
    virtual vd::status Release(Framebuffer* fb);
    virtual vd::status Clear(Framebuffer* fb, vd::f32 r=0.0f, vd::f32 g=0.0f, vd::f32 b=0.0f, vd::f32 a=0.0f);
	
    virtual Shader* CompileShaderFromFile(const vd::string& name, const vd::string& vertex, const vd::string& geometry, const vd::string& fragment);
    virtual vd::status Bind(Shader* s);
    virtual vd::status Unbind(Shader* s);
    virtual vd::status Release(Shader* s);

    Runtime::Context* GetRuntime();
    
    void RenderToScreen(
        vd::u32 uiTextureId, const Graphics::Viewport& vp, 
        vd::f32 *afData, bool bClear);

    void ReportErrors(
        const char* msg,
        const char *func, 
        const char *filename, 
        int linenum);

    VD_DECLARE_OBJECT(Context);

protected:

    virtual vd::status Bind(Geometry::AttributeSlot::Value attrib, Buffer* buffer, vd::u32 slot);
    virtual vd::status Unbind(Geometry::AttributeSlot::Value attrib, Buffer* buffer, vd::u32 slot);

protected:

    void*               m_Native;
    Viewport            m_Viewport;
    BufferData          m_Buffers;
    GeometryData        m_Geometry;
    TextureData         m_Textures;
    FramebufferData     m_Framebuffers;
    ShaderData          m_Shaders;
};

// ============================================================================================== //

VD_GRAPHICS_OPENGL_NAMESPACE_END();

// ============================================================================================== //

#endif 
