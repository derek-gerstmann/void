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

#ifndef VD_RUNTIME_SYSTEM_INCLUDED
#define VD_RUNTIME_SYSTEM_INCLUDED

// ============================================================================================== //

#include "runtime/runtime.h"
#include "runtime/context.h"
#include "core/paramset.h"

// ============================================================================================== //

VD_RUNTIME_NAMESPACE_BEGIN();

// ============================================================================================== //

class System 
{
public:
    static bool Startup(int* argc=NULL, void** argv=NULL);
    static bool Shutdown(void);

    static Context* CreateContext(const Core::ParamSet& params);
    static void Release(Context* context);
        
//    static Graphics::Context* CreateGraphicsContext(const Core::ParamSet& params);
//    static Storage::Context* CreateStorageContext(const Core::ParamSet& params);
//    static Compute::Context* CreateComputeContext(const Core::ParamSet& params);
//    static Render::Context* CreateRenderContext(const Core::ParamSet& params);
//    static Interface::Context* CreateInterfaceContext(const Core::ParamSet& params);

};

// ============================================================================================== //

VD_RUNTIME_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_RUNTIME_SYSTEM_INCLUDED
