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

#ifndef VD_RUNTIME_APPLICATION_INCLUDED
#define VD_RUNTIME_APPLICATION_INCLUDED

// ============================================================================================== //

#include "runtime/runtime.h"
#include "runtime/system.h"
#include "runtime/context.h"

#include "core/object.h"
#include "core/logging.h"
#include "core/framework.h"
#include "core/builtins.h"
#include "core/utilities.h"

#include "containers/buffer.h"
#include "containers/paramset.h"

// ============================================================================================== //

VD_RUNTIME_NAMESPACE_BEGIN();

// ============================================================================================== //

class Application : public Core::Object
{
public:
 
    Application();
    virtual ~Application();

    virtual vd::status Startup(int* argc = 0, void ** argv = 0);
    virtual vd::status Shutdown();
    virtual vd::status Execute() = 0;

    virtual vd::status OpenFile(const char* filename);
    virtual vd::status LoadSettings(const vd::string& filename);
    virtual vd::status LoadResources();
    virtual Runtime::Context* GetRuntime();

	VD_DECLARE_OBJECT(Application);
	
protected:

	static void SetActive(Application* app);
	static Application* GetActive();
	
	VD_DISABLE_COPY_CONSTRUCTORS(Application);

private:

    Containers::ParamSet m_ParamSet;
    Runtime::Context*    m_Runtime;
    vd::status           m_Status;
};

// ============================================================================================== //

VD_RUNTIME_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_RUNTIME_APPLICATION_INCLUDED
