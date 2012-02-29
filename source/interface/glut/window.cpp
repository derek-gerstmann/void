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

#include "interface/glut/window.h"

#include "core/system.h"
#include "core/process.h"
#include "core/handles.h"
#include "core/threading.h"

// ============================================================================================== //

VD_INTERFACE_GLUT_NAMESPACE_BEGIN();

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
	static Glut::Window* ActiveWindow = NULL;
}

// ============================================================================================== //

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================================== //

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

// ============================================================================================== //

static Keyboard::KeyCode::Value
vdGuiTranslateGlutAsciiKey(int code)
{
	switch(code)
	{
		case 8:				{ return Keyboard::KeyCode::Delete;  }
		case 9:				{ return Keyboard::KeyCode::Tab;  }
		case 10:			{ return Keyboard::KeyCode::Return;  }
		case 13:			{ return Keyboard::KeyCode::Return;  }
		case 27:			{ return Keyboard::KeyCode::Escape;  }
		case '0':			{ return Keyboard::KeyCode::Num0;  }
		case '1':			{ return Keyboard::KeyCode::Num1;  }
		case '2':			{ return Keyboard::KeyCode::Num2;  }
		case '3':			{ return Keyboard::KeyCode::Num3;  }
		case '4':			{ return Keyboard::KeyCode::Num4;  }
		case '5':			{ return Keyboard::KeyCode::Num5;  }
		case '6':			{ return Keyboard::KeyCode::Num6;  }
		case '7':			{ return Keyboard::KeyCode::Num7;  }
		case '8':			{ return Keyboard::KeyCode::Num8;  }
		case '9':			{ return Keyboard::KeyCode::Num9;  }
		case 'a':			{ return Keyboard::KeyCode::A;  }
		case 'A':			{ return Keyboard::KeyCode::A;  }
		case 'b':
		case 'B':			{ return Keyboard::KeyCode::B;  }
		case 'c':
		case 'C':			{ return Keyboard::KeyCode::C;  }
		case 'd':
		case 'D':			{ return Keyboard::KeyCode::D;  }
		case 'e':
		case 'E':			{ return Keyboard::KeyCode::E;  }
		case 'f':
		case 'F':			{ return Keyboard::KeyCode::F;  }
		case 'g':
		case 'G':			{ return Keyboard::KeyCode::G;  }
		case 'h':
		case 'H':			{ return Keyboard::KeyCode::H;  }
		case 'i':
		case 'I':			{ return Keyboard::KeyCode::I;  }
		case 'j':
		case 'J':			{ return Keyboard::KeyCode::J;  }
		case 'k':			{ return Keyboard::KeyCode::K;  }
		case 'K':			{ return Keyboard::KeyCode::K;  }
		case 'l':			{ return Keyboard::KeyCode::L;  }
		case 'L':			{ return Keyboard::KeyCode::L;  }
		case 'm':
		case 'M':			{ return Keyboard::KeyCode::M;  }
		case 'n':
		case 'N':			{ return Keyboard::KeyCode::N;  }
		case 'o':
		case 'O':			{ return Keyboard::KeyCode::O;  }
		case 'p':
		case 'P':			{ return Keyboard::KeyCode::P;  }
		case 'q':
		case 'Q':			{ return Keyboard::KeyCode::Q;  }
		case 'r':
		case 'R':			{ return Keyboard::KeyCode::R;  }
		case 's':
		case 'S':			{ return Keyboard::KeyCode::S;  }
		case 't':
		case 'T':			{ return Keyboard::KeyCode::T;  }
		case 'u':
		case 'U':			{ return Keyboard::KeyCode::U;  }
		case 'v':
		case 'V':			{ return Keyboard::KeyCode::V;  }
		case 'w':
		case 'W':			{ return Keyboard::KeyCode::W;  }
		case 'x':
		case 'X':			{ return Keyboard::KeyCode::X;  }
		case 'y':
		case 'Y':			{ return Keyboard::KeyCode::Y;  }
		case 'z':
		case 'Z':			{ return Keyboard::KeyCode::Z;  }
		case '~':			{ return Keyboard::KeyCode::Tilde;  }
		case '@':			{ return Keyboard::KeyCode::AtSign;  }
		case '#':			{ return Keyboard::KeyCode::NumSign;  }
		case '$':			{ return Keyboard::KeyCode::DollarSign;  }
		case '%':			{ return Keyboard::KeyCode::PercentSign;  }
		case '^':			{ return Keyboard::KeyCode::UpCarat;  }
		case '&':			{ return Keyboard::KeyCode::Ampersand;  }
		case '*':			{ return Keyboard::KeyCode::Asterisk;  }
		case '(':			{ return Keyboard::KeyCode::LeftParen;  }
		case ')':			{ return Keyboard::KeyCode::RightParen;  }
		case '=':			{ return Keyboard::KeyCode::EqualSign;  }
		case '-':			{ return Keyboard::KeyCode::MinusSign;  }
		case '+':			{ return Keyboard::KeyCode::PlusSign;  }
		case '[':			{ return Keyboard::KeyCode::LeftBracket;  }
		case ']':			{ return Keyboard::KeyCode::RightBracket;  }
		case '{':			{ return Keyboard::KeyCode::LeftBrace;  }
		case '}':			{ return Keyboard::KeyCode::RightBrace;  }
		default:			{ return Keyboard::KeyCode::FromInteger(code);  }
	};

	return Keyboard::KeyCode::FromInteger(code);
}

static Keyboard::KeyCode::Value
vdGuiTranslateGlutSpecialKey(int code)
{
	switch(code)
	{
		case GLUT_KEY_F1:		{ return Keyboard::KeyCode::F1;  }
		case GLUT_KEY_F2:		{ return Keyboard::KeyCode::F2;  }
		case GLUT_KEY_F3:		{ return Keyboard::KeyCode::F3;  }
		case GLUT_KEY_F4:		{ return Keyboard::KeyCode::F4;  }
		case GLUT_KEY_F5:		{ return Keyboard::KeyCode::F5;  }
		case GLUT_KEY_F6:		{ return Keyboard::KeyCode::F6;  }
		case GLUT_KEY_F7:		{ return Keyboard::KeyCode::F7;  }
		case GLUT_KEY_F8:		{ return Keyboard::KeyCode::F8;  }
		case GLUT_KEY_F9:		{ return Keyboard::KeyCode::F9;  }
		case GLUT_KEY_F10:		{ return Keyboard::KeyCode::F10;  }
		case GLUT_KEY_F11:		{ return Keyboard::KeyCode::F11;  }
		case GLUT_KEY_F12:		{ return Keyboard::KeyCode::F12;  }
		case GLUT_KEY_UP:		{ return Keyboard::KeyCode::Up;  }
		case GLUT_KEY_DOWN:		{ return Keyboard::KeyCode::Down;  }
		case GLUT_KEY_LEFT:		{ return Keyboard::KeyCode::Left;  }
		case GLUT_KEY_RIGHT:	{ return Keyboard::KeyCode::Right;  }
		case GLUT_KEY_HOME:		{ return Keyboard::KeyCode::Home;  }
		case GLUT_KEY_END:		{ return Keyboard::KeyCode::End;  }
		case GLUT_KEY_INSERT:	{ return Keyboard::KeyCode::Insert;  }
		default:				{ return Keyboard::KeyCode::Invalid;  }
	};

	return Keyboard::KeyCode::Invalid;
}

static void
vdGuiShutdown()
{
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
	glutIdleFunc(vdNullCallback);

	if(Global::ActiveWindow)
	{	
		Event event;
		event.Kind = Event::Type::Shutdown;
		event.Update.Time = Core::Process::GetTimeInSeconds();
		Global::ActiveWindow->ProcessEvent( event );
		Global::ActiveWindow = NULL;
	}
}

static void 
vdGuiMotionCallback(int x, int y)
{
	static int dx = 0;
	static int dy = 0;
	
	dx = x - dx;
	dy = y - dy;
	
	Event event;
	event.MouseMove.X = x;
	event.MouseMove.Y = y;
	event.Kind = Event::Type::MouseMoved;
	
	dx = x;
	dy = y;
	
	if(Global::ActiveWindow) 
		Global::ActiveWindow->ProcessEvent( event );
}

static void 
vdGuiMouseCallback(int button, int state, int x, int y)
{
	Event event;
	Mouse::Button::Value value = Mouse::Button::Left;
	
	switch(button)
	{
		case 0:		{ value = Mouse::Button::Left; break; }
		case 1:		{ value = Mouse::Button::Right; break; }
		case 2:		{ value = Mouse::Button::Middle; break; }
		case 3:		{ value = Mouse::Button::X1; break; }
		case 4:		{ value = Mouse::Button::X2; break; }
		case 5:		{ value = Mouse::Button::X3; break; }
		case 6:		{ value = Mouse::Button::X4; break; }
		case 7:		{ value = Mouse::Button::X5; break; }
		case 8:		{ value = Mouse::Button::X6; break; }
		case 9:		{ value = Mouse::Button::X7; break; }
		case 10:	{ value = Mouse::Button::X8; break; }
		default:	break;
	};

	event.MouseButton.Button = value;
	event.MouseButton.X = x;
	event.MouseButton.Y = y;

    if (state == GLUT_DOWN)
		event.Kind = Event::Type::MouseDown;
    else
    	event.Kind = Event::Type::MouseUp;

	if(Global::ActiveWindow) 
		Global::ActiveWindow->ProcessEvent( event );
}

static void 
vdGuiDisplayCallback(void)
{
	if(Global::IsExiting)
		return;

	if(Global::ActiveWindow) 
	{
		Event event;
		event.Kind = Event::Type::Display;
		event.Update.Time = Core::Process::GetTimeInSeconds();
		Global::ActiveWindow->ProcessEvent( event );
	}	
}

static void 
vdGuiReshapeCallback(int width, int height)
{
	Event event;
	event.Size.Width = width;
	event.Size.Height = height;
	event.Kind = Event::Type::Resize;
	if(Global::ActiveWindow) 
		Global::ActiveWindow->ProcessEvent( event );
}

static void 
vdGuiKeyboardCallback(unsigned char key, int x, int y)
{
	Event event;

	bool ctrl = glutGetModifiers() & GLUT_ACTIVE_CTRL ? true : false;
	bool shift = glutGetModifiers() & GLUT_ACTIVE_SHIFT ? true : false;
	bool alt = glutGetModifiers() & GLUT_ACTIVE_ALT ? true : false;
	
	event.Kind = Event::Type::KeyDown;
	event.Key.Control = ctrl;
	event.Key.Alt = alt;
	event.Key.Shift = shift;
	event.Key.Code = vdGuiTranslateGlutAsciiKey(key);
	
	if(key >= 'A' && key <= 'Z')
		event.Key.Shift = true;
	else if(key >= 'a' && key <= 'z')
		event.Key.Shift = false;
	
	if(Global::ActiveWindow) 
		Global::ActiveWindow->ProcessEvent( event );
}

static void 
vdGuiKeyboardUpCallback(unsigned char key, int x, int y)
{
	Event event;
	bool ctrl = glutGetModifiers() & GLUT_ACTIVE_CTRL ? true : false;
	bool shift = glutGetModifiers() & GLUT_ACTIVE_SHIFT ? true : false;
	bool alt = glutGetModifiers() & GLUT_ACTIVE_ALT ? true : false;
	
	event.Kind = Event::Type::KeyUp;
	event.Key.Control = ctrl;
	event.Key.Alt = alt;
	event.Key.Shift = shift;
	event.Key.Code = vdGuiTranslateGlutAsciiKey(key);

	if(key >= 'A' && key <= 'Z')
		event.Key.Shift = true;
	else if(key >= 'a' && key <= 'z')
		event.Key.Shift = false;

	if(Global::ActiveWindow) 
		Global::ActiveWindow->ProcessEvent( event );
}

static void 
vdGuiPassiveMotionCallback(int x, int y)
{
	Event event;
	event.Kind = Event::Type::MouseMoved;
	event.MouseMove.X = x;
	event.MouseMove.Y = y;
	if(Global::ActiveWindow) 
		Global::ActiveWindow->ProcessEvent( event );
}

static void 
vdGuiEntryCallback(int state)
{
	Event event;
	if(state == GLUT_ENTERED)
		event.Kind = Event::Type::MouseEntered;
	else if(state == GLUT_LEFT)
		event.Kind = Event::Type::MouseExited;
	if(Global::ActiveWindow) 
		Global::ActiveWindow->ProcessEvent( event );
}

static void
vdGuiSpecialKeyDownCallback(int key, int x, int y)
{
	Event event;
	bool ctrl = glutGetModifiers() & GLUT_ACTIVE_CTRL ? true : false;
	bool shift = glutGetModifiers() & GLUT_ACTIVE_SHIFT ? true : false;
	bool alt = glutGetModifiers() & GLUT_ACTIVE_ALT ? true : false;
	
	event.Kind = Event::Type::KeyDown;
	event.Key.Control = ctrl;
	event.Key.Alt = alt;
	event.Key.Shift = shift;
	event.Key.Code = vdGuiTranslateGlutSpecialKey(key);

	if(Global::ActiveWindow) Global::ActiveWindow->ProcessEvent( event );
}

static void
vdGuiSpecialKeyUpCallback(int key, int x, int y)
{
	Event event;
	bool ctrl = glutGetModifiers() & GLUT_ACTIVE_CTRL ? true : false;
	bool shift = glutGetModifiers() & GLUT_ACTIVE_SHIFT ? true : false;
	bool alt = glutGetModifiers() & GLUT_ACTIVE_ALT ? true : false;

	event.Kind = Event::Type::KeyDown;
	event.Key.Control = ctrl;
	event.Key.Alt = alt;
	event.Key.Shift = shift;
	event.Key.Code = vdGuiTranslateGlutSpecialKey(key);

	if(Global::ActiveWindow) Global::ActiveWindow->ProcessEvent( event );
}

static void 
vdGuiVisibilityCallback(int state)
{
	Event event;

	if(state == GLUT_NOT_VISIBLE)
	{
		event.Kind = Event::Type::LostFocus;
	}
	else if(state == GLUT_VISIBLE)
	{
		event.Kind = Event::Type::GainedFocus;
	
	}
	if(Global::ActiveWindow) Global::ActiveWindow->ProcessEvent( event );
}

static void
vdGuiIdleCallback(void)
{	
	static vd::u64 IdleCallbackCount = 0;
	static double LastCallbackTime = 0;
	
	if(Global::IsExiting)
	{
		vdGuiShutdown();
		exit(0);
		return;
	}
		
	if(Global::ActiveWindow->GetFrameRateLimit() < 1 || LastCallbackTime < Core::Process::GetTimeInSeconds())
    {
		Event event;
		event.Kind = Event::Type::Update;
		event.Update.Time = Core::Process::GetTimeInSeconds();
		if(Global::ActiveWindow)
		{
			Global::ActiveWindow->ProcessEvent( event );
			LastCallbackTime = Core::Process::GetTimeInSeconds() + 1.0f / Global::ActiveWindow->GetFrameRateLimit();
			IdleCallbackCount = 0;
		}
    }
}


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
	glutInit(argc, (char**)argv);		
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

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutInitWindowPosition(vp.X, vp.Y);
	glutInitWindowSize(vp.Width, vp.Height);
	glutCreateWindow(m_Title.size() ? m_Title.c_str() : "Main Window");
	glutReshapeWindow(vp.Width, vp.Height);
	glEnable(GL_MULTISAMPLE);

	m_Handle = glutGetWindow();
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
        m_Size[0] = vp[2];
        m_Size[1] = vp[3];
		glutFullScreen();
	}
	else
	{
		glutReshapeWindow(m_Size.x, m_Size.y);
	}	
}	

void
Window::SetActive(bool enable)
{
	ScopedMutex locker(&m_Mutex);
	if(enable)
	{
		Global::ActiveWindow = this;	
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
	}
	else
	{
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
	glutDestroyWindow(m_Handle);
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
	glutSwapBuffers();
}
	
void
Window::FlushEvents(void)
{
//	ScopedMutex locker(&m_Mutex);

	EventList::iterator evit;
	for(evit = m_Events.begin(); evit != m_Events.end(); ++evit)
	{
		const Event& event = (*evit);
		vd::u64 kind = (vd::u64)event.Kind;

		vdLogInfo("Processing deferred event '%s' ... ", 
			Event::Type::ToString(event.Kind));

		EventChannel::iterator dhit;
		for(dhit = m_DeferredHandlers[kind].begin(); dhit != m_DeferredHandlers[kind].end(); ++dhit)
		{
			vd::status status = (*dhit)(event);
			if(status != Status::Code::Success)
			{
				vdLogInfo("Event handler for '%s' returned '%s' status!", 
					Event::Type::ToString(kind),  
					Status::Code::ToString(status));  
			}
		}		
	}
	m_Events.clear();
}

void
Window::PostRedisplay(void)
{
	if(!Global::IsExiting)
		glutPostRedisplay();
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
	    glutPostRedisplay();
	    glutSetWindow(m_Handle);
	}
    m_Mutex.Unlock();

	glutMainLoop();	
	
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

Interface::Window::Config
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
	config.FrameRateLimit = m_FrameRateLimit;
	config.Borderless = 0;
	return config;
}

// ============================================================================================== //

VD_IMPLEMENT_OBJECT(Window, vd_sym(GlutWindow), vd_sym(InterfaceWindow));

// ============================================================================================== //

VD_INTERFACE_GLUT_NAMESPACE_END();

// ============================================================================================== //



