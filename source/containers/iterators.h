// ============================================================================================== //
//
// License:             The Lesser GNU Public License (LGPL) v3.0.
//
// Author(s):   Derek Gerstmann. The University of Western Australia (UWA).
//                              As well as the shoulders of many giants...
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

#ifndef VD_CONTAINERS_ARRAY_ITERATORS_HEADER_INCLUDED
#define VD_CONTAINERS_ARRAY_ITERATORS_HEADER_INCLUDED

// ============================================================================================== //

#include "containers/containers.h"

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_BEGIN();

// ============================================================================================== //

template< typename T >
class ConstArrayIterator
{
public:
    typedef T ValueType;
    typedef T* PointerType;
    typedef T& ReferenceType;
    typedef const T* ConstPointerType;
    typedef const T& ConstReferenceType;

    ConstArrayIterator();
    explicit ConstArrayIterator(const T* pElement);

    ConstReferenceType operator*() const;
    ConstPointerType operator->() const;

    ConstArrayIterator& operator++();
    ConstArrayIterator operator++(int);
    ConstArrayIterator& operator--();
    ConstArrayIterator operator--(int);

    ConstArrayIterator& operator+=(ptrdiff_t offset);
    ConstArrayIterator operator+(ptrdiff_t offset) const;
    ConstArrayIterator& operator-=(ptrdiff_t offset);
    ConstArrayIterator operator-(ptrdiff_t offset) const;

    ptrdiff_t operator-(const ConstArrayIterator& rOther) const;

    bool operator==(const ConstArrayIterator& rOther) const;
    bool operator!=(const ConstArrayIterator& rOther) const;
    bool operator<(const ConstArrayIterator& rOther) const;
    bool operator>(const ConstArrayIterator& rOther) const;
    bool operator<=(const ConstArrayIterator& rOther) const;
    bool operator>=(const ConstArrayIterator& rOther) const;

protected:
    PointerType m_CursorPtr;
};

template< typename T >
class ArrayIterator : public ConstArrayIterator< T >
{
public:
    typedef T ValueType;
    typedef T* PointerType;
    typedef T& ReferenceType;
    typedef const T* ConstPointerType;
    typedef const T& ConstReferenceType;

    ArrayIterator();
    explicit ArrayIterator(T* pElement);

    ReferenceType operator*() const;
    PointerType operator->() const;

    ArrayIterator& operator++();
    ArrayIterator operator++(int);
    ArrayIterator& operator--();
    ArrayIterator operator--(int);

    ArrayIterator& operator+=(ptrdiff_t offset);
    ArrayIterator operator+(ptrdiff_t offset) const;
    ArrayIterator& operator-=(ptrdiff_t offset);
    ArrayIterator operator-(ptrdiff_t offset) const;

    ptrdiff_t operator-(const ConstArrayIterator& rOther) const;

};

template< typename T >
ConstArrayIterator< T >::ConstArrayIterator()
{

}

template< typename T >
ConstArrayIterator< T >::ConstArrayIterator(const T* pElement)
    : m_CursorPtr(const_cast< T* >(pElement))
{
}

template< typename T >
typename ConstArrayIterator< T >::ConstReferenceType ConstArrayIterator< T >::operator*() const
{
    return *m_CursorPtr;
}

template< typename T >
typename ConstArrayIterator< T >::ConstPointerType ConstArrayIterator< T >::operator->() const
{
    return m_CursorPtr;
}

template< typename T >
ConstArrayIterator< T >& ConstArrayIterator< T >::operator++()
{
    ++m_CursorPtr;

    return *this;
}

template< typename T >
ConstArrayIterator< T > ConstArrayIterator< T >::operator++(int)
{
    ConstArrayIterator result = *this;
    ++m_CursorPtr;

    return result;
}

template< typename T >
ConstArrayIterator< T >& ConstArrayIterator< T >::operator--()
{
    --m_CursorPtr;
    return *this;
}

template< typename T >
ConstArrayIterator< T > ConstArrayIterator< T >::operator--(int)
{
    ConstArrayIterator result = *this;
    --m_CursorPtr;
    return result;
}

template< typename T >
ConstArrayIterator< T >& ConstArrayIterator< T >::operator+=(ptrdiff_t offset)
{
    m_CursorPtr += offset;
    return *this;
}

template< typename T >
ConstArrayIterator< T > ConstArrayIterator< T >::operator+(ptrdiff_t offset) const
{
    return ConstArrayIterator(m_CursorPtr + offset);
}

template< typename T >
ConstArrayIterator< T >& ConstArrayIterator< T >::operator-=(ptrdiff_t offset)
{
    m_CursorPtr -= offset;
    return *this;
}

template< typename T >
ConstArrayIterator< T > ConstArrayIterator< T >::operator-(ptrdiff_t offset) const
{
    return ConstArrayIterator(m_CursorPtr - offset);
}

template< typename T >
ptrdiff_t ConstArrayIterator< T >::operator-(const ConstArrayIterator& rOther) const
{
    return (m_CursorPtr - rOther.m_CursorPtr);
}

template< typename T >
bool ConstArrayIterator< T >::operator==(const ConstArrayIterator& rOther) const
{
    return (m_CursorPtr == rOther.m_CursorPtr);
}

template< typename T >
bool ConstArrayIterator< T >::operator!=(const ConstArrayIterator& rOther) const
{
    return (m_CursorPtr != rOther.m_CursorPtr);
}

template< typename T >
bool ConstArrayIterator< T >::operator<(const ConstArrayIterator& rOther) const
{
    return (m_CursorPtr < rOther.m_CursorPtr);
}

template< typename T >
bool ConstArrayIterator< T >::operator>(const ConstArrayIterator& rOther) const
{
    return (m_CursorPtr > rOther.m_CursorPtr);
}

template< typename T >
bool ConstArrayIterator< T >::operator<=(const ConstArrayIterator& rOther) const
{
    return (m_CursorPtr <= rOther.m_CursorPtr);
}

template< typename T >
bool ConstArrayIterator< T >::operator>=(const ConstArrayIterator& rOther) const
{
    return (m_CursorPtr >= rOther.m_CursorPtr);
}

template< typename T >
ArrayIterator< T >::ArrayIterator()
{
}

template< typename T >
ArrayIterator< T >::ArrayIterator(T* pElement)
    : ConstArrayIterator< T >(pElement)
{
}

template< typename T >
typename ArrayIterator< T >::ReferenceType ArrayIterator< T >::operator*() const
{
    return *ConstArrayIterator< T >::m_CursorPtr;
}

template< typename T >
typename ArrayIterator< T >::PointerType ArrayIterator< T >::operator->() const
{
    return ConstArrayIterator< T >::m_CursorPtr;
}

template< typename T >
ArrayIterator< T >& ArrayIterator< T >::operator++()
{
    ++ConstArrayIterator< T >::m_CursorPtr;

    return *this;
}

template< typename T >
ArrayIterator< T > ArrayIterator< T >::operator++(int)
{
    ArrayIterator result = *this;
    ++ConstArrayIterator< T >::m_CursorPtr;

    return result;
}

template< typename T >
ArrayIterator< T >& ArrayIterator< T >::operator--()
{
    --ConstArrayIterator< T >::m_CursorPtr;

    return *this;
}

template< typename T >
ArrayIterator< T > ArrayIterator< T >::operator--(int)
{
    ArrayIterator result = *this;
    --ConstArrayIterator< T >::m_CursorPtr;

    return result;
}

template< typename T >
ArrayIterator< T >& ArrayIterator< T >::operator+=(ptrdiff_t offset)
{
    ConstArrayIterator< T >::m_CursorPtr += offset;

    return *this;
}

template< typename T >
ArrayIterator< T > ArrayIterator< T >::operator+(ptrdiff_t offset) const
{
    return ArrayIterator(ConstArrayIterator< T >::m_CursorPtr + offset);
}

template< typename T >
ArrayIterator< T >& ArrayIterator< T >::operator-=(ptrdiff_t offset)
{
    ConstArrayIterator< T >::m_CursorPtr -= offset;

    return *this;
}

template< typename T >
ArrayIterator< T > ArrayIterator< T >::operator-(ptrdiff_t offset) const
{
    return ArrayIterator(ConstArrayIterator< T >::m_CursorPtr - offset);
}

template< typename T >
ptrdiff_t ArrayIterator< T >::operator-(
    const ConstArrayIterator& rOther) const
{
    return ConstArrayIterator< T >::operator-(rOther);
}

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CONTAINERS_ARRAY_ITERATORS_HEADER_INCLUDED

