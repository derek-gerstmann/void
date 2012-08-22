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

#include "graphics/gl/context.h"
#include "core/core.h"
#include "core/asserts.h"
#include "core/memory.h"

#if defined(VD_PLATFORM_OSX)        
#include <CGLCurrent.h>
#include <CGLTypes.h>
#endif

// ============================================================================================== //

VD_GRAPHICS_OPENGL_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_IMPORT(Core, Memory);

// ============================================================================================== //

template<typename T>
bool IsOdd(T x)
{
    return ((x) % 2);
}

static const char*
ConvertGLEnumToString(
    GLenum value)
{
    switch(value)
    {
        case GL_BYTE:                       { return "GL_BYTE"; }
        case GL_UNSIGNED_BYTE:              { return "GL_UNSIGNED_BYTE"; }
        case GL_SHORT:                      { return "GL_SHORT"; }
        case GL_INT:                        { return "GL_INT"; }
        case GL_UNSIGNED_INT:               { return "GL_UNSIGNED_INT"; }
        case GL_HALF_FLOAT:                 { return "GL_HALF_FLOAT"; }
        case GL_FLOAT:                      { return "GL_FLOAT"; }
        case GL_DOUBLE:                     { return "GL_DOUBLE"; }

        case GL_POINTS:                     { return "GL_POINTS"; }
        case GL_LINES:                      { return "GL_LINES"; }
        case GL_LINE_STRIP:                 { return "GL_LINE_STRIP"; }
        case GL_LINE_LOOP:                  { return "GL_LINE_LOOP"; }
        case GL_QUADS:                      { return "GL_QUADS"; }
        case GL_QUAD_STRIP:                 { return "GL_QUAD_STRIP"; }
        case GL_TRIANGLES:                  { return "GL_TRIANGLES"; }
        case GL_TRIANGLE_STRIP:             { return "GL_TRIANGLE_STRIP"; }
        case GL_TRIANGLE_FAN:               { return "GL_TRIANGLE_FAN"; }
        case GL_POLYGON:                    { return "GL_POLYGON"; }

        case GL_ARRAY_BUFFER:               { return "GL_ARRAY_BUFFER"; }
        case GL_ELEMENT_ARRAY_BUFFER:       { return "GL_ELEMENT_ARRAY_BUFFER";  }
        case GL_PIXEL_UNPACK_BUFFER:        { return "GL_PIXEL_UNPACK_BUFFER"; }
        case GL_FRAMEBUFFER:                { return "GL_FRAMEBUFFER"; }
        case GL_RENDERBUFFER:               { return "GL_RENDERBUFFER"; }
        case GL_TEXTURE_BUFFER:             { return "GL_TEXTURE_BUFFER"; }

        case GL_VERTEX_ARRAY:               { return "GL_VERTEX_ARRAY"; }
        case GL_NORMAL_ARRAY:               { return "GL_NORMAL_ARRAY"; }
        case GL_COLOR_ARRAY:                { return "GL_COLOR_ARRAY"; }
        case GL_TEXTURE_COORD_ARRAY:        { return "GL_TEXTURE_COORD_ARRAY"; }

        case GL_ALPHA:                      { return "GL_ALPHA"; }
        case GL_ALPHA8I_EXT:                { return "GL_ALPHA8I_EXT"; }
        case GL_ALPHA8UI_EXT:               { return "GL_ALPHA8UI_EXT"; }
        case GL_ALPHA16I_EXT:               { return "GL_ALPHA16I_EXT"; } 
        case GL_ALPHA16UI_EXT:              { return "GL_ALPHA16UI_EXT"; }
        case GL_ALPHA16F_ARB:               { return "GL_ALPHA16F_ARB"; }
        case GL_ALPHA32I_EXT:               { return "GL_ALPHA32I_EXT"; } 
        case GL_ALPHA32UI_EXT:              { return "GL_ALPHA32UI_EXT"; }
        case GL_ALPHA32F_ARB:               { return "GL_ALPHA32F_ARB"; }

        case GL_LUMINANCE:                  { return "GL_LUMINANCE"; } 
        case GL_LUMINANCE8I_EXT:            { return "GL_LUMINANCE8I_EXT"; } 
        case GL_LUMINANCE8UI_EXT:           { return "GL_LUMINANCE8UI_EXT"; } 
        case GL_LUMINANCE16I_EXT:           { return "GL_LUMINANCE16I_EXT"; }
        case GL_LUMINANCE16UI_EXT:          { return "GL_LUMINANCE16UI_EXT"; }
        case GL_LUMINANCE16F_ARB:           { return "GL_LUMINANCE16F_ARB"; }
        case GL_LUMINANCE32I_EXT:           { return "GL_LUMINANCE32I_EXT"; }
        case GL_LUMINANCE32UI_EXT:          { return "GL_LUMINANCE32UI_EXT"; }
        case GL_LUMINANCE32F_ARB:           { return "GL_LUMINANCE32F_ARB"; }

        case GL_LUMINANCE_ALPHA:            { return "GL_LUMINANCE_ALPHA"; }
        case GL_LUMINANCE_ALPHA8I_EXT:      { return "GL_LUMINANCE_ALPHA8I_EXT"; }
        case GL_LUMINANCE_ALPHA8UI_EXT:     { return "GL_LUMINANCE_ALPHA8UI_EXT"; }
        case GL_LUMINANCE_ALPHA16I_EXT:     { return "GL_LUMINANCE_ALPHA16I_EXT"; }
        case GL_LUMINANCE_ALPHA16UI_EXT:    { return "GL_LUMINANCE_ALPHA16UI_EXT"; }
        case GL_LUMINANCE_ALPHA16F_ARB:     { return "GL_LUMINANCE_ALPHA16F_ARB"; }
        case GL_LUMINANCE_ALPHA32I_EXT:     { return "GL_LUMINANCE_ALPHA32I_EXT"; }
        case GL_LUMINANCE_ALPHA32UI_EXT:    { return "GL_LUMINANCE_ALPHA32UI_EXT"; }
        case GL_LUMINANCE_ALPHA32F_ARB:     { return "GL_LUMINANCE_ALPHA32F_ARB"; }

        case GL_INTENSITY:                  { return "GL_INTENSITY"; }  
        case GL_INTENSITY8I_EXT:            { return "GL_INTENSITY8I_EXT"; }  
        case GL_INTENSITY8UI_EXT:           { return "GL_INTENSITY8UI_EXT"; }  
        case GL_INTENSITY16I_EXT:           { return "GL_INTENSITY16I_EXT"; }
        case GL_INTENSITY16UI_EXT:          { return "GL_INTENSITY16UI_EXT"; } 
        case GL_INTENSITY16F_ARB:           { return "GL_INTENSITY16F_ARB"; }
        case GL_INTENSITY32I_EXT:           { return "GL_INTENSITY32I_EXT"; }  
        case GL_INTENSITY32UI_EXT:          { return "GL_INTENSITY32UI_EXT"; }
        case GL_INTENSITY32F_ARB:           { return "GL_INTENSITY32F_ARB"; }

        case GL_R:                          { return "GL_R"; } 
        case GL_R8I:                        { return "GL_R8I"; } 
        case GL_R8UI:                       { return "GL_R8UI"; } 
        case GL_R16I:                       { return "GL_R16I"; }
        case GL_R16UI:                      { return "GL_R16UI"; }
        case GL_R16F:                       { return "GL_R16F"; }
        case GL_R32I:                       { return "GL_R32I"; }
        case GL_R32UI:                      { return "GL_R32UI"; }
        case GL_R32F:                       { return "GL_R32F"; }

        case GL_RG:                         { return "GL_RG"; } 
        case GL_RG8I:                       { return "GL_RG8I"; } 
        case GL_RG8UI:                      { return "GL_RG8UI"; } 
        case GL_RG16I:                      { return "GL_RG16I"; }
        case GL_RG16UI:                     { return "GL_RG16UI"; }
        case GL_RG16F:                      { return "GL_RG16F"; }
        case GL_RG32I:                      { return "GL_RG32I"; }
        case GL_RG32UI:                     { return "GL_RG32UI"; }
        case GL_RG32F:                      { return "GL_RG32F"; }

        case GL_RGB:                        { return "GL_RGB"; } 
        case GL_RGB8I:                      { return "GL_RGB8I"; } 
        case GL_RGB8UI:                     { return "GL_RGB8UI"; }
        case GL_RGB16I:                     { return "GL_RGB16I"; } 
        case GL_RGB16UI:                    { return "GL_RGB16UI"; } 
        case GL_RGB16F:                     { return "GL_RGB16F"; }
        case GL_RGB32I:                     { return "GL_RGB32I"; } 
        case GL_RGB32UI:                    { return "GL_RGB32UI"; }
        case GL_RGB32F:                     { return "GL_RGB32F"; }

        case GL_RGBA:                       { return "GL_RGBA"; } 
        case GL_RGBA8I:                     { return "GL_RGBA8I"; } 
        case GL_RGBA8UI:                    { return "GL_RGBA8UI"; }
        case GL_RGBA16I:                    { return "GL_RGBA16I"; } 
        case GL_RGBA16UI:                   { return "GL_RGBA16UI"; } 
        case GL_RGBA16F_ARB:                { return "GL_RGBA16F"; }
        case GL_RGBA32I:                    { return "GL_RGBA32I"; } 
        case GL_RGBA32UI:                   { return "GL_RGBA32UI"; }
        case GL_RGBA32F_ARB:                { return "GL_RGBA32F"; }

        case GL_DEPTH_COMPONENT16:          { return "GL_DEPTH_COMPONENT16"; }
        case GL_DEPTH_COMPONENT32:          { return "GL_DEPTH_COMPONENT32"; }
        case GL_DEPTH_COMPONENT32F:         { return "GL_DEPTH_COMPONENT32F"; }

        case GL_INVALID_ENUM:               { return "GL_INVALID_ENUM"; }
        default:
        {
            vdLogGlobalError("Invalid GL enum value '%d : 0x%X' specified for conversion!", 
                value, value);

            return "GL_INVALID_ENUM"; 
        }
    };
    return "GL_INVALID_ENUM";

}

static GLuint
ConvertObjectIdToGL(
    vd::u32 value)
{
    return (GLuint)(value);
}

static GLenum 
ConvertBufferTargetToGL(
	Buffer::TargetType::Value value)
{
	switch(value)
	{
		case Buffer::TargetType::ArrayBuffer: 		{ return GL_ARRAY_BUFFER; }
		case Buffer::TargetType::IndexBuffer: 		{ return GL_ELEMENT_ARRAY_BUFFER;  }
		case Buffer::TargetType::PixelBuffer: 		{ return GL_PIXEL_UNPACK_BUFFER; }
		case Buffer::TargetType::FrameBuffer: 		{ return GL_FRAMEBUFFER; }
		case Buffer::TargetType::RenderBuffer: 		{ return GL_RENDERBUFFER; }
		case Buffer::TargetType::TextureBuffer: 	{ return GL_TEXTURE_BUFFER; }
		case Buffer::TargetType::Invalid:	
		default:
		{
			vdLogGlobalError("Invalid buffer target '%s' specified for conversion!", 
				Buffer::TargetType::ToString(value));

			return GL_INVALID_ENUM;	
		}
	};
	return GL_INVALID_ENUM;
}

static GLenum 
ConvertGeometryPrimitiveTypeToGL(
    Geometry::PrimitiveType::Value value)
{
    switch(value)
    {
        case Geometry::PrimitiveType::Points:       { return GL_POINTS; }
        case Geometry::PrimitiveType::Lines:        { return GL_LINES; }
        case Geometry::PrimitiveType::LineStrip:    { return GL_LINE_STRIP; }
        case Geometry::PrimitiveType::LineLoop:     { return GL_LINE_LOOP; }
        case Geometry::PrimitiveType::Quads:        { return GL_QUADS; }
        case Geometry::PrimitiveType::QuadStrip:    { return GL_QUAD_STRIP; }
        case Geometry::PrimitiveType::Triangles:    { return GL_TRIANGLES; }
        case Geometry::PrimitiveType::TriangleStrip:{ return GL_TRIANGLE_STRIP; }
        case Geometry::PrimitiveType::TriangleFan:  { return GL_TRIANGLE_FAN; }
        case Geometry::PrimitiveType::Polygon:      { return GL_POLYGON; }
        case Geometry::PrimitiveType::Invalid:
        default:
        {
            vdLogGlobalError("Invalid geometry primitive type '%s' specified for conversion!", 
                Geometry::PrimitiveType::ToString(value));

            return GL_INVALID_ENUM; 
        }
    };
    return GL_INVALID_ENUM;
}
		
static GLenum 
ConvertBufferAttributeToGL(
	Buffer::AttributeType::Value value)
{
	switch(value)
	{
		case Buffer::AttributeType::Index:			{ return GL_ELEMENT_ARRAY_BUFFER; }
		case Buffer::AttributeType::Position:		{ return GL_VERTEX_ARRAY; }
		case Buffer::AttributeType::Normal:			{ return GL_NORMAL_ARRAY; }
		case Buffer::AttributeType::Color:			{ return GL_COLOR_ARRAY; }
		case Buffer::AttributeType::TexCoord:		{ return GL_TEXTURE_COORD_ARRAY; }
		case Buffer::AttributeType::UserAttrib:		{ return GL_COLOR_ARRAY; }
		case Buffer::AttributeType::Invalid:
		default:
		{
			vdLogGlobalError("Invalid buffer attribute '%s' specified for conversion!", 
				Buffer::AttributeType::ToString(value));

			return GL_INVALID_ENUM;	
		}
	};
	return GL_INVALID_ENUM;
}


static GLenum
ConvertInternalAlphaFormatToGL(
    Graphics::ChannelLayout::Value channel_layout,
    Graphics::ScalarTypeId::Value channel_datatype)
{
    vdGlobalAssert(channel_layout == Graphics::ChannelLayout::Alpha);

    switch(channel_datatype)
    {
        case Graphics::ScalarTypeId::I8:     return GL_ALPHA8I_EXT;
        case Graphics::ScalarTypeId::U8:     return GL_ALPHA8UI_EXT;
        case Graphics::ScalarTypeId::I16:    return GL_ALPHA16I_EXT; 
        case Graphics::ScalarTypeId::U16:    return GL_ALPHA16UI_EXT;
        case Graphics::ScalarTypeId::F16:    return GL_ALPHA16F_ARB;
        case Graphics::ScalarTypeId::I32:    return GL_ALPHA32I_EXT; 
        case Graphics::ScalarTypeId::U32:    return GL_ALPHA32UI_EXT;
        case Graphics::ScalarTypeId::F32:    return GL_ALPHA32F_ARB;
        default:
        {
            vdLogGlobalError("Invalid scalar datatype '%s' specified for conversion!", 
                Graphics::ScalarTypeId::ToString(channel_datatype));

            return GL_INVALID_ENUM; 
        }
    };
    return GL_INVALID_ENUM;
}

static GLenum
ConvertInternalLuminanceFormatToGL(
    Graphics::ChannelLayout::Value channel_layout,
    Graphics::ScalarTypeId::Value channel_datatype)
{
    vdGlobalAssert(channel_layout == Graphics::ChannelLayout::Luminance);
    
    switch(channel_datatype)
    {
        case Graphics::ScalarTypeId::I8:     return GL_LUMINANCE8I_EXT; 
        case Graphics::ScalarTypeId::U8:     return GL_LUMINANCE8UI_EXT; 
        case Graphics::ScalarTypeId::I16:    return GL_LUMINANCE16I_EXT;
        case Graphics::ScalarTypeId::U16:    return GL_LUMINANCE16UI_EXT;
        case Graphics::ScalarTypeId::F16:    return GL_LUMINANCE16F_ARB;
        case Graphics::ScalarTypeId::I32:    return GL_LUMINANCE32I_EXT;
        case Graphics::ScalarTypeId::U32:    return GL_LUMINANCE32UI_EXT;
        case Graphics::ScalarTypeId::F32:    return GL_LUMINANCE32F_ARB;
        default:
        {
            vdLogGlobalError("Invalid scalar datatype '%s' specified for conversion!", 
                Graphics::ScalarTypeId::ToString(channel_datatype));

            return GL_INVALID_ENUM; 
        }
    };
    return GL_INVALID_ENUM;
}

static GLenum
ConvertInternalLuminanceAlphaFormatToGL(
    Graphics::ChannelLayout::Value channel_layout,
    Graphics::ScalarTypeId::Value channel_datatype)
{
    vdGlobalAssert(channel_layout == Graphics::ChannelLayout::LuminanceAlpha);
    
    switch(channel_datatype)
    {
        case Graphics::ScalarTypeId::I8:     return GL_LUMINANCE_ALPHA8I_EXT;
        case Graphics::ScalarTypeId::U8:     return GL_LUMINANCE_ALPHA8UI_EXT;
        case Graphics::ScalarTypeId::I16:    return GL_LUMINANCE_ALPHA16I_EXT;
        case Graphics::ScalarTypeId::U16:    return GL_LUMINANCE_ALPHA16UI_EXT;
        case Graphics::ScalarTypeId::F16:    return GL_LUMINANCE_ALPHA16F_ARB;
        case Graphics::ScalarTypeId::I32:    return GL_LUMINANCE_ALPHA32I_EXT;
        case Graphics::ScalarTypeId::U32:    return GL_LUMINANCE_ALPHA32UI_EXT;
        case Graphics::ScalarTypeId::F32:    return GL_LUMINANCE_ALPHA32F_ARB;
        default:
        {
            vdLogGlobalError("Invalid scalar datatype '%s' specified for conversion!", 
                Graphics::ScalarTypeId::ToString(channel_datatype));

            return GL_INVALID_ENUM; 
        }
    };
    return GL_INVALID_ENUM;
}

static GLenum
ConvertInternalIntensityFormatToGL(
    Graphics::ChannelLayout::Value channel_layout,
    Graphics::ScalarTypeId::Value channel_datatype)
{
    vdGlobalAssert(channel_layout == Graphics::ChannelLayout::Intensity);
    
    switch(channel_datatype)
    {
        case Graphics::ScalarTypeId::I8:     return GL_INTENSITY8I_EXT;  
        case Graphics::ScalarTypeId::U8:     return GL_INTENSITY8UI_EXT;  
        case Graphics::ScalarTypeId::I16:    return GL_INTENSITY16I_EXT;
        case Graphics::ScalarTypeId::U16:    return GL_INTENSITY16UI_EXT; 
        case Graphics::ScalarTypeId::F16:    return GL_INTENSITY16F_ARB;
        case Graphics::ScalarTypeId::I32:    return GL_INTENSITY32I_EXT;  
        case Graphics::ScalarTypeId::U32:    return GL_INTENSITY32UI_EXT;
        case Graphics::ScalarTypeId::F32:    return GL_INTENSITY32F_ARB;
        default:
        {
            vdLogGlobalError("Invalid scalar datatype '%s' specified for conversion!", 
                Graphics::ScalarTypeId::ToString(channel_datatype));

            return GL_INVALID_ENUM; 
        }
    };
    return GL_INVALID_ENUM;
}

static GLenum
ConvertInternalRFormatToGL(
    Graphics::ChannelLayout::Value channel_layout,
    Graphics::ScalarTypeId::Value channel_datatype)
{
    vdGlobalAssert(channel_layout == Graphics::ChannelLayout::R);
    
    switch(channel_datatype)
    {
        case Graphics::ScalarTypeId::I8:     return GL_R8I; 
        case Graphics::ScalarTypeId::U8:     return GL_R8UI; 
        case Graphics::ScalarTypeId::I16:    return GL_R16I;
        case Graphics::ScalarTypeId::U16:    return GL_R16UI;
        case Graphics::ScalarTypeId::F16:    return GL_R16F;
        case Graphics::ScalarTypeId::I32:    return GL_R32I;
        case Graphics::ScalarTypeId::U32:    return GL_R32UI;
        case Graphics::ScalarTypeId::F32:    return GL_R32F;
        default:
        {
            vdLogGlobalError("Invalid scalar datatype '%s' specified for conversion!", 
                Graphics::ScalarTypeId::ToString(channel_datatype));

            return GL_INVALID_ENUM; 
        }
    };
    return GL_INVALID_ENUM;
}

static GLenum
ConvertInternalRGFormatToGL(
    Graphics::ChannelLayout::Value channel_layout,
    Graphics::ScalarTypeId::Value channel_datatype)
{
    vdGlobalAssert(channel_layout == Graphics::ChannelLayout::RG);
    
    switch(channel_datatype)
    {
        case Graphics::ScalarTypeId::I8:     return GL_RG8I; 
        case Graphics::ScalarTypeId::U8:     return GL_RG8UI; 
        case Graphics::ScalarTypeId::I16:    return GL_RG16I;
        case Graphics::ScalarTypeId::U16:    return GL_RG16UI;
        case Graphics::ScalarTypeId::F16:    return GL_RG16F;
        case Graphics::ScalarTypeId::I32:    return GL_RG32I;
        case Graphics::ScalarTypeId::U32:    return GL_RG32UI;
        case Graphics::ScalarTypeId::F32:    return GL_RG32F;
        default:
        {
            vdLogGlobalError("Invalid scalar datatype '%s' specified for conversion!", 
                Graphics::ScalarTypeId::ToString(channel_datatype));

            return GL_INVALID_ENUM; 
        }
    };
    return GL_INVALID_ENUM;
}

static GLenum
ConvertInternalRGBFormatToGL(
    Graphics::ChannelLayout::Value channel_layout,
    Graphics::ScalarTypeId::Value channel_datatype)
{
    vdGlobalAssert(channel_layout == Graphics::ChannelLayout::RGB);
    
    switch(channel_datatype)
    {
        case Graphics::ScalarTypeId::I8:     return GL_RGB8I; 
        case Graphics::ScalarTypeId::U8:     return GL_RGB8UI;
        case Graphics::ScalarTypeId::I16:    return GL_RGB16I; 
        case Graphics::ScalarTypeId::U16:    return GL_RGB16UI; 
        case Graphics::ScalarTypeId::F16:    return GL_RGB16F;
        case Graphics::ScalarTypeId::I32:    return GL_RGB32I; 
        case Graphics::ScalarTypeId::U32:    return GL_RGB32UI;
        case Graphics::ScalarTypeId::F32:    return GL_RGB32F;
        default:
        {
            vdLogGlobalError("Invalid scalar datatype '%s' specified for conversion!", 
                Graphics::ScalarTypeId::ToString(channel_datatype));

            return GL_INVALID_ENUM; 
        }
    };
    return GL_INVALID_ENUM;
}

static GLenum
ConvertInternalRGBAFormatToGL(
    Graphics::ChannelLayout::Value channel_layout,
    Graphics::ScalarTypeId::Value channel_datatype)
{
    vdGlobalAssert(channel_layout == Graphics::ChannelLayout::RGBA);
    
    switch(channel_datatype)
    {
        case Graphics::ScalarTypeId::I8:     return GL_RGBA8I; 
        case Graphics::ScalarTypeId::U8:     return GL_RGBA8UI;
        case Graphics::ScalarTypeId::I16:    return GL_RGBA16I; 
        case Graphics::ScalarTypeId::U16:    return GL_RGBA16UI; 
        case Graphics::ScalarTypeId::F16:    return GL_RGBA16F_ARB;
        case Graphics::ScalarTypeId::I32:    return GL_RGBA32I; 
        case Graphics::ScalarTypeId::U32:    return GL_RGBA32UI;
        case Graphics::ScalarTypeId::F32:    return GL_RGBA32F_ARB;
        default:
        {
            vdLogGlobalError("Invalid scalar datatype '%s' specified for conversion!", 
                Graphics::ScalarTypeId::ToString(channel_datatype));

            return GL_INVALID_ENUM; 
        }
    };
    return GL_INVALID_ENUM;
}


static GLenum
ConvertInternalDepthFormatToGL(
    Graphics::ChannelLayout::Value channel_layout,
    Graphics::ScalarTypeId::Value channel_datatype)
{
    vdGlobalAssert(channel_layout == Graphics::ChannelLayout::Depth);
    
    switch(channel_datatype)
    {
        case Graphics::ScalarTypeId::I16:   return GL_DEPTH_COMPONENT16;
        case Graphics::ScalarTypeId::U16:   return GL_DEPTH_COMPONENT16;
        case Graphics::ScalarTypeId::I32:   return GL_DEPTH_COMPONENT32;
        case Graphics::ScalarTypeId::U32:   return GL_DEPTH_COMPONENT32;
        case Graphics::ScalarTypeId::F32:   return GL_DEPTH_COMPONENT32F;
        default:
        {
            vdLogGlobalError("Invalid scalar datatype '%s' specified for conversion!", 
                Graphics::ScalarTypeId::ToString(channel_datatype));

            return GL_INVALID_ENUM; 
        }
    };
    return GL_INVALID_ENUM;
}

static GLenum
ConvertInternalChannelLayoutToGL(
    Graphics::ChannelLayout::Value channel_layout,
    Graphics::ScalarTypeId::Value channel_datatype)
{
    switch(channel_layout)
    {
        case Graphics::ChannelLayout::Alpha:                { return ConvertInternalAlphaFormatToGL(channel_layout, channel_datatype); }
        case Graphics::ChannelLayout::Intensity:            { return ConvertInternalIntensityFormatToGL(channel_layout, channel_datatype); }
        case Graphics::ChannelLayout::Luminance:            { return ConvertInternalLuminanceFormatToGL(channel_layout, channel_datatype); }
        case Graphics::ChannelLayout::LuminanceAlpha:       { return ConvertInternalLuminanceAlphaFormatToGL(channel_layout, channel_datatype); }
        case Graphics::ChannelLayout::Depth:                { return ConvertInternalDepthFormatToGL(channel_layout, channel_datatype); }
        case Graphics::ChannelLayout::R:                    { return ConvertInternalRFormatToGL(channel_layout, channel_datatype); }
        case Graphics::ChannelLayout::RG:                   { return ConvertInternalRGFormatToGL(channel_layout, channel_datatype); }
        case Graphics::ChannelLayout::RGB:                  { return ConvertInternalRGBFormatToGL(channel_layout, channel_datatype); }
        case Graphics::ChannelLayout::RGBA:                 { return ConvertInternalRGBAFormatToGL(channel_layout, channel_datatype); }
        default:
        {
            vdLogGlobalError("Invalid scalar datatype '%s' specified for conversion!", 
                Graphics::ChannelLayout::ToString(channel_layout));

            return GL_INVALID_ENUM; 
        }
    };
    return GL_INVALID_ENUM;
}

static GLenum
ConvertDepthFormatToGL(
    Graphics::DepthFormat::Value channel_layout,
    Graphics::ScalarTypeId::Value channel_datatype)
{
    vdGlobalAssert(Graphics::DepthFormat::IsValid(channel_layout) == true);
    
    switch(channel_datatype)
    {
        case Graphics::ScalarTypeId::I16:   return GL_DEPTH_COMPONENT16;
        case Graphics::ScalarTypeId::U16:   return GL_DEPTH_COMPONENT16;
        case Graphics::ScalarTypeId::I32:   return GL_DEPTH_COMPONENT32;
        case Graphics::ScalarTypeId::U32:   return GL_DEPTH_COMPONENT32;
        case Graphics::ScalarTypeId::F32:   return GL_DEPTH_COMPONENT32F;
        default:
        {
            vdLogGlobalError("Invalid scalar datatype '%s' specified for conversion!", 
                Graphics::ScalarTypeId::ToString(channel_datatype));

            return GL_INVALID_ENUM; 
        }
    };
    return GL_INVALID_ENUM;
}

static GLenum
ConvertBaseChannelLayoutToGL(
    Graphics::ChannelLayout::Value value)
{
    switch(value)
    {
        case Graphics::ChannelLayout::R:                    { return GL_R; }
        case Graphics::ChannelLayout::RG:                   { return GL_RG; }
        case Graphics::ChannelLayout::RGB:                  { return GL_RGB; }
        case Graphics::ChannelLayout::RGBA:                 { return GL_RGBA; }
        case Graphics::ChannelLayout::Alpha:                { return GL_ALPHA; }
        case Graphics::ChannelLayout::Depth:                { return GL_DEPTH_COMPONENT; }
        case Graphics::ChannelLayout::Intensity:            { return GL_INTENSITY; }
        case Graphics::ChannelLayout::Luminance:            { return GL_LUMINANCE; }
        case Graphics::ChannelLayout::LuminanceAlpha:       { return GL_LUMINANCE_ALPHA; }
        default:
        {
            vdLogGlobalError("Invalid scalar datatype '%s' specified for conversion!", 
                Graphics::ChannelLayout::ToString(value));

            return GL_INVALID_ENUM; 
        }
    };
    return GL_INVALID_ENUM;
}

static GLenum
ConvertScalarTypeIdToGL(
    Graphics::ScalarTypeId::Value datatype)
{
    switch(datatype)
    {
        case Graphics::ScalarTypeId::U8:                    { return GL_UNSIGNED_BYTE; }
        case Graphics::ScalarTypeId::U16:                   { return GL_UNSIGNED_SHORT; }
        case Graphics::ScalarTypeId::U32:                   { return GL_UNSIGNED_INT; }
        case Graphics::ScalarTypeId::I8:                    { return GL_BYTE; }
        case Graphics::ScalarTypeId::I16:                   { return GL_SHORT; }
        case Graphics::ScalarTypeId::I32:                   { return GL_INT; }
        case Graphics::ScalarTypeId::F16:                   { return GL_HALF_FLOAT; }
        case Graphics::ScalarTypeId::F32:                   { return GL_FLOAT; }
        case Graphics::ScalarTypeId::F64:                   { return GL_DOUBLE; }
        case Graphics::ScalarTypeId::Invalid:
        default:
        {
            vdLogGlobalError("Invalid scalar datatype '%s' specified for conversion!", 
                Graphics::ScalarTypeId::ToString(datatype));

            return GL_INVALID_ENUM; 
        }
    };
    return GL_INVALID_ENUM;
}

static GLenum
ConvertBufferTypeIdToGL(
	Buffer::TypeId::Value value)
{
	switch(value)
	{
		case Buffer::TypeId::U8:					{ return GL_UNSIGNED_BYTE; }
		case Buffer::TypeId::U16:					{ return GL_UNSIGNED_SHORT; }
		case Buffer::TypeId::U32:					{ return GL_UNSIGNED_INT; }
		case Buffer::TypeId::I8:					{ return GL_BYTE; }
		case Buffer::TypeId::I16:					{ return GL_SHORT; }
		case Buffer::TypeId::I32:					{ return GL_INT; }
		case Buffer::TypeId::F16:					{ return GL_HALF_FLOAT; }
		case Buffer::TypeId::F32:					{ return GL_FLOAT; }
		case Buffer::TypeId::F64:					{ return GL_DOUBLE; }
		case Buffer::TypeId::Invalid:
		default:
		{
			vdLogGlobalError("Invalid buffer datatype '%s' specified for conversion!", 
				Buffer::TypeId::ToString(value));

			return GL_INVALID_ENUM;	
		}
	};
	return GL_INVALID_ENUM;
}

static GLenum
ConvertBufferAccessUpdateModeToGL(
	Buffer::AccessMode::Value access,
	Buffer::UpdateMode::Value update)
{
/*
	STREAM
	The data store contents will be modified once and used at most a few times.
	
	STATIC
	The data store contents will be modified once and used many times.
	
	DYNAMIC
	The data store contents will be modified repeatedly and used many times.

	DRAW
	The data store contents are modified by the application, and used as the source for GL drawing and image specification commands.
	
	READ
	The data store contents are modified by reading data from the GL, and used to return that data when queried by the application.
	
	COPY
	The data store contents are modified by reading data from the GL, and used as the source for GL drawing and image specification commands.
*/
	switch(update)
	{
		case Buffer::UpdateMode::Static:
		{
			switch(access)
			{
				case Buffer::AccessMode::ReadOnly:	{ return GL_STATIC_DRAW; }
				case Buffer::AccessMode::WriteOnly: { return GL_STATIC_READ; }
				case Buffer::AccessMode::ReadWrite: { return GL_STATIC_COPY; }
				case Buffer::AccessMode::Invalid:
				default:
				{
					vdLogGlobalError("Invalid buffer update '%s' and access '%s' modes specified for conversion!", 
						Buffer::UpdateMode::ToString(update), Buffer::AccessMode::ToString(access));
		
					return GL_INVALID_ENUM;	
				}
			};
		}
		case Buffer::UpdateMode::Stream:
		{
			switch(access)
			{
				case Buffer::AccessMode::ReadOnly:	{ return GL_STREAM_DRAW; }
				case Buffer::AccessMode::WriteOnly: { return GL_STREAM_READ; }
				case Buffer::AccessMode::ReadWrite: { return GL_STREAM_COPY; }
				case Buffer::AccessMode::Invalid:
				default:
				{
					vdLogGlobalError("Invalid buffer update '%s' and access '%s' modes specified for conversion!", 
						Buffer::UpdateMode::ToString(update), Buffer::AccessMode::ToString(access));
		
					return GL_INVALID_ENUM;	
				}
			};
		}
		case Buffer::UpdateMode::Dynamic:
		{
			switch(access)
			{
				case Buffer::AccessMode::ReadOnly:	{ return GL_DYNAMIC_DRAW; }
				case Buffer::AccessMode::WriteOnly: { return GL_DYNAMIC_READ; }
				case Buffer::AccessMode::ReadWrite: { return GL_DYNAMIC_COPY; }
				case Buffer::AccessMode::Invalid:
				default:
				{
					vdLogGlobalError("Invalid buffer update '%s' and access '%s' modes specified for conversion!", 
						Buffer::UpdateMode::ToString(update), Buffer::AccessMode::ToString(access));
		
					return GL_INVALID_ENUM;	
				}
			};
		}
		case Buffer::UpdateMode::Invalid:
        default:
        {
            vdLogGlobalError("Invalid buffer update '%s' and access '%s' modes specified for conversion!", 
                Buffer::UpdateMode::ToString(update), Buffer::AccessMode::ToString(access));

            return GL_INVALID_ENUM; 
        }
	};
	return GL_INVALID_ENUM;
}

static GLenum
ConvertBufferAccessModeToGL(
	Buffer::AccessMode::Value access)
{
	switch(access)
	{
		case Buffer::AccessMode::ReadOnly:	{ return GL_READ_ONLY; }
		case Buffer::AccessMode::WriteOnly:	{ return GL_WRITE_ONLY; }
		case Buffer::AccessMode::ReadWrite:	{ return GL_READ_WRITE; }
		case Buffer::AccessMode::Invalid:
		default:
		{
			vdLogGlobalError("Invalid buffer access '%s' mode specified for conversion!", 
				Buffer::AccessMode::ToString(access));

			return GL_INVALID_ENUM;	
		}
	};
	return GL_INVALID_ENUM;
}

static size_t
GetSizeOfTypeIdFromGL(
    GLenum eTypeId)
{
    switch(eTypeId)
    {
        case (GL_BYTE):                 
            return sizeof(GLbyte);
        case (GL_UNSIGNED_BYTE):                 
            return sizeof(GLubyte);
        case (GL_SHORT):                      
            return sizeof(GLshort);
        case (GL_UNSIGNED_SHORT):                
            return sizeof(GLushort);
        case (GL_INT):                 
            return sizeof(GLint);
        case (GL_UNSIGNED_INT):                   
            return sizeof(GLuint);
        case (GL_FLOAT):                          
            return sizeof(GLfloat);
        case (GL_DOUBLE):
            return sizeof(GLdouble);
        default:
        {
            vdLogGlobalError("Invalid data type request for conversion!");
            return GL_INVALID_ENUM; 
        }
    };
    
    return 0;
}


// ============================================================================================== //

Context::Context(
    Runtime::Context* runtime) :
    Graphics::Context(runtime),
	m_Native(NULL)
{
	// EMPTY!
}

Context::~Context()
{
    Destroy();
}

vd::status
Context::Setup()
{
    glHint(GL_CLIP_VOLUME_CLIPPING_HINT_EXT, GL_FASTEST);
    return Status::Code::Success;
}

vd::status
Context::Destroy()
{
    for(size_t i = 0; i < m_Buffers.size(); i++)
    {
        Buffer* b = m_Buffers[i];
        if(b != NULL) Destroy(b);
        m_Buffers[i] = NULL;
    }

    for(size_t i = 0; i < m_Geometry.size(); i++)
    {
        Geometry* geo = m_Geometry[i];
        if(geo != NULL) Destroy(geo);
        m_Geometry[i] = NULL;
    }

    for(size_t i = 0; i < m_Framebuffers.size(); i++)
    {
        Framebuffer* fb = m_Framebuffers[i];
        if(fb != NULL) Destroy(fb);
        m_Framebuffers[i] = NULL;
    }

    for(size_t i = 0; i < m_Shaders.size(); i++)
    {
        Shader* s = m_Shaders[i];
        if(s != NULL) Destroy(s);
        m_Shaders[i] = NULL;
    }

    m_Buffers.clear();
    m_Geometry.clear();
    m_Framebuffers.clear();
    m_Textures.clear();
    m_Shaders.clear();
    return Status::Code::Success;
}

// ================================================================================================ //

vd::status 
Context::Clear(
    Graphics::Viewport viewport, 
    vd::f32 r, vd::f32 g, vd::f32 b, vd::f32 a,
    bool clear_color, bool clear_depth)
{
    glViewport(viewport.X, viewport.Y, viewport.Width, viewport.Height);
    glClearColor(r, g, b, a);
    
    GLuint clear_flags = GL_COLOR_BUFFER_BIT;
    if(clear_depth) clear_flags |= GL_DEPTH_BUFFER_BIT;

    glClear(clear_flags);

    return Status::Code::Success;
}

vd::status 
Context::Flush()
{
    glFlush();   
    return Status::Code::Success;
}

vd::status 
Context::Finish()
{
    glFinish();
    return Status::Code::Success;
}

// ============================================================================================== //

void
Context::ReportErrors(
    const char* msg,
    const char *func, 
    const char *filename, 
    int linenum)
{
#ifndef VD_RELEASE_BUILD
    GLenum err;
    bool die = false;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        die = true;
        const char* str = (const char*)gluErrorString(err);
        vdLogWarning("%s %s at line %d : ", filename, func, linenum);
        vdLogWarning("%s '%s' (%d - 0x%X)", 
            msg ? msg : "   ", 
            str != NULL ? str : "<NULL>",
            err, err);
    }

    vdAssertMsg(die == false, "OpenGL Errors Encountered!");
#endif
}

void
Context::RenderToScreen(
    vd::u32 uiTextureId, const Graphics::Viewport& vp, 
    vd::f32 *afData, bool bClear)
{
    static Geometry* quad = NULL;
    if(quad == NULL)
    {
        quad = CreateQuad(
                    -1.0f, +1.0f, +1.0f, -1.0f,
                    +0.0f, +1.0f, +0.0f, +1.0f);
    }
        
    int iViewport[4];
    glDisable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glGetIntegerv(GL_VIEWPORT, iViewport);
    glViewport( vp.X, vp.Y, vp.Width, vp.Height );

    if(bClear)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D( -1.0, 1.0, -1.0, 1.0 );
    
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode( GL_TEXTURE );
    glPushMatrix();
    glLoadIdentity();

    glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );
    glDisable( GL_ALPHA_TEST );
    glDisable( GL_BLEND );
    
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, uiTextureId );

    if(afData)
    {
        vdLogDebug("Uploading texture data [%x] %p\n", uiTextureId, afData);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, vp.Width, vp.Height, GL_RGBA, GL_FLOAT, afData);
    }
    
    glColor3f( 1.0f, 1.0f, 1.0f );

    Bind(quad);
    Submit(quad);
    Unbind(quad);

    glActiveTexture(GL_TEXTURE0);
    glDisable( GL_TEXTURE_2D );
    glMatrixMode( GL_TEXTURE );
    glPopMatrix();

    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();    

    glViewport(iViewport[0], iViewport[1], iViewport[2], iViewport[3]);
    glMatrixMode( GL_MODELVIEW );
}

// ==================================================================================================== //

Buffer*
Context::CreateBuffer(
	Buffer::TargetType::Value target, 
	Buffer::AttributeType::Value attrib,
	Buffer::AccessMode::Value access,
	Buffer::UpdateMode::Value update,
	Buffer::TypeId::Value datatype,	
	vd::u8 components, vd::u32 count, 
    const void* ptr)
{
    vdLogOpenGLErrors("Start");

    GLuint gl_id = 0;
    glGenBuffers(1, &gl_id);
    if(!gl_id)
    {
    	vdLogGlobalError("Failed to create buffer object!");
		return NULL;
	}
	
    GLenum gl_target = ConvertBufferTargetToGL(target);
    GLenum gl_update = ConvertBufferAccessUpdateModeToGL(access, update);
    GLenum gl_datatype = ConvertBufferTypeIdToGL(datatype);
    size_t gl_bytes = GetSizeOfTypeIdFromGL(gl_datatype) * components * count;

    glBindBuffer(gl_target, gl_id);
    glBufferData(gl_target, gl_bytes, ptr, gl_update);
    glBindBuffer(gl_target, 0);

    vdGlobalAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error after creating buffer!");

    Buffer::Data data;
    Core::Memory::SetBytes(&data, 0, sizeof(data));

    data.Id = gl_id;
    data.Target = target;
    data.Attribute = attrib;
    data.AccessMode = access;
    data.UpdateMode = update;
    data.DataType = datatype;
    data.Components = components;
    data.Count = count;
    data.Bytes = gl_bytes;
    data.Index = m_Buffers.size();
    data.State = Buffer::StateId::Allocated;

    Buffer* buffer = VD_NEW(Buffer, this);
    buffer->Setup(data);
    m_Buffers.push_back(buffer);

    vdLogOpenGLErrors("End");
    return buffer;
}

vd::status
Context::Acquire(
    Buffer* buffer)
{
    vdLogOpenGLErrors("Start");
    vdAssert(buffer != NULL);

    vdLogOpenGLErrors("End");
    return Status::Code::Success;
}

vd::status
Context::Release(
    Buffer* buffer)
{
    vdAssert(buffer != NULL);
    vdLogOpenGLErrors("Start");

    Unbind(buffer);

    const Buffer::Data* data = buffer->GetPtr();
    vdAssert(data != NULL);

    if(data->Id)
    {
        GLuint gl_id = ConvertObjectIdToGL(data->Id);
        glDeleteBuffers(1, &gl_id);
    }

    if(data->Index < m_Buffers.size())
        m_Buffers[data->Index] = NULL;

    vdLogOpenGLErrors("End");
    return Status::Code::Success;
}

vd::status
Context::Retain(
    Buffer* buffer)
{
    vdLogOpenGLErrors("Start");
    vdAssert(buffer != NULL);

    const Buffer::Data* data = buffer->GetPtr();
    vdAssert(data != NULL);

    bool managed = false;

    for(vd::i32 i = 0; i < (vd::i32)m_Buffers.size(); i++)
    {
        if(m_Buffers[i] == NULL)
            continue;

        if(m_Buffers[i]->GetPtr()->Id == data->Id)
            managed = true;
    }

    if(managed == false)
    {
        Buffer::Data clone;
        Core::Memory::CopyBytes(&clone, &data, sizeof(clone));
        clone.Index = m_Buffers.size();
        m_Buffers.push_back(buffer);
    }

    vdLogOpenGLErrors("End");
    return Status::Code::Success;
}


vd::status
Context::Destroy(
	Buffer* buffer)
{
    vdAssert(buffer != NULL);
    vdLogOpenGLErrors("Start");
    
    Release(buffer);
    VD_DELETE(buffer);

    vdLogOpenGLErrors("End");
    return Status::Code::Success;
}

void* 
Context::Map(
    Buffer* buffer, 
    Buffer::AccessMode::Value access)
{
    vdLogOpenGLErrors("Start");

    vdAssert(buffer != NULL);
    const Buffer::Data& data = buffer->GetData();
    GLuint gl_id = ConvertObjectIdToGL(data.Id);
    if(gl_id < 1) return NULL;

	GLenum gl_access = ConvertBufferAccessModeToGL(access);
    GLenum gl_target = ConvertBufferTargetToGL(data.Target);

    buffer->Bind();
    buffer->SetState(Buffer::StateId::Mapped);

    glBindBuffer(gl_target, gl_id);
    void* ptr = glMapBuffer(gl_target, gl_access);

    vdLogOpenGLErrors("End");
    return ptr;
}

vd::status 
Context::Unmap(
	Buffer* buffer)
{
    vdLogOpenGLErrors("Start");

    vdAssert(buffer != NULL);
    if(buffer->GetState() != Buffer::StateId::Mapped)
        return Status::Code::Reject;

    const Buffer::Data& data = buffer->GetData();
    GLenum gl_target = ConvertBufferTargetToGL(data.Target);

    glUnmapBuffer(gl_target);
    glBindBuffer(gl_target, 0);

    buffer->Unbind();
    buffer->SetState(Buffer::StateId::Allocated);

    vdLogOpenGLErrors("End");

    return Status::Code::Success;
}

vd::status
Context::Bind(
	Buffer* buffer)
{
    vdLogOpenGLErrors("Start");

    vdAssert(buffer != NULL);
    if(buffer->IsActive() == true)
        return Status::Code::Reject;

    const Buffer::Data& data = buffer->GetData();

    GLuint gl_id = ConvertObjectIdToGL(data.Id);
    GLenum gl_target = ConvertBufferTargetToGL(data.Target);
    GLenum gl_attrib = ConvertBufferAttributeToGL(data.Attribute);
	GLenum gl_datatype = ConvertBufferTypeIdToGL(data.DataType);
	GLuint gl_components = (GLuint)data.Components;
	GLuint gl_stride = 0;
	const GLvoid* gl_addr = 0;

    if(gl_id < 1) return Status::Code::Reject;
    
    buffer->Bind();                           	
    buffer->SetState(Buffer::StateId::Bound);

    glBindBuffer(gl_target, gl_id);
	switch(gl_attrib)
	{
		case(GL_VERTEX_ARRAY):
		{   
			glVertexPointer(gl_components, gl_datatype, gl_stride, gl_addr);
			glEnableClientState(GL_VERTEX_ARRAY);
			break;
		}
		case(GL_COLOR_ARRAY):
		{   
			glColorPointer(gl_components, gl_datatype, gl_stride, gl_addr);
			glEnableClientState(GL_COLOR_ARRAY);
			break;
		}
		case(GL_NORMAL_ARRAY):
		{
			glNormalPointer(gl_datatype, gl_stride, gl_addr);
			glEnableClientState(GL_NORMAL_ARRAY);
			break;
		}
		case(GL_TEXTURE_COORD_ARRAY):
		{
			glTexCoordPointer(gl_components, gl_datatype, gl_stride, gl_addr);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);        
			break;
		}
		case(GL_INDEX_ARRAY):
        {
            break;
        }
		case(GL_EDGE_FLAG_ARRAY):
		default:
			break;
	};

    vdLogOpenGLErrors("End");
    
    return Status::Code::Success;
}

vd::status 
Context::Submit(
    Buffer* buffer, 
    Geometry::PrimitiveType::Value primitives, 
    vd::u32 offset, vd::u32 count)
{
    vdAssert(buffer != NULL);
    vdLogOpenGLErrors("Start");

    vdAssert(buffer != NULL);
    if(buffer->GetState() != Buffer::StateId::Bound)
        return Status::Code::Reject;

    const Buffer::Data& data = buffer->GetData();

    GLuint gl_id = ConvertObjectIdToGL(data.Id);
    GLenum gl_prim = ConvertGeometryPrimitiveTypeToGL(primitives);
    GLenum gl_datatype = ConvertBufferTypeIdToGL(data.DataType);

    if(data.Attribute == Buffer::AttributeType::Index)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_id);
        glDrawElements(gl_prim, count, gl_datatype, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    else
    {
        glDrawArrays( gl_prim, offset, count );
    }

    vdLogOpenGLErrors("End");
    return Status::Code::Success;
}

vd::status
Context::Unbind(
	Buffer* buffer)
{
    vdAssert(buffer != NULL);
    vdLogOpenGLErrors("Start");
    
    if(buffer->IsActive() == false)
        return Status::Code::Reject;

    if(buffer->GetState() != Buffer::StateId::Bound)
        return Status::Code::Reject;

    const Buffer::Data& data = buffer->GetData();
    GLuint gl_id = ConvertObjectIdToGL(data.Id);
    GLenum gl_target = ConvertBufferTargetToGL(data.Target);
    GLenum gl_attrib = ConvertBufferAttributeToGL(data.Attribute);

    glBindBuffer(gl_target, gl_id);
	switch(gl_attrib)
	{
		case(GL_VERTEX_ARRAY):
		{   
			glDisableClientState(GL_VERTEX_ARRAY);
			break;
		}
		case(GL_COLOR_ARRAY):
		{   
			glDisableClientState(GL_COLOR_ARRAY);
			break;
		}
		case(GL_NORMAL_ARRAY):
		{
			glDisableClientState(GL_NORMAL_ARRAY);
			break;
		}
		case(GL_TEXTURE_COORD_ARRAY):
		{
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);        
			break;
		}
		case(GL_INDEX_ARRAY):
        {
            break;
        }
		case(GL_EDGE_FLAG_ARRAY):
		default:
			break;
	};
    
    glBindBuffer(gl_target, 0);

    buffer->Unbind();
    buffer->SetState(Buffer::StateId::Allocated);

    vdLogOpenGLErrors("End");
    return Status::Code::Success;
}

// ======================================================================================================= //

vd::status
Context::Acquire(
    Geometry* geo)
{
    vdLogOpenGLErrors("Start");
    vdAssert(geo != NULL);

    vdLogOpenGLErrors("End");
    return Status::Code::Success;
}

vd::status
Context::Release(
    Geometry* geo)
{
    vdLogOpenGLErrors("Start");
    vdAssert(geo != NULL);

    Unbind(geo);

    const Geometry::Data& data = geo->GetData();
    for(vd::u32 i = Geometry::AttributeSlot::StartIndex; i < Geometry::AttributeSlot::Count; i++)
    {
        vd::u32 index = data.Buffers[i];
        if(m_Buffers.size() < index)
            continue;

        Buffer* buffer = m_Buffers[index];
        if(buffer == NULL)
            continue;

        Release(buffer);
    }

    vdLogOpenGLErrors("End");
    return Status::Code::Success;
}

vd::status
Context::Retain(
    Geometry* geo)
{
    vdLogOpenGLErrors("Start");
    vdAssert(geo != NULL);

    const Geometry::Data* data = geo->GetPtr();
    vdAssert(data != NULL);

    bool managed = false;

    for(vd::i32 i = 0; i < (vd::i32)m_Geometry.size(); i++)
    {
        if(m_Geometry[i] == NULL)
            continue;

        if(m_Geometry[i]->GetPtr()->Id == data->Id)
            managed = true;
    }

    if(managed == false)
    {
        Geometry::Data clone;
        Core::Memory::CopyBytes(&clone, &data, sizeof(clone));
        clone.Index = m_Geometry.size();
        m_Geometry.push_back(geo);
    }

    vdLogOpenGLErrors("End");
    return Status::Code::Success;
}

vd::status
Context::Destroy(
	Geometry* geo)
{
    vdAssert(geo != NULL);
    vdLogOpenGLErrors("Start");

    Release(geo);
    VD_DELETE(geo);

    vdLogOpenGLErrors("End");
    return Status::Code::Success;	
}

// ======================================================================================================= //

Geometry*
Context::CreatePointList(
	vd::f32* positions, vd::i32 components, vd::u64 count)
{
    vdLogOpenGLErrors("Start");

    Geometry::Data data;
    Core::Memory::SetBytes(&data, 0, sizeof(data));

    data.IndexCount = 0;
    data.PrimitiveType = Geometry::PrimitiveType::Points; // GL_POINTS;
    data.PrimitiveCount = count;
    data.Index = m_Geometry.size();
    data.Id = data.Index;

    Buffer* position_buffer = CreateBuffer(
        Buffer::TargetType::ArrayBuffer, 
        Buffer::AttributeType::Position,
        Buffer::AccessMode::ReadOnly,
        Buffer::UpdateMode::Static,
        Buffer::TypeId::F32,
        components, data.PrimitiveCount, positions
    );

    const Buffer::Data& pbd = position_buffer->GetData();

    Geometry* geo = VD_NEW(Geometry, this);
    geo->Setup(data);
    geo->Attach(Geometry::AttributeSlot::Position, pbd.Index);
    m_Geometry.push_back(geo);

    vdLogOpenGLErrors("End");

    return geo;
}

Geometry*
Context::CreatePoint(
	vd::f32 x, vd::f32 y, vd::f32 z)
{
    vdLogOpenGLErrors("Start");

    vd::f32 positions[] = {
    	x, y, z, 1.0f
    };
    
	unsigned int faces[] = { 0 };
    
    Geometry* geo = VD_NEW(Geometry, this);
    Geometry::Data data = geo->GetData();
    data.IndexCount = 1;
    data.PrimitiveCount = 1;
    data.PrimitiveType = Geometry::PrimitiveType::Points; // GL_POINTS;
    data.Index = m_Geometry.size();
    data.Id = data.Index;
    
    Buffer* position_buffer = CreateBuffer(
        Buffer::TargetType::ArrayBuffer, 
        Buffer::AttributeType::Position,
        Buffer::AccessMode::ReadOnly,
        Buffer::UpdateMode::Static,
        Buffer::TypeId::F32,
        4, data.PrimitiveCount, positions
    );

    Buffer* index_buffer = CreateBuffer(
        Buffer::TargetType::IndexBuffer, 
        Buffer::AttributeType::Index,
        Buffer::AccessMode::ReadOnly,
        Buffer::UpdateMode::Static,
        Buffer::TypeId::U32,
        1, data.IndexCount, faces
    );

    const Buffer::Data& pbd = position_buffer->GetData();
    const Buffer::Data& ibd = index_buffer->GetData();
  
    geo->Setup(data);
    geo->Attach(Geometry::AttributeSlot::Position, pbd.Index);
    geo->Attach(Geometry::AttributeSlot::Index, ibd.Index);
    m_Geometry.push_back(geo);

    vdLogOpenGLErrors("End");
    return geo;
}

Geometry*
Context::CreateQuad(
	vd::f32 left,  vd::f32 top, 
    vd::f32 right, vd::f32 bottom,
    vd::f32 u0, vd::f32 u1, vd::f32 v0, vd::f32 v1)
{
    vdLogOpenGLErrors("Start");

    const vd::f32 positions[] = {
        left,   top,   
        right,  top,    
        right,  bottom, 
        left,   bottom, 
    };

    const vd::f32 texcoords[] = {
        u0, v1,
        u1, v1,
        u1, v0,
        u0, v0,
    };

    const unsigned int faces[] = { 3, 2, 1, 1, 0, 3 };

    Geometry::Data data;
    Core::Memory::SetBytes(&data, 0, sizeof(data));
    data.IndexCount = 6;
    data.PrimitiveCount = 4;
    data.PrimitiveType = Geometry::PrimitiveType::Triangles; // GL_TRIANGLES;
    data.Index = m_Geometry.size();
    data.Id = data.Index;

    Buffer* position_buffer = CreateBuffer(
        Buffer::TargetType::ArrayBuffer, 
        Buffer::AttributeType::Position,
        Buffer::AccessMode::ReadOnly,
        Buffer::UpdateMode::Static,
        Buffer::TypeId::F32,
        2, data.PrimitiveCount, positions
    );

    Buffer* texcoord_buffer = CreateBuffer(
        Buffer::TargetType::ArrayBuffer, 
        Buffer::AttributeType::TexCoord,
        Buffer::AccessMode::ReadOnly,
        Buffer::UpdateMode::Static,
        Buffer::TypeId::F32,
        2, data.PrimitiveCount, texcoords
    );

    Buffer* index_buffer = CreateBuffer(
        Buffer::TargetType::IndexBuffer, 
        Buffer::AttributeType::Index,
        Buffer::AccessMode::ReadOnly,
        Buffer::UpdateMode::Static,
        Buffer::TypeId::U32,
        1, data.IndexCount, faces
    );
    
    const Buffer::Data& pbd = position_buffer->GetData();
    const Buffer::Data& tbd = texcoord_buffer->GetData();
    const Buffer::Data& ibd = index_buffer->GetData();

    Geometry* geo = VD_NEW(Geometry, this);
    geo->Setup(data);
    geo->Attach(Geometry::AttributeSlot::Position, pbd.Index);
    geo->Attach(Geometry::AttributeSlot::TexCoord, tbd.Index);
    geo->Attach(Geometry::AttributeSlot::Index, ibd.Index);    
	m_Geometry.push_back(geo);

    vdLogOpenGLErrors("End");
    return geo;
}

Geometry*
Context::CreateQuad()
{
	return CreateQuad(-1.0f, +1.0f, +1.0f, -1.0f, 
                      +0.0f, +1.0f, +0.0f, +1.0f);
}

Geometry*
Context::CreateSphere(
    vd::v3f32 center,
    vd::f32 radius,
    vd::i32 stacks, 
    vd::i32 slices)
{
    std::vector<vd::f32> positions;
    std::vector<vd::f32> normals;
    std::vector<vd::f32> texcoords;
    std::vector<vd::u32> indices;

    size_t count = (stacks-2) * slices + 2;
    positions.resize(count * 3);
    normals.resize(count * 3);
    texcoords.resize(count * 2);

    vd::i32 i = 0;
    vd::i32 j = 0;

    vd::u64 vidx = 0;
    vd::f32 mpi = Constants::Pi;
    for (i = 1; i < (stacks-1); i++)
    {
        for (j = 0; j < slices; j++)
        {
            vd::f32 y = 1.0 - vd::f32(i) / vd::f32(stacks-1) * 2.0f;
            vd::f32 r = Core::Sin( Core::ArcCos(y) ); 
            vd::f32 x = r * Core::Sin(vd::f32(j) / vd::f32(slices) * mpi * 2.0f);            
            vd::f32 z = r * Core::Cos(vd::f32(j) / vd::f32(slices) * mpi * 2.0f);

            vd::f32 px = center.x + x * radius;
            vd::f32 py = center.y + y * radius;
            vd::f32 pz = center.z + z * radius;

            vd::v3f32 nm(x,y,z);
            vd::v3f32 n = Core::Normalise(nm);

            vd::v2f32 tc((vd::f32)(i) / vd::f32(stacks-1), (vd::f32)(j) / vd::f32(slices));

            vidx = (i-1)*slices+j;
            positions[vidx * 3 + 0] = px;
            positions[vidx * 3 + 1] = py;
            positions[vidx * 3 + 2] = pz;

            normals[vidx * 3 + 0] = n.x;
            normals[vidx * 3 + 1] = n.y;
            normals[vidx * 3 + 2] = n.z;
            
            texcoords[vidx * 2 + 0] = tc.x;
            texcoords[vidx * 2 + 1] = tc.y;
        }
    }

    vidx = (stacks-2)*slices;
    positions[vidx * 3 + 0] = center.x + 0.0;
    positions[vidx * 3 + 1] = center.y + 1.0 * radius;
    positions[vidx * 3 + 2] = center.z + 0.0;

    normals[vidx * 3 + 0] = 0.0f;
    normals[vidx * 3 + 1] = 1.0f;
    normals[vidx * 3 + 2] = 0.0f;

    texcoords[vidx * 2 + 0] = 0.0;
    texcoords[vidx * 2 + 1] = 0.0;

    vidx = (stacks-2)*slices+1;
    positions[vidx * 3 + 0] = center.x + 0.0;
    positions[vidx * 3 + 1] = center.y - 1.0 * radius;
    positions[vidx * 3 + 2] = center.z + 0.0;

    normals[vidx * 3 + 0] = 0.0f;
    normals[vidx * 3 + 1] = 1.0f;
    normals[vidx * 3 + 2] = 0.0f;

    texcoords[vidx * 2 + 0] = 1.0;
    texcoords[vidx * 2 + 1] = 1.0;

    for (i = 1; i < (stacks-2); i++)
    {
        for (j = 0; j < (slices-1); j++)
        {
            indices.push_back((i-1)*slices+j);
            indices.push_back((i-1)*slices+j+1);
            indices.push_back((i)*slices+j);

            indices.push_back((i-1)*slices+j+1);
            indices.push_back((i)*slices+j+1);
            indices.push_back((i)*slices+j);
        }

        indices.push_back((i-1)*slices+slices-1);
        indices.push_back((i-1)*slices);
        indices.push_back((i)*slices+j);

        indices.push_back((i)*slices);
        indices.push_back((i-1)*slices);
        indices.push_back((i)*slices+j);
    }       

    for (j = 0; j< (slices-1); j++)
    {
        indices.push_back(j);
        indices.push_back(j+1);
        indices.push_back((stacks-2)*slices);
    }
    indices.push_back(j);
    indices.push_back(0);
    indices.push_back((stacks-2)*slices);

    for (j = 0; j< (slices-1); j++)
    {
        indices.push_back((stacks-3)*slices+j);
        indices.push_back((stacks-3)*slices+j+1);
        indices.push_back((stacks-2)*slices+1);
    }
    indices.push_back((stacks-3)*slices+j);
    indices.push_back((stacks-3)*slices);
    indices.push_back((stacks-2)*slices+1);

    Geometry::Data data;
    Core::Memory::SetBytes(&data, 0, sizeof(data));
    data.IndexCount = indices.size();
    data.PrimitiveCount = positions.size() / 3;
    data.PrimitiveType = Geometry::PrimitiveType::Triangles; // GL_TRIANGLES;
    data.Index = m_Geometry.size();
    data.Id = data.Index;

    Buffer* position_buffer = CreateBuffer(
        Buffer::TargetType::ArrayBuffer, 
        Buffer::AttributeType::Position,
        Buffer::AccessMode::ReadOnly,
        Buffer::UpdateMode::Static,
        Buffer::TypeId::F32,
        3, data.PrimitiveCount, &positions[0]
    );

    Buffer* normal_buffer = CreateBuffer(
        Buffer::TargetType::ArrayBuffer, 
        Buffer::AttributeType::Normal,
        Buffer::AccessMode::ReadOnly,
        Buffer::UpdateMode::Static,
        Buffer::TypeId::F32,
        3, data.PrimitiveCount, &normals[0]
    );

    Buffer* texcoord_buffer = CreateBuffer(
        Buffer::TargetType::ArrayBuffer, 
        Buffer::AttributeType::TexCoord,
        Buffer::AccessMode::ReadOnly,
        Buffer::UpdateMode::Static,
        Buffer::TypeId::F32,
        2, data.PrimitiveCount, &texcoords[0]
    );

    Buffer* index_buffer = CreateBuffer(
        Buffer::TargetType::IndexBuffer, 
        Buffer::AttributeType::Index,
        Buffer::AccessMode::ReadOnly,
        Buffer::UpdateMode::Static,
        Buffer::TypeId::U32,
        1, data.IndexCount, &indices[0]
    );
    
    const Buffer::Data& pbd = position_buffer->GetData();
    const Buffer::Data& nbd = normal_buffer->GetData();
    const Buffer::Data& tbd = texcoord_buffer->GetData();
    const Buffer::Data& ibd = index_buffer->GetData();

    Geometry* geo = VD_NEW(Geometry, this);
    geo->Setup(data);
    geo->Attach(Geometry::AttributeSlot::Position, pbd.Index);
    geo->Attach(Geometry::AttributeSlot::TexCoord, tbd.Index);
    geo->Attach(Geometry::AttributeSlot::Normal, nbd.Index);
    geo->Attach(Geometry::AttributeSlot::Index, ibd.Index);    
    m_Geometry.push_back(geo);

    vdLogGlobalDebug("Created Sphere: PrimitiveCount: %d IndexCount: %d\n", data.PrimitiveCount, data.IndexCount);
    vdLogOpenGLErrors("End");
    return geo;
}

Geometry*
Context::CreateWireGrid(
	int rows, int columns,
	vd::f32 left, vd::f32 right,
	vd::f32 bottom, vd::f32 top)
{
    vdLogOpenGLErrors("Start");

    vd::f32 fDX = (right - left) / (columns-1.0f);
    vd::f32 fDY = (top - bottom) / (rows-1.0f);
    size_t vertex_count = (rows+1) * 2 + (columns+1) * 2;

    Geometry::Data data;
    Core::Memory::SetBytes(&data, 0, sizeof(data));

    data.IndexCount = vertex_count;
    data.PrimitiveCount = vertex_count;
    data.PrimitiveType = Geometry::PrimitiveType::Lines; //GL_LINES;
    data.Index = m_Geometry.size();
    data.Id = data.Index;

	vd::f32* positions = VD_NEW_ARRAY(vd::f32, vertex_count * 4);
	vd::f32* p = positions;
	for (int i=0; i<=rows; i++) 
	{
		vd::f32 fX = left + columns * fDX;
		vd::f32 fY = bottom + i * fDY;

		*p++ = 0.0f;
		*p++ = fY;
		*p++ = 0.0f;
		*p++ = 1.0f;

		*p++ = fX;
		*p++ = fY;
		*p++ = 0.0f;
		*p++ = 1.0f;

	}

	for (int i=0; i<=columns; i++) 
	{
		vd::f32 fX = left + i * fDX;
		vd::f32 fY = bottom + rows * fDY;

		*p++ = fX;
		*p++ = 0.0f;
		*p++ = 0.0f;
		*p++ = 1.0f;

		*p++ = fX;
		*p++ = fY;
		*p++ = 0.0f;
		*p++ = 1.0f;
	}

	vdLogGlobalDebug("Allocated Vertices: %d\n", vertex_count * 4);

    Buffer* position_buffer = CreateBuffer(
        Buffer::TargetType::ArrayBuffer, 
        Buffer::AttributeType::Position,
        Buffer::AccessMode::ReadOnly,
        Buffer::UpdateMode::Static,
        Buffer::TypeId::F32,
        4, data.PrimitiveCount, positions
    );
    VD_DELETE_ARRAY(positions);
    
    const Buffer::Data& pbd = position_buffer->GetData();

    Geometry* geo = VD_NEW(Geometry, this);
    geo->Setup(data);
    geo->Attach(Geometry::AttributeSlot::Position, pbd.Index);
    m_Geometry.push_back(geo);

    vdLogGlobalDebug("Created Wire Grid: PrimitiveCount: %d IndexCount: %d\n", data.PrimitiveCount, data.IndexCount); 
    vdLogOpenGLErrors("End");
    return geo;		
}

	
Geometry*
Context::CreateGrid(
	int uiCountX, int uiCountY,
	vd::f32 fX0, vd::f32 fX1, 
	vd::f32 fY0, vd::f32 fY1, 
	bool bVertical)
{
    vdLogOpenGLErrors("Start");

    vd::f32 fDX = (fX1 - fX0) / (uiCountX-1.0f);
    vd::f32 fDY = (fY1 - fY0) / (uiCountY-1.0f);

	vd::f32 fTU = (1.0f) / (uiCountX-1.0f);
	vd::f32 fTV = (1.0f) / (uiCountY-1.0f);

    size_t vertex_count = uiCountX * uiCountY;
    size_t index_count = 2 * (uiCountY - 1) * uiCountX;

    Geometry::Data data;
    Core::Memory::SetBytes(&data, 0, sizeof(data));

    data.IndexCount = index_count;
    data.PrimitiveCount = vertex_count;
    data.PrimitiveType = Geometry::PrimitiveType::QuadStrip; //GL_QUAD_STRIP;
    data.Index = m_Geometry.size();
    data.Id = data.Index;

    vd::u64 pi = 0;
	vd::f32* positions = VD_NEW_ARRAY(vd::f32, 4 * vertex_count);
	for(int y=0; y < uiCountY; y++)
	{
		for(int x=0; x < uiCountX; x++)
		{           
			vd::f32 fX = fX0 + x * fDX;
			vd::f32 fY = fY0 + y * fDY;
			
			if(bVertical)
			{
				positions[pi++] = fX;
				positions[pi++] = fY;
				positions[pi++] = 0.0f;
				positions[pi++] = 1.0f;
			}
			else
			{
				positions[pi++] = fX;
				positions[pi++] = 0.0f;
				positions[pi++] = fY;
				positions[pi++] = 1.0f;
			}	
		}
	}

    Buffer* position_buffer = CreateBuffer(
        Buffer::TargetType::ArrayBuffer, 
        Buffer::AttributeType::Position,
        Buffer::AccessMode::ReadOnly,
        Buffer::UpdateMode::Static,
        Buffer::TypeId::F32,
        4, data.PrimitiveCount, positions
    );

	VD_DELETE_ARRAY(positions);

	size_t ti = 0;
	vd::f32* texcoords = VD_NEW_ARRAY(vd::f32, 2 * vertex_count );
	for(int y=0; y < uiCountY; y++)
	{
		for(int x=0; x < uiCountX; x++)
		{           
			vd::f32 fU = x * fTU;
			vd::f32 fV = y * fTV;

			texcoords[ti++] = fU;
			texcoords[ti++] = fV;
		}
	}

    Buffer* texcoord_buffer = CreateBuffer(
        Buffer::TargetType::ArrayBuffer, 
        Buffer::AttributeType::TexCoord,
        Buffer::AccessMode::ReadOnly,
        Buffer::UpdateMode::Static,
        Buffer::TypeId::F32,
        2, data.PrimitiveCount, texcoords
    );

	VD_DELETE_ARRAY(texcoords);
	
	size_t fi = 0;	
	unsigned int* faces = VD_NEW_ARRAY(unsigned int, index_count);
	for(int y = 0; y < uiCountY - 1; y++)
	{
		if(IsOdd(y))
		{
			for(int x = uiCountX - 1; x >= 0; x--)
			{
				faces[fi++] = (uiCountX * (y + 1) + x);
				faces[fi++] = (uiCountX * (y + 0) + x);
			}
		}
		else
		{
			for(int x = 0; x < uiCountX; x++)
			{
				faces[fi++] = (uiCountX * (y + 0) + x);
				faces[fi++] = (uiCountX * (y + 1) + x);
			}
		}
	}

    Buffer* index_buffer = CreateBuffer(
        Buffer::TargetType::IndexBuffer, 
        Buffer::AttributeType::Index,
        Buffer::AccessMode::ReadOnly,
        Buffer::UpdateMode::Static,
        Buffer::TypeId::U32,
        1, data.IndexCount, faces
    );

	VD_DELETE_ARRAY(faces);

    const Buffer::Data& pbd = position_buffer->GetData();
    const Buffer::Data& tbd = texcoord_buffer->GetData();
    const Buffer::Data& ibd = index_buffer->GetData();

    Geometry* geo = VD_NEW(Geometry, this);
    geo->Setup(data);
    geo->Attach(Geometry::AttributeSlot::Position, pbd.Index);
    geo->Attach(Geometry::AttributeSlot::TexCoord, tbd.Index);
    geo->Attach(Geometry::AttributeSlot::Index, ibd.Index);    
    m_Geometry.push_back(geo);

    vdLogGlobalDebug("Created Grid: PrimitiveCount: %d IndexCount: %d\n", data.PrimitiveCount, data.IndexCount);
    vdLogOpenGLErrors("End");
    return geo;
}

vd::status 
Context::Bind(
    Geometry* geo)
{
    vdLogOpenGLErrors("Start");

    const Geometry::Data& data = geo->GetData();

    vdGlobalAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error prior to render geometry.");

    for(vd::u32 i = Geometry::AttributeSlot::StartIndex; i < Geometry::AttributeSlot::Count; i++)
    {
        vd::u32 index = data.Buffers[i];
        vd::u32 slot = data.Bindings[i];
        Geometry::AttributeSlot::Value attrib = Geometry::AttributeSlot::FromInteger(i);

        if(m_Buffers.size() < index)
            return Status::Code::Reject;
        
        Buffer* buffer = m_Buffers[index];
        if(buffer == NULL)
            return Status::Code::Reject;
                
        Bind(buffer);
        Bind(attrib, buffer, slot);
    }

    geo->Bind();

    vdLogOpenGLErrors("End");

    return Status::Code::Success;
}

vd::status 
Context::Submit(
    Geometry* geo,
    Shader::Pass::Value pass,
    vd::u32 start, 
    vd::u32 end, 
    vd::u32 stride)
{
    vdLogOpenGLErrors("Start");

    const Geometry::Data& data = geo->GetData();

    Shader* shader = NULL;
    if(Shader::Pass::IsValid(pass))
    {
        vd::u32 pass_index = Shader::Pass::ToInteger(pass);
        vd::u32 shader_index = data.Shaders[pass_index];
        if(shader_index < m_Shaders.size())
            shader = m_Shaders[shader_index];
    }

    if(shader != NULL)
        shader->Bind();

    vdGlobalAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error prior to render geometry.");
    if(geo->IsBufferUsed(Geometry::AttributeSlot::Index))
    {
        vd::u32 attrib = Geometry::AttributeSlot::ToInteger(Geometry::AttributeSlot::Index);
        vd::u32 index = data.Buffers[attrib];
        if(m_Buffers.size() < index)
            return Status::Code::Reject;
        
        Buffer* buffer = m_Buffers[index];
        if(buffer == NULL)
            return Status::Code::Reject;


        GLuint gl_id = ConvertObjectIdToGL(buffer->GetData().Id);
        GLenum gl_prim = ConvertGeometryPrimitiveTypeToGL(data.PrimitiveType);
        GLenum gl_datatype = ConvertBufferTypeIdToGL(buffer->GetData().DataType);

        vdGlobalAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error pre submit index buffer.");
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_id);

        vdGlobalAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error post submit index buffer.");
        glDrawElements(gl_prim, data.IndexCount, gl_datatype, 0);

        vdGlobalAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error during draw elements.");
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//        Bind(buffer);
//        Submit(buffer, data.PrimitiveType, 0, data.IndexCount);
//        Unbind(buffer);

        vdGlobalAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error during draw elements.");
    }
    else
    {
        vdGlobalAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error pre submit draw arrays.");
        GLenum gl_prim = ConvertGeometryPrimitiveTypeToGL(data.PrimitiveType);
        glDrawArrays( gl_prim, 0, data.PrimitiveCount );
        vdGlobalAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error post submit draw arrays.");
    }

    if(shader != NULL)
        shader->Unbind();

    vdLogOpenGLErrors("End");
    return Status::Code::Success;
}

vd::status 
Context::Unbind(
    Geometry* geo)
{
    vdLogOpenGLErrors("Start");

    vdAssert(geo != NULL);
    const Geometry::Data& data = geo->GetData();
    vdGlobalAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error prior to render geometry.");

    for(vd::u32 i = Geometry::AttributeSlot::StartIndex; i < Geometry::AttributeSlot::Count; i++)
    {
        vd::u32 slot = data.Bindings[i];
        vd::u32 index = data.Buffers[i];
        Geometry::AttributeSlot::Value attrib = Geometry::AttributeSlot::FromInteger(i);
        
        if(m_Buffers.size() < index)
            return Status::Code::Reject;
        
        Buffer* buffer = m_Buffers[index];
        if(buffer == NULL)
            return Status::Code::Reject;
        
        Unbind(attrib, buffer, slot);
        Unbind(buffer);
    }

    if(geo->IsBufferUsed(Geometry::AttributeSlot::Index))
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    geo->Unbind();
    
    vdLogOpenGLErrors("End");
    return Status::Code::Success;
}

vd::status
Context::Attach(
    Geometry* geo, 
    Geometry::AttributeSlot::Value attrib, 
    vd::u32 buffer,
    vd::u32 slot)
{
    vdAssert(geo != NULL);
    return geo->Attach(attrib, buffer, slot);
}

vd::status
Context::Detach(
    Geometry* geo, 
    Geometry::AttributeSlot::Value attrib)
{
    vdAssert(geo != NULL);
    return geo->Detach(attrib);
}

vd::status
Context::Attach(
    Geometry* geo,
    Shader* shader,
    Shader::Pass::Value pass)
{
    bool attached = false;
    for(vd::u32 i = Geometry::AttributeSlot::StartIndex; i < Geometry::AttributeSlot::Count; i++)
    {
        Geometry::AttributeSlot::Value attrib = Graphics::Geometry::AttributeSlot::FromInteger(i);
        vd::u32 buffer = geo->GetBuffer(attrib);
        if(buffer < VD_INVALID_INDEX)
        {
            Core::Symbol name = Symbol::Register(Graphics::Geometry::AttributeSlot::ToString(attrib));
            vd::u32 slot = shader->GetAttributeSlot(name);
            if(slot < VD_INVALID_INDEX)
            {
                Attach(geo, attrib, buffer, slot);
                geo->Attach(pass, shader->GetData().Index);
                attached = true;
            }

            if(attached == false)
                vdLogDebug("Failed to attach Shader to Geometry -- '%s' attribute slot mismatch!", 
                    Graphics::Geometry::AttributeSlot::ToString(attrib));
            else
                vdLogDebug("Attached Shader to Geometry -- '%s' attribute slot!", 
                    Graphics::Geometry::AttributeSlot::ToString(attrib));

            if(attached)
                break;
        }
    }

    vdAssertMsg(attached == true, "Failed to attach Shader to Geometry!");
    vdAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error attaching shader to geometry!");        
    return Status::Code::Success;
}

vd::status
Context::Detach(
    Geometry* geo,
    Shader* shader,
    Shader::Pass::Value pass)
{
    bool located = false;

    for(vd::u32 i = Geometry::AttributeSlot::StartIndex; i < Geometry::AttributeSlot::Count; i++)
    {
        Graphics::Geometry::AttributeSlot::Value attrib = Graphics::Geometry::AttributeSlot::FromInteger(i);
        vd::u32 buffer = geo->GetBuffer(attrib);
        if(buffer < VD_INVALID_INDEX)
        {
            Detach(geo, attrib);
            geo->Detach(pass);
            located = true;
        }

        if(located == false)
            vdLogDebug("Failed to detach Shader from Geometry -- '%s' attribute slot mismatch!", 
                Graphics::Geometry::AttributeSlot::ToString(attrib));
        else
            vdLogDebug("Attached Shader to Geometry -- '%s' attribute slot!", 
                Graphics::Geometry::AttributeSlot::ToString(attrib));

        if(located)
            break;
    }

    vdAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error detaching shader from geometry!");        
    return Status::Code::Success;
}

vd::status
Context::Bind(
    Geometry::AttributeSlot::Value attrib,
    Buffer* buffer,
    vd::u32 slot)
{
    vdAssert(buffer != NULL);

    const Buffer::Data& data = buffer->GetData();
    GLuint components = data.Components;
    GLenum datatype = ConvertBufferTypeIdToGL(data.DataType);
    GLsizei stride = GetSizeOfTypeIdFromGL(datatype) * components;
    const GLvoid* ptr = data.Ptr;

    vdAssert(components >= 1);

    if(slot < Geometry::InvalidSlot)
    {    
        glVertexAttribPointer(slot, components, datatype, GL_FALSE, stride, ptr);
        glEnableVertexAttribArray(slot);    
    }
    else
    {
        switch(attrib)
        {
            case Geometry::AttributeSlot::Index:
            {
                // Used for submission of geometry
                break;
            }
            case Geometry::AttributeSlot::Position: 
            {
                glVertexPointer( components, datatype, stride, ptr);
                glEnableClientState( GL_VERTEX_ARRAY );
                break;
            }  
            case Geometry::AttributeSlot::Normal:
            {
                glNormalPointer( datatype, 0, ptr);
                glEnableClientState( GL_NORMAL_ARRAY );
                break;
            } 
            case Geometry::AttributeSlot::TexCoord:
            {
                glTexCoordPointer( components, datatype, stride, ptr );
                glEnableClientState( GL_TEXTURE_COORD_ARRAY );
                break;
            }    
            case Geometry::AttributeSlot::Color:
            {
                glColorPointer( components, datatype, stride, ptr );
                glEnableClientState( GL_COLOR_ARRAY );
                break;
            }
            default:
            {
                vdLogGlobalError("Invalid attribute slot '%s' specified for submitting geometry!",
                    Geometry::AttributeSlot::ToString(attrib));
                break;
            }    
        };
    }

    vdAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error enabling vertex attribute array!");        
    return Status::Code::Success;
}

vd::status
Context::Unbind(
    Geometry::AttributeSlot::Value attrib,
    Buffer* buffer, vd::u32 slot)
{
    if(slot < Geometry::InvalidSlot)
    {    
        glDisableVertexAttribArray(slot);
    }
    else 
    {
        switch(attrib)
        {
            case Geometry::AttributeSlot::Index:
            {
                // Used for submission of geometry
                break;
            }
            case Geometry::AttributeSlot::Position:
            {
                glDisableClientState( GL_VERTEX_ARRAY );
                break;
            }  
            case Geometry::AttributeSlot::Normal:
            {
                glDisableClientState( GL_NORMAL_ARRAY );
                break;
            } 
            case Geometry::AttributeSlot::TexCoord:
            {
                glDisableClientState( GL_TEXTURE_COORD_ARRAY );
                break;
            }    
            case Geometry::AttributeSlot::Color:
            {
                glDisableClientState( GL_COLOR_ARRAY );
                break;
            }    
            default:
            {
                vdLogGlobalError("Invalid attribute slot '%s' specified for submitting geometry!",
                    Geometry::AttributeSlot::ToString(attrib));
                break;
            }
        };
    }

    vdGlobalAssertMsg(glGetError() == GL_NO_ERROR, "OpenGL error disabling vertex attribute array!");     
       
    return Status::Code::Success;
}

// ============================================================================================== //


Framebuffer* 
Context::CreateFramebuffer(
    Graphics::Viewport viewport,
    Graphics::ChannelLayout::Value channel_layout,
     Graphics::ScalarTypeId::Value channel_datatype,
      Graphics::DepthFormat::Value depth_format,
     Graphics::ScalarTypeId::Value depth_datatype)
{
    GLuint gl_id = 0;
    GLuint gl_renderbuffer = 0;
    GLuint gl_color_texture = 0;
    GLuint gl_depth_texture = 0;

    GLuint width = viewport.Width;
    GLuint height = viewport.Height;

    vdLogOpenGLErrors("Start");

    glGenFramebuffers(1, &gl_id);
    glBindFramebuffer(GL_FRAMEBUFFER, gl_id);

    vdLogOpenGLErrors("Bind Framebuffer");

    if(Graphics::DepthFormat::IsValid(depth_format) && 
       depth_format != Graphics::DepthFormat::None)
    {
        GLenum gl_depth_format = ConvertDepthFormatToGL(depth_format, depth_datatype);
        GLenum gl_depth_datatype = ConvertScalarTypeIdToGL(depth_datatype);

        glGenTextures(1, &gl_depth_texture);
        glBindTexture(GL_TEXTURE_2D, gl_depth_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, gl_depth_format, 
                     width, height, 0, GL_DEPTH_COMPONENT, 
                     gl_depth_datatype, 0);
/*
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 
                     width, height, 0, GL_DEPTH_COMPONENT, 
                     GL_UNSIGNED_SHORT, 0);
*/
        vdLogOpenGLErrors("Unable to create depth texture");   
    }
    
    if(Graphics::ChannelLayout::IsValid(channel_layout) && 
       channel_layout != Graphics::ChannelLayout::None)
    {
        glGenTextures(1, &gl_color_texture);
        glBindTexture(GL_TEXTURE_2D, gl_color_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum gl_base = ConvertBaseChannelLayoutToGL(channel_layout);
        GLenum gl_type = ConvertScalarTypeIdToGL(channel_datatype);
        GLenum gl_internal = ConvertInternalChannelLayoutToGL(channel_layout, channel_datatype);

        vdLogDebug("Creating framebuffer attachment: Internal[%s : 0x%X] Base[%s : 0x%X] Type[%s : 0x%X] for '%s' '%s' ",
            ConvertGLEnumToString(gl_internal), gl_internal, 
            ConvertGLEnumToString(gl_base), gl_base, 
            ConvertGLEnumToString(gl_type), gl_type,
            Graphics::ChannelLayout::ToString(channel_layout), 
            Graphics::ScalarTypeId::ToString(channel_datatype));

        glTexImage2D(GL_TEXTURE_2D, 0, gl_internal, width, height, 0, gl_base, gl_type, 0);
        vdLogOpenGLErrors("Unable to create color texture");
    }

    if(gl_color_texture > 0 || gl_depth_texture > 0)
    {
        glGenRenderbuffers(1, &gl_renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, gl_renderbuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gl_color_texture, 0);
        vdLogOpenGLErrors("Unable to attach color texture to frame buffer");
        if(gl_depth_texture)
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gl_depth_texture, 0);
            vdLogOpenGLErrors("Unable to attach depth texture to frame buffer");
        }
        vdAssertMsg(GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus(GL_FRAMEBUFFER), "Unable to create framebuffer!\n");
        glDisable(GL_TEXTURE_2D);
    }

    Framebuffer::Data data;
    Core::Memory::SetBytes(&data, 0, sizeof(data));

    data.Viewport = viewport;
    data.ChannelLayout = channel_layout;
    data.ChannelDataType = channel_datatype;
    data.DepthFormat = depth_format;
    data.DepthDataType = depth_datatype;
    data.Renderbuffer = gl_renderbuffer;
    data.ColorTexture = gl_color_texture;
    data.DepthTexture = gl_depth_texture;
    data.Index = m_Framebuffers.size();
    data.Id = gl_id;

    Framebuffer* fb = VD_NEW(Framebuffer, this);
    fb->Setup(data);
    m_Framebuffers.push_back(fb);

    Bind(fb);
    Clear(fb, 0.0f, 0.0f, 0.0f, 0.0f);
    Unbind(fb);

    vdLogOpenGLErrors("End");
    return fb;
}


vd::status
Context::Acquire(
    Framebuffer* fb)
{
    vdLogOpenGLErrors("Start");
    vdAssert(fb != NULL);

    vdLogOpenGLErrors("End");
    return Status::Code::Success;
}

vd::status
Context::Release(
    Framebuffer* fb)
{
    vdLogOpenGLErrors("Start");
    vdAssert(fb != NULL);

    Unbind(fb);

    const Framebuffer::Data* data = fb->GetPtr();
    vdAssert(data != NULL);

    GLuint gl_id = 0;
    if(data->ColorTexture)
    {
        gl_id = ConvertObjectIdToGL(data->ColorTexture);
        glDeleteTextures(1, &gl_id);
    }

    if(data->DepthTexture)
    {
        gl_id = ConvertObjectIdToGL(data->DepthTexture);
        glDeleteTextures(1, &gl_id);
    }

    if(data->Id)
    {
        gl_id = ConvertObjectIdToGL(data->Id);
        glDeleteFramebuffers(1, &gl_id);
    }       

    if(data->Index < m_Framebuffers.size())
        m_Framebuffers[data->Index] = NULL;

    vdLogOpenGLErrors("End");
    return Status::Code::Success;
}

vd::status
Context::Retain(
    Framebuffer* fb)
{
    vdAssert(fb != NULL);
    vdLogOpenGLErrors("Start");

    const Framebuffer::Data* data = fb->GetPtr();
    vdAssert(data != NULL);

    bool managed = false;

    for(vd::i32 i = 0; i < (vd::i32)m_Framebuffers.size(); i++)
    {
        if(m_Framebuffers[i] == NULL)
            continue;

        if(m_Framebuffers[i]->GetPtr()->Id == data->Id)
            managed = true;
    }

    if(managed == false)
    {
        Framebuffer::Data clone;
        Core::Memory::CopyBytes(&clone, &data, sizeof(clone));
        clone.Index = m_Framebuffers.size();
        m_Framebuffers.push_back(fb);
    }

    vdLogOpenGLErrors("End");
    return Status::Code::Success;
}

vd::status 
Context::Destroy(
    Framebuffer* fb)
{
    vdLogOpenGLErrors("Start");
    vdAssert(fb != NULL);
    
    Release(fb);
    VD_DELETE(fb);

    vdLogOpenGLErrors("End");
    return Status::Code::Success;
}

vd::status 
Context::Bind(
    Framebuffer* fb)
{
    vdAssert(fb != NULL);
    vdLogOpenGLErrors("Start");

    if(fb->IsActive() == true)
        return Status::Code::Reject;

    fb->Bind();

    const Framebuffer::Data& data = fb->GetData();

    Viewport vp = data.Viewport;
    GLuint gl_id = ConvertObjectIdToGL(data.Id);

    glBindFramebuffer(GL_FRAMEBUFFER, gl_id);
    glViewport(vp.X, vp.Y, vp.Width, vp.Height);

    vdLogOpenGLErrors("End");

    return Status::Code::Success;
}

vd::status 
Context::Unbind(
    Framebuffer* fb)
{
    vdAssert(fb != NULL);
    vdLogOpenGLErrors("Start");

    vdGlobalAssertMsg(GL_NO_ERROR == glGetError(), "Error prior to unbinding frame buffer!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    if(fb->IsActive() == false)
        return Status::Code::Reject;

    fb->Unbind();

    vdLogOpenGLErrors("End");

    return Status::Code::Success;
}

vd::status 
Context::Clear(
    Framebuffer* fb,
    vd::f32 r, vd::f32 g, vd::f32 b, vd::f32 a)
{
    vdAssert(fb != NULL);
    vdLogOpenGLErrors("Start");

    if(fb->IsActive() == false)
        return Status::Code::Reject;

    int vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);

    const Framebuffer::Data& data = fb->GetData();
    const Viewport fpvp = data.Viewport;

    GLuint clear_flags = GL_COLOR_BUFFER_BIT;
    if(data.DepthTexture) clear_flags |= GL_DEPTH_BUFFER_BIT;

    glViewport(fpvp.X, fpvp.Y, fpvp.Width, fpvp.Height);
    glClearColor(r, g, b, a);

    glClear(clear_flags);
    glViewport(vp[0], vp[1], vp[2], vp[3]);
    vdLogOpenGLErrors("End");

    return Status::Code::Success;
}

// ============================================================================================== //

Graphics::Shader*
Context::CreateShaderFromFile(
    const vd::string& name, 
    const vd::string& vertex, 
    const vd::string& geometry, 
    const vd::string& fragment)
{
    vdLogOpenGLErrors("Start");

    Graphics::Shader::Data data;
    Memory::SetBytes(&data, 0, sizeof(data));
    data.Index = m_Shaders.size();

    Graphics::Shader* shader = VD_NEW(Shader, this, name);
    shader->Setup(data);
    shader->Load(vertex.c_str(), geometry.c_str(), fragment.c_str());
    m_Shaders.push_back(shader);

    vdLogOpenGLErrors("End");

    return shader;
}

Graphics::Shader*
Context::CreateShaderFromSource(
    const vd::string& name, 
    const vd::string& vertex, 
    const vd::string& geometry, 
    const vd::string& fragment)
{
    vdLogOpenGLErrors("Start");

    Graphics::Shader::Data data;
    Memory::SetBytes(&data, 0, sizeof(data));
    data.Index = m_Shaders.size();

    Graphics::Shader* shader = VD_NEW(Shader, this, name);
    shader->Setup(data);

    shader->Compile(
        vertex.size() ? vertex.c_str() : NULL,
        geometry.size() ? geometry.c_str() : NULL, 
        fragment.size() ? fragment.c_str() : NULL);

    m_Shaders.push_back(shader);

    vdLogOpenGLErrors("End");

    return shader;
}

vd::status 
Context::Acquire(
    Shader* s)
{
    vdAssert(s != NULL);
#if 0    
    if(data->Index < m_Shaders.size())
        m_Shaders[data->Index] = NULL;
#endif    
    return Status::Code::Success;
}

vd::status 
Context::Release(
    Shader* s)
{
    vdAssert(s != NULL);

    Unbind(s);

    const Shader::Data* data = s->GetPtr();
    vdAssert(data != NULL);

    if(data->Id)
    {
        GLuint gl_id = ConvertObjectIdToGL(data->Id);
        glDeleteProgram(gl_id);
    }
    return Status::Code::Success;
}

vd::status 
Context::Retain(
    Shader* s)
{
    vdAssert(s != NULL);

#if 0    
    if(data->Index < m_Shaders.size())
        m_Shaders[data->Index] = NULL;
#endif    
    return Status::Code::Success;
}

vd::status 
Context::Destroy(
    Shader* s)
{
    vdAssert(s != NULL);
    vdLogOpenGLErrors("Start");

    Release(s);
    VD_DELETE(s);
    
    vdLogOpenGLErrors("End");
    return Status::Code::Success;
}

vd::status 
Context::Bind(
    Shader* s)
{
    vdAssert(s != NULL);
    vdLogOpenGLErrors("Start");

    s->Bind();

    vdLogOpenGLErrors("End");
    return Status::Code::Success;
}

vd::status 
Context::Unbind(
    Shader* s)
{
    vdAssert(s != NULL);
    vdLogOpenGLErrors("Start");

    s->Unbind();

    vdLogOpenGLErrors("End");
    return Status::Code::Success;
}

// ============================================================================================== //

vd::status
Context::SetViewport(
    const Viewport& vp)
{
    vdLogOpenGLErrors("Start");

//    glViewport(vp.X, vp.Y, vp.Width, vp.Height);
    m_Viewport = vp;

    vdLogOpenGLErrors("End");

    return Status::Code::Success;
}

const Viewport&
Context::GetViewport()
{
    return m_Viewport;
}

vd::status
Context::BindOrthographicView()
{
    vdLogOpenGLErrors("Start");

    Viewport vp = m_Viewport;
    SetViewport(vp);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-1.0, +1.0, +1.0, -1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
//    glTranslatef(0.375, 0.375, 0);

    vdLogOpenGLErrors("End");

    return Status::Code::Success;
}

vd::status
Context::UnbindOrthographicView()
{
    vdLogOpenGLErrors("Start");

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    vdLogOpenGLErrors("End");

    return Status::Code::Success;    
}

vd::status
Context::SetActive(
    bool v)
{
    vdLogOpenGLErrors("Start");

    if(v)
    {
#if defined(VD_PLATFORM_OSX)        
        if(m_Native == NULL)
        {
            CGLContextObj cgl = CGLGetCurrentContext();
            m_Native = (void*)cgl;
        }
        else
        {
            CGLError error = CGLSetCurrentContext(m_Native);
            if(error != kCGLNoError)
            {
                const char* str = CGLErrorString(error);
                vdLogError("Failed to set current CGL Context: %s", str);
                return Status::Code::InvalidContext;
            }
        }
        return Status::Code::Success;
#endif
    }

    return Status::Code::Success;

}

bool
Context::IsActive()
{
    vdLogOpenGLErrors("Start");

#if defined(VD_PLATFORM_OSX)        
    CGLContextObj cgl = CGLGetCurrentContext();
    return (m_Native == ((void*)cgl);
#else
    return false;
#endif
}


// ============================================================================================== //

VD_IMPLEMENT_OBJECT(Context, vd_sym(OpenGLContext), vd_sym(GraphicsContext));

// ============================================================================================== //

VD_GRAPHICS_OPENGL_NAMESPACE_END();

// ============================================================================================== //
