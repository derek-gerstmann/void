/*
 * Cogl
 *
 * An object oriented GL/GLES Abstraction/Utility Layer
 *
 * Copyright (C) 2011 Intel Corporation.
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
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 *
 * Authors:
 *   Robert Bragg <robert@linux.intel.com>
 */

#if !defined(__COGL_H_INSIDE__) && !defined(CLUTTER_COMPILATION)
#error "Only <cogl/cogl.h> can be included directly."
#endif

#ifndef __COGL_TEXURE_2D_H
#define __COGL_TEXURE_2D_H

#include "cogl-context.h"

#include <glib.h>

G_BEGIN_DECLS

/**
 * SECTION:cogl-texture-2d
 * @short_description: Functions for creating and manipulating 2D textures
 *
 * These functions allow low-level 2D textures to be allocated. These
 * differ from sliced textures for example which may internally be
 * made up of multiple 2D textures, or atlas textures where Cogl must
 * internally modify user texture coordinates before they can be used
 * by the GPU.
 *
 * You should be aware that many GPUs only support power of two sizes
 * for #CoglTexture2D textures. You can check support for non power of
 * two textures by checking for the %COGL_FEATURE_ID_TEXTURE_NPOT feature
 * via cogl_has_feature().
 */

typedef struct _CoglTexture2D CoglTexture2D;
#define COGL_TEXTURE_2D(X) ((CoglTexture2D *)X)

#define cogl_is_texture_2d cogl_is_texture_2d_EXP
/**
 * cogl_is_texture_2d:
 * @object: A #CoglObject
 *
 * Gets whether the given object references an existing #CoglTexture2D
 * object.
 *
 * Return value: %TRUE if the object references a #CoglTexture2D,
 *   %FALSE otherwise
 */
gboolean
cogl_is_texture_2d (void *object);

#define cogl_texture_2d_new_with_size cogl_texture_2d_new_with_size_EXP
/**
 * cogl_texture_2d_new_with_size:
 * @ctx: A #CoglContext
 * @width: Width of the texture to allocate
 * @height: Height of the texture to allocate
 * @internal_format: The format of the texture
 * @error: A #GError for exceptions
 *
 * Allocates a low-level #CoglTexture2D texture that your GPU can
 * texture from directly. This is unlike sliced textures for example
 * which may be comprised of multiple internal textures, or atlas
 * textures where Cogl has to modify texture coordinates before they
 * may be used by the GPU.
 *
 * <note>Many GPUs only support power of two sizes for #CoglTexture2D
 * textures. You can check support for non power of two textures by
 * checking for the %COGL_FEATURE_ID_TEXTURE_NPOT feature via
 * cogl_has_feature().</note>
 *
 * Returns: A newly allocated #CoglTexture2D, or if the size is not
 *          supported (because it is too large or a non-power-of-two
 *          size that the hardware doesn't support) it will return
 *          %NULL and set @error.
 *
 * Since: 2.0
 */
CoglTexture2D *
cogl_texture_2d_new_with_size (CoglContext *ctx,
                               int width,
                               int height,
                               CoglPixelFormat internal_format,
                               GError **error);

#define cogl_texture_2d_new_from_data cogl_texture_2d_new_from_data_EXP
/**
 * cogl_texture_2d_new_from_data:
 * @ctx: A #CoglContext
 * @width: width of texture in pixels
 * @height: height of texture in pixels
 * @format: the #CoglPixelFormat the buffer is stored in in RAM
 * @internal_format: the #CoglPixelFormat that will be used for storing
 *    the buffer on the GPU. If COGL_PIXEL_FORMAT_ANY is given then a
 *    premultiplied format similar to the format of the source data will
 *    be used. The default blending equations of Cogl expect premultiplied
 *    color data; the main use of passing a non-premultiplied format here
 *    is if you have non-premultiplied source data and are going to adjust
 *    the blend mode (see cogl_material_set_blend()) or use the data for
 *    something other than straight blending.
 * @rowstride: the memory offset in bytes between the starts of
 *    scanlines in @data. A value of 0 will make Cogl automatically
 *    calculate @rowstride from @width and @format.
 * @data: pointer the memory region where the source buffer resides
 * @error: A #GError for exceptions
 *
 * Creates a new #CoglTexture2D texture based on data residing in memory.
 * These are unlike sliced textures for example which may be comprised
 * of multiple internal textures, or atlas textures where Cogl has to
 * modify texture coordinates before they may be used by the GPU.
 *
 * <note>Many GPUs only support power of two sizes for #CoglTexture2D
 * textures. You can check support for non power of two textures by
 * checking for the %COGL_FEATURE_ID_TEXTURE_NPOT feature via
 * cogl_has_feature().</note>
 *
 * Returns: A newly allocated #CoglTexture2D, or if the size is not
 *          supported (because it is too large or a non-power-of-two
 *          size that the hardware doesn't support) it will return
 *          %NULL and set @error.
 *
 * Since: 2.0
 */
CoglTexture2D *
cogl_texture_2d_new_from_data (CoglContext *ctx,
                               int width,
                               int height,
                               CoglPixelFormat format,
                               CoglPixelFormat internal_format,
                               int rowstride,
                               const guint8 *data,
                               GError **error);

#define cogl_texture_2d_new_from_foreign cogl_texture_2d_new_from_foreign_EXP
/**
 * cogl_texture_2d_new_from_foreign:
 * @ctx: A #CoglContext
 * @gl_handle: A GL handle for a GL_TEXTURE_2D texture object
 * @width: Width of the foreign GL texture
 * @height: Height of the foreign GL texture
 * @internal_format: The format of the texture
 * @error: A #GError for exceptions
 *
 * Wraps an existing GL_TEXTURE_2D texture object as a #CoglTexture2D.
 * This can be used for integrating Cogl with software using OpenGL
 * directly.
 *
 * <note>The results are undefined for passing an invalid @gl_handle
 * or if @width or @height don't have the correct texture
 * geometry.</note>
 *
 * Returns: A newly allocated #CoglTexture2D, or if Cogl could not
 *          validate the @gl_handle in some way (perhaps because of
 *          an unsupported format) it will return %NULL and set
 *          @error.
 *
 * Since: 2.0
 */
CoglTexture2D *
cogl_texture_2d_new_from_foreign (CoglContext *ctx,
                                  unsigned int gl_handle,
                                  int width,
                                  int height,
                                  CoglPixelFormat format,
                                  GError **error);

G_END_DECLS

#endif /* __COGL_TEXURE_2D_H */
