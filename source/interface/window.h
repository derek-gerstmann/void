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

#ifndef VD_INTERFACE_WINDOW_INCLUDED
#define VD_INTERFACE_WINDOW_INCLUDED

// ============================================================================================== //


#include "core/core.h"
#include "core/object.h"
#include "core/status.h"

#include "runtime/runtime.h"
#include "runtime/context.h"

#include "graphics/graphics.h"
#include "graphics/context.h"
#include "graphics/viewport.h"

#include "interface/event.h"
#include "interface/keyboard.h"
#include "interface/mouse.h"

#include "containers/containers.h"
#include "containers/paramset.h"

// ============================================================================================== //

VD_INTERFACE_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_IMPORT(Core, Mutex);
VD_IMPORT(Interface, Keyboard);
VD_IMPORT(Interface, Mouse);
VD_IMPORT(Interface, Event);
VD_IMPORT(Containers, Vector);

// ============================================================================================== //

class VD_API Window : public Core::Object
{
public:

    typedef Vector< Event >::type               EventList;
    typedef Vector< Event::Callback >::type     EventChannel;

    struct VD_API Config
    {
        const char* Title;
        int X;
        int Y;
        int Width;
        int Height;
        int MultiSampling;
        int VerticalSync;
        int Borderless;
        int FrameRateLimit;
    };

public:

    Window(Runtime::Context* runtime);
    virtual ~Window();

    virtual vd::status Destroy();
    
    virtual void ProcessArgs(int* argc = 0, void ** argv = 0) = 0;
    virtual void Setup(const vd::string& title, const Graphics::Viewport& vp, const Containers::ParamSet& params) = 0;
    virtual void SetActive(bool enable=true) = 0;
    virtual bool ProcessEvent(Event& event) = 0;   
    virtual bool PollEvent(Event& event) = 0;
    virtual bool AddEventHandler(Event::Type::Value kind, const Event::Callback& cb, bool deferred=true ) = 0;
    virtual bool RemoveEventHandlers(Event::Type::Value kind) = 0;
    virtual void RemoveAllEventHandlers() = 0;
    
    virtual void SetFullScreen( bool enable = true ) = 0;
    virtual void SetClearColor( const vd::v4f32& color )  = 0;
    virtual const vd::v4f32& GetClearColor() = 0;
    virtual Config GetConfig() = 0;
    
    virtual void Finish(void) = 0;
    virtual void Flush(void) = 0;
    virtual void PostRedisplay(void) = 0;
    virtual void DisplayFrame(void) = 0;
    virtual void FlushEvents(void) = 0;
    virtual void ClearFrame(void) = 0;
    virtual void SwapFrame(void) = 0;
    virtual vd::i32 GetFrameRateLimit(void) = 0;
    virtual void SetFrameRateLimit(vd::i32 v)  = 0;
    
    virtual bool IsOpened(void) = 0;
    virtual bool IsExiting(void) = 0;
    
    virtual vd::status Execute(void) = 0;
    virtual void Close(void) = 0;
    virtual void Exit(void) = 0;
    
    Runtime::Context* GetRuntime();
    virtual Graphics::Context* GetGraphics() = 0;

    VD_DECLARE_OBJECT(Window);

protected:
    Window::Config              m_Config;

private:

    VD_DISABLE_COPY_CONSTRUCTORS(Window);

    Runtime::Context*           m_Runtime;

};

// ============================================================================================== //

VD_INTERFACE_NAMESPACE_END();

// ============================================================================================== //

#endif  // VD_INTERFACE_WINDOW_INCLUDED
