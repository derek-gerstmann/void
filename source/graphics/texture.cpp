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

#include "graphics/texture.h"
#include "graphics/image.h"
#include "graphics/gl/opengl.h"
#include "core/asserts.h"

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_IMPORT(Core, Status);
VD_IMPORT(Core, Memory);

// ============================================================================================== //

Texture*
Texture::Load2D(
	const Symbol& name,    
    const vd::string& filename)
{
	ImageFormat format;
	ImageBuffer8u pixels;
	ImageInput& input = ImageInput::GetInstance();
	
	if(input.Open(filename, format) != Status::Code::Success)
	{
		vdLogGlobalWarning("Error: Failed to load texture from image file '%s'\n", filename.c_str());	
		return false;
	}
	
	if(input.Read(pixels) != Status::Code::Success)
	{
		vdLogGlobalWarning("Error: Failed to load texture from image file '%s'\n", filename.c_str());	
		return false;
	}

	input.Close();

	GLenum internal = GL_RGBA;
	GLenum layout = GL_RGBA;
	switch(format.Channels.Count)
	{
		case 1:
			internal = layout = GL_LUMINANCE;
			break;
		case 2:
			internal = layout = GL_LUMINANCE_ALPHA;
			break;
		case 3:
			internal = layout = GL_RGB;
			break;
		case 4:
			internal = layout = GL_RGBA;
			break;
		default:
			vdLogGlobalWarning("Invalid channel layout for creating texture from image file '%s'\n", filename.c_str());
			return NULL;
	};

    Texture::Properties properties;
    properties.Reset();
    properties.Name = name;
    properties.Handle = 0;
    properties.Width = format.Width;
    properties.Height = format.Height;
    properties.Target = GL_TEXTURE_2D;
    properties.Internal = internal;
    properties.Format = layout;
    properties.DataType = GL_UNSIGNED_BYTE;
    properties.WrapMode = GL_MIRRORED_REPEAT;
    properties.MagFilterMode = GL_LINEAR;
    properties.MinFilterMode = GL_LINEAR_MIPMAP_LINEAR;
    properties.UseMipMaps = true;

    Texture* texture = Texture::Create2D(properties, pixels.Data);

	if(texture == NULL)
		vdLogGlobalWarning("Failed to load 2D texture '%s' from file '%s'\n", AsString(name), filename.c_str());

	vdGlobalAssertMsg(texture != NULL, "Failed to load 2D texture!");
	return texture;
}

Texture*
Texture::LoadCubeMap(
	const Symbol& name,    
	vd::string filenames[6])
{
	ImageFormat format;
	ImageBuffer8u pixels;
	ImageInput& input = ImageInput::GetInstance();
	
	if(input.Open(filenames[0], format) != Status::Code::Success)
	{
		vdLogGlobalWarning("Error: Failed to load texture from image file '%s'\n", filenames[0].c_str());	
		return false;
	}
	
	if(input.Read(pixels) != Status::Code::Success)
	{
		vdLogGlobalWarning("Error: Failed to load texture from image file '%s'\n", filenames[0].c_str());	
		return false;
	}

	GLenum internal = GL_RGBA;
	GLenum layout = GL_RGBA;
	switch(format.Channels.Count)
	{
		case 1:
			internal = layout = GL_LUMINANCE;
			break;
		case 2:
			internal = layout = GL_LUMINANCE_ALPHA;
			break;
		case 3:
			internal = layout = GL_RGB;
			break;
		case 4:
			internal = layout = GL_RGBA;
			break;
		default:
			vdLogGlobalWarning("Invalid channel layout for creating texture from image file '%s'\n", filenames[0].c_str());
			return NULL;
	};

    Texture::Properties properties;
    properties.Reset();
    properties.Name = name;
    properties.Handle = 0;
    properties.Width = format.Width;
    properties.Height = format.Height;
    properties.Target = GL_TEXTURE_2D;
    properties.Internal = internal;
    properties.Format = layout;
    properties.DataType = GL_UNSIGNED_BYTE;
    properties.WrapMode = GL_MIRRORED_REPEAT;
    properties.MagFilterMode = GL_LINEAR;
    properties.MinFilterMode = GL_LINEAR_MIPMAP_LINEAR;
    properties.UseMipMaps = true;

    Texture* cubemap = Create2D(properties, NULL);
    if(!cubemap)
    {
		vdLogGlobalWarning("Error: Failed to allocate memory for cube map '%s'\n", filenames[0].c_str());	
        return cubemap;
    }
    const GLenum aeCubeFaces[] = 
    {
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

	vd::u8* rgba = (vd::u8*)pixels.Data;
    for(GLuint i = 0; i < 6; i++)
    {
        if(i > 0)
        {
			if(input.Open(filenames[i], format) != Status::Code::Success)
			{
				vdLogGlobalWarning("Error: Failed to load texture from image file '%s'\n", filenames[i].c_str());	
				return false;
			}
			
			if(input.Read(pixels) != Status::Code::Success)
			{
				vdLogGlobalWarning("Error: Failed to load texture from image file '%s'\n", filenames[i].c_str());	
				return false;
			}
			rgba = (vd::u8*)pixels.Data;
		}
    
    	glTexImage2D(aeCubeFaces[i], 0, properties.Internal, properties.Width, properties.Height, 
    								 0, properties.Format, properties.DataType, rgba); 
        Texture::CheckStatus(name, "Allocating cubemap face texture");
    }

	input.Close();

    return cubemap;
}

Texture*
Texture::Create2D(
	const Texture::Properties& properties,
    const void* ptr)
{
	Texture* texture = VD_NEW(Texture);
    bool success = texture->Create(properties, ptr);         
    if(!success)
    {
        vdLogGlobalWarning("Texture: Error Creating Texture!");
    	VD_DELETE(texture);
        return NULL;    
    }
    return texture;
}

Texture*
Texture::Create3D(
	const Texture::Properties& properties,
    const void* ptr)
{
	Texture* texture = VD_NEW(Texture);
    bool success = texture->Create(properties, ptr);         
    if(!success)
    {
        vdLogGlobalWarning("Texture: Error Creating Texture!");
    	VD_DELETE(texture);
        return NULL;    
    }
    return texture;
}

// ============================================================================================== //

Texture::Texture() :
    m_IsActive(false)
{
    m_Properties.Reset();
}

Texture::~Texture()
{
    Destroy();
}

vd::status
Texture::Destroy()
{
    if(m_Properties.Handle)
    {
        Unbind();
        glDeleteTextures( 1, &m_Properties.Handle);
    }
	Memory::MemSet(&m_Properties, 0, sizeof(Texture::Properties));
    return Status::Code::Success;
}

void
Texture::Render()
{
    int aiViewport[4];
    glGetIntegerv(GL_VIEWPORT, aiViewport);
    glViewport( 0, 0, m_Properties.Width, m_Properties.Height );
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glDisable(GL_BLEND);
    glDisable(GL_LIGHTING);
    
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

    if(m_Properties.Handle)
    {
        Bind(m_Properties.TextureUnit);
        glTexParameteri(m_Properties.Target, GL_TEXTURE_COMPARE_MODE_ARB, GL_NONE);
        
        glColor3f( 1.0f, 1.0f, 1.0f );
        glBegin( GL_QUADS );
        {
            glTexCoord2f( 0.0f, 0.0f );
            glVertex3f( -1.0f, -1.0f, 0.0f );
        
            glTexCoord2f( 0.0f, 1.0f );
            glVertex3f( -1.0f, +1.0f, 0.0f );
    
            glTexCoord2f( 1.0f, 1.0f );
            glVertex3f( +1.0f, +1.0f, 0.0f );
    
            glTexCoord2f( 1.0f, 0.0f );
            glVertex3f( +1.0f, -1.0f, 0.0f );
        }
        glEnd();

        Unbind();
    }
    
    glMatrixMode( GL_TEXTURE );
    glPopMatrix();

    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();    

    glViewport(aiViewport[0], aiViewport[1], aiViewport[2], aiViewport[3]);
    glMatrixMode( GL_MODELVIEW );
}

void
Texture::Bind(vd::i32 eTextureUnit)
{
    if(m_Properties.Handle == 0)
        return;
        
    m_Properties.TextureUnit = eTextureUnit < 1 ? GL_TEXTURE0 : eTextureUnit;
    Texture::CheckStatus(m_Properties.Name, "Preparing texture unit");
    
//    glActiveTextureARB(m_Properties.TextureUnit);
//    Texture::CheckStatus(m_Properties.Name, "Activating texture unit");

    glBindTexture(m_Properties.Target, m_Properties.Handle);
    Texture::CheckStatus(m_Properties.Name, "Binding texture");
    
    glEnable(m_Properties.Target);    
    Texture::CheckStatus(m_Properties.Name, "Enabling texture target");
    m_IsActive = true;
}

void
Texture::Update()
{
    if(!m_IsActive)
    {
//        glActiveTextureARB(m_Properties.TextureUnit);
//        Texture::CheckStatus(m_Properties.Name, "Activating texture unit");

        glBindTexture(m_Properties.Target, m_Properties.Handle);
        Texture::CheckStatus(m_Properties.Name, "Binding texture");
    }
    
    if(m_Properties.MinFilterMode)
        glTexParameteri(m_Properties.Target, GL_TEXTURE_MIN_FILTER, m_Properties.MinFilterMode);

    if(m_Properties.MagFilterMode)
        glTexParameteri(m_Properties.Target, GL_TEXTURE_MAG_FILTER, m_Properties.MagFilterMode);

    if(m_Properties.WrapMode)
    {
        if(m_Properties.Target == GL_TEXTURE_3D)
            glTexParameteri(m_Properties.Target, GL_TEXTURE_WRAP_R, m_Properties.WrapMode);
     
        glTexParameteri(m_Properties.Target, GL_TEXTURE_WRAP_S, m_Properties.WrapMode);
        glTexParameteri(m_Properties.Target, GL_TEXTURE_WRAP_T, m_Properties.WrapMode);
    }
    
    if(m_Properties.CompareMode)
        glTexParameteri(m_Properties.Target, GL_TEXTURE_COMPARE_MODE_ARB, m_Properties.CompareMode);
        
    if(m_Properties.CompareFunc)
        glTexParameteri(m_Properties.Target, GL_TEXTURE_COMPARE_FUNC_ARB, m_Properties.CompareFunc);
    
    if(m_Properties.EnvMode)
        glTexParameteri(m_Properties.Target, GL_TEXTURE_ENV_MODE, m_Properties.EnvMode);
        
    if(m_Properties.DepthMode)
        glTexParameteri(m_Properties.Target, GL_DEPTH_TEXTURE_MODE_ARB, m_Properties.DepthMode);
        
    Texture::CheckStatus(m_Properties.Name, "Setting texture parameters");

    if(!m_IsActive)
    {
        glBindTexture(m_Properties.Target, 0);
        Texture::CheckStatus(m_Properties.Name, "Unbinding texture");

        glDisable(m_Properties.Target);    
        Texture::CheckStatus(m_Properties.Name, "Disabling texture target");
    }
}

void
Texture::Unbind()
{
//    if(!m_IsActive)
//        return;

    Texture::CheckStatus(m_Properties.Name, "Preparing texture unit for disabling");
        
//    glActiveTextureARB(m_Properties.TextureUnit);
//    Texture::CheckStatus(m_Properties.Name, "Activating texture unit for disabling");

    glBindTexture(m_Properties.Target, 0);
    Texture::CheckStatus(m_Properties.Name, "Unbinding texture");

    glDisable(m_Properties.Target);    
    Texture::CheckStatus(m_Properties.Name, "Disabling texture target");

//    glActiveTextureARB(GL_TEXTURE0);
//    Texture::CheckStatus(m_Properties.Name, "Disabling texture unit");
    m_IsActive = false;
}

bool
Texture::Create(
	const Texture::Properties& properties,
    const void* ptr)
{	
	Destroy();
	SetProperties(properties);
	if(IsValid())
		return true;
		
    GLuint handle = properties.Handle;
    if(handle < 1)
		glGenTextures(1, &handle);

    if(handle < 1)
        return false;

	m_Properties.Handle = handle;
        
//    glActiveTextureARB(m_Properties.TextureUnit);
//    Texture::CheckStatus(m_Properties.Name, "Activating texture unit");

    glBindTexture(m_Properties.Target, handle);
    Texture::CheckStatus(m_Properties.Name, "Binding texture");
   
	if(!m_Properties.Internal) m_Properties.Internal = GL_RGBA;
	if(!m_Properties.Format) m_Properties.Format = m_Properties.Internal;
	if(!m_Properties.DataType) m_Properties.DataType = GL_UNSIGNED_BYTE;

    if(m_Properties.Internal == GL_DEPTH_COMPONENT)
    {   
        int iDepth = 8;
        glGetIntegerv(GL_DEPTH_BITS, &iDepth);      
        if(iDepth == 16)
            m_Properties.Internal = GL_DEPTH_COMPONENT16;
        else if(iDepth == 24)
            m_Properties.Internal = GL_DEPTH_COMPONENT24;
        else if(iDepth == 32)
            m_Properties.Internal = GL_DEPTH_COMPONENT32;
    }

    if(m_Properties.Target == GL_TEXTURE_3D)
    {
		if(m_Properties.UseMipMaps && ptr)
		{
			if(!m_Properties.MinFilterMode) m_Properties.MinFilterMode = GL_LINEAR_MIPMAP_NEAREST;
			if(!m_Properties.MagFilterMode) m_Properties.MagFilterMode = GL_LINEAR_MIPMAP_LINEAR;

			gluBuild3DMipmaps(m_Properties.Target, m_Properties.Internal, 
							  m_Properties.Width, m_Properties.Height, m_Properties.Depth, 
							  m_Properties.Format, m_Properties.DataType, ptr );
		}
    	else
    	{
			if(!m_Properties.MinFilterMode) m_Properties.MinFilterMode = GL_LINEAR;
			if(!m_Properties.MagFilterMode) m_Properties.MagFilterMode = GL_LINEAR;

			glTexImage3D(m_Properties.Target, 0, m_Properties.Internal, 
						 m_Properties.Width, m_Properties.Height, m_Properties.Depth, 0, 
						 m_Properties.Format, m_Properties.DataType, ptr);	
    				 
		}	
	}
    else if(m_Properties.Target == GL_TEXTURE_2D || GL_TEXTURE_RECTANGLE_ARB)
    {
		if(m_Properties.UseMipMaps && ptr)
		{
			if(!m_Properties.MinFilterMode) m_Properties.MinFilterMode = GL_LINEAR_MIPMAP_NEAREST;
			if(!m_Properties.MagFilterMode) m_Properties.MagFilterMode = GL_LINEAR_MIPMAP_LINEAR;

			gluBuild2DMipmaps(m_Properties.Target, m_Properties.Internal, 
							  m_Properties.Width, m_Properties.Height, 
							  m_Properties.Format, m_Properties.DataType, ptr );

		}
    	else
    	{
			if(!m_Properties.MinFilterMode) m_Properties.MinFilterMode = GL_LINEAR;
			if(!m_Properties.MagFilterMode) m_Properties.MagFilterMode = GL_LINEAR;

	    	glTexImage2D(m_Properties.Target, 0, m_Properties.Internal, 
	    				 m_Properties.Width, m_Properties.Height, 0, 
	    				 m_Properties.Format, m_Properties.DataType, ptr);					
		}
	}

	glTexParameteri(m_Properties.Target, GL_TEXTURE_MIN_FILTER, m_Properties.MinFilterMode);
	glTexParameteri(m_Properties.Target, GL_TEXTURE_MAG_FILTER, m_Properties.MagFilterMode);

    Texture::CheckStatus(m_Properties.Name, "Allocating texture");

    glBindTexture(m_Properties.Target, 0);
    Texture::CheckStatus(m_Properties.Name, "Unbinding texture");

//    glActiveTextureARB(GL_TEXTURE0);
//    Texture::CheckStatus(m_Properties.Name, "Disabling texture unit");

    return true;
}

bool 
Texture::IsValid() const
{
	if(m_Properties.Target == GL_TEXTURE_2D && 
	   m_Properties.Handle && m_Properties.Target && 
	   m_Properties.Width && m_Properties.Height)
	{
		return true;
	}	
	else if(m_Properties.Target == GL_TEXTURE_3D && 
	   m_Properties.Handle && m_Properties.Target && 
	   m_Properties.Width && m_Properties.Height && m_Properties.Depth)
	{
		return true;
	}	
	
	return false;
}

void
Texture::Capture()
{
    Bind();
    
    if(m_Properties.Target == GL_TEXTURE_3D)
    {
        // glCopyTexSubImage3D(m_Properties.Target,0,0,0,0,0,m_Properties.Width,m_Properties.Height);
    }
    else
    {
        glCopyTexSubImage2D(m_Properties.Target,0,0,0,0,0,m_Properties.Width,m_Properties.Height);
    }
    
    Unbind();
}

void
Texture::CheckStatus(
	const Symbol& name,
	const char *acMessage)
{
    GLenum eError = glGetError();
    if(eError == GL_FALSE)
        return;

    if(acMessage)
        vdLogGlobalWarning("Texture[%s]: OpenGL Error: %s\n", AsString(name), acMessage);
        
    switch(eError)
    {
    case(GL_INVALID_ENUM):
        vdLogGlobalWarning("Texture[%s]: OpenGL Error: Invalid Enumerate!\n", AsString(name));
        break;
    case(GL_INVALID_VALUE):
        vdLogGlobalWarning("Texture[%s]: OpenGL Error: Invalid Value!\n", AsString(name));
        break;
    case(GL_INVALID_OPERATION):
        vdLogGlobalWarning("Texture[%s]: OpenGL Error: Invalid Operation!\n", AsString(name));
        break;
    case(GL_STACK_OVERFLOW):
        vdLogGlobalWarning("Texture[%s]: OpenGL Error: Stack Overflow!\n", AsString(name));
        break;
    case(GL_STACK_UNDERFLOW):
        vdLogGlobalWarning("Texture[%s]: OpenGL Error: Stack Underflow!\n", AsString(name));
        break;
    case(GL_OUT_OF_MEMORY):
        vdLogGlobalWarning("Texture[%s]: OpenGL Error: Out of Memory!\n", AsString(name));
        break;
    case(GL_INVALID_FRAMEBUFFER_OPERATION_EXT):
        vdLogGlobalWarning("Texture[%s]: OpenGL Error: Invalid framebuffer operation!\n", AsString(name));
        break;
    default:
        vdLogGlobalWarning("Texture[%s]: Unknown OpenGL Error '%d'\n", AsString(name), (int)eError);
        break;
    }
}

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_END();

// ============================================================================================== //
