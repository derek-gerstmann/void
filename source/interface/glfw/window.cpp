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

#include "interface/glfw/window.h"

#include "core/system.h"
#include "core/process.h"
#include "core/handles.h"
#include "core/threading.h"

// ============================================================================================== //

VD_INTERFACE_GLFW_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_USING(Core, ScopedMutex);

VD_USING(Interface, Mouse);
VD_USING(Interface, Keyboard);
VD_USING(Interface, Event);

// ============================================================================================== //

namespace Global 
{
	static bool IsExiting = false;
	static Glfw::Window* ActiveWindow = NULL;
}

// ============================================================================================== //

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================================== //

static void GLFWCALL vdNullWindowSizeCallback(int,int) { }
static int  GLFWCALL vdNullWindowCloseCallback(void) { }
static void GLFWCALL vdNullWindowRefreshCallback(void) { }
static void GLFWCALL vdNullMouseButtonCallback(int,int) { }
static void GLFWCALL vdNullMousePositionCallback(int,int) { }
static void GLFWCALL vdNullMouseWheelCallback(int) { }
static void GLFWCALL vdNullKeyboardCallback(int,int) { }
static void GLFWCALL vdNullCharCallback(int,int) { }
static void GLFWCALL vdNullThreadCallback(void*) { }

// ============================================================================================= //
        
static Keyboard::KeyCode::Value
vdGuiTranslateAsciiKey(int code)
{
	switch(code)
	{
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
		case 'b':			{ return Keyboard::KeyCode::B;  }
		case 'B':			{ return Keyboard::KeyCode::B;  }
		case 'c':			{ return Keyboard::KeyCode::C;  }
		case 'C':			{ return Keyboard::KeyCode::C;  }
		case 'd':			{ return Keyboard::KeyCode::D;  }
		case 'D':			{ return Keyboard::KeyCode::D;  }
		case 'e':			{ return Keyboard::KeyCode::E;  }
		case 'E':			{ return Keyboard::KeyCode::E;  }
		case 'f':			{ return Keyboard::KeyCode::F;  }
		case 'F':			{ return Keyboard::KeyCode::F;  }
		case 'g':			{ return Keyboard::KeyCode::G;  }
		case 'G':			{ return Keyboard::KeyCode::G;  }
		case 'h':			{ return Keyboard::KeyCode::H;  }
		case 'H':			{ return Keyboard::KeyCode::H;  }
		case 'i':			{ return Keyboard::KeyCode::I;  }
		case 'I':			{ return Keyboard::KeyCode::I;  }
		case 'j':			{ return Keyboard::KeyCode::J;  }
		case 'J':			{ return Keyboard::KeyCode::J;  }
		case 'k':			{ return Keyboard::KeyCode::K;  }
		case 'K':			{ return Keyboard::KeyCode::K;  }
		case 'l':			{ return Keyboard::KeyCode::L;  }
		case 'L':			{ return Keyboard::KeyCode::L;  }
		case 'm':			{ return Keyboard::KeyCode::M;  }
		case 'M':			{ return Keyboard::KeyCode::M;  }
		case 'n':			{ return Keyboard::KeyCode::N;  }
		case 'N':			{ return Keyboard::KeyCode::N;  }
		case 'o':			{ return Keyboard::KeyCode::O;  }
		case 'O':			{ return Keyboard::KeyCode::O;  }
		case 'p':			{ return Keyboard::KeyCode::P;  }
		case 'P':			{ return Keyboard::KeyCode::P;  }
		case 'q':			{ return Keyboard::KeyCode::Q;  }
		case 'Q':			{ return Keyboard::KeyCode::Q;  }
		case 'r':			{ return Keyboard::KeyCode::R;  }
		case 'R':			{ return Keyboard::KeyCode::R;  }
		case 's':			{ return Keyboard::KeyCode::S;  }
		case 'S':			{ return Keyboard::KeyCode::S;  }
		case 't':			{ return Keyboard::KeyCode::T;  }
		case 'T':			{ return Keyboard::KeyCode::T;  }
		case 'u':			{ return Keyboard::KeyCode::U;  }
		case 'U':			{ return Keyboard::KeyCode::U;  }
		case 'v':			{ return Keyboard::KeyCode::V;  }
		case 'V':			{ return Keyboard::KeyCode::V;  }
		case 'w':			{ return Keyboard::KeyCode::W;  }
		case 'W':			{ return Keyboard::KeyCode::W;  }
		case 'x':			{ return Keyboard::KeyCode::X;  }
		case 'X':			{ return Keyboard::KeyCode::X;  }
		case 'y':			{ return Keyboard::KeyCode::Y;  }
		case 'Y':			{ return Keyboard::KeyCode::Y;  }
		case 'z':			{ return Keyboard::KeyCode::Z;  }
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
vdGuiTranslateSpecialKey(int code)
{
	switch(code)
	{
		case GLFW_KEY_UP:           { return Keyboard::KeyCode::Up; }
		case GLFW_KEY_DOWN:         { return Keyboard::KeyCode::Down; }
		case GLFW_KEY_LEFT:         { return Keyboard::KeyCode::Left; }
		case GLFW_KEY_RIGHT:        { return Keyboard::KeyCode::Right; }
		case GLFW_KEY_LSHIFT:       { return Keyboard::KeyCode::Shift; }
		case GLFW_KEY_RSHIFT:       { return Keyboard::KeyCode::Shift; }
		case GLFW_KEY_LCTRL:        { return Keyboard::KeyCode::Control; }
		case GLFW_KEY_RCTRL:        { return Keyboard::KeyCode::Control; }
		case GLFW_KEY_LALT:         { return Keyboard::KeyCode::Alt; }
		case GLFW_KEY_RALT:         { return Keyboard::KeyCode::Alt; }
		case GLFW_KEY_TAB:          { return Keyboard::KeyCode::Tab; }
		case GLFW_KEY_ENTER:        { return Keyboard::KeyCode::Return; }
		case GLFW_KEY_BACKSPACE:    { return Keyboard::KeyCode::Backspace; }
		case GLFW_KEY_INSERT:       { return Keyboard::KeyCode::Insert; }
		case GLFW_KEY_DEL:          { return Keyboard::KeyCode::Delete; }
		case GLFW_KEY_PAGEUP:       { return Keyboard::KeyCode::PageUp; }
		case GLFW_KEY_PAGEDOWN:     { return Keyboard::KeyCode::PageDown; }
		case GLFW_KEY_HOME:         { return Keyboard::KeyCode::Home; }
		case GLFW_KEY_END:          { return Keyboard::KeyCode::End; }
		case GLFW_KEY_CAPS_LOCK:    { return Keyboard::KeyCode::CapsLock; }
		case GLFW_KEY_SCROLL_LOCK:  { return Keyboard::KeyCode::ScrollLock; }
		case GLFW_KEY_PAUSE:        { return Keyboard::KeyCode::Pause; }
		case GLFW_KEY_MENU:         { return Keyboard::KeyCode::Menu; }
		case GLFW_KEY_F1:			{ return Keyboard::KeyCode::F1;  }
		case GLFW_KEY_F2:			{ return Keyboard::KeyCode::F2;  }
		case GLFW_KEY_F3:			{ return Keyboard::KeyCode::F3;  }
		case GLFW_KEY_F4:			{ return Keyboard::KeyCode::F4;  }
		case GLFW_KEY_F5:			{ return Keyboard::KeyCode::F5;  }
		case GLFW_KEY_F6:			{ return Keyboard::KeyCode::F6;  }
		case GLFW_KEY_F7:			{ return Keyboard::KeyCode::F7;  }
		case GLFW_KEY_F8:			{ return Keyboard::KeyCode::F8;  }
		case GLFW_KEY_F9:			{ return Keyboard::KeyCode::F9;  }
		case GLFW_KEY_F10:			{ return Keyboard::KeyCode::F10;  }
		case GLFW_KEY_F11:			{ return Keyboard::KeyCode::F11;  }
		case GLFW_KEY_F12:			{ return Keyboard::KeyCode::F12;  }
		default:					{ return Keyboard::KeyCode::Invalid;  }
	};

	return Keyboard::KeyCode::Invalid;
}


static void
vdGuiShutdown()
{
    glfwSetWindowSizeCallback( vdNullWindowSizeCallback );
    glfwSetWindowCloseCallback( vdNullWindowCloseCallback );
    glfwSetWindowRefreshCallback( vdNullWindowRefreshCallback );
    glfwSetKeyCallback( vdNullKeyboardCallback );
    glfwSetCharCallback( vdNullCharCallback );
    glfwSetMouseButtonCallback( vdNullMouseButtonCallback );
    glfwSetMousePosCallback( vdNullMousePositionCallback );
    glfwSetMouseWheelCallback( vdNullMouseWheelCallback );

	if(Global::ActiveWindow)
	{	
		Event event;
		event.Kind = Event::Type::Shutdown;
		event.Update.Time = Core::Process::GetTimeInSeconds();
		Global::ActiveWindow->ProcessEvent( event );
		Global::ActiveWindow = NULL;
	}

	glfwTerminate();
}
    
static void GLFWCALL
vdGuiMousePositionCallback(int x, int y)
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
    
static void GLFWCALL
vdGuiMouseButtonCallback(int action, int button)
{
	Event event;
	Mouse::Button::Value value = Mouse::Button::Left;
	
	switch(button)
	{
		case GLFW_MOUSE_BUTTON_LEFT:	{ value = Mouse::Button::Left; break; }
		case GLFW_MOUSE_BUTTON_RIGHT:	{ value = Mouse::Button::Right; break; }
		case GLFW_MOUSE_BUTTON_MIDDLE:	{ value = Mouse::Button::Middle; break; }
		// case GLFW_MOUSE_BUTTON_1:		{ value = Mouse::Button::X1; break; }
		// case GLFW_MOUSE_BUTTON_2:		{ value = Mouse::Button::X2; break; }
		// case GLFW_MOUSE_BUTTON_3:		{ value = Mouse::Button::X3; break; }
		case GLFW_MOUSE_BUTTON_4:		{ value = Mouse::Button::X4; break; }
		case GLFW_MOUSE_BUTTON_5:		{ value = Mouse::Button::X5; break; }
		case GLFW_MOUSE_BUTTON_6:		{ value = Mouse::Button::X6; break; }
		case GLFW_MOUSE_BUTTON_7:		{ value = Mouse::Button::X7; break; }
		case GLFW_MOUSE_BUTTON_8:       { value = Mouse::Button::X8; break; }
		default:	break;
	};

    int xpos = 0;
    int ypos = 0;
    glfwGetMousePos( &xpos, &ypos );

	event.MouseButton.Button = value;
	event.MouseButton.X = xpos;
	event.MouseButton.Y = ypos;

    if (action == GLFW_PRESS)
		event.Kind = Event::Type::MouseDown;
    else if(action == GLFW_RELEASE)
    	event.Kind = Event::Type::MouseUp;

	if(Global::ActiveWindow) 
		Global::ActiveWindow->ProcessEvent( event );
}

static void GLFWCALL
vdGuiMouseWheelCallback(int x)
{
    int xpos = 0;
    int ypos = 0;

    glfwGetMousePos( &xpos, &ypos );

	Event event;
	event.MouseWheel.X = xpos;
	event.MouseWheel.Y = ypos;
	event.MouseWheel.Delta = x;
	event.Kind = Event::Type::MouseScroll;

	if(Global::ActiveWindow) 
		Global::ActiveWindow->ProcessEvent( event );
}

static void GLFWCALL
vdGuiWindowRefreshCallback(void)
{
	if(Global::ActiveWindow) 
	{
		Event event;
		event.Kind = Event::Type::Display;
		event.Update.Time = Core::Process::GetTimeInSeconds();
		Global::ActiveWindow->ProcessEvent( event );
	}	
}

static void GLFWCALL
vdGuiWindowSizeCallback(int width, int height)
{
	Event event;
	event.Size.Width = width;
	event.Size.Height = height;
	event.Kind = Event::Type::Resize;

	if(Global::ActiveWindow) 
		Global::ActiveWindow->ProcessEvent( event );
}

static int GLFWCALL
vdGuiWindowCloseCallback(void)
{
	Event event;
	event.Kind = Event::Type::Close;

	if(Global::ActiveWindow) 
		Global::ActiveWindow->ProcessEvent( event );

	return 0;
}

static void GLFWCALL
vdGuiKeyboardCallback(int key, int action)
{
	Event event;

	bool shift = glfwGetKey( GLFW_KEY_LSHIFT ) || glfwGetKey(GLFW_KEY_RSHIFT);
	bool ctrl = glfwGetKey( GLFW_KEY_LCTRL ) || glfwGetKey(GLFW_KEY_RCTRL);
	bool alt = glfwGetKey( GLFW_KEY_LALT ) || glfwGetKey(GLFW_KEY_RALT);
	
	if(action == GLFW_PRESS)
		event.Kind = Event::Type::KeyDown;
	else
		event.Kind = Event::Type::KeyUp;


	event.Key.Control = ctrl;
	event.Key.Alt = alt;
	event.Key.Shift = shift;
	event.Key.Code = vdGuiTranslateSpecialKey(key);
	if(event.Key.Code == Keyboard::KeyCode::Invalid)
		event.Key.Code = vdGuiTranslateAsciiKey(key);
	
	if(Global::ActiveWindow) 
		Global::ActiveWindow->ProcessEvent( event );
}

static void
vdGuiIdleCallback(void)
{	
	static vd::u64 IdleCallbackCount = 0;
	static double LastCallbackTime = 0;
	
	if(Global::IsExiting)
	{
		vdGuiShutdown();
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
    if( !glfwInit() )
    {
        vdLogError("Failed to initialize GLFW\n" );
        return;
    }
}

void 
Window::Setup(
	const vd::string& title,
	const Graphics::Viewport& vp,
	const Core::ParamSet&)
{
	ScopedMutex locker(&m_Mutex);

	m_Title = title;
    m_Position = vd::v2i32(vp.X, vp.Y);
	m_Size = vd::v2i32(vp.Width, vp.Height);
    
    if( !glfwOpenWindow( vp.Width, vp.Height, 0,0,0,0, 0,0, GLFW_WINDOW ) )
    {
        glfwTerminate();
        vdLogError("Failed to create GLFW window!\n" );
        return;
    }
    
    glfwSetWindowTitle( m_Title.c_str() );
	glfwDisable( GLFW_AUTO_POLL_EVENTS );
//    glfwEnable( GLFW_STICKY_KEYS );
    glfwSwapInterval( 1 );

	m_Handle = 1;
	m_Graphics = VD_NEW(Graphics::OpenGL::Context, GetRuntime());
	m_IsOpened = true;

	DisplayFrame();
	PostRedisplay();
}

void
Window::SetActive(bool enable)
{
	ScopedMutex locker(&m_Mutex);
	if(enable)
	{
		Global::ActiveWindow = this;	
        glfwSetWindowSizeCallback( vdGuiWindowSizeCallback );
        glfwSetWindowCloseCallback( vdGuiWindowCloseCallback );
        glfwSetWindowRefreshCallback( vdGuiWindowRefreshCallback );
        glfwSetKeyCallback( vdGuiKeyboardCallback );
        glfwSetMouseButtonCallback( vdGuiMouseButtonCallback );
        glfwSetMousePosCallback( vdGuiMousePositionCallback );
        glfwSetMouseWheelCallback( vdGuiMouseWheelCallback );
	}
	else
	{
        glfwSetWindowSizeCallback( vdNullWindowSizeCallback );
        glfwSetWindowCloseCallback( vdNullWindowCloseCallback );
        glfwSetWindowRefreshCallback( vdNullWindowRefreshCallback );
        glfwSetKeyCallback( vdNullKeyboardCallback );
        glfwSetCharCallback( vdNullCharCallback );
        glfwSetMouseButtonCallback( vdNullMouseButtonCallback );
        glfwSetMousePosCallback( vdNullMousePositionCallback );
        glfwSetMouseWheelCallback( vdNullMouseWheelCallback );
		Global::ActiveWindow = NULL;	
	}
}
	
vd::status 
Window::Destroy() 
{
	Exit();
	return Status::Code::Success;
}

void
Window::RemoveAllEventHandlers()
{
	ScopedMutex locker(&m_Mutex);
	
	for(vd::u32 i = 0; i < Event::Type::Count; ++i)
	{
		vd::u32 kind = (vd::u32)i;
		if(m_Handlers[kind].size())
			RemoveEventHandlers(Event::Type::FromInteger(kind));
	}
}	

bool 
Window::ProcessEvent( Event& event )	
{
//	ScopedMutex locker(&m_Mutex);

	vd::u32 kind = Event::Type::ToInteger(event.Kind);
	if(m_Handlers[kind].empty() == false)
	{
		EventChannel::iterator it;
		for(it = m_Handlers[kind].begin(); it != m_Handlers[kind].end(); ++it)
		{
			(*it)(event);
		}		
	}
	else if(m_DeferredHandlers[kind].empty() == false)
	{
		vdLogDebug("Deferring event '%s' [%d of %d] ... ", 
			Event::Type::ToString(event.Kind),
			kind,
			(int)m_DeferredHandlers[kind].size());
		m_Events.push_back(event);
	}
	return true;
}
	
bool
Window::PollEvent( Event& event )
{
	ScopedMutex locker(&m_Mutex);

	if(m_Events.empty() == false)
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

	vd::u32 kind = Event::Type::ToInteger(value);

	vdLogDebug("Adding event handler for [%d] '%s' ... ", 
		(int)kind,
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
	glfwCloseWindow();
	m_Mutex.Lock();
	m_Handle = 0;
	m_IsOpened = false;	
	m_Mutex.Unlock();
}

bool
Window::RemoveEventHandlers(
	Event::Type::Value value)
{
	vd::u32 kind = Event::Type::ToInteger(value);
	if(m_Handlers[kind].empty() == false)
	{
		m_Handlers[kind].clear();
	}

	if(m_DeferredHandlers[kind].empty() == false)
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
	glfwSwapBuffers();
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

		vdLogDebug("Processing deferred event '%s' ... ", Event::Type::ToString(event.Kind));

		EventChannel::iterator dhit;
		for(dhit = m_DeferredHandlers[kind].begin(); dhit != m_DeferredHandlers[kind].end(); ++dhit)
		{
			vd::status result = (*dhit)(event);
			Status::Code::Value code = Status::Code::FromInteger(result);
			if(code != Status::Code::Success)
			{
				vdLogDebug("Event handler for '%s' returned '%s' status!", 
					Event::Type::ToString(event.Kind),  
					Status::Code::ToString(code));  
			}
		}		
	}
	m_Events.clear();
}

void
Window::PostRedisplay(void)
{
	Event event;
	event.Kind = Event::Type::Display;
	event.Update.Time = Core::Process::GetTimeInSeconds();
	ProcessEvent( event );
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
	bool running = true;

	SetActive(true);
	DisplayFrame();

	vd::f64 interval = 1.0/30.0f;
	if(GetFrameRateLimit())
		interval = 1.0/GetFrameRateLimit();

	do
	{
		glfwPollEvents();
		glfwSleep(interval);
		
		Event event;
		event.Kind = Event::Type::Update;
		event.Update.Time = Core::Process::GetTimeInSeconds();
		ProcessEvent( event );

		if(Global::IsExiting)
			running = false;
	}
	while( running );

	Finish();
	glfwTerminate();	
	return Status::Code::Success;
}

void
Window::Exit()
{
	m_Mutex.Lock();
	Global::IsExiting = true;
	m_Exit = true;
	m_Mutex.Unlock();
}

void
Window::EnterFullScreen()
{
	if(m_Handle == 0)
		return;

	GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);

    m_Size[0] = vp[2];
    m_Size[1] = vp[3];

    Close();

	GLFWvidmode mode;
	glfwGetDesktopMode( &mode );
    if( !glfwOpenWindow( mode.Width, mode.Height, 0,0,0,0, 0,0, GLFW_FULLSCREEN ) )
    {
        glfwTerminate();
        vdLogError("Failed to create fullscreen window!\n" );
        return;
    }

	m_Mutex.Lock();
	m_Handle = 1;
	m_IsOpened = true;	
	m_Mutex.Unlock();
	
	glfwEnable( GLFW_MOUSE_CURSOR );
	SetActive(true);
}

void
Window::ExitFullScreen()
{
	if(m_Handle == 0)
		return;

	Close();

    if( !glfwOpenWindow( m_Size[0], m_Size[1], 0,0,0,0, 0,0, GLFW_WINDOW ) )
    {
        glfwTerminate();
        vdLogError("Failed to create fullscreen window!\n" );
        return;
    }

	m_Mutex.Lock();
	m_Handle = 1;
	m_IsOpened = true;	
	m_Mutex.Unlock();

	SetActive(true);
}

void
Window::Reshape(vd::i32 w, vd::i32 h)
{
	if(m_Handle == 0)
		return;

    m_Size[0] = w;
    m_Size[1] = h;

    glfwSetWindowSize(w, h);
    PostRedisplay();
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
	config.MultiSampling = 0;
	config.FrameRateLimit = m_FrameRateLimit;
	config.Borderless = 0;
	return config;
}

// ============================================================================================== //

VD_IMPLEMENT_OBJECT(Window, vd_sym(GlfwWindow), vd_sym(InterfaceWindow));

// ============================================================================================== //

VD_INTERFACE_GLFW_NAMESPACE_END();

// ============================================================================================== //



