// ============================================================================================== //
//
// License:     The Lesser GNU Public License (LGPL) v3.0.
// 
// Author(s):   Derek Gerstmann. The University of Western Australia (UWA). 
//              As well as the shoulders of many giants...
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

#ifndef VD_COCOA_CALLBACKS_INCLUDED
#define VD_COCOA_CALLBACKS_INCLUDED

// ============================================================================================== //

#include "interface/interface.h"
#include "interface/cocoa/window.h"

// ============================================================================================== //

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================================== //

VD_API_C Window::Config
vdCocoaGetWindowConfig(void);

VD_API_C Keyboard::KeyCode::Value
vdCocoaTranslateAsciiKey(int code);

VD_API_C Keyboard::KeyCode::Value
vdCocoaTranslateSpecialKey(int code);

VD_API_C void
vdCocoaShutdown();

VD_API_C void 
vdCocoaMotionCallback(int x, int y);

VD_API_C void 
vdCocoaMouseCallback(int button, int state, int x, int y);

VD_API_C void 
vdCocoaDisplayCallback(void);

VD_API_C void 
vdCocoaReshapeCallback(int width, int height);

VD_API_C void 
vdCocoaKeyboardCallback(unsigned char key, int x, int y);

VD_API_C void 
vdCocoaKeyboardUpCallback(unsigned char key, int x, int y);

VD_API_C void 
vdCocoaPassiveMotionCallback(int x, int y);

VD_API_C void 
vdCocoaEntryCallback(int state);

VD_API_C void
vdCocoaSpecialKeyDownCallback(int key, int x, int y);

VD_API_C void
vdCocoaSpecialKeyUpCallback(int key, int x, int y);

VD_API_C void 
vdCocoaVisibilityCallback(int state);

VD_API_C void
vdCocoaIdleCallback(void);

// ============================================================================================== //

#ifdef __cplusplus
}
#endif

// ============================================================================================== //

#endif // VD_COCOA_CALLBACKS_INCLUDED

// ============================================================================================== //

