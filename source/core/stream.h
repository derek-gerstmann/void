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

#ifndef VD_CORE_STREAM_INCLUDED
#define VD_CORE_STREAM_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/exceptions.h"
#include "core/logging.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class VD_API Stream : public Core::Object 
{
public:

	VD_DECLARE_ENUM(ByteOrder,
		BigEndian,                
		LittleEndian,             
		NetworkByteOrder); 

	Stream();
	
	virtual vd::string ToString() const;

	void SetByteOrder(ByteOrder::Value order);
	inline ByteOrder::Value GetStreamByteOrder() const { return m_StreamByteOrder; }
	inline ByteOrder::Value GetHostByteOrder() { return m_HostByteOrder; }
	
	virtual void Read(void *ptr, size_t bytes) = 0;
	virtual void Write(const void *ptr, size_t bytes) = 0;

	virtual void SetPos(size_t pos) = 0;
	virtual size_t GetPos() const = 0;
	virtual size_t GetSize() const = 0;
	virtual void Truncate(size_t bytes) = 0;
	virtual void Flush() = 0;
	virtual bool IsWriteCapable() const = 0;
	virtual bool IsReadCapable() const = 0;
	void Skip(size_t amount);

	inline void WriteBool(bool value) { WriteU8(value); }
	void WriteI8(vd::i8 value);
	void WriteU8(vd::u8 value);
	void WriteI16(vd::i16 value);
	void WriteU16(vd::u16 value);
	void WriteI32(vd::i32 value);
	void WriteU32(vd::u32 value);
	void WriteI64(vd::i64 value);
	void WriteU64(vd::u64 value);
	void WriteF32(vd::f32 value);
	void WriteF64(vd::f64 value);
	void WriteLine(const vd::string &value);
	void WriteString(const vd::string &value);
	void WriteSize(size_t value) { WriteU64((vd::u64) value); }
	inline void WriteReal(vd::real value) 
	{
#if defined(VD_USE_DOUBLE_PRECISION)
		WriteF64(value);
#else
		WriteF32(value);
#endif
	}

	void WriteArrayI16(const vd::i16* array, size_t count);
	void WriteArrayU16(const vd::u16* array, size_t count);
	void WriteArrayI32(const vd::i32* array, size_t count);
	void WriteArrayU32(const vd::u32* array, size_t count);
	void WriteArrayI64(const vd::i64* array, size_t count);
	void WriteArrayU64(const vd::u64* array, size_t count);
	void WriteArrayF32(const vd::f32* array, size_t count);
	void WriteArrayF64(const vd::f64* array, size_t count);

	inline void WriteArrayReal(const vd::real* array, size_t count) 
	{
#if defined(VD_USE_DOUBLE_PRECISION)
		WriteArrayF64(array, count);
#else
		WriteArrayF32(array, count);
#endif
	}

	bool IsEOF() const;

	inline bool 	ReadBool() { return static_cast<bool>(ReadU8()); }
	vd::i8 			ReadI8();
	vd::u8 			ReadU8();
	vd::i16 		ReadI16();
	vd::u16 		ReadU16();
	vd::i32 		ReadI32();
	vd::u32 		ReadU32();
	vd::i64 		ReadI64();
	vd::u64 		ReadU64();
	vd::f32 		ReadF32();
	vd::f64 		ReadF64();
	vd::string 		ReadLine();
	vd::string 		ReadString();
	size_t 			ReadSize() { return (size_t) ReadU64(); }
	inline vd::real	ReadReal() 
	{
#if defined(VD_USE_DOUBLE_PRECISION)
		return ReadF64();
#else
		return ReadF32();
#endif
	}

	void ReadArrayI16(vd::i16* array, size_t count);
	void ReadArrayU16(vd::u16* array, size_t count);
	void ReadArrayI32(vd::i32* array, size_t count);
	void ReadArrayU32(vd::u32* array, size_t count);
	void ReadArrayI64(vd::i64* array, size_t count);
	void ReadArrayU64(vd::u64* array, size_t count);
	void ReadArrayF32(vd::f32* array, size_t count);
	void ReadArrayF64(vd::f64* array, size_t count);

	inline void ReadArrayReal(vd::real* array, size_t count) 
	{
#if defined(VD_USE_DOUBLE_PRECISION)
		ReadArrayF64(array, count);
#else
		ReadArrayF32(array, count);
#endif
	}

	void CopyTo(Stream *stream, vd::i64 numBytes = -1);
	template <typename T> T ReadValue();
	template <typename T> void WriteValue(T value);
	template <typename T> void ReadArray(T* array, size_t count);
	template <typename T> void WriteArray(const T* array, size_t count);

	VD_DECLARE_OBJECT(Stream);

protected:

	/// Virtual destructor
	virtual ~Stream() { }

private:
	static ByteOrder::Value m_HostByteOrder;
	ByteOrder::Value 		m_StreamByteOrder;
};

// ============================================================================================== //

template <typename T> inline T 
Stream::ReadValue() 
{
	vdLogError("ReadValue<T>: not implemented!");
}

template <typename T> inline void 
Stream::WriteValue(
	T value) 
{
	vdLogError("WriteValue<T>: not implemented!");
}

template <typename T> inline void 
Stream::ReadArray(
	T *array, size_t count) 
{
	vdLogError("Stream::ReadArray<T>: not implemented!");
}

template <typename T> inline void 
Stream::WriteArray(
	const T *array, size_t count) 
{
	vdLogError("Stream::::WriteArray<T>: not implemented!");
}

// ============================================================================================== //

template <> inline bool Stream::ReadValue() 							{ return ReadBool(); 	 }
template <> inline vd::i8 Stream::ReadValue() 							{ return ReadI8(); 	     }
template <> inline vd::u8 Stream::ReadValue() 							{ return ReadU8(); 	     }
template <> inline vd::i16 Stream::ReadValue() 							{ return ReadI16(); 	 }
template <> inline vd::u16 Stream::ReadValue() 							{ return ReadU16(); 	 }
template <> inline vd::i32 Stream::ReadValue() 							{ return ReadI32(); 	 }
template <> inline vd::u32 Stream::ReadValue() 							{ return ReadU32(); 	 }
template <> inline vd::i64 Stream::ReadValue() 							{ return ReadI64(); 	 }
template <> inline vd::u64 Stream::ReadValue() 							{ return ReadU64(); 	 }
template <> inline vd::f32 Stream::ReadValue() 							{ return ReadF32(); 	 }
template <> inline vd::f64 Stream::ReadValue() 							{ return ReadF64(); 	 }
template <> inline vd::string Stream::ReadValue() 						{ return ReadString(); 	 }

template <> inline void Stream::WriteValue(bool v) 						{ return WriteBool(v); 	 }
template <> inline void Stream::WriteValue(vd::i8 v) 					{ return WriteI8(v); 	 }
template <> inline void Stream::WriteValue(vd::u8 v) 					{ return WriteU8(v); 	 }
template <> inline void Stream::WriteValue(vd::i16 v) 					{ return WriteI16(v); 	 }
template <> inline void Stream::WriteValue(vd::u16 v) 					{ return WriteU16(v); 	 }
template <> inline void Stream::WriteValue(vd::i32 v) 					{ return WriteI32(v); 	 }
template <> inline void Stream::WriteValue(vd::u32 v) 					{ return WriteU32(v); 	 }
template <> inline void Stream::WriteValue(vd::i64 v) 					{ return WriteI64(v);    }
template <> inline void Stream::WriteValue(vd::u64 v) 					{ return WriteU64(v); 	 }
template <> inline void Stream::WriteValue(vd::f32 v) 					{ return WriteF32(v); 	 }
template <> inline void Stream::WriteValue(vd::f64 v) 					{ return WriteF64(v); 	 }
template <> inline void Stream::WriteValue(const vd::string &v) 		{ return WriteString(v); }

template <> inline void Stream::ReadArray(bool*    array, size_t count) { return Read((vd::u8*)array, count); }
template <> inline void Stream::ReadArray(vd::i8*  array, size_t count) { return Read((vd::u8*)array, count); }
template <> inline void Stream::ReadArray(vd::u8*  array, size_t count) { return Read((vd::u8*)array, count); }
template <> inline void Stream::ReadArray(vd::i16* array, size_t count) { return ReadArrayI16(array, count);  }
template <> inline void Stream::ReadArray(vd::u16* array, size_t count) { return ReadArrayU16(array, count);  }
template <> inline void Stream::ReadArray(vd::i32* array, size_t count) { return ReadArrayI32(array, count);  }
template <> inline void Stream::ReadArray(vd::u32* array, size_t count) { return ReadArrayU32(array, count);  }
template <> inline void Stream::ReadArray(vd::i64* array, size_t count) { return ReadArrayI64(array, count);  }
template <> inline void Stream::ReadArray(vd::u64* array, size_t count) { return ReadArrayU64(array, count);  }
template <> inline void Stream::ReadArray(vd::f32* array, size_t count) { return ReadArrayF32(array, count);  }
template <> inline void Stream::ReadArray(vd::f64* array, size_t count) { return ReadArrayF64(array, count);  }

template <> inline void Stream::WriteArray(const bool*    array, size_t count) { return Write((vd::u8*)array, count); }
template <> inline void Stream::WriteArray(const vd::i8*  array, size_t count) { return Write((vd::u8*)array, count); }
template <> inline void Stream::WriteArray(const vd::u8*  array, size_t count) { return Write((vd::u8*)array, count); }
template <> inline void Stream::WriteArray(const vd::i16* array, size_t count) { return WriteArrayI16(array, count);  }
template <> inline void Stream::WriteArray(const vd::u16* array, size_t count) { return WriteArrayU16(array, count);  }
template <> inline void Stream::WriteArray(const vd::i32* array, size_t count) { return WriteArrayI32(array, count);  }
template <> inline void Stream::WriteArray(const vd::u32* array, size_t count) { return WriteArrayU32(array, count);  }
template <> inline void Stream::WriteArray(const vd::i64* array, size_t count) { return WriteArrayI64(array, count);  }
template <> inline void Stream::WriteArray(const vd::u64* array, size_t count) { return WriteArrayU64(array, count);  }
template <> inline void Stream::WriteArray(const vd::f32* array, size_t count) { return WriteArrayF32(array, count);  }
template <> inline void Stream::WriteArray(const vd::f64* array, size_t count) { return WriteArrayF64(array, count);  }

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif  // VD_CORE_STREAM_INCLUDED