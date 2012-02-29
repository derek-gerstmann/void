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

#include "core/filename.h"

#include <iostream>
#include <fstream>

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

#if defined(VD_PLAFORM_WINDOWS)
const char path_sep = '\\';
#else
const char path_sep = '/';
#endif

// ============================================================================================== //

static size_t 
vdMaxSize(size_t a, size_t b)
{ 
	return vd::u64(a) < vd::u64(b) ? b : a; 
}

// ============================================================================================== //

FileName::FileName() {}

FileName::FileName(const char* in)
{
    m_Location = in;

    for(size_t i = 0; i < m_Location.size(); i++)
        if(m_Location[i] == '\\' || m_Location[i] == '/')
            m_Location[i] = path_sep;

    while(!m_Location.empty() && m_Location[m_Location.size() - 1] == path_sep)
        m_Location.resize(m_Location.size() - 1);
}

FileName::FileName(const vd::string& in)
{
    m_Location = in;

    for(size_t i = 0; i < m_Location.size(); i++)
        if(m_Location[i] == '\\' || m_Location[i] == '/')
            m_Location[i] = path_sep;

    while(!m_Location.empty() && m_Location[m_Location.size() - 1] == path_sep)
        m_Location.resize(m_Location.size() - 1);
}

FileName 
FileName::GetPath() const
{
    size_t pos = vdMaxSize(m_Location.find_last_of('\\'), m_Location.find_last_of('/'));

    if(pos == vd::string::npos) 
    	return FileName();

    return m_Location.substr(0, pos);
}

vd::string 
FileName::GetBase() const
{
    size_t pos = vdMaxSize(m_Location.find_last_of('\\'), m_Location.find_last_of('/'));

    if(pos == vd::string::npos) 
    	return m_Location;

    return m_Location.substr(pos + 1);
}

vd::string 
FileName::GetExt() const
{
    size_t pos = m_Location.find_last_of('.');

    if(pos == vd::string::npos) 
    	return "";

    return m_Location.substr(pos + 1);
}

vd::string 
FileName::GetName() const
{
    size_t start = vdMaxSize(m_Location.find_last_of('\\'), m_Location.find_last_of('/')) + 1;

    if(start == vd::string::npos) 
    	start = 0;

    size_t end = m_Location.find_last_of('.');

    if(end == vd::string::npos || end < start) 
    	end = m_Location.size();

    return m_Location.substr(start, end - start);
}

FileName 
FileName::SetExt(const vd::string& ext) const
{
    size_t start = vdMaxSize(m_Location.find_last_of('\\'), m_Location.find_last_of('/')) + 1;

    if(start == vd::string::npos) 
    	start = 0;

    size_t end = m_Location.find_last_of('.');

    if(end == vd::string::npos || end < start) 
    	return FileName(m_Location + ext);

    return FileName(m_Location.substr(0, end) + ext);
}

FileName 
FileName::AddExt(const vd::string& ext) const
{
    return FileName(m_Location + ext);
}

FileName 
FileName::operator +(const FileName& other) const
{
    if(m_Location == "") 
    	return FileName(other);
    else 
    	return FileName(m_Location + path_sep + other.m_Location);
}

FileName FileName::operator +(const vd::string& other) const
{
    return operator+(FileName(other));
}

FileName FileName::operator -(const FileName& base) const
{
    size_t pos = m_Location.find_first_of(base);

    if(pos == vd::string::npos) 
    	return *this;

    return FileName(m_Location.substr(pos + 1));
}

std::ostream& operator<<(std::ostream& cout, const FileName& m_Location)
{
    return cout << m_Location.m_Location;
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

