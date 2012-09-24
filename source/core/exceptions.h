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

#ifndef VD_CORE_EXCEPTIONS_INCLUDED
#define VD_CORE_EXCEPTIONS_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/logging.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class BadCastException : public std::bad_cast
{
public:
    BadCastException()
    { 
        // EMPTY!
    }

    virtual ~BadCastException() throw()
    { 
        // EMPTY!
    }
    
    virtual const char *what() const throw()
    {
        return "bad cast";
    }
};

class BadLexicalCastException : public std::bad_cast
{
public:
    BadLexicalCastException()
    { 
        // EMPTY!
    }

    virtual ~BadLexicalCastException() throw()
    { 
        // EMPTY!
    }
    
    virtual const char *what() const throw()
    {
        return "bad lexical cast";
    }
};

class EndOfStreamException : public std::runtime_error 
{
public:
    inline EndOfStreamException(
        const vd::string &str, size_t completed
    ) : 
        std::runtime_error(str), m_Bytes(completed) 
    { 
        // EMPTY!
    }

    virtual ~EndOfStreamException() throw()
    { 
        // EMPTY!
    }

    inline size_t GetBytesCompleted() const {
        return m_Bytes;
    }

    virtual const char *what() const throw()
    {
        return "end of stream";
    }
private:
    size_t m_Bytes;
};


// ============================================================================================== //

#if !defined(vdException)
#if defined(VD_NDEBUG) || defined(VD_RELEASE_BUILD)

// ============================================================================================== //

#define vdException(exception, fmt, ...)        ((void) 0)
#define vdGlobalException(exception, fmt, ...)  ((void) 0)

// ============================================================================================== //

#else

// ============================================================================================== //
    
/// Scope specific exception with a custom error message
#define vdException(exception, fmt, ...)  do {                                                      \
        vdLogError("Exception '%s' caught in %s:%i!", #exception, __FILE__, __LINE__);              \
        vdLogError(fmt, ## __VA_ARGS__);                                                            \
    } while (0)
    
/// Global exception with a custom error message
#define vdGlobalException(exception, fmt, ...)  do {                                                \
        vdLogGlobalError("Exception '%s' caught in %s:%i!", #exception, __FILE__, __LINE__);        \
        vdLogGlobalError(fmt, ## __VA_ARGS__);                                                      \
    } while (0)
    
    
#endif
#endif
    
// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif  // VD_CORE_EXCEPTIONS_INCLUDED

