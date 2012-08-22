/*
 * Cogl
 *
 * An object oriented GL/GLES Abstraction/Utility Layer
 *
 * Copyright (C) 2010 Intel Corporation.
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
 * Authors:
 *  Robert Bragg <robert@linux.intel.com>
 *
 */

#if !defined(__COGL_H_INSIDE__) && !defined(CLUTTER_COMPILATION)
#error "Only <cogl/cogl.h> can be included directly."
#endif

#ifndef __COGL_CONTEXT_H__
#define __COGL_CONTEXT_H__

/* We forward declare the CoglContext type here to avoid some circular
 * dependency issues with the following headers.
 */
typedef struct _CoglContext CoglContext;

#include <cogl/cogl-defines.h>
#include <cogl/cogl-display.h>
#include <cogl/cogl-primitive.h>
#ifdef COGL_HAS_EGL_PLATFORM_ANDROID_SUPPORT
#include <android/native_window.h>
#endif

G_BEGIN_DECLS

/**
 * SECTION:cogl-context
 * @short_description: The top level application context.
 *
 * A #CoglContext is the top most sandbox of Cogl state for an
 * application or toolkit. Its main purpose is to act as a sandbox
 * for the memory management of state objects. Normally an application
 * will only create a single context since there is no way to share
 * resources between contexts.
 *
 * For those familiar with OpenGL or perhaps Cairo it should be
 * understood that unlike these APIs a Cogl context isn't a rendering
 * context as such. In other words Cogl doesn't aim to provide a state
 * machine style model for configuring rendering parameters. Most
 * rendering state in Cogl is directly associated with user managed
 * objects called pipelines and geometry is drawn with a specific
 * pipeline object to a framebuffer object and those 3 things fully
 * define the state for drawing. This is an important part of Cogl's
 * design since it helps you write orthogonal rendering components
 * that can all access the same GPU without having to worry about
 * what state other components have left you with.
 */

#ifdef COGL_ENABLE_EXPERIMENTAL_API

#define COGL_CONTEXT(OBJECT) ((CoglContext *)OBJECT)

/**
 * cogl_context_new:
 * @display: A #CoglDisplay pointer
 * @error: A GError return location.
 *
 * Creates a new #CoglContext which acts as an application sandbox
 * for any state objects that are allocated.
 *
 * Return value: (transfer full): A newly allocated #CoglContext
 * Since: 1.8
 * Stability: unstable
 */
CoglContext *
cogl_context_new (CoglDisplay *display,
                  GError **error);

/**
 * cogl_context_get_display:
 * @context: A #CoglContext pointer
 *
 * Retrieves the #CoglDisplay that is internally associated with the
 * given @context. This will return the same #CoglDisplay that was
 * passed to cogl_context_new() or if %NULL was passed to
 * cogl_context_new() then this function returns a pointer to the
 * display that was automatically setup internally.
 *
 * Return value: (transfer none): The #CoglDisplay associated with the
 *               given @context.
 * Since: 1.8
 * Stability: unstable
 */
CoglDisplay *
cogl_context_get_display (CoglContext *context);

#ifdef COGL_HAS_EGL_SUPPORT
/**
 * cogl_egl_context_get_egl_display:
 * @context: A #CoglContext pointer
 *
 * If you have done a runtime check to determine that Cogl is using
 * EGL internally then this API can be used to retrieve the EGLDisplay
 * handle that was setup internally. The result is undefined if Cogl
 * is not using EGL.
 *
 * Return value: The internally setup EGLDisplay handle.
 * Since: 1.8
 * Stability: unstable
 */
EGLDisplay
cogl_egl_context_get_egl_display (CoglContext *context);
#endif

#ifdef COGL_HAS_EGL_PLATFORM_ANDROID_SUPPORT
/**
 * cogl_android_set_native_window:
 * @window: A native Android window
 *
 * Allows Android applications to inform Cogl of the native window
 * that they have been given which Cogl can render too. On Android
 * this API must be used before creating a #CoglRenderer, #CoglDisplay
 * and #CoglContext.
 *
 * Since: 1.8
 * Stability: unstable
 */
void
cogl_android_set_native_window (ANativeWindow *window);
#endif

/**
 * cogl_is_context:
 * @object: An object or %NULL
 *
 * Gets whether the given object references an existing context object.
 *
 * Return value: %TRUE if the handle references a #CoglContext,
 *   %FALSE otherwise
 *
 * Since: 1.10
 * Stability: Unstable
 */
gboolean
cogl_is_context (void *object);

#endif /* COGL_ENABLE_EXPERIMENTAL_2_0_API */

/* XXX: not guarded by the EXPERIMENTAL_API defines to avoid
 * upsetting glib-mkenums, but this can still be considered implicitly
 * experimental since it's only useable with experimental API... */
/**
 * CoglFeatureID:
 * @COGL_FEATURE_ID_TEXTURE_NPOT_BASIC: The hardware supports non power
 *     of two textures, but you also need to check the
 *     %COGL_FEATURE_ID_TEXTURE_NPOT_MIPMAP and %COGL_FEATURE_ID_TEXTURE_NPOT_REPEAT
 *     features to know if the hardware supports npot texture mipmaps
 *     or repeat modes other than
 *     %COGL_RENDERER_PIPELINE_WRAP_MODE_CLAMP_TO_EDGE respectively.
 * @COGL_FEATURE_ID_TEXTURE_NPOT_MIPMAP: Mipmapping is supported in
 *     conjuntion with non power of two textures.
 * @COGL_FEATURE_ID_TEXTURE_NPOT_REPEAT: Repeat modes other than
 *     %COGL_RENDERER_PIPELINE_WRAP_MODE_CLAMP_TO_EDGE are supported by the
 *     hardware.
 * @COGL_FEATURE_ID_TEXTURE_NPOT: Non power of two textures are supported
 *    by the hardware. This is a equivalent to the
 *    %COGL_FEATURE_ID_TEXTURE_NPOT_BASIC, %COGL_FEATURE_ID_TEXTURE_NPOT_MIPMAP
 *    and %COGL_FEATURE_ID_TEXTURE_NPOT_REPEAT features combined.
 * @COGL_FEATURE_ID_TEXTURE_RECTANGLE: Support for rectangular
 *    textures with non-normalized texture coordinates.
 * @COGL_FEATURE_ID_TEXTURE_3D: 3D texture support
 * @COGL_FEATURE_ID_OFFSCREEN: Offscreen rendering support
 * @COGL_FEATURE_ID_OFFSCREEN_MULTISAMPLE: Multisample support for
 *    offscreen framebuffers
 * @COGL_FEATURE_ID_ONSCREEN_MULTIPLE: Multiple onscreen framebuffers
 *    supported.
 * @COGL_FEATURE_ID_GLSL: GLSL support
 * @COGL_FEATURE_ID_ARBFP: ARBFP support
 * @COGL_FEATURE_ID_UNSIGNED_INT_INDICES: Set if
 *     %COGL_RENDERER_INDICES_TYPE_UNSIGNED_INT is supported in
 *     cogl_indices_new().
 * @COGL_FEATURE_ID_DEPTH_RANGE: cogl_pipeline_set_depth_range() support
 * @COGL_FEATURE_ID_POINT_SPRITE: Whether
 *     cogl_pipeline_set_layer_point_sprite_coords_enabled() is supported.
 * @COGL_FEATURE_ID_MAP_BUFFER_FOR_READ: Whether cogl_buffer_map() is
 *     supported with CoglBufferAccess including read support.
 * @COGL_FEATURE_ID_MAP_BUFFER_FOR_WRITE: Whether cogl_buffer_map() is
 *     supported with CoglBufferAccess including write support.
 * @COGL_FEATURE_ID_MIRRORED_REPEAT: Whether
 *    %COGL_PIPELINE_WRAP_MODE_MIRRORED_REPEAT is supported.
 * @COGL_FEATURE_ID_SWAP_BUFFERS_EVENT:
 *     Available if the window system supports reporting an event
 *     for swap buffer completions.
 *
 * All the capabilities that can vary between different GPUs supported
 * by Cogl. Applications that depend on any of these features should explicitly
 * check for them using cogl_has_feature() or cogl_has_features().
 *
 * Since: 1.10
 */
typedef enum _CoglFeatureID
{
  COGL_FEATURE_ID_TEXTURE_NPOT_BASIC = 1,
  COGL_FEATURE_ID_TEXTURE_NPOT_MIPMAP,
  COGL_FEATURE_ID_TEXTURE_NPOT_REPEAT,
  COGL_FEATURE_ID_TEXTURE_NPOT,
  COGL_FEATURE_ID_TEXTURE_RECTANGLE,
  COGL_FEATURE_ID_TEXTURE_3D,
  COGL_FEATURE_ID_GLSL,
  COGL_FEATURE_ID_ARBFP,
  COGL_FEATURE_ID_OFFSCREEN,
  COGL_FEATURE_ID_OFFSCREEN_MULTISAMPLE,
  COGL_FEATURE_ID_ONSCREEN_MULTIPLE,
  COGL_FEATURE_ID_UNSIGNED_INT_INDICES,
  COGL_FEATURE_ID_DEPTH_RANGE,
  COGL_FEATURE_ID_POINT_SPRITE,
  COGL_FEATURE_ID_MAP_BUFFER_FOR_READ,
  COGL_FEATURE_ID_MAP_BUFFER_FOR_WRITE,
  COGL_FEATURE_ID_MIRRORED_REPEAT,
  COGL_FEATURE_ID_SWAP_BUFFERS_EVENT,

  /*< private > */
  _COGL_N_FEATURE_IDS
} CoglFeatureID;


#ifdef COGL_ENABLE_EXPERIMENTAL_API

/**
 * cogl_has_feature:
 * @context: A #CoglContext pointer
 * @feature: A #CoglFeatureID
 *
 * Checks if a given @feature is currently available
 *
 * Cogl does not aim to be a lowest common denominator API, it aims to
 * expose all the interesting features of GPUs to application which
 * means applications have some responsibility to explicitly check
 * that certain features are available before depending on them.
 *
 * Returns: %TRUE if the @feature is currently supported or %FALSE if
 * not.
 *
 * Since: 1.10
 * Stability: unstable
 */
gboolean
cogl_has_feature (CoglContext *context, CoglFeatureID feature);

/**
 * cogl_has_features:
 * @context: A #CoglContext pointer
 * @...: A 0 terminated list of CoglFeatureID<!-- -->s
 *
 * Checks if a list of features are all currently available.
 *
 * This checks all of the listed features using cogl_has_feature() and
 * returns %TRUE if all the features are available or %FALSE
 * otherwise.
 *
 * Return value: %TRUE if all the features are available, %FALSE
 * otherwise.
 *
 * Since: 1.10
 * Stability: unstable
 */
gboolean
cogl_has_features (CoglContext *context, ...);

/**
 * CoglFeatureCallback:
 * @feature: A single feature currently supported by Cogl
 * @user_data: A private pointer passed to cogl_foreach_feature().
 *
 * A callback used with cogl_foreach_feature() for enumerating all
 * context level features supported by Cogl.
 *
 * Since: 0.10
 * Stability: unstable
 */
typedef void (*CoglFeatureCallback) (CoglFeatureID feature, void *user_data);

/**
 * cogl_foreach_feature:
 * @context: A #CoglContext pointer
 * @callback: A #CoglFeatureCallback called for each supported feature
 * @user_data: Private data to pass to the callback
 *
 * Iterates through all the context level features currently supported
 * for a given @context and for each feature @callback is called.
 *
 * Since: 1.10
 * Stability: unstable
 */
void
cogl_foreach_feature (CoglContext *context,
                      CoglFeatureCallback callback,
                      void *user_data);

#endif /* COGL_ENABLE_EXPERIMENTAL_API */

G_END_DECLS

#endif /* __COGL_CONTEXT_H__ */

