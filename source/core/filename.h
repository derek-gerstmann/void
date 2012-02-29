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

#ifndef VD_CORE_FILENAME_INCLUDED
#define VD_CORE_FILENAME_INCLUDED

#include "core/core.h"

#include <string>
#include <cstdio>

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class FileName
{
public:

	FileName ();
	FileName (const char* filename);
	FileName (const vd::string& filename);
	
	operator vd::string() const { return m_Location; }
	const vd::string str() const { return m_Location; }
	const char* c_str() const { return m_Location.c_str(); }
	
	FileName GetPath() const;
	vd::string GetBase() const;
	vd::string GetName() const;
	vd::string GetExt() const;
	
	FileName SetExt(const vd::string& ext = "") const;
	FileName AddExt(const vd::string& ext = "") const;
	
	FileName operator +( const FileName& other ) const;
	FileName operator +( const vd::string& other ) const;
	FileName operator -( const FileName& base ) const;
	friend std::ostream& operator<<(std::ostream& cout, const FileName& filename);
	
private:
	vd::string m_Location;
};

// ============================================================================================== //
  
VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_FILENAME_INCLUDED

