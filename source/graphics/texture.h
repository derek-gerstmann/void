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

#ifndef VD_GRAPHICS_TEXTURE_INCLUDED
#define VD_GRAPHICS_TEXTURE_INCLUDED

// ============================================================================================== //

#include "graphics/graphics.h"
#include "core/symbol.h"
#include "core/memory.h"

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_USING(Core, Symbol);
VD_USING(Core, Memory);

// ============================================================================================== //

class Texture
{
public:

	VD_DECLARE_ENUM(CubeFace,
		Front,
		Back,
		Left,
		Right,
		Top,
		Bottom);

	struct Properties
	{
		Symbol Name;
		vd::u32 Handle;
		vd::u32 Width;
		vd::u32 Height;
		vd::u32 Depth;
		vd::u32 Target;
		vd::u32 TextureUnit;
		vd::u32 Internal;
		vd::u32 Format;
		vd::u32 DataType;
		vd::u32 MinFilterMode;
		vd::u32 MagFilterMode;
		vd::u32 WrapMode;
		vd::u32 CompareMode;
		vd::u32 CompareFunc;
		vd::u32 EnvMode;
		vd::u32 DepthMode;
		bool UseMipMaps;
		
		Properties() { }
		void Reset() { Memory::SetBytes(this, 0, sizeof(Texture::Properties)); }
	};
	

    static Texture* 
    LoadCubeMap(
    	const Symbol& name,    
	    vd::string filenames[6]);
    
    static Texture* 
    Load2D(
	    const Symbol& name,    
        const vd::string& filename);
    
    static Texture* 
    Create2D(
    	const Properties& properties,
        const void* pvData);   

    static Texture*
    Create3D(
    	const Properties& properties,
        const void* pvData);   

    static vd::v3f32 GetCubeFaceOrientation(
    	CubeFace::Value face)
    {
    	switch(face)
    	{
    	case CubeFace::Front: 	{ return vd::v3f32(   0.0f,   0.0f,  0.0f); } // front }
    	case CubeFace::Back: 	{ return vd::v3f32(   0.0f,+180.0f,  0.0f); } // back
    	case CubeFace::Left: 	{ return vd::v3f32(   0.0f, -90.0f,  0.0f); } // left
    	case CubeFace::Right: 	{ return vd::v3f32(   0.0f, +90.0f,  0.0f); } // right
    	case CubeFace::Top: 	{ return vd::v3f32( +90.0f,   0.0f,  0.0f); } // top
    	case CubeFace::Bottom: 	{ return vd::v3f32( -90.0f,   0.0f,  0.0f); } // bottom
    	case CubeFace::Invalid:
    	default:    			{ return vd::v3f32(   0.0f,   0.0f,  0.0f); }
    	};
		return vd::v3f32(0.0f, 0.0f, 0.0f);
    }

public:

	Texture(
		const Properties& properties) 
	: 
		m_Properties(properties)
	{
		// EMPTY!
	}
	

    Texture();
    virtual ~Texture();

    virtual vd::status Destroy(void);

    bool 
    Create(
    	const Properties& properties,
        const void* pvData);
            
    void Bind(vd::i32 eUnit = 0);
    void Unbind();
    void Update();

    void Render();
    void Capture();
    bool IsValid() const;
    bool IsActive() const                               { return m_IsActive;      	}
    void SetTextureMatrix(const float rkM[16])          { Memory::CopyBytes(m_TextureMatrix, rkM, 16*sizeof(float));   }
    bool SetProperties(const Properties& properties)	{ Memory::CopyBytes(&m_Properties, &properties, sizeof(Properties)); return true; }
	const Properties& GetProperties() const				{ return m_Properties;		}

protected:
    
    static void CheckStatus(const Symbol& name, const char* acMessage=0);

        
protected:

    bool m_IsActive;
    Texture::Properties m_Properties;
    float m_TextureMatrix[16];
};

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_END();

// ============================================================================================== //

#endif 
