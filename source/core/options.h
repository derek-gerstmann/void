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

#ifndef VD_CORE_OPTIONS_INCLUDED
#define VD_CORE_OPTIONS_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/memory.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

/*
    Option implementation was initially based on SimpleOpt.  License follows:

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

#if defined(VD_MAX_ARGS)
#define VD_FIXED_BUFFER_SIZE   VD_MAX_ARGS
#else
#define VD_FIXED_BUFFER_SIZE   128
#endif

namespace Option {

VD_DECLARE_ENUM(Format,
    None,
    Separate,
    Combined,
    Optional,
    Multiple);

VD_DECLARE_ENUM(Result,
    Success,
    Unregistered,
    Missing,
    InvalidFormat,
    InvalidData,
    Ambiguous);

enum Flags
{
    VD_OPTION_EXACT       = 0x0001,
    VD_OPTION_NOSLASH     = 0x0002,
    VD_OPTION_SHORTARG    = 0x0004,
    VD_OPTION_CLUMP       = 0x0008,
    VD_OPTION_USEALL      = 0x0010,
    VD_OPTION_NOERR       = 0x0020,
    VD_OPTION_PEDANTIC    = 0x0040,
    VD_OPTION_ICASE_SHORT = 0x0100,
    VD_OPTION_ICASE_LONG  = 0x0200,
    VD_OPTION_ICASE_WORD  = 0x0400,
    VD_OPTION_ICASE       = 0x0700
};

#define VD_END_OF_OPTIONS   { -1, NULL, Foramt::None }

template<class CharType>
class Template
{
public:

    struct Entry
    {
        int Id;
        const CharType* Arg;
        Option::Format::Value Format;
    };

    Template()
        : m_Buffer(NULL)
    {
        Setup(0, NULL, NULL, 0);
    }

    Template(
        int argc,
        CharType* argv[],
        const Entry* entries,
        int flags = 0
    ) : 
        m_Buffer(NULL)
    {
        Setup(argc, argv, entries, flags);
    }

    ~Template() 
    {
#if defined(VD_MAX_ARGS)
        if(m_Buffer) VD_DELETE_ARRAY(m_Buffer);
#endif
    }

    bool Setup(
        int argc,
        CharType* argv[],
        const Entry* options,
        int flags = 0
    );

    VD_INLINE void 
    SetOptions(const Entry* options)
    {
        m_Entries = options;
    }

    VD_INLINE void 
    SetFlags(int flags) { m_Flags = flags; }

    VD_INLINE bool 
    HasFlag(int flag) const
    {
        return (m_Flags & flag) == flag;
    }

    bool Next();
    void Stop();

    VD_INLINE Result::Value 
    GetLastError() const  { return m_LastError; }
    
    VD_INLINE int 
    GetOptionId() const { return m_OptId; }
    
    VD_INLINE const CharType* 
    GetOptionText() const { return m_OptText; }
    
    VD_INLINE CharType* 
    GetOptionArg() const { return m_OptArg; }
    
    CharType** 
    GetMultiArg(int n);
    
    VD_INLINE int 
    GetFileCount() const { return m_ArgCount - m_LastArg; }

    VD_INLINE CharType* 
    GetFile(int n) const 
    {
        vdGlobalAssert(n >= 0 && n < GetFileCount());
        return m_ArgData[m_LastArg + n];
    }

    VD_INLINE CharType** 
    GetFiles() const 
    { 
        return &m_ArgData[m_LastArg]; 
    }

private:

    VD_DISABLE_COPY_CONSTRUCTORS(Template);

    CharType PrepareArg(CharType* a_pszString) const;
    
    bool NextClumped();
    
    void ShuffleArg(int index, int count);

    Result LookupOption(const CharType* option, int* index) const;

    int CalcMatch(const CharType* source, const CharType* match) const;

    VD_INLINE 
    CharType* FindEquals(CharType* s) const
    {
        while(*s && *s != (CharType)'=') ++s;
        return *s ? s : NULL;
    }

    bool IsEqual(CharType left, CharType right, int arg_type) const;

    VD_INLINE void 
    Copy(CharType** dst, CharType** src, int count) const
    {
#if defined(VD_MAX_ARGS)
        while(count-- > 0) *dst++ = *src++;
#else
        Memory::MemCopy(dst, src, count * sizeof(CharType*));
#endif
    }

private:
    const Entry*    m_Entries;
    int             m_Flags;
    int             m_OptIndex;
    int             m_OptId;
    int             m_OptNext; 
    int             m_LastArg;
    int             m_ArgCount;
    CharType**      m_ArgData;
    const CharType* m_OptText; 
    CharType*       m_OptArg;
    CharType*       m_Clump;
    CharType        m_Short[3]; 
    Result          m_LastError;
    CharType**      m_Buffer;  
};

template<class CharType>
bool Template<CharType>::Setup(
    int             argc,
    CharType*       argv[],
    const Entry*    entries,
    int             flags)
{
    m_ArgCount = argc;
    m_LastArg = argc;
    m_ArgData = argv;
    m_Entries = entries;
    m_LastError = Result::Success;
    m_OptIndex = 0;
    m_OptId = -1;
    m_OptText = NULL;
    m_OptArg = NULL;
    m_OptNext = (flags & VD_OPTION_USEALL) ? 0 : 1;
    m_Short[0] = (CharType)'-';
    m_Short[2] = (CharType)'\0';
    m_Flags = flags;
    m_Clump = NULL;

#if defined(VD_MAX_ARGS)

    if(m_ArgCount > VD_MAX_ARGS)
    {
        m_LastError = Result::InvalidData;
        m_LastArg = 0;
        return false;
    }

#else

    if(m_Buffer)
    {
        VD_DELETE_ARRAY(m_Buffer);
    }

    if(m_ArgCount > VD_FIXED_BUFFER_SIZE)
    {
        m_Buffer = VD_NEW_ARRAY(CharType*, m_ArgCount);
        if(!m_Buffer)
        {
            return false;
        }
    }

#endif

    return true;
}

template<class CharType>
bool Template<CharType>::Next()
{
#if defined(VD_MAX_ARGS)

    if(m_ArgCount > VD_MAX_ARGS)
    {
        vdGlobalAssert(!"Too many args! Check the return value of Setup()!");
        return false;
    }

#endif

    if(m_Clump && *m_Clump)
    {
        bool valid = NextClumped();
        while(*m_Clump && !valid && HasFlag(VD_OPTION_NOERR))
        {
            valid = NextClumped();
        }
        if(valid || !HasFlag(VD_OPTION_NOERR))
        {
            return true;
        }
    }

    vdGlobalAssert(!m_Clump || !*m_Clump);

    m_Clump = NULL;
    m_OptIndex = m_OptNext;
    m_OptId = -1;
    m_OptText = NULL;
    m_OptArg = NULL;
    m_LastError = Result::Success;

    CharType cFirst;
    int table_index = -1;
    int opt_index = m_OptIndex;
    Result found = Result::Invalid;
    while(table_index < 0 && opt_index < m_LastArg)
    {
        CharType* arg = m_ArgData[opt_index];
        m_OptArg  = NULL;

        cFirst = PrepareArg(arg);
        if(arg[0] == (CharType)'-')
        {
            m_OptArg = FindEquals(arg);
            if(m_OptArg)
            {
                *m_OptArg++ = (CharType)'\0';
            }
        }

        found = LookupOption(arg, &table_index);
        if(table_index < 0
                && !m_OptArg
                && arg[0] == (CharType)'-'
                && arg[1]
                && arg[1] != (CharType)'-'
                && arg[2])
        {
            if(HasFlag(VD_OPTION_SHORTARG))
            {
                m_Short[1] = arg[1];
                int index = 0;
                found = LookupOption(m_Short, &index);

                if((index >= 0) && 
                   ((m_Entries[index].Format == Format::Combined ||
                    m_Entries[index].Format == Format::Optional)))
                {
                    m_OptArg = &arg[2];
                    arg = m_Short;
                    table_index = index;
                }
            }

            if(table_index < 0 && HasFlag(VD_OPTION_CLUMP))
            {
                m_Clump = &arg[1];
                ++m_OptNext;
                if(opt_index > m_OptIndex)
                {
                    ShuffleArg(m_OptIndex, opt_index - m_OptIndex);
                }

                return Next();
            }
        }

        if(table_index < 0)
        {
            if(!HasFlag(VD_OPTION_NOERR) && arg[0] == (CharType)'-')
            {
                m_OptText = arg;
                break;
            }

            arg[0] = cFirst;
            ++opt_index;

            if(m_OptArg)
            {
                *(--m_OptArg) = (CharType)'=';
            }
        }
    }

    if(opt_index >= m_LastArg)
    {
        if(opt_index > m_OptIndex)
        {
            ShuffleArg(m_OptIndex, opt_index - m_OptIndex);
        }

        return false;
    }

    ++m_OptNext;

    Format::Value format = Format::None;
    if(table_index < 0)
    {
        m_LastError = found;
    }
    else
    {
        m_OptId = m_Entries[table_index].Id;
        m_OptText = m_Entries[table_index].Arg;
        format = m_Entries[table_index].Format;

        switch(format)
        {
            case Format::None:
            {
                if(m_OptArg)
                {
                    m_LastError = Result::InvalidData;
                }
                break;
            }
            case Format::Separate:
            {
                if(m_OptArg)
                {
                    if(HasFlag(VD_OPTION_PEDANTIC))
                    {
                        m_LastError = Result::InvalidData;
                    }
                }
                break;
            }
            case Format::Combined:
            {
                if(!m_OptArg)
                {
                    m_LastError = Result::Missing;
                }
                break;
            }
            case Format::Optional:
                break;

            case Format::Multiple:
                break;
            default:
                break;
        }
    }

    if(opt_index > m_OptIndex)
    {
        ShuffleArg(m_OptIndex, opt_index - m_OptIndex);
    }

    if(format == Format::Separate && !m_OptArg && m_LastError == Result::Success)
    {
        CharType** args = GetMultiArg(1);
        if(args)
        {
            m_OptArg = *args;
        }
    }

    return true;
}

template<class CharType>
void Template<CharType>::Stop()
{
    if(m_OptNext < m_LastArg)
    {
        ShuffleArg(m_OptNext, m_LastArg - m_OptNext);
    }
}

template<class CharType>
CharType Template<CharType>::PrepareArg(
    CharType* arg) const
{

#ifdef _WIN32
    if(!HasFlag(VD_OPTION_NOSLASH)
            && arg[0] == (CharType)'/'
            && arg[1]
            && arg[1] != (CharType)'-')
    {
        arg[0] = (CharType)'-';
        return (CharType)'/';
    }

#endif
    return arg[0];
}

template<class CharType>
bool Template<CharType>::NextClumped()
{
    m_Short[1] = *m_Clump++;
    m_OptId = -1;
    m_OptText = NULL;
    m_OptArg  = NULL;
    m_LastError = Result::Success;

    int save_flags = m_Flags;
    int table_index = 0;

    m_Flags = VD_OPTION_EXACT;
    Result found = LookupOption(m_Short, &table_index);
    m_Flags = save_flags;
    if(table_index < 0)
    {
        m_LastError = found;
        return false;
    }

    m_OptText = m_Entries[table_index].arg;
    Format::Value format = m_Entries[table_index].Format;

    if(format == Format::None)
    {
        m_OptId = m_Entries[table_index].nId;
        return true;
    }

    if(format == Format::Combined && *m_Clump)
    {
        m_OptId = m_Entries[table_index].nId;
        m_OptArg = m_Clump;
        while(*m_Clump) ++m_Clump; 
        return true;
    }

    m_LastError = Result::Missing;
    return true;
}

template<class CharType>
void Template<CharType>::ShuffleArg(
    int index, int count)
{
    CharType* FixedBuffer[VD_FIXED_BUFFER_SIZE];
    CharType** buffer = m_Buffer ? m_Buffer : FixedBuffer;
    int end = m_ArgCount - index - count;
    Copy(buffer, m_ArgData + index, count);
    Copy(m_ArgData + index, m_ArgData + index + count, end);
    Copy(m_ArgData + index + end, buffer, count);
    m_LastArg -= count;
}

template<class CharType>
Result Template<CharType>::LookupOption(
    const CharType* option, int* match) const
{
    int best_match = -1;    // index of best match so far
    int best_length = 0;  // matching characters of best match
    int last_length = 0;  // matching characters of last best match

    for(int n = 0; m_Entries[n].Id >= 0; ++n)
    {
        vdGlobalAssert(m_Entries[n].Arg[0] != (CharType)'/');
        int match_length = FindMatch(m_Entries[n].Arg, option);
        if(match_length == -1)
        {
            *match = n;
            return Result::Success;
        }

        if(match_length > 0 && match_length >= best_length)
        {
            last_length = best_length;
            best_length = match_length;
            best_match = n;
            *match = n;
        }
    }

    if(HasFlag(VD_OPTION_EXACT) || best_match == -1)
    {
        return Result::InvalidData;
    }

    return (best_length > last_length) ? Result::Success : Result::Ambiguous;
}

template<class CharType>
int Template<CharType>::CalcMatch(
    const CharType*   source,
    const CharType*   match ) const
{
    if(!source || !match)
    {
        return 0;
    }

    int arg_type = VD_OPTION_ICASE_LONG;
    if(source[0] != '-')
    {
        arg_type = VD_OPTION_ICASE_WORD;
    }
    else if(source[1] != '-' && !source[2])
    {
        arg_type = VD_OPTION_ICASE_SHORT;
    }

    while(*source == (CharType)'-' && *source == *match)
    {
        ++source;
        ++match;
    }

    if(*source == (CharType)'-' || *source == (CharType)'-')
    {
        return 0;
    }

    int length = 0;
    while(*source && IsEqual(*source, *match, arg_type))
    {
        ++source;
        ++match;
        ++length;
    }

    if(!*source)
    {
        if(!*match)
        {
            return -1;
        }
        return 0;
    }

    if(*match)
    {
        return 0;
    }

    return length;
}

template<class CharType>
bool Template<CharType>::IsEqual(
    CharType left, CharType right, int type) const
{
    if(m_Flags & type)
    {
        if(left  >= 'A' && left  <= 'Z') left  += 'a' - 'A';
        if(right >= 'A' && right <= 'Z') right += 'a' - 'A';
    }
    return left == right;
}

template<class CharType>
CharType** Template<CharType>::GetMultiArg(
    int count)
{
    if(m_OptNext + count > m_LastArg)
    {
        m_LastError = Result::Missing;
        return NULL;
    }

    CharType** args = &m_ArgData[m_OptNext];
    if(!HasFlag(VD_OPTION_NOERR))
    {
        for(int n = 0; n < count; ++n)
        {
            CharType ch = PrepareArg(args[n]);
            if(args[n][0] == (CharType)'-')
            {
                args[n][0] = ch;
                m_LastError = Result::InvalidData;
                return NULL;
            }
            args[n][0] = ch;
        }
    }
    m_OptNext += count;
    return args;
}

// ---------------------------------------------------------------------------------------------- //

} // END NAMESPACE: Option

// ---------------------------------------------------------------------------------------------- //
// Specific Types
// ---------------------------------------------------------------------------------------------- //

typedef Option::Template<char>    OptionA;
typedef Option::Template<wchar_t> OptionW;

#if defined(_UNICODE)
#define Option                    OptionW
#else
#define Option                    OptionA
#endif

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_OPTIONS_INCLUDED

