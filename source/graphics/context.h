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

#ifndef VD_GRAPHICS_CONTEXT_INCLUDED
#define VD_GRAPHICS_CONTEXT_INCLUDED

// ============================================================================================== //

#include "graphics/graphics.h"
#include "graphics/buffer.h"
#include "graphics/geometry.h"
#include "graphics/framebuffer.h"
#include "graphics/viewport.h"
#include "graphics/shader.h"

#include "runtime/runtime.h"
#include "runtime/context.h"

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_BEGIN();

// ============================================================================================== //

class Context : public Object 
{
public:

    Context(Runtime::Context* runtime);
    virtual ~Context();

    virtual vd::status Setup();
    virtual vd::status Destroy();
    
    virtual vd::status Clear(
        Graphics::Viewport vp, 
        vd::f32 r=0.0f, vd::f32 g=0.0f, vd::f32 b=0.0f, vd::f32 a=0.0f,
        bool clear_color=true, bool clear_depth=true) = 0;

    virtual vd::status Flush() = 0;
    virtual vd::status Finish() = 0;

    virtual vd::status SetActive(bool v) = 0;
    virtual bool IsActive() = 0;

    virtual vd::status SetViewport(const Viewport& vp) = 0;
    virtual const Viewport& GetViewport() = 0;

    virtual vd::status BindOrthographicView() = 0;
    virtual vd::status UnbindOrthographicView() = 0;

    virtual Graphics::Geometry* CreateQuad() = 0;
    virtual Graphics::Geometry* CreateQuad(vd::f32 left, vd::f32 top, vd::f32 right, vd::f32 bottom, vd::f32 u0=0.0f, vd::f32 u1=1.0f, vd::f32 v0=0.0f, vd::f32 v1=1.0f ) = 0;
    virtual Graphics::Geometry* CreateSphere(vd::v3f32 center, vd::f32 radius, vd::i32 stacks, vd::i32 slices) = 0;
    virtual Graphics::Geometry* CreateGrid(int xres, int yres, vd::f32 fX0, vd::f32 fX1, vd::f32 fY0, vd::f32 fY1, bool vertical) = 0;
    virtual Graphics::Geometry* CreateWireGrid(int xres, int yres, vd::f32 left, vd::f32 right, vd::f32 bottom, vd::f32 top) = 0;
    virtual Graphics::Geometry* CreatePointList(vd::f32* coordinates, vd::i32 components, vd::u64 count) = 0;
    virtual Graphics::Geometry* CreatePoint(vd::f32 x, vd::f32 y, vd::f32 z) = 0;

    virtual vd::status Attach(Geometry* geo, Geometry::AttributeSlot::Value attrib, vd::u32 buffer, vd::u32 slot) = 0;
    virtual vd::status Detach(Geometry* geo, Geometry::AttributeSlot::Value attrib) = 0;

    virtual vd::status Attach(Geometry* geo, Shader* shader, Shader::Pass::Value pass) = 0;
    virtual vd::status Detach(Geometry* geo, Shader* shader, Shader::Pass::Value pass) = 0;

    virtual vd::status Bind(Geometry* geo) = 0;
    virtual vd::status Submit(Geometry* geo, Shader::Pass::Value pass = Shader::Pass::Normal, 
                              vd::u32 start=0, vd::u32 end=VD_U32_MAX, vd::u32 count=VD_U32_MAX) = 0;
    virtual vd::status Unbind(Geometry* geo) = 0;
    virtual vd::status Acquire(Geometry* buffer) = 0;
    virtual vd::status Release(Geometry* buffer) = 0;
    virtual vd::status Retain(Geometry* buffer) = 0;
    virtual vd::status Destroy(Geometry* buffer) = 0;
        
    virtual Graphics::Buffer* CreateBuffer(
        Buffer::TargetType::Value target, 
        Buffer::AttributeType::Value attrib,
        Buffer::AccessMode::Value access,
        Buffer::UpdateMode::Value update,
        Buffer::TypeId::Value datatype,
        vd::u8 components, 
        vd::u32 count, 
        const void* ptr) = 0;

    virtual vd::status Bind(Buffer* buffer) = 0;
    virtual vd::status Submit(Buffer* buffer, Geometry::PrimitiveType::Value primitives, vd::u32 offset, vd::u32 count) = 0;
    virtual vd::status Unbind(Buffer* buffer) = 0;
    virtual void* Map(Buffer* buffer, Buffer::AccessMode::Value access) = 0;
    virtual vd::status Unmap(Buffer* buffer) = 0;
    virtual vd::status Acquire(Buffer* buffer) = 0;
    virtual vd::status Release(Buffer* buffer) = 0;
    virtual vd::status Retain(Buffer* buffer) = 0;
    virtual vd::status Destroy(Buffer* buffer) = 0;

    virtual Framebuffer* 
    CreateFramebuffer(
          Graphics::Viewport viewport, 
          Graphics::ChannelLayout::Value channel_format   = Graphics::ChannelLayout::RGBA, 
           Graphics::ScalarTypeId::Value channel_datatype = Graphics::ScalarTypeId::U8, 
            Graphics::DepthFormat::Value depth_format     = Graphics::DepthFormat::None, 
           Graphics::ScalarTypeId::Value depth_datatype   = Graphics::ScalarTypeId::Invalid) = 0;

    virtual vd::status Bind(Framebuffer* fb) = 0;
    virtual vd::status Unbind(Framebuffer* fb) = 0;
    virtual vd::status Acquire(Framebuffer* fb) = 0;
    virtual vd::status Release(Framebuffer* fb) = 0;
    virtual vd::status Retain(Framebuffer* fb) = 0;
    virtual vd::status Destroy(Framebuffer* fb) = 0;
    virtual vd::status Clear(Framebuffer* fb, vd::f32 r, vd::f32 g, vd::f32 b, vd::f32 a) = 0;

    virtual Shader* CreateShaderFromSource(
        const vd::string& name, 
        const vd::string& vp, 
        const vd::string& gp, 
        const vd::string& fp) = 0;
    
    virtual Shader* CreateShaderFromFile(
        const vd::string& name, 
        const vd::string& vertex, 
        const vd::string& geometry, 
        const vd::string& fragment) = 0;

    virtual vd::status Bind(Shader* s) = 0;
    virtual vd::status Unbind(Shader* s) = 0;
    virtual vd::status Acquire(Shader* s) = 0;
    virtual vd::status Release(Shader* s) = 0;
    virtual vd::status Retain(Shader* s) = 0;
    virtual vd::status Destroy(Shader* s) = 0;
    
    virtual void RenderToScreen(
        vd::u32 uiTextureId, const Graphics::Viewport& vp, 
        vd::f32 *afData, bool bClear) = 0;

    Runtime::Context* GetRuntime();

    VD_DECLARE_OBJECT(Context);
    
protected:

    VD_DISABLE_COPY_CONSTRUCTORS(Context);
    
    Runtime::Context* m_Runtime;
};

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_END();

// ============================================================================================== //

#endif 
