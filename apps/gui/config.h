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

#ifndef VD_DEMO_CONFIG_INCLUDED
#define VD_DEMO_CONFIG_INCLUDED

// ============================================================================================== //

#include "runtime/runtime.h"

#include "core/numerics.h"
#include "core/collections.h"

#include "graphics/graphics.h"
#include "graphics/viewport.h"

#include "interface/interface.h"
#include "interface/keyboard.h"
#include "interface/mouse.h"
#include "interface/event.h"

// ============================================================================================== //

VD_RUNTIME_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_USING(Core, Memory);
VD_USING(Core, AlignedMap);
VD_USING(Interface, Event);
VD_USING(Interface, Mouse);
VD_USING(Interface, Keyboard);
VD_USING(Graphics, Viewport);

// ============================================================================================== //

struct Config
{
    typedef AlignedMap<vd::i32, vd::uid>::type KeyMapType;
	
    Config()
	{
	    Memory::SetBytes(this, 0, sizeof(Config));
	}
	
	~Config(){}
	
    struct
    {
		Graphics::Viewport Viewport;
    } Window;

    struct
    {
        Event::KeyEvent Down[Keyboard::KeyCode::Count];
        vd::i32 Pending;
    } KeyState;

    struct
    {
        KeyMapType KeyMap;
        bool MouseDown[Mouse::Button::Count];
        bool KeyDown[Keyboard::KeyCode::Count];
        bool Ctrl[Keyboard::KeyCode::Count];
        bool Alt[Keyboard::KeyCode::Count];
        bool Shift[Keyboard::KeyCode::Count];
    } Controls;

    struct
    {
        bool Initialised;
        vd::u64 LastRenderTime;
        vd::u64 FrameCount;
        vd::u64 TotalFrames;
        vd::u64 StartTimeForFrame;
        vd::u64 CurrentTimeForFrame;
        bool Terminate;
        bool Stale;
        bool Paused;
        bool FullScreen;
    } Status;

    struct
    {
        bool CaptureScreen;
        bool OpenDataset;
        bool Shutdown;
    } Commands;
};

// ============================================================================================== //

VD_RUNTIME_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_DEMO_CONFIG_INCLUDED
