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

#ifndef VD_CORE_GLOBBING_INCLUDED
#define VD_CORE_GLOBBING_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/asserts.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

/*
    Glob implementation was initially based on SimpleGlob.  License follows:

    The licence text below is the boilerplate "MIT Licence" used from:
    http://www.opensource.org/licenses/mit-license.php

    Copyright (c) 2006-2007, Brodie Thiesfield

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

// ============================================================================================== //

namespace Glob 
{

// ============================================================================================== //

enum Flags
{
    VD_GLOB_ERR         = 1 << 0,
    VD_GLOB_MARK        = 1 << 1,
    VD_GLOB_NOSORT      = 1 << 2,
    VD_GLOB_NOCHECK     = 1 << 3,
    VD_GLOB_TILDE       = 1 << 4,
    VD_GLOB_ONLYDIR     = 1 << 5,
    VD_GLOB_ONLYFILE    = 1 << 6,
    VD_GLOB_NODOT       = 1 << 7,
    VD_GLOB_FULLSORT    = 1 << 8
};

VD_DECLARE_ENUM(Result,
    Success,
    NotFound,
    OutOfMemory,
    UnknownFailure);

VD_DECLARE_ENUM(FileType,
    Normal,
    Directory);

VD_DECLARE_ENUM(ContentType,
    Offsets,
    Pointers);

// ============================================================================================== //

class Methods
{
public:
    
    static const char* strchr(const char* s, char c)
    {
        return (char*) vd_strchr((const vd_char*)s, c);
    }

    static const wchar_t* strchr(const wchar_t* s, wchar_t c)
    {
        return ::wcschr(s, c);
    }

#if VD_HAVE_ICU
    static const UChar* strchr(const UChar* s, UChar c)
    {
        return ::u_strchr(s, c);
    }
#endif

    static const char* strrchr(const char* s, char c)
    {
        return (char*) vd_strrchr((const vd_char*)s, c);
    }

    static const wchar_t* strrchr(const wchar_t* s, wchar_t c)
    {
        return ::wcsrchr(s, c);
    }

#if VD_HAVE_ICU
    static const UChar* strrchr(const UChar* s, UChar c)
    {
        return ::u_strrchr(s, c);
    }
#endif

    static size_t strlen(const char* s) 
    { 
        return ::strlen(s); 
    }

    static size_t strlen(const wchar_t* s) 
    { 
        return ::wcslen(s); 
    }

#if VD_HAVE_ICU
    static size_t strlen(const UChar* s) 
    { 
        return ::u_strlen(s); 
    }
#endif

    static void strcpy_s(char* dst, size_t n, const char* src)
    {
        (void) n;
        vd_strcpy_s((vd_char*)dst, n, (const vd_char*)src);
    }

    static void strcpy_s(wchar_t* dst, size_t n, const wchar_t* src)
    {
#if __STDC_WANT_SECURE_LIB__
        ::wcscpy_s(dst, n, src);
#else
        (void) n;
        ::wcscpy(dst, src);
#endif
    }

#if VD_HAVE_ICU
    static void strcpy_s(UChar* dst, size_t n, const UChar* src)
    {
        ::u_strncpy(dst, src, n);
    }
#endif

    static vd::i32 strcmp(const char* s1, const char* s2)
    {
        return vd_strcmp((const vd_char*)s1, (const vd_char*)s2);
    }

    static vd::i32 strcmp(const wchar_t* s1, const wchar_t* s2)
    {
        return ::wcscmp(s1, s2);
    }

#if VD_HAVE_ICU
    static vd::i32 strcmp(const UChar* s1, const UChar* s2)
    {
        return ::u_strcmp(s1, s2);
    }
#endif

    static vd::i32 strcasecmp(const char* s1, const char* s2)
    {
        return vd_strcasecmp((const vd_char*)s1, (const vd_char*)s2);
    }

#if defined(VD_TARGET_WINDOWS)
    static vd::i32 strcasecmp(const wchar_t* s1, const wchar_t* s2)
    {
        return ::_wcsicmp(s1, s2);
    }
#endif

#if VD_HAVE_ICU
    static vd::i32 strcasecmp(const UChar* s1, const UChar* s2)
    {
        return u_strcasecmp(s1, s2, 0);
    }
#endif

};

// ---------------------------------------------------------------------------------------------- //

#if defined(VD_TARGET_WINDOWS)

// ---------------------------------------------------------------------------------------------- //

#if !defined(VD_INVALID_FILE_ATTRIBUTES)
    #define  VD_INVALID_FILE_ATTRIBUTES  ((DWORD)-1)
#endif

#if !defined(VD_PATH_CHAR)
    #define  VD_PATH_CHAR                '\\'
#endif

// ---------------------------------------------------------------------------------------------- //

template<class CharType>
struct Base
{
    Base() : m_Handle(INVALID_HANDLE_VALUE) { }

    Glob::Result 
    FindFirstFileS(const char* file_spec, vd::u32)
    {
        m_Handle = FindFirstFileA(file_spec, &m_AsciiBuffer);

        if(m_Handle != INVALID_HANDLE_VALUE)
        {
            return Result::Success;
        }

        DWORD dwErr = GetLastError();

        if(dwErr == ERROR_FILE_NOT_FOUND)
        {
            return Result::NotFound;
        }

        return Result::UnknownFailure;
    }

    Glob::Result 
    FindFirstFileS(const wchar_t* file_spec, vd::u32)
    {
        m_Handle = FindFirstFileW(file_spec, &m_UnicodeBuffer);

        if(m_Handle != INVALID_HANDLE_VALUE)
        {
            return Result::Success;
        }

        DWORD err = GetLastError();
        if(err == ERROR_FILE_NOT_FOUND)
        {
            return Result::NotFound;
        }

        return Result::UnknownFailure;
    }

    bool 
    FindNextFileS(char)
    {
        return FindNextFileA(m_Handle, &m_AsciiBuffer) != FALSE;
    }
    
    bool 
    FindNextFileS(wchar_t)
    {
        return FindNextFileW(m_Handle, &m_UnicodeBuffer) != FALSE;
    }

    void 
    FindDone()
    {
        FindClose(m_Handle);
    }

    const char* 
    GetFileNameS(char) const
    {
        return m_AsciiBuffer.cFileName;
    }

    const wchar_t* 
    GetFileNameS(wchar_t) const
    {
        return m_UnicodeBuffer.cFileName;
    }

    bool 
    IsDirS(char) const
    {
        return GetFileTypeS(m_AsciiBuffer.dwFileAttributes) == FileType::Directory;
    }

    bool 
    IsDirS(wchar_t) const
    {
        return GetFileTypeS(m_UnicodeBuffer.dwFileAttributes) == FileType::Directory;
    }

    Glob::FileType 
    GetFileTypeS(const char* path)
    {
        return GetFileTypeS(GetFileAttributesA(path));
    }
    
    Glob::FileType 
    GetFileTypeS(const wchar_t* path)
    {
        return GetFileTypeS(GetFileAttributesW(path));
    }

    Glob::FileType 
    GetFileTypeS(DWORD attribs) const
    {
        if(attribs == VD_INVALID_FILE_ATTRIBUTES)
        {
            return FileType::Invalid;
        }

        if(attribs & FILE_ATTRIBUTE_DIRECTORY)
        {
            return FileType::Directory;
        }

        return FileType::Normal;
    }

private:
    HANDLE              m_Handle;
    WIN32_FIND_DATAA    m_AsciiBuffer;
    WIN32_FIND_DATAW    m_UnicodeBuffer;
};

// ---------------------------------------------------------------------------------------------- //

#else // !_WIN32

// ---------------------------------------------------------------------------------------------- //

#define VD_PATH_CHAR        '/'
#define VD_INVALID_CURSOR   ((size_t) - 1)

template<class CharType>
struct Base
{
    Base() : 
        m_Cursor(VD_INVALID_CURSOR),
        m_IsDir(false),
#if VD_HAVE_ICU
        m_UnicodeBuffer(NULL),
#endif
        m_AsciiBuffer(NULL)
    {
        Memory::SetBytes(&m_Glob, 0, sizeof(m_Glob));
    }

    ~Base()
    {
#if VD_HAVE_ICU
        VD_DELETE_ARRAY(m_UnicodeBuffer);
#endif
        VD_DELETE_ARRAY(m_AsciiBuffer);
        globfree(&m_Glob);
    }

    void 
    SetupFile()
    {
        m_AsciiBuffer = VD_NEW_ARRAY(char, VD_MAX_PATH_LENGTH);
        Memory::SetBytes(m_AsciiBuffer, 0, sizeof(char) * VD_MAX_PATH_LENGTH);

#if VD_HAVE_ICU
        m_UnicodeBuffer = VD_NEW_ARRAY(UChar, VD_MAX_PATH_LENGTH);
        Memory::SetBytes(m_UnicodeBuffer, 0, sizeof(UChar) * VD_MAX_PATH_LENGTH);
#endif

        m_IsDir = false;
        size_t length = ::strlen(m_Glob.gl_pathv[m_Cursor]);
        if(m_Glob.gl_pathv[m_Cursor][length-1] == '/')
        {
            m_IsDir = true;
            m_Glob.gl_pathv[m_Cursor][length-1] = 0;
        }
    }

    Glob::Result
    FindFirstFileS(
        const char* file_spec, vd::u32 flags)
    {
        vd::i32 nFlags = GLOB_MARK | GLOB_NOSORT;

        if(flags & VD_GLOB_ERR)    
            nFlags |= GLOB_ERR;

        if(flags & VD_GLOB_TILDE)  
            nFlags |= GLOB_TILDE;

        vd::i32 rc = glob(file_spec, nFlags, NULL, &m_Glob);

        if(rc == GLOB_NOSPACE) 
            return Result::OutOfMemory;

        if(rc == GLOB_ABORTED) 
            return Result::UnknownFailure;

        if(rc == GLOB_NOMATCH) 
            return Result::NotFound;

        m_Cursor = 0;
        SetupFile();
        return Result::Success;
    }

#if VD_HAVE_ICU
    Glob::Result
    FindFirstFileS(
        const UChar* file_spec, vd::u32 flags)
    {
        UErrorCode status = U_ZERO_ERROR;
        u_strToUTF8(m_AsciiBuffer, sizeof(char) * VD_MAX_PATH_LENGTH, NULL, file_spec, -1, &status);
        if(U_FAILURE(status)) return Result::UnknownFailure;
        return FindFirstFileS(m_AsciiBuffer, flags);
    }
#endif

    bool 
    FindNextFileS(char)
    {
        vdGlobalAssert(m_Cursor != VD_INVALID_CURSOR);

        if(++m_Cursor >= m_Glob.gl_pathc)
        {
            return false;
        }

        SetupFile();
        return true;
    }

#if VD_HAVE_ICU
    bool 
    FindNextFileS(UChar)
    {
        return FindNextFileS((char)0);
    }
#endif

    void 
    FindDone()
    {
        globfree(&m_Glob);
        memset(&m_Glob, 0, sizeof(m_Glob));
        m_Cursor = (size_t) - 1;
    }

    const char* 
    GetFileNameS(char) const
    {
        vdGlobalAssert(m_Cursor != VD_INVALID_CURSOR);
        return m_Glob.gl_pathv[m_Cursor];
    }

#if VD_HAVE_ICU
    const UChar* 
    GetFileNameS(UChar) const
    {
        const char* filename = GetFileNameS((char)0);
        if(!filename) 
            return NULL;

        UErrorCode status = U_ZERO_ERROR;
        u_strToUTF8(m_AsciiBuffer, sizeof(char) * VD_MAX_PATH_LENGTH, NULL, filename, -1, &status);
        if(U_FAILURE(status)) 
            return NULL;

        return m_AsciiBuffer;
    }
#endif

    bool 
    IsDirS(char) const
    {
        vdGlobalAssert(m_Cursor != VD_INVALID_CURSOR);
        return m_IsDir;
    }

#if VD_HAVE_ICU
    bool 
    IsDirS(UChar) const
    {
        return IsDirS((char)0);
    }
#endif

    Glob::FileType 
    GetFileTypeS(const char* path) const
    {
        struct stat sb;

        if(0 != stat(path, &sb))
        {
            return FileType::Invalid;
        }

        if(S_ISDIR(sb.st_mode))
        {
            return FileType::Directory;
        }

        if(S_ISREG(sb.st_mode))
        {
            return FileType::Normal;
        }

        return FileType::Invalid;
    }

#if VD_HAVE_ICU
    Glob::FileType 
    GetFileTypeS(const UChar* path) const
    {
        UErrorCode status = U_ZERO_ERROR;
        u_strToUTF8(m_AsciiBuffer, sizeof(char) * VD_MAX_PATH_LENGTH, NULL, path, -1, &status);
        if(U_FAILURE(status)) return FileType::Invalid;
        return GetFileTypeS(m_AsciiBuffer);
    }
#endif

private:
    size_t  m_Cursor;
    bool    m_IsDir;
#if VD_HAVE_ICU
    mutable UChar* m_UnicodeBuffer;
#endif
    mutable char* m_AsciiBuffer;
    glob_t  m_Glob;
};

// ---------------------------------------------------------------------------------------------- //

#endif // VD_TARGET_WINDOWS

// ---------------------------------------------------------------------------------------------- //

template<class CharType>
class Template : private Base<CharType>
{
public:
    Template(vd::u32 flags = 0, vd::i32 slots = 0);
    ~Template();

    Glob::Result 
    Setup(vd::u32 flags = 0, vd::i32 slots = 0);
    
    Glob::Result 
    Add(const CharType* file_spec);
    
    Glob::Result 
    Add(vd::i32 count, const CharType* const* file_spec);
    
    VD_INLINE vd::i32 
    GetFileCount() const 
    { 
        return m_ArgCount; 
    }

    VD_INLINE CharType** 
    GetFileList()
    {
        SetArgvContentType(ContentType::Pointers);
        return m_ArgData;
    }

    VD_INLINE CharType* 
    GetFile(vd::i32 n)
    {
        vdGlobalAssert(n >= 0 && n < m_ArgCount);
        return GetFileList()[n];
    }

private:
    VD_DISABLE_COPY_CONSTRUCTORS(Template);

    void 
    SetArgvContentType(Glob::ContentType::Value type);

    Glob::Result 
    AppendName(const CharType* filename, bool is_dir);
    
    bool 
    GrowArgvArray(vd::i32 capacity);
    
    bool 
    GrowStringBuffer(vd::i32 capacity);

    static vd::i32 
    FileSortCompare(const void* a1, const void* a2);

private:
    vd::u32     m_Flags;
    ContentType m_ContentType;
    vd::i32     m_SlotCount; 
    CharType**  m_ArgData; 
    vd::i32     m_ArgCapacity;
    vd::i32     m_ArgCount; 
    CharType*   m_StringData; 
    vd::i32     m_StringCapacity; 
    vd::i32     m_StringCount;    
    CharType*   m_PathPrefix; 
};

template<class CharType>
Template<CharType>::Template(
    vd::u32 flags, vd::i32 slots
) : 
    m_Flags(0),
    m_ContentType(ContentType::Invalid),
    m_SlotCount(0),
    m_ArgData(NULL),
    m_ArgCapacity(0),
    m_ArgCount(0),
    m_StringData(NULL),
    m_StringCapacity(0),
    m_StringCount(0),
    m_PathPrefix(NULL)
{
    Setup(flags, slots);
}

template<class CharType>
Template<CharType>::~Template()
{
    VD_SAFE_DELETE(m_ArgData);
    VD_SAFE_DELETE(m_StringData);
    VD_SAFE_DELETE(m_PathPrefix);
}

template<class CharType>
Glob::Result 
Template<CharType>::Setup(
    vd::u32 flags, vd::i32 slots)
{
    m_PathPrefix = VD_NEW_ARRAY(CharType, VD_MAX_PATH_LENGTH);
    Memory::SetBytes(m_PathPrefix, 0, sizeof(CharType) * VD_MAX_PATH_LENGTH);

    m_ContentType = ContentType::Pointers;
    m_Flags = flags;
    m_ArgCount = slots;
    m_SlotCount = slots;
    m_StringCount = 0;

    if(m_SlotCount > 0)
    {
        if(!GrowArgvArray(m_SlotCount))
        {
            return Result::OutOfMemory;
        }

        for(vd::i32 n = 0; n < m_SlotCount; ++n)
        {
            m_ArgData[n] = NULL;
        }
    }

    return Result::Success;
}

template<class CharType>
Glob::Result 
Template<CharType>::Add(
    const CharType* file_spec)
{

#if defined(VD_TARGET_WINDOWS)
    CharType szFileSpec[VD_MAX_PATH_LENGTH];
    Methods::strcpy_s(szFileSpec, VD_MAX_PATH_LENGTH, file_spec);
    const CharType* pszPath = Methods::strchr(szFileSpec, '/');

    while(pszPath)
    {
        szFileSpec[pszPath - szFileSpec] = VD_PATH_CHAR;
        pszPath = Methods::strchr(pszPath + 1, '/');
    }

    file_spec = szFileSpec;
#endif

    m_PathPrefix[0] = 0;
    if(!Methods::strchr(file_spec, '*') && !Methods::strchr(file_spec, '?'))
    {
        FileType nType = GetFileTypeS(file_spec);
        if(nType == FileType::Invalid)
        {
            if(m_Flags & VD_GLOB_NOCHECK)
            {
                return AppendName(file_spec, false);
            }

            return Result::NotFound;
        }

        return AppendName(file_spec, nType == FileType::Directory);
    }

#if defined(VD_TARGET_WINDOWS)
    const CharType* filename = Methods::strrchr(file_spec, VD_PATH_CHAR);
    if(filename)
    {
        Methods::strcpy_s(m_PathPrefix, VD_MAX_PATH_LENGTH, file_spec);
        m_PathPrefix[filename - file_spec + 1] = 0;
    }
#endif

    Glob::Result rc = FindFirstFileS(file_spec, m_Flags);
    if(rc != Result::Success)
    {
        if(rc == Result::NotFound && (m_Flags & VD_GLOB_NOCHECK))
        {
            Glob::Result ok = AppendName(file_spec, false);
            if(ok != Result::Success) rc = ok;
        }

        return rc;
    }

    Glob::Result status;
    bool success = false;;
    vd::i32 start = m_ArgCount;
    do
    {
        status = AppendName(GetFileNameS((CharType)0), IsDirS((CharType)0));
        success = FindNextFileS((CharType)0);
    }
    while(status == Result::Success && success);

    Base<CharType>::FindDone();

    if(m_ArgCount > start && !(m_Flags & VD_GLOB_NOSORT))
    {
        if(m_Flags & VD_GLOB_FULLSORT)
        {
            start = m_SlotCount;
        }

        SetArgvContentType(ContentType::Pointers);
        qsort(
            m_ArgData + start,
            m_ArgCount - start,
            sizeof(m_ArgData[0]), FileSortCompare);
    }

    return status;
}

template<class CharType> 
Glob::Result 
Template<CharType>::Add(
    vd::i32                     count,
    const CharType* const*   file_spec)
{
    Glob::Result result;
    for(vd::i32 n = 0; n < count; ++n)
    {
        result = Add(file_spec[n]);
        if(result != Result::Success)
        {
            return result;
        }
    }

    return Result::Success;
}

template<class CharType>
Glob::Result 
Template<CharType>::AppendName(
    const CharType* filename, bool is_dir)
{
    SetArgvContentType(ContentType::Offsets);
    if((m_Flags & VD_GLOB_ONLYDIR) && !is_dir)
    {
        return Result::Success;
    }

    if((m_Flags & VD_GLOB_ONLYFILE) && is_dir)
    {
        return Result::Success;
    }

    if((m_Flags & VD_GLOB_NODOT) && is_dir)
    {
        if(filename[0] == '.')
        {
            if(filename[1] == '\0')
            {
                return Result::Success;
            }

            if(filename[1] == '.' && filename[2] == '\0')
            {
                return Result::Success;
            }
        }
    }

    if(!GrowArgvArray(m_ArgCount + 1))
    {
        return Result::OutOfMemory;
    }

    size_t prefix_length = Methods::strlen(m_PathPrefix);
    size_t length = prefix_length + Methods::strlen(filename) + 1;

    if(is_dir && (m_Flags & VD_GLOB_MARK) == VD_GLOB_MARK)
    {
        ++length;
    }

    if(!GrowStringBuffer(m_StringCount + length))
    {
        return Result::OutOfMemory;
    }

    m_ArgData[m_ArgCount++] = (CharType*)m_StringCount;
    Methods::strcpy_s(m_StringData + m_StringCount, m_StringCapacity - m_StringCount, m_PathPrefix);
    Methods::strcpy_s(m_StringData + m_StringCount + prefix_length, m_StringCapacity - m_StringCount - prefix_length, filename);
    m_StringCount += length;

    if(is_dir && (m_Flags & VD_GLOB_MARK) == VD_GLOB_MARK)
    {
        const static CharType separator[] = { VD_PATH_CHAR, 0 };
        Methods::strcpy_s(m_StringData + m_StringCount - 2, m_StringCapacity - (m_StringCount - 2), separator);
    }

    return Result::Success;
}

template<class CharType>
void Template<CharType>::SetArgvContentType(
    Glob::ContentType::Value type)
{
    if(m_ContentType == type) 
        return;

    switch(type)
    {
        case ContentType::Pointers:
        {
            for(vd::i32 n = 0; n < m_ArgCount; ++n)
            {
                m_ArgData[n] = (m_ArgData[n] == (CharType*) - 1) ?
                                NULL                             : (m_StringData + (size_t) m_ArgData[n]);
            }
            m_ContentType = type;
            break;
        }
        case ContentType::Offsets:
        {
            for(vd::i32 n = 0; n < m_ArgCount; ++n)
            {
                m_ArgData[n] = (m_ArgData[n] == NULL) ?
                               (CharType*) - 1        : (CharType*)(m_ArgData[n] - m_StringData);
            }
            m_ContentType = type;
            break;
        }
        case ContentType::Invalid:
        default:
        {
            vdGlobalAssertMsg(false, "Invalid content type used for Glob arg data!");
        }
    };
}

template<class CharType>
bool Template<CharType>::GrowArgvArray(
    vd::i32 capacity)
{
    if(capacity >= m_ArgCapacity)
    {
        static const vd::i32 initial = 128;

        vd::i32 reserve = (m_ArgCapacity > 0) ?
                          (m_ArgCapacity * 2) : initial;

        while(reserve >= capacity)
        {
            reserve *= 2;
        }

        void* data = Memory::RawRealloc(m_ArgData, reserve * sizeof(CharType*));
        if(!data) return false;

        m_ArgCapacity = reserve;
        m_ArgData = (CharType**) data;
    }

    return true;
}

template<class CharType>
bool Template<CharType>::GrowStringBuffer(
    vd::i32 capacity)
{
    if(capacity >= m_StringCapacity)
    {
        static const vd::i32 initial = 1024;

        vd::i32 reserve = (m_StringCapacity > 0) ?
                         (m_StringCapacity * 2) : initial;

        while(capacity >= reserve)
        {
            reserve *= 2;
        }

        void* data = Memory::RawRealloc(m_StringData, reserve * sizeof(CharType));
        if(!data) return false;

        m_StringCapacity = reserve;
        m_StringData = (CharType*) data;
    }

    return true;
}

template<class CharType>
vd::i32 Template<CharType>::FileSortCompare(
    const void* a1, const void* a2)
{
    const CharType* s1 = *(const CharType**)a1;
    const CharType* s2 = *(const CharType**)a2;

    if(s1 && s2)
    {
        return Methods::strcasecmp(s1, s2);
    }

    return s1 == s2 ? 0 : (s1 ? 1 : -1);
}

// ---------------------------------------------------------------------------------------------- //

} // END NAMESPACE: Glob

// ---------------------------------------------------------------------------------------------- //

typedef Glob::Template<char>             GlobA;
typedef Glob::Template<wchar_t>          GlobW;
#if VD_HAVE_ICU
typedef Glob::Template<UChar>            GlobU;
#endif

// ---------------------------------------------------------------------------------------------- //

#if defined(_UNICODE)
#   if VD_HAVE_ICU
#       define Glob                      GlobU
#   else
#       define Glob                      GlobW
#   endif
#else
#   define Glob                          GlobA
#endif

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif
