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

#include "core/memmap.h"
#include "core/logging.h"

// ============================================================================================== //

#if (defined(VD_TARGET_LINUX) || defined(VD_TARGET_OSX))
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#endif

#include <stdio.h>

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

MemoryMappedFile::MemoryMappedFile(
	const char* filename, AccessMode::Value access_mode)

: 
	m_IsMapped(false),
	m_Ptr(NULL),
	m_MapSize(0),
	m_FileSize(0),
	m_Id(0),
    m_Filename(filename),
    m_AccessMode(access_mode),
    m_GrowAmount(0),
    m_PageSize(0),
#if defined(VD_TARGET_WINDOWS)
    m_Native(NULL),
    m_MapMode(0)
#else
    m_Native(0),
    m_MapMode(0)
#endif
{
    GetPageSize();
    m_IsMapped = Map(m_Filename.c_str(), m_AccessMode, 0, 0, 0) == 0;
}

// ============================================================================================== //

MemoryMappedFile::MemoryMappedFile(
	const char* filename,
	AccessMode::Value access_mode,
	void* buffer, size_t bytes) 
: 
	m_IsMapped(false),
	m_Ptr(NULL),
	m_MapSize(0),
	m_FileSize(0),
	m_Id(0),
    m_Filename(filename),
    m_AccessMode(access_mode),
    m_GrowAmount(0),
    m_PageSize(0),
#if defined(VD_TARGET_WINDOWS)
    m_Native(NULL),
    m_MapMode(0)
#else
    m_Native(0),
    m_MapMode(0)
#endif
{	
    FILE *fh;
    fh = fopen(m_Filename.c_str(), "wb+");
    if (fh == 0)
    {
		vdLogError("Could not open '%s'!", m_Filename.c_str());
        return;
	}

    size_t written = fwrite(buffer, sizeof(char), bytes, fh);
	if(written != bytes)
	{
		vdLogWarning("Warning: Wrote only '%u' out of '%u' bytes to file: '%s'!\n", 
			(unsigned)written, (unsigned)bytes, m_Filename.c_str());
	}
	
	fclose(fh);
	
    GetPageSize();
    m_IsMapped = Map(m_Filename.c_str(), m_AccessMode, written, 0, written) == 0;
}	

// ============================================================================================== //

MemoryMappedFile::MemoryMappedFile(
	const char* filename, 
	const AccessMode::Value access, 
	const size_t& filesize, 
	const size_t& offset, 
	const size_t& mapsize) 
:
	m_IsMapped(false),
	m_Ptr(NULL),
	m_MapSize(0),
	m_FileSize(0),
	m_Id(0),
    m_Filename(filename),
    m_AccessMode(access),
    m_GrowAmount(0),
    m_PageSize(0),
#if defined(VD_TARGET_WINDOWS)
    m_Native(NULL),
    m_MapMode(0)
#else
    m_Native(0),
    m_MapMode(0)
#endif
{
    GetPageSize();
    m_IsMapped = Map(m_Filename.c_str(), m_AccessMode, filesize, offset, mapsize) == 0;
}

// ============================================================================================== //

MemoryMappedFile::~MemoryMappedFile(void)
{
    Unmap();
}

// ============================================================================================== //

void  MemoryMappedFile::Flush()
{
#if defined(VD_TARGET_WINDOWS)
    if (m_Ptr) FlushViewOfFile(m_Ptr, 0);
#else
    if (m_Ptr) msync(m_Ptr, m_MapSize, MS_SYNC);
#endif
}

// ============================================================================================== //

void  
MemoryMappedFile::Unmap()
{
    if (m_IsMapped)
    {
#if defined(VD_TARGET_WINDOWS)
        CloseHandle(m_Native);
        if (m_Ptr) UnmapViewOfFile(m_Ptr);
#else
        close(m_Native);
        if (m_Ptr) munmap(m_Ptr, m_MapSize);
#endif
        m_IsMapped = false;
        m_Ptr = NULL;

    }
}

// ============================================================================================== //

void* 
MemoryMappedFile::Reopen(
	const size_t& offset, 
	const size_t& mapsize)
{
    if (m_IsMapped) 
    	Unmap();

    m_IsMapped = Map(m_Filename.c_str(), m_AccessMode, m_GrowAmount, offset, mapsize) == 0;
    return m_Ptr;
}

// ============================================================================================== //

void* 
MemoryMappedFile::Remap(
	const size_t& offset, 
	const size_t& mapsize)
{
    if (!m_IsMapped) 
    	return Reopen(offset, mapsize);

    size_t aligned = (offset % size_t(m_PageSize));

#if defined(VD_TARGET_WINDOWS)
    if (m_Ptr) UnmapViewOfFile(m_Ptr);

    DWORD dwHigh = DWORD((((offset-aligned) >> 32) & 0xFFFFFFFF));
    DWORD dwLow = DWORD(((offset-aligned) & 0xFFFFFFFF));

    m_Ptr = MapViewOfFile(m_Native, m_MapMode, dwHigh, dwLow, (mapsize == 0) ? 0 : (size_t)(mapsize+aligned));
    if (m_Ptr == NULL)
    {
#else
    if (m_Ptr) munmap(m_Ptr, m_MapSize);

    m_Ptr = mmap(NULL, ((mapsize == 0) ? (m_FileSize-offset) : mapsize)+aligned, m_MapMode, MAP_SHARED, m_Native, offset-aligned);
    if (m_Ptr == (void *)-1)
    {
#endif
        m_MapSize = 0;
        return NULL;
    }

    m_Ptr = (char*)m_Ptr + aligned;
    m_MapSize = (mapsize != 0) ? mapsize : m_FileSize;

    return m_Ptr;
}

// ============================================================================================== //

int 
MemoryMappedFile::Map(
	const char* strPath, 
	const AccessMode::Value access, 
	const size_t& growsize, 
	const size_t& offset, 
	const size_t& mapsize)
{
    m_Id = rand();

    bool exists = true, grow = false;
    char buffer[4096];
    int res;

    size_t aligned = (offset % m_PageSize);

#if defined(VD_TARGET_WINDOWS)

    struct _stati64 stat_buf;

    HANDLE fd;
    DWORD sz;

    res = _stat64(strPath, &stat_buf);
    if (res < 0)
    {
        if (errno == ENOENT)
            exists = false;
        else
            return -1;
    }
    if (exists)
    {
        if (size_t(stat_buf.st_size)>=growsize)
        {
            m_FileSize = stat_buf.st_size;
        }
        else
        {
            m_FileSize = growsize;
            grow = true;
        }
    }
    else
    {
        if (growsize == 0)
        {
            return -1;
        }
        else
        {
            m_FileSize = growsize;
        }
    }

    DWORD desired_access = (!exists || access == AccessMode::ReadWrite) ? GENERIC_READ    | GENERIC_WRITE     : GENERIC_READ;
    DWORD share_mode     = (!exists || access == AccessMode::ReadWrite) ? FILE_SHARE_READ | FILE_SHARE_WRITE  : FILE_SHARE_READ;
    DWORD protect_mode   = (!exists || access == AccessMode::ReadWrite) ? PAGE_READWRITE : PAGE_READONLY;
    m_MapMode     	     = (!exists || access == AccessMode::ReadWrite) ? FILE_MAP_ALL_ACCESS : FILE_MAP_READ;

    PSECURITY_DESCRIPTOR pSD = (PSECURITY_DESCRIPTOR) malloc(SECURITY_DESCRIPTOR_MIN_LENGTH);
    if(pSD == NULL) return -2;

    InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(pSD, TRUE, (PACL) NULL, FALSE);

    SECURITY_ATTRIBUTES sa;

    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = pSD;
    sa.bInheritHandle = TRUE;

    fd = CreateFileA (strPath, desired_access, share_mode, &sa, exists ? OPEN_EXISTING : OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fd == INVALID_HANDLE_VALUE)
    {
        free(pSD);
        return -3;
    }
    if (! exists || grow)
    {
        size_t write_size;
        if (grow)
        {
            SetFilePointer(fd, 0, NULL, FILE_END);
            write_size = m_FileSize - stat_buf.st_size;
        }
        else
        {
            write_size = m_FileSize;
        }

        // ensure that file is long enough and filled with zero
        memset(buffer, 0, sizeof(buffer));
        for (size_t i = 0; i < write_size/sizeof(buffer); ++i)
        {
            if (! WriteFile(fd, buffer, sizeof(buffer), &sz, NULL))
            {
                return -3;
            }
        }
        if (! WriteFile(fd, buffer, static_cast<DWORD>(write_size%sizeof(buffer)), &sz, NULL))
        {
            return -3;
        }
    }

    sprintf_s(buffer, sizeof(buffer), "%d", m_Id);
    m_Native = CreateFileMappingA(fd, &sa, protect_mode, static_cast<DWORD>((m_FileSize & 0xFFFFFFFF00000000) >> 32), static_cast<DWORD>(m_FileSize & 0xFFFFFFFF), buffer);
    free(pSD);
    if (NULL == m_Native) return -3;

    DWORD dwHigh  = DWORD((((offset-aligned) >> 32) & 0xFFFFFFFF));
    DWORD dwLow = DWORD(((offset-aligned) & 0xFFFFFFFF));

    m_Ptr = MapViewOfFile(m_Native, m_MapMode, dwHigh, dwLow, (mapsize == 0) ? 0 : (size_t)(mapsize+aligned));

    CloseHandle(fd);

    if (NULL == m_Ptr)
    {
        CloseHandle(m_Native);
        return -3;
    }

#else

    struct stat stat_buf;

    res = stat(strPath, &stat_buf);
    if (res < 0)
    {
        if (errno == ENOENT)
            exists = false;
        else
            return -1;
    }
    if (exists)
    {
        if (size_t(stat_buf.st_size)>growsize)
        {
            m_FileSize = stat_buf.st_size;
        }
        else
        {
            m_FileSize = growsize;
            grow = true;
        }
    }
    else
    {
        if (growsize == 0)
            return -1;
        else
            m_FileSize = growsize;
    }

    int desired_access  = (!exists || access == AccessMode::ReadWrite) ? O_RDWR : O_RDONLY;
    int share_mode      = (!exists || access == AccessMode::ReadWrite) ? S_IRUSR | S_IWUSR  : S_IRUSR;
    m_MapMode    		= (!exists || access == AccessMode::ReadWrite) ? PROT_READ | PROT_WRITE : PROT_READ;

    m_Native = open(strPath, (exists) ? desired_access : desired_access | O_CREAT, share_mode);
    if (m_Native < -1) return -1;

    if (! exists || grow)
    {
        size_t write_size;
        if (grow)
        {
            lseek(m_Native, 0, SEEK_END);
            write_size = m_FileSize - stat_buf.st_size;
        }
        else write_size = m_FileSize;

        memset(buffer, 0, sizeof(buffer));
        for (size_t i = 0; i < write_size/sizeof(buffer); ++i)
        {
            if (write(m_Native, buffer, sizeof(buffer)) != sizeof(buffer))
            {
                return -1;
            }
        }
        if ((size_t)(write(m_Native, buffer, write_size%sizeof(buffer))) != write_size%sizeof(buffer))
        {
            return -1;
        }
    }
    
    m_Ptr = mmap(NULL, ((mapsize == 0) ? (m_FileSize-offset) : mapsize)+aligned, m_MapMode, MAP_SHARED, m_Native, offset-aligned);
    if (m_Ptr == (void *)-1) return -1;

#endif

    m_Ptr = (char*)m_Ptr + aligned;
    m_MapSize = (mapsize != 0) ? mapsize : m_FileSize;

    return 0;
}

// ============================================================================================== //

void MemoryMappedFile::GetPageSize()
{
#if defined(VD_TARGET_WINDOWS)
    SYSTEM_INFO siSysInfo;
    GetSystemInfo(&siSysInfo);
    m_PageSize = siSysInfo.dwAllocationGranularity;
#else
    m_PageSize = sysconf(_SC_PAGE_SIZE);
#endif
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //
