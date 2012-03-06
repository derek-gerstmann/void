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

#ifndef VD_INTERFACE_GWEN_INCLUDED
#define VD_INTERFACE_GWEN_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/object.h"

#include "interface/interface.h"
#include "interface/gwen/namespace.h"
#include "interface/gwen/types.h"
#include "interface/gwen/controls.h"

// ============================================================================================== //

VD_INTERFACE_GWEN_NAMESPACE_BEGIN();

// ============================================================================================== //

namespace Align = Gwen::Align;
namespace Hook = Gwen::Hook;
namespace Cursor = Gwen::CursorType;
namespace Utility = Gwen::Utility;
namespace Clipboard = Gwen::DragAndDrop;
namespace Tooltip = ToolTip;
namespace Platform = Gwen::Platform;
namespace Render = Gwen::Renderer;
namespace Skins = Gwen::Skin;

using namespace Gwen;

// ============================================================================================== //

template< class T >
T* vd_ui_cast( Widget* p )
{
    if ( !p ) return NULL;

    Widget* pReturn = p->DynamicCast( T::GetIdentifier() );
    if ( !pReturn ) return NULL;

    return static_cast<T*>(pReturn);
}

// ============================================================================================== //

VD_INTERFACE_GWEN_NAMESPACE_END();

// ============================================================================================== //

#define VD_UI_DYNAMIC GWEN_DYNAMIC
#define VD_UI_CONTROL GWEN_CONTROL
#define VD_UI_CONTROL_INLINE GWEN_CONTROL_INLINE
#define VD_UI_CONTROL_CONSTRUCTOR GWEN_CONTROL_CONTRUCTOR

#if 0
#define VD_UI_DYNAMIC( ThisName, BaseName )                                 \
                                                                            \
    static const char* GetIdentifier()                                      \
    {                                                                       \
        static const char* ident = #BaseName ":" #ThisName;                 \
        return ident;                                                       \
    };                                                                      \
                                                                            \
    virtual Widget* DynamicCast( const char* Variable )                     \
    {                                                                       \
        if ( GetIdentifier() == Variable )                                  \
            return this;                                                    \
                                                                            \
        return BaseClass::DynamicCast( Variable);                           \
    }

#define VD_UI_CONTROL( ThisName, BaseName )                                 \
    public:                                                                 \
        typedef BaseName BaseClass;                                         \
        typedef ThisName ThisClass;                                         \
        VD_UI_DYNAMIC( ThisName, BaseName )                                 \
        ThisName( Widget* pParent )

#define VD_UI_CONTROL_INLINE( ThisName, BaseName )                          \
    VD_UI_CONTROL( ThisName, BaseName ) : BaseClass( pParent )

#define VD_UI_CONTROL_CONSTRUCTOR( ThisName )                               \
    ThisName::ThisName( Widget* pParent ) : BaseClass( pParent )
#endif

// ============================================================================================== //

#endif	// VD_INTERFACE_GWEN_INCLUDED

// ============================================================================================== //
// end file

