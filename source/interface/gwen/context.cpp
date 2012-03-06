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

#include "interface/gwen/context.h"

#include "core/core.h"
#include "core/asserts.h"
#include "core/functions.h"

#include "graphics/graphics.h"
#include "graphics/gl/opengl.h"

// ============================================================================================== //

VD_INTERFACE_GWEN_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_IMPORT(Core, Function);
VD_IMPORT(Core, Symbol);

// ============================================================================================== //

Context::Context(
	Graphics::Context* graphics) :
	Object(),
	m_Width(0),
	m_Height(0),
	m_Skin(NULL),
	m_Input(NULL),
	m_Canvas(NULL),
	m_Renderer(NULL),
	m_Graphics(graphics)
{
	// EMPTY!
}

vd::status
Context::Destroy()
{
	VD_SAFE_DELETE( m_Input );
	VD_SAFE_DELETE( m_Canvas );
	VD_SAFE_DELETE( m_Skin );
	VD_SAFE_DELETE( m_Renderer );	
	return Status::Code::Success;
}

Canvas*
Context::GetCanvas(void)
{
	return m_Canvas;	
}

vd::status 
Context::Initialize(
	vd::i32 width, vd::i32 height)
{
	Destroy();

	m_Renderer = VD_NEW(Renderer, m_Graphics);
	
	m_Skin = VD_NEW(Skin);
	m_Skin->SetRender( m_Renderer );
//	m_Skin->Init("DefaultSkin.png");

	m_Canvas = VD_NEW(Canvas, m_Skin );
	m_Canvas->Initialize();
	m_Canvas->SetSize( width, height - 24 );
//	m_Canvas->SetDrawBackground( true );
//	m_Canvas->SetBackgroundColor( Gwen::Color( 150, 170, 170, 255 ) );

	m_Input = VD_NEW(Input);
	m_Input->Initialize( m_Canvas );
	return Status::Code::Success;
}

vd::status
Context::OnShutdown(
	const Event&)
{
	Destroy();
	return Status::Code::Success;
}

vd::status
Context::OnUpdate(
	const Event& event)
{
	vdAssert(m_Input);
	m_Input->OnEvent(event);
    return Status::Code::Success;
}

vd::status
Context::OnKeyEvent(
	const Event& event)
{
	vdAssert(m_Input);
	m_Input->OnEvent(event);
    return Status::Code::Success;
}

vd::status 
Context::OnMouseDown(const Event & event)
{
	vdAssert(m_Input);
	m_Input->OnEvent(event);
    return Status::Code::Success;
}

vd::status 
Context::OnMouseUp(const Event & event)
{
	vdAssert(m_Input);
	m_Input->OnEvent(event);
    return Status::Code::Success;
}

vd::status 
Context::OnMouseMotion(const Event & event)
{
	vdAssert(m_Input);
	m_Input->OnEvent(event);
    return Status::Code::Success;
}

vd::status 
Context::OnMouseWheel(const Event & event)
{
	vdAssert(m_Input);
	m_Input->OnEvent(event);
    return Status::Code::Success;
}

vd::status 
Context::OnCloseEvent(const Event & event)
{
	Destroy();
	return Status::Code::Success;
}

vd::status 
Context::Render(void)
{
	if(m_Canvas == NULL)
		return Status::Code::Reject;

    vd::i32 width = m_Width;
    vd::i32 height = m_Height;

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	glOrtho( 0, width, height, 0, -1.0, 1.0);

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
	glViewport(0, 0, width, height);

	m_Canvas->RenderCanvas();

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
    return Status::Code::Success;
}

vd::status 
Context::OnDisplay(
	const Event&)
{
	return Render();
}

vd::status
Context::OnResize(
    const Event& event)
{
	if(m_Canvas == NULL)
		return Status::Code::Reject;

    vd::i32 width = event.Size.Width;
    vd::i32 height = event.Size.Height;

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	glOrtho( 0, width, height, 0, -1.0, 1.0);

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
	glViewport(0, 0, width, height);

	m_Width = width;
	m_Height = height;
	m_Canvas->SetSize( m_Width, m_Height );

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
    return Status::Code::Success;
}


// ============================================================================================== //

VD_IMPLEMENT_OBJECT(Context, Symbol(0, "Context"), Symbol(0, "Object"));

// ============================================================================================== //

VD_INTERFACE_GWEN_NAMESPACE_END();

// ============================================================================================== //
// end file

