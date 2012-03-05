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

#ifndef VD_FRAMEWORK_TRAITS_INCLUDED
#define VD_FRAMEWORK_TRAITS_INCLUDED

// ============================================================================================== //

#include "vd.h"
#include "framework/enum.h"

// ============================================================================================== //

VD_NAMESPACE_BEGIN();

// ============================================================================================== //
// Enums
// ============================================================================================== //

VD_DECLARE_ENUM(TypeId,
    Void,
    Char,
    UnsignedChar,
    Short,
    UnsignedShort,
    Integer,
    UnsignedInteger,
    Long,
    UnsignedLong,
    LongLong,
    UnsignedLongLong,
    Half,
    Float,
    Double,
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
        return TypeId::ToString(TypeId::Invalid);
    }

    static TypeId::Value GetTypeId() 
    {
        return TypeId::Invalid;
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

template <> inline vd::cstr TypeTraits<void>::GetName()                 { return TypeId::ToString(TypeId::Void);            }
template <> inline vd::cstr TypeTraits<vd::i8>::GetName()               { return TypeId::ToString(TypeId::Char);            }
template <> inline vd::cstr TypeTraits<vd::i16>::GetName()              { return TypeId::ToString(TypeId::Short);           }
template <> inline vd::cstr TypeTraits<vd::i32>::GetName()              { return TypeId::ToString(TypeId::Integer);         }
template <> inline vd::cstr TypeTraits<vd::i64>::GetName()              { return TypeId::ToString(TypeId::Long);            }
template <> inline vd::cstr TypeTraits<vd::u8>::GetName()               { return TypeId::ToString(TypeId::UnsignedChar);    }
template <> inline vd::cstr TypeTraits<vd::u16>::GetName()              { return TypeId::ToString(TypeId::UnsignedShort);   }
template <> inline vd::cstr TypeTraits<vd::u32>::GetName()              { return TypeId::ToString(TypeId::UnsignedInteger); }
template <> inline vd::cstr TypeTraits<vd::u64>::GetName()              { return TypeId::ToString(TypeId::UnsignedLong);    }
template <> inline vd::cstr TypeTraits<vd::f16>::GetName()              { return TypeId::ToString(TypeId::Half);            }
template <> inline vd::cstr TypeTraits<vd::f32>::GetName()              { return TypeId::ToString(TypeId::Float);           }
template <> inline vd::cstr TypeTraits<vd::f64>::GetName()              { return TypeId::ToString(TypeId::Double);          }
template <> inline vd::cstr TypeTraits<vd::ptr>::GetName()              { return TypeId::ToString(TypeId::Pointer);         }
template <> inline vd::cstr TypeTraits<vd::address>::GetName()          { return TypeId::ToString(TypeId::Address);         }
template <> inline vd::cstr TypeTraits<vd::bytesize>::GetName()         { return TypeId::ToString(TypeId::ByteSize);        }

template <> inline TypeId::Value TypeTraits<void>::GetTypeId()          { return TypeId::Void;              }
template <> inline TypeId::Value TypeTraits<vd::i8>::GetTypeId()        { return TypeId::Char;              }
template <> inline TypeId::Value TypeTraits<vd::i16>::GetTypeId()       { return TypeId::Short;             }
template <> inline TypeId::Value TypeTraits<vd::i32>::GetTypeId()       { return TypeId::Integer;           }
template <> inline TypeId::Value TypeTraits<vd::i64>::GetTypeId()       { return TypeId::Long;              }
template <> inline TypeId::Value TypeTraits<vd::u8>::GetTypeId()        { return TypeId::UnsignedChar;      }
template <> inline TypeId::Value TypeTraits<vd::u16>::GetTypeId()       { return TypeId::UnsignedShort;     }
template <> inline TypeId::Value TypeTraits<vd::u32>::GetTypeId()       { return TypeId::UnsignedInteger;   }
template <> inline TypeId::Value TypeTraits<vd::u64>::GetTypeId()       { return TypeId::UnsignedLong;      }
template <> inline TypeId::Value TypeTraits<vd::f16>::GetTypeId()       { return TypeId::Half;              }
template <> inline TypeId::Value TypeTraits<vd::f32>::GetTypeId()       { return TypeId::Float;             }
template <> inline TypeId::Value TypeTraits<vd::f64>::GetTypeId()       { return TypeId::Double;            }
template <> inline TypeId::Value TypeTraits<vd::ptr>::GetTypeId()       { return TypeId::Pointer;           }
template <> inline TypeId::Value TypeTraits<vd::address>::GetTypeId()   { return TypeId::Address;           }
template <> inline TypeId::Value TypeTraits<vd::bytesize>::GetTypeId()  { return TypeId::ByteSize;          }

// ============================================================================================== //

VD_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_FRAMEWORK_TRAITS_INCLUDED

