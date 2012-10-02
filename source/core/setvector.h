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

#ifndef VD_CORE_SETVECTOR_INCLUDED
#define VD_CORE_SETVECTOR_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/collections.h"

// ============================================================================================== //

#include <list>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <set>

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

template <
    typename Key, 
    class Traits = std::less<Key>,
    class Alloc = std::allocator<Key> 
>
class SetVector
{
public:
    typedef Key                                             KeyType;
    typedef Key                                             ValueType;
    
    typedef Traits                                          KeyCmpOp;
    typedef Traits                                          ValueCmpOp;
    typedef Traits                                          CompareType;
    
    typedef Alloc                                           AllocatorType;
    typedef typename Core::Vector<Key, Alloc>::type         ContainerType;
 
    typedef typename ContainerType::const_iterator          const_iterator;
    typedef typename ContainerType::const_reverse_iterator  const_reverse_iterator;
    typedef typename ContainerType::iterator                iterator;
    typedef typename ContainerType::reverse_iterator        reverse_iterator;

    typedef typename ContainerType::size_type               size_type;
    typedef typename ContainerType::difference_type         difference_type;

    typedef typename AllocatorType::pointer                 pointer;
    typedef typename AllocatorType::const_pointer           const_pointer;
    typedef typename AllocatorType::reference               reference;
    typedef typename AllocatorType::const_reference         const_reference;

public:
    explicit SetVector(
        KeyCmpOp const & comp = KeyCmpOp(),
        Alloc const & alloc = Alloc()
    ) : 
        m_Container(alloc), m_CompareOp(comp)
    { 
        // EMPTY!
    }

    template <class InputIterator>
    SetVector(
        InputIterator first, 
        InputIterator last,
        KeyCmpOp const & comp = KeyCmpOp(),
        Alloc const & alloc = Alloc()
    ) : 
        m_Container(first, last, alloc), 
        m_CompareOp(comp)
    { 
        std::sort(this->begin(), this->end(), m_CompareOp); 
    }

    SetVector& operator=(SetVector const & rhs)
    {
        SetVector(rhs).swap(*this);
        return *this;
    }

    iterator begin()
        { return m_Container.begin(); }

    const_iterator begin() const
        { return m_Container.begin(); }

    iterator end()
        { return m_Container.end(); }

    const_iterator end() const
        { return m_Container.end(); }

    void clear()
        { m_Container.clear(); }

    bool empty() const
        { return m_Container.empty(); }

    size_type max_size() const
        { return m_Container.max_size(); }

    size_type size() const
        { return m_Container.size(); }

    size_type count(Key const & key) const
        { return this->find(key) != end(); }

    std::pair<iterator, iterator> equal_range(Key const & key)
        { return std::equal_range(this->begin(), this->end(), key, m_CompareOp); }

    std::pair<const_iterator, const_iterator> equal_range(Key const & key) const
        { return std::equal_range(this->begin(), this->end(), key, m_CompareOp); }

    iterator erase(iterator where)
        { return m_Container.erase(where); }

    iterator erase(iterator first, iterator last)
        { return m_Container.erase(first, last); }
    
    size_type erase(KeyType const & key)
    {
        iterator iter(this->find(key));
        if (iter != this->end())
        {
            this->erase(iter);
            return 1;
        }
        return 0;
    }

    iterator find(Key const & key)
    {
        iterator iter(this->lower_bound(key));
        if (iter != this->end() && m_CompareOp(key, *iter))
        {
            iter = end();
        }
        return iter;
    }
    const_iterator find(Key const & key) const
    {
        const_iterator iter(this->lower_bound(key));
        if (iter != this->end() && m_CompareOp(key, *iter))
        {
            iter = end();
        }
        return iter;
    }

    AllocatorType get_allocator() const
    {
        return m_Container.get_allocator();
    }

    std::pair<iterator, bool> insert(ValueType const & val)
    {
        bool found(true);
        iterator iter(this->lower_bound(val));

        if ((iter == end()) || m_CompareOp(val, *iter))
        {
            iter = m_Container.insert(iter, val);
            found = false;
        }
        return std::make_pair(iter, !found);
    }

    iterator insert(iterator where, ValueType const & val)
    {
        if (where != this->end() && m_CompareOp(*where, val)
            && (where == this->end() - 1
                || (!m_CompareOp(val, where + 1)
                    && m_CompareOp(where + 1, val))))
        {
            return m_Container.insert(where, val);
        }
        return this->insert(val)->first;
    }

    template<class InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
        for (; first != last; ++ first)
        {
            this->insert(*first);
        }
    }

    KeyCmpOp key_comp() const
        { return m_CompareOp; }

    iterator lower_bound(Key const & key)
        { return std::lower_bound(this->begin(), this->end(), key, m_CompareOp); }

    const_iterator lower_bound(Key const & key) const
        { return std::lower_bound(this->begin(), this->end(), key, m_CompareOp); }

    iterator upper_bound(Key const & key)
        { return std::upper_bound(this->begin(), this->end(), key, m_CompareOp); }

    const_iterator upper_bound(Key const & key) const
        { return std::upper_bound(this->begin(), this->end(), key, m_CompareOp); }

    reverse_iterator rbegin()
        { return m_Container.rbegin(); }

    const_reverse_iterator rbegin() const
        { return m_Container.rbegin(); }

    reverse_iterator rend()
        { return m_Container.rend(); }

    const_reverse_iterator rend() const
        { return m_Container.rend(); }

    void swap(SetVector& rhs)
    {
        std::swap(m_Container, rhs.m_Container);
        std::swap(m_CompareOp, rhs.m_CompareOp);
    }

    ValueCmpOp value_comp() const
        { return ValueCmpOp(m_CompareOp); }

    friend bool operator==(SetVector const & lhs, SetVector const & rhs)
        { return lhs.m_Container == rhs.m_Container; }

    friend bool operator<(SetVector const & lhs, SetVector const & rhs)
        { return lhs.m_Container < rhs.m_Container; }

private:
    ContainerType   m_Container;
    CompareType     m_CompareOp;
};

// --------------------------------------------------------------------------------------------- //

template <
    class Key, 
    class Traits, 
    class Alloc
>
inline bool
operator!=(
    SetVector<Key, Traits, Alloc> const & lhs,
    SetVector<Key, Traits, Alloc> const & rhs)
{
    return !(lhs == rhs);
}

// --------------------------------------------------------------------------------------------- //

template <
    class Key, 
    class Traits, 
    class Alloc 
> 
inline bool
operator>(
    SetVector<Key, Traits, Alloc> const & lhs,
    SetVector<Key, Traits, Alloc> const & rhs)
{
    return rhs < lhs;
}

// --------------------------------------------------------------------------------------------- //

template <
    class Key, 
    class Traits, 
    class Alloc 
> 
inline bool
operator>=(
    SetVector<Key, Traits, Alloc> const & lhs,
    SetVector<Key, Traits, Alloc> const & rhs)
{
    return !(lhs < rhs);
}

// --------------------------------------------------------------------------------------------- //

template <
    class Key, 
    class Traits, 
    class Alloc 
> 
inline bool
operator<=(
    SetVector<Key, Traits, Alloc> const & lhs,
    SetVector<Key, Traits, Alloc> const & rhs)
{
    return !(rhs < lhs);
}
// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

namespace std
{
    VD_USING(Core, SetVector);
    
    template <
        class Key, 
        class Traits, 
        class Alloc 
    > 
    inline void
    swap(
        SetVector<Key, Traits, Alloc>& lhs, 
        SetVector<Key, Traits, Alloc>& rhs)
    {
        lhs.swap(rhs);
    }
}

// ============================================================================================== //

#endif // VD_CORE_SETVECTOR_INCLUDED

