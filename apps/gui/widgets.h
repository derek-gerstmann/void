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

#ifndef VD_UI_DEMO_WIDGETS_INCLUDED
#define VD_UI_DEMO_WIDGETS_INCLUDED

// ============================================================================================== //

//#include "interface/interface.h"
#include "interface/gwen/gwen.h"
// #include "interface/gwen/context.h"
// #include "interface/gwen/controls.h"
// #include "interface/gwen/layouts.h"

// ============================================================================================== //

VD_INTERFACE_GWEN_NAMESPACE_BEGIN();

// ============================================================================================== //

class MainMenu : public Gwen::Controls::Base
{
public:
	VD_UI_CONTROL( MainMenu, Widget );

private:
	void OnOpenFile( Gwen::Controls::Base* );
	void OnSaveFile( Gwen::Controls::Base* );
	void OnQuit( Gwen::Controls::Base* );

	MenuStrip*	m_MenuStrip;
};

// ============================================================================================== //

class CloseButton;

class WindowControl : public Gwen::Controls::ResizableControl
{
public:

	VD_UI_CONTROL( WindowControl, ResizableControl );

	virtual ~WindowControl();
	virtual void Render( Gwen::Skin::Base* skin );
	virtual void RenderUnder( Gwen::Skin::Base* skin );

	virtual void SetTitle( UnicodeString title );
	virtual void SetTitle( String title ){ SetTitle( Gwen::Utility::StringToUnicode( title ) ); }
	virtual void SetClosable(bool closeable);

	virtual void Touch();
	bool IsOnTop();

	virtual void SetHidden(bool hidden);

	void CloseButtonPressed( Gwen::Controls::Base* pFromPanel );
	void RenderFocus( Gwen::Skin::Base* skin );
	void SetDeleteOnClose( bool b ){ m_bDeleteOnClose = b; }

	void MakeModal( bool bDrawBackground = true );
	void DestroyModal();

protected:

	Internal::Dragger* m_TitleBar;
	Gwen::Controls::Label* m_Title;
	CloseButton* m_CloseButton;
	bool m_bDeleteOnClose;

	Internal::Modal* m_Modal;
};

class CloseButton : public Gwen::Controls::Button
{
	VD_UI_CONTROL( CloseButton, Button );

	virtual void Render( Gwen::Skin::Base* skin )
	{
		skin->DrawWindowCloseButton( this, 
			IsDepressed() && IsHovered(), 
			IsHovered() && ShouldDrawHover(), 
			!m_pWindow->IsOnTop() );
	}

	virtual void SetWindow( WindowControl* p ) { m_pWindow = p; }

	WindowControl* m_pWindow;
};

// ============================================================================================== //

class PlayerControl : public Gwen::Controls::WindowControl
{
public:

//	PlayerControl(Gwen::Controls::Base* parent);
	VD_UI_CONTROL( PlayerControl, WindowControl );

private:

	MainMenu*					m_MenuStrip;
	Gwen::Controls::ListBox*	m_TextOutput;
	Gwen::Controls::StatusBar*	m_StatusBar;
	Gwen::Controls::Base* 		m_ActiveControl;
};

// ============================================================================================== //

class MainControl : public DockBase
{
public:
//	MainControl(Gwen::Controls::Base* parent);
	VD_UI_CONTROL( MainControl, DockBase );
	void PrintText( const vd::string& str );
	void PrintStatus( const vd::string& str );

private:

	void OnCategorySelect( Gwen::Controls::Base* pBase, Gwen::Controls::Base* pControl );

	MainMenu*					m_MenuStrip;
	Gwen::Controls::TabControl*	m_TabControl;
	Gwen::Controls::ListBox*	m_TextOutput;
	Gwen::Controls::StatusBar*	m_StatusBar;
	Gwen::Controls::TabButton* 	m_TabButton;
	Gwen::Controls::Base* 		m_ActiveControl;
};

// ============================================================================================== //

VD_INTERFACE_GWEN_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_UI_DEMO_WIDGETS_INCLUDED

// ============================================================================================== //
// end file

