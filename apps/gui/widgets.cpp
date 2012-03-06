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

#include "gui/widgets.h"

// ============================================================================================== //

VD_INTERFACE_GWEN_NAMESPACE_BEGIN();

// ============================================================================================== //

CloseButton::CloseButton(
	Gwen::Controls::Base* parent) : Gwen::Controls::Button(parent)
{
	
}

// ============================================================================================== //

WindowControl::WindowControl(
	Gwen::Controls::Base* parent) : Gwen::Controls::ResizableControl(parent)
{
	m_Modal = NULL;
	m_bDeleteOnClose = false;

	m_TitleBar = VD_NEW( Internal::Dragger, this );
	m_TitleBar->SetHeight( 24 );
	m_TitleBar->SetPadding( Padding( 0, 0, 0, 0 ) );
	m_TitleBar->SetMargin( Margin( 0, 0, 0, 4 ) );
	m_TitleBar->SetTarget( this );
	m_TitleBar->Dock( Pos::Top );

	m_Title = VD_NEW(Gwen::Controls::Label, m_TitleBar );
	m_Title->SetAlignment( Pos::Left | Pos::CenterV );
	m_Title->SetText( "Window" );
	m_Title->Dock( Pos::Fill );
	m_Title->SetPadding( Padding( 8, 0, 0, 0 ) );
	m_Title->SetTextColor( GetSkin()->Colors.Window.TitleInactive );

	m_CloseButton = VD_NEW(CloseButton, m_TitleBar );
	m_CloseButton->SetText( "" );
	m_CloseButton->SetMargin( Margin( 6, 6, 6, 6 ) );
	m_CloseButton->SetSize( 12, 12 );
	m_CloseButton->Dock( Pos::Right );
	m_CloseButton->onPress.Add( this, &WindowControl::CloseButtonPressed );
	m_CloseButton->SetTabable( false );
	m_CloseButton->SetName( "closeButton" );
	m_CloseButton->SetWindow( this );

	//Create a blank content control, dock it to the top - Should this be a ScrollControl?
	m_InnerPanel = VD_NEW(Gwen::Controls::Base, this );
	m_InnerPanel->Dock( Pos::Fill );

	GetResizer( 8 )->Hide();

	BringToFront();

	SetTabable( false );
	Focus();

	SetMinimumSize( Gwen::Point( 100, 40 ) );
	SetClampMovement( true );
	SetKeyboardInputEnabled( false );
}


WindowControl::~WindowControl()
{
	DestroyModal();
}

void WindowControl::MakeModal( bool bDrawBackground )
{
	if ( m_Modal ) return;

	m_Modal = new ControlsInternal::Modal( GetCanvas() );
	SetParent( m_Modal );

	m_Modal->SetShouldDrawBackground( bDrawBackground );
}

void WindowControl::DestroyModal()
{
	if ( !m_Modal ) return;

	// Really should be restoring our parent here.. but we don't know who it is.
	// Assume it's the canvas.
	SetParent( GetCanvas() );
	
	m_Modal->DelayedDelete();
	m_Modal = NULL;
}

bool WindowControl::IsOnTop()
{
	for (Base::List::reverse_iterator iter = GetParent()->Children.rbegin(); iter != GetParent()->Children.rend(); ++iter)
	{
		WindowControl* pWindow = gwen_cast<WindowControl>(*iter);

		if ( !pWindow )
			continue;

		if ( pWindow == this )
			return true;

		return false;
	}

	return false;

}

void WindowControl::Render( Skin::Base* skin )
{
	bool bHasFocus = IsOnTop();

	if ( bHasFocus )
		m_Title->SetTextColor( GetSkin()->Colors.Window.TitleActive );
	else
		m_Title->SetTextColor( GetSkin()->Colors.Window.TitleInactive );

	skin->DrawWindow( this, m_TitleBar->Bottom(), bHasFocus );
}

void WindowControl::RenderUnder( Skin::Base* skin )
{
	BaseClass::RenderUnder( skin );
	skin->DrawShadow( this );
}

void WindowControl::SetTitle(Gwen::UnicodeString title)
{
	m_Title->SetText( title );
}

void WindowControl::SetClosable( bool closeable )
{
	m_CloseButton->SetHidden( !closeable );
}

void WindowControl::SetHidden( bool hidden )
{
	if ( !hidden )
		BringToFront();

	BaseClass::SetHidden(hidden);
}

void WindowControl::Touch()
{
	BaseClass::Touch();
	BringToFront();
}

void WindowControl::CloseButtonPressed( Gwen::Controls::Base* /*pFromPanel*/ )
{
	DestroyModal();

	SetHidden( true );

	if ( m_bDeleteOnClose )
		DelayedDelete();
}


void WindowControl::RenderFocus( Gwen::Skin::Base* /*skin*/ )
{

}

// ============================================================================================== //

MainMenu::MainMenu(
	Gwen::Controls::Base* parent) : Gwen::Controls::Base(parent)
{
	m_MenuStrip = VD_NEW( MenuStrip, this );
	SetSize( 1024, 768 );

	MenuItem* root = m_MenuStrip->AddItem( L"File" );
	root->GetMenu()->AddItem( L"Open", "", "Ctrl+O" )->SetAction( this, &ThisClass::OnOpenFile );
	root->GetMenu()->AddItem( L"Save", "", "Ctrl+S" )->SetAction( this, &ThisClass::OnSaveFile );
	root->GetMenu()->AddItem( L"Save As..", "", "Ctrl+Shift+S" )->SetAction( this, &ThisClass::OnSaveFile );
	root->GetMenu()->AddItem( L"Quit", "", "Ctrl+Q" )->SetAction( this, &ThisClass::OnQuit );
}

void MainMenu::OnOpenFile( 
	Gwen::Controls::Base* )
{
	
}

void MainMenu::OnSaveFile( 
	Gwen::Controls::Base* )
{
	
}

void MainMenu::OnQuit( 
	Gwen::Controls::Base* )
{
	
}

// ============================================================================================== //

PlayerControl::PlayerControl(
	Gwen::Controls::Base* parent) : Gwen::Controls::WindowControl(parent)
{
	Dock( Location::Fill );

	m_MenuStrip = VD_NEW( MainMenu, this );
	m_MenuStrip->Dock( Location::Top );
//	m_MenuStrip->SetHeight( 36 );

	m_StatusBar = VD_NEW( StatusBar, this );
	m_StatusBar->Dock( Location::Bottom );

	CollapsibleList* clist = VD_NEW(CollapsibleList, this );
	(void)clist;
/*
	GetLeft()->GetTabControl()->AddPage( "CollapsibleList", clist );
	GetLeft()->SetWidth( 150 );
	
	m_TextOutput = VD_NEW( ListBox, this );
	m_TabButton = GetBottom()->GetTabControl()->AddPage( "Output", m_TextOutput );
	GetBottom()->SetHeight( 200 );

	PrintText( "Demo Started.\n" );

*/
}

// ============================================================================================== //

MainControl::MainControl(
	Gwen::Controls::Base* parent) : Gwen::Controls::DockBase(parent)
{
	m_TabButton = NULL;
	m_ActiveControl = NULL;

	Dock( Location::Fill );

	m_MenuStrip = VD_NEW( MainMenu, this );
	m_MenuStrip->Dock( Location::Top );
	m_MenuStrip->SetHeight( 30 );

	m_StatusBar = VD_NEW(Gwen::Controls::StatusBar, this );
	m_StatusBar->Dock( Location::Bottom );

	CollapsibleList* clist = VD_NEW(Gwen::Controls::CollapsibleList, this );
	GetLeft()->GetTabControl()->AddPage( "CollapsibleList", clist );
	GetLeft()->SetWidth( 150 );
	
	m_TextOutput = VD_NEW(Gwen::Controls::ListBox, this );
	m_TabButton = GetBottom()->GetTabControl()->AddPage( "Output", m_TextOutput );
	GetBottom()->SetHeight( 200 );

	PrintText( "Demo Started.\n" );
}

void MainControl::OnCategorySelect( 
	Gwen::Controls::Base* base, 
	Gwen::Controls::Base* control )
{
	(void)(base);

	if ( m_ActiveControl )
	{
		m_ActiveControl->Hide();
	}

	control->Show();
	m_ActiveControl = control;
}

void MainControl::PrintText( const vd::string& str )
{
	m_TextOutput->AddItem( str );
	m_TextOutput->ScrollToBottom();
}

void MainControl::PrintStatus( const vd::string& str )
{
	m_StatusBar->SetText( str );
}

// ============================================================================================== //

VD_INTERFACE_GWEN_NAMESPACE_END();

// ============================================================================================== //
// end file

