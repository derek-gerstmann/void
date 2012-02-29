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

#ifndef VD_INTERFACE_EVENT_INCLUDED
#define VD_INTERFACE_EVENT_INCLUDED

// ============================================================================================== //

#include "interface/interface.h"
#include "interface/mouse.h"
#include "interface/keyboard.h"

#include "core/functions.h"

// ============================================================================================== //

VD_INTERFACE_NAMESPACE_BEGIN();

// ============================================================================================== //

class Event
{

public :

	typedef VD_CORE_SCOPE::Function< vd::status ( const Event& ) > Callback;
	
	VD_DECLARE_ENUM(Type,
    	Invalid,
        Startup,                 
        Shutdown,                 
        Update,                 
        Display,                 
        Refresh,                 
        Closed,                 
        Resize,                
        LostFocus,              
        GainedFocus,            
        TextEntered,            
        KeyDown,             
        KeyUp,            
        MouseScroll,        
        MouseUp,     
        MouseDown,    
        MouseMoved,             
        MouseEntered,           
        MouseExited,
        UserEvent,
        SystemEvent,
        Unknown
    );

    struct SizeEvent
    {
        vd::u32 Width;  
        vd::u32 Height; 
    };

    struct KeyEvent
    {
        Keyboard::KeyCode::Value Code;    
        bool          			 Alt;     
        bool          			 Control; 
        bool          			 Shift;   
        bool          			 System;  
    };

    struct TextEvent
    {
        vd::u32 Unicode; 
    };

    struct MouseMoveEvent
    {
        vd::i32 X; 
        vd::i32 Y; 
    };

    struct MouseButtonEvent
    {
        Mouse::Button::Value Button; 
        vd::i32       		 X;      
        vd::i32       		 Y;      
    };

    struct MouseWheelEvent
    {
        int Delta; 
        vd::i32 X;     
        vd::i32 Y;     
    };

	struct UpdateEvent
	{
		vd::f64 Time;		
	};
	
    union
    {
        SizeEvent            Size;           
        KeyEvent             Key;            
        TextEvent            Text;            
        MouseMoveEvent       MouseMove;       
        MouseButtonEvent     MouseButton;    
        MouseWheelEvent      MouseWheel;      
        UpdateEvent			 Update;
    };

    Type::Value 			 Kind;
};

// ============================================================================================== //

VD_INTERFACE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_INTERFACE_EVENT_INCLUDED

// ============================================================================================== //
// end file


