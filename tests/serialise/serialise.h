
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

#ifndef VD_CORE_SERIALISE_INCLUDED
#define VD_CORE_SERIALISE_INCLUDED

// ---------------------------------------------------------------------------------------------- //
// Derived from Gerrit Daniel's Template Reflection Library.  License follows:
// ---------------------------------------------------------------------------------------------- //
// 
// Copyright (C) 2010 by Gerrit Daniels <gerrit.daniels@gmail.com>
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
// 
// ============================================================================================== //

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <iostream>
#include <iomanip>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

// ---------------------------------------------------------------------------------------------- //

#ifndef __APPLE__
#include <endian.h>
#else
#include <architecture/byte_order.h>
#include <libkern/OSByteOrder.h>
#define __LITTLE_ENDIAN    1234
#define __BIG_ENDIAN       4321
#endif

#define VD_BIG_ENDIAN     __BIG_ENDIAN
#define VD_LITTLE_ENDIAN  __LITTLE_ENDIAN

#ifndef __BYTE_ORDER
#define __BYTE_ORDER       __LITTLE_ENDIAN
#   define VD_BYTE_ORDER  VD_LITTLE_ENDIAN
#endif

// ============================================================================================== //

struct CheckAlignment { char c; long l; };
#define VD_MEMORY_ALIGNMENT sizeof(CheckAlignment) - sizeof(long)

// ============================================================================================== //

namespace vd
{

// ---------------------------------------------------------------------------------------------- //

class Exception : public std::exception
{
public:
    Exception(const char* reason) : m_reason(reason) { }

    virtual const char* what() const throw() { return m_reason; }
private:
    const char* m_reason;
};


// ---------------------------------------------------------------------------------------------- //

namespace Core
{

class WriteBuffer;
class ReadBuffer;

// ---------------------------------------------------------------------------------------------- //

template <class ObjectType>
void Deserialise(ObjectType& value, ReadBuffer& buffer, int version);

template <class ElementType>
void Deserialise(std::vector<ElementType>& elements, ReadBuffer& buffer, int version);

template <class ElementType>
void Deserialise(std::list<ElementType>& elements, ReadBuffer& buffer, int version);

template <class KeyType, class ValueType>
void Deserialise(std::map<KeyType, ValueType>& elements, ReadBuffer& buffer, int version);

template <class ElementType>
void Deserialise(boost::shared_ptr<ElementType>& ptr, ReadBuffer& buffer, int version);

template <class ElementType>
void Deserialise(boost::weak_ptr<ElementType>& ptr, ReadBuffer& buffer, int version);

template <class ElementType>
void Deserialise(std::auto_ptr<ElementType>& ptr, ReadBuffer& buffer, int version);

template <>
void Deserialise(bool& value, ReadBuffer& buffer, int version);

template <>
void Deserialise(char& value, ReadBuffer& buffer, int version);

template <>
void Deserialise(short& value, ReadBuffer& buffer, int version);

template <>
void Deserialise(int& value, ReadBuffer& buffer, int version);

template <>
void Deserialise(long& value, ReadBuffer& buffer, int version);

template <>
void Deserialise(unsigned char& value, ReadBuffer& buffer, int version);

template <>
void Deserialise(unsigned short& value, ReadBuffer& buffer, int version);

template <>
void Deserialise(unsigned int& value, ReadBuffer& buffer, int version);

template <>
void Deserialise(unsigned long& value, ReadBuffer& buffer, int version);

template <>
void Deserialise(float& value, ReadBuffer& buffer, int version);

template <>
void Deserialise(double& value, ReadBuffer& buffer, int version);

template <>
void Deserialise(std::string& value, ReadBuffer& buffer, int version);

// ---------------------------------------------------------------------------------------------- //

template <class ObjectType>
void Serialise(const ObjectType& object, WriteBuffer& buffer, int version);

template <class ElementType>
void Serialise(const std::vector<ElementType>& elements, WriteBuffer& buffer, int version);

template <class ElementType>
void Serialise(const std::list<ElementType>& elements, WriteBuffer& buffer, int version);

template <class KeyType, class ValueType>
void Serialise(const std::map<KeyType, ValueType>& elements, WriteBuffer& buffer, int version);

template <class ElementType>
void Serialise(const boost::shared_ptr<ElementType>& ptr, WriteBuffer& buffer, int version);

template <class ElementType>
void Serialise(const boost::weak_ptr<ElementType>& ptr, WriteBuffer& buffer, int version);

template <class ElementType>
void Serialise(const std::auto_ptr<ElementType>& ptr, WriteBuffer& buffer, int version);

template <>
void Serialise(const bool& value, WriteBuffer& buffer, int version);

template <>
void Serialise(const char& value, WriteBuffer& buffer, int version);

template <>
void Serialise(const short& value, WriteBuffer& buffer, int version);

template <>
void Serialise(const int& value, WriteBuffer& buffer, int version);

template <>
void Serialise(const long& value, WriteBuffer& buffer, int version);

template <>
void Serialise(const unsigned char& value, WriteBuffer& buffer, int version);

template <>
void Serialise(const unsigned short& value, WriteBuffer& buffer, int version);

template <>
void Serialise(const unsigned int& value, WriteBuffer& buffer, int version);

template <>
void Serialise(const unsigned long& value, WriteBuffer& buffer, int version);

template <>
void Serialise(const float& value, WriteBuffer& buffer, int version);

template <>
void Serialise(const double& value, WriteBuffer& buffer, int version);

template <>
void Serialise(const std::string& value, WriteBuffer& buffer, int version);

// ---------------------------------------------------------------------------------------------- //

template <class ObjectType>
void Serialise(const ObjectType& object, WriteBuffer& buffer, int version);

template <class ObjectType>
void Deserialise(ObjectType& value, ReadBuffer& buffer, int version);

template <class ObjectType>
void Serialise(const ObjectType& object, WriteBuffer& buffer, int version);

template <class ObjectType>
void Deserialise(ObjectType& value, ReadBuffer& buffer, int version);

// ---------------------------------------------------------------------------------------------- //

template <class FromType, class ToType>
void* dynamic_reinterpret_cast(void* ptr)
{
    return reinterpret_cast<void*>(
               dynamic_cast<ToType*>(
                   reinterpret_cast<FromType*>(ptr)));
}

// ---------------------------------------------------------------------------------------------- //

template <class HT, class TT>
struct TypeList
{
    typedef HT HeadType;
    typedef TT TailType;
};

class NullType {};

template <int i_>
struct Enumerate
{
    enum { Identifier = i_ };
};

template <int condition_, class T0_, class T1_>
struct Select
{
    typedef T0_ Result;
};

template <class T0_, class T1_>
struct Select<false, T0_, T1_>
{
    typedef T1_ Result;
};

// ---------------------------------------------------------------------------------------------- //

template <class Number_, int length_>
struct ByteSwapper
{
    static void swap(Number_& val) { }
};

template <class Number_>
struct ByteSwapper<Number_, 1>
{
    static void swap(Number_& val) { }
};

template <class Number_>
struct ByteSwapper<Number_, 2>
{
    static void swap(Number_& val)
    {
        val = ((val & 0xff00) >> 8)
              | ((val & 0x00ff) << 8);
    }
};

template <class Number_>
struct ByteSwapper<Number_, 4>
{
    static void swap(Number_& val)
    {
        val = ((val & 0xff000000) >> 24)
              | ((val & 0x00ff0000) >> 8)
              | ((val & 0x0000ff00) << 8)
              | ((val & 0x000000ff) << 24);
    }
};

template <class Number_>
struct ByteSwapper<Number_, 8>
{
    static void swap(Number_& val)
    {
        val = ((val & 0xff00000000000000) >> 56)
              | ((val & 0x00ff000000000000) >> 40)
              | ((val & 0x0000ff0000000000) >> 24)
              | ((val & 0x000000ff00000000) >> 8)
              | ((val & 0x00000000ff000000) << 8)
              | ((val & 0x0000000000ff0000) << 24)
              | ((val & 0x000000000000ff00) << 40)
              | ((val & 0x00000000000000ff) << 56);
    }
};

// ---------------------------------------------------------------------------------------------- //

template<class Number_, int doConversion_>
class IntegerConvertor
{
public:
    static Number_ convert(const Number_& in)
    {
        Number_ out = in;
        ByteSwapper<Number_, sizeof(Number_)>::swap(out);
        return out;
    }
};

template<class Number_>
class IntegerConvertor<Number_, false>
{
public:
    static Number_ convert(const Number_& in) { return in; }
};

// ---------------------------------------------------------------------------------------------- //

class WeakPtrWrapper
{
public:
    class Internal
    {
    public:
        Internal();
        virtual ~Internal();
        virtual void assign(const boost::shared_ptr<void>& ptr, const std::string& sharedName) = 0;
    };

    WeakPtrWrapper(boost::shared_ptr<Internal> internal);
    WeakPtrWrapper(const WeakPtrWrapper& that);

    void assign(boost::shared_ptr<void> ptr, const std::string& sharedName);

private:
    boost::shared_ptr<Internal> m_Internal;
};

// ---------------------------------------------------------------------------------------------- //

class ReadBuffer
{
public:
    class Internal
    {
    public:
        Internal();
        virtual ~Internal();

        virtual void Read(bool& val) = 0;
        virtual void Read(char& val) = 0;
        virtual void Read(short& val) = 0;
        virtual void Read(int& val) = 0;
        virtual void Read(long& val) = 0;
        virtual void Read(unsigned char& val) = 0;
        virtual void Read(unsigned short& val) = 0;
        virtual void Read(unsigned int& val) = 0;
        virtual void Read(unsigned long& val) = 0;
        virtual void Read(float& val) = 0;
        virtual void Read(double& val) = 0;
        virtual void Read(std::string& val) = 0;
    }; // class Internal

    ReadBuffer(std::auto_ptr<Internal> internal);

    template <class T>
    void Read(T& val)
    {
        m_Internal->Read(val);
    }

private:
    std::auto_ptr<Internal> m_Internal;
};

// ---------------------------------------------------------------------------------------------- //

template <int alignment_, int endian_>
class ReadBufferInternal : public ReadBuffer::Internal
{
private:
    union FloatUnion { float f; int i; };
    union DoubleUnion { double d; long l; };
public:
    ReadBufferInternal(const char* data)
        : m_data(data)
    {

    }

    virtual ~ReadBufferInternal() { }

    virtual void Read(bool& val) { ReadNumber(val); }
    virtual void Read(char& val) { ReadNumber(val); }
    virtual void Read(short& val) { ReadNumber(val); }
    virtual void Read(int& val) { ReadNumber(val); }
    virtual void Read(long& val) { ReadNumber(val); }
    virtual void Read(unsigned char& val) { ReadNumber(val); }
    virtual void Read(unsigned short& val) { ReadNumber(val); }
    virtual void Read(unsigned int& val) { ReadNumber(val); }
    virtual void Read(unsigned long& val) { ReadNumber(val); }
    virtual void Read(float& val)
    {
        FloatUnion fu;
        ReadNumber(fu.i);
        val = fu.f;
    }

    virtual void Read(double& val)
    {
        DoubleUnion du;
        ReadNumber(du.l);
        val = du.d;
    }

    virtual void Read(std::string& val)
    {
        std::string::size_type length;
        ReadNumber(length);
        val = std::string(m_data, length);
        m_data += length + ((alignment_ - (length % alignment_)) % alignment_);
    }


private:
    template <typename Number_>
    void ReadNumber(Number_& n)
    {
        Number_ in = *(Number_*)(m_data);
        n = IntegerConvertor < Number_, endian_ != VD_BYTE_ORDER >::convert(in);
        m_data += sizeof(Number_) + ((alignment_ - (sizeof(Number_) % alignment_)) % alignment_);
    }

    const char* m_data;
};

// ---------------------------------------------------------------------------------------------- //

class WriteBuffer
{
public:
    class Internal
    {
    public:
        Internal();
        virtual ~Internal();

        long GetLength();
        char* GetData();

        virtual void Append(const bool& val) = 0;
        virtual void Append(const char& val) = 0;
        virtual void Append(const short& val) = 0;
        virtual void Append(const int& val) = 0;
        virtual void Append(const long& val) = 0;
        virtual void Append(const unsigned char& val) = 0;
        virtual void Append(const unsigned short& val) = 0;
        virtual void Append(const unsigned int& val) = 0;
        virtual void Append(const unsigned long& val) = 0;
        virtual void Append(const float& val) = 0;
        virtual void Append(const double& val) = 0;
        virtual void Append(const std::string& val) = 0;
        virtual void Append(const char* data, long length) = 0;

    protected:
        static const char* Padding;
        void AppendNoPadding(const char* data, long length);
        void Resize(long newLength);

    private:
        char* m_Data;
        long m_Capacity;
        long m_Length;
        bool m_IsOwner;
    }; // class Internal

    WriteBuffer(std::auto_ptr<Internal> internal);

    long GetLength();
    char* GetData();

    template <class T>
    void Append(const T& val)
    {
        m_Internal->Append(val);
    }

    void Append(const char* data, long length);

private:
    std::auto_ptr<Internal> m_Internal;
};

// ---------------------------------------------------------------------------------------------- //

template <int alignment_, int endian_>
class WriteBufferInternal : public WriteBuffer::Internal
{
private:
    union FloatUnion { float f; int i; };
    union DoubleUnion { double d; long l; };
public:
    WriteBufferInternal() { }
    virtual ~WriteBufferInternal() { }

    virtual void Append(const bool& val) { AppendNumber(val); }
    virtual void Append(const char& val) { AppendNumber(val); }
    virtual void Append(const short& val) { AppendNumber(val); }
    virtual void Append(const int& val) { AppendNumber(val); }
    virtual void Append(const long& val) { AppendNumber(val); }
    virtual void Append(const unsigned char& val) { AppendNumber(val); }
    virtual void Append(const unsigned short& val) { AppendNumber(val); }
    virtual void Append(const unsigned int& val) { AppendNumber(val); }
    virtual void Append(const unsigned long& val) { AppendNumber(val); }
    virtual void Append(const float& val)
    {
        FloatUnion fu;
        fu.f = val;
        AppendNumber(fu.i);
    }

    virtual void Append(const double& val)
    {
        DoubleUnion du;
        du.d = val;
        AppendNumber(du.l);
    }

    virtual void Append(const std::string& val)
    {
        std::string::size_type length = val.length();
        Append(length);
        AppendNoPadding(val.data(), length);
        AppendNoPadding(Padding, (alignment_ - (length % alignment_)) % alignment_);
    }

    virtual void Append(const char* data, long length)
    {
        AppendNoPadding(data, length);
        AppendNoPadding(Padding, (alignment_ - (length % alignment_)) % alignment_);
    }

private:
    template <typename Number_>
    void AppendNumber(const Number_& n)
    {
        Number_ out = IntegerConvertor < Number_, endian_ != VD_BYTE_ORDER >::convert(n);
        AppendNoPadding((char*)(&out), sizeof(Number_));
        AppendNoPadding(Padding, (alignment_ - (sizeof(Number_) % alignment_)) % alignment_);
    }
};


// ---------------------------------------------------------------------------------------------- //

template <class T, int V>
struct MemberInfo
{
    typedef T Type;
    static const int Version = V;
};

// ---------------------------------------------------------------------------------------------- //

template <class ObjectType>
struct BaseClassSerializer
{
    template <class LT>
    static void Serialise(const ObjectType& object, LT, WriteBuffer& buffer, int version)
    {
        vd::Core::Serialise<typename LT::HeadType>(object, buffer, version);
        Serialise(object, typename LT::TailType(), buffer, version);
    }

    static void Serialise(const ObjectType& object, NullType, WriteBuffer& buffer, int version)
    {

    }

    template <class LT>
    static void Deserialise(ObjectType& object, LT, ReadBuffer& buffer, int version)
    {
        vd::Core::Deserialise<typename LT::HeadType>(object, buffer, version);
        Deserialise(object, typename LT::TailType(), buffer, version);
    }

    static void Deserialise(ObjectType& object, NullType, ReadBuffer& buffer, int version)
    {

    }
};

// ---------------------------------------------------------------------------------------------- //

template <class ObjectType>
class ClassSerializer
{
public:
    template <class Indices_>
    static void Serialise(const ObjectType& object, Indices_ indices, WriteBuffer& buffer, int version)
    {
        if(version >= Indices_::HeadType::Version)
            vd::Core::Serialise(object.* ObjectType::GetMemberPtr(typename Indices_::HeadType::Type())
                                    , buffer, version);

        Serialise(object, typename Indices_::TailType(), buffer, version);
    }

    static void Serialise(const ObjectType& object, NullType indices, WriteBuffer& buffer, int version)
    {

    }

    template <class Indices_>
    static void Deserialise(ObjectType& object, Indices_ indices, ReadBuffer& buffer, int version)
    {
        if(version >= Indices_::HeadType::Version)
            vd::Core::Deserialise(object.* ObjectType::GetMemberPtr(typename Indices_::HeadType::Type())
                                      , buffer, version);

        Deserialise(object, typename Indices_::TailType(), buffer, version);
    }

    static void Deserialise(ObjectType& object, NullType indices, ReadBuffer& buffer, int version)
    {

    }
};

// ---------------------------------------------------------------------------------------------- //

template <class LT, int SL, int EL, class ObjectType>
struct CreateBaseClassListInternal
{
    typedef typename ObjectType::template IsBaseClassPresent<EL> HasBase;
    enum { HasBaseClass = HasBase::Value };
    typedef typename Select < HasBaseClass, TypeList<typename HasBase::BaseClass, LT>, LT >::Result SelectedListType;
    typedef CreateBaseClassListInternal < SelectedListType, SL, EL - 1, ObjectType > BaseClassListInternal;
    typedef typename BaseClassListInternal::ListType ListType;
};

// ---------------------------------------------------------------------------------------------- //

template <class LT, int SL, class ObjectType>
struct CreateBaseClassListInternal<LT, SL, SL, ObjectType>
{
    typedef LT ListType;
};

// ---------------------------------------------------------------------------------------------- //

template <int SL, int EL, class ObjectType>
struct CreateBaseClassList
{
    typedef CreateBaseClassListInternal < NullType, SL, EL - 1, ObjectType > BaseClassListInternal;
    typedef typename BaseClassListInternal::ListType ListType;
};

// ---------------------------------------------------------------------------------------------- //

template <class LT, int SL, int EL, class ObjectType>
struct CreateMemberIndicesInternal
{
    typedef typename ObjectType::template IsMemberPresent<EL> IsMemberPresent;
    enum { HasMember = IsMemberPresent::Value };

    typedef typename Select < HasMember, TypeList<MemberInfo<Enumerate<EL>, IsMemberPresent::Version>, LT>, LT >::Result NewListType;
    typedef CreateMemberIndicesInternal < NewListType, SL, EL - 1, ObjectType > MemberIndicesInternal;
    typedef typename MemberIndicesInternal::Indices Indices;
};

// ---------------------------------------------------------------------------------------------- //

template <class LT, int SL, class ObjectType>
struct CreateMemberIndicesInternal<LT, SL, SL, ObjectType>
{
    typedef LT Indices;
};

// ---------------------------------------------------------------------------------------------- //

template <int SL, int EL, class ObjectType>
struct CreateMemberIndices
{
    typedef CreateMemberIndicesInternal < NullType, SL
    , EL - 1, ObjectType > MemberIndicesInternal;
    typedef typename MemberIndicesInternal::Indices Indices;
};

// ---------------------------------------------------------------------------------------------- //

class Polymorphic
{
private:
    Polymorphic();

    static std::auto_ptr<Polymorphic> Serialiser;

public:
    static Polymorphic& GetInstance();

    bool IsPolymorphic(const std::string& className) const;

    typedef void* (*DeserialiseFunction)(ReadBuffer&, int);
    int RegisterDeserialise(const std::string& className, DeserialiseFunction func);
    void* Deserialise(const std::string& className, ReadBuffer& buffer, int version) const;

    typedef void* (*CastFunction)(void*);
    int RegisterCast(const std::string& from, const std::string& to, CastFunction func);
    CastFunction GetCast(const std::string& from, const std::string& to) const;
    bool HasCast(const std::string& from, const std::string& to) const;

private:
    std::map<std::string, DeserialiseFunction> m_functions;

    typedef std::map< std::string, std::map<std::string, CastFunction> > CastMap;
    CastMap m_Casts;
};

// ---------------------------------------------------------------------------------------------- //

template <class ElementType>
class WeakPtrWrapperInternal : public WeakPtrWrapper::Internal
{
public:
    WeakPtrWrapperInternal(boost::weak_ptr<ElementType>& ptr) : m_ptr(ptr) { }

    virtual void assign(const boost::shared_ptr<void>& ptr, const std::string& sharedName)
    {
        std::string weakName = ElementType::staticClassName();
        void* p = ptr.get();

        if(Polymorphic::GetInstance().HasCast(sharedName, weakName))
            p = Polymorphic::GetInstance().GetCast(sharedName, weakName)(p);

        m_ptr = boost::shared_ptr<ElementType>(ptr, reinterpret_cast<ElementType*>(p));
    }

private:
    boost::weak_ptr<ElementType>& m_ptr;
};

// ---------------------------------------------------------------------------------------------- //

class Repository
{
private:
    Repository();

    static std::auto_ptr<Repository> repository;

public:
    static Repository& GetInstance();

    void reset();

    bool IsRegistered(void* p) const;
    int Get(void* p) const;
    int add(void* p);

    int resrveIndex(void* p);
    bool IsReserved(void* p);
    int GetReservedIndex(void* p);
    void clearReserved(void* p);

    bool IsRegistered(const int& i) const;
    boost::shared_ptr<void> Get(const int& i) const;
    void add(const int& i, boost::shared_ptr<void> p, const std::string& className);
    std::string GetPointerTypeName(const int& i) const;

    bool HasDelayedAssignment(const int& index) const;

    template <class ElementType>
    void delayAssignment(const int& index, boost::weak_ptr<ElementType>& ptr)
    {
        boost::shared_ptr<WeakPtrWrapper::Internal> wrapperInternal(new WeakPtrWrapperInternal<ElementType>(ptr));
        m_delayed[index].push_back(WeakPtrWrapper(wrapperInternal));
    }

    void Assign(const int& index, boost::shared_ptr<void> voidPtr, std::string sharedName);

private:
    std::map< void*, int > m_indices;
    std::map< void*, int > m_reserved;
    std::map< int, std::pair<boost::shared_ptr<void>, std::string> > m_ptrs;
    std::map< int, std::vector<WeakPtrWrapper> > m_delayed;
    int m_nextIndex;
};

// ---------------------------------------------------------------------------------------------- //

template <class ObjectType>
void Deserialise(ObjectType& value, ReadBuffer& buffer, int version)
{
    typedef typename ObjectType::BaseClasses ListType;
    BaseClassSerializer<ObjectType>::Deserialise(value, ListType(), buffer, version);

    typedef typename ObjectType::MemberIndices Indices;
    ClassSerializer<ObjectType>::Deserialise(value, Indices(), buffer, version);

    ObjectType::initialize(value, version);
}

template <class ElementType>
void Deserialise(std::vector<ElementType>& elements, ReadBuffer& buffer, int version)
{
    typename std::vector<ElementType>::size_type size;
    buffer.Read(size);

    for(typename std::vector<ElementType>::size_type i = 0; i < size; ++i)
    {
        ElementType el;
        Deserialise(el, buffer, version);
        elements.push_back(el);
    }
}

template <class ElementType>
void Deserialise(std::list<ElementType>& elements, ReadBuffer& buffer, int version)
{
    typename std::list<ElementType>::size_type size;
    buffer.Read(size);

    for(typename std::list<ElementType>::size_type i = 0; i < size; ++i)
    {
        ElementType el;
        Deserialise(el, buffer, version);
        elements.push_back(el);
    }
}

template <class KeyType, class ValueType>
void Deserialise(std::map<KeyType, ValueType>& elements, ReadBuffer& buffer, int version)
{
    typename std::map<KeyType, ValueType>::size_type size;
    buffer.Read(size);

    for(typename std::map<KeyType, ValueType>::size_type i = 0; i < size; ++i)
    {
        KeyType key;
        Deserialise(key, buffer, version);
        ValueType val;
        Deserialise(val, buffer, version);
        elements[key] = val;
    }
}

template <class ElementType>
void Deserialise(boost::shared_ptr<ElementType>& ptr, ReadBuffer& buffer, int version)
{
    int index;
    buffer.Read(index);

    if(index == 0)
        return;

    boost::shared_ptr<void> voidPtr;
    std::string staticClassName(ElementType::staticClassName());

    if(Repository::GetInstance().IsRegistered(index))
    {
        voidPtr = Repository::GetInstance().Get(index);
        void* p = voidPtr.get();

        std::string className(Repository::GetInstance().GetPointerTypeName(index));

        if(Polymorphic::GetInstance().HasCast(className, staticClassName))
            p = Polymorphic::GetInstance().GetCast(className, staticClassName)(p);

        ptr = boost::shared_ptr<ElementType>(voidPtr, reinterpret_cast<ElementType*>(p));
    }
    else
    {
        if(Polymorphic::GetInstance().IsPolymorphic(staticClassName))
        {
            std::string className;
            buffer.Read(className);
            void* p = Polymorphic::GetInstance().Deserialise(className, buffer, version);

            // we can't delete p as long as it Is a void pointer
            if(Polymorphic::GetInstance().HasCast(className, staticClassName))
                p = Polymorphic::GetInstance().GetCast(className, staticClassName)(p);

            ptr = boost::shared_ptr<ElementType>(reinterpret_cast<ElementType*>(p));
        }
        else
        {
            ptr = boost::shared_ptr<ElementType>(new ElementType());
            Deserialise(*ptr, buffer, version);
        }

        voidPtr = boost::shared_ptr<void>(ptr);
        Repository::GetInstance().add(index, voidPtr, staticClassName);

        if(Repository::GetInstance().HasDelayedAssignment(index))
            Repository::GetInstance().Assign(index, voidPtr, staticClassName);
    }
}

template <class ElementType>
void Deserialise(boost::weak_ptr<ElementType>& ptr, ReadBuffer& buffer, int version)
{
    int index;
    buffer.Read(index);

    if(index == 0)
        return;

    if(Repository::GetInstance().IsRegistered(index))
    {
        boost::shared_ptr<void> voidPtr = Repository::GetInstance().Get(index);
        void* p = voidPtr.get();

        std::string className(Repository::GetInstance().GetPointerTypeName(index));
        std::string staticClassName(ElementType::staticClassName());

        if(Polymorphic::GetInstance().HasCast(className, staticClassName))
            p = Polymorphic::GetInstance().GetCast(className, staticClassName)(p);

        ptr = boost::shared_ptr<ElementType>(voidPtr, reinterpret_cast<ElementType*>(p));
    }
    else
        Repository::GetInstance().delayAssignment(index, ptr);
}

template <class ElementType>
void Deserialise(std::auto_ptr<ElementType>& ptr, ReadBuffer& buffer, int version)
{
    int index;
    buffer.Read(index);

    if(index == 0)
        return;

    std::string staticClassName(ElementType::staticClassName());

    if(Polymorphic::GetInstance().IsPolymorphic(staticClassName))
    {
        std::string className;
        buffer.Read(className);
        void* p = Polymorphic::GetInstance().Deserialise(className, buffer, version);

        // we can't delete p as long as it Is a void pointer
        if(Polymorphic::GetInstance().HasCast(className, staticClassName))
            p = Polymorphic::GetInstance().GetCast(className, staticClassName)(p);

        ptr = std::auto_ptr<ElementType>(reinterpret_cast<ElementType*>(p));
    }
    else
    {
        ptr = std::auto_ptr<ElementType>(new ElementType());
        Deserialise(*ptr, buffer, version);
    }
}

template <>
void Deserialise(bool& value, ReadBuffer& buffer, int version)
{
    buffer.Read(value);
}

template <>
void Deserialise(char& value, ReadBuffer& buffer, int version)
{
    buffer.Read(value);
}

template <>
void Deserialise(short& value, ReadBuffer& buffer, int version)
{
    buffer.Read(value);
}

template <>
void Deserialise(int& value, ReadBuffer& buffer, int version)
{
    buffer.Read(value);
}

template <>
void Deserialise(long& value, ReadBuffer& buffer, int version)
{
    buffer.Read(value);
}

template <>
void Deserialise(unsigned char& value, ReadBuffer& buffer, int version)
{
    buffer.Read(value);
}

template <>
void Deserialise(unsigned short& value, ReadBuffer& buffer, int version)
{
    buffer.Read(value);
}

template <>
void Deserialise(unsigned int& value, ReadBuffer& buffer, int version)
{
    buffer.Read(value);
}

template <>
void Deserialise(unsigned long& value, ReadBuffer& buffer, int version)
{
    buffer.Read(value);
}

template <>
void Deserialise(float& value, ReadBuffer& buffer, int version)
{
    buffer.Read(value);
}

template <>
void Deserialise(double& value, ReadBuffer& buffer, int version)
{
    buffer.Read(value);
}

template <>
void Deserialise(std::string& value, ReadBuffer& buffer, int version)
{
    buffer.Read(value);
}

// ---------------------------------------------------------------------------------------------- //

template <class ObjectType>
void Serialise(const ObjectType& object, WriteBuffer& buffer, int version)
{
    typedef typename ObjectType::BaseClasses BaseClasses;
    BaseClassSerializer<ObjectType>::Serialise(object, BaseClasses(), buffer, version);

    typedef typename ObjectType::MemberIndices Indices;
    ClassSerializer<ObjectType>::Serialise(object, Indices(), buffer, version);
}

template <class ElementType>
void Serialise(const std::vector<ElementType>& elements, WriteBuffer& buffer, int version)
{
    buffer.Append(elements.size());

    for(typename std::vector<ElementType>::const_iterator iter = elements.begin(); iter != elements.end(); ++iter)
        Serialise(*iter, buffer, version);
}

template <class ElementType>
void Serialise(const std::list<ElementType>& elements, WriteBuffer& buffer, int version)
{
    buffer.Append(elements.size());

    for(typename std::list<ElementType>::const_iterator iter = elements.begin(); iter != elements.end(); ++iter)
        Serialise(*iter, buffer, version);
}

template <class KeyType, class ValueType>
void Serialise(const std::map<KeyType, ValueType>& elements, WriteBuffer& buffer, int version)
{
    buffer.Append(elements.size());

    for(typename std::map<KeyType, ValueType>::const_iterator iter = elements.begin(); iter != elements.end(); ++iter)
    {
        Serialise(iter->first, buffer, version);
        Serialise(iter->second, buffer, version);
    }
}

template <class ElementType>
void Serialise(const boost::shared_ptr<ElementType>& ptr, WriteBuffer& buffer, int version)
{
    if(ptr.get() == 0)
    {
        buffer.Append((int)0);
        return;
    }

    void* p = reinterpret_cast<void*>(ptr.get());
    std::string className = ptr->className();
    std::string staticClassName = ElementType::staticClassName();

    if(Polymorphic::GetInstance().HasCast(staticClassName, className))
        p = Polymorphic::GetInstance().GetCast(staticClassName, className)(p);

    if(Repository::GetInstance().IsRegistered(p))
        buffer.Append(Repository::GetInstance().Get(p));
    else
    {
        if(Repository::GetInstance().IsReserved(p))
        {
            buffer.Append(Repository::GetInstance().GetReservedIndex(p));
            Repository::GetInstance().clearReserved(p);
        }
        else
            buffer.Append(Repository::GetInstance().add(p));

        if(Polymorphic::GetInstance().IsPolymorphic(className))
        {
            buffer.Append(className);
            ptr->Serialise(buffer, version);
        }
        else
            Serialise(*ptr, buffer, version);
    }
}

template <class ElementType>
void Serialise(const boost::weak_ptr<ElementType>& ptr, WriteBuffer& buffer, int version)
{
    boost::shared_ptr<ElementType> shared = ptr.lock();

    if(shared.get() == 0)
    {
        buffer.Append((int)0);
        return;
    }

    void* p = reinterpret_cast<void*>(shared.get());
    std::string className = shared->className();
    std::string staticClassName = ElementType::staticClassName();

    if(Polymorphic::GetInstance().HasCast(staticClassName, className))
        p = Polymorphic::GetInstance().GetCast(staticClassName, className)(p);

    if(Repository::GetInstance().IsRegistered(p))
        buffer.Append(Repository::GetInstance().Get(p));
    else
        buffer.Append(Repository::GetInstance().resrveIndex(p));
}

template <class ElementType>
void Serialise(const std::auto_ptr<ElementType>& ptr, WriteBuffer& buffer, int version)
{
    if(ptr.get() == 0)
    {
        buffer.Append((int)0);
        return;
    }

    buffer.Append((int)1);
    std::string className(ptr->className());

    if(Polymorphic::GetInstance().IsPolymorphic(className))
    {
        buffer.Append(className);
        ptr->Serialise(buffer, version);
    }
    else
        Serialise(*ptr, buffer, version);
}

template <>
void Serialise(const bool& value, WriteBuffer& buffer, int version)
{
    buffer.Append(value);
}

template <>
void Serialise(const char& value, WriteBuffer& buffer, int version)
{
    buffer.Append(value);
}

template <>
void Serialise(const short& value, WriteBuffer& buffer, int version)
{
    buffer.Append(value);
}

template <>
void Serialise(const int& value, WriteBuffer& buffer, int version)
{
    buffer.Append(value);
}

template <>
void Serialise(const long& value, WriteBuffer& buffer, int version)
{
    buffer.Append(value);
}

template <>
void Serialise(const unsigned char& value, WriteBuffer& buffer, int version)
{
    buffer.Append(value);
}

template <>
void Serialise(const unsigned short& value, WriteBuffer& buffer, int version)
{
    buffer.Append(value);
}

template <>
void Serialise(const unsigned int& value, WriteBuffer& buffer, int version)
{
    buffer.Append(value);
}

template <>
void Serialise(const unsigned long& value, WriteBuffer& buffer, int version)
{
    buffer.Append(value);
}

template <>
void Serialise(const float& value, WriteBuffer& buffer, int version)
{
    buffer.Append(value);
}

template <>
void Serialise(const double& value, WriteBuffer& buffer, int version)
{
    buffer.Append(value);
}

template <>
void Serialise(const std::string& value, WriteBuffer& buffer, int version)
{
    buffer.Append(value);
}

// ---------------------------------------------------------------------------------------------- //

template <int alignment_, class ObjectType>
void Serialise(const ObjectType& object, WriteBuffer& buffer, int version);

template <int alignment_, class ObjectType>
void Deserialise(ObjectType& value, const ReadBuffer& buffer, int version);

// ---------------------------------------------------------------------------------------------- //

} // namespace Core

// ---------------------------------------------------------------------------------------------- //

template <int alignment_, int endian_, class ObjectType>
char* Serialise(const ObjectType& object, long& length, int version = 1)
{
    Core::Repository::GetInstance().reset();
    std::auto_ptr<Core::WriteBuffer::Internal> internal(new Core::WriteBufferInternal<alignment_, endian_>());
    Core::WriteBuffer buffer(internal);
    buffer.Append(version);
    Core::Serialise(object, buffer, version);
    Core::Repository::GetInstance().reset();
    length = buffer.GetLength();
    return buffer.GetData();
}

template <int alignment_, class ObjectType>
char* Serialise(const ObjectType& object, long& length, int version = 1)
{
    return Serialise<alignment_, VD_BYTE_ORDER, ObjectType>(object, length, version);
}

template <class ObjectType>
char* Serialise(const ObjectType& object, long& length, int version = 1)
{
    return Serialise<VD_MEMORY_ALIGNMENT, VD_BYTE_ORDER, ObjectType>(object, length, version);
}

// ---------------------------------------------------------------------------------------------- //

template <class ObjectType, int alignment_, int endian_>
ObjectType* Deserialise(const char* bytes, int version = 1)
{
    Core::Repository::GetInstance().reset();
    ObjectType* ptr;

    try
    {
        ObjectType* ptr = new ObjectType();
        std::auto_ptr<Core::ReadBuffer::Internal> internal(new Core::ReadBufferInternal<alignment_, endian_>(bytes));
        Core::ReadBuffer buffer(internal);
        
        int data_version;
        buffer.Read(data_version);

        if(data_version != version)
            throw Exception("Deserialise: version mismatch");

        Core::Deserialise(*ptr, buffer, version);
        Core::Repository::GetInstance().reset();
        return ptr;
    }
    catch(...) { delete ptr; throw; }
}

template <class ObjectType, int alignment_>
ObjectType* Deserialise(const char* bytes, int version = 1)
{
    return Deserialise<ObjectType, alignment_, VD_BYTE_ORDER>(bytes, version);
}

template <class ObjectType>
ObjectType* Deserialise(const char* bytes, int version = 1)
{
    return Deserialise<ObjectType, VD_MEMORY_ALIGNMENT, VD_BYTE_ORDER>(bytes, version);
}

// ============================================================================================== //

} // namespace vd

// ============================================================================================== //

#define VD_BEGIN_MEMBERS(ObjectType)                                      \
private:                                                                       \
   typedef ObjectType ConcreteClass;                                       \
   enum { StartLine = __LINE__ };                                              \
public:                                                                        \
   ObjectType() { }                                                        \
   template <class Dummy_>                                                     \
   static void initialize(Dummy_&, int) { }                                    \
                                                                               \
   virtual const char* className() { return #ObjectType; }                 \
   static const char* staticClassName() { return #ObjectType; }            \
                                                                               \
   virtual void Serialise(vd::Core::WriteBuffer& buffer, int version) const\
   {                                                                           \
      vd::Core::Serialise(*this, buffer, version);                         \
   }                                                                           \
   static void* Deserialise(vd::Core::ReadBuffer& buffer, int version)     \
   {                                                                           \
      ObjectType* ptr;                                                     \
      try                                                                      \
      {                                                                        \
         ptr = new ObjectType();                                           \
         vd::Core::Deserialise(*ptr, buffer, version);                     \
         return reinterpret_cast<void*>(ptr);                                  \
      }                                                                        \
      catch(...) { delete ptr; throw; }                                        \
   }                                                                           \
   static int RegisterDeserialiser[];                                          \
                                                                               \
   template <int lineNb_, class Dummy_ = vd::Core::NullType>               \
   struct IsMemberPresent                                                      \
   {                                                                           \
      enum { Value = false };                                                  \
      enum { Version = 0 };                                                    \
   };                                                                          \
                                                                               \
   template <int lineNb_, class Dummy_ = vd::Core::NullType>               \
   struct IsBaseClassPresent                                                   \
   {                                                                           \
      enum { Value = false };                                                  \
      typedef vd::Core::NullType BaseClass;                                \
   };


#define VD_ADD_MEMBER(access, DataType, name)                              \
access:                                                                    \
   DataType name;                                                          \
private:                                                                   \
   typedef DataType ConcreteClass::* MemberPtr##name;                      \
public:                                                                    \
   template <class Dummy_>                                                 \
   struct IsMemberPresent<__LINE__, Dummy_>                                \
   {                                                                       \
      enum { Value = true };                                               \
      enum { Version = 1 };                                                \
   };                                                                      \
   static MemberPtr##name GetMemberPtr(vd::Core::Enumerate<__LINE__>)      \
      { return &ConcreteClass::name; }


#define VD_ADD_MEMBER_V(access, DataType, name, version)                   \
access:                                                                    \
   DataType name;                                                          \
private:                                                                   \
   typedef DataType ConcreteClass::* MemberPtr##name;                      \
public:                                                                    \
   template <class Dummy_>                                                 \
   struct IsMemberPresent<__LINE__, Dummy_>                                \
   {                                                                       \
      enum { Value = true };                                               \
      enum { Version = version };                                          \
   };                                                                      \
   static MemberPtr##name GetMemberPtr(vd::Core::Enumerate<__LINE__>)   \
      { return &ConcreteClass::name; }



#define VD_BASE_CLASS(BaseClass_)                                         \
public:                                                                    \
   template <class Dummy_>                                                 \
   struct IsBaseClassPresent<__LINE__, Dummy_>                             \
   {                                                                       \
      enum { Value = true };                                               \
      typedef BaseClass_ BaseClass;                                        \
   };


#define VD_END_MEMBERS()                                                           \
private:                                                                            \
   enum { EndLine = __LINE__ };                                                     \
public:                                                                             \
   typedef vd::Core::CreateMemberIndices<StartLine, EndLine, ConcreteClass>     \
              ::Indices MemberIndices;                                              \
   typedef vd::Core::CreateBaseClassList<StartLine, EndLine, ConcreteClass>     \
              ::ListType BaseClasses;



#define VD_POLYMORPH(ClassName_)                                          \
int ClassName_::RegisterDeserialiser[] =                                   \
{                                                                          \
   vd::Core::Polymorphic::GetInstance().RegisterDeserialise(    \
         #ClassName_, &ClassName_::Deserialise)                            \
};


#define VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T1_)                     \
   vd::Core::Polymorphic::GetInstance().RegisterCast(           \
         #T0_, #T1_, &vd::Core::dynamic_reinterpret_cast<T0_, T1_>),   \
   vd::Core::Polymorphic::GetInstance().RegisterCast(           \
         #T1_, #T0_, &vd::Core::dynamic_reinterpret_cast<T1_, T0_>)


#define VD_POLYMORPH_MULTIPLE_2(Class_, T0_, T1_)                         \
int Class_::RegisterDeserialiser[] =                                       \
{                                                                          \
   vd::Core::Polymorphic::GetInstance().RegisterDeserialise(    \
         #Class_, &Class_::Deserialise),                                   \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T0_),                      \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T1_),                      \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T1_)                          \
};

#define VD_POLYMORPH_MULTIPLE_3(Class_, T0_, T1_, T2_)                    \
int Class_::RegisterDeserialiser[] =                                       \
{                                                                          \
   vd::Core::Polymorphic::GetInstance().RegisterDeserialise(    \
         #Class_, &Class_::Deserialise),                                   \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T0_),                      \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T1_),                      \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T2_),                      \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T1_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T2_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T2_)                          \
};

#define VD_POLYMORPH_MULTIPLE_4(Class_, T0_, T1_, T2_, T3_)               \
int Class_::RegisterDeserialiser[] =                                       \
{                                                                          \
   vd::Core::Polymorphic::GetInstance().RegisterDeserialise(    \
         #Class_, &Class_::Deserialise),                                   \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T0_),                      \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T1_),                      \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T2_),                      \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T3_),                      \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T1_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T2_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T3_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T2_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T3_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T2_, T3_)                          \
};

#define VD_POLYMORPH_MULTIPLE_5(Class_, T0_, T1_, T2_, T3_, T4_)          \
int Class_::RegisterDeserialiser[] =                                       \
{                                                                          \
   vd::Core::Polymorphic::GetInstance().RegisterDeserialise(    \
         #Class_, &Class_::Deserialise),                                   \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T0_),                      \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T1_),                      \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T2_),                      \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T3_),                      \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T4_),                      \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T1_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T2_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T3_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T4_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T2_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T3_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T4_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T2_, T3_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T2_, T4_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T3_, T4_)                          \
};

#define VD_POLYMORPH_MULTIPLE_6(Class_, T0_, T1_, T2_, T3_, T4_, T5_)     \
int Class_::RegisterDeserialiser[] =                                       \
{                                                                          \
   vd::Core::Polymorphic::GetInstance().RegisterDeserialise(    \
         #Class_, &Class_::Deserialise),                                   \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T0_),                      \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T1_),                      \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T2_),                      \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T3_),                      \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T4_),                      \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(Class_, T5_),                      \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T1_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T2_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T3_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T4_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T0_, T5_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T2_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T3_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T4_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T1_, T5_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T2_, T3_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T2_, T4_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T2_, T5_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T3_, T4_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T3_, T5_),                         \
   VD_POLYMORPH_MULTIPLE_REGISTER_CAST(T4_, T5_)                          \
};

// ============================================================================================== //

#endif

