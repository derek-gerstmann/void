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

#ifndef VD_CONTAINERS_CACHE_INCLUDED
#define VD_CONTAINERS_CACHE_INCLUDED

// ============================================================================================== //

#include "containers/containers.h"
#include "core/locking.h"
#include "core/functions.h"

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_IMPORT(Core, Function);
VD_IMPORT(Core, Mutex);
VD_IMPORT(Core, ScopedMutex);

// ============================================================================================== //

template < class T >
struct CountFn 
{
    unsigned long operator()( const T &x ) { return 1; }
};

template< class Key, class Value, class SizeFn = CountFn< Value > > 
class LruCache 
{

public:

    typedef Key                                             KeyType;
    typedef Value                                           ValueType;
    typedef std::list< std::pair< Key, Value > >            ListType; 
    typedef typename ListType::iterator                     ListItr;              
    typedef typename ListType::const_iterator               ListCItr;      
    typedef std::vector< Key >                              KeyListType;                  
    typedef typename KeyListType::iterator                  KeyListItr;        
    typedef typename KeyListType::const_iterator            KeyListCItr; 
    typedef std::map< Key, ListItr >                        MapType;                
    typedef std::pair< Key, ListItr >                       PairType;                
    typedef typename MapType::iterator                      MapItr;                 
    typedef typename MapType::const_iterator                MapCItr;           
    typedef Function<ValueType (const KeyType&)>            FetchFn;
    typedef Function<void (ValueType)>                      EvictFn;

public:

    LruCache() :
        m_SlotsUsed(0),
        m_Capacity(0),
        m_FetchFn(),
        m_EvictFn(),
        m_NullValue()
    {
        // EMPTY!
    }

    void Setup(
        vd::u64 capacity,
        FetchFn fetch,
        EvictFn evict = EvictFn(),
        ValueType empty = 0)     
    {
        Destroy();
        m_SlotsUsed = 0;
        m_Capacity = capacity;
        m_FetchFn = fetch;
        m_EvictFn = evict;
        m_NullValue = empty;
    }

    ~LruCache() { Destroy(); }

    inline vd::u64 
    GetSlotsUsed( void ) const 
    { 
        ScopedMutex locker(&m_Mutex);
        return m_SlotsUsed; 
    }

    inline vd::u64 
    GetCapacity( void ) const 
    { 
        ScopedMutex locker(&m_Mutex);
        return m_Capacity; 
    }

    inline void
    Destroy()
    {
        KeyListType list;
        GetKeys(list);
        while(list.size())
        {
            Remove(list.front());
            list.erase(list.begin());
        }
        m_List.clear();
        m_Index.clear();
        m_SlotsUsed = 0;
        m_Capacity = 0;
    }

    inline bool 
    IsResident( const KeyType& key ) 
    {
        ScopedMutex locker(&m_Mutex);
        return m_Index.find( key ) != m_Index.end();
    }

    inline void 
    Remove( const KeyType& key ) 
    {
        ScopedMutex locker(&m_Mutex);
        MapItr mitr = m_Index.find( key );
        if( mitr == m_Index.end() ) return;
        OnRemove( mitr );
    }

    inline void 
    Touch( const KeyType& key ) 
    {
        ScopedMutex locker(&m_Mutex);
        OnTouch( key );
    }

    inline bool
    Load(const KeyType& key, bool touch = true ) 
    {
        bool hit = false;
        m_Mutex.Lock();
        MapItr mitr = m_Index.find( key );
        if( mitr == m_Index.end() )
        {
            m_Mutex.Unlock();
            const ValueType& value = m_FetchFn(key);
            if(value != m_NullValue)
            {
                Insert(key, value);
            }
            m_Mutex.Lock();
            hit = false;
        }
        else
        {
            if( touch )
                OnTouch( key );
            hit = true;
        }
        m_Mutex.Unlock();
        return hit;
    }

    inline bool
    Fetch(const KeyType& key, ValueType& value, bool touch = true ) 
    {
        bool hit = false;
        m_Mutex.Lock();
        MapItr mitr = m_Index.find( key );
        if( mitr == m_Index.end() )
        {
            m_Mutex.Unlock();
            value = m_FetchFn(key);
            if(value != m_NullValue)
            {
                Insert(key, value);
            }
            m_Mutex.Lock();
            hit = false;
        }
        else
        {
            if( touch )
                OnTouch( key );

            value = mitr->second->second;
            hit = true;
        }
        m_Mutex.Unlock();
        return hit;
    }

    inline void 
    Insert( const KeyType &key, const ValueType& value ) 
    {
        ScopedMutex locker(&m_Mutex);

        MapItr mitr = OnTouch( key );
        OnRemove( mitr );

        m_List.push_front( std::make_pair( key, value ) );
        ListItr litr = m_List.begin();

        m_Index.insert( std::make_pair( key, litr ) );
        m_SlotsUsed += SizeFn()( value );

        while( m_SlotsUsed > m_Capacity ) 
        {
            vdLogGlobalInfo("Evicting '%d/%d'", m_SlotsUsed, m_Capacity);
            litr = m_List.end();
            --litr;
        
            OnRemove( litr->first );      
        }
    }

    inline vd::u64 
    GetKeys( KeyListType& list ) 
    {
        ScopedMutex locker(&m_Mutex);
        list.clear();
        for( ListCItr litr = m_List.begin(); litr != m_List.end(); litr++ )
            list.push_back( litr->first );
        return list.size();
    }

private:
    
    inline MapItr 
    OnTouch( const Key &key ) 
    {
        MapItr mitr = m_Index.find( key );
        if( mitr == m_Index.end() ) return mitr;
        m_List.splice( m_List.begin(), m_List, mitr->second );
        return mitr;
    }

    inline void 
    OnRemove( const MapItr &mitr ) 
    {
        if( mitr == m_Index.end() )
            return;

        m_SlotsUsed -= SizeFn()( mitr->second->second );
        m_EvictFn( mitr->second->second );              
        m_List.erase( mitr->second );
        m_Index.erase( mitr );
    }

    inline void 
    OnRemove( const KeyType &key ) 
    {
        MapItr mitr = m_Index.find( key );
        OnRemove( mitr );
    }
        
private:

    VD_DISABLE_COPY_CONSTRUCTORS(LruCache);
    
    ListType m_List;               
    MapType m_Index;               
    vd::u64 m_SlotsUsed; 
    vd::u64 m_Capacity; 
    FetchFn m_FetchFn;
    EvictFn m_EvictFn;
    ValueType m_NullValue;
    Mutex m_Mutex;
};

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CONTAINERS_CACHE_INCLUDED

