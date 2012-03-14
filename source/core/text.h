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

#include "vd.h"
#include "core/core.h"
#include "core/object.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

namespace Text 
{

// ============================================================================================== //

#ifdef _MSC_VER
	#pragma warning( push )
	#pragma warning( disable : 4996 )
#endif

// ============================================================================================== //

inline vd::ascii 
ToAscii( const vd::unicode& text )
{
	if ( !text.length() ) return "";
	
	vd::ascii temp(text.length(), (char)0);
	
	std::use_facet< std::ctype<vd::wchar> >(std::locale()). \
		narrow(&text[0], &text[0] + text.length(), ' ', &temp[0]);
	
	return temp;
}

inline vd::unicode 
ToUnicode( const vd::ascii& text )
{
	if ( !text.length() ) return L"";
	
	vd::unicode temp(text.length(), (vd::wchar)0);
	
	std::use_facet< std::ctype<vd::wchar> >(std::locale()). \
		widen(&text[0], &text[0] + text.length(), &temp[0]);
	
	return temp; 
}

// ============================================================================================== //

#ifdef _MSC_VER
#pragma warning( pop )
#endif

// ============================================================================================== //

class Buffer
{
public:

	VD_FORCE_INLINE 
	Buffer(){}

	VD_FORCE_INLINE 
	Buffer( const vd::cstr text )
	{
		vd::ascii tmp(text);
		*this = Text::ToUnicode(tmp);
	}

	VD_FORCE_INLINE 
	Buffer( const vd::cwstr text )
	{
		*this = vd::unicode( text );
	}

	VD_FORCE_INLINE 
	Buffer( const vd::unicode& text )
	{
		*this = text;
	}

	VD_FORCE_INLINE 
	Buffer( const vd::ascii& text )
	{
		*this = Text::ToUnicode(text);
	}
	
	VD_FORCE_INLINE 
	void operator = ( const vd::ascii& text )
	{
		m_Data = Text::ToUnicode( text );
	}

	VD_FORCE_INLINE 
	void operator = ( const vd::unicode& text )
	{
		m_Data = text;
	}

	VD_FORCE_INLINE 
	bool operator == ( const Buffer& text ) const
	{
		return m_Data == text.m_Data;
	}

	VD_FORCE_INLINE 
	vd::ascii ToAscii() const
	{
		return Text::ToAscii( m_Data );
	}

	VD_FORCE_INLINE 
	const vd::unicode& ToUnicode() const
	{
		return m_Data;
	}

private:

	vd::unicode m_Data;
};

// ============================================================================================== //

std::vector<vd::string> 
Tokenize(const vd::string& string, const vd::string& delim);

vd::string 
Trim(const vd::string& str);

vd::string 
Indent(const vd::string& string, int amount = 1);

vd::string 
Format(const char* pFmt, ...);

vd::string 
FormatTime(vd::f64 time, bool precise = false);

vd::string 
FormatByteSize(size_t size);

template<class Iterator> 
vd::string 
ContainerToString(
	const Iterator& start, const Iterator& end)
{
    std::ostringstream oss;
    oss << "{" << std::endl;
    Iterator it = start;

    while(it != end)
    {
        oss << "  " << Text::Indent((*it)->ToString());
        ++it;

        if(it != end)
            oss << "," << std::endl;
        else
            oss << std::endl;
    }

    oss << "}";
    return oss.str();
}

// ============================================================================================== //

} // end namespace: Text 

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif
