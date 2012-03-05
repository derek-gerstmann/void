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

#ifndef VD_CORE_FILE_INCLUDED
#define VD_CORE_FILE_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/object.h"

#include <iostream>
#include <fstream>

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class FileAccess : public Object
{
public:
	VD_DECLARE_ENUM(Mode,
        ReadOnly,
        WriteOnly,
        ReadOnlySwapEndian,
        WriteOnlySwapEndian);

    FileAccess ();
    FileAccess (const std::string& filename, Mode::Value mode);
    ~FileAccess ();

    bool Open (const std::string& filename, Mode::Value mode);
    bool Close ();

    operator bool () const;

    int GetMode () const;

    bool Read (size_t bytes, void* datum);
    bool Read (size_t bytes, int count, void* data);
    
    bool Write (size_t bytes, const void* data);
    bool Write (size_t bytes, int count, const void* data);

    static bool Load (const std::string& filename, bool binary, int& capacity, char*& buffer);
    static bool Save (const std::string& filename, bool binary, int capacity, const char* buffer);
    static bool Append (const std::string& filename, bool binary, int capacity, const char* buffer);

	VD_DECLARE_OBJECT(FileAccess);
	
private:
	VD_DISABLE_COPY_CONSTRUCTORS(FileAccess);
	
    FILE* m_Handle;
    Mode::Value m_Mode;
};

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif
