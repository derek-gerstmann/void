/*
 * Cogl
 *
 * An object oriented GL/GLES Abstraction/Utility Layer
 *
 * Copyright (C) 2007,2008,2009,2010 Intel Corporation.
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

#if !defined(__COGL_H_INSIDE__) && !defined(CLUTTER_COMPILATION)
#error "Only <cogl/cogl.h> can be included directly."
#endif

#ifndef __COGL_DEFINES_H__
#define __COGL_DEFINES_H__

#include <glib.h>

#include <OpenGL/gl.h>

G_BEGIN_DECLS


#define COGL_SYSDEF_POLLIN 1
#define COGL_SYSDEF_POLLPRI 2
#define COGL_SYSDEF_POLLOUT 4
#define COGL_SYSDEF_POLLERR 8
#define COGL_SYSDEF_POLLHUP 16
#define COGL_SYSDEF_POLLNVAL 32

#define COGL_HAS_GLIB_SUPPORT 1
#define COGL_HAS_GL 1
#define CLUTTER_COGL_HAS_GL 1

#ifdef COGL_HAS_EGL_SUPPORT

#define NativeDisplayType EGLNativeDisplayType
#define NativeWindowType EGLNativeWindowType
#endif

#ifndef GL_OES_EGL_image
#define GLeglImageOES void *
#endif
G_END_DECLS

#endif
