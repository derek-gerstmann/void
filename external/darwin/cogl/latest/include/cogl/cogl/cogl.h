/*
 * Cogl
 *
 * An object oriented GL/GLES Abstraction/Utility Layer
 *
 * Copyright (C) 2008,2009 Intel Corporation.
 *
 * This library is free software; you can redistribute it and/or

 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */

#ifndef __COGL_H__
#define __COGL_H__

#include <glib.h>

#define __COGL_H_INSIDE__

#ifdef COGL_ENABLE_EXPERIMENTAL_2_0_API
#ifndef COGL_ENABLE_EXPERIMENTAL_API
#define COGL_ENABLE_EXPERIMENTAL_API
#endif
#endif

/* We currently keep gtype integration delimited in case we eventually
 * want to split it out into a separate utility library when Cogl
 * becomes a standalone project. (like cairo-gobject.so)
 */
#define _COGL_SUPPORTS_GTYPE_INTEGRATION

/*
 * API common to the 1.x and 2.0 api...
 */

#include <cogl/cogl-defines.h>

#include <cogl/cogl-object.h>
#include <cogl/cogl1-context.h>
#include <cogl/cogl-bitmap.h>
#include <cogl/cogl-color.h>
#include <cogl/cogl-matrix.h>
#include <cogl/cogl-offscreen.h>
#include <cogl/cogl-primitives.h>
#include <cogl/cogl-texture.h>
#include <cogl/cogl-types.h>

/*
 * 1.x only api...
 */
#ifndef COGL_ENABLE_EXPERIMENTAL_2_0_API
#include <cogl/cogl-path.h>
#include <cogl/cogl-clip-state.h>
#include <cogl/cogl-vertex-buffer.h>
#include <cogl/cogl-enum-types.h>
#include <cogl/cogl-fixed.h>
#include <cogl/cogl-material-compat.h>
#include <cogl/cogl-shader.h>
#endif

/*
 * 2.0 api that's compatible with the 1.x api...
 */
#if defined (COGL_ENABLE_EXPERIMENTAL_API)
#include <cogl/cogl-swap-chain.h>
#include <cogl/cogl-renderer.h>
#include <cogl/cogl-display.h>
#include <cogl/cogl-context.h>
#include <cogl/cogl-buffer.h>
#include <cogl/cogl-pixel-buffer.h>
#include <cogl/cogl-vector.h>
#include <cogl/cogl-euler.h>
#include <cogl/cogl-quaternion.h>
#include <cogl/cogl-texture-2d.h>
#include <cogl/cogl-texture-rectangle.h>
#include <cogl/cogl-texture-3d.h>
#include <cogl/cogl-texture-2d-sliced.h>
#include <cogl/cogl-sub-texture.h>
#include <cogl/cogl-meta-texture.h>
#include <cogl/cogl-index-buffer.h>
#include <cogl/cogl-attribute-buffer.h>
#include <cogl/cogl-indices.h>
#include <cogl/cogl-attribute.h>
#include <cogl/cogl-primitive.h>
#include <cogl/cogl-depth-state.h>
#include <cogl/cogl-pipeline.h>
#include <cogl/cogl-pipeline-state.h>
#include <cogl/cogl-pipeline-layer-state.h>
#include <cogl/cogl-snippet.h>
#include <cogl/cogl-framebuffer.h>
#include <cogl/cogl-onscreen.h>
#include <cogl/cogl-poll.h>
#if defined (COGL_HAS_EGL_PLATFORM_KMS_SUPPORT)
#include <cogl/cogl-kms-renderer.h>
#endif
#if COGL_HAS_WIN32_SUPPORT
#include <cogl/cogl-win32-renderer.h>
#endif
#ifdef COGL_HAS_GLIB_SUPPORT
#include <cogl/cogl-glib-source.h>
#endif
/* XXX: This will definitly go away once all the Clutter winsys
 * code has been migrated down into Cogl! */
#include <cogl/cogl-clutter.h>
#endif

/*
 * 2.0 only api...
 */
#ifdef COGL_ENABLE_EXPERIMENTAL_2_0_API
#include <cogl/cogl2-path.h>
/* This header will be removed in Cogl 1.12 */
#include <cogl/cogl2-compatibility.h>
#endif

/*
 * API deprecations
 */
#include <cogl/cogl-deprecated.h>

/**
 * SECTION:cogl
 * @short_description: General purpose API
 *
 * General utility functions for COGL.
 */

#undef __COGL_H_INSIDE__

#endif /* __COGL_H__ */
