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

#ifndef VD_CORE_LOCKING_INCLUDED
#define VD_CORE_LOCKING_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/shared.h"
#include "containers/containers.h"

// ============================================================================================== //

VD_IMPORT(Containers, Vector);

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class Semaphore : public Shared<Semaphore>
{
public:

    explicit Semaphore(unsigned int count = 0);
    ~Semaphore();

    void Wait();
    void Notify();

private:
	void* m_Handle;

	VD_DISABLE_COPY_CONSTRUCTORS(Semaphore);

};

// ============================================================================================== //

class Mutex : public Shared<Mutex>
{
    friend class ConditionVariable;

public:

	VD_DECLARE_ENUM(Type,
		Fast,
		Robust,
		Recursive);

    explicit Mutex();
    explicit Mutex(Type::Value type);

    ~Mutex();
    void Lock();
    bool TryToLock();
    bool TryToLock(vd::f64 sec);
    void Unlock();

private:

    void* m_Handle;

	VD_DISABLE_COPY_CONSTRUCTORS(Mutex);
};

// ============================================================================================== //

class ScopedMutex
{
public:

    explicit ScopedMutex(Mutex* mutex);
    ~ScopedMutex();

private:

    Mutex* m_Mutex;
    
	VD_DISABLE_COPY_CONSTRUCTORS(ScopedMutex);    
};

// ============================================================================================== //

class SlottedMutex
{
public:
    explicit SlottedMutex(size_t slotnum);
    ~SlottedMutex();

    void Lock(size_t idx);
    void Unlock(size_t idx);
    void LockAll();
    void UnlockAll();

private:
    void* m_Handle;

	VD_DISABLE_COPY_CONSTRUCTORS(SlottedMutex);    
};

// ============================================================================================== //

class SpinLock : public Shared<SpinLock>
{
public:

    explicit SpinLock();
    ~SpinLock();
    void Lock();
    bool TryToLock();
    void Unlock();

private:

    void* m_Handle;

	VD_DISABLE_COPY_CONSTRUCTORS(SpinLock);
};

// ============================================================================================== //

class ScopedSpinLock
{
public:

    explicit ScopedSpinLock(SpinLock* spinlock); 
    ~ScopedSpinLock();

private:

    SpinLock* m_SpinLock;

	VD_DISABLE_COPY_CONSTRUCTORS(ScopedSpinLock);
};

// ============================================================================================== //

class SlottedSpinLock : public Shared<SlottedSpinLock>
{
public:
    explicit SlottedSpinLock(size_t slotnum);
    ~SlottedSpinLock();

    void Lock(size_t idx);
    void Unlock(size_t idx);

    void LockAll();
    void UnlockAll();

private:
    void* m_Handle;

	VD_DISABLE_COPY_CONSTRUCTORS(SlottedSpinLock);
};

// ============================================================================================== //

class RWLock : public Shared<RWLock>
{
public:

    explicit RWLock();
    ~RWLock();

    void LockWriter();
    bool TryToLockWriter();

    void LockReader();

    bool TryToLockReader();
    void Unlock();

private:

    void* m_Handle;
    
	VD_DISABLE_COPY_CONSTRUCTORS(RWLock);
    
};

// ============================================================================================== //

class ScopedRWLock
{
public:

    explicit ScopedRWLock(RWLock* rwlock, bool writer);
    ~ScopedRWLock();

private:
    VD_DISABLE_COPY_CONSTRUCTORS(ScopedRWLock);

    RWLock* m_RWLock;
};

// ============================================================================================== //

class SlottedRWLock : public Shared<SlottedRWLock>
{
public:

    explicit SlottedRWLock(size_t slotnum);
    ~SlottedRWLock();

    void LockWriter(size_t idx);
    void LockReader(size_t idx);
    void Unlock(size_t idx);
    void LockAllWriters();
    void LockAllReaders();
    void UnlockAll();

private:
    VD_DISABLE_COPY_CONSTRUCTORS(SlottedRWLock);

    void* m_Handle;    
};

// ============================================================================================== //

class SpinRWLock : public Shared<SpinRWLock>
{
public:

    explicit SpinRWLock();
    ~SpinRWLock();

    void LockWriter();
    bool TryToLockWriter();

    void LockReader();
    bool TryToLockReader();

    void Unlock();
    bool Promote();
    void Demote();

private:
    VD_DISABLE_COPY_CONSTRUCTORS(SpinRWLock);

    void* m_Handle;    
};

// ============================================================================================== //

class ScopedSpinRWLock
{
public:
    explicit ScopedSpinRWLock(SpinRWLock* srwlock, bool writer);
    ~ScopedSpinRWLock();

private:
    VD_DISABLE_COPY_CONSTRUCTORS(ScopedSpinRWLock);

    SpinRWLock* m_SpinRWLock;    
};

// ============================================================================================== //

class SlottedSpinRWLock : public Shared<SlottedSpinRWLock>
{
public:
    explicit SlottedSpinRWLock(size_t slotnum);
    ~SlottedSpinRWLock();

    void LockWriter(size_t idx);
    void LockReader(size_t idx);

    void Unlock(size_t idx);

    void LockAllWriters();
    void LockAllReaders();

    void UnlockAll();

private:
    VD_DISABLE_COPY_CONSTRUCTORS(SlottedSpinRWLock);

    void* m_Handle;    
};

// ============================================================================================== //

class ConditionVariable : public Shared<ConditionVariable>
{
public:
    explicit ConditionVariable();
    ~ConditionVariable();

    void Wait(Mutex* mutex);
    bool Wait(Mutex* mutex, vd::f64 sec);

    void Signal();
    void NotifyOne() { Signal(); }
    void Broadcast();
    void NotifyAll() { Broadcast(); }

private:

    void* m_Handle;

	VD_DISABLE_COPY_CONSTRUCTORS(ConditionVariable);
};

// ============================================================================================== //

class ConditionMap : public Shared<ConditionMap>
{
public:

    static const size_t SlotCount = 64;
    struct Count
    {
        size_t Queued; 
        size_t Running;
    };

    typedef Containers::Map<size_t, Count>::type CountMap;

    struct Slot
    {
        ConditionVariable Condition;       
        Mutex Lock;                      
        CountMap Counters;            
    };

public:

    explicit ConditionMap();
    ~ConditionMap();
    bool Wait(size_t key, vd::f64 sec = -1);
    size_t Signal(size_t key);
    size_t Broadcast(size_t key);
    size_t BroadcastToAll();
    size_t GetWaitCount();

private:
    Slot* FindSlot(size_t key);

    Slot m_Slots[SlotCount];
};


// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif  // VD_CORE_LOCKING_INCLUDED                     
