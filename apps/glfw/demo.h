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

#ifndef VD_DEMO_GLUT_INCLUDED
#define VD_DEMO_GLUT_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/builtins.h"
#include "core/utilities.h"
#include "core/process.h"
#include "core/buffer.h"

#include "graphics/graphics.h"

#include "interface/interface.h"
#include "interface/gwen/gwen.h"
#include "interface/gwen/context.h"
#include "interface/glfw/window.h"

#include "runtime/application.h"

#include "widgets.h"
#include "config.h"

#if defined(VD_USE_EXTRACTED_SYMBOLS)
#include "symbols.h"
#endif

// ============================================================================================== //

VD_RUNTIME_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_USING(Core, ParamSet);
VD_USING(Interface, Event);
VD_USING(Interface, Window);

// ============================================================================================== //

class Demo : public Application
{

public:
    Demo();
    virtual ~Demo();

	virtual vd::status Initialize(int* argc = 0, void ** argv = 0);
    virtual vd::status Startup(int* argc = 0, void ** argv = 0);
    vd::status Execute();
    virtual vd::status Shutdown();

    vd::status OpenDataset(const char* filename);
    vd::status LoadSettings(std::string filename);
	vd::status Reshape(int width, int height);
    vd::status LoadResources();
    vd::status Render();

    void SaveTiles();
    void RenderFrame();
    void RenderOverlay();
    void ClearFrame();

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
    vd::status HandleKeys();
    
	VD_DECLARE_OBJECT(Demo);
	
protected:

	VD_DISABLE_COPY_CONSTRUCTORS(Demo);

    vd::status CreateWindow(int* argc, void** argv);
	vd::status UpdateData(void);
	
	void DrawScene();
    void CaptureScreen();
    void SaveScreen();
    void SetFrameViewport();
    void SetWindowViewport();
    void SaveFramebuffer();
    
private:

	Window*                         m_Window;
    Interface::Gw::Context*         m_Gui;
    Interface::Gw::WindowControl*   m_Controls;
    ParamSet                        m_Params;
    Config                          m_Config;
    vd::f64                         m_LastUpdate;
    vd::status                      m_Status;
};

// ============================================================================================== //

VD_RUNTIME_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_DEMO_GLUT_INCLUDED
