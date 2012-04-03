
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

// ---------------------------------------------------------------------------------------------- //
// Derived from Gerrit Daniel's Template Reflection Library.  License follows:
// ---------------------------------------------------------------------------------------------- //
// 
// Copyright (C) 2010 by Gerrit Daniels <gerrit.daniels@gmail.com>
// 
// This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
// 
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
// 
// ============================================================================================== //

#include "serialise.h"

// ---------------------------------------------------------------------------------------------- //

using namespace vd;
using namespace vd::Core;

// ---------------------------------------------------------------------------------------------- //

WeakPtrWrapper::Internal::Internal()
{ 
    // EMPTY!
}

WeakPtrWrapper::Internal::~Internal()
{ 
    // EMPTY!
}

WeakPtrWrapper::WeakPtrWrapper(
    boost::shared_ptr<WeakPtrWrapper::Internal> internal
) : 
    m_Internal(internal)
{ 
    // EMPTY!
}


WeakPtrWrapper::WeakPtrWrapper(
    const WeakPtrWrapper& that
) : 
    m_Internal(that.m_Internal)
{ 
    // EMPTY!
}    

void WeakPtrWrapper::assign(
    boost::shared_ptr<void> ptr, 
    const std::string& sharedName)
{
    m_Internal->assign(ptr, sharedName);
}

// ---------------------------------------------------------------------------------------------- //

ReadBuffer::Internal::Internal() 
{ 
    // EMPTY!
}

ReadBuffer::Internal::~Internal() 
{ 
    // EMPTY!
}

ReadBuffer::ReadBuffer(
    std::auto_ptr<ReadBuffer::Internal> internal
) : 
    m_Internal(internal)
{
    // EMPTY!
}

// ---------------------------------------------------------------------------------------------- //

const char* WriteBuffer::Internal::Padding = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

WriteBuffer::Internal::Internal() :
   m_Data(new char[16]),
   m_Capacity(16),
   m_Length(0),
   m_IsOwner(true)
{ 
    // EMPTY!
}

WriteBuffer::Internal::~Internal()
{
    if(m_IsOwner) delete[] m_Data;
}

long WriteBuffer::Internal::GetLength()
{
    return m_Length;
}

char* WriteBuffer::Internal::GetData()
{
    m_IsOwner = false;
    return m_Data;
}

void WriteBuffer::Internal::AppendNoPadding(
   const char* data, long length)
{
    long quantity = m_Length + length;

    if(quantity > m_Capacity)
        Resize(quantity);

    for(long i = m_Length; i < quantity; ++i)
        m_Data[i] = data[i - m_Length];

    m_Length += length;
}

void WriteBuffer::Internal::Resize(long size)
{
    long quantity = m_Capacity * 16;

    while(quantity < size)
        size *= 16;

    char* data = new char[size];

    for(long i = 0; i < m_Length; ++i)
        data[i] = m_Data[i];

    delete[] m_Data;

    m_Data = data;
    m_Capacity = size;
}

WriteBuffer::WriteBuffer(
   std::auto_ptr<WriteBuffer::Internal> internal
)
   : m_Internal(internal)
{
   // EMPTY!
}

long WriteBuffer::GetLength()
{
    return m_Internal->GetLength();
}

char* WriteBuffer::GetData()
{
    return m_Internal->GetData();
}

void WriteBuffer::Append(const char* data, long length)
{
    m_Internal->Append(data, length);
}

// ---------------------------------------------------------------------------------------------- //

std::auto_ptr<Repository> Repository::repository;

Repository::Repository()
    : m_nextIndex(1)
{

}

Repository& Repository::GetInstance()
{
    if(repository.get() == 0)
        repository = std::auto_ptr<Repository>(new Repository);

    return *repository;
}

bool Repository::IsRegistered(void* p) const
{
    return m_indices.find(p) != m_indices.end();
}

int Repository::Get(void* p) const
{
    return m_indices.at(p);
}

int Repository::add(void* p)
{
    m_indices[p] = m_nextIndex;
    return m_nextIndex++;
}

int Repository::resrveIndex(void* p)
{
    m_reserved[p] = m_nextIndex;
    return m_nextIndex++;
}

bool Repository::IsReserved(void* p)
{
    return m_reserved.find(p) != m_reserved.end();
}

int Repository::GetReservedIndex(void* p)
{
    return m_reserved.at(p);
}

void Repository::clearReserved(void* p)
{
    int index = m_reserved.at(p);
    m_reserved.erase(p);
    m_indices[p] = index;
}

bool Repository::IsRegistered(const int& i) const
{
    return m_ptrs.find(i) != m_ptrs.end();
}

boost::shared_ptr<void> Repository::Get(const int& i) const
{
    return m_ptrs.at(i).first;
}

void Repository::add(const int& i, boost::shared_ptr<void> p
                            , const std::string& className)
{
    m_ptrs[i] = std::pair<boost::shared_ptr<void>, std::string>(p, className);
}

std::string Repository::GetPointerTypeName(const int& i) const
{
    return m_ptrs.at(i).second;
}
bool Repository::HasDelayedAssignment(const int& index) const
{
    return m_delayed.find(index) != m_delayed.end();
}

void Repository::Assign(const int& index
                                     , boost::shared_ptr<void> voidPtr
                                     , std::string sharedName)
{
    std::vector<WeakPtrWrapper>& weakWrappers(m_delayed.at(index));

    for(std::vector<WeakPtrWrapper>::iterator iter = weakWrappers.begin();
            iter != weakWrappers.end(); ++iter)
        iter->assign(voidPtr, sharedName);
}


void Repository::reset()
{
    m_indices.clear();
    m_ptrs.clear();
    m_delayed.clear();
    m_nextIndex = 1;
}

// ---------------------------------------------------------------------------------------------- //

std::auto_ptr<Polymorphic> Polymorphic::Serialiser;

Polymorphic::Polymorphic()
{

}

Polymorphic& Polymorphic::GetInstance()
{
    if(Serialiser.get() == 0)
        Serialiser = std::auto_ptr<Polymorphic>(new Polymorphic());

    return *Serialiser;
}

bool Polymorphic::IsPolymorphic(const std::string& className) const
{
    return m_functions.find(className) != m_functions.end();
}

void* Polymorphic::Deserialise(const std::string& className, ReadBuffer& buffer, int version) const
{
    return m_functions.at(className)(buffer, version);
}

int Polymorphic::RegisterDeserialise(const std::string& className
                                               , Polymorphic::DeserialiseFunction func)
{
    m_functions[className] = func;
    return 0;
}

int Polymorphic::RegisterCast(const std::string& from
                                        , const std::string& to
                                        , Polymorphic::CastFunction func)
{
    m_Casts[from][to] = func;
    return 0;
}

Polymorphic::CastFunction Polymorphic::GetCast(
    const std::string& from, const std::string& to) const
{
    return m_Casts.at(from).at(to);
}

bool Polymorphic::HasCast(const std::string& from
                                    , const std::string& to) const
{
    CastMap::const_iterator iter = m_Casts.find(from);

    if(iter == m_Casts.end())
        return false;

    return iter->second.find(to) != iter->second.end();
}

