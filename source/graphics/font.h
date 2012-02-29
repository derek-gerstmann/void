// ============================================================================================== //
//
// License:   The Lesser GNU Public License (LGPL) v3.0.
// 
// Author(s):   Derek Gerstmann. The University of Western Australia (UWA). 
//        As well as the shoulders of many giants...
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

#ifndef VD_GRAPHICS_FONT_SYSTEM_INCLUDED
#define VD_GRAPHICS_FONT_SYSTEM_INCLUDED

// ============================================================================================== //

#include "graphics/graphics.h"

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_BEGIN();

// ============================================================================================== //

namespace font {

// ============================================================================================== //

struct Kerning
{
    vd::wchar   CharCode;
    vd::f32     Kerning;  
};

class Atlas : public vd::core::Object 
{

public:

    Atlas() : vd::core::Object() {}
    ~Atlas(){ Destroy(); }

    vd::status
    Initialize( const size_t width, const size_t height, const size_t depth );

    void
    Destroy();

    void
    UpdateData();

    ivec4
    GetRegion( const size_t width,
               const size_t height );
    void
    SetRegion(const size_t x,
              const size_t y,
              const size_t width,
              const size_t height,
              const unsigned char *data,
              const size_t stride );


    void Clear();  

    VD_DECLARE_OBJECT(Atlas);

private:
  
    VD_DISABLE_COPY_CONSTRUCTORS(Atlas);

    vector_t * nodes;
    size_t width;
    size_t height;
    size_t depth;
    size_t used;
    unsigned int id;
    unsigned char * data;
};

class Font : public Object
{
public:
    Font() : Object() {}
    ~Font(){ Destroy(); }

    void Destroy(void);

    vd::status Initialize( size_t width, size_t height, size_t depth );

    void
    Remove(texture_font_t * font );

    static Font* 
    LoadFontFromFilename( const char * filename, const float size );

    static Font* 
    LoadFontFromDescription( const char * family,
                             const float size,
                             const int bold,
                             const int italic );

    texture_font_t *
    LoadFontFromMarkup( font_manager_t *self,
                                  const markup_t *markup );

    char *
    FindMatchingFontDescription( font_manager_t * self,
                                    const char * family,
                                    const float size,
                                    const int bold,
                                    const int italic );

    const wchar_t *
    GetCache();

    void
    SetCache(const wchar_t * cache );

    VD_DECLARE_OBJECT(System);

private:
  
    VD_DISABLE_COPY_CONSTRUCTORS(System);
  
    vd::u32         m_Id;    
    texture_atlas_t * atlas;
    vector_t * fonts;
    wchar_t * cache;
};


// ============================================================================================== //

} // END NAMESPACE: font

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_END();

// ============================================================================================== //

#endif

