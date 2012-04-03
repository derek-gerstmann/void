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

#include "vd.h"
#include "demo.h"
#include "symbols.h"
#include "core/symbol.h"
#include <iostream>

// ============================================================================================== //

VD_RUNTIME_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_IMPORT(Core, Symbol);
VD_IMPORT(Core, Process);

using namespace Constants::Symbols::Demo;
    
// ============================================================================================== //

Demo::Demo()
:	Application(),
	m_Window(0),
	m_Status(Status::Code::Invalid)
{
	// EMPTY
}

Demo::~Demo()
{
    VD_SAFE_DELETE(m_Gui);
}

void
Demo::DrawScene(void)
{
    ClearFrame();
}


vd::status
Demo::CreateWindow(int* argc, void** argv)
{
    Runtime::Context* runtime = GetRuntime();
    vd::string title = vd::string("Glut Demo");

    m_Window = runtime->CreateWindow(m_Params);
    m_Window->ProcessArgs(argc, argv);
    m_Window->Setup(title, m_Config.Window.Viewport, m_Params );
    m_Window->AddEventHandler(Event::Type::Display, 	VD_BIND_MEMBER_FUNCTION(this, &Demo::OnDisplay), false);
    m_Window->AddEventHandler(Event::Type::Update, 		VD_BIND_MEMBER_FUNCTION(this, &Demo::OnUpdate), false);
    m_Window->AddEventHandler(Event::Type::Closed, 		VD_BIND_MEMBER_FUNCTION(this, &Demo::OnCloseEvent), false);
    m_Window->AddEventHandler(Event::Type::Shutdown, 	VD_BIND_MEMBER_FUNCTION(this, &Demo::OnShutdown), false);
    m_Window->AddEventHandler(Event::Type::KeyDown, 	VD_BIND_MEMBER_FUNCTION(this, &Demo::OnKeyEvent), false);
    m_Window->AddEventHandler(Event::Type::KeyUp, 		VD_BIND_MEMBER_FUNCTION(this, &Demo::OnKeyEvent), false);
    m_Window->AddEventHandler(Event::Type::Resize, 		VD_BIND_MEMBER_FUNCTION(this, &Demo::OnResize), false);
    m_Window->AddEventHandler(Event::Type::MouseDown, 	VD_BIND_MEMBER_FUNCTION(this, &Demo::OnMouseDown), false);
    m_Window->AddEventHandler(Event::Type::MouseUp, 	VD_BIND_MEMBER_FUNCTION(this, &Demo::OnMouseUp), false);
    m_Window->AddEventHandler(Event::Type::MouseMoved, 	VD_BIND_MEMBER_FUNCTION(this, &Demo::OnMouseMotion), false);
    m_Window->SetFrameRateLimit(60);

    m_Gui = VD_NEW(Interface::Gw::Context, m_Window->GetGraphics());
    m_Gui->Initialize(m_Config.Window.Viewport.Width, m_Config.Window.Viewport.Height);

    m_Controls = VD_NEW(Interface::Gw::WindowControl, m_Gui->GetCanvas());

    return Status::Code::Success;
}

vd::status
Demo::LoadSettings(
    vd::string filename)
{
    (void)filename;
    m_Config.Status.CurrentTimeForFrame = 0.0;
    return Status::Code::Success;
}

vd::status
Demo::OpenDataset(
	const char* filename)
{
	vd::f64 t0, t1;

	m_Config.Commands.OpenDataset = false;
	m_Config.Status.Paused = true;

    if(filename == NULL)
	    filename = vdOpenFileDialog();

    if(!filename)
        return Status::Code::Cancel;

	vdLogInfo("Opened Dataset '%s' in '%f' sec ...", filename, t1 - t0);

	m_Config.Commands.OpenDataset = false;
    return Status::Code::Success;
}

vd::status
Demo::LoadResources()
{
    m_Config.Status.Initialised = 0;

    vd::string base = vdGetResourceFolder();
    LoadSettings("settings.txt");
    m_Config.Status.Initialised = 1;
    return Status::Code::Success;
}

void
Demo::RenderFrame()
{
    m_Config.Status.FrameCount++;

	if(m_Status != Status::Code::Running)
		return;

    if(m_Config.Status.Stale == 0 || m_Config.Status.Initialised == 0)
        return;

    if(m_Config.Commands.OpenDataset)
        return;

	Reshape(m_Config.Window.Viewport.Width, m_Config.Window.Viewport.Height);
	DrawScene();
}

void
Demo::ClearFrame()
{
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

vd::status 
Demo::Reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    return Status::Code::Success;    
}

vd::status
Demo::OnUpdate(
    const Event& event)
{
	if(m_Status != Status::Code::Running)
		return Status::Code::Reject;

    m_Gui->OnUpdate(event);

    float dt = (m_LastUpdate - event.Update.Time);
    dt = dt < 0.0f ? 0.0f : dt;
    m_LastUpdate = event.Update.Time;

    if(m_Config.Status.Initialised == 0)
        return Status::Code::Success;
    
	if(	m_Config.Commands.OpenDataset == true)
	{
		m_Config.Commands.OpenDataset = false;
		OpenDataset(NULL);
	}
	
	HandleKeys();

	if( m_Config.Status.Paused == true )
		return Status::Code::Success;
		
	if(m_Config.Status.Stale)
		m_Window->PostRedisplay();
    
    return Status::Code::Success;
}

vd::status
Demo::OnKeyEvent(
    const Event& event)
{
	if(m_Status != Status::Code::Running)
		return Status::Code::Reject;

    bool down = (event.Kind == Event::Type::KeyDown) ? true : false;
    
	m_Config.Controls.KeyDown[event.Key.Code] = down ? true : false;
	m_Config.Controls.Ctrl[event.Key.Code] = down ? event.Key.Control : false;
	m_Config.Controls.Alt[event.Key.Code] = down ? event.Key.Alt : false;
	m_Config.Controls.Shift[event.Key.Code] = down ? event.Key.Shift : false;

    if(event.Key.Code == Keyboard::KeyCode::Escape)
    {
        m_Config.Commands.Shutdown = true;
        return Status::Code::Accept;
    }

    m_Gui->OnKeyEvent(event);

    return Status::Code::Accept;
}

vd::status
Demo::HandleKeys()
{
	if(m_Status != Status::Code::Running)
		return Status::Code::Reject;

	for(vd::u32 code = 0; code < Keyboard::KeyCode::Count; ++code)
	{
		if(m_Config.Controls.KeyDown[code] == false)
			continue;
		
		m_Config.Status.Stale = true;
		switch(code)
		{
            case Keyboard::KeyCode::Escape:
            case Keyboard::KeyCode::Q:
            {
                m_Window->Exit();
                break;
            }
			case Keyboard::KeyCode::P:
			{
				m_Config.Status.Paused = (m_Config.Status.Paused == true) ? false : true;
				break;
			}
			case Keyboard::KeyCode::F:
			{        	
                bool enable = m_Config.Status.FullScreen ? false : true;
                m_Window->SetFullScreen(enable);
				m_Config.Status.FullScreen = enable;
				m_Config.Controls.KeyDown[code] = false;
				break;
			}
			default:
				break;
		}
    }

    return Status::Code::Reject;
}

void
Demo::SetWindowViewport()
{
    Graphics::Viewport vp = m_Config.Window.Viewport;
    glViewport(vp.X, vp.Y, vp.Width, vp.Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, vp.Width, vp.Height, 0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void
Demo::SetFrameViewport()
{
    Graphics::Viewport vp = m_Config.Window.Viewport;
    glViewport(vp.X, vp.Y, vp.Width, vp.Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, vp.Width, vp.Height, 0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.375, 0.375, 0);
}

vd::status
Demo::OnDisplay(
    const Event& event)
{
	if(m_Status != Status::Code::Running)
		return Status::Code::Reject;

    return Render();
}

vd::status
Demo::OnResize(
    const Event& event)
{
	if(m_Status != Status::Code::Running)
		return Status::Code::Reject;

	if(m_Config.Status.Initialised == 0)
        return Status::Code::Reject;

    vd::i32 width = event.Size.Width;
    vd::i32 height = event.Size.Height;

	vd::i32 max_dim = width > height ? width : height;

	width = max_dim;
	height = max_dim;
	
    if(width != (int)m_Config.Window.Viewport.Width || height != (int)m_Config.Window.Viewport.Height)
    {
        m_Config.Window.Viewport.Width = width;
        m_Config.Window.Viewport.Height = height;
        m_Config.Status.Stale = 1;

        SetWindowViewport();
        ClearFrame();
        m_Controls->SetSize( width, height );
        m_Gui->OnResize(event);
    }
    return Status::Code::Success;
}

vd::status
Demo::OnMouseDown(
    const Event& event)
{
    int button = event.MouseButton.Button;
    int x = event.MouseButton.X;
    int y = event.MouseButton.Y;

    vdLogInfo("MouseDown(%d,%d): %d", x, y, button);
    m_Gui->OnMouseDown(event);
    return Status::Code::Success;
}

vd::status
Demo::OnMouseUp(
    const Event& event)
{
    int button = event.MouseButton.Button;
    int x = event.MouseButton.X;
    int y = event.MouseButton.Y;
    m_Config.Controls.MouseDown[button] = false;
    m_Config.Status.Stale = 1;
    vdLogInfo("MouseUp(%d,%d): %d", x, y, button);
    m_Gui->OnMouseUp(event);
    return Status::Code::Success;
}

vd::status
Demo::OnMouseMotion(
    const Event& event)
{
    int x = event.MouseMove.X;
    int y = event.MouseMove.Y;
    if(m_Config.Controls.MouseDown[0] || m_Config.Controls.MouseDown[1] || m_Config.Controls.MouseDown[2])
    {
        m_Config.Status.Stale = 1;
	    vdLogInfo("MouseMove(%d,%d)", x, y);
    }

    m_Gui->OnMouseMotion(event);
    return Status::Code::Success;
}

vd::status
Demo::OnMouseWheel(
    const Event& event)
{
    m_Config.Status.Stale = 1;
    m_Gui->OnMouseWheel(event);
    return Status::Code::Success;
}

vd::status
Demo::OnCloseEvent(
    const Event& event)
{
	m_Config.Commands.Shutdown = true;
    m_Gui->OnCloseEvent(event);
	return Status::Code::Exiting; 
}

vd::status
Demo::OnShutdown(const Event& event)
{
    m_Gui->OnShutdown(event);
	Shutdown();
	return Status::Code::Exiting; 
}

vd::status
Demo::Render()
{
    m_Config.Status.StartTimeForFrame = Process::GetTimeInSeconds();

    m_Window->ClearFrame();
    ClearFrame();
    RenderFrame();
    m_Gui->Render();
    m_Window->DisplayFrame();

    if(m_Config.Status.LastRenderTime < Process::GetTimeInSeconds())
    {
        vdLogInfo("%d fps", (int)m_Config.Status.FrameCount);
        m_Config.Status.LastRenderTime = Process::GetTimeInSeconds() + 1.0f;
        m_Config.Status.FrameCount = 0;
    }

    return Status::Code::Success;
}

vd::status
Demo::UpdateData(void)
{
    if(m_Config.Commands.Shutdown == true)
    {
        m_Window->Exit();
        m_Window->RemoveEventHandlers(Event::Type::Display);
        m_Window->RemoveEventHandlers(Event::Type::Update);
        m_Window->RemoveEventHandlers(Event::Type::Closed);
        m_Window->RemoveEventHandlers(Event::Type::KeyDown);
        m_Window->RemoveEventHandlers(Event::Type::KeyUp);
        m_Window->RemoveEventHandlers(Event::Type::Resize);
        m_Window->RemoveEventHandlers(Event::Type::MouseDown);
        m_Window->RemoveEventHandlers(Event::Type::MouseUp);
        m_Window->RemoveEventHandlers(Event::Type::MouseMoved);
    }   
    return Status::Code::Success;
}

// ============================================================================================== //

vd::status
Demo::Execute()
{
    m_Status = Status::Code::Running;
    return m_Window->Execute();
}

vd::status
Demo::Initialize(
    int* argc, void** argv)
{
    vd::status result = Startup(argc, argv);

    if(result != Status::Code::Success)
        return result;

    return Status::Code::Success;
}

vd::status
Demo::Startup(int* argc, void** argv)
{
    m_Status = Status::Code::Starting;
    Application::Startup(argc, argv);

    m_Config.Window.Viewport.Width = 960;
    m_Config.Window.Viewport.Height = 540;

    if(!CreateWindow(argc, argv))
    {
        vdLogWarning("Failed to create window!");
        return Status::Code::StartupError;
    }

    Graphics::System::Startup();

    if(!LoadResources())
    {
        vdLogWarning("Failed to load resources!");
        return Status::Code::LoadError;
    }

	if(*argc > 1 && argv[1] != NULL)
	{
		const char* filename = (const char*)argv[1];
		OpenDataset(filename);
	}
	
	SetActive(this);
    m_Config.Status.Initialised = true;
	return Status::Code::Success;
}

vd::status
Demo::Shutdown()
{
    if(m_Status == Status::Code::Exiting)
        return Status::Code::Reject;

    m_Status = Status::Code::Exiting;
	m_Config.Status.Terminate = 1;
    m_Config.Status.Initialised = 0;
	
//    Runtime::Context* runtime = GetRuntime();
//    runtime->Release(m_Window);

    vdLogInfo("Shutting Down...");
	Application::Shutdown();	
	return Status::Code::Success;
}

// ============================================================================================== //

VD_IMPLEMENT_OBJECT(Demo, vd_sym(Demo), vd_sym(Application));

// ============================================================================================== //

VD_RUNTIME_NAMESPACE_END();

// ============================================================================================== //

