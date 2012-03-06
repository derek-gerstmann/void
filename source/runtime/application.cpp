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

#include "runtime/application.h"
#include "runtime/system.h"
#include "core/symbol.h"
#include "core/handles.h"

// ============================================================================================== //

VD_RUNTIME_NAMESPACE_BEGIN();

// ============================================================================================== //
    
static Application* g_ActiveApplication;

// ============================================================================================== //

Application*  
Application::GetActive(void)
{
	return g_ActiveApplication;
}

void Application::SetActive(
	Application* app)
{
	g_ActiveApplication = app;
}
	
Application::Application()
:	Object(),
	m_Status(Status::Code::Invalid)
{
	// EMPTY!
}

Application::~Application()
{
    m_Status = Status::Code::Exiting;
}

vd::status
Application::LoadSettings(
	const vd::string& filename)
{
    return Status::Code::Success;
}

vd::status
Application::OpenFile(
	const char* filename)
{
    if(!filename)
        return Status::Code::Cancel;

    return Status::Code::Success;
}
	
vd::status
Application::LoadResources()
{
    return Status::Code::Success;
}

Runtime::Context*
Application::GetRuntime()
{
    return m_Runtime;
}

// ============================================================================================== //

vd::status
Application::Startup(int* argc, void** argv)
{
    m_Status = Status::Code::Starting;
    Core::System::Startup();
    Runtime::System::Startup();

    if(!LoadResources())
    {
        vdLogWarning("Failed to load resources!");
        return Status::Code::LoadError;
    }

    m_Runtime = System::CreateContext(m_ParamSet);
	SetActive(this);
    return Status::Code::Success;
}

vd::status
Application::Shutdown()
{
    m_Status = Status::Code::Exiting;
    SetActive(NULL);

    vdLogInfo("Shutting Down...");
    Runtime::System::Release(m_Runtime);
    Runtime::System::Shutdown();
    Core::System::Shutdown();
    return m_Status;
}

// ============================================================================================== //

VD_IMPLEMENT_OBJECT(Application, vd_sym(Application), vd_sym(Object));

// ============================================================================================== //

VD_RUNTIME_NAMESPACE_END();

// ============================================================================================== //

