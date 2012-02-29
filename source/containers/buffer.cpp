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

#include "containers/buffer.h"

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_BEGIN();

// ============================================================================================== //

void ReadBuffer::ReadNoSizeCheck(
    void* buffer, size_t size)
{
    vdGlobalAssert((m_BasePtr != 0 && m_EndPtr == 0) || m_CursorPtr + size <= m_EndPtr);
    if (buffer)
        memcpy(buffer, m_CursorPtr, size);
    m_CursorPtr += size;
}

const void* ReadBuffer::Skip(size_t size)
{
    const void* result = m_CursorPtr;
    ReadNoSizeCheck(NULL, size);
    return result;
}

size_t ReadBuffer::SkipToAlign4()
{
    size_t pos = this->GetOffset();
    size_t n = VD_PTR_ALIGN_4(pos) - pos;
    m_CursorPtr += n;
    return n;
}

void* WriteBuffer::Skip(size_t size)
{
    void* result = m_CursorPtr;
    WriteNoSizeCheck(NULL, size);
    return m_BasePtr == NULL ? NULL : result;
}

void WriteBuffer::WriteNoSizeCheck(
    const void* buffer, size_t size)
{
    vdGlobalAssert(m_BasePtr == 0 || m_EndPtr == 0 || m_CursorPtr + size <= m_EndPtr);
    if (m_BasePtr && buffer)
        memcpy(m_CursorPtr, buffer, size);
    m_CursorPtr += size;
}

size_t WriteBuffer::PadToAlign4()
{
    size_t pos = this->GetOffset();
    size_t n = VD_PTR_ALIGN_4(pos) - pos;

    if (n && m_BasePtr)
    {
        char* p = m_CursorPtr;
        char* stop = p + n;
        do {
            *p++ = 0;
        } while (p < stop);
    }
    m_CursorPtr += n;
    return n;
}

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_END();

// ============================================================================================== //


