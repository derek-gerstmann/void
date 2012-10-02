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

#ifndef VD_INTERFACE_GWEN_RENDERER_INCLUDED
#define VD_INTERFACE_GWEN_RENDERER_INCLUDED

// ============================================================================================== //

#include "interface/gwen/gwen.h"
#include "graphics/gl/opengl.h"

// ============================================================================================== //

extern "C"
{

// Freetype GL collides with the macro definition in GLEW for GL_TYPE -- 	
#ifdef GL_TYPE
#undef GL_TYPE
#endif

#include "font-manager.h"

}

// ============================================================================================== //

VD_INTERFACE_GWEN_NAMESPACE_BEGIN();

// ============================================================================================== //

namespace Renderer {

// ============================================================================================== //

class Base : public Render::Base
{
public:

	struct Vertex
	{
		float x, y, z;
		float u, v;
		unsigned char r, g, b, a;
	};

	Base(Graphics::Context* context);
	virtual ~Base();

	void Destroy();
	virtual void Begin();
	virtual void End();

	virtual void SetDrawColor( Gwen::Color color );
	virtual void DrawFilledRect( Gwen::Rect rect );

	void StartClip();
	void EndClip();

	void DrawTexturedRect( Gwen::Texture* pTexture, Gwen::Rect pTargetRect, float u1=0.0f, float v1=0.0f, float u2=1.0f, float v2=1.0f );
	void LoadTexture( Gwen::Texture* pTexture );
	void FreeTexture( Gwen::Texture* pTexture );
	Gwen::Color PixelColour( Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color& col_default );

	void LoadFont( Gwen::Font* font );
	void FreeFont( Gwen::Font* pFont );

//		virtual void DrawLinedRect( Gwen::Rect rect );
	virtual void DrawShavedCornerRect( Gwen::Rect rect, bool bSlight = false );
	void RenderText( Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString& text );
	Gwen::Point MeasureText( Gwen::Font* pFont, const Gwen::UnicodeString& text );

protected:

	void SetActiveTexture( unsigned int id );
	void RenderDefaultFont(	Gwen::Point pos, const Gwen::UnicodeString& text );
	Gwen::Point MeasureDefaultFont( const Gwen::UnicodeString& text );

	void DrawBox(int mode, float minx, float miny, float maxx, float maxy, float rad);

	static const int	MaxVerts = 1024;

	void Flush(GLenum primitive=GL_TRIANGLES);
	void AddVert( float x, float y, float u = 0.0f , float v = 0.0f );

	Gwen::Color			m_Color;
	int					m_iVertNum;
	Vertex				m_Vertices[ MaxVerts ];
	font_manager_t* 	m_FontManager;
	Graphics::Context*	m_Graphics;
	Graphics::Geometry* m_Geometry;
};

// ============================================================================================== //

}

// ============================================================================================== //

VD_INTERFACE_GWEN_NAMESPACE_END();

// ============================================================================================== //

#endif	// VD_INTERFACE_GWEN_RENDERER_INCLUDED

// ============================================================================================== //
// end file
