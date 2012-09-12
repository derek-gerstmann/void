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

#ifndef VD_GRAPHICS_TYPES_INCLUDED
#define VD_GRAPHICS_TYPES_INCLUDED

// ============================================================================================== //

#include "core/object.h"
#include "constants/constants.h"

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_USING(Core, Object);

// ============================================================================================== //

class Context;
class CommandQueue;
class Buffer;
class Texture;
class Framebuffer;
class Geometry;
class Program;
class Shader;    

// ============================================================================================== //
// Enums
// ============================================================================================== //

VD_DECLARE_ENUM(ScalarTypeId,
    Void,
    I8,
    U8,
    I16,
    U16,
    I32,
    U32,
    I64,
    U64,
    F16,
    F32,
    F64,
    Pointer,
    Address,
    ByteSize);

// ============================================================================================== //
// Data Structures
// ============================================================================================== //

template <typename T>
struct TypeTraits 
{  
    static vd::cstr GetName() 
    {
        return ScalarTypeId::ToString(ScalarTypeId::Invalid);
    }

    static ScalarTypeId::Value GetTypeId() 
    {
        return ScalarTypeId::Invalid;
    }

    static int GetByteSize() 
    {
        return (int)(sizeof(T));
    }

    static int GetBitSize()
    { 
        return (int)(sizeof(T) * CHAR_BIT);
    }

    static int GetDimensions()
    {
        return 1;
    }
};

// ============================================================================================== //
// Template specializations
// ============================================================================================== //

template <> VD_FORCE_INLINE vd::cstr TypeTraits<void>::GetName()                 { return ScalarTypeId::ToString(ScalarTypeId::Void);       }
template <> VD_FORCE_INLINE vd::cstr TypeTraits<vd::i8>::GetName()               { return ScalarTypeId::ToString(ScalarTypeId::I8);         }
template <> VD_FORCE_INLINE vd::cstr TypeTraits<vd::i16>::GetName()              { return ScalarTypeId::ToString(ScalarTypeId::I16);        }
template <> VD_FORCE_INLINE vd::cstr TypeTraits<vd::i32>::GetName()              { return ScalarTypeId::ToString(ScalarTypeId::I32);        }
template <> VD_FORCE_INLINE vd::cstr TypeTraits<vd::i64>::GetName()              { return ScalarTypeId::ToString(ScalarTypeId::I64);        }
template <> VD_FORCE_INLINE vd::cstr TypeTraits<vd::u8>::GetName()               { return ScalarTypeId::ToString(ScalarTypeId::U8);         }
template <> VD_FORCE_INLINE vd::cstr TypeTraits<vd::u16>::GetName()              { return ScalarTypeId::ToString(ScalarTypeId::U16);        }
template <> VD_FORCE_INLINE vd::cstr TypeTraits<vd::u32>::GetName()              { return ScalarTypeId::ToString(ScalarTypeId::U32);        }
template <> VD_FORCE_INLINE vd::cstr TypeTraits<vd::u64>::GetName()              { return ScalarTypeId::ToString(ScalarTypeId::U64);        }
template <> VD_FORCE_INLINE vd::cstr TypeTraits<vd::f16>::GetName()              { return ScalarTypeId::ToString(ScalarTypeId::F16);        }
template <> VD_FORCE_INLINE vd::cstr TypeTraits<vd::f32>::GetName()              { return ScalarTypeId::ToString(ScalarTypeId::F32);        }
template <> VD_FORCE_INLINE vd::cstr TypeTraits<vd::f64>::GetName()              { return ScalarTypeId::ToString(ScalarTypeId::F64);        }
template <> VD_FORCE_INLINE vd::cstr TypeTraits<vd::ptr>::GetName()              { return ScalarTypeId::ToString(ScalarTypeId::Pointer);    }
template <> VD_FORCE_INLINE vd::cstr TypeTraits<size_t>::GetName()               { return ScalarTypeId::ToString(ScalarTypeId::ByteSize);   }

template <> VD_FORCE_INLINE ScalarTypeId::Value TypeTraits<void>::GetTypeId()          { return ScalarTypeId::Void;     }
template <> VD_FORCE_INLINE ScalarTypeId::Value TypeTraits<vd::i8>::GetTypeId()        { return ScalarTypeId::I8;       }
template <> VD_FORCE_INLINE ScalarTypeId::Value TypeTraits<vd::i16>::GetTypeId()       { return ScalarTypeId::I16;      }
template <> VD_FORCE_INLINE ScalarTypeId::Value TypeTraits<vd::i32>::GetTypeId()       { return ScalarTypeId::I32;      }
template <> VD_FORCE_INLINE ScalarTypeId::Value TypeTraits<vd::i64>::GetTypeId()       { return ScalarTypeId::I64;      }
template <> VD_FORCE_INLINE ScalarTypeId::Value TypeTraits<vd::u8>::GetTypeId()        { return ScalarTypeId::U8;       }
template <> VD_FORCE_INLINE ScalarTypeId::Value TypeTraits<vd::u16>::GetTypeId()       { return ScalarTypeId::U16;      }
template <> VD_FORCE_INLINE ScalarTypeId::Value TypeTraits<vd::u32>::GetTypeId()       { return ScalarTypeId::U32;      }
template <> VD_FORCE_INLINE ScalarTypeId::Value TypeTraits<vd::u64>::GetTypeId()       { return ScalarTypeId::U64;      }
template <> VD_FORCE_INLINE ScalarTypeId::Value TypeTraits<vd::f16>::GetTypeId()       { return ScalarTypeId::F16;      }
template <> VD_FORCE_INLINE ScalarTypeId::Value TypeTraits<vd::f32>::GetTypeId()       { return ScalarTypeId::F32;      }
template <> VD_FORCE_INLINE ScalarTypeId::Value TypeTraits<vd::f64>::GetTypeId()       { return ScalarTypeId::F64;      }
template <> VD_FORCE_INLINE ScalarTypeId::Value TypeTraits<vd::ptr>::GetTypeId()       { return ScalarTypeId::Pointer;  }
template <> VD_FORCE_INLINE ScalarTypeId::Value TypeTraits<size_t>::GetTypeId()        { return ScalarTypeId::ByteSize; }

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_GRAPHICS_TYPES_INCLUDED