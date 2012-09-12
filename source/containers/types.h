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

#ifndef VD_CONTAINERS_TYPES_INCLUDED
#define VD_CONTAINERS_TYPES_INCLUDED

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

#if defined(VD_USE_BOOST_CONTAINERS)
  #include <boost/tuple/tuple.hpp>
  #include <boost/functional/hash.hpp>
  #include <boost/unordered_map.hpp>
  #include <boost/unordered_set.hpp>
#endif

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_USING(Core, RawAllocator);
VD_USING(Core, AlignedAllocator);

// ============================================================================================== //

template<typename T>
struct List
{
    typedef std::list<T, RawAllocator<T> > type;
};

template<typename T>
struct Vector
{
    typedef std::vector<T, RawAllocator<T> > type;
};

template<typename T>
struct Queue
{
    typedef std::queue<T, std::deque<T, RawAllocator<T> > > type;
};

template<typename T>
struct Stack
{
    typedef std::stack<T, std::vector<T, RawAllocator<T> > > type;
};

#if defined(VD_USE_BOOST_CONTAINERS)

template <class K, class T, class H = boost::hash<K>, class P = std::equal_to<K>,
          class A = RawAllocator<std::pair<const K, T> > >
struct Map 
{
    typedef boost::unordered_map<K, T, H, P, A> type;
};

template <class T, class H = boost::hash<std::string>, class P = std::less<std::string>,
          class A = RawAllocator<std::pair<const std::string, T> > >
struct StringMap 
{
    typedef boost::unordered_map<std::string, T, H, P, A> type;
};

template <class T, class H = boost::hash<T>, class P = std::equal_to<T>,    
          class A = RawAllocator<T> >
struct Set
{
    typedef boost::unordered_set<T, H, P, A> type;
};

#else

template<typename T, typename Y>
struct Map
{
    typedef std::map<T, Y, std::equal_to<T>, RawAllocator< std::pair<T, Y> > > type;
};

template<typename Y>
struct StringMap
{
    typedef std::map<std::string, Y, std::less<std::string>, RawAllocator< std::pair<std::string, Y> > > type;
};

template<typename T>
struct Set
{
    typedef std::set<T, std::equal_to<T>, RawAllocator<T> > type;
};

#endif

template<typename T, typename Y>
struct OrderedMap
{
    typedef std::map<T, Y, std::equal_to<T>, RawAllocator< std::pair<T, Y> > > type;
};

// ============================================================================================== //

template<typename T>
struct AlignedList
{
    typedef std::list<T, AlignedAllocator<T> > type;
};

template<typename T>
struct AlignedVector
{
    typedef std::vector<T, AlignedAllocator<T> > type;
};

template<typename T>
struct AlignedQueue
{
    typedef std::queue<T, std::deque<T, AlignedAllocator<T> > > type;
};

template<typename T>
struct AlignedStack
{
    typedef std::stack<T, std::vector<T, AlignedAllocator<T> > > type;
};

#if defined(VD_USE_BOOST_CONTAINERS)

template <class K, class T, class H = boost::hash<K>, class P = std::equal_to<K>,
          class A = AlignedAllocator<std::pair<const K, T> > >
struct AlignedMap 
{
    typedef boost::unordered_map<K, T, H, P, A> type;
};

template <class T, class H = boost::hash<T>, class P = std::equal_to<T>,    
          class A = AlignedAllocator<T> >
struct AlignedSet
{
    typedef boost::unordered_set<T, H, P, A> type;
};

#else

template<typename T, typename Y>
struct AlignedMap
{
    typedef std::map<T, Y, std::equal_to<T>, AlignedAllocator<std::pair<T, Y> > > type;
};

template<typename T>
struct AlignedSet
{
    typedef std::set<T, std::equal_to<T>, AlignedAllocator<T> > type;
};

#endif

template<typename T, typename Y>
struct AlignedOrderedMap
{
    typedef std::map<T, Y, std::equal_to<T>, AlignedAllocator< std::pair<T, Y> > > type;
};

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

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CONTAINERS_TYPES_INCLUDED

