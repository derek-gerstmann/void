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

#ifndef VD_INTERFACE_GLUT_NAMESPACE

// ============================================================================================== //

#define VD_INTERFACE_GLUT_NAMESPACE             Glut
#define VD_INTERFACE_GLUT_SCOPE                 VD_INTERFACE_SCOPE::VD_INTERFACE_GLUT_NAMESPACE

#define VD_INTERFACE_GLUT_NAMESPACE_ENTER       VD_INTERFACE_NAMESPACE_ENTER { namespace VD_INTERFACE_GLUT_NAMESPACE 
#define VD_INTERFACE_GLUT_NAMESPACE_EXIT        } VD_INTERFACE_NAMESPACE_EXIT 

#define VD_INTERFACE_GLUT_NAMESPACE_BEGIN(x)    VD_INTERFACE_GLUT_NAMESPACE_ENTER { enum { }
#define VD_INTERFACE_GLUT_NAMESPACE_END(x)      VD_INTERFACE_GLUT_NAMESPACE_EXIT } enum { }

#define VD_USING_INTERFACE_GLUT_NAMESPACE       using namespace VD_INTERFACE_GLUT_SCOPE;
#define VD_USING_INTERFACE_GLUT                 using VD_INTERFACE_GLUT_SCOPE

// ============================================================================================== //

#endif // VD_INTERFACE_GLUT_NAMESPACE

// ============================================================================================== //

