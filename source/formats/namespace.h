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

#ifndef VD_FORMATS_NAMESPACE

// ============================================================================================== //

#define VD_FORMATS_NAMESPACE             Formats
#define VD_FORMATS_SCOPE                 VD_NAMESPACE_SCOPE::VD_FORMATS_NAMESPACE

#define VD_FORMATS_NAMESPACE_ENTER       VD_NAMESPACE_ENTER { namespace VD_FORMATS_NAMESPACE 
#define VD_FORMATS_NAMESPACE_EXIT        } VD_NAMESPACE_EXIT 

#define VD_FORMATS_NAMESPACE_BEGIN(x)    VD_FORMATS_NAMESPACE_ENTER { enum { }
#define VD_FORMATS_NAMESPACE_END(x)      VD_FORMATS_NAMESPACE_EXIT } enum { }

#define VD_USING_FORMATS_NAMESPACE       using namespace VD_FORMATS_SCOPE;
#define VD_USING_FORMATS                 using VD_FORMATS_SCOPE

// ============================================================================================== //

#endif // VD_FORMATS_NAMESPACE

// ============================================================================================== //

