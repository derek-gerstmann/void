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

#ifndef VD_CONTAINERS_SET_INCLUDED
#define VD_CONTAINERS_SET_INCLUDED

// ============================================================================================== //

#include "containers/namespace.h"
#include "core/core.h"
#include "core/allocators.h"

// ============================================================================================== //

#include <list>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <set>

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_BEGIN();

// ============================================================================================== //

template <typename Key, class Traits = std::less<Key>,
            class Allocator = std::allocator<Key> >
class SetVector
{
    typedef std::vector<Key, Allocator> ContainerType;
    typedef Traits CompareType;

public:
    typedef Traits          key_compare;
    typedef Traits          value_compare;
    typedef Key             key_type;
    typedef Key             value_type;
    typedef Allocator       allocator_type;

    typedef typename ContainerType::const_iterator          const_iterator;
    typedef typename ContainerType::const_reverse_iterator  const_reverse_iterator;
    typedef typename ContainerType::iterator                iterator;
    typedef typename ContainerType::reverse_iterator        reverse_iterator;

    typedef typename ContainerType::size_type               size_type;
    typedef typename ContainerType::difference_type         difference_type;

    typedef typename Allocator::pointer                 pointer;
    typedef typename Allocator::const_pointer           const_pointer;
    typedef typename Allocator::reference               reference;
    typedef typename Allocator::const_reference         const_reference;

public:
    explicit SetVector(key_compare const & comp = key_compare(),
                            Allocator const & alloc = Allocator())
                : container_(alloc), compare_(comp)
        { }

    template <class InputIterator>
    SetVector(InputIterator first, InputIterator last,
                    key_compare const & comp = key_compare(),
                    Allocator const & alloc = Allocator())
                : container_(first, last, alloc), compare_(comp)
        { std::sort(this->begin(), this->end(), compare_); }

    SetVector& operator=(SetVector const & rhs)
    {
        SetVector(rhs).swap(*this);
        return *this;
    }

    iterator begin()
        { return container_.begin(); }
    const_iterator begin() const
        { return container_.begin(); }

    iterator end()
        { return container_.end(); }
    const_iterator end() const
        { return container_.end(); }

    void clear()
        { container_.clear(); }

    bool empty() const
        { return container_.empty(); }

    size_type max_size() const
        { return container_.max_size(); }

    size_type size() const
        { return container_.size(); }

    size_type count(Key const & key) const
        { return this->find(key) != end(); }

    std::pair<iterator, iterator> equal_range(Key const & key)
        { return std::equal_range(this->begin(), this->end(), key, compare_); }
    std::pair<const_iterator, const_iterator> equal_range(Key const & key) const
        { return std::equal_range(this->begin(), this->end(), key, compare_); }

    iterator erase(iterator where)
        { return container_.erase(where); }
    iterator erase(iterator first, iterator last)
        { return container_.erase(first, last); }
    size_type erase(key_type const & key)
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
        if (iter != this->end() && compare_(key, *iter))
        {
            iter = end();
        }
        return iter;
    }
    const_iterator find(Key const & key) const
    {
        const_iterator iter(this->lower_bound(key));
        if (iter != this->end() && compare_(key, *iter))
        {
            iter = end();
        }
        return iter;
    }

    Allocator get_allocator() const
    {
        return container_.get_allocator();
    }

    std::pair<iterator, bool> insert(value_type const & val)
    {
        bool found(true);
        iterator iter(this->lower_bound(val));

        if ((iter == end()) || compare_(val, *iter))
        {
            iter = container_.insert(iter, val);
            found = false;
        }
        return std::make_pair(iter, !found);
    }
    iterator insert(iterator where, value_type const & val)
    {
        if (where != this->end() && compare_(*where, val)
            && (where == this->end() - 1
                || (!compare_(val, where + 1)
                    && compare_(where + 1, val))))
        {
            return container_.insert(where, val);
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

    key_compare key_comp() const
        { return compare_; }

    iterator lower_bound(Key const & key)
        { return std::lower_bound(this->begin(), this->end(), key, compare_); }
    const_iterator lower_bound(Key const & key) const
        { return std::lower_bound(this->begin(), this->end(), key, compare_); }

    iterator upper_bound(Key const & key)
        { return std::upper_bound(this->begin(), this->end(), key, compare_); }
    const_iterator upper_bound(Key const & key) const
        { return std::upper_bound(this->begin(), this->end(), key, compare_); }

    reverse_iterator rbegin()
        { return container_.rbegin(); }
    const_reverse_iterator rbegin() const
        { return container_.rbegin(); }

    reverse_iterator rend()
        { return container_.rend(); }
    const_reverse_iterator rend() const
        { return container_.rend(); }

    void swap(SetVector& rhs)
    {
        std::swap(container_, rhs.container_);
        std::swap(compare_, rhs.compare_);
    }

    value_compare value_comp() const
        { return value_compare(compare_); }


    friend bool operator==(SetVector const & lhs, SetVector const & rhs)
        { return lhs.container_ == rhs.container_; }

    friend bool operator<(SetVector const & lhs, SetVector const & rhs)
        { return lhs.container_ < rhs.container_; }

private:
    ContainerType   container_;
    CompareType     compare_;
};

template <class Key, class Traits, class Allocator>
inline bool
operator!=(SetVector<Key, Traits, Allocator> const & lhs,
                SetVector<Key, Traits, Allocator> const & rhs)
{
    return !(lhs == rhs);
}

template <class Key, class Traits, class Allocator>
inline bool
operator>(SetVector<Key, Traits, Allocator> const & lhs,
                SetVector<Key, Traits, Allocator> const & rhs)
{
    return rhs < lhs;
}

template <class Key, class Traits, class Allocator>
inline bool
operator>=(SetVector<Key, Traits, Allocator> const & lhs,
                SetVector<Key, Traits, Allocator> const & rhs)
{
    return !(lhs < rhs);
}

template <class Key, class Traits, class Allocator>
inline bool
operator<=(SetVector<Key, Traits, Allocator> const & lhs,
                SetVector<Key, Traits, Allocator> const & rhs)
{
    return !(rhs < lhs);
}

// ============================================================================================== //

namespace std
{
    template <class Key, class Traits, class Allocator>
    inline void
    swap(
        Containers::SetVector<Key, Traits, Allocator>& lhs, 
        Containers::SetVector<Key, Traits, Allocator>& rhs)
    {
        lhs.swap(rhs);
    }
}

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_END();

// ============================================================================================== //


// ============================================================================================== //

#endif // VD_CONTAINERS_SET_INCLUDED

