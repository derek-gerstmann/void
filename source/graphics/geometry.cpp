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

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_BEGIN();

// ============================================================================================== //

vd::u32 Geometry::InvalidBuffer = VD_U32_MAX;
vd::u32 Geometry::InvalidSlot = VD_U32_MAX;

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
    Core::Memory::MemSet(&m_Data, 0, sizeof(m_Data));
    Core::Memory::MemSet(&(m_Data.Buffers), Geometry::InvalidBuffer, sizeof(m_Data.Buffers));
    Core::Memory::MemSet(&(m_Data.Bindings), Geometry::InvalidSlot, sizeof(m_Data.Bindings));
}

void
Geometry::Setup(
    const Geometry::Data& data)
{
    Core::Memory::MemCopy(&m_Data, &data, sizeof(m_Data));
}

vd::status 
Geometry::Destroy()
{
    if(m_Context)
        m_Context->Release(this);

    Reset();
    return Core::Status::Code::Success;
}

const Geometry::Data&
Geometry::GetData() const
{
    return m_Data;
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
    return Core::Status::Code::Success;
}

vd::status 
Geometry::Detach(
    AttributeSlot::Value attrib)
{
    vd::u32 index = AttributeSlot::ToInteger(attrib);
    m_Data.Buffers[index] = Geometry::InvalidBuffer;    
    m_Data.Bindings[index] = Geometry::InvalidSlot;    
    return Core::Status::Code::Success;
}

void
Geometry::SetActive(bool b)
{
    m_Data.IsActive = b;
}

bool 
Geometry::IsActive()
{
    return m_Data.IsActive;
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

