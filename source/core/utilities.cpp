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

#include "core/utilities.h"
#include "core/memory.h"

#include <stdarg.h>

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

namespace Convert {
    
// ============================================================================================== //

vd::string 
ToString(vd::f32 v, vd::i32 precision) 
{ 
	std::ostringstream os; 
	os << std::setprecision(precision);
	os << v; 
	return os.str(); 
}

vd::string
ToString(vd::i32 v, vd::i32 padding, char fill)
{
    std::ostringstream ss;
    if(padding)
	    ss << std::setw( padding ) << std::setfill( fill ) << v;
    else
    	ss << v;
    	
    return ss.str();
}

vd::string
ToString(const char* fmt, ...)
{
    char tmp[512];
    va_list iterator;

#if defined(VD_TARGET_WINDOWS)

    va_start(iterator, fmt);
    size_t size = _vscprintf(fmt, iterator) + 1;

    if(size >= sizeof(tmp))
    {
        char* dest = VD_NEW_ARRAY(char, size);
        vsnprintf_s(dest, size, size - 1, fmt, iterator);
        va_end(iterator);
        vd::string result(dest);
        VD_DELETE_ARRAY(dest);
        return result;
    }

    vsnprintf_s(tmp, size, size - 1, fmt, iterator);
    va_end(iterator);

#else

    va_start(iterator, fmt);
    size_t size = vsnprintf(tmp, sizeof(tmp), fmt, iterator);
    va_end(iterator);
    if(size >= sizeof(tmp))
    {
        char* dest = VD_NEW_ARRAY( char, size + 1 );
        va_start(iterator, fmt);
        vsnprintf(dest, size + 1, fmt, iterator);
        va_end(iterator);

        vd::string result(dest);
        VD_DELETE_ARRAY(dest);
        return result;
    }

#endif

    return vd::string(tmp);
}

vd::string 
ToLowerCase(const vd::string& old)
{
	std::string str = old;
	for (std::string::iterator i = str.begin(); i != str.end(); ++i)
		*i = static_cast<char>(std::tolower(*i));
	return str;
}

vd::string 
ToUpperCase(const std::string& old)
{
	std::string str = old;
	for (std::string::iterator i = str.begin(); i != str.end(); ++i)
		*i = static_cast<char>(std::toupper(*i));
	return str;
}

vd::string 
FormatString( 
    const char* fmt, ...)
{
    char tmp[512] = {0};
    char *msg = tmp;
    va_list iterator;

#if defined(VD_TARGET_WINDOWS)

    va_start(iterator, fmt);
    size_t size = _vscprintf(fmt, iterator) + 1;
    if(size >= sizeof(tmp))
    {
        // Temp buffer overflow! -- dynamically allocate memory
        msg = VD_NEW_ARRAY(char, size + 1);
        Memory::SetBytes(msg, 0, size);
    }
    vsnprintf_s(msg, size, size - 1, fmt, iterator);
    va_end(iterator);

#else

    va_start(iterator, fmt);
    size_t size = vsnprintf(tmp, sizeof(tmp), fmt, iterator);
    va_end(iterator);
    if(size >= sizeof(tmp))
    {
        // Temp buffer overflow! -- dynamically allocate memory
        msg = VD_NEW_ARRAY(char, size + 1);
        Memory::SetBytes(msg, 0, size);
        va_start(iterator, fmt);
        vsnprintf(msg, size + 1, fmt, iterator);
        va_end(iterator);
    }

#endif

    vd::string result(msg);
    if(msg != tmp)
        VD_DELETE_ARRAY(msg);

    return result;
}
// ============================================================================================== //

} // END NAMESPACE: Convert

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //
