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

enum VD_Flags
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

/*! @brief Error return codes */
enum VD_Error
{
    VD_SUCCESS          =  0,
    VD_ERR_NOMATCH      =  1,
    VD_ERR_MEMORY       = -1,
    VD_ERR_FAILURE      = -2
};

// ---------------------------------------------------------------------------
// Platform dependent implementations

// if we aren't on Windows and we have ICU available, then enable ICU
// by default. Define this to 0 to intentially disable it.
#ifndef VD_HAVE_ICU
# if !defined(_WIN32) && defined(USTRING_H)
#   define VD_HAVE_ICU 1
# else
#   define VD_HAVE_ICU 0
# endif
#endif

// don't include this in documentation as it isn't relevant
#ifndef DOXYGEN

/*! @brief String manipulation functions. */
class GlobMethods
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

    // Note: char strlen returns number of bytes, not characters
    static size_t strlen(const char* s) { return ::strlen(s); }
    static size_t strlen(const wchar_t* s) { return ::wcslen(s); }
#if VD_HAVE_ICU
    static size_t strlen(const UChar* s) { return ::u_strlen(s); }
#endif

    static void strcpy_s(char* dst, size_t n, const char* src)
    {
        (void) n;
        vd_strcpy_s((vd_char*)dst, n, (const vd_char*)src);
    }
    static void strcpy_s(wchar_t* dst, size_t n, const wchar_t* src)
    {
# if __STDC_WANT_SECURE_LIB__
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

    static int strcmp(const char* s1, const char* s2)
    {
        return vd_strcmp((const vd_char*)s1, (const vd_char*)s2);
    }
    static int strcmp(const wchar_t* s1, const wchar_t* s2)
    {
        return ::wcscmp(s1, s2);
    }
#if VD_HAVE_ICU
    static int strcmp(const UChar* s1, const UChar* s2)
    {
        return ::u_strcmp(s1, s2);
    }
#endif

    static int strcasecmp(const char* s1, const char* s2)
    {
        return vd_strcasecmp((const vd_char*)s1, (const vd_char*)s2);
    }
#if _WIN32
    static int strcasecmp(const wchar_t* s1, const wchar_t* s2)
    {
        return ::_wcsicmp(s1, s2);
    }
#endif // _WIN32
#if VD_HAVE_ICU
    static int strcasecmp(const UChar* s1, const UChar* s2)
    {
        return u_strcasecmp(s1, s2, 0);
    }
#endif
};

enum VD_FileType
{
    VD_FILETYPE_INVALID,
    VD_FILETYPE_FILE,
    VD_FILETYPE_DIR
};

#ifdef _WIN32

#ifndef INVALID_FILE_ATTRIBUTES
# define INVALID_FILE_ATTRIBUTES    ((DWORD)-1)
#endif

#define VD_PATH_CHAR    '\\'

/*! @brief Windows glob implementation. */
template<class CharType>
struct GlobBase
{
    GlobBase() : m_hFind(INVALID_HANDLE_VALUE) { }

    int FindFirstFileS(const char* a_pszFileSpec, unsigned int)
    {
        m_hFind = FindFirstFileA(a_pszFileSpec, &m_oFindDataA);

        if(m_hFind != INVALID_HANDLE_VALUE)
        {
            return VD_SUCCESS;
        }

        DWORD dwErr = GetLastError();

        if(dwErr == ERROR_FILE_NOT_FOUND)
        {
            return VD_ERR_NOMATCH;
        }

        return VD_ERR_FAILURE;
    }
    int FindFirstFileS(const wchar_t* a_pszFileSpec, unsigned int)
    {
        m_hFind = FindFirstFileW(a_pszFileSpec, &m_oFindDataW);

        if(m_hFind != INVALID_HANDLE_VALUE)
        {
            return VD_SUCCESS;
        }

        DWORD dwErr = GetLastError();

        if(dwErr == ERROR_FILE_NOT_FOUND)
        {
            return VD_ERR_NOMATCH;
        }

        return VD_ERR_FAILURE;
    }

    bool FindNextFileS(char)
    {
        return FindNextFileA(m_hFind, &m_oFindDataA) != FALSE;
    }
    bool FindNextFileS(wchar_t)
    {
        return FindNextFileW(m_hFind, &m_oFindDataW) != FALSE;
    }

    void FindDone()
    {
        FindClose(m_hFind);
    }

    const char* GetFileNameS(char) const
    {
        return m_oFindDataA.cFileName;
    }
    const wchar_t* GetFileNameS(wchar_t) const
    {
        return m_oFindDataW.cFileName;
    }

    bool IsDirS(char) const
    {
        return GetFileTypeS(m_oFindDataA.dwFileAttributes) == VD_FILETYPE_DIR;
    }
    bool IsDirS(wchar_t) const
    {
        return GetFileTypeS(m_oFindDataW.dwFileAttributes) == VD_FILETYPE_DIR;
    }

    VD_FileType GetFileTypeS(const char* a_pszPath)
    {
        return GetFileTypeS(GetFileAttributesA(a_pszPath));
    }
    VD_FileType GetFileTypeS(const wchar_t* a_pszPath)
    {
        return GetFileTypeS(GetFileAttributesW(a_pszPath));
    }
    VD_FileType GetFileTypeS(DWORD a_dwAttribs) const
    {
        if(a_dwAttribs == INVALID_FILE_ATTRIBUTES)
        {
            return VD_FILETYPE_INVALID;
        }

        if(a_dwAttribs & FILE_ATTRIBUTE_DIRECTORY)
        {
            return VD_FILETYPE_DIR;
        }

        return VD_FILETYPE_FILE;
    }

private:
    HANDLE              m_hFind;
    WIN32_FIND_DATAA    m_oFindDataA;
    WIN32_FIND_DATAW    m_oFindDataW;
};

#else // !_WIN32

#define VD_PATH_CHAR    '/'

/*! @brief Unix glob implementation. */
template<class CharType>
struct GlobBase
{
    GlobBase()
    {
        memset(&m_glob, 0, sizeof(m_glob));
        m_uiCurr = (size_t) - 1;
    }

    ~GlobBase()
    {
        globfree(&m_glob);
    }

    void FilePrep()
    {
        m_bIsDir = false;
        size_t len = strlen(m_glob.gl_pathv[m_uiCurr]);

        if(m_glob.gl_pathv[m_uiCurr][len-1] == '/')
        {
            m_bIsDir = true;
            m_glob.gl_pathv[m_uiCurr][len-1] = 0;
        }
    }

    int FindFirstFileS(const char* a_pszFileSpec, unsigned int a_uiFlags)
    {
        int nFlags = GLOB_MARK | GLOB_NOSORT;

        if(a_uiFlags & VD_GLOB_ERR)    nFlags |= GLOB_ERR;

        if(a_uiFlags & VD_GLOB_TILDE)  nFlags |= GLOB_TILDE;

        int rc = glob(a_pszFileSpec, nFlags, NULL, &m_glob);

        if(rc == GLOB_NOSPACE) return VD_ERR_MEMORY;

        if(rc == GLOB_ABORTED) return VD_ERR_FAILURE;

        if(rc == GLOB_NOMATCH) return VD_ERR_NOMATCH;

        m_uiCurr = 0;
        FilePrep();
        return VD_SUCCESS;
    }

#if VD_HAVE_ICU
    int FindFirstFileS(const UChar* a_pszFileSpec, unsigned int a_uiFlags)
    {
        char buf[PATH_MAX] = { 0 };
        UErrorCode status = U_ZERO_ERROR;
        u_strToUTF8(buf, sizeof(buf), NULL, a_pszFileSpec, -1, &status);

        if(U_FAILURE(status)) return VD_ERR_FAILURE;

        return FindFirstFileS(buf, a_uiFlags);
    }
#endif

    bool FindNextFileS(char)
    {
        VD_ASSERT(m_uiCurr != (size_t) - 1);

        if(++m_uiCurr >= m_glob.gl_pathc)
        {
            return false;
        }

        FilePrep();
        return true;
    }

#if VD_HAVE_ICU
    bool FindNextFileS(UChar)
    {
        return FindNextFileS((char)0);
    }
#endif

    void FindDone()
    {
        globfree(&m_glob);
        memset(&m_glob, 0, sizeof(m_glob));
        m_uiCurr = (size_t) - 1;
    }

    const char* GetFileNameS(char) const
    {
        VD_ASSERT(m_uiCurr != (size_t) - 1);
        return m_glob.gl_pathv[m_uiCurr];
    }

#if VD_HAVE_ICU
    const UChar* GetFileNameS(UChar) const
    {
        const char* pszFile = GetFileNameS((char)0);

        if(!pszFile) return NULL;

        UErrorCode status = U_ZERO_ERROR;
        memset(m_szBuf, 0, sizeof(m_szBuf));
        u_strFromUTF8(m_szBuf, PATH_MAX, NULL, pszFile, -1, &status);

        if(U_FAILURE(status)) return NULL;

        return m_szBuf;
    }
#endif

    bool IsDirS(char) const
    {
        VD_ASSERT(m_uiCurr != (size_t) - 1);
        return m_bIsDir;
    }

#if VD_HAVE_ICU
    bool IsDirS(UChar) const
    {
        return IsDirS((char)0);
    }
#endif

    VD_FileType GetFileTypeS(const char* a_pszPath) const
    {
        struct stat sb;

        if(0 != stat(a_pszPath, &sb))
        {
            return VD_FILETYPE_INVALID;
        }

        if(S_ISDIR(sb.st_mode))
        {
            return VD_FILETYPE_DIR;
        }

        if(S_ISREG(sb.st_mode))
        {
            return VD_FILETYPE_FILE;
        }

        return VD_FILETYPE_INVALID;
    }

#if VD_HAVE_ICU
    VD_FileType GetFileTypeS(const UChar* a_pszPath) const
    {
        char buf[PATH_MAX] = { 0 };
        UErrorCode status = U_ZERO_ERROR;
        u_strToUTF8(buf, sizeof(buf), NULL, a_pszPath, -1, &status);

        if(U_FAILURE(status)) return VD_FILETYPE_INVALID;

        return GetFileTypeS(buf);
    }
#endif

private:
    glob_t  m_glob;
    size_t  m_uiCurr;
    bool    m_bIsDir;
#if VD_HAVE_ICU
    mutable UChar m_szBuf[PATH_MAX];
#endif
};

#endif // _WIN32

#endif // DOXYGEN

// ---------------------------------------------------------------------------
//                              MAIN TEMPLATE CLASS
// ---------------------------------------------------------------------------

/*! @brief Implementation of the SimpleGlob class */
template<class CharType>
class GlobTemplate : private GlobBase<CharType>
{
public:
    /*! @brief Initialize the class.

        @param a_uiFlags            Combination of VD_GLOB flags.
        @param a_nReservedSlots     Number of slots in the argv array that
            should be reserved. In the returned array these slots
            argv[0] ... argv[a_nReservedSlots-1] will be left empty for
            the caller to fill in.
     */
    GlobTemplate(unsigned int a_uiFlags = 0, int a_nReservedSlots = 0);

    /*! @brief Deallocate all memory buffers. */
    ~GlobTemplate();

    /*! @brief Initialize (or re-initialize) the class in preparation for
        adding new filespecs.

        All existing files are cleared. Note that allocated memory is only
        deallocated at object destruction.

        @param a_uiFlags            Combination of VD_GLOB flags.
        @param a_nReservedSlots     Number of slots in the argv array that
            should be reserved. In the returned array these slots
            argv[0] ... argv[a_nReservedSlots-1] will be left empty for
            the caller to fill in.
     */
    int Init(unsigned int a_uiFlags = 0, int a_nReservedSlots = 0);

    /*! @brief Add a new filespec to the glob.

        The filesystem will be immediately scanned for all matching files and
        directories and they will be added to the glob.

        @param a_pszFileSpec    Filespec to add to the glob.

        @return VD_SUCCESS      Matching files were added to the glob.
        @return VD_ERR_NOMATCH  Nothing matched the pattern. To ignore this
                                error compare return value to >= VD_SUCCESS.
        @return VD_ERR_MEMORY   Out of memory failure.
        @return VD_ERR_FAILURE  General failure.
     */
    int Add(const CharType* a_pszFileSpec);

    /*! @brief Add an array of filespec to the glob.

        The filesystem will be immediately scanned for all matching files and
        directories in each filespec and they will be added to the glob.

        @param a_nCount         Number of filespec in the array.
        @param a_rgpszFileSpec  Array of filespec to add to the glob.

        @return VD_SUCCESS      Matching files were added to the glob.
        @return VD_ERR_NOMATCH  Nothing matched the pattern. To ignore this
                                error compare return value to >= VD_SUCCESS.
        @return VD_ERR_MEMORY   Out of memory failure.
        @return VD_ERR_FAILURE  General failure.
     */
    int Add(int a_nCount, const CharType* const* a_rgpszFileSpec);

    /*! @brief Return the number of files in the argv array.
     */
    inline int FileCount() const { return m_nArgsLen; }

    /*! @brief Return the full argv array. */
    inline CharType** Files()
    {
        SetArgvArrayType(POINTERS);
        return m_rgpArgs;
    }

    /*! @brief Return the a single file. */
    inline CharType* File(int n)
    {
        VD_ASSERT(n >= 0 && n < m_nArgsLen);
        return Files()[n];
    }

private:
    GlobTemplate(const GlobTemplate&);  // disabled
    GlobTemplate& operator=(const GlobTemplate&);   // disabled

    /*! @brief The argv array has it's members stored as either an offset into
        the string buffer, or as pointers to their string in the buffer. The
        offsets are used because if the string buffer is dynamically resized,
        all pointers into that buffer would become invalid.
     */
    enum ARG_ARRAY_TYPE { OFFSETS, POINTERS };

    /*! @brief Change the type of data stored in the argv array. */
    void SetArgvArrayType(ARG_ARRAY_TYPE a_nNewType);

    /*! @brief Add a filename to the array if it passes all requirements. */
    int AppendName(const CharType* a_pszFileName, bool a_bIsDir);

    /*! @brief Grow the argv array to the required size. */
    bool GrowArgvArray(int a_nNewLen);

    /*! @brief Grow the string buffer to the required size. */
    bool GrowStringBuffer(size_t a_uiMinSize);

    /*! @brief Compare two (possible NULL) strings */
    static int fileSortCompare(const void* a1, const void* a2);

private:
    unsigned int        m_uiFlags;
    ARG_ARRAY_TYPE      m_nArgArrayType;    //!< argv is indexes or pointers
    CharType**            m_rgpArgs;          //!< argv
    int                 m_nReservedSlots;   //!< # client slots in argv array
    int                 m_nArgsSize;        //!< allocated size of array
    int                 m_nArgsLen;         //!< used length
    CharType*             m_pBuffer;          //!< argv string buffer
    size_t              m_uiBufferSize;     //!< allocated size of buffer
    size_t              m_uiBufferLen;      //!< used length of buffer
    CharType              m_szPathPrefix[MAX_PATH]; //!< wildcard path prefix
};

// ---------------------------------------------------------------------------
//                                  IMPLEMENTATION
// ---------------------------------------------------------------------------

template<class CharType>
GlobTemplate<CharType>::GlobTemplate(
    unsigned int    a_uiFlags,
    int             a_nReservedSlots
)
{
    m_rgpArgs           = NULL;
    m_nArgsSize         = 0;
    m_pBuffer           = NULL;
    m_uiBufferSize      = 0;

    Init(a_uiFlags, a_nReservedSlots);
}

template<class CharType>
GlobTemplate<CharType>::~GlobTemplate()
{
    if(m_rgpArgs) free(m_rgpArgs);

    if(m_pBuffer) free(m_pBuffer);
}

template<class CharType>
int
GlobTemplate<CharType>::Init(
    unsigned int    a_uiFlags,
    int             a_nReservedSlots
)
{
    m_nArgArrayType     = POINTERS;
    m_uiFlags           = a_uiFlags;
    m_nArgsLen          = a_nReservedSlots;
    m_nReservedSlots    = a_nReservedSlots;
    m_uiBufferLen       = 0;

    if(m_nReservedSlots > 0)
    {
        if(!GrowArgvArray(m_nReservedSlots))
        {
            return VD_ERR_MEMORY;
        }

        for(int n = 0; n < m_nReservedSlots; ++n)
        {
            m_rgpArgs[n] = NULL;
        }
    }

    return VD_SUCCESS;
}

template<class CharType>
int
GlobTemplate<CharType>::Add(
    const CharType* a_pszFileSpec
)
{
#ifdef _WIN32
    // Windows FindFirst/FindNext recognizes forward slash as the same as
    // backward slash and follows the directories. We need to do the same
    // when calculating the prefix and when we have no wildcards.
    CharType szFileSpec[MAX_PATH];
    GlobMethods::strcpy_s(szFileSpec, MAX_PATH, a_pszFileSpec);
    const CharType* pszPath = GlobMethods::strchr(szFileSpec, '/');

    while(pszPath)
    {
        szFileSpec[pszPath - szFileSpec] = VD_PATH_CHAR;
        pszPath = GlobMethods::strchr(pszPath + 1, '/');
    }

    a_pszFileSpec = szFileSpec;
#endif

    // if this doesn't contain wildcards then we can just add it directly
    m_szPathPrefix[0] = 0;

    if(!GlobMethods::strchr(a_pszFileSpec, '*') &&
            !GlobMethods::strchr(a_pszFileSpec, '?'))
    {
        VD_FileType nType = GetFileTypeS(a_pszFileSpec);

        if(nType == VD_FILETYPE_INVALID)
        {
            if(m_uiFlags & VD_GLOB_NOCHECK)
            {
                return AppendName(a_pszFileSpec, false);
            }

            return VD_ERR_NOMATCH;
        }

        return AppendName(a_pszFileSpec, nType == VD_FILETYPE_DIR);
    }

#ifdef _WIN32
    // Windows doesn't return the directory with the filename, so we need to
    // extract the path from the search string ourselves and prefix it to the
    // filename we get back.
    const CharType* pszFilename =
        GlobMethods::strrchr(a_pszFileSpec, VD_PATH_CHAR);

    if(pszFilename)
    {
        GlobMethods::strcpy_s(m_szPathPrefix, MAX_PATH, a_pszFileSpec);
        m_szPathPrefix[pszFilename - a_pszFileSpec + 1] = 0;
    }

#endif

    // search for the first match on the file
    int rc = FindFirstFileS(a_pszFileSpec, m_uiFlags);

    if(rc != VD_SUCCESS)
    {
        if(rc == VD_ERR_NOMATCH && (m_uiFlags & VD_GLOB_NOCHECK))
        {
            int ok = AppendName(a_pszFileSpec, false);

            if(ok != VD_SUCCESS) rc = ok;
        }

        return rc;
    }

    // add it and find all subsequent matches
    int nError, nStartLen = m_nArgsLen;
    bool bSuccess;

    do
    {
        nError = AppendName(GetFileNameS((CharType)0), IsDirS((CharType)0));
        bSuccess = FindNextFileS((CharType)0);
    }
    while(nError == VD_SUCCESS && bSuccess);

    GlobBase<CharType>::FindDone();

    // sort these files if required
    if(m_nArgsLen > nStartLen && !(m_uiFlags & VD_GLOB_NOSORT))
    {
        if(m_uiFlags & VD_GLOB_FULLSORT)
        {
            nStartLen = m_nReservedSlots;
        }

        SetArgvArrayType(POINTERS);
        qsort(
            m_rgpArgs + nStartLen,
            m_nArgsLen - nStartLen,
            sizeof(m_rgpArgs[0]), fileSortCompare);
    }

    return nError;
}

template<class CharType>
int
GlobTemplate<CharType>::Add(
    int                     a_nCount,
    const CharType* const*   a_rgpszFileSpec
)
{
    int nResult;

    for(int n = 0; n < a_nCount; ++n)
    {
        nResult = Add(a_rgpszFileSpec[n]);

        if(nResult != VD_SUCCESS)
        {
            return nResult;
        }
    }

    return VD_SUCCESS;
}

template<class CharType>
int
GlobTemplate<CharType>::AppendName(
    const CharType*   a_pszFileName,
    bool            a_bIsDir
)
{
    // we need the argv array as offsets in case we resize it
    SetArgvArrayType(OFFSETS);

    // check for special cases which cause us to ignore this entry
    if((m_uiFlags & VD_GLOB_ONLYDIR) && !a_bIsDir)
    {
        return VD_SUCCESS;
    }

    if((m_uiFlags & VD_GLOB_ONLYFILE) && a_bIsDir)
    {
        return VD_SUCCESS;
    }

    if((m_uiFlags & VD_GLOB_NODOT) && a_bIsDir)
    {
        if(a_pszFileName[0] == '.')
        {
            if(a_pszFileName[1] == '\0')
            {
                return VD_SUCCESS;
            }

            if(a_pszFileName[1] == '.' && a_pszFileName[2] == '\0')
            {
                return VD_SUCCESS;
            }
        }
    }

    // ensure that we have enough room in the argv array
    if(!GrowArgvArray(m_nArgsLen + 1))
    {
        return VD_ERR_MEMORY;
    }

    // ensure that we have enough room in the string buffer (+1 for null)
    size_t uiPrefixLen = GlobMethods::strlen(m_szPathPrefix);
    size_t uiLen = uiPrefixLen + GlobMethods::strlen(a_pszFileName) + 1;

    if(a_bIsDir && (m_uiFlags & VD_GLOB_MARK) == VD_GLOB_MARK)
    {
        ++uiLen;    // need space for the backslash
    }

    if(!GrowStringBuffer(m_uiBufferLen + uiLen))
    {
        return VD_ERR_MEMORY;
    }

    // add this entry. m_uiBufferLen is offset from beginning of buffer.
    m_rgpArgs[m_nArgsLen++] = (CharType*)m_uiBufferLen;
    GlobMethods::strcpy_s(m_pBuffer + m_uiBufferLen,
                          m_uiBufferSize - m_uiBufferLen, m_szPathPrefix);
    GlobMethods::strcpy_s(m_pBuffer + m_uiBufferLen + uiPrefixLen,
                          m_uiBufferSize - m_uiBufferLen - uiPrefixLen, a_pszFileName);
    m_uiBufferLen += uiLen;

    // add the directory slash if desired
    if(a_bIsDir && (m_uiFlags & VD_GLOB_MARK) == VD_GLOB_MARK)
    {
        const static CharType szDirSlash[] = { VD_PATH_CHAR, 0 };
        GlobMethods::strcpy_s(m_pBuffer + m_uiBufferLen - 2,
                              m_uiBufferSize - (m_uiBufferLen - 2), szDirSlash);
    }

    return VD_SUCCESS;
}

template<class CharType>
void
GlobTemplate<CharType>::SetArgvArrayType(
    ARG_ARRAY_TYPE  a_nNewType
)
{
    if(m_nArgArrayType == a_nNewType) return;

    if(a_nNewType == POINTERS)
    {
        VD_ASSERT(m_nArgArrayType == OFFSETS);

        for(int n = 0; n < m_nArgsLen; ++n)
        {
            m_rgpArgs[n] = (m_rgpArgs[n] == (CharType*) - 1) ?
                           NULL : m_pBuffer + (size_t) m_rgpArgs[n];
        }
    }
    else
    {
        VD_ASSERT(a_nNewType == OFFSETS);
        VD_ASSERT(m_nArgArrayType == POINTERS);

        for(int n = 0; n < m_nArgsLen; ++n)
        {
            m_rgpArgs[n] = (m_rgpArgs[n] == NULL) ?
                           (CharType*) - 1 : (CharType*)(m_rgpArgs[n] - m_pBuffer);
        }
    }

    m_nArgArrayType = a_nNewType;
}

template<class CharType>
bool
GlobTemplate<CharType>::GrowArgvArray(
    int a_nNewLen
)
{
    if(a_nNewLen >= m_nArgsSize)
    {
        static const int VD_ARGV_INITIAL_SIZE = 32;
        int nNewSize = (m_nArgsSize > 0) ?
                       m_nArgsSize * 2 : VD_ARGV_INITIAL_SIZE;

        while(a_nNewLen >= nNewSize)
        {
            nNewSize *= 2;
        }

        void* pNewBuffer = realloc(m_rgpArgs, nNewSize * sizeof(CharType*));

        if(!pNewBuffer) return false;

        m_nArgsSize = nNewSize;
        m_rgpArgs = (CharType**) pNewBuffer;
    }

    return true;
}

template<class CharType>
bool
GlobTemplate<CharType>::GrowStringBuffer(
    size_t a_uiMinSize
)
{
    if(a_uiMinSize >= m_uiBufferSize)
    {
        static const int VD_BUFFER_INITIAL_SIZE = 1024;
        size_t uiNewSize = (m_uiBufferSize > 0) ?
                           m_uiBufferSize * 2 : VD_BUFFER_INITIAL_SIZE;

        while(a_uiMinSize >= uiNewSize)
        {
            uiNewSize *= 2;
        }

        void* pNewBuffer = realloc(m_pBuffer, uiNewSize * sizeof(CharType));

        if(!pNewBuffer) return false;

        m_uiBufferSize = uiNewSize;
        m_pBuffer = (CharType*) pNewBuffer;
    }

    return true;
}

template<class CharType>
int
GlobTemplate<CharType>::fileSortCompare(
    const void* a1,
    const void* a2
)
{
    const CharType* s1 = *(const CharType**)a1;
    const CharType* s2 = *(const CharType**)a2;

    if(s1 && s2)
    {
        return GlobMethods::strcasecmp(s1, s2);
    }

    // NULL sorts first
    return s1 == s2 ? 0 : (s1 ? 1 : -1);
}

// ---------------------------------------------------------------------------
//                                  TYPE DEFINITIONS
// ---------------------------------------------------------------------------

typedef GlobTemplate<char>    GlobA;
typedef GlobTemplate<wchar_t> GlobW;

#if VD_HAVE_ICU
typedef GlobTemplate<UChar> GlobU;
#endif

#ifdef _UNICODE
# if VD_HAVE_ICU
#  define Glob GlobU
# else
#  define Glob GlobW
# endif
#else
# define Glob GlobA
#endif

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //
