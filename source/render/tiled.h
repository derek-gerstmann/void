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

#ifndef VD_RENDER_TILED_INCLUDED
#define VD_RENDER_TILED_INCLUDED

// ============================================================================================== //

#include "render/render.h"
#include "core/memory.h"

// ============================================================================================== //

VD_RENDER_NAMESPACE_BEGIN();

// ============================================================================================== //

class TiledRenderer : public Object
{
public:

    VD_DECLARE_ENUM(RowOrder,
                    Invalid,
                    TopToBottom,
                    BottomToTop);

    TiledRenderer();
    
    TiledRenderer(
        vd::i32 img_width,
        vd::i32 img_height,
        vd::i32 tile_width = 256,
        vd::i32 tile_heignt = 256,
        vd::i32 border = 0,
        RowOrder::Value RowOrder = RowOrder::BottomToTop);
    
    virtual ~TiledRenderer() { };

    void SetTileSize(vd::i32 width, vd::i32 height, vd::i32 border = 0);
    void SetTileBufferData(vd::i32 format, vd::i32 type, void* buffer);
    void SetImageSize(vd::i32 width, vd::i32 height);
    void SetImageBufferData(vd::i32 format, vd::i32 type, void* buffer);
    void SetupTiles();

    void SetOrtho(
        vd::f32 left, vd::f32 right,
        vd::f32 bottom, vd::f32 top,
        vd::f32 zNear, vd::f32 zFar);

    void SetFrustum(
        vd::f32 left, vd::f32 right,
        vd::f32 bottom, vd::f32 top,
        vd::f32 zNear, vd::f32 zFar);

    void SetPerspective(
        vd::f32 fovy, vd::f32 aspect,
        vd::f32 zNear, vd::f32 zFar);

    void BeginTile();
    int EndTile();

    void Bind();
    void Unbind();
    
    void SetRasterPos(
        vd::f32 x, vd::f32 y, vd::f32 z);
    
    vd::i32 GetImageWidth(void) const { return m_Image.Width; }
    vd::i32 GetImageHeight(void) const { return m_Image.Height; }
    vd::i32 GetImageFormat(void) const { return m_Image.Format; }
    vd::i32 GetImageType(void) const { return m_Image.Type; }
    void* GetImageBuffer(void) const { return m_Image.Buffer; }

    vd::i32 GetTileWidth(void) const { return m_Tile.Width; }
    vd::i32 GetTileHeight(void) const { return m_Tile.Height; }
    vd::i32 GetInnerTileWidth(void) const { return m_Tile.InnerWidth; }
    vd::i32 GetInnerTileHeight(void) const { return m_Tile.InnerHeight; }
    vd::i32 GetTileBorder(void) const { return m_Tile.Border; }
    vd::i32 GetTileFormat(void) const { return m_Tile.Format; }
    vd::i32 GetTileType(void) const { return m_Tile.Type; }
    void* GetTileBuffer(void) const { return m_Tile.Buffer; }

	vd::i32 GetCurrentTileIndex(void) const { return m_CurrentTile; }
	vd::i32 GetCurrentColumn(void) const { return m_CurrentColumn; }
	vd::i32 GetCurrentRow(void) const { return m_CurrentRow; }
	vd::i32 GetCurrentTileWidth(void) const { return m_CurrentTileWidth; }
	vd::i32 GetCurrentTileHeight(void) const { return m_CurrentTileHeight; }
	RowOrder::Value GetRowOrder(void) const { return m_RowOrder; }
	void SetRowOrder(RowOrder::Value v) { m_RowOrder = v; }
	
	vd::i32 GetColumnCount(void) const { return m_Columns; }
	vd::i32 GetRowCount(void) const { return m_Rows; }
	
	VD_DECLARE_OBJECT(TiledRenderer);
	
protected:

	VD_DISABLE_COPY_CONSTRUCTORS(TiledRenderer);

    struct ImageRegion
    {
        vd::i32 Width;
        vd::i32 Height;
        vd::i32 Format;
        vd::i32 Type;
        void* 	Buffer;
    };

    struct TileRegion
    {
        vd::i32 Width;
        vd::i32 Height;
        vd::i32 InnerWidth;
        vd::i32 InnerHeight;
        vd::i32 Border;
        vd::i32 Format;
        vd::i32 Type;
        void* Buffer;
    };

    struct Frustum
    {
        vd::f32 Left;
        vd::f32 Right;
        vd::f32 Bottom;
        vd::f32 Top;
        vd::f32 Near;
        vd::f32 Far;
    };

    vd::i32 m_Rows;
    vd::i32 m_Columns;
    vd::i32 m_CurrentTile;
    vd::i32 m_CurrentTileWidth;
    vd::i32 m_CurrentTileHeight;
    vd::i32 m_CurrentRow;
    vd::i32 m_CurrentColumn;
    vd::i32 m_SavedViewport[4];
    RowOrder::Value m_RowOrder;
    bool m_UsePerspective;

    ImageRegion m_Image;
    TileRegion m_Tile;
    Frustum m_Frustum;
};

// ============================================================================================== //

VD_RENDER_NAMESPACE_END();

// ============================================================================================== //

#endif	// VD_RENDER_TILED_INCLUDED

