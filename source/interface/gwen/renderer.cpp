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

#include "interface/gwen/renderer.h"
#include "core/filesystem.h"
#include "graphics/gl/opengl.h"
#include "graphics/image.h"

#include "Gwen/Utility.h"
#include "Gwen/Font.h"
#include "Gwen/Texture.h"

#include "font-manager.h"
#include "texture-font.h"
#include "texture-atlas.h"
#include "vertex-buffer.h"

#include <math.h>

// ============================================================================================== //

VD_INTERFACE_GWEN_NAMESPACE_BEGIN();

// ============================================================================================== //

namespace DefaultFont 
{
	#include "arial-16.h"
	static GLuint id = 0;
}

void Renderer::RenderDefaultFont( 
	Gwen::Point pos, 
	const Gwen::UnicodeString& text )
{
	vec2 pen;
	pen.x = pos.x;
	pen.y = pos.y;

	if(DefaultFont::id == 0)
	{
		GLuint texid;
		glGenTextures( 1, &texid );
		glBindTexture( GL_TEXTURE_2D, texid );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_ALPHA, DefaultFont::font.tex_width, DefaultFont::font.tex_height,
					  0, GL_ALPHA, GL_UNSIGNED_BYTE, DefaultFont::font.tex_data );
		glBindTexture( GL_TEXTURE_2D, texid );
		DefaultFont::id = texid;
	}
	
	SetActiveTexture( m_FontManager->atlas->id );

    size_t i, j;
	for ( i=0; i<text.length(); i++ )
    {
        DefaultFont::texture_glyph_t* glyph = 0;
        for( j=0; j<DefaultFont::font.glyphs_count; ++j)
        {
            if( DefaultFont::font.glyphs[j].charcode == text[i] )
            {
                glyph = &DefaultFont::font.glyphs[j];
                break;
            }
        }

        if( !glyph )
        {
            continue;
        }
        
        vd::i32 x = pen.x + glyph->offset_x;
        vd::i32 y = pen.y + glyph->offset_y;
        
        vd::i32 w  = glyph->width;
        vd::i32 h  = glyph->height;
        
        AddVert(x+0, y+0, glyph->s0, glyph->t0 );
        AddVert(x+0, y-h, glyph->s0, glyph->t1 );
        AddVert(x+w, y-h, glyph->s1, glyph->t1 );
        
        AddVert(x+0, y+0, glyph->s0, glyph->t0 );
        AddVert(x+w, y-h, glyph->s1, glyph->t1 );
        AddVert(x+w, y+0, glyph->s1, glyph->t0 );
        
        pen.x += glyph->advance_x;
        pen.y += glyph->advance_y;

    }
}

Gwen::Point 
Renderer::MeasureDefaultFont( 
	const Gwen::UnicodeString& text )
{
	int w = 0;
	int h = 0;
	
	vec2 pen;
	pen.x = 0;
	pen.y = 0;

    size_t i, j;
	for ( i=0; i<text.length(); i++ )
    {
        DefaultFont::texture_glyph_t* glyph = 0;
        for( j=0; j<DefaultFont::font.glyphs_count; ++j)
        {
            if( DefaultFont::font.glyphs[j].charcode == text[i] )
            {
                glyph = &DefaultFont::font.glyphs[j];
                break;
            }
        }
        if( !glyph )
        {
            continue;
        }
        
        vd::i32 x0 = pen.x + glyph->offset_x;
        vd::i32 y0 = pen.y + glyph->offset_y;
        
        vd::i32 x1  = (vd::i32)( x0 + glyph->width );
        vd::i32 y1  = (vd::i32)( y0 - glyph->height );
        
		w = x1;
		h = y1;

        pen.x += glyph->advance_x;
        pen.y += glyph->advance_y;

    }

	w = w < 0 ? -w : w;
	h = h < 0 ? -h : h;
	
	return Gwen::Point( w, h );			
}

Renderer::Renderer(
	Graphics::Context* context) :
	Gwen::Renderer::Base(),
	m_iVertNum(0),
	m_FontManager(NULL),
	m_Graphics(context)
{

}

Renderer::~Renderer()
{
	Destroy();
}

void Renderer::Destroy()
{
	if(m_FontManager != NULL)
		font_manager_delete(m_FontManager);
	m_FontManager = NULL;

}

void Renderer::Begin()
{
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
//	glAlphaFunc( GL_GREATER, 1.0f );	
	glEnable ( GL_BLEND );
}

void Renderer::End()
{
	Flush();
}

void Renderer::Flush(
	GLenum primitive)
{
	if ( m_iVertNum == 0 ) return;

	glVertexPointer( 3, GL_FLOAT,  sizeof(Vertex), (void*) &m_Vertices[0].x );
	glEnableClientState( GL_VERTEX_ARRAY );

	glColorPointer( 4, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)&m_Vertices[0].r );
	glEnableClientState( GL_COLOR_ARRAY );

	glTexCoordPointer( 2, GL_FLOAT, sizeof(Vertex), (void*) &m_Vertices[0].u );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	glDrawArrays( primitive, 0, (GLsizei) m_iVertNum );

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	m_iVertNum = 0;
	glFlush();
}

void Renderer::AddVert( 
	float x, float y, float u, float v )
{
	if ( m_iVertNum >= MaxVerts-1 )
	{
		Flush();
	}

	m_Vertices[ m_iVertNum ].x = (float)x;
	m_Vertices[ m_iVertNum ].y = (float)y;
	m_Vertices[ m_iVertNum ].z = 0.5f;
	m_Vertices[ m_iVertNum ].u = u;
	m_Vertices[ m_iVertNum ].v = v;

	m_Vertices[ m_iVertNum ].r = m_Color.r;
	m_Vertices[ m_iVertNum ].g = m_Color.g;
	m_Vertices[ m_iVertNum ].b = m_Color.b;
	m_Vertices[ m_iVertNum ].a = m_Color.a;

	m_iVertNum++;
}

/*
void 
Renderer::DrawShavedCornerRect( Gwen::Rect rect, bool slight )
{
	float vec[7][2]= {{0.195, 0.02}, {0.383, 0.067}, {0.55, 0.169}, {0.707, 0.293},
					  {0.831, 0.45}, {0.924, 0.617}, {0.98, 0.805}};
	int a;

	SetActiveTexture( 0 );

	Translate( rect );

	float minx = rect.x;
	float miny = rect.y;
	float maxx = rect.x + rect.w;
	float maxy = rect.y + rect.h;

	float rad = 24.0f * Scale();
	if(slight)
		rad = 12.0f * Scale();
	
	float maxrady = (maxy - miny - 10.0f)/2.0f;
	float maxradx = (maxx - minx - 10.0f)/2.0f;
	if(rad > maxrady)
		rad= maxrady;
	if(rad > maxradx)
		rad= maxradx;

	for(a=0; a<7; a++) {
		vec[a][0]*= rad; vec[a][1]*= rad;
	}

	Flush();

	AddVert(maxx-rad, miny);
	for(a=0; a<7; a++) {
		AddVert(maxx-rad+vec[a][0], miny+vec[a][1]);
	}
	AddVert(maxx, miny+rad);

	AddVert(maxx, maxy-rad);
	for(a=0; a<7; a++) {
		AddVert(maxx-vec[a][1], maxy-rad+vec[a][0]);
	}
	AddVert(maxx-rad, maxy);

	AddVert(minx+rad, maxy);
	for(a=0; a<7; a++) {
		AddVert(minx+rad-vec[a][0], maxy-vec[a][1]);
	}
	AddVert(minx, maxy-rad);

	AddVert(minx, miny+rad);
	for(a=0; a<7; a++) {
		AddVert(minx+vec[a][1], miny+rad-vec[a][0]);
	}
	AddVert(minx+rad, miny);
	if(slight)
		Flush(GL_LINE_STRIP);
	else
		Flush(GL_POLYGON);
}
*/

void Renderer::DrawFilledRect( 
	Gwen::Rect rect )
{
	SetActiveTexture( 0 );

	Translate( rect );

	AddVert( rect.x, rect.y );
	AddVert( rect.x+rect.w, rect.y );
	AddVert( rect.x, rect.y + rect.h );

	AddVert( rect.x+rect.w, rect.y );
	AddVert( rect.x+rect.w, rect.y+rect.h );
	AddVert( rect.x, rect.y + rect.h );
}

void Renderer::SetDrawColor(
	Gwen::Color color)
{
	glColor4ubv( (GLubyte*)&color );
	m_Color = color;
}

void Renderer::StartClip()
{
	Flush();
	Gwen::Rect rect = ClipRegion();

	// Renderer's coords are from the bottom left
	// so we need to translate them here.
	{
		GLint view[4];
		glGetIntegerv( GL_VIEWPORT, &view[0] );
		rect.y = view[3] - (rect.y + rect.h);
	}

	glScissor( rect.x * Scale(), rect.y * Scale(), rect.w * Scale(), rect.h * Scale() );
	glEnable( GL_SCISSOR_TEST );
};

void Renderer::EndClip()
{
	Flush();
	glDisable( GL_SCISSOR_TEST );
	
};

void Renderer::SetActiveTexture( 
	unsigned int id )
{
	GLuint bound;
	GLboolean enabled;

	glGetBooleanv(GL_TEXTURE_2D, &enabled);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *)&bound);

	if ( !enabled || id != bound )
	{
		Flush();

		glBindTexture( GL_TEXTURE_2D, id );
		glEnable(GL_TEXTURE_2D);
	}		
}

void Renderer::DrawTexturedRect( 
	Gwen::Texture* pTexture, 
	Gwen::Rect rect, 
	float s0, float t0, float s1, float t1 )
{
	GLuint* tex = (GLuint*)pTexture->data;

	// Missing image, not loaded properly?
	if ( !tex )
	{
		return DrawMissingImage( rect );
	}

	Translate( rect );

	SetActiveTexture( *tex );
	
	AddVert( rect.x, rect.y,				s0, t0 );
	AddVert( rect.x+rect.w, rect.y,			s1, t0 );
	AddVert( rect.x, rect.y + rect.h,		s0, t1 );

	AddVert( rect.x+rect.w, rect.y,			s1, t0 );
	AddVert( rect.x+rect.w, rect.y+rect.h, 	s1, t1 );
	AddVert( rect.x, rect.y + rect.h, 		s0, t1 );			
}

void Renderer::LoadTexture( 
	Gwen::Texture* pTexture )
{
	Graphics::ImageFormat format;
	Graphics::ImageBuffer8u pixels;
	
	const std::string& filename = pTexture->name.Get();

	Runtime::Context* runtime = m_Graphics->GetRuntime();
	Core::FileSystem* filesystem = runtime->GetFileSystem();
	std::string fullpath = filesystem->Resolve(filename);

	Graphics::ImageInput& input = Graphics::ImageInput::GetInstance();
	if(input.Open(fullpath, format) != Status::Code::Success)
	{
		vdLogGlobalWarning("Failed to read image from file '%s'\n", fullpath.c_str() );
		pTexture->failed = true;
		return;
	}
	
	if(input.Read(pixels) != Status::Code::Success)
	{
		vdLogGlobalWarning("Failed to read image from file '%s'\n", fullpath.c_str());
		pTexture->failed = true;
		return;
	}

	GLenum internal = GL_RGBA;
	GLenum layout = GL_RGBA;
	switch(format.Channels.Count)
	{
		case 1:
			layout = GL_LUMINANCE;
			break;
		case 2:
			layout = GL_LUMINANCE_ALPHA;
			break;
		case 3:
			layout = GL_RGB;
			break;
		case 4:
			layout = GL_RGBA;
			break;
		default:
			vdLogGlobalWarning("Invalid channel layout for creating texture from image file '%s'\n", fullpath.c_str());
			pTexture->failed = true;
			return;
	};

	GLuint* pglTexture = new GLuint;
	pTexture->data = pglTexture;
	pTexture->width = format.Width;
	pTexture->height = format.Height;
	
	glGenTextures( 1, pglTexture );
	glBindTexture( GL_TEXTURE_2D, *pglTexture );
	glTexImage2D( GL_TEXTURE_2D, 0, internal, pTexture->width, pTexture->height, 0, layout, 
				  GL_UNSIGNED_BYTE, (const GLvoid*)pixels.Data );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	
	pixels.Destroy();

}

void Renderer::FreeTexture( 
	Gwen::Texture* pTexture )
{
	GLuint* tex = (GLuint*)pTexture->data;
	if ( !tex ) return;

	glDeleteTextures( 1, tex );
	delete tex;
	pTexture->data = NULL;
}

Gwen::Color 
Renderer::PixelColour( 
	Gwen::Texture* pTexture, 
	unsigned int x, unsigned int y, 
	const Gwen::Color& col_default )
{
	GLuint* tex = (GLuint*)pTexture->data;
	if ( !tex ) return col_default;

	unsigned int iPixelSize = sizeof(unsigned char) * 4;

	SetActiveTexture( *tex );
	
	unsigned char* data = (unsigned char*) malloc( iPixelSize * pTexture->width * pTexture->height );

	glGetTexImage( GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	unsigned int iOffset = (y * pTexture->width + x) * 4;

	Gwen::Color c;
	c.r = data[0 + iOffset];
	c.g = data[1 + iOffset];
	c.b = data[2 + iOffset];
	c.a = data[3 + iOffset];

	//
	// Retrieving the entire texture for a single pixel read
	// is kind of a waste - maybe cache this pointer in the texture
	// data and then release later on? It's never called during runtime
	// - only during initialization.
	//
	free( data );

	return c;
}

void Renderer::LoadFont( 
	Gwen::Font* pFont )
{	
	pFont->realsize = pFont->size * Scale();

	if(!m_FontManager)
		m_FontManager = font_manager_new( 1024, 1024, 1 );

	const std::string& filename = Gwen::Utility::UnicodeToString( pFont->facename );
	Runtime::Context* runtime = m_Graphics->GetRuntime();
	Core::FileSystem* filesystem = runtime->GetFileSystem();
	std::string fullpath = filesystem->Resolve(filename);

	texture_font_t* font = font_manager_get_from_filename( m_FontManager, fullpath.c_str(), pFont->realsize );
	if(!font)
	{
		font = font_manager_get_from_description( m_FontManager, filename.c_str(), pFont->realsize, 0, 0);
	}
	pFont->data = font;
}

void Renderer::FreeFont( 
	Gwen::Font* pFont )
{
	if ( !pFont->data ) return;

	texture_font_t* font = ((texture_font_t* )pFont->data);
	font_manager_delete_font( m_FontManager, font );
	pFont->data = NULL;
}

void Renderer::RenderText( 
	Gwen::Font* pFont, Gwen::Point pos, 
	const Gwen::UnicodeString& text )
{
	Translate( pos.x, pos.y );

	texture_font_t* font = ((texture_font_t* )pFont->data);
	if  ( !font )
	{
		RenderDefaultFont(pos, text);
		return;
	}

	if ( fabs( pFont->realsize - pFont->size * Scale() ) > 2 )
	{
		FreeFont( pFont );
		LoadFont( pFont );
	}

	font = ((texture_font_t* )pFont->data);
	vd::f32 scale = pFont->size * Scale();
	
	vd::v2f32 pen;
	pen.x = pos.x;
	pen.y = pos.y;
	pen.y += font->ascender;
	SetActiveTexture( m_FontManager->atlas->id );

	for ( size_t i=0; i<text.length(); i++ )
	{
		texture_glyph_t *glyph = texture_font_get_glyph( font, text[i] );
        if( glyph == NULL )
        	continue;

        vd::i32 kerning = 0;
        if( i > 0)
        {
            kerning = texture_glyph_get_kerning( glyph, text[i-1] );
        }

        pen.x += kerning;// * scale;

		vd::f32 hh = (glyph->height * 0.5f);
		vd::f32 hw = (glyph->width * 0.5f);
		vd::f32 dy = (glyph->offset_y - hh);

    	vd::f32 ox = pen.x + glyph->offset_x + hw;
    	vd::f32 oy = pen.y - dy + scale * 0.5f;

		vd::f32 x0 = ox - hw;
		vd::f32 x1 = ox + hw;
		
		vd::f32 y0 = oy - hh;
	    vd::f32 y1 = oy + hh; 

		vd::f32 s0 = glyph->s0;
		vd::f32 t0 = glyph->t0;

		vd::f32 s1 = glyph->s1;
		vd::f32 t1 = glyph->t1;

        AddVert( x0, y0, s0, t0 );
        AddVert( x1, y1, s1, t1 );
        AddVert( x1, y0, s1, t0 );
		
        AddVert( x0, y0, s0, t0 );
        AddVert( x0, y1, s0, t1 );
        AddVert( x1, y1, s1, t1 );
        
		pen.x += glyph->advance_x;// * scale;
		pen.y += glyph->advance_y;// * scale;
	}
}

Gwen::Point Renderer::MeasureText( 
	Gwen::Font* pFont, 
	const Gwen::UnicodeString& text )
{
	texture_font_t* font = ((texture_font_t* )pFont->data);

	if ( !font || fabs( pFont->realsize - pFont->size * Scale() ) > 2 )
	{
		FreeFont( pFont );
		LoadFont( pFont );
		font = ((texture_font_t* )pFont->data);
	}

	if  ( !font )
	{
		return MeasureDefaultFont(text);
	}

	vd::f32 scale = pFont->size * Scale();
	vd::string str = Gwen::Utility::UnicodeToString(text);

	vd::v2f32 pen;
	pen.x = 0.0f;
	pen.y = 0.0f;
	pen.y += font->ascender + scale;
	for ( size_t i=0; i<text.length(); i++ )
	{
		texture_glyph_t *glyph = texture_font_get_glyph( font, text[i] );
        if( glyph == NULL )
        	continue;

        vd::i32 kerning = 0;
        if( i > 0)
        {
            kerning = texture_glyph_get_kerning( glyph, text[i-1] );
        }

        pen.x += kerning;// * scale;        
		pen.x += glyph->advance_x;// * scale;
		pen.y += glyph->advance_y;// * scale;
	}
	return Gwen::Point( pen.x, pen.y );			
}

// ============================================================================================== //

VD_INTERFACE_GWEN_NAMESPACE_END();

// ============================================================================================== //
// end file
