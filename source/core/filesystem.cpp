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

#include "core/filesystem.h"
#include "core/system.h"
#include "core/resources.h"
#include "core/logging.h"
#include "constants/constants.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#if (defined(VD_TARGET_OSX) || defined(VD_TARGET_UNIX))
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#endif

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

static void
SplitPathList(
    const vd::string &str, 
    std::vector<vd::string>& results) 
{
    vd::string substr = vd::string("");
    vd::string endstr = vd::string(":;");
    
    vd::string::size_type cursor = 0;
    vd::string::size_type end;
    do {
        if(cursor == 0)
            cursor = str.find(substr);
        else
            cursor = str.find(substr, cursor);
    
        if (cursor != vd::string::npos)
        {
            end = str.find_first_of(endstr, cursor);
            if(end != vd::string::npos)
            {
                vd::string sym = str.substr(cursor + substr.size(), end-substr.size()-cursor);
                results.push_back( sym );
                cursor += substr.size();
            }
        }
    } while (cursor != vd::string::npos);
}

// ============================================================================================== //

#if defined(VD_TARGET_WINDOWS)
const FileSystem::FileHandle FileSystem::InvalidFileHandle = INVALID_HANDLE_VALUE;
#elif (defined(VD_TARGET_OSX) || defined(VD_TARGET_UNIX))
const FileSystem::FileHandle FileSystem::InvalidFileHandle = -1;
#endif

// ============================================================================================== //

FileSystem::FileSystem()
{

}

FileSystem::~FileSystem()
{
    Destroy();
}

vd::status
FileSystem::Destroy()
{
    Close();        
    return Status::Code::Success;
}

vd::status
FileSystem::AddSearchPath(const vd::string& path)
{
    SearchPath::const_iterator it;
    for(it = m_SearchPath.begin(); it != m_SearchPath.end(); ++it)
    {
        if(*it == path)
        {
            return Status::Code::Reject;
        }
    }

    m_SearchPath.push_back(path);
    return Status::Code::Success;    
}

vd::status
FileSystem::RemoveSearchPath(const vd::string& path)
{
    int i = 0;
    SearchPath::const_iterator it;
    for(it = m_SearchPath.begin(); it != m_SearchPath.end(); ++it, ++i)
    {
        if(*it == path)
        {
            m_SearchPath.erase(m_SearchPath.begin()+i);
            return Status::Code::Success;    
        }
    }
    return Status::Code::Reject;
}

vd::status 
FileSystem::Open(const vd::string&)
{
    AddSearchPath(vdGetResourceFolder());   
    return Status::Code::Success;
}

vd::status 
FileSystem::Close()
{
    return Status::Code::Success;
}

vd::string
FileSystem::GetFilename(
    const vd::string& filepath)
{
    size_t found;
    found = filepath.find_last_of("/\\");
    return filepath.substr(found+1);
}

vd::string
FileSystem::GetExtension(
    const vd::string& filepath)
{
    size_t found;
    found = filepath.find_last_of(".");
    return filepath.substr(found+1);
}

void
FileSystem::SplitSearchPath(
    std::vector<vd::string> &dirs,
    const vd::string& searchpath,
    bool validonly)
{
    dirs.clear();

    std::vector<vd::string> tokens;
    vd::string path_copy = searchpath;
    vd::string last_token;

    SplitPathList(searchpath, tokens);

    std::vector<vd::string>::iterator it;
    for(it = tokens.begin(); it != tokens.end(); last_token = *it, ++it)
    {
        vd::string path = *it;

#if defined(VD_TARGET_WINDOWS)
        if(last_token.length() == 1 && last_token[0] != '.')
        {
            path = last_token + ":" + (*it);
        }
        else
#endif
            path = *it;

        size_t len = path.length();

        while(len > 1 && (path[len - 1] == '/' || path[len - 1] == '\\'))
            path.erase(--len);

        if(!validonly || FileSystem::IsDirectory(path))
            dirs.push_back(path);
    }
}

vd::string
FileSystem::FindInSearchPath(
    const vd::string& filename,
    bool testcwd)
{
    bool abspath = FileSystem::IsPathAbsolute(filename);
    if(testcwd || abspath)
    {
        if(FileSystem::IsRegularFile(filename))
            return filename;
    }

    SearchPath::const_iterator it;
    for(it = m_SearchPath.begin(); it != m_SearchPath.end(); ++it)
    {
        vd::string path = *it;
#if defined(VD_TARGET_WINDOWS)
        vd::string fullname = path + vd::string("\\") + filename;
#else
        vd::string fullname = path + vd::string("/") + filename;
#endif
        vdLogGlobalInfo("Searching for file '%s' ...", fullname.c_str());

        if(FileSystem::IsRegularFile(fullname))
        {
            vdLogGlobalInfo("Found file '%s'!", fullname.c_str());
            return fullname;
        }
    }
    return vd::string();
}

bool
FileSystem::IsPathAbsolute(
    const vd::string& path,
    bool dot_is_absolute)
{
    size_t len = path.length();

    if(!len)
        return false;

    return (
        (path[0] == '/')
        || (dot_is_absolute && path[0] == '.' && path[1] == '/')
        || (dot_is_absolute && path[0] == '.' && path[1] == '.' && path[2] == '/')
#if defined(VD_TARGET_WINDOWS)
        || path[0] == '\\'
        || (dot_is_absolute && path[0] == '.' && path[1] == '\\')
        || (dot_is_absolute && path[0] == '.' && path[1] == '.' && path[2] == '\\')
        || (isalpha(path[0]) && path[1] == ':')
#endif
    );
}

bool
FileSystem::Exists(
    const vd::string& path)
{
    struct stat st;
    if (stat(path.c_str(), &st) == 0)
        return true;
    return false;
}

bool
FileSystem::IsDirectory(
    const vd::string& path)
{
    struct stat st;
    if (stat(path.c_str(), &st) == 0)
        return(S_IFDIR & st.st_mode);
    return false;
}

bool
FileSystem::IsRegularFile(
    const vd::string& path)
{
    struct stat st;
    if (stat(path.c_str(), &st) == 0)
        return(S_IFREG & st.st_mode);
    return false;
}

vd::string
FileSystem::Resolve(
    const vd::string& path)
{
    static bool first = true;
    static vd::string resources;

    if(first)
    {
        resources = vdGetResourceFolder();
        first = false;
    }

    vd::string fullpath;

    if(path[1] == ':' || path[0] == '/')
    {
        fullpath = path;
    }
    else
    {

        fullpath = resources;
        char trailingChar = fullpath[fullpath.size() - 1];

        if(trailingChar != '/' && trailingChar != '\\')
        {
            fullpath = fullpath + '/';
        }

        fullpath = fullpath + path;
    }

    return fullpath;
}

#if defined(VD_TARGET_WINDOWS)

FileHandle
FileSystem::OpenFile(
    const vd::string& filepath,
    vd::u32 flag, 
    vd::u32 mode)
{
    FileHandle tfh;

    DWORD oflags = 0;
    DWORD createflags = 0;
    DWORD attributes = FILE_ATTRIBUTE_NORMAL;
    DWORD sharemode = FILE_SHARE_READ | FILE_SHARE_WRITE ;

    if(flag & VD_FILE_OPEN_READ)
    {
        oflags |= GENERIC_READ;
    }

    if(flag & VD_FILE_OPEN_WRITE)
    {
        oflags |= GENERIC_WRITE;
    }

    if(flag & VD_FILE_OPEN_CREATE)
    {
        if(flag & VD_FILE_OPEN_EXCL)
        {
            createflags = CREATE_NEW;
        }
        else if(flag & VD_FILE_OPEN_TRUNCATE)
        {
            createflags = CREATE_ALWAYS;
        }
        else
        {
            createflags = OPEN_ALWAYS;
        }
    }
    else if(flag & VD_FILE_OPEN_TRUNCATE)
    {
        createflags = TRUNCATE_EXISTING;
    }
    else
    {
        createflags = OPEN_EXISTING;
    }

    if((flag & VD_FILE_OPEN_EXCL) && !(flag & VD_FILE_OPEN_CREATE))
    {
        vdLogGlobalWarning("Failed to open file '%s'!", filepath.c_str());
        return INVALID_HANDLE_VALUE;
    }

#if (_WIN32_WINNT >= 0x0400)
    sharemode |= FILE_SHARE_DELETE;
#endif

    if(flag & VD_FILE_OPEN_UNSHARED)
    {
        sharemode = 0;
    }

    tfh = ::CreateFileA(filepath.c_str(), oflags, sharemode, NULL, createflags, attributes, NULL);
    if(tfh == INVALID_HANDLE_VALUE)
    {
        vdLogGlobalWarning("Failed to open file '%s'!", filepath.c_str());
        return tfh;
    }

    if(flag & VD_FILE_OPEN_APPEND)
    {
        ::SetFilePointer(tfh, 0, NULL, FILE_END);
    }

    return tfh;
}

template<bool b64bit = true>
struct SeekSelector
{
    static BOOL SeekFile(FileHandle h, std::size_t offset)
    {
        LARGE_INTEGER li;
        li.QuadPart = offset;
        return ::SetFilePointer(h, li.LowPart, &li.HighPart, FILE_BEGIN);
    }
};
template<>
struct SeekSelector<false>
{
    static BOOL SeekFile(FileHandle h, std::size_t offset)
    {
        return ::SetFilePointer(h, (LONG)offset, NULL, FILE_BEGIN);
    }
};

vd::status
FileSystem::SeekFile(
    FileHandle h, std::size_t offset)
{
    return (SeekSelector<sizeof(std::size_t) == 8>::SeekFile(h, offset) > 0) ? Status::Code::Success : Status::Code::InvalidFile;
}

vd::u64 
FileSystem::ReadFile (
    FileHandle h, void* buffer, size_t sz)
{
    DWORD nRead=0;
    if(::ReadFile(h,buffer,(DWORD)sz,&nRead,NULL))
    {
        return (vd::u64)nRead;
    }
    return 0; 
}

vd::u64 
FileSystem::WriteFile(
    FileHandle h, const void* buffer, size_t sz)
{
    DWORD nRead=0;
    if(::WriteFile(h,buffer,(DWORD)sz,&nRead,NULL))
    {
        return (vd::u64 )nRead;
    }
    return 0; 
}

vd::status
FileSystem::CloseFile(
    FileHandle h)
{
    return ::CloseHandle(h)? Status::Code::Success : Status::Code::InvalidFile;
}

vd::status
FileSystem::FlushFile(
    FileHandle h)
{
    return ::FlushFileBuffers(h) ? Status::Code::Success : Status::Code::InvalidFile;
}

vd::status
FileSystem::RemoveFile(
    const vd::string& filepath)
{
    return ::DeleteFileA(filepath.c_str()) ? Status::Code::Success : Status::Code::InvalidFile;
}

vd::status
FileSystem::DeleteFile(
    const vd::string& filepath)
{
    return RemoveFile(filepath);
}

size_t
FileSystem::GetMaxPathLength()
{
    return (size_t)MAX_PATH;
}

vd::string
FileSystem::GetTemporaryDirectory(void)
{
    vd::string tmpdir;
    char buffer[MAX_PATH+1] = {0};
    DWORD nsz = ::GetTempPathA((DWORD)MAX_PATH, buffer);
    if(nsz == 0)
    {
        vdLogGlobalWarning("Failed to locate suitable temporary directory!");
        return tmpdir;        
    }
    tmpdir = vd::string(buffer);
    return tmpdir;
}

vd::status
FileSystem::GetTemporaryFile(
    const vd::string& tmpdir, 
    const vd::string& prefix, 
    bool persistent)
{
    vd::string filename;
    char* buffer[MAX_PATH+15] = {0};
    if(0 == ::GetTempFileNameA(tmpdir.c_str(), prefix.c_str(), 0, buffer))
    {
        vdLogGlobalWarning("Failed to locate suitable temporary file for '%s!", tmpdir.c_str());
        return filename;        
    }

    if(persistent == false)
        DeleteFileA(buffer);

    filename = vd::string(buffer);
    return filename;
}

#elif (defined(VD_TARGET_OSX) | defined(VD_TARGET_UNIX))


static vd::u32
ConvertFlagsToFileMode(vd::u32 flags)
{
    int mode = 0;

    if(flags & VD_FILE_MODE_UREAD)
        mode |= S_IRUSR;

    if(flags & VD_FILE_MODE_UWRITE)
        mode |= S_IWUSR;

    if(flags & VD_FILE_MODE_UEXECUTE)
        mode |= S_IXUSR;

    if(flags & VD_FILE_MODE_GREAD)
        mode |= S_IRGRP;

    if(flags & VD_FILE_MODE_GWRITE)
        mode |= S_IWGRP;

    if(flags & VD_FILE_MODE_GEXECUTE)
        mode |= S_IXGRP;


    if(flags & VD_FILE_MODE_WREAD)
        mode |= S_IROTH;

    if(flags & VD_FILE_MODE_WWRITE)
        mode |= S_IWOTH;

    if(flags & VD_FILE_MODE_WEXECUTE)
        mode |= S_IXOTH;

    return mode;
}

FileSystem::FileHandle
FileSystem::OpenFile(
    const vd::string& filepath,
    vd::u32 flag,
    vd::u32 mode)
{
    FileHandle tfh;

    int oflags = 0;

    if((flag & VD_FILE_OPEN_READ) && (flag & VD_FILE_OPEN_WRITE))
    {
        oflags = O_RDWR;
    }
    else if(flag & VD_FILE_OPEN_READ)
    {
        oflags = O_RDONLY;
    }
    else if(flag & VD_FILE_OPEN_WRITE)
    {
        oflags = O_WRONLY;
    }
    else
    {
        return false;
    }

    if(flag & VD_FILE_OPEN_CREATE)
    {
        oflags |= O_CREAT;

        if(flag & VD_FILE_OPEN_EXCL)
        {
            oflags |= O_EXCL;
        }
    }

    if((flag & VD_FILE_OPEN_EXCL) && !(flag & VD_FILE_OPEN_CREATE))
    {
        return false;
    }

    if(flag & VD_FILE_OPEN_APPEND)
    {
        oflags |= O_APPEND;
    }

    if(flag & VD_FILE_OPEN_TRUNCATE)
    {
        oflags |= O_TRUNC;
    }

#ifdef O_BINARY

    if(flag & VD_FILE_OPEN_BINARY)
    {
        oflags |= O_BINARY;
    }

#endif

#if defined(_LARGEFILE64_SOURCE)
    oflags |= O_LARGEFILE;
#elif defined(O_LARGEFILE)

    if(flag & VD_FILE_OPEN_LARGE)
    {
        oflags |= O_LARGEFILE;
    }

#endif

    tfh = open(filepath.c_str(), oflags, ConvertFlagsToFileMode(mode));
    if(tfh < 0)
    {
        vdLogGlobalWarning("Failed to open file '%s'!", filepath.c_str());
        return tfh;
    }
    return tfh;
}

template<class T>
struct SeekSelector
{
    static T seek(int fd, T offset, int origin)
    {
        return lseek(fd, (off_t)offset, origin);
    }
};

#if !defined(VD_TARGET_OSX)
template<>
struct SeekSelector<unsigned long long>
{
    static unsigned long long seek(int fd, unsigned long long offset, int origin)
    {
        return lseek64(fd, offset, origin);
    }
};
#endif


vd::u64 
FileSystem::ReadFile(FileHandle h,       void* buffer, size_t sz)
{
    return ::read(h,buffer,sz); 
}

vd::u64 
FileSystem::WriteFile(FileHandle h, const void* buffer, size_t sz)
{
    return ::write(h,buffer,sz);
}

vd::status
FileSystem::SeekFile(FileHandle h, std::size_t offset)
{
    return (SeekSelector<std::size_t>::seek(h, offset, SEEK_SET) > 0) ? Status::Code::Success : Status::Code::InvalidFile;
}

vd::status
FileSystem::CloseFile(FileHandle h)
{
    return (0 == ::close(h)) ? Status::Code::Success : Status::Code::InvalidFile;
}

vd::status
FileSystem::FlushFile(FileHandle h)
{
    return (0 == ::fsync(h)) ? Status::Code::Success : Status::Code::InvalidFile;
}

vd::status
FileSystem::RemoveFile(
    const vd::string& filepath)
{
    return (0 == ::unlink(filepath.c_str())) ? Status::Code::Success : Status::Code::InvalidFile;
}

vd::status
FileSystem::DeleteFile(
    const vd::string& filepath)
{
    return RemoveFile(filepath);
}

size_t GetMaxPathLength()
{
    return (size_t)1024;
}

static bool
TestTemporaryDirectory(const char* temp_dir)
{
    vd::string tmp(temp_dir);
    tmp += vd::string("/vd.XXXXXX");

    char* buffer = VD_NEW_ARRAY(char, (tmp.size() + 1));
    std::strcpy(buffer, tmp.c_str());
    int fd = mkstemp(buffer);
    if(fd == -1)
    {
        VD_DELETE_ARRAY(buffer);
        return false;
    }
    unlink(buffer);
    VD_DELETE_ARRAY(buffer);
    close(fd);
    return true;
}

vd::string
FileSystem::GetTemporaryDirectory(void)
{
    static const char* try_envs[] = { "TMP", "TEMP", "TMPDIR" };
    static const char* try_dirs[] = { "/tmp", "/usr/tmp", "/var/tmp" };

    vd::string filepath = vd::string("");

    const char* cp = NULL;
    for(std::size_t i = 0; i < (sizeof(try_envs) / sizeof(const char*)); i++)
    {
        cp = getenv(try_envs[i]);
        if(cp != NULL)
            break;
    }

    if(cp == NULL)
    {
        for(std::size_t i = 0; i < (sizeof(try_dirs) / sizeof(const char*)); i++)
        {
            if(TestTemporaryDirectory(try_dirs[i]))
            {
                cp = try_dirs[i];
                break;
            }
        }
    }

    if(cp == NULL)
    {
        vdLogWavdLogGlobalWarningrning("Failed to locate suitable temporary directory!");
        vdLogGlobalWarning("Set one of the following ENV variables to a writeable location: '%s' '%s' '%s'!",
            try_envs[0], try_envs[1], try_envs[2]);
        return filepath;
    }
    filepath = vd::string(cp);
    return filepath;

}

vd::string
FileSystem::GetTemporaryFile(
    const vd::string& tmpdir, 
    const vd::string& prefix,
    bool persistent)
{
    vd::string tempfile(tmpdir);

    char lc = tempfile[tempfile.size() - 1];
    if(lc != '/')
    {
        if(lc != '\\')
        {
            tempfile.push_back('/');
        }
    }

    tempfile += vd::string(prefix);
    tempfile += vd::string("XXXXXX");

    char* buffer = VD_NEW_ARRAY(char, tempfile.size() + 15);

    int fd = mkstemp(buffer);
    if(fd == -1)
    {
        VD_DELETE_ARRAY(buffer);
        vdLogGlobalWarning("Failed to create temporary file for '%s'!", tempfile.c_str());
        return vd::string();
    }

    close(fd);
    if(persistent == false)
        unlink(buffer);
    
    tempfile = vd::string(buffer);
    VD_DELETE_ARRAY(buffer);
    return tempfile;
}

#endif


vd::string
FileSystem::GetTemporaryPath(
    const vd::string& prefix,
    bool persistent)
{
    vd::string tempdir = GetTemporaryDirectory();
    if(tempdir.size() < 1)
    {
        vdLogGlobalWarning("Failed to create temporary file for '%s'!", prefix.c_str());
        return vd::string();
    }

    vd::string tempfile = GetTemporaryFile(tempdir, prefix, persistent);
    if(tempfile.size() < 1)
    {
        vdLogGlobalWarning("Failed to create temporary file for '%s'!", tempfile.c_str());
        return vd::string();
    }

    return tempfile;
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

