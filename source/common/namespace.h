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

#ifndef VD_FRAMEWORK_NAMESPACE
#define VD_FRAMEWORK_NAMESPACE			Void
#define VD_FRAMEWORK_SCOPE				VD_FRAMEWORK_NAMESPACE::VD_VERSION_NAMESPACE

// ============================================================================================== //

#ifndef VD_NAMESPACE
#define VD_NAMESPACE 					vd
#define VD_NAMESPACE_SCOPE				VD_FRAMEWORK_SCOPE::VD_NAMESPACE
#define VD_NAMESPACE_ENTER 				namespace VD_FRAMEWORK_NAMESPACE { namespace VD_VERSION_NAMESPACE { namespace VD_NAMESPACE
#define VD_NAMESPACE_EXIT 				} using namespace VD_VERSION_NAMESPACE; } 

#define VD_NAMESPACE_BEGIN(x)			VD_NAMESPACE_ENTER { struct EnterVoidNamespace
#define VD_NAMESPACE_END(x)				VD_NAMESPACE_EXIT } struct ExitVoidNamespace

#define VD_USING_NAMESPACE(x)			using namespace VD_FRAMEWORK_SCOPE
#define VD_USING(M,C)                   using VD_NAMESPACE_SCOPE::M::C
#define VD_IMPORT(M,C)                  using VD_NAMESPACE_SCOPE::M::C
#define VD_IMPORT_MODULE(M)             using namespace VD_NAMESPACE_SCOPE
#define VD_IMPORT_NAMESPACE(M,C)        using namespace VD_NAMESPACE_SCOPE::M::C
#endif

// ============================================================================================== //

#endif  // VD_FRAMEWORK_NAMESPACE

// ============================================================================================== //
// END FILE
