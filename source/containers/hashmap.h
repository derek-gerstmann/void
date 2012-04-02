// ============================================================================================== //
//
// License:       The Lesser GNU Public License (LGPL) v3.0.
//
// Author(s):     Derek Gerstmann. The University of Western Australia (UWA).
//                      As well as the shoulders of many giants...
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

#ifndef VD_CONTAINERS_HASHMAP_INCLUDED
#define VD_CONTAINERS_HASHMAP_INCLUDED

// ============================================================================================== //

#include "containers/containers.h"
#include "core/asserts.h"

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_BEGIN();

// ============================================================================================== //

template<class Allocator>
class HashMapBase
{
public:
    typedef bool (*MatchFun)(void* key1, void* key2);

    HashMapBase(MatchFun match, vd::u32 initial_capacity = 8);
    ~HashMapBase();

    struct Entry
    {
        void* key;
        void* value;
        vd::u32 hash;
    };

    Entry* Lookup(void* key, vd::u32 hash, bool insert);
    void Remove(void* key, vd::u32 hash);
    void Clear();
    vd::u32 GetOccupancy() const { return m_Occupancy; }
    vd::u32 GetCapacity() const { return m_Capacity; }
    Entry* Begin() const;
    Entry* Next(Entry* p) const;

private:
    MatchFun m_MatchFn;
    Entry* m_EntryMap;
    vd::u32 m_Capacity;
    vd::u32 m_Occupancy;

    Entry* End() const { return m_EntryMap + m_Capacity; }
    Entry* Probe(void* key, vd::u32 hash);
    void Setup(vd::u32 capacity);
    void Resize();
};

typedef HashMapBase<FreeStoreAllocator> HashMap;

template<class P>
HashMapBase<P>::HashMapBase(
    MatchFun match,
    vd::u32 initial_capacity)
{
    m_MatchFn = match;
    Setup(initial_capacity);
}


template<class P>
HashMapBase<P>::~HashMapBase()
{
    P::Delete(m_EntryMap);
}


template<class P>
typename HashMapBase<P>::Entry* 
HashMapBase<P>::Lookup(
    void* key, vd::u32 hash, bool insert)
{
    Entry* p = Probe(key, hash);
    if(p->key != NULL)
    {
        return p;
    }

    if(insert)
    {
        p->key = key;
        p->value = NULL;
        p->hash = hash;
        m_Occupancy++;

        if(m_Occupancy + m_Occupancy / 4 >= m_Capacity)
        {
            Resize();
            p = Probe(key, hash);
        }

        return p;
    }

    return NULL;
}


template<class P>
void HashMapBase<P>::Remove(void* key, vd::u32 hash)
{
    Entry* p = Probe(key, hash);
    if(p->key == NULL)
    {
        return;
    }

    vdGlobalAssert(m_Occupancy < m_Capacity);

    Entry* q = p;
    while(true)
    {
        q = q + 1;

        if(q == End())
        {
            q = m_EntryMap;
        }

        if(q->key == NULL)
        {
            break;
        }

        Entry* r = m_EntryMap + (q->hash & (m_Capacity - 1));

        if((q > p && (r <= p || r > q)) ||
                (q < p && (r <= p && r > q)))
        {
            *p = *q;
            p = q;
        }
    }

    p->key = NULL;
    m_Occupancy--;
}


template<class P>
void HashMapBase<P>::Clear()
{
    // Mark all entries as empty.
    const Entry* end = End();

    for(Entry* p = m_EntryMap; p < end; p++)
    {
        p->key = NULL;
    }

    m_Occupancy = 0;
}


template<class P>
typename HashMapBase<P>::Entry*
HashMapBase<P>::Begin() const
{
    return Next(m_EntryMap - 1);
}


template<class P>
typename HashMapBase<P>::Entry*
HashMapBase<P>::Next(Entry* p) const
{
    const Entry* end = End();
    vdGlobalAssert(m_EntryMap - 1 <= p && p < end);

    for(p++; p < end; p++)
    {
        if(p->key != NULL)
        {
            return p;
        }
    }

    return NULL;
}


template<class P>
typename HashMapBase<P>::Entry*
HashMapBase<P>::Probe(
    void* key, vd::u32 hash)
{
    vdGlobalAssert(key != NULL);
    vdGlobalAssert(IsPowerOf2(m_Capacity));

    Entry* p = m_EntryMap + (hash & (m_Capacity - 1));
    const Entry* end = End();

    vdGlobalAssert(m_EntryMap <= p && p < end);
    vdGlobalAssert(m_Occupancy < m_Capacity); 

    while(p->key != NULL && (hash != p->hash || !m_MatchFn(key, p->key)))
    {
        p++;

        if(p >= end)
        {
            p = m_EntryMap;
        }
    }

    return p;
}


template<class P>
void HashMapBase<P>::Setup(
    vd::u32 capacity)
{
    vdGlobalAssert(IsPowerOf2(capacity));
    m_EntryMap = reinterpret_cast<Entry*>(P::New(capacity * sizeof(Entry)));

    if(m_EntryMap == NULL)
    {
        vdGlobalException("FatalProcessOutOfMemory: HashMap::Setup");
        return;
    }

    m_Capacity = capacity;
    Clear();
}


template<class P>
void HashMapBase<P>::Resize()
{
    Entry* map = m_EntryMap;
    vd::u32 n = m_Occupancy;

    // Allocate larger map.
    Setup(m_Capacity * 2);

    // Rehash all current entries.
    for(Entry* p = map; n > 0; p++)
    {
        if(p->key != NULL)
        {
            Lookup(p->key, p->hash, true)->value = p->value;
            n--;
        }
    }

    // Delete old map.
    P::Delete(map);
}

template<class Key, class Value, class Allocator>
class TemplateHashMap : private HashMapBase<Allocator>
{
public:

    VD_STATIC_ASSERT(sizeof(Key*) == sizeof(void*), HashMapKeySizeMismatch);  // NOLINT
    VD_STATIC_ASSERT(sizeof(Value*) == sizeof(void*), HashMapValueSizeMismatch);  // NOLINT

    struct value_type
    {
        Key* first;
        Value* second;
    };

    class Iterator
    {
    public:
            Iterator& operator++()
            {
                  entry_ = m_EntryMap->Next(entry_);
                  return *this;
            }

            value_type* operator->() { return reinterpret_cast<value_type*>(entry_); }
            bool operator!=(const Iterator& other) { return  entry_ != other.entry_; }

      private:

            Iterator(
                  const HashMapBase<Allocator>* map,
                  typename HashMapBase<Allocator>::Entry* entry
            ) :
                  m_EntryMap(map), entry_(entry) 
            { 
                  // EMPTY
            }

            const HashMapBase<Allocator>* m_EntryMap;
            typename HashMapBase<Allocator>::Entry* entry_;

            friend class TemplateHashMap;
    };

    TemplateHashMap(
        typename HashMapBase<Allocator>::MatchFun match)
        : HashMapBase<Allocator>(match) { }

    Iterator begin() const { return Iterator(this, this->Begin()); }
    Iterator end() const { return Iterator(this, NULL); }
    Iterator find(Key* key, bool insert = false)
    {
        return Iterator(this, this->Lookup(key, key->Hash(), insert));
    }
};

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CONTAINERS_HASHMAP_INCLUDED
