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

#include "core/memory.h"
#include "core/system.h"
#include "core/atomics.h"
#include "core/locking.h"
#include "core/asserts.h"
#include "core/symbol.h"

#include "containers/containers.h"
#include "constants/constants.h"

#if defined(VD_USE_ASMLIB)
#include "asmlib.h"
#endif

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class MemoryIndex : public Object
{
public:

	struct Info
	{
		vd::i32 	    FileIndex;
		vd::i32 	    FunctionIndex;
		vd::i32 	    LineNumber;
		vd::i64		    AllocNumber;
		vd::bytesize 	SizeInBytes;
	};

public:
	
	MemoryIndex() : m_Size(0) {}
	~MemoryIndex() { }
	
	void* Track(void* ptr, vd::bytesize bytes, const char* file, const char* function, vd::i32 line);
	void Ignore(void* ptr);
	void Dump(void);
	bool Lookup(void* ptr, Info* info);

	VD_DECLARE_OBJECT(MemoryIndex);
	
private:

	typedef Containers::Map<const char*, vd::i32>::type    LocationMap;
	typedef Containers::Map<uintptr_t, Info>::type         InfoMap;
	typedef Containers::Vector<const char*>::type          StringTable;

	VD_DISABLE_COPY_CONSTRUCTORS(MemoryIndex);

	LocationMap        m_Locations;
	StringTable        m_Strings;
	InfoMap            m_Info;
	AtomicCounter      m_Size;
	Mutex              m_Mutex;
	
};
	
// ============================================================================================== //

void* 
Memory::MemSet(
    void* b, int c, size_t len)
{
#if defined(VD_USE_ASMLIB)
	return A_memset(b, c, len);
#else
	return ::memset(b, c, len);
#endif
}

void* 
Memory::MemCopy(
    void* s1, const void* s2, size_t n)
{
#if defined(VD_USE_ASMLIB)
	return A_memcpy(s1, s2, n);
#else
	return ::memcpy(s1, s2, n);
#endif
}

void* 
Memory::MemMove(
    void *s1, const void *s2, size_t n)
{
#if defined(VD_USE_ASMLIB)
	return A_memmove(s1, s2, n);
#else
	return ::memmove(s1, s2, n);
#endif
}


#if defined(VD_DEBUG_MEMORY)

static Core::MemoryIndex* GlobalMemoryIndex = NULL;

void* 
Memory::Track(
    void* ptr, size_t bytes, 
    const char* file, 
    const char* function, 
    int line)
{
    if(GlobalMemoryIndex) 
    	return GlobalMemoryIndex->Track(ptr, bytes, file, function, line);
    return ptr;
}

void 
Memory::Ignore(
    void* ptr)
{
    if(GlobalMemoryIndex) 
    	GlobalMemoryIndex->Ignore(ptr);
}

void 
Memory::Dump(void)
{
    if(GlobalMemoryIndex) 
    	GlobalMemoryIndex->Dump();
}

void 
Memory::Startup(void)
{
    GlobalMemoryIndex = VD_NEW(vd::memory::Tracker);
}

void 
Memory::Shutdown(void)
{
    VD_DELETE(GlobalMemoryIndex);
    GlobalMemoryIndex = NULL;
}
#else

#endif // VD_DEBUG_MEMORY

// ============================================================================================== //

void*
Memory::RawMalloc(
    size_t size)
{
    void* ptr = std::malloc(size);
    vdGlobalAssertMsg(ptr != NULL, "Memory: System memory raw allocation failed!");
    return ptr;
}

void*
Memory::RawRealloc(
    void* ptr, size_t size)
{
    void* nptr = std::realloc(ptr, size);
    vdGlobalAssertMsg(nptr != NULL, "Memory: System memory raw reallocation failed!");
    return nptr;
}

void
Memory::RawFree(
    void* ptr)
{
    if(ptr != NULL)
    {
        std::free(ptr);
	}
}

// ============================================================================================== //

#if defined(VD_TARGET_WINDOWS)

// ============================================================================================== //

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// ============================================================================================== //

void*
Memory::AlignedMalloc(
    size_t size, size_t align)
{
	void* ptr ::_aligned_malloc(size, align);
//    void* ptr = _mm_Malloc(size, align);
    vdGlobalAssertMsg((ptr != NULL), "Memory: System aligned allocation failed!");
    return ptr;
}

void*
Memory::AlignedRealloc(
    void* ptr, size_t size, size_t align)
{
	void* nptr = ::_aligned_realloc(ptr, size, align);
    vdGlobalAssertMsg((nptr != NULL), "Memory: System aligned reallocation failed!");
	return nptr;
}

void
Memory::AlignedFree(
    void* ptr)
{
    if(ptr != NULL)
    {
		::_aligned_free(ptr);
//	    _mm_Free(ptr);
	}
}

// ============================================================================================== //

#endif

// ============================================================================================== //

#if (defined(VD_TARGET_OSX) || defined(VD_TARGET_LINUX))

// ============================================================================================== //

#include <cstdlib>

void*
Memory::AlignedMalloc(
    size_t size, size_t align)
{
    void* ptr = std::malloc(size + (align - 1) + sizeof(void*));
    vdGlobalAssertMsg((ptr != NULL), "Memory: System aligned allocation failed!");

    char* aligned = ((char*)ptr) + sizeof(void*);
    aligned += align - ((uintptr_t)aligned & (align - 1));
    ((void**)aligned)[-1] = ptr;
    
    return aligned;
}

void* 
Memory::AlignedRealloc(
	void* ptr, size_t size, size_t align)
{
	if (ptr == NULL) 
		return Memory::AlignedMalloc(size, align);
	
	void *optr = *(reinterpret_cast<void**>(ptr) - 1);
	void* nptr = std::realloc(optr, size + align);
    vdGlobalAssertMsg((nptr != NULL), "Memory: System aligned reallocation failed!");
	
    char* aligned = ((char*)nptr) + sizeof(void*);
    aligned += align - ((uintptr_t)aligned & (align - 1));
    ((void**)aligned)[-1] = nptr;

	return aligned;
}

void
Memory::AlignedFree(
    void* ptr)
{
    if(ptr != NULL)
    {
		std::free(((void**)ptr)[-1]);
	}
}

// ============================================================================================== //

#endif

// ============================================================================================== //

void* 
MemoryIndex::Track(
    void* ptr, vd::bytesize bytes, 
    const char* file,
    const char* function, 
    vd::i32 line_number)
{
    if(ptr == NULL)
        return ptr;

    ScopedMutex lock(&m_Mutex);
    
    const uintptr_t iptr = (uintptr_t) ptr;
    vdAssertMsg(m_Info.find(iptr) == m_Info.end(), "Pointer already being tracked!");

    vd::i32 file_index = 0;
    LocationMap::const_iterator file_itr = m_Locations.find(file);
    if(file_itr == m_Locations.end())
    {
        m_Strings.push_back(file);
        m_Locations[file] = file_index = m_Strings.size() - 1;
    }
    else
    {
        file_index = m_Locations[file];
	}
	
    vd::i32 func_index = 0;
    LocationMap::const_iterator func_itr = m_Locations.find(function);
    if(func_itr == m_Locations.end())
    {
        m_Strings.push_back(function);
        m_Locations[function] = func_index = m_Strings.size() - 1;
    }
    else
    {
        func_index = m_Locations[function];
	}
	
	Info info;
	info.AllocNumber = m_Size;
	info.SizeInBytes= bytes;
	info.FileIndex = file_index;
	info.FunctionIndex = func_index;
	info.LineNumber = line_number;
    m_Info[iptr] = info;
	m_Size++;
    return ptr;
}

void 
MemoryIndex::Ignore(
    void* ptr)
{
    if(ptr == NULL) return;

    ScopedMutex lock(&m_Mutex);
    const uintptr_t iptr = (uintptr_t) ptr;
    vdAssertMsg(m_Info.find(iptr) == m_Info.end(), "Pointer not being tracked!");
    m_Info.erase(iptr);
}

bool 
MemoryIndex::Lookup(
    void* ptr, Info* info)
{
	if(ptr == NULL) 
		return false;
	
	const uintptr_t iptr = (uintptr_t) ptr;
	InfoMap::const_iterator it = m_Info.find(iptr);
	if(it == m_Info.end())
		return false;
		
	if(info != NULL)
		(*info) = it->second;	

	return true;
}

void 
MemoryIndex::Dump(void)
{
    Core::ScopedMutex lock(&m_Mutex);

    vdLogGlobalInfo("-- start memory tracker info -- \n");
 	vd::bytesize total_bytes = 0;
    for(InfoMap::const_iterator it = m_Info.begin(); it != m_Info.end(); ++it)
    {
    	const uintptr_t iptr = (uintptr_t) it->first;
        const Info& info = it->second;
    	total_bytes += info.SizeInBytes;
        
        vdLogGlobalInfo("[%d] %p (%ull bytes) : %s : %s : %d\n",
        	info.AllocNumber, 
        	iptr, 
        	(vd::i64)info.SizeInBytes,
        	m_Strings[info.FileIndex], 
        	m_Strings[info.FunctionIndex], 
        	info.LineNumber);
    }

    vdLogGlobalInfo("------------------------------\n");
    vdLogGlobalInfo("Total Allocations:  '%d'\n", m_Size.Get());
    vdLogGlobalInfo("Active Allocations: '%d' (%ull bytes)\n", (vd::i32)m_Info.size(), total_bytes);
    vdLogGlobalInfo("-- end memory tracker info -- \n");
}

// ============================================================================================== //

VD_IMPLEMENT_OBJECT(MemoryIndex, vd_sym(MemoryIndex), vd_sym(Object));

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //


