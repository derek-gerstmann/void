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

#include "vd.h"

// ============================================================================================== //

VD_NAMESPACE_BEGIN();

// ============================================================================================== //

// const uid uid::Zero;

// ============================================================================================== //

uid& uid::operator = ( const vd::string& from )
{
    char* next = 0;
#ifdef _MSC_VER
    m_Data[1] = ::_strtoui64( from.c_str(), &next, 16 );
#else
    m_Data[1] = ::strtoull( from.c_str(), &next, 16 );
#endif
    assert( next != from.c_str( ));
    if( *next == '\0' ) // short representation, high was 0
    {
        m_Data[0] = m_Data[1];
        m_Data[1] = 0;
    }
    else
    {
        assert( *next == ':' );
        ++next;
#ifdef _MSC_VER
        m_Data[0] = ::_strtoui64( next, 0, 16 );
#else
        m_Data[0] = ::strtoull( next, 0, 16 );
#endif
    }
    return *this;
}

// ============================================================================================== //

VD_NAMESPACE_END();

// ============================================================================================== //
