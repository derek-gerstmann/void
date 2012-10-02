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

#include "runtime/context.h"

#if defined(VD_USE_GLUT_RUNTIME)
#include "interface/glut/window.h"
#endif

#if defined(VD_USE_GLFW_RUNTIME)
#include "interface/glfw/window.h"
#endif

// ============================================================================================== //

VD_RUNTIME_NAMESPACE_BEGIN();

// ============================================================================================== //

Context::Context() :
    Object()
{
    
}

Context::~Context()
{
    Destroy();
}

vd::status
Context::Destroy()
{
    if(m_FileSystem)
        m_FileSystem->Close();
    
    VD_DELETE(m_FileSystem);     
    return Status::Code::Success;   
}

vd::status 
Context::Setup(
    const Core::ParamSet& params)
{
    m_FileSystem = VD_NEW(Core::FileSystem);
    m_FileSystem->Open();
    return Status::Code::Success;
}

Core::FileSystem*
Context::GetFileSystem()
{
    return m_FileSystem;
}

Interface::Window* 
Context::CreateWindow(
    const Core::ParamSet& params)
{
    Interface::Window* window = NULL;
#if defined(VD_USE_GLUT_RUNTIME)
    if(params.Get1b(vd_sym(UseGlut)) == true)
        window = VD_NEW(Interface::Glut::Window, this);
#endif
#if defined(VD_USE_GLFW_RUNTIME)
    if(params.Get1b(vd_sym(UseGlfw)) == true)
        window = VD_NEW(Interface::Glfw::Window, this);
#endif
    
    return window;    
}

vd::status 
Context::Release(Interface::Window* w)
{
    VD_SAFE_DELETE(w);
    return Status::Code::Success;
}

vd::status
Context::Lock(void)
{
    m_Mutex.Lock();
    return Status::Code::Success;
}

vd::status
Context::Unlock(void)
{
    m_Mutex.Unlock();
    return Status::Code::Success;
}

Core::Mutex*
Context::GetLockPtr(void)
{
    return &m_Mutex;
}

// ============================================================================================== //

VD_IMPLEMENT_OBJECT(Context, vd_sym(RuntimeContext), vd_sym(Object));

// ============================================================================================== //

VD_RUNTIME_NAMESPACE_END();

// ============================================================================================== //
