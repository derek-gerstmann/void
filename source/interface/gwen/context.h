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

#ifndef VD_INTERFACE_GWEN_CONTEXT_INCLUDED
#define VD_INTERFACE_GWEN_CONTEXT_INCLUDED

// ============================================================================================== //

#include "interface/interface.h"
#include "interface/event.h"

#include "interface/gwen/gwen.h"
#include "interface/gwen/renderer.h"
#include "interface/gwen/canvas.h"
#include "interface/gwen/skin.h"
#include "interface/gwen/input.h"

// ============================================================================================== //

VD_INTERFACE_GWEN_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_IMPORT(Core, Status);

// ============================================================================================== //

class Context : public Core::Object
{
public:
	Context(Graphics::Context* graphics);
	virtual ~Context(){ Destroy(); }
	virtual vd::status Destroy();

	Canvas* GetCanvas(void);
	vd::status Initialize(vd::i32 width, vd::i32 height);
    vd::status OnShutdown(const Event&);
    vd::status OnUpdate(const Event& event);
    vd::status OnKeyEvent(const Event & event);
    vd::status OnResize(const Event & event);
    vd::status OnMouseDown(const Event & event);
    vd::status OnMouseUp(const Event & event);
    vd::status OnMouseMotion(const Event & event);
    vd::status OnMouseWheel(const Event & event);
    vd::status OnCloseEvent(const Event & event);
    vd::status OnDisplay(const Event & event);    
    vd::status Render(void);

	VD_DECLARE_OBJECT(Context);

private:

	VD_DISABLE_COPY_CONSTRUCTORS(Context);

    vd::i32             m_Width;
    vd::i32             m_Height;
	Skin* 	            m_Skin;
	Input* 	            m_Input;
	Canvas*             m_Canvas;
    Renderer*           m_Renderer; 
    Graphics::Context*  m_Graphics;
};

// ============================================================================================== //

VD_INTERFACE_GWEN_NAMESPACE_END();

// ============================================================================================== //

#endif	// VD_INTERFACE_GWEN_CONTEXT_INCLUDED

// ============================================================================================== //
// end file

