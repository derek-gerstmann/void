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

#include "interface/cocoa/window.h"

#include "core/system.h"
#include "core/process.h"
#include "core/handles.h"
#include "core/threading.h"

// ============================================================================================== //

VD_INTERFACE_COCOA_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_IMPORT(Core, Status);
VD_IMPORT(Core, ScopedMutex);

VD_IMPORT(Interface, Mouse);
VD_IMPORT(Interface, Keyboard);
VD_IMPORT(Interface, Event);

// ============================================================================================== //

namespace Global 
{
	static bool IsExiting = false;
	static Cocoa::Window* ActiveWindow = NULL;
}

// ============================================================================================== //

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================================== //

#if 0
static void
vdNullCallback(void){}

static void 
vdNullMotionCallback(int x, int y) {}

static void 
vdNullMouseCallback(int button, int state, int x, int y) {}

static void 
vdNullDisplayCallback(void){}

static void 
vdNullReshapeCallback(int width, int height) {}

static void 
vdNullKeyboardCallback(unsigned char key, int x, int y) { }

static void 
vdNullEntryCallback(int state){}

static void
vdNullSpecialKeyCallback(int key, int x, int y){}
#endif

// ============================================================================================== //

#ifdef __cplusplus
}
#endif

// ============================================================================================== //

Window::Window(
	Runtime::Context* runtime
) :
	Interface::Window(runtime),
	m_Exit(false),
	m_IsOpened(false),
	m_Handle(0),
	m_FrameRateLimit(60),
	m_ClearColor(0.25, 0.25, 0.25, 0.0),
	m_Graphics(NULL)
{
	// EMPTY
}

Window::~Window()
{
	Destroy();
}

void
Window::ProcessArgs(int* argc, void ** argv)
{
//	glutInit(argc, (char**)argv);		
}

void 
Window::Setup(
	const vd::string& title,
	const Graphics::Viewport& vp,
	const Containers::ParamSet&)
{
	ScopedMutex locker(&m_Mutex);

	m_Title = title;
    m_Position = vd::v2i32(vp.X, vp.Y);
	m_Size = vd::v2i32(vp.Width, vp.Height);

/*
	glutInitDisplayMode(COCOA_RGBA | COCOA_DEPTH | COCOA_DOUBLE | COCOA_MULTISAMPLE);
	glutInitWindowPosition(vp.X, vp.Y);
	glutInitWindowSize(vp.Width, vp.Height);
	glutCreateWindow(m_Title.size() ? m_Title.c_str() : "Main Window");
	glutReshapeWindow(vp.Width, vp.Height);
	glEnable(GL_MULTISAMPLE);

	m_Handle = glutGetWindow();
*/
	m_Handle = NULL;
	m_Graphics = VD_NEW(Graphics::OpenGL::Context, GetRuntime());
	m_IsOpened = true;

	DisplayFrame();
	PostRedisplay();
}

void
Window::SetFullScreen(bool enable)
{
	ScopedMutex locker(&m_Mutex);

	if(enable)
	{
		GLint vp[4];
        glGetIntegerv(GL_VIEWPORT, vp);
        m_Position[0] = vp[0];
        m_Position[1] = vp[1];
        m_Size[0] = vp[2];
        m_Size[1] = vp[3];
//		glutFullScreen();
	}
	else
	{
//		glutReshapeWindow(m_Size.x, m_Size.y);
	}	
}	

void
Window::SetActive(bool enable)
{
	ScopedMutex locker(&m_Mutex);
	if(enable)
	{
		Global::ActiveWindow = this;	
/*
		glutSetWindow(m_Handle);
		glutDisplayFunc(vdGuiDisplayCallback);
		glutReshapeFunc(vdGuiReshapeCallback);
		glutMouseFunc(vdGuiMouseCallback);
		glutPassiveMotionFunc(vdGuiPassiveMotionCallback);
		glutMotionFunc(vdGuiMotionCallback);
		glutKeyboardFunc(vdGuiKeyboardCallback);
		glutKeyboardUpFunc(vdGuiKeyboardUpCallback);
		glutSpecialFunc(vdGuiSpecialKeyDownCallback);
		glutSpecialUpFunc(vdGuiSpecialKeyUpCallback);
		glutVisibilityFunc(vdGuiVisibilityCallback);
		glutEntryFunc(vdGuiEntryCallback);
		glutIdleFunc(vdGuiIdleCallback);
*/
	}
	else
	{
/*
		glutDisplayFunc(vdNullDisplayCallback);
		glutReshapeFunc(vdNullReshapeCallback);
		glutMouseFunc(vdNullMouseCallback);
		glutPassiveMotionFunc(vdNullMotionCallback);
		glutMotionFunc(vdNullMotionCallback);
		glutKeyboardFunc(vdNullKeyboardCallback);
		glutKeyboardUpFunc(vdNullKeyboardCallback);
		glutSpecialFunc(vdNullSpecialKeyCallback);
		glutSpecialUpFunc(vdNullSpecialKeyCallback);
		glutVisibilityFunc(vdNullEntryCallback);
		glutEntryFunc(vdNullEntryCallback);
//		glutIdleFunc(vdNullCallback);
*/
		Global::ActiveWindow = NULL;	
	}
}
	
vd::status 
Window::Destroy() 
{
	Close();
    return Core::Status::Code::Success;
}

void
Window::RemoveAllEventHandlers()
{
	m_Mutex.Lock();
	for(vd::u32 i = 0; i < Event::Type::Count; ++i)
	{
		vd::u64 kind = (vd::u64)i;
		if(m_Handlers[kind].size())
			RemoveEventHandlers(Event::Type::FromInteger(kind));
	}
	m_Mutex.Unlock();	
}	

bool 
Window::ProcessEvent( Event& event )	
{
	ScopedMutex locker(&m_Mutex);

	vd::u64 kind = (vd::u64)event.Kind;
	if(!m_Handlers[kind].empty())
	{
		EventChannel::iterator it;
		for(it = m_Handlers[kind].begin(); it != m_Handlers[kind].end(); ++it)
		{
			(*it)(event);
		}		
	}
	else if(!m_DeferredHandlers[kind].empty())
	{
		vdLogDebug("Deferring event [%d] '%s' [%d] ... ", 
			(int)event.Kind,
			Event::Type::ToString(event.Kind),
			(int)m_DeferredHandlers[kind].size());
		m_Events.push_back(event);
	}
	return true;
}
	
bool
Window::PollEvent( Event& event )
{
	ScopedMutex locker(&m_Mutex);

	if(!m_Events.empty())
	{
		event = m_Events.at(0);
		m_Events.erase(m_Events.begin());
		return true;
	}	
	return false;
}

bool
Window::AddEventHandler(
	Event::Type::Value value,  
	const Event::Callback& cb,
	bool deferred)
{
	ScopedMutex locker(&m_Mutex);

	vd::u64 kind = (vd::u64)value;

	vdLogDebug("Adding event handler for [%d] '%s' ... ", 
		(int)value,
		Event::Type::ToString(value));
	
	if(deferred)
	{
		m_DeferredHandlers[kind].push_back(cb);
	}
	else
	{
		m_Handlers[kind].push_back(cb);
	}
	return true;
}

void
Window::Flush()
{
	FlushEvents();
	m_Graphics->Flush();
	if(Global::IsExiting)
		Destroy();
}

void
Window::Finish()
{
	m_Graphics->Finish();
	if(Global::IsExiting)
		Destroy();
}

void
Window::Close()
{
	if(m_Handle == 0)
		return;

	SetActive(false);
	RemoveAllEventHandlers();
	Finish();

	m_Mutex.Lock();
//	glutDestroyWindow(m_Handle);
	m_Handle = 0;
	m_IsOpened = false;	
	m_Mutex.Unlock();
}

bool
Window::RemoveEventHandlers(
	Event::Type::Value value)
{
	ScopedMutex locker(&m_Mutex);

	vd::u64 kind = (vd::u64)value;
	if(!m_Handlers[kind].empty())
	{
		m_Handlers[kind].clear();
	}

	if(!m_DeferredHandlers[kind].empty())
	{
		m_DeferredHandlers[kind].clear();
	}
	return true;
}
	
void 
Window::DisplayFrame(void)
{
	FlushEvents();
	m_Graphics->Flush();
	SwapFrame();
}
	
void
Window::ClearFrame(void)
{
	vd::f32 r = m_ClearColor.x;	
	vd::f32 g = m_ClearColor.y;	
	vd::f32 b = m_ClearColor.z;	
	vd::f32 a = m_ClearColor.w;	

	m_Graphics->Clear(r, g, b, a);
}
	
void
Window::SwapFrame(void)
{
//	glutSwapBuffers();
}
	
void
Window::FlushEvents(void)
{
//	ScopedMutex locker(&m_Mutex);

	EventList::iterator evit;
	for(evit = m_Events.begin(); evit != m_Events.end(); ++evit)
	{
		const Event& event = (*evit);
		vd::u32 kind = Event::Type::ToInteger(event.Kind);

		vdLogInfo("Processing deferred event '%s' ... ", 
			Event::Type::ToString(event.Kind));

		EventChannel::iterator dhit;
		for(dhit = m_DeferredHandlers[kind].begin(); dhit != m_DeferredHandlers[kind].end(); ++dhit)
		{
			vd::status result = (*dhit)(event);
			Core::Status::Code::Value code = Core::Status::Code::FromInteger(result);
			if(code != Status::Code::Success)
			{
				vdLogInfo("Event handler for '%s' returned '%s' status!", 
					Event::Type::ToString(event.Kind),  
					Core::Status::Code::ToString(code));  
			}
		}		
	}
	m_Events.clear();
}

void
Window::PostRedisplay(void)
{
//	if(!Global::IsExiting)
//		glutPostRedisplay();
}

vd::i32 
Window::GetFrameRateLimit(void)
{
	return m_FrameRateLimit;
}
	
bool
Window::IsOpened(void)
{
	return m_IsOpened;
}

bool
Window::IsExiting(void)
{
	return m_Exit;
}
	
vd::status 
Window::Execute()
{
	SetActive(true);
	m_Mutex.Lock();
	{
//		atexit(vdGuiShutdown);
//	    glutPostRedisplay();
//	    glutSetWindow(m_Handle);
	}
    m_Mutex.Unlock();

//	glutMainLoop();	
	
	return Status::Code::Success;
}

void
Window::Exit()
{
//	m_Mutex.Lock();
	Global::IsExiting = true;
	m_Exit = true;
//	m_Mutex.Unlock();
}

Graphics::Context*
Window::GetGraphics()
{
	return m_Graphics;
}

Window::Config
Window::GetConfig()
{
	Config config;
	config.Title = m_Title.c_str();
	config.X = m_Position.x;
	config.Y = m_Position.y;
	config.Width = m_Size.x;
	config.Height = m_Size.y;
	config.VerticalSync = 1;
	config.MultiSampling = 16;
	return config;
}

// ============================================================================================== //

// VD_IMPLEMENT_OBJECT(Window, vd_sym(CocoaWindow), vd_sym(InterfaceWindow));

// ============================================================================================== //

VD_INTERFACE_COCOA_NAMESPACE_END();

// ============================================================================================== //



