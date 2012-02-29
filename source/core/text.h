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

#ifndef VD_CORE_TEXT_INCLUDED
#define VD_CORE_TEXT_INCLUDED

#include "core/core.h"
#include "core/object.h"

#include <string>

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //
	
class Text : public Object
{
public:

	Text(){}

	Text( const vd::dstr& text )
	{
		*this = text;
	}

	Text( const vd::cstr text )
	{
		*this = vd::dstr( text );
	}

	Text( const vd::cwstr text )
	{
		*this = vd::dwstr( text );
	}

	Text( const vd::dwstr& text )
	{
		*this = text;
	}
	
	void operator = ( const vd::dstr& text )
	{
		m_Data = ToUnicode( text );
	}

	void operator = ( const vd::dwstr& text )
	{
		m_Data = text;
	}

	bool operator == ( const Text& text ) const
	{
		return m_Data == text.m_Data;
	}

	vd::dstr ToString() const
	{
		return ToString( m_Data );
	}

	const vd::dwstr& ToUnicode() const
	{
		return m_Data;
	}

	vd::dwstr m_Data;
};

#ifdef _MSC_VER
	#pragma warning( push )
	#pragma warning( disable : 4996 )
#endif

inline vd::dstr 
ToString( const vd::dwstr& text )
{
	if ( !text.length() ) return "";
	
	vd::dstr temp(text.length(), (char)0);
	
	std::use_facet< std::ctype<vd::wchar> >(std::locale()). \
		narrow(&text[0], &text[0] + text.length(), ' ', &temp[0]);
	
	return temp;
}

inline vd::dwstr 
ToUnicode( const vd::dstr& text )
{
	if ( !text.length() ) return L"";
	
	vd::dwstr temp(text.length(), (vd::wchar)0);
	
	std::use_facet< std::ctype<vd::wchar> >(std::locale()). \
		widen(&text[0], &text[0] + text.length(), &temp[0]);
	
	return temp; 
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif
