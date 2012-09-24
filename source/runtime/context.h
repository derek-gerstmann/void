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

#ifndef VD_RUNTIME_CONTEXT_INCLUDED
#define VD_RUNTIME_CONTEXT_INCLUDED

// ============================================================================================== //

#include "runtime/runtime.h"

#include "core/object.h"
#include "core/paramset.h"
#include "core/filesystem.h"

#include "interface/interface.h"
#include "interface/window.h"

#include "graphics/viewport.h"

// ============================================================================================== //

VD_RUNTIME_NAMESPACE_BEGIN();

// ============================================================================================== //

class Context : public Core::Object 
{
public:

    Context();
    virtual ~Context();
    virtual vd::status Destroy();
    virtual vd::status Setup(const Core::ParamSet& params);
    virtual Core::FileSystem* GetFileSystem();
    virtual Interface::Window* CreateWindow(const Core::ParamSet& params);
    virtual vd::status Release(Interface::Window* w);
    virtual vd::status Lock();
    virtual vd::status Unlock();
    virtual Core::Mutex* GetLockPtr();

    VD_DECLARE_OBJECT(Context);
    
protected:

    VD_DISABLE_COPY_CONSTRUCTORS(Context);

    Core::FileSystem*   m_FileSystem;
    Core::Mutex         m_Mutex;

};

// ============================================================================================== //

VD_RUNTIME_NAMESPACE_END();

// ============================================================================================== //

#endif 
