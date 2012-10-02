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

#ifndef VD_CORE_COLLECTION_INCLUDED
#define VD_CORE_COLLECTION_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/allocators.h"

// ============================================================================================== //

#include <list>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <set>

#if defined(VD_USE_BOOST_COLLECTIONS)
  #include <boost/tuple/tuple.hpp>
  #include <boost/functional/hash.hpp>
  #include <boost/unordered_map.hpp>
  #include <boost/unordered_set.hpp>
  #include <boost/circular_buffer.hpp>
#endif

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

template<
  typename T, 
  class A = RawAllocator<T> 
>
struct List
{
    typedef std::list<T, A> type;
};

// --------------------------------------------------------------------------------------------- //

template<
  typename T, 
  class A = RawAllocator<T> 
>
struct Vector
{
    typedef std::vector<T, A> type;
};

// --------------------------------------------------------------------------------------------- //

template<
  typename T, 
  class A = RawAllocator<T> 
>
struct Queue
{
    typedef std::queue<T, std::deque<T, A > > type;
};

// --------------------------------------------------------------------------------------------- //

template<
  typename T, 
  class A = RawAllocator<T> 
>
struct Stack
{
    typedef std::stack<T, std::vector<T, A > > type;
};

// --------------------------------------------------------------------------------------------- //

#if defined(VD_USE_BOOST_COLLECTIONS)

// --------------------------------------------------------------------------------------------- //

template <
  class K, 
  class T, 
  class H = boost::hash<K>, 
  class P = std::equal_to<K>,
  class A = RawAllocator<std::pair<const K, T> > 
>
struct Map 
{
    typedef boost::unordered_map<K, T, H, P, A> type;
};

// --------------------------------------------------------------------------------------------- //

template <
  class T, 
  class H = boost::hash<std::string>, 
  class P = std::less<std::string>,
  class A = RawAllocator< std::pair<const std::string, T> > 
>
struct StringMap 
{
    typedef boost::unordered_map<std::string, T, H, P, A> type;
};

// --------------------------------------------------------------------------------------------- //

template <
  class T, 
  class H = boost::hash<T>, 
  class P = std::equal_to<T>,    
  class A = RawAllocator<T> 
>
struct Set
{
    typedef boost::unordered_set<T, H, P, A> type;
};

// --------------------------------------------------------------------------------------------- //

#else

// --------------------------------------------------------------------------------------------- //

template<
  typename T, 
  typename Y,
  class A = RawAllocator< std::pair<T, Y> >
>
struct Map
{
    typedef std::map<T, Y, std::equal_to<T>, A > type;
};

// --------------------------------------------------------------------------------------------- //

template<
  typename Y,
  class A = RawAllocator< std::pair<std::string, Y> >
>
struct StringMap
{
    typedef std::map<std::string, Y, std::less<std::string>, A > type;
};

// --------------------------------------------------------------------------------------------- //

template<
  typename T,
  class A = RawAllocator<T> 
>
struct Set
{
    typedef std::set<T, std::equal_to<T>, A > type;
};

// --------------------------------------------------------------------------------------------- //

#endif

// --------------------------------------------------------------------------------------------- //

template<
  typename T, 
  typename Y,
  class A = RawAllocator< std::pair<T, Y> >
>
struct OrderedMap
{
    typedef std::map<T, Y, std::equal_to<T>, A > type;
};

// --------------------------------------------------------------------------------------------- //

template<
  typename T,
  class A = AlignedAllocator<T>
>
struct AlignedList
{
    typedef std::list<T, A> type;
};

// --------------------------------------------------------------------------------------------- //

template<
  typename T,
  class A = AlignedAllocator<T>
>
struct AlignedVector
{
    typedef std::vector<T, A> type;
};

// --------------------------------------------------------------------------------------------- //

template<
  typename T,
  class A = AlignedAllocator<T>
>
struct AlignedQueue
{
    typedef std::queue<T, std::deque<T, A> > type;
};

// --------------------------------------------------------------------------------------------- //

template<
  typename T,
  class A = AlignedAllocator<T>
>
struct AlignedStack
{
    typedef std::stack<T, std::vector<T, A> > type;
};

// --------------------------------------------------------------------------------------------- //

#if defined(VD_USE_BOOST_COLLECTIONS)

// --------------------------------------------------------------------------------------------- //

template <
  class K, 
  class T, 
  class H = boost::hash<K>, 
  class P = std::equal_to<K>,
  class A = AlignedAllocator<std::pair<const K, T> > 
>
struct AlignedMap 
{
    typedef boost::unordered_map<K, T, H, P, A> type;
};

// --------------------------------------------------------------------------------------------- //

template <
  class T, 
  class H = boost::hash<T>, 
  class P = std::equal_to<T>,    
  class A = AlignedAllocator<T> 
>
struct AlignedSet
{
    typedef boost::unordered_set<T, H, P, A> type;
};

// --------------------------------------------------------------------------------------------- //

#else

// --------------------------------------------------------------------------------------------- //

template<
  typename T, 
  typename Y,
  class A = AlignedAllocator<std::pair<T, Y> >
>
struct AlignedMap
{
    typedef std::map<T, Y, std::equal_to<T>, A > type;
};

// --------------------------------------------------------------------------------------------- //

template<
  typename T,
  class A = AlignedAllocator<T>
>
struct AlignedSet
{
    typedef std::set<T, std::equal_to<T>, A > type;
};

// --------------------------------------------------------------------------------------------- //

#endif

// --------------------------------------------------------------------------------------------- //

template<
  typename T, 
  typename Y,
  class A = AlignedAllocator< std::pair<T, Y> >
>
struct AlignedOrderedMap
{
    typedef std::map<T, Y, std::equal_to<T>, A > type;
};

// --------------------------------------------------------------------------------------------- //

struct select1st
{
   template< typename P >
   typename P::first_type const& operator()( P const& p ) const
   {
       return p.first;
   }
};

struct select2nd
{
   template< typename P >
   typename P::second_type const& operator()( P const& p ) const
   {
       return p.second;
   }
};

// --------------------------------------------------------------------------------------------- //

template <typename ArrayType>
void SwapClear(
  ArrayType &array)
{
  ArrayType tmp;
  array.swap(tmp);
}

// --------------------------------------------------------------------------------------------- //

template <typename MapType>
bool HasKey(
    const MapType& map, 
    const typename MapType::key_type& key)
{
    return map.find(key) != map.end();
}

// --------------------------------------------------------------------------------------------- //

template <typename MapType>
typename MapType::mapped_type 
Retrieve(
    const MapType& map, 
    const typename MapType::key_type& key,
    const typename MapType::mapped_type& def)
{
    typename MapType::const_iterator i(map.find(key));
    return i != map.end() ? i->second : def;
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_COLLECTIONS_INCLUDED

