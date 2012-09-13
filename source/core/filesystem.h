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

#ifndef VD_CORE_FILESYSTEM_INCLUDED
#define VD_CORE_FILESYSTEM_INCLUDED

#include "core/core.h"
#include "core/system.h"
#include "containers/containers.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

enum {
    VD_FILE_OPEN_READ      = 0x1,
    VD_FILE_OPEN_WRITE     = 0x2,
    VD_FILE_OPEN_CREATE    = 0x4,
    VD_FILE_OPEN_APPEND    = 0x8,
    VD_FILE_OPEN_TRUNCATE  = 0x10,
    VD_FILE_OPEN_BINARY    = 0x20,
    VD_FILE_OPEN_EXCL      = 0x40,
    VD_FILE_OPEN_LARGE     = 0x4000,    
    VD_FILE_OPEN_UNSHARED  = 0x8000
};

enum { 
    VD_FILE_MODE_DEFAULT   = 0x0666,
    VD_FILE_MODE_TEMP      = 0x0666,
    VD_FILE_MODE_UREAD     = 0x0400,
    VD_FILE_MODE_UWRITE    = 0x0200,
    VD_FILE_MODE_UEXECUTE  = 0x0100,
    VD_FILE_MODE_GREAD     = 0x0040,
    VD_FILE_MODE_GWRITE    = 0x0020,
    VD_FILE_MODE_GEXECUTE  = 0x0010,
    VD_FILE_MODE_WREAD     = 0x0004,
    VD_FILE_MODE_WWRITE    = 0x0002,
    VD_FILE_MODE_WEXECUTE  = 0x0001
};

// ============================================================================================== //

class FileSystem
{
public:

    typedef Containers::Vector< vd::string >::type          SearchPath;

#if defined(VD_TARGET_WINDOWS)
    typedef HANDLE                                          FileHandle;
#elif (defined(VD_TARGET_OSX) || defined(VD_TARGET_UNIX))
    typedef int                                             FileHandle;
#endif

    static const FileHandle InvalidFileHandle;

public:

    FileSystem();
    virtual ~FileSystem();

    virtual vd::status Destroy();

    vd::status Open(const vd::string& location = vd::string());
    vd::status Close();

    void 
    SplitSearchPath(
        std::vector<vd::string> &dirs, 
        const vd::string& searchpath, 
        bool validonly=true);

    vd::status
    AddSearchPath(const vd::string& path);

    vd::status
    RemoveSearchPath(const vd::string& path);

    vd::string 
    FindInSearchPath(
        const vd::string& filename, bool testcwd=true);

    bool IsPathAbsolute(const vd::string& path, bool dot_is_absolute=true);
    bool Exists(const vd::string& path);
    bool IsDirectory(const vd::string& path);
    bool IsRegularFile(const vd::string& path);

    vd::string Resolve(const vd::string& path);
    vd::string GetFilename(const vd::string& filepath);
    vd::string GetPrefix(const vd::string& filepath);
    vd::string GetExtension(const vd::string& filepath);

    FileHandle OpenFile(const vd::string& filepath, vd::u32 flag,  vd::u32 mode = VD_FILE_MODE_DEFAULT);
    vd::u64 ReadFile(FileHandle h,       void* buffer, size_t sz);
    vd::u64 WriteFile(FileHandle h, const void* buffer, size_t sz);

    vd::status SeekFile(FileHandle h, std::size_t offset);
    vd::status CloseFile(FileHandle h);
    vd::status FlushFile(FileHandle h);
    vd::status RemoveFile(const vd::string& filepath);
    vd::status DeleteFile(const vd::string& filepath);

    size_t GetMaxPathLength();

    vd::string GetTemporaryDirectory(void);
    vd::string GetTemporaryFile(const vd::string& tmpdir, const vd::string& prefix, bool persistent);
    vd::string GetTemporaryPath(const vd::string& prefix, bool persistent);

private:

    VD_DISABLE_COPY_CONSTRUCTORS(FileSystem);

    SearchPath m_SearchPath;
}; 

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_FILESYSTEM_INCLUDED

