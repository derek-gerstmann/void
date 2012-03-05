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

#ifndef VD_CORE_MEMMAP_INCLUDED
#define VD_CORE_MEMMAP_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/object.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class MemoryMappedFile : public Object 
{
public:

	VD_DECLARE_ENUM(AccessMode,
		ReadOnly,
        WriteOnly,
		ReadWrite);

	/// Map the specified file into memory
	MemoryMappedFile(const char* filename, AccessMode::Value mode);

	/// Map or create the specified file and map the given region into memory
	MemoryMappedFile(const char* filename, const AccessMode::Value access, 
				 	 const size_t& filesize, const size_t& offset, const size_t& mapsize);
	
	/// Create a new file and map the file into memory
	MemoryMappedFile(const char* filename, AccessMode::Value mode, void* buffer, size_t bytes);

	// Unmap any existing file
	~MemoryMappedFile();

	/// Return a pointer to the file contents in memory
	inline void *GetData() { return m_Ptr; }

	/// Return a pointer to the file contents in memory (const version)
	inline const void *GetData() const { return m_Ptr; }

	/// Return the size of the mapped region
    inline size_t  GetMappedSize() const {   return m_MapSize;    }
    inline size_t  GetFileSize() const { return m_FileSize;    }
    inline bool IsMapped() const { return m_IsMapped;  }

    void  Flush();
    void  Unmap();
    void* Reopen(const size_t& offset=0, const size_t& mapsize=0);
    void* Remap(const size_t& offset=0, const size_t& mapsize=0);
    void  Offset(const size_t& offset=0, const size_t& mapsize=0);

	VD_DECLARE_OBJECT(MemoryMappedFile);
	
protected:
    bool  				m_IsMapped;
    void*  				m_Ptr;
    size_t  			m_MapSize;
    size_t  			m_FileSize;
    int    				m_Id;
    std::string	      	m_Filename;
    AccessMode::Value   m_AccessMode;
    size_t        		m_GrowAmount;

    int    Map(const char* strPath, 
    		   const AccessMode::Value access, 
    		   const size_t& growsize = 0, 
    		   const size_t& offset=0,
    		   const size_t& mapsize=0);

private:
	MemoryMappedFile(const MemoryMappedFile&);
    void GetPageSize();

#if defined(VD_TARGET_WINDOWS)
    DWORD  	m_PageSize;
    HANDLE  m_Native;
    DWORD  	m_MapMode;
#else
    long  	m_PageSize;
    int    	m_Native;
    int    	m_MapMode;
#endif
};

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_MEMMAP_INCLUDED

