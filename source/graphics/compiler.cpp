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

#include "graphics/compiler.h"
#include "graphics/context.h"
#include "core/memory.h"
#include "constants/values.h"

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_BEGIN();

// ============================================================================================== //

Compiler::Compiler(
    Graphics::Context* ctx) :
    Object(),
    m_Context(ctx)
{
}

Compiler::~Compiler()
{
    Destroy();
}

vd::status
Compiler::Reset()
{
    Destroy();
    return Status::Code::Success;
}

vd::status 
Compiler::Acquire()
{
    return Status::Code::Success;
}

vd::status 
Compiler::Release()
{
    return Status::Code::Success;
}

vd::status 
Compiler::Destroy()
{
//    if(m_Context)
//        m_Context->Release(this);

    return Status::Code::Success;
}

// ============================================================================================== //

VD_IMPLEMENT_OBJECT(Compiler, vd_sym(Compiler), vd_sym(Object));

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_END();

// ============================================================================================== //

