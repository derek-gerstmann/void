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

#ifndef VD_GRAPHICS_ENGINE_INCLUDED
#define VD_GRAPHICS_ENGINE_INCLUDED

// ============================================================================================== //

#include "graphics/graphics.h"
#include "graphics/geometry.h"
#include "graphics/surface.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/image.h"

// ============================================================================================== //

#if defined(VD_USE_OPENGL)
    #include "graphics/gl/opengl.h"
#if defined(VD_USE_GLUT)
    #include "graphics/gl/glut.h"
#endif
#endif

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_BEGIN();

// ============================================================================================== //

template<typename PlatformType>
class Engine
{
	Texture* CreateTexture(const TextureProperties& properties);
	Buffer* CreateBuffer(const BufferProperties& properties);
	Geometry* CreateGeometry(const GeometryProperties& properties);
	Shader* CreateShader(const ShaderProperties& properties);
	Framebuffer* CreateFramebuffer(const FramebufferProperties& properties);
};

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_END();

// ============================================================================================== //

#endif	// VD_GRAPHICS_ENGINE_INCLUDED