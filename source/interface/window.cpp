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

#include "interface/window.h"

#include "core/system.h"
#include "core/process.h"
#include "core/handles.h"
#include "core/threading.h"

// ============================================================================================== //

VD_INTERFACE_NAMESPACE_BEGIN();

// ============================================================================================== //

Window::Window(
    Runtime::Context* runtime
) :
    Core::Object(),
    m_Runtime(runtime)
{
    // EMPTY
}

Window::~Window()
{
    Destroy();
}

Runtime::Context*
Window::GetRuntime()
{
    return m_Runtime;
}

vd::status 
Window::Destroy()
{
    return Status::Code::Success;
}

// ============================================================================================== //

VD_IMPLEMENT_OBJECT(Window, vd_sym(InterfaceWindow), vd_sym(Object));

// ============================================================================================== //

VD_INTERFACE_NAMESPACE_END();

// ============================================================================================== //



