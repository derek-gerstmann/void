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

#include "core/stream.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

static Stream::ByteOrder::Value 
GetByteOrder()
{
    union
    {
        vd::u8  v8[2];
        vd::u16 v16;
    };
    v8[0] = 1;
    v8[1] = 0;

    if (v16 == 1)
        return Stream::ByteOrder::LittleEndian;
    else
        return Stream::ByteOrder::BigEndian;
}

// ============================================================================================== //

Stream::ByteOrder::Value Stream::m_HostByteOrder = GetByteOrder();

// ============================================================================================== //

Stream::Stream()
{
    m_StreamByteOrder = m_HostByteOrder;
}

void 
Stream::SetByteOrder(
    Stream::ByteOrder::Value order)
{
    m_StreamByteOrder = order;
}

void 
Stream::Skip(
    size_t amount)
{
    SetPos(GetPos() + amount);
}

void 
Stream::WriteI32(
    vd::i32 value)
{
    if (m_StreamByteOrder != m_HostByteOrder)
        value = Core::ByteOrder::Swap(value);

    Write(&value, sizeof(vd::i32));
}

void 
Stream::WriteArrayI32(
    const vd::i32* array, size_t count)
{
    if (m_StreamByteOrder != m_HostByteOrder)
    {
        vd::i32* temp = VD_NEW_ARRAY(vd::i32, count);
        for (size_t i = 0; i < count; ++i)
            temp[i] = Core::ByteOrder::Swap(array[i]);
        Write(temp, sizeof(vd::i32) * count);
        VD_DELETE_ARRAY(temp);
    }
    else
    {
        Write(array, sizeof(vd::i32) * count);
    }
}

void 
Stream::WriteU32(
    vd::u32 value)
{
    if (m_StreamByteOrder != m_HostByteOrder)
        value = Core::ByteOrder::Swap(value);

    Write(&value, sizeof(vd::u32));
}

void 
Stream::WriteArrayU32(
    const vd::u32* array, size_t count)
{
    if (m_StreamByteOrder != m_HostByteOrder)
    {
        vd::u32* temp = VD_NEW_ARRAY(vd::u32, count);
        for (size_t i = 0; i < count; ++i)
            temp[i] = Core::ByteOrder::Swap(array[i]);
        Write(temp, sizeof(vd::u32) * count);
        VD_DELETE_ARRAY(temp);
    }
    else
    {
        Write(array, sizeof(vd::u32) * count);
    }
}

void 
Stream::WriteI64(
    vd::i64 value)
{
    if (m_StreamByteOrder != m_HostByteOrder)
        value = Core::ByteOrder::Swap(value);
    Write(&value, sizeof(vd::i64));
}

void 
Stream::WriteArrayI64(
    const vd::i64* array, size_t count)
{
    if (m_StreamByteOrder != m_HostByteOrder)
    {
        vd::i64* temp = VD_NEW_ARRAY(vd::i64, count);
        for (size_t i = 0; i < count; ++i)
            temp[i] = Core::ByteOrder::Swap(array[i]);
        Write(temp, sizeof(vd::i64) * count);
        VD_DELETE_ARRAY(temp);
    }
    else
    {
        Write(array, sizeof(vd::i64) * count);
    }
}

void 
Stream::WriteU64(
    vd::u64 value)
{
    if (m_StreamByteOrder != m_HostByteOrder)
        value = Core::ByteOrder::Swap(value);
    Write(&value, sizeof(vd::u64));
}

void 
Stream::WriteArrayU64(
    const vd::u64* array, size_t count)
{
    if (m_StreamByteOrder != m_HostByteOrder)
    {
        vd::u64* temp = VD_NEW_ARRAY(vd::u64, count);
        for (size_t i = 0; i < count; ++i)
            temp[i] = Core::ByteOrder::Swap(array[i]);
        Write(temp, sizeof(vd::u64) * count);
        VD_DELETE_ARRAY(temp);
    }
    else
    {
        Write(array, sizeof(vd::u64) * count);
    }
}

void 
Stream::WriteI16(
    vd::i16 value)
{
    if (m_StreamByteOrder != m_HostByteOrder)
        value = Core::ByteOrder::Swap(value);
    Write(&value, sizeof(vd::i16));
}

void 
Stream::WriteArrayI16(
    const vd::i16* array, size_t count)
{
    if (m_StreamByteOrder != m_HostByteOrder)
    {
        vd::i16* temp = VD_NEW_ARRAY(vd::i16, count);
        for (size_t i = 0; i < count; ++i)
            temp[i] = Core::ByteOrder::Swap(array[i]);
        Write(temp, sizeof(vd::i16) * count);
        VD_DELETE_ARRAY(temp);
    }
    else
    {
        Write(array, sizeof(vd::i16) * count);
    }
}

void 
Stream::WriteU16(
    vd::u16 value)
{
    if (m_StreamByteOrder != m_HostByteOrder)
        value = Core::ByteOrder::Swap(value);
    Write(&value, sizeof(vd::u16));
}

void Stream::WriteArrayU16(
    const vd::u16* array, size_t count)
{
    if (m_StreamByteOrder != m_HostByteOrder)
    {
        vd::u16* temp = VD_NEW_ARRAY(vd::u16, count);
        for (size_t i = 0; i < count; ++i)
            temp[i] = Core::ByteOrder::Swap(array[i]);
        Write(temp, sizeof(vd::u16) * count);
        VD_DELETE_ARRAY(temp);
    }
    else
    {
        Write(array, sizeof(vd::u16) * count);
    }
}

void 
Stream::WriteI8(
    vd::i8 value)
{
    Write(&value, sizeof(vd::i8));
}

void 
Stream::WriteU8(
    vd::u8 value)
{
    Write(&value, sizeof(vd::u8));
}

void 
Stream::WriteF32(
    vd::f32 value)
{
    if (m_StreamByteOrder != m_HostByteOrder)
    {
        value = Core::ByteOrder::Swap(value);
    }
    Write(&value, sizeof(vd::f32));
}

void 
Stream::WriteArrayF32(
    const vd::f32* array, size_t count)
{
    if (m_StreamByteOrder != m_HostByteOrder)
    {
        vd::f32* temp = VD_NEW_ARRAY(vd::f32, count);
        for (size_t i = 0; i < count; ++i)
            temp[i] = Core::ByteOrder::Swap<vd::f32>(array[i]);
        Write(temp, sizeof(vd::f32) * count);
        VD_DELETE_ARRAY(temp);
    }
    else
    {
        Write(array, sizeof(vd::f32) * count);
    }
}


void 
Stream::WriteF64(
    vd::f64 value)
{
    if (m_StreamByteOrder != m_HostByteOrder)
        value = Core::ByteOrder::Swap(value);
    Write(&value, sizeof(vd::f64));
}

void 
Stream::WriteArrayF64(
    const vd::f64* array, size_t count)
{
    if (m_StreamByteOrder != m_HostByteOrder)
    {
        vd::f64* temp = VD_NEW_ARRAY(vd::f64, count);
        for (size_t i = 0; i < count; ++i)
            temp[i] = Core::ByteOrder::Swap<vd::f64>(array[i]);
        Write(temp, sizeof(vd::f64) * count);
        VD_DELETE_ARRAY(temp);
    }
    else
    {
        Write(array, sizeof(vd::f64) * count);
    }
}

void 
Stream::WriteString(
    const std::string &value)
{
    Write(value.c_str(), value.length() + 1);
}

void Stream::WriteLine(
    const std::string &value)
{
    Write(value.c_str(), value.length());
    WriteI8('\n');
}

bool Stream::IsEOF() const
{
    return (GetPos() == GetSize());
}

vd::i8 
Stream::ReadI8()
{
    vd::i8 value;
    Read(&value, sizeof(vd::i8));
    return value;
}

vd::u8 
Stream::ReadU8()
{
    vd::u8 value;
    Read(&value, sizeof(vd::u8));
    return value;
}

vd::i16 
Stream::ReadI16()
{
    vd::i16 value;
    Read(&value, sizeof(vd::i16));
    if (m_StreamByteOrder != m_HostByteOrder)
        value = Core::ByteOrder::Swap<vd::i16>(value);
    return value;
}

void 
Stream::ReadArrayI16(
    vd::i16* array, size_t count)
{
    Read(array, sizeof(vd::i16) * count);
    if (m_StreamByteOrder != m_HostByteOrder)
    {
        for (size_t i = 0; i < count; ++i)
            array[i] = Core::ByteOrder::Swap<vd::i16>(array[i]);
    }
}

vd::u16
Stream::ReadU16()
{
    vd::u16 value;
    Read(&value, sizeof(vd::u16));
    if (m_StreamByteOrder != m_HostByteOrder)
        value = Core::ByteOrder::Swap<vd::u16>(value);
    return value;
}

void 
Stream::ReadArrayU16(
    vd::u16* array, size_t count)
{
    Read(array, sizeof(vd::u16) * count);
    if (m_StreamByteOrder != m_HostByteOrder)
    {
        for (size_t i = 0; i < count; ++i)
            array[i] = Core::ByteOrder::Swap<vd::u16>(array[i]);
    }
}


vd::i32 
Stream::ReadI32()
{
    vd::i32 value;
    Read(&value, sizeof(vd::i32));
    if (m_StreamByteOrder != m_HostByteOrder)
        value = Core::ByteOrder::Swap<vd::i32>(value);
    return value;
}

void 
Stream::ReadArrayI32(
    vd::i32* array, size_t count)
{
    Read(array, sizeof(vd::i32) * count);
    if (m_StreamByteOrder != m_HostByteOrder)
    {
        for (size_t i = 0; i < count; ++i)
            array[i] = Core::ByteOrder::Swap<vd::i32>(array[i]);
    }
}

vd::u32
Stream::ReadU32()
{
    vd::u32 value;
    Read(&value, sizeof(vd::u32));
    if (m_StreamByteOrder != m_HostByteOrder)
        value = Core::ByteOrder::Swap<vd::u32>(value);
    return value;
}

void 
Stream::ReadArrayU32(
    vd::u32* array, size_t count)
{
    Read(array, sizeof(vd::u32) * count);
    if (m_StreamByteOrder != m_HostByteOrder)
    {
        for (size_t i = 0; i < count; ++i)
            array[i] = Core::ByteOrder::Swap<vd::u32>(array[i]);
    }
}

vd::i64
Stream::ReadI64()
{
    vd::i64 value;
    Read(&value, sizeof(vd::i64));
    if (m_StreamByteOrder != m_HostByteOrder)
        value = Core::ByteOrder::Swap<vd::i64>(value);
    return value;
}

void 
Stream::ReadArrayI64(
    vd::i64* array, size_t count)
{
    Read(array, sizeof(vd::i64) * count);
    if (m_StreamByteOrder != m_HostByteOrder)
    {
        for (size_t i = 0; i < count; ++i)
            array[i] = Core::ByteOrder::Swap<vd::i64>(array[i]);
    }
}

vd::u64 
Stream::ReadU64()
{
    vd::u64 value;
    Read(&value, sizeof(vd::u64));
    if (m_StreamByteOrder != m_HostByteOrder)
        value = Core::ByteOrder::Swap<vd::u64>(value);
    return value;
}

void 
Stream::ReadArrayU64(
    vd::u64* array, size_t count)
{
    Read(array, sizeof(vd::u64) * count);
    if (m_StreamByteOrder != m_HostByteOrder)
    {
        for (size_t i = 0; i < count; ++i)
            array[i] = Core::ByteOrder::Swap<vd::u64>(array[i]);
    }
}

vd::f32 
Stream::ReadF32()
{
    vd::f32 value;
    Read(&value, sizeof(float));
    if (m_StreamByteOrder != m_HostByteOrder)
        value = Core::ByteOrder::Swap<vd::f32>(value);
    return value;
}

void 
Stream::ReadArrayF32(
    vd::f32* array, size_t count)
{
    Read(array, sizeof(vd::f32) * count);
    if (m_StreamByteOrder != m_HostByteOrder)
    {
        for (size_t i = 0; i < count; ++i)
            array[i] = Core::ByteOrder::Swap<vd::f32>(array[i]);
    }
}

vd::f64 
Stream::ReadF64()
{
    vd::f64 value;
    Read(&value, sizeof(vd::f64));
    if (m_StreamByteOrder != m_HostByteOrder)
        value = Core::ByteOrder::Swap<vd::f64>(value);
    return value;
}

void 
Stream::ReadArrayF64(
    vd::f64* array, size_t count)
{
    Read(array, sizeof(vd::f64) * count);
    if (m_StreamByteOrder != m_HostByteOrder)
    {
        for (size_t i = 0; i < count; ++i)
            array[i] = Core::ByteOrder::Swap<vd::f64>(array[i]);
    }
}

std::string 
Stream::ReadLine()
{
    std::string retval;
    vd::i8 data;
    bool nl = false;

    do
    {
        try
        {
            Read(&data, sizeof(vd::i8));
        }
        catch (std::exception &e)
        {
            if (GetPos() == GetSize())
            {
                if (retval.size() != 0)
                {
                    return retval;
                }
            }
            throw e;
        }
        if (data != 13 && data != 10)
            retval += data;
        else if (data == 10)
            nl = true;
    }
    while (!nl);
    return retval;
}

std::string 
Stream::ReadString()
{
    std::string retval;
    vd::i8 data;

    do
    {
        Read(&data, sizeof(vd::i8));
        if (data != 0)
            retval += data;
    }
    while (data != 0);
    return retval;
}

void 
Stream::CopyTo(
    Stream* stream, vd::i64 bytes)
{
    const int block_size = 512;
    char data[block_size];
    size_t copied = 0;

    size_t amount = (bytes == -1) ? (GetSize() - GetPos()) : (size_t) bytes;
    for (size_t i = 0; i<amount; i+=block_size)
    {
        size_t actual_size = (i + block_size) <= amount ? block_size : amount-i;

        Read(data, actual_size);
        copied += actual_size;
        stream->Write(data, actual_size);
    }
}

std::string 
Stream::ToString() const
{
    std::ostringstream oss;

    oss << "HostByteOrder="
        << Stream::ByteOrder::ToString(m_HostByteOrder)
        << ", StreamByteOrder="
        << Stream::ByteOrder::ToString(m_StreamByteOrder);

    return oss.str();
}
// ============================================================================================== //

VD_IMPLEMENT_ABSTRACT_OBJECT(Stream, vd_sym(Stream), vd_sym(Object));

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //
