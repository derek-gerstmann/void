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

#include "vd.h"
#include "core/file.h"
#include "core/core.h"
#include "core/logging.h"
#include "core/memory.h"
#include "core/symbol.h"
#include <sys/stat.h>

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

FileAccess::FileAccess () :
    m_Handle(0),
    m_Mode(Mode::Invalid)
{
}

FileAccess::FileAccess (
	const std::string& filename, 
	Mode::Value mode
) :
    m_Handle(0),
    m_Mode(Mode::Invalid)
{
    Open(filename, mode);
}

FileAccess::~FileAccess ()
{
    if (m_Mode != Mode::Invalid)
    {
        Close();
    }
}

bool FileAccess::Open (
	const std::string& filename, 
	Mode::Value mode)
{
    if (m_Mode == Mode::Invalid)
    {
        if (mode == Mode::ReadOnly || mode == Mode::ReadOnlySwapEndian)
        {
            m_Handle = fopen(filename.c_str(), "rbS");
        }
        else
        {
            m_Handle = fopen(filename.c_str(), "wbS");
        }

        if (m_Handle)
        {
            m_Mode = mode;
            return true;
        }

        vdLogWarning("Failed to open file %s\n", filename.c_str());
    }
    else
    {
        vdLogWarning("File %s is already open\n", filename.c_str());
    }

    return false;
}

bool FileAccess::Close ()
{
    m_Mode = Mode::Invalid;
    if (fclose(m_Handle) == 0)
    {
        return true;
    }

    vdLogWarning("Failed to close file\n");
    return false;
}

FileAccess::operator bool () const
{
    return m_Mode != Mode::Invalid;
}

bool FileAccess::Read (
	size_t bytes, void* data)
{
    if ((m_Mode != Mode::ReadOnly && m_Mode != Mode::ReadOnlySwapEndian) || !m_Handle || !data
    ||  (bytes != 1 && bytes != 2 && bytes != 4 && bytes != 8))
    {
        vdLogWarning("Invalid file access for read request!");
        return false;
    }

    int read = (int)fread(data, bytes, 1, m_Handle);
    if (read != 1)
    {
        vdLogWarning("File read request was truncated!  Returned only %x / %x bytes!", read, bytes);
        return false;
    }

    if (m_Mode == Mode::ReadOnlySwapEndian && bytes > 1)
    {
        ByteOrder::Swap(bytes, data);
    }

    return true;
}

bool FileAccess::Read (
	size_t bytes, int count, void* data)
{
    if ((m_Mode != Mode::ReadOnly && m_Mode != Mode::ReadOnlySwapEndian) || !m_Handle
    ||  count <= 0 || !data
    ||  (bytes != 1 && bytes != 2 && bytes != 4 && bytes != 8))
    {
        vdLogWarning("Invalid file access for read request!");
        return false;
    }

    int read = (int)fread(data, bytes, count, m_Handle);
    if (read != count)
    {
        vdLogWarning("File read request was truncated!  Returned only %x / %x bytes!", read, bytes);
        return false;
    }

    if (m_Mode == Mode::ReadOnlySwapEndian && bytes > 1)
    {
        ByteOrder::Swap(bytes, count, data);
    }
    return true;
}

bool FileAccess::Write (
	size_t bytes, const void* data)
{
    if ((m_Mode != Mode::WriteOnly && m_Mode != Mode::WriteOnlySwapEndian) || !m_Handle || !data
    ||  (bytes != 1 && bytes != 2 && bytes != 4 && bytes != 8))
    {
        vdLogWarning("Invalid file access for write request!");
        return false;
    }

    int written;
    if (m_Mode == Mode::WriteOnlySwapEndian && bytes > 1)
    {
        if (bytes == 2)
        {
            short swap = *(const short*)data;
            ByteOrder::Swap(bytes, &swap);
            written = (int)fwrite(&swap, bytes, 1, m_Handle);
        }
        else if (bytes == 4)
        {
            int swap = *(const int*)data;
            ByteOrder::Swap(bytes, &swap);
            written = (int)fwrite(&swap, bytes, 1, m_Handle);
        }
        else if (bytes == 8)
        {
            double swap = *(const double*)data;
            ByteOrder::Swap(bytes, &swap);
            written = (int)fwrite(&swap, bytes, 1, m_Handle);
        }
        else
        {
            written = 0;
	        vdLogWarning("Invalid file access for write request!");
        }
    }
    else
    {
        written = (int)fwrite(data, bytes, 1, m_Handle);
    }

    if (written != 1)
    {
        vdLogWarning("File write request was truncated!  Returned only %x / %x bytes!", written, bytes);
        return false;
    }

    return true;
}

bool FileAccess::Write (
	size_t bytes, int count, const void* data)
{
    if ((m_Mode != Mode::WriteOnly && m_Mode != Mode::WriteOnlySwapEndian) || !m_Handle
    ||  count <= 0 || !data
    ||  (bytes != 1 && bytes != 2 && bytes != 4 && bytes != 8))
    {
        vdLogWarning("Invalid file acccess for write request");
        return false;
    }

    int written;
    if (m_Mode == Mode::WriteOnlySwapEndian && bytes > 1)
    {
        written = 0;
        if (bytes == 2)
        {
            const short* swap = (const short*)data;
            for (int i = 0; i < count; ++i, ++swap)
            {
                short temp = *swap;
                ByteOrder::Swap(bytes, &temp);
                written += (int)fwrite(&temp, bytes, 1, m_Handle);
            }
        }
        else if (bytes == 4)
        {
            const int* swap = (const int*)data;
            for (int i = 0; i < count; ++i, ++swap)
            {
                int temp = *swap;
                ByteOrder::Swap(bytes, &temp);
                written += (int)fwrite(&temp, bytes, 1, m_Handle);
            }
        }
        else if (bytes == 8)
        {
            const double* swap = (const double*)data;
            for (int i = 0; i < count; ++i, ++swap)
            {
                double temp = *swap;
                ByteOrder::Swap(bytes, &temp);
                written += (int)fwrite(&temp, bytes, 1, m_Handle);
            }
        }
        else
        {
            vdLogWarning("Invalid file access for write request!");
        }
    }
    else
    {
        written = (int)fwrite(data, bytes, count, m_Handle);
    }

    if (written != count)
    {
        vdLogWarning("File write request was truncated!  Returned only %x / %x bytes!", write, bytes);
        return false;
    }

    return true;
}

bool FileAccess::Load (
	const std::string& filename, bool binary,
    int& capacity, char*& buffer)
{
    struct stat statistics;
    if (stat(filename.c_str(), &statistics) != 0)
    {
        // The file does not exist.
        vdLogGlobalWarning("Failed to open file %s\n", filename.c_str());
        buffer = 0;
        capacity = 0;
        return false;
    }

    FILE* input;
    if (binary)
    {
        input = fopen(filename.c_str(), "rb");
    }
    else
    {
        input = fopen(filename.c_str(), "rt");
    }
    if (!input)
    {
        // The file cannot be opened for reading. 
        vdLogGlobalWarning("Failed to open file %s\n", filename.c_str());
        buffer = 0;
        capacity = 0;
        return false;
    }

    capacity = statistics.st_size;
    buffer = VD_NEW_ARRAY(char, capacity+1);
    Memory::MemSet(buffer, 0, capacity);
    int read = (int)fread(buffer, sizeof(char), capacity, input);
    if (fclose(input) != 0 || read != capacity)
    {
        vdLogGlobalWarning("Failed to read or close file %s\n", filename.c_str());

        VD_DELETE_ARRAY(buffer);
        buffer = 0;
        capacity = 0;
        return false;
    }

    return true;
}

bool FileAccess::Save (
	const std::string& filename, bool binary,
    int capacity, const char* buffer)
{
    if (!buffer || capacity <= 0)
    {
        vdLogGlobalWarning("Invalid inputs, file %s\n", filename.c_str());
        return false;
    }

    FILE* output;
    if (binary)
    {
        output = fopen(filename.c_str(), "wb");
    }
    else
    {
        output = fopen(filename.c_str(), "wt");
    }

    if (!output)
    {
        vdLogGlobalWarning("Failed to open file %s\n", filename.c_str());
        return false;
    }

    int written = (int)fwrite(buffer, sizeof(char), capacity, output);
    if (fclose(output) != 0 || written != capacity)
    {
        vdLogGlobalWarning("Failed to write or close file %s\n",
            filename.c_str());
        return false;
    }

    return true;
}

bool FileAccess::Append (
	const std::string& filename, bool binary,
    int capacity, const char* buffer)
{
    if (!buffer || capacity <= 0)
    {
        vdLogGlobalWarning("Invalid inputs, file %s\n", filename.c_str());
        return false;
    }

    FILE* output;
    if (binary)
    {
        output = fopen(filename.c_str(), "ab");
    }
    else
    {
        output = fopen(filename.c_str(), "at");
    }

    if (!output)
    {
        vdLogGlobalWarning("Failed to open file %s\n", filename.c_str());
        return false;
    }

    int written = (int)fwrite(buffer, sizeof(char), capacity, output);
    if (fclose(output) != 0 || written != capacity)
    {
        vdLogGlobalWarning("Failed to write or close file %s\n", filename.c_str());
        return false;
    }

    return true;
}

// ============================================================================================== //

VD_IMPLEMENT_OBJECT(FileAccess, vd_sym(FileAccess), vd_sym(Object));

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //
