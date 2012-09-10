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

#include "graphics/geometry.h"
#include "graphics/context.h"
#include "core/memory.h"
#include "constants/values.h"

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_BEGIN();

// ============================================================================================== //

vd::u32 Geometry::InvalidBuffer = Constants::InvalidIndex;
vd::u32 Geometry::InvalidSlot = Constants::InvalidIndex;

// ============================================================================================== //

Geometry::Geometry(
    Context* ctx) :
    Object(),
    m_Context(ctx)
{
    Reset();
}

Geometry::~Geometry()
{
    Destroy();
}

void
Geometry::Reset()
{
    Core::Memory::SetBytes(&m_Data, 0, sizeof(m_Data));
    Core::Memory::SetBytes(m_Data.Shaders,  Constants::InvalidIndex, sizeof(m_Data.Shaders));
    Core::Memory::SetBytes(m_Data.Buffers,  Constants::InvalidIndex, sizeof(m_Data.Buffers));
    Core::Memory::SetBytes(m_Data.Bindings, Constants::InvalidIndex, sizeof(m_Data.Bindings));
}

void
Geometry::Setup(
    const Geometry::Data& data)
{
    if(&m_Data == &data)
        return;

    Destroy();
    Core::Memory::CopyBytes(&m_Data, &data, sizeof(m_Data));
}

vd::status 
Geometry::Acquire()
{
    return Status::Code::Success;
}

vd::status 
Geometry::Release()
{
    m_Data.Index = Constants::InvalidIndex;
    return Status::Code::Success;
}

vd::status 
Geometry::Retain()
{
    return Status::Code::Success;
}

vd::status 
Geometry::Destroy()
{
    if(m_Context)
        m_Context->Release(this);

    Reset();
    m_Data.Index = Constants::InvalidIndex;    
    return Status::Code::Success;
}

const Geometry::Data&
Geometry::GetData() const
{
    return m_Data;
}

const Geometry::Data*
Geometry::GetPtr() const
{
    return &m_Data;
}

vd::status 
Geometry::Attach(
    AttributeSlot::Value attrib,
    vd::u32 buffer,
    vd::u32 slot)
{
    vd::u32 index = AttributeSlot::ToInteger(attrib);
    m_Data.Buffers[index] = buffer;
    m_Data.Bindings[index] = slot;
    return Status::Code::Success;
}

vd::status 
Geometry::Attach(
    Shader::Pass::Value pass,
    vd::u32 shader)
{
    vd::u32 index = Shader::Pass::ToInteger(pass);
    m_Data.Shaders[index] = shader;
    return Status::Code::Success;
}

vd::status 
Geometry::Detach(
    AttributeSlot::Value attrib)
{
    vd::u32 index = AttributeSlot::ToInteger(attrib);
    m_Data.Buffers[index] = Geometry::InvalidBuffer;    
    m_Data.Bindings[index] = Geometry::InvalidSlot;    
    return Status::Code::Success;
}

vd::status 
Geometry::Detach(
    Shader::Pass::Value pass)
{
    vd::u32 index = Shader::Pass::ToInteger(pass);
    m_Data.Shaders[index] = Geometry::InvalidSlot;
    return Status::Code::Success;
}

bool 
Geometry::IsActive()
{
    return m_Data.Usage > 0;
}

bool
Geometry::IsAttributeUsed(AttributeSlot::Value attrib)
{
    if(m_Data.Bindings[attrib] < Geometry::InvalidSlot)
    {
        return true;
    }
    return false;
}

bool
Geometry::IsBufferUsed(AttributeSlot::Value attrib)
{
    if(m_Data.Buffers[attrib] < Geometry::InvalidBuffer)
    {
        return true;
    }
    return false;    
}

void
Geometry::Bind()
{
    m_Data.Usage++;
}

void
Geometry::Unbind()
{
    vdAssert(m_Data.Usage > 0);
    m_Data.Usage--;
}

vd::u32 
Geometry::GetBuffer(AttributeSlot::Value attrib)
{
    return m_Data.Buffers[attrib];
}

vd::u32 
Geometry::GetBinding(AttributeSlot::Value attrib)
{
    return m_Data.Bindings[attrib];
}

// ============================================================================================== //

VD_IMPLEMENT_OBJECT(Geometry, vd_sym(Geometry), vd_sym(Object));

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_END();

// ============================================================================================== //

