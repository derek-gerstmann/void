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

#ifndef VD_SPATIAL_NAMESPACE

// ============================================================================================== //

#define VD_SPATIAL_NAMESPACE             Spatial
#define VD_SPATIAL_SCOPE                 VD_NAMESPACE_SCOPE::VD_SPATIAL_NAMESPACE

#define VD_SPATIAL_NAMESPACE_ENTER       VD_NAMESPACE_ENTER { namespace VD_SPATIAL_NAMESPACE 
#define VD_SPATIAL_NAMESPACE_EXIT        } VD_NAMESPACE_EXIT 

#define VD_SPATIAL_NAMESPACE_BEGIN(x)    VD_SPATIAL_NAMESPACE_ENTER { enum { }
#define VD_SPATIAL_NAMESPACE_END(x)      VD_SPATIAL_NAMESPACE_EXIT } enum { }

#define VD_USING_SPATIAL_NAMESPACE       using namespace VD_SPATIAL_SCOPE;
#define VD_USING_SPATIAL                 using VD_SPATIAL_SCOPE

// ============================================================================================== //

#endif // VD_SPATIAL_NAMESPACE

// ============================================================================================== //

