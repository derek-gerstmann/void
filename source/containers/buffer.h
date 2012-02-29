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

#ifndef VD_CONTAINERS_BUFFER_INCLUDED
#define VD_CONTAINERS_BUFFER_INCLUDED

// ============================================================================================== //

#include "containers/containers.h"
#include "core/asserts.h"

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_IMPORT(Core, Memory);

// ============================================================================================== //

template< typename Type >
class Buffer
{
public:
	
	typedef Type ValueType;
	
 	Buffer() :	
 		m_Ptr(NULL),
		m_Bytes(0),
		m_EntryCount(0),
		m_ComponentsPerEntry(0)
 	{
 		// EMPTY!
 	}
 	
    virtual ~Buffer()
    {
    	Destroy();
    }

	bool Setup(
		vd::u64 entry_count, 
		vd::u64 components_per_entry,
		bool clear=true)
	{
		vd::bytesize bytes = (entry_count * components_per_entry * sizeof(Type));
		if(m_Ptr != NULL && (m_Bytes >= bytes))
		{
			m_EntryCount = entry_count;
			m_ComponentsPerEntry = components_per_entry;
			return true;
		}
		
		Destroy();

		if(bytes <= 1)
			return true;

		m_Ptr = VD_NEW_ARRAY( Type, entry_count * components_per_entry );
		if(m_Ptr == NULL)
			return false;

	    if(clear)
	        Memory::MemSet(m_Ptr, 0, bytes);

		m_Bytes = bytes;
		m_EntryCount = entry_count;
		m_ComponentsPerEntry = components_per_entry;
		return true;
	}
	
	void Destroy()
	{
		VD_SAFE_DELETE_ARRAY(m_Ptr);

		m_Ptr = NULL;
		m_Bytes = 0;
		m_EntryCount = 0;
		m_ComponentsPerEntry = 0;
	}

    Type* GetPtr()          				 	{ return m_Ptr; }
    const Type* GetPtr() const				 	{ return m_Ptr; }
    vd::u32 GetComponentsPerEntry() const    	{ return m_ComponentsPerEntry; }
    vd::u64 GetEntryCount() const       	 	{ return m_EntryCount; }
    vd::bytesize GetSizeInBytes() const     	{ return m_Bytes; }
    vd::bytesize GetEntrySizeInBytes() const    { return sizeof(Type) * m_ComponentsPerEntry; }

private:
	
	VD_DISABLE_COPY_CONSTRUCTORS(Buffer);

    Type* m_Ptr;
	vd::bytesize m_Bytes;
    vd::u64 m_EntryCount;
    vd::u32 m_ComponentsPerEntry;

};

// ============================================================================================== //

template <typename T>
class DoubleBuffer
{
public:

	DoubleBuffer() :
		m_Front(0), 
		m_Back(1)
	{
		// EMPTY
	}

	DoubleBuffer(const DoubleBuffer& other) :
		m_Front(other.m_Front), 
		m_Back(other.m_Back)
	{
		m_Data[0] = other.m_Data[0];	
		m_Data[1] = other.m_Data[1];	
	}

	DoubleBuffer& operator=(const DoubleBuffer& other)
	{
		if(&other == this)
			return *this;
			
		m_Data[0] = other.m_Data[0];	
		m_Data[1] = other.m_Data[1];	
		m_Front = other.m_Front;
		m_Back = other.m_Back;
		return *this;
	}
	    
	void Swap()
	{
		std::swap(m_Front, m_Back);
	}

	void SetFrontData(const T& v) { m_Data[m_Front] = v; }
	void SetBackData(const T& v) { m_Data[m_Back] = v; }
	
	const T& GetFrontData() const { return m_Data[m_Front]; }
	const T& GetBackData() const { return m_Data[m_Back]; }
	
private:

	T m_Data[2];
	vd::u8 m_Front;
	vd::u8 m_Back;

};


class ReadBuffer 
{
public:
    ReadBuffer() : m_BasePtr(0), m_CursorPtr(0), m_EndPtr(0) {}

    ReadBuffer(const void* data) {
        m_BasePtr = (const char*)data;
        m_CursorPtr = (const char*)data;
        m_EndPtr = 0;  // no bounds checking
    }

    ReadBuffer(const void* data, size_t size) 
    {
        vdGlobalAssert(data != 0 || size == 0);
        m_BasePtr = (const char*)data;
        m_CursorPtr = (const char*)data;
        m_EndPtr = (const char*)data + size;
    }
    
    size_t  GetOffset() const { return m_CursorPtr - m_BasePtr; }

    size_t  size() const { return m_EndPtr - m_BasePtr; }
    bool    eof() const { return m_CursorPtr >= m_EndPtr; }

    void Read(void* buffer, size_t size) {
        if (size) {
            this->ReadNoSizeCheck(buffer, size);
        }
    }

    const void* Skip(size_t size); // return start of Skipped data
    size_t  SkipToAlign4();

    void*       ReadPtr() { void* ptr; Read(&ptr, sizeof(ptr)); return ptr; }
    uint32_t    ReadU32() { uint32_t x; Read(&x, 4); return x; }
    int32_t     ReadS32() { int32_t x; Read(&x, 4); return x; }
    uint16_t    ReadU16() { uint16_t x; Read(&x, 2); return x; }
    int16_t     ReadS16() { int16_t x; Read(&x, 2); return x; }
    uint8_t     ReadU8() { uint8_t x; Read(&x, 1); return x; }
    bool        ReadBool() { return this->ReadU8() != 0; }

protected:
    VD_DISABLE_COPY_CONSTRUCTORS(ReadBuffer);

    void    ReadNoSizeCheck(void* buffer, size_t size);

    const char* m_BasePtr;
    const char* m_CursorPtr;
    const char* m_EndPtr;
};

class WriteBuffer 
{
public:
    WriteBuffer() : m_BasePtr(0), m_CursorPtr(0), m_EndPtr(0) {}
    WriteBuffer(void* data) { Reset(data); }
    WriteBuffer(void* data, size_t size) { Reset(data, size); }

    void Reset(void* data) {
        m_BasePtr = (char*)data;
        m_CursorPtr = (char*)data;
        m_EndPtr = 0;  // no bounds checking
    }

    void Reset(void* data, size_t size) {
        vdGlobalAssert(data != 0 || size == 0);
        m_BasePtr = (char*)data;
        m_CursorPtr = (char*)data;
        m_EndPtr = (char*)data + size;
    }
    
    size_t  GetOffset() const { return m_CursorPtr - m_BasePtr; }
    void*   Skip(size_t size); // return start of Skipped data

    void Write(const void* buffer, size_t size) {
        if (size) {
            this->WriteNoSizeCheck(buffer, size);
        }
    }

    size_t  PadToAlign4();

    void    WritePtr(const void* x) { this->WriteNoSizeCheck(&x, sizeof(x)); }
    void    Write32(int32_t x) { this->WriteNoSizeCheck(&x, 4); }
    void    Write16(int16_t x) { this->WriteNoSizeCheck(&x, 2); }
    void    Write8(int8_t x) { this->WriteNoSizeCheck(&x, 1); }
    void    WriteBool(bool x) { this->Write8(x); }

protected:
    VD_DISABLE_COPY_CONSTRUCTORS(WriteBuffer);

    void    WriteNoSizeCheck(const void* buffer, size_t size);

    char* m_BasePtr;
    char* m_CursorPtr;
    char* m_EndPtr;
};

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CONTAINERS_BUFFER_INCLUDED

