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

#ifndef VD_INTERFACE_COCOA_WINDOW_INCLUDED
#define VD_INTERFACE_COCOA_WINDOW_INCLUDED

// ============================================================================================== //

#include "graphics/gl/opengl.h"
#include "graphics/gl/context.h"

#include "core/core.h"
#include "core/object.h"

#include "runtime/runtime.h"
#include "interface/cocoa/namespace.h"

#include "graphics/graphics.h"
#include "graphics/gl/opengl.h"

#include "interface/event.h"
#include "interface/keyboard.h"
#include "interface/mouse.h"
#include "interface/window.h"

#include "core/collections.h"
#include "core/paramset.h"

// ============================================================================================== //

VD_INTERFACE_COCOA_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_USING(Core, Mutex);
VD_USING(Interface, Keyboard);
VD_USING(Interface, Mouse);
VD_USING(Interface, Event);
VD_USING(Core, Vector);

// ============================================================================================== //

class Window : public Interface::Window
{

public:

	typedef Vector< Event >::type 				EventList;
	typedef Vector< Event::Callback >::type 	EventChannel;

	Window(Runtime::Context* runtime);
	virtual ~Window();
	
	virtual void ProcessArgs(int* argc = 0, void ** argv = 0);
    virtual void Setup(const vd::string& title, const Graphics::Viewport& vp, const Core::ParamSet& params);
	virtual void SetActive(bool enable=true);
	virtual vd::status Destroy();
	virtual bool ProcessEvent(Event& event);	
	virtual bool PollEvent(Event& event);
	virtual bool AddEventHandler(Event::Type::Value kind, const Event::Callback& cb, bool deferred=true );
	virtual bool RemoveEventHandlers(Event::Type::Value kind);
	virtual void RemoveAllEventHandlers();
	
	virtual void SetFullScreen( bool enable = true );
	virtual void SetClearColor( const vd::v4f32& color )	{ m_ClearColor = color; }
	virtual const vd::v4f32& GetClearColor()				{ return m_ClearColor; }
    virtual Config GetConfig();
	
	virtual void Finish(void);
	virtual void Flush(void);
	virtual void PostRedisplay(void);
	virtual void DisplayFrame(void);
	virtual void FlushEvents(void);
	virtual void ClearFrame(void);
	virtual void SwapFrame(void);
	virtual vd::i32 GetFrameRateLimit(void);
	virtual void SetFrameRateLimit(vd::i32 v) { m_FrameRateLimit = v; }
	
	virtual bool IsOpened(void);
	virtual bool IsExiting(void);
	
	virtual vd::status Execute(void);
	virtual void Close(void);
	virtual void Exit(void);
	
	virtual Graphics::Context* GetGraphics();
	
	VD_DECLARE_OBJECT(Window);

private:

	VD_DISABLE_COPY_CONSTRUCTORS(Window);

	Mutex						m_Mutex;
	bool						m_Exit;
	bool						m_IsOpened;
	vd::i32						m_Handle;
	vd::i32						m_FrameRateLimit;
	vd::v2i32					m_Size;
	vd::v2i32					m_Position;
	vd::v4f32					m_ClearColor;
	vd::string					m_Title;
	EventList					m_Events;
	EventChannel				m_Handlers[Event::Type::Count];
	EventChannel				m_DeferredHandlers[Event::Type::Count];	
    Graphics::Context*          m_Graphics;
};

// ============================================================================================== //

VD_INTERFACE_COCOA_NAMESPACE_END();

// ============================================================================================== //

#endif	// VD_INTERFACE_COCOA_WINDOW_INCLUDED
