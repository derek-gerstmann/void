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

#include "core/endian.h"
#include "core/asserts.h"

#include <iostream>
#include <fstream>

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

static int Goldilocks = 1;
bool Endian::m_IsLittle = (*(char*)&Goldilocks != 0);

// ============================================================================================== //

bool Endian::IsBig ()
{
    return !m_IsLittle;
}

bool Endian::IsLittle ()
{
    return m_IsLittle;
}

void Endian::Swap2 (void* data)
{
    char* raw = (char*) data;
    char save = raw[0];
    raw[0] = raw[1];
    raw[1] = save;
}

void Endian::Swap2 (int count, void* data)
{
    char* raw = (char*) data;
    for (int i = 0; i < count; ++i, raw += 2)
    {
        char save = raw[0];
        raw[0] = raw[1];
        raw[1] = save;
    }
}

void Endian::Swap4 (void* datum)
{
    char* raw = (char*) datum;
    char save = raw[0];
    raw[0] = raw[3];
    raw[3] = save;
    save = raw[1];
    raw[1] = raw[2];
    raw[2] = save;
}

void Endian::Swap4 (int count, void* data)
{
    char* raw = (char*) data;
    for (int i = 0; i < count; ++i, raw += 4)
    {
        char save = raw[0];
        raw[0] = raw[3];
        raw[3] = save;
        save = raw[1];
        raw[1] = raw[2];
        raw[2] = save;
    }
}

void Endian::Swap8 (void* data)
{
    char* raw = (char*) data;
    char save = raw[0];
    raw[0] = raw[7];
    raw[7] = save;
    save = raw[1];
    raw[1] = raw[6];
    raw[6] = save;
    save = raw[2];
    raw[2] = raw[5];
    raw[5] = save;
    save = raw[3];
    raw[3] = raw[4];
    raw[4] = save;
}

void Endian::Swap8 (int count, void* data)
{
    char* raw = (char*) data;
    for (int i = 0; i < count; ++i, raw += 8)
    {
        char save = raw[0];
        raw[0] = raw[7];
        raw[7] = save;
        save = raw[1];
        raw[1] = raw[6];
        raw[6] = save;
        save = raw[2];
        raw[2] = raw[5];
        raw[5] = save;
        save = raw[3];
        raw[3] = raw[4];
        raw[4] = save;
    }
}

void Endian::Swap (size_t bytes, void* data)
{
    vdGlobalAssertMsg(bytes == 2 || bytes == 4 || bytes == 8,
        "Size must be 2, 4, or 8\n");

    int size = (int)bytes;
    char* raw = (char*) data;
    for (int i0 = 0, i1 = size - 1; i0 < size/2; ++i0, --i1)
    {
        char save = raw[i0];
        raw[i0] = raw[i1];
        raw[i1] = save;
    }
}

void Endian::Swap (size_t bytes, int count, void* data)
{
    vdGlobalAssertMsg(bytes == 2 || bytes == 4 || bytes == 8,
        "Size must be 2, 4, or 8\n");

    int size = (int)bytes;
    char* raw = (char*) data;
    for (int i = 0; i < count; ++i, raw += bytes)
    {
        for (int i0 = 0, i1 = size - 1; i0 < size/2; ++i0, --i1)
        {
            char save = raw[i0];
            raw[i0] = raw[i1];
            raw[i1] = save;
        }
    }
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //
