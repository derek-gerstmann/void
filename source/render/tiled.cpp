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

#include "render/tiled.h"
#include "core/memory.h"
#include "core/asserts.h"
#include "core/builtins.h"

#include "graphics/gl/opengl.h"
#if defined(VD_USE_GLUT)
#include "graphics/gl/glut.h"
#endif

// ============================================================================================== //

VD_RENDER_NAMESPACE_BEGIN();

// ============================================================================================== //

using VD_CORE_SCOPE::Memory;

// ============================================================================================== //

TiledRenderer::TiledRenderer() :
	Object(),
	m_Rows(0),
	m_Columns(0),
	m_CurrentTile(-1),
	m_CurrentTileWidth(0),
	m_CurrentTileHeight(0),
	m_CurrentRow(0),
	m_CurrentColumn(0),
	m_RowOrder(RowOrder::BottomToTop),
	m_UsePerspective(true),
    m_IsDone(true)
{
	Memory::SetBytes(&m_Tile, 0, sizeof(TileRegion));
	Memory::SetBytes(&m_Image, 0, sizeof(ImageRegion));
	Memory::SetBytes(&m_Frustum, 0, sizeof(Frustum));
	Memory::SetBytes(m_SavedViewport, 0, sizeof(m_SavedViewport));
}

TiledRenderer::TiledRenderer(
	vd::i32 img_width,
	vd::i32 img_height,
	vd::i32 tile_width,
	vd::i32 tile_height,
	vd::i32 border,
    vd::f32 reduce,
	RowOrder::Value RowOrder
) :
	Object(),
	m_Rows(0),
	m_Columns(0),
	m_CurrentTile(-1),
	m_CurrentTileWidth(0),
	m_CurrentTileHeight(0),
	m_CurrentRow(0),
	m_CurrentColumn(0),
	m_RowOrder(RowOrder),
	m_UsePerspective(true),
    m_IsDone(true)
{
	Memory::SetBytes(&m_Tile, 0, sizeof(TileRegion));
	Memory::SetBytes(&m_Image, 0, sizeof(ImageRegion));
	Memory::SetBytes(&m_Frustum, 0, sizeof(Frustum));
	Memory::SetBytes(m_SavedViewport, 0, sizeof(m_SavedViewport));

	SetImageSize(img_width, img_height);
	SetTileSize(tile_width, tile_height, border, reduce);
}


void TiledRenderer::SetTileSize(
	vd::i32 width,
	vd::i32 height,
	vd::i32 border,
    vd::f32 reduce)
{
	vdAssert(border >= 0);
	vdAssert(width >= 1);
	vdAssert(height >= 1);
	vdAssert(width >= 2 * border);
	vdAssert(height >= 2 * border);

	m_Tile.Width = width;
	m_Tile.Height = height;
	m_Tile.InnerWidth = width - 2 * border;
	m_Tile.InnerHeight = height - 2 * border;
    m_Tile.Border = border;
    m_Tile.Reduce = reduce;
}

void TiledRenderer::SetTileBufferData(
	vd::i32 format,
	vd::i32 type,
	void* buffer)
{
	m_Tile.Buffer = buffer;
	m_Tile.Format = format;
	m_Tile.Type = type;
}

void TiledRenderer::SetImageSize(
	vd::i32 width,
	vd::i32 height)
{
	m_Image.Width = width;
	m_Image.Height = height;
}

void TiledRenderer::SetImageBufferData(
	vd::i32 format,
	vd::i32 type,
	void* buffer)
{
	m_Image.Buffer = buffer;
	m_Image.Format = format;
	m_Image.Type = type;
}

void
TiledRenderer::SetupTiles()
{
    m_Columns = Core::DivideUp(m_Image.Width, m_Tile.InnerWidth);
    m_Rows = Core::DivideUp(m_Image.Height, m_Tile.InnerHeight);
    m_CurrentTile = 0;
    m_IsDone = false;

	vdLogDebug("Tiled Config: %d x %d (%d x %d => %d x %d / %f)", 
		m_Rows, m_Columns, m_Tile.Width, m_Tile.Height, 
		m_Rows*m_Tile.Width, m_Columns*m_Tile.Height, m_Tile.Reduce);
	
    vdAssert(m_Columns >= 0);
    vdAssert(m_Rows >= 0);
}

void
TiledRenderer::SetOrthographic(
    vd::f32 left, vd::f32 right,
    vd::f32 bottom, vd::f32 top,
    vd::f32 zNear, vd::f32 zFar)
{
    m_UsePerspective = false;
    m_Frustum.Left = left;
    m_Frustum.Right = right;
    m_Frustum.Bottom = bottom;
    m_Frustum.Top = top;
    m_Frustum.Near = zNear;
    m_Frustum.Far = zFar;
}


void
TiledRenderer::SetFrustum(
    vd::f32 left, vd::f32 right,
    vd::f32 bottom, vd::f32 top,
    vd::f32 zNear, vd::f32 zFar)
{
    m_UsePerspective = true;
    m_Frustum.Left = left;
    m_Frustum.Right = right;
    m_Frustum.Bottom = bottom;
    m_Frustum.Top = top;
    m_Frustum.Near = zNear;
    m_Frustum.Far = zFar;
}


void
TiledRenderer::SetPerspective(
    vd::f32 fovy, vd::f32 aspect,
    vd::f32 zNear, vd::f32 zFar)
{
    vd::f32 xmin, xmax, ymin, ymax;

    ymax = zNear * Core::Tan(Core::Deg2Rad(fovy / vd::f32(Constants::Two)));
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;
    SetFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}


void
TiledRenderer::BeginTile()
{
    GLint tile_width, tile_height;

    if(m_CurrentTile <= 0)
    {
        SetupTiles();
        glGetIntegerv(GL_VIEWPORT, m_SavedViewport);
    }

    switch(m_RowOrder)
    {
        case RowOrder::BottomToTop:
        {
            m_CurrentRow = m_CurrentTile / m_Columns;
            m_CurrentColumn = m_CurrentTile % m_Columns;
            break;
        }
        case RowOrder::TopToBottom:
        default:
        {
            m_CurrentRow = m_Rows - (m_CurrentTile / m_Columns) - 1;
            m_CurrentColumn = m_CurrentTile % m_Columns;
            break;
        }
    };

    vdAssert(m_CurrentRow < m_Rows);
    vdAssert(m_CurrentColumn < m_Columns);

    GLint border = m_Tile.Border;

    if(m_Rows == 1 && m_Tile.Height >= m_Image.Height)
    	tile_height = m_Image.Height;
	else if(m_CurrentRow < m_Rows - 1)
        tile_height = m_Tile.Height;
    else
        tile_height = m_Image.Height - (m_Rows - 1) * (m_Tile.InnerHeight) + 2 * border;

	if(m_Columns == 1 && m_Tile.Width >= m_Image.Width)
    	tile_width = m_Image.Width;
    else if(m_CurrentColumn < m_Columns - 1)
        tile_width = m_Tile.Width;
    else
        tile_width = m_Image.Width - (m_Columns - 1) * (m_Tile.InnerWidth) + 2 * border;

    m_CurrentTileWidth = tile_width;
    m_CurrentTileHeight = tile_height;

    vdLogDebug("BeginTile[%d, %d] => [%d, %d]", 
        m_CurrentColumn, m_CurrentRow, 
        m_CurrentTileWidth, m_CurrentTileHeight);
}

void
TiledRenderer::Bind()
{    
    GLint matrixMode;

    glViewport(0, 0, m_CurrentTileWidth, m_CurrentTileHeight);
    glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    vd::f32 tile_reduce = m_Tile.Reduce < 1.0f ? 1.0f : m_Tile.Reduce;

    GLint tile_x = m_CurrentColumn;
    GLint tile_y = m_CurrentRow;
    GLint img_width = m_Image.Width * tile_reduce;
    GLint img_height = m_Image.Height * tile_reduce;
    GLint inner_width = m_Tile.InnerWidth * tile_reduce;
    GLint inner_height = m_Tile.InnerHeight * tile_reduce;
    GLint tile_width = m_CurrentTileWidth * tile_reduce;
    GLint tile_height = m_CurrentTileHeight * tile_reduce;
    GLint tile_border = m_Tile.Border * tile_reduce;

    vd::f32 left_plane = m_Frustum.Left;
    vd::f32 right_plane = m_Frustum.Right;
    vd::f32 top_plane = m_Frustum.Top;
    vd::f32 bottom_plane = m_Frustum.Bottom;
    vd::f32 frustum_width = right_plane - left_plane;
    vd::f32 frustum_height = top_plane - bottom_plane;
    vd::f32 frustum_near = m_Frustum.Near;
    vd::f32 frustum_far = m_Frustum.Far;

    vd::f32 tile_left = left_plane + (frustum_width) * (tile_x * inner_width - tile_border) / img_width;
    vd::f32 tile_right = tile_left + (frustum_width) * tile_width / img_width;
    vd::f32 tile_bottom = bottom_plane + (frustum_height) * (tile_y * inner_height - tile_border) / img_height;
    vd::f32 tile_top = tile_bottom + (frustum_height) * tile_height / img_height;

    if(m_UsePerspective)
        glFrustum(tile_left, tile_right, tile_bottom, tile_top, frustum_near, frustum_far);
    else
        glOrtho(tile_left, tile_right, tile_bottom, tile_top, frustum_near, frustum_far);

    glMatrixMode(matrixMode);
}

void
TiledRenderer::Unbind()
{    
    GLint matrixMode;
    glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(matrixMode);
}

bool
TiledRenderer::EndTile()
{
    bool more = false;
    GLint old_row_length;
    GLint old_skip_rows;
    GLint old_skip_pixels; 
    GLint old_alignment;

    vdAssert(m_CurrentTile >= 0);
    glFlush();

    if(m_Tile.Buffer)
    {
        m_Tile.Reduce = m_Tile.Reduce <= 1.0f ? 1.0f : m_Tile.Reduce;

#if VD_USE_CONDENSED_TILE        
        GLint src_x = m_Tile.Border;
        GLint src_y = m_Tile.Border;
        GLint src_width = (m_CurrentTileWidth - 2 * m_Tile.Border);
        GLint src_height = (m_CurrentTileHeight - 2 * m_Tile.Border);
//        GLint src_width = m_Tile.InnerWidth;
//        GLint src_height = m_Tile.InnerHeight;
#else
        GLint src_x = 0;
        GLint src_y = 0;
        GLint src_width = m_CurrentTileWidth;
        GLint src_height = m_CurrentTileHeight;
#endif
//        glGetIntegerv(GL_PACK_ALIGNMENT, &old_alignment);
        glPixelStorei(GL_PACK_ALIGNMENT, 4);
        if(src_x < m_Image.Width && src_y < m_Image.Height)
	        glReadPixels(src_x, src_y, src_width, src_height, m_Tile.Format, m_Tile.Type, m_Tile.Buffer);
//        glPixelStorei(GL_PACK_ALIGNMENT, old_alignment);
    }

    if(m_Image.Buffer)
    {
		glGetIntegerv(GL_PACK_ROW_LENGTH, &old_row_length);
		glGetIntegerv(GL_PACK_SKIP_ROWS, &old_skip_rows);
		glGetIntegerv(GL_PACK_SKIP_PIXELS, &old_skip_pixels);
		glGetIntegerv(GL_PACK_ALIGNMENT, &old_alignment);
	
        GLint src_x = m_Tile.Border;
        GLint src_y = m_Tile.Border;
        GLint src_width = m_CurrentTileWidth - 2 * m_Tile.Border;
        GLint src_height = m_CurrentTileHeight - 2 * m_Tile.Border;
        GLint dst_x = m_Tile.InnerWidth * m_CurrentColumn;
        GLint dst_y = m_Tile.InnerHeight * m_CurrentRow;

        glPixelStorei(GL_PACK_ROW_LENGTH, m_Image.Width);
        glPixelStorei(GL_PACK_SKIP_ROWS, dst_y);
        glPixelStorei(GL_PACK_SKIP_PIXELS, dst_x);
        // glPixelStorei(GL_PACK_ALIGNMENT, 1);
        if(src_x < m_Image.Width && src_y < m_Image.Height)
	        glReadPixels(src_x, src_y, src_width, src_height, m_Image.Format, m_Image.Type, m_Image.Buffer);

		glPixelStorei(GL_PACK_ROW_LENGTH, old_row_length);
		glPixelStorei(GL_PACK_SKIP_ROWS, old_skip_rows);
		glPixelStorei(GL_PACK_SKIP_PIXELS, old_skip_pixels);
		glPixelStorei(GL_PACK_ALIGNMENT, old_alignment);
    }

    m_CurrentTile++;

    if(m_CurrentTile >= m_Rows * m_Columns)
    {
        m_CurrentTile = -1;
        glViewport(m_SavedViewport[0], m_SavedViewport[1], 
        		   m_SavedViewport[2], m_SavedViewport[3]);

        m_IsDone = true;
        more = false;
        return more;
    }

    m_IsDone = false;
    more = true;
    return more;
}

void
TiledRenderer::SetRasterPos(
    GLfloat x, GLfloat y, GLfloat z)
{
    if(m_CurrentTile < 0)
    {
        glRasterPos3f(x, y, z);
    }
    else
    {
        GLdouble modelview[16], proj[16];
        GLint viewport[4];
        GLdouble winX, winY, winZ;

        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, proj);
        
        viewport[0] = 0;
        viewport[1] = 0;
        viewport[2] = m_CurrentTileWidth;
        viewport[3] = m_CurrentTileHeight;

        if(gluProject(x, y, z, modelview, proj, viewport, &winX, &winY, &winZ))
        {
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            glOrtho(0.0, m_CurrentTileWidth,
                    0.0, m_CurrentTileHeight, 0.0, 1.0);
            glRasterPos3f(0.0, 0.0, -winZ);

            {
                GLubyte bitmap[1] = {0};
                glBitmap(1, 1, 0.0, 0.0, winX, winY, bitmap);
            }

            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
            glPopMatrix();
        }
    }
}

// ============================================================================================== //

VD_IMPLEMENT_OBJECT(TiledRenderer, vd_sym(TiledRenderer), vd_sym(Object));

// ============================================================================================== //

VD_RENDER_NAMESPACE_END();

// ============================================================================================== //


