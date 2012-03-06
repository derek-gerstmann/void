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

#include "graphics/framebuffer.h"
#include "graphics/context.h"
#include "core/memory.h"

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_BEGIN();

// ============================================================================================== //

Framebuffer::Framebuffer(
    Context* ctx) :
    Object(),
    m_Context(ctx)
{
    Core::Memory::MemSet(&m_Data, 0, sizeof(m_Data));
}

Framebuffer::~Framebuffer()
{
    Destroy();
}

void
Framebuffer::Reset()
{
    Destroy();
    Core::Memory::MemSet(&m_Data, 0, sizeof(m_Data));
}

void
Framebuffer::Setup(
    const Framebuffer::Data& data)
{
    Core::Memory::MemCopy(&m_Data, &data, sizeof(m_Data));
}

vd::status 
Framebuffer::Destroy()
{
    if(m_Context)
        m_Context->Release(this);
    return Status::Code::Success;
}

const Framebuffer::Data&
Framebuffer::GetData() const
{
    return m_Data;
}

void
Framebuffer::SetActive(bool b)
{
    m_Data.IsActive = b;
}

bool 
Framebuffer::IsActive()
{
    return m_Data.IsActive;
}

// ============================================================================================== //

VD_IMPLEMENT_OBJECT(Framebuffer, vd_sym(Framebuffer), vd_sym(Object));

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_END();

// ============================================================================================== //

