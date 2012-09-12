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

#ifndef VD_COMMON_MACROS_INCLUDED
#define VD_COMMON_MACROS_INCLUDED

// ============================================================================================== //
// Pragmas and messages
// ============================================================================================== //

#define VD_PRAGMA(x)                     _Pragma (#x)
#define VD_TODO(x)                       VD_PRAGMA(message ("TODO - " #x))
#define VD_INFO(x)                       VD_PRAGMA(message ("INFO - " #x))

// ============================================================================================== //
// Stringification macros
// ============================================================================================== //

#define VD_STRINGIFY_HELP(x)             #x
#define VD_STRINGIFY(x)                  VD_STRINGIFY_HELP(x)
#define VD_STR_JOIN(x, y)                VD_STR_JOIN_WRAP(x, y)
#define VD_STR_JOIN_WRAP(x, y)           VD_STR_JOIN_HELP(x, y)
#define VD_STR_JOIN_HELP(x, y)           x##y

// ============================================================================================== //
/// Type Registry Macros
// ============================================================================================== //

// Macro to generate a unique variable name based on the line number
#define VD_ANONYMOUS       							VD_ANONYMOUS_JOIN1(anonymous,__LINE__)
#define VD_ANONYMOUS_JOIN1(N1,N2)    				VD_ANONYMOUS_JOIN2(N1,N2)
#define VD_ANONYMOUS_JOIN2(N1,N2)    				N1##N2

// Macros which specify whether a class is Concrete (creatable) or Abstract (non-creatable).
#define VD_CONCRETE_CLASS
#define VD_ABSTRACT_CLASS , 0

// Macros to register a product and its base classes (if any)
#define VD_REGISTER_TYPE(AC,X)                      const bool VD_ANONYMOUS = Core::Factory::Register<X>( #X AC )
#define VD_REGISTER_TYPE_BASE1(AC,X,B1)             VD_REGISTER_TYPE(AC,X)                   && Core::Factory::RegisterBase<X, B1>()
#define VD_REGISTER_TYPE_BASE2(AC,X,B1,B2)          VD_REGISTER_TYPE_BASE1(AC,X,B1)          && Core::Factory::RegisterBase<X, B2>()
#define VD_REGISTER_TYPE_BASE3(AC,X,B1,B2,B3)       VD_REGISTER_TYPE_BASE2(AC,X,B1,B2)       && Core::Factory::RegisterBase<X, B3>()
#define VD_REGISTER_TYPE_BASE4(AC,X,B1,B2,B3,B4)    VD_REGISTER_TYPE_BASE3(AC,X,B1,B2,B3)    && Core::Factory::RegisterBase<X, B4>()
#define VD_REGISTER_TYPE_BASE5(AC,X,B1,B2,B3,B4,B5) VD_REGISTER_TYPE_BASE4(AC,X,B1,B2,B3,B4) && Core::Factory::RegisterBase<X, B5>()

// Note: For any class X, with [n] number of base classes, the series goes like this:
// #define VD_REGISTER_TYPE_BASE[n](AC,X,B1,B2,...,B[n]) VD_REGISTER_TYPE_BASE[n-1](AC,X,B1,B2,...,B[n-1]) && Factory::RegisterBase<X, B[n]>()

// ============================================================================================== //
/// Memory Allocation & Tracking Macros
// ============================================================================================== //

#define VD_TRACK_PTR(PTR,SZ)  	    	 Core::Memory::Reserve((PTR), (SZ), VD_FILE_ID, VD_FUNC_ID, VD_LINE_ID)
#define VD_IGNORE_PTR(PTR) 	  	         Core::Memory::Release((PTR))

#define VD_NEW(T,...)         	         Core::Memory::Reserve(new T(__VA_ARGS__), sizeof(T), VD_FILE_ID, VD_FUNC_ID, VD_LINE_ID)
#define VD_NEW_ARRAY(T,N,...) 	         Core::Memory::Reserve(new T[(N)](__VA_ARGS__), sizeof(T) * (N), VD_FILE_ID, VD_FUNC_ID, VD_LINE_ID)
#define VD_DELETE(x)          	         do { Core::Memory::Release((x)); delete (x); } while (0)
#define VD_DELETE_ARRAY(x)    	         do { Core::Memory::Release((x)); delete[] (x); } while (0)
#define VD_SAFE_DELETE(x) 		         do { if ((x) != NULL) VD_DELETE(x); (x) = NULL; } while (0)
#define VD_SAFE_DELETE_ARRAY(x)          do { if ((x) != NULL) VD_DELETE_ARRAY(x); (x) = NULL; } while (0)

// ============================================================================================== //

#define VD_PTR_ALIGN_128(ptr)       ((((uintptr_t)(ptr)) + 127) & ~127)
#define VD_PTR_ALIGN_64(ptr)        ((((uintptr_t)(ptr)) + 63 ) & ~63 )
#define VD_PTR_ALIGN_32(ptr)        ((((uintptr_t)(ptr)) + 31 ) & ~31 )
#define VD_PTR_ALIGN_16(ptr)        ((((uintptr_t)(ptr)) + 15 ) & ~15 )
#define VD_PTR_ALIGN_8(ptr)         ((((uintptr_t)(ptr)) + 7  ) & ~7  )
#define VD_PTR_ALIGN_4(ptr)         ((((uintptr_t)(ptr)) + 3  ) & ~3  )
#define VD_PTR_ALIGN_2(ptr)         ((((uintptr_t)(ptr)) + 1  ) & ~1  )
#define VD_PTR_ALIGN_N(ptr, n)      ((((uintptr_t)(ptr)) + ((n)-1)) & ~((n)-1))  // 'n' needs to be a power of 2

#ifdef _MSC_VER
#define VD_PRE_STRUCT_ALIGN( bits ) __declspec( align( bits ) )
#define VD_POST_STRUCT_ALIGN( bits )
#else
#define VD_PRE_STRUCT_ALIGN( bits )
#define VD_POST_STRUCT_ALIGN( bits ) __attribute__ (( aligned ( bits ) ))
#endif

// ============================================================================================== //
/// Declaration Macros
// ============================================================================================== //

/// Hint to ignore an unused variable 
#define	VD_IGNORE_UNUSED(VariableName)				\
	(void)VariableName

/// Disable all constructors in the declaration of the given struct / class
#define VD_DISABLE_ALL_CONSTRUCTORS(TypeName)   	\
private:											\
    TypeName();					                  	\
    TypeName(const TypeName&);                  	\
    void operator=(const TypeName&)

/// Disable the copy constructor and copy operator in the declaration of the given struct / class
#define VD_DISABLE_COPY_CONSTRUCTORS(TypeName)   	\
private:											\
    TypeName(const TypeName&);                  	\
    void operator=(const TypeName&)

/// Disable the copy operator in the declaration of the given struct / class
#define VD_DISABLE_COPY_OPERATOR(TypeName) 			\
private:											\
    void operator=(const TypeName&)

// ============================================================================================== //

#endif	// VD_COMMON_MACROS_INCLUDED

// ============================================================================================== //
// END FILE


