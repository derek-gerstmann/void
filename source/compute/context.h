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

#ifndef VD_COMPUTE_CONTEXT_INCLUDED
#define VD_COMPUTE_CONTEXT_INCLUDED

// ============================================================================================== //

#if 0
#include "compute/compute.h"
#include "compute/buffer.h"
#include "compute/image.h"
#include "compute/platform.h"
#include "compute/compiler.h"
#include "compute/program.h"
#include "compute/commands.h"
#include "compute/kernel.h"
#endif

#include "runtime/runtime.h"
#include "runtime/context.h"

// ============================================================================================== //

VD_COMPUTE_NAMESPACE_BEGIN();

// ============================================================================================== //

class Context : public Object 
{
public:

    Context(Runtime::Context* runtime);
    virtual ~Context();

    virtual void Destroy();

    virtual vd::status Flush() = 0;
    virtual vd::status Finish() = 0;

    virtual vd::status SetActive(bool v) = 0;
    virtual bool IsActive() = 0;

    Runtime::Context* GetRuntime();

    VD_DECLARE_OBJECT(Context);
    
protected:

    VD_DISABLE_COPY_CONSTRUCTORS(Context);
    
    Runtime::Context* m_Runtime;
};

// ============================================================================================== //

VD_COMPUTE_NAMESPACE_END();

// ============================================================================================== //

#endif
