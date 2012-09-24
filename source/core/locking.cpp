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

#include "core/locking.h"
#include "core/system.h"
#include "core/process.h"
#include "core/memory.h"
#include "core/exceptions.h"
#include "core/asserts.h"
#include "core/collections.h"

// ============================================================================================== //

#if   defined(VD_TARGET_WINDOWS)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#elif defined(VD_TARGET_OSX)
	#include <mach/mach_init.h>
	#include <mach/thread_act.h>
	#include <mach/thread_policy.h>
	#include <mach/semaphore.h>
	#include <mach/task.h>
#elif defined(VD_TARGET_LINUX)
	#include <semaphore.h>
    #include <sys/time.h>
#endif

#if defined(VD_USE_POSIX)
	#include <pthread.h>
	#include <sched.h>
	#include <time.h>
	#include <errno.h>
#endif

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

namespace
{
	const vd::u32 	VD_THREAD_SPINLOCK_THRESHOLD = 8192;
	const vd::u32 	VD_THREAD_LOCK_SEMA_COUNT = 256;     
	const vd::u32 	VD_THREAD_NAP_TIME_IN_MILLISEC = 21;
}

// ============================================================================================== //

struct SemaphoreCore
{
#if   defined(VD_TARGET_WINDOWS)
    ::HANDLE            semaphore;
#elif defined(VD_TARGET_OSX)
    ::task_t            task;
    ::semaphore_t       semaphore;
#elif defined(VD_TARGET_LINUX)
    ::sem_t             semaphore;
#endif
};

// ============================================================================================== //

struct ConditionVariableCore
{
#if defined(VD_TARGET_WINDOWS)
    ::CRITICAL_SECTION  mutex;              
    vd::u32             wait;                        
    vd::u32             wake;                         
    ::HANDLE            emitter;                          
    ::HANDLE            finisher;                          
#elif defined(VD_USE_POSIX)
    ::pthread_cond_t    condition;                 
#endif
};

// ============================================================================================== //

Semaphore::Semaphore(unsigned int count)
{
	SemaphoreCore* core = VD_NEW(SemaphoreCore);
	
#if defined(VD_TARGET_WINDOWS)
    core->semaphore = CreateSemaphore(NULL, count, 2147483647, NULL);
    if (core->semaphore == NULL) 
    {
        vdGlobalException("CreateSemaphore", "Failed to create semaphore!");
    }
#elif defined(VD_TARGET_OSX)
    core->task = mach_task_self();
    if (semaphore_create(core->task, &core->semaphore, SYNC_POLICY_FIFO, count)) 
    {
        vdGlobalException("semaphore_create", "Failed to create semaphore!");
    }
#elif defined(VD_TARGET_LINUX)
    if (sem_init(&core->semaphore, 0, count)) 
    {
        vdGlobalException("sem_init", "Failed to initialize semaphore!");
    }
#endif

	m_Handle = reinterpret_cast<void*>(core);
}

// ============================================================================================== //

Semaphore::~Semaphore()
{
	if(m_Handle == NULL)
		return;

	SemaphoreCore* core = reinterpret_cast<SemaphoreCore*>(m_Handle);

#if defined(VD_TARGET_WINDOWS)
    
    CloseHandle(core->semaphore);
    
#elif defined(VD_TARGET_OSX)

    if(semaphore_destroy(core->task, core->semaphore) != 0)
    {
        vdGlobalException("semaphore_destroy", "Failed to destroy semaphore!");
    }

#elif defined(VD_TARGET_LINUX)

    if(sem_destroy(&core->semaphore) != 0)
    {
        vdGlobalException("sem_destroy", "Failed to destroy semaphore!");
    }

#endif
	VD_DELETE(core);
	m_Handle = NULL;
}

// ============================================================================================== //

void
Semaphore::Wait()
{
	SemaphoreCore* core = reinterpret_cast<SemaphoreCore*>(m_Handle);

#if defined(VD_TARGET_WINDOWS)

    DWORD dwRet = WaitForSingleObject(core->semaphore, INFINITE);
    if (dwRet != WAIT_OBJECT_0) 
    {
		vdGlobalException("WaitForSingleObject", "Semaphore failed to wait for single object!");
    }

#elif defined(VD_TARGET_OSX)

    while (true) 
    {
        if (!semaphore_wait(core->semaphore))
            return;
            
        if (errno != EINTR) 
        {
			vdGlobalException("semaphore_wait", "Semaphore failed to wait for single object!");
        }
    }

#elif defined(VD_TARGET_LINUX)

    while (true) 
    {
        if (!sem_wait(&core->semaphore))
            return;
        
        if (errno != EINTR) 
        {
            vdGlobalException("sem_wait", "Semaphore failed to wait for single object!");
        }
    }

#endif
}

// ============================================================================================== //

void
Semaphore::Notify()
{
	SemaphoreCore* core = reinterpret_cast<SemaphoreCore*>(m_Handle);

#if defined(VD_TARGET_WINDOWS)

    if (!ReleaseSemaphore(core->semaphore, 1, NULL)) 
    {
		vdGlobalException("ReleaseSemaphore", "Failed to release semaphore!");
    }

#elif defined(VD_TARGET_OSX)

    semaphore_signal(core->semaphore);
    
#elif (VD_TARGET_LINUX)

    if (sem_post(&core->semaphore)) 
    {
		vdGlobalException("sem_post", "Failed to release semaphore!");
    }

#endif
}

// ============================================================================================== //

Mutex::Mutex() : m_Handle(NULL)
{
    
#if defined(VD_TARGET_WINDOWS)

    ::CRITICAL_SECTION* mutex = VD_NEW( CRITICAL_SECTION );
    ::InitializeCriticalSection(mutex);
	m_Handle = reinterpret_cast<void*>(mutex);

#elif defined(VD_USE_POSIX)

    ::pthread_mutex_t* mutex = VD_NEW(pthread_mutex_t);
    if(::pthread_mutex_init(mutex, NULL) != 0)
    {
		vdGlobalException("pthread_mutex_init", "Failed to initialize mutex!");
    }
   	m_Handle = reinterpret_cast<void*>(mutex);

#else

	#error "Mutex::Mutex() needs to be implemented on this platform!"

#endif

}

// ============================================================================================== //

Mutex::Mutex(Mutex::Type::Value type)
{
    
#if defined(VD_TARGET_WINDOWS)

    ::CRITICAL_SECTION* mutex = VD_NEW( CRITICAL_SECTION );
    ::InitializeCriticalSection(mutex);
	m_Handle = reinterpret_cast<void*>(mutex);

#elif defined(VD_USE_POSIX)

    ::pthread_mutexattr_t attr;
    if(::pthread_mutexattr_init(&attr) != 0) 
    {
		vdGlobalException("pthread_mutexattr_init", "Failed to initialize mutex attribute!");
    }
    
    switch(type)
    {
        case Mutex::Type::Fast:
        {
            break;
        }

        case Mutex::Type::Robust:
        {
            if(::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK) != 0)
			{
				vdGlobalException("pthread_mutexattr_settype", "Failed to set mutex attribute type to enable robust mutex!");
			}
            break;
        }

        case Mutex::Type::Recursive:
        {
            if(::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) != 0)
			{
				vdGlobalException("pthread_mutexattr_settype", "Failed to set mutex attribute type to enable recursive mutex!");
			}
			break;
        }

		case Mutex::Type::Invalid:
    	default:
    	{
			vdGlobalException("pthread_mutexattr_settype", "Invalid type specified for setting mutex attribute!");
			break;
		}
    }

    ::pthread_mutex_t* mutex = VD_NEW( pthread_mutex_t );
    if(::pthread_mutex_init(mutex, &attr) != 0) 
	{
		vdGlobalException("pthread_mutex_init", "Failed to initialize mutex!");
	}

    ::pthread_mutexattr_destroy(&attr);
	m_Handle = reinterpret_cast<void*>(mutex);
    
#endif

}

// ============================================================================================== //

Mutex::~Mutex()
{
#if defined(VD_TARGET_WINDOWS)

	::CRITICAL_SECTION* mutex = reinterpret_cast<CRITICAL_SECTION*>(m_Handle);
    ::DeleteCriticalSection(mutex);
    VD_DELETE(mutex);

#elif defined(VD_USE_POSIX)

    ::pthread_mutex_t* mutex = reinterpret_cast<pthread_mutex_t*>(m_Handle);
    ::pthread_mutex_destroy(mutex);
    VD_DELETE(mutex);

#endif
}

// ============================================================================================== //

void Mutex::Lock()
{
#if defined(VD_TARGET_WINDOWS)
    
    ::CRITICAL_SECTION* mutex = (::CRITICAL_SECTION*)m_Handle;
    ::EnterCriticalSection(mutex);

#elif defined(VD_USE_POSIX)
    
    ::pthread_mutex_t* mutex = (::pthread_mutex_t*)m_Handle;
    if(::pthread_mutex_lock(mutex) != 0) 
	{
		vdGlobalException("pthread_mutex_lock", "Failed to lock mutex!");
	}

#endif
}

// ============================================================================================== //

bool Mutex::TryToLock()
{
#if defined(VD_TARGET_WINDOWS)
    
    ::CRITICAL_SECTION* mutex = (::CRITICAL_SECTION*)m_Handle;
    if(!::TryEnterCriticalSection(mutex)) 
    	return false;

    return true;

#elif defined(VD_USE_POSIX)
    
    ::pthread_mutex_t* mutex = (::pthread_mutex_t*)m_Handle;
    int32_t ecode = ::pthread_mutex_trylock(mutex);

    if(ecode == 0) 
    	return true;
    
    if(ecode != EBUSY) 
	{
		vdGlobalException("pthread_mutex_trylock", "Failed to check mutex lock status!");
	}

    return false;
#endif
}

// ============================================================================================== //

bool Mutex::TryToLock(
    vd::f64 sec)
{
#if (defined(VD_TARGET_WINDOWS) || defined(VD_TARGET_OSX))
    vdGlobalAssert(sec >= 0.0);

    if(TryToLock()) return true;

    vd::f64 end = Process::GetTimeInSeconds() + sec;
    Thread::Yield();
    vd::u32 wait_count = 0;

    while(!TryToLock())
    {
        if(Process::GetTimeInSeconds() > end) return false;

        if(wait_count >= VD_THREAD_SPINLOCK_THRESHOLD)
        {
            Thread::Nap();
        }
        else
        {
            Thread::Yield();
            wait_count++;
        }
    }

    return true;

#elif defined(VD_USE_POSIX)

    vdGlobalAssert(sec >= 0.0);
    ::pthread_mutex_t* mutex = (::pthread_mutex_t*)m_Handle;
    struct ::timeval tv;
    struct ::timespec ts;

    if(::gettimeofday(&tv, NULL) == 0)
    {
        vd::f64 integ;
        vd::f64 fract = std::modf(sec, &integ);
        ts.tv_sec = tv.tv_sec + (time_t)integ;
        ts.tv_nsec = (long)(tv.tv_usec * 1000.0 + fract * 999999000);

        if(ts.tv_nsec >= 1000000000)
        {
            ts.tv_nsec -= 1000000000;
            ts.tv_sec++;
        }
    }
    else
    {
        ts.tv_sec = ::time(NULL) + 1;
        ts.tv_nsec = 0;
    }

    int32_t ecode = ::pthread_mutex_timedlock(mutex, &ts);

    if(ecode == 0) 
    	return true;

    if(ecode != ETIMEDOUT) 
	{
		vdGlobalException("pthread_mutex_timedlock", "Failed to check timed mutex lock status!");
	}
    
    return false;
#endif
}

// ============================================================================================== //

void Mutex::Unlock()
{
#if defined(VD_TARGET_WINDOWS)
    
    ::CRITICAL_SECTION* mutex = (::CRITICAL_SECTION*)m_Handle;
    ::LeaveCriticalSection(mutex);

#elif defined(VD_USE_POSIX)
    
    ::pthread_mutex_t* mutex = (::pthread_mutex_t*)m_Handle;
    if(::pthread_mutex_unlock(mutex) != 0) 
	{
		vdGlobalException("pthread_mutex_unlock", "Failed to unlock mutex!");
	}

#endif
}

// ============================================================================================== //

ScopedMutex::ScopedMutex(
    Mutex* mutex
) : m_Mutex(mutex)
{
    vdGlobalAssert(mutex);
    m_Mutex->Lock();
}

// ============================================================================================== //

ScopedMutex::~ScopedMutex()
{
    vdGlobalAssert(true);
    m_Mutex->Unlock();
}

// ============================================================================================== //

struct SlottedMutexCore
{
#if defined(VD_TARGET_WINDOWS)

    ::CRITICAL_SECTION* mutexes;           
    size_t              capacity;                        

#elif defined(VD_USE_POSIX)

    ::pthread_mutex_t*  mutexes;            
    size_t              capacity;                        

#endif
};

// ============================================================================================== //

SlottedMutex::SlottedMutex(
	size_t capacity) 
: 
	m_Handle(NULL)
{
#if defined(VD_TARGET_WINDOWS)
    
    SlottedMutexCore* core = VD_NEW( SlottedMutexCore );
    ::CRITICAL_SECTION* mutexes = VD_NEW_ARRAY( CRITICAL_SECTION, capacity );

    for(size_t i = 0; i < capacity; i++)
    {
        ::InitializeCriticalSection(mutexes + i);
    }

    core->mutexes = mutexes;
    core->capacity = capacity;
    m_Handle = reinterpret_cast<void*>(core);

#elif defined(VD_USE_POSIX)
    
    SlottedMutexCore* core = VD_NEW( SlottedMutexCore );
    ::pthread_mutex_t* mutexes = VD_NEW_ARRAY( pthread_mutex_t, capacity );

    for(size_t i = 0; i < capacity; i++)
    {
        if(::pthread_mutex_init(mutexes + i, NULL) != 0)
		{
			vdGlobalException("pthread_mutex_init", "Failed to initialize slotted mutex!");
		}
    }

    core->mutexes = mutexes;
    core->capacity = capacity;
    m_Handle = reinterpret_cast<void*>(core);

#endif
}

// ============================================================================================== //

SlottedMutex::~SlottedMutex()
{
#if defined(VD_TARGET_WINDOWS)
    
    SlottedMutexCore* core = reinterpret_cast<SlottedMutexCore*>(m_Handle);
    ::CRITICAL_SECTION* mutexes = core->mutexes;
    size_t capacity = core->capacity;

    for(size_t i = 0; i < capacity; i++)
    {
        ::DeleteCriticalSection(mutexes + i);
    }

    VD_DELETE_ARRAY(mutexes);
    VD_DELETE(core);

#elif defined(VD_USE_POSIX)
    
    SlottedMutexCore* core = reinterpret_cast<SlottedMutexCore*>(m_Handle);
    ::pthread_mutex_t* mutexes = core->mutexes;
    size_t capacity = core->capacity;

    for(size_t i = 0; i < capacity; i++)
    {
        ::pthread_mutex_destroy(mutexes + i);
    }

    VD_DELETE_ARRAY(mutexes);
    VD_DELETE(core);
    
#endif
}

// ============================================================================================== //

void SlottedMutex::Lock(size_t idx)
{
#if defined(VD_TARGET_WINDOWS)
    
    SlottedMutexCore* core = reinterpret_cast<SlottedMutexCore*>(m_Handle);
    ::EnterCriticalSection(core->mutexes + idx);

#elif defined(VD_USE_POSIX)
    
    SlottedMutexCore* core = reinterpret_cast<SlottedMutexCore*>(m_Handle);
    if(::pthread_mutex_lock(core->mutexes + idx) != 0)
	{
		vdGlobalException("pthread_mutex_lock", "Failed to lock slotted mutex!");
	}

#endif
}

// ============================================================================================== //

void SlottedMutex::Unlock(size_t idx)
{
#if defined(VD_TARGET_WINDOWS)
    
    SlottedMutexCore* core = reinterpret_cast<SlottedMutexCore*>(m_Handle);
    ::LeaveCriticalSection(core->mutexes + idx);

#elif defined(VD_USE_POSIX)
    
    SlottedMutexCore* core = reinterpret_cast<SlottedMutexCore*>(m_Handle);
    if(::pthread_mutex_unlock(core->mutexes + idx) != 0)
	{
		vdGlobalException("pthread_mutex_lock", "Failed to unlock slotted mutex!");
	}

#endif
}

// ============================================================================================== //

void SlottedMutex::LockAll()
{
#if defined(VD_TARGET_WINDOWS)
    
    SlottedMutexCore* core = reinterpret_cast<SlottedMutexCore*>(m_Handle);
    ::CRITICAL_SECTION* mutexes = core->mutexes;
    size_t capacity = core->capacity;

    for(size_t i = 0; i < capacity; i++)
    {
        ::EnterCriticalSection(core->mutexes + i);
    }

#elif defined(VD_USE_POSIX)
    
    SlottedMutexCore* core = reinterpret_cast<SlottedMutexCore*>(m_Handle);
    ::pthread_mutex_t* mutexes = core->mutexes;
    size_t capacity = core->capacity;

    for(size_t i = 0; i < capacity; i++)
    {
        if(::pthread_mutex_lock(mutexes + i) != 0)
		{
			vdGlobalException("pthread_mutex_lock", "Failed to lock slotted mutex!");
		}
    }

#endif
}

// ============================================================================================== //

void SlottedMutex::UnlockAll()
{
#if defined(VD_TARGET_WINDOWS)
    
    SlottedMutexCore* core = reinterpret_cast<SlottedMutexCore*>(m_Handle);
    ::CRITICAL_SECTION* mutexes = core->mutexes;
    size_t capacity = core->capacity;

    for(size_t i = 0; i < capacity; i++)
    {
        ::LeaveCriticalSection(mutexes + i);
    }

#elif defined(VD_USE_POSIX)
    
    SlottedMutexCore* core = reinterpret_cast<SlottedMutexCore*>(m_Handle);
    ::pthread_mutex_t* mutexes = core->mutexes;
    size_t capacity = core->capacity;

    for(size_t i = 0; i < capacity; i++)
    {
        if(::pthread_mutex_unlock(mutexes + i) != 0)
		{
			vdGlobalException("pthread_mutex_unlock", "Failed to unlock slotted mutex!");
		}
    }

#endif
}

// ============================================================================================== //

SpinLock::SpinLock() : m_Handle(NULL)
{
#if defined(VD_TARGET_WINDOWS)
    
	// EMPTY
	
#elif defined(VD_USE_GCC_ATOMICS)

    // EMPTY

#elif defined(VD_USE_POSIX)

    ::pthread_spinlock_t* spin = VD_NEW( pthread_spinlock_t );
    if(::pthread_spin_init(spin, PTHREAD_PROCESS_PRIVATE) != 0)
	{
		vdGlobalException("pthread_spin_init", "Failed to initialize spinlock!");
	}

    m_Handle = reinterpret_cast<void*>(spin);

#endif
}

// ============================================================================================== //

SpinLock::~SpinLock()
{
#if defined(VD_TARGET_WINDOWS)
    
    // EMPTY
    
#elif defined(VD_USE_GCC_ATOMICS)
    
    // EMPTY
    
#elif defined(VD_USE_POSIX)

    ::pthread_spinlock_t* spin = reinterpret_cast<pthread_spinlock_t*>(m_Handle);
    ::pthread_spin_destroy(spin);
    VD_DELETE(spin);
	
#endif

	m_Handle = NULL;
}

// ============================================================================================== //

void SpinLock::Lock()
{
#if defined(VD_TARGET_WINDOWS)
    
    vd::u32 wait_count = 0;
    while(::InterlockedCompareExchange((LONG*)&m_Handle, 1, 0) != 0)
    {
        if(wait_count >= VD_THREAD_SPINLOCK_THRESHOLD)
        {
            Thread::Nap();
        }
        else
        {
            Thread::Yield();
            wait_count++;
        }
    }

#elif defined(VD_USE_GCC_ATOMICS)
    
    vd::u32 wait_count = 0;

    while(!__sync_bool_compare_and_swap(&m_Handle, (void*)0, (void*)1))
    {
        if(wait_count >= VD_THREAD_SPINLOCK_THRESHOLD)
        {
            Thread::Nap();
        }
        else
        {
            Thread::Yield();
            wait_count++;
        }
    }

#elif defined(VD_USE_POSIX)
    
    ::pthread_spinlock_t* spin = reinterpret_cast<pthread_spinlock_t*>(m_Handle);
    if(::pthread_spin_lock(spin) != 0) 
	{
		vdGlobalException("pthread_spin_lock", "Failed to lock spinlock!");
	}

#endif
}

// ============================================================================================== //

bool SpinLock::TryToLock()
{
#if defined(VD_TARGET_WINDOWS)
    
    return ::InterlockedCompareExchange((LONG*)&m_Handle, 1, 0) == 0;

#elif defined(VD_USE_GCC_ATOMICS)
    
    return __sync_bool_compare_and_swap(&m_Handle, (void*)0, (void*)1);
    
#elif defined(VD_USE_POSIX)
    
    ::pthread_spinlock_t* spin = reinterpret_cast<pthread_spinlock_t*>(m_Handle);

    vd::i32 ecode = ::pthread_spin_trylock(spin);
    if(ecode == 0) 
    	return true;

    if(ecode != EBUSY) 
	{
		vdGlobalException("pthread_spin_lock", "Failed to check spinlock status!");
	}

    return false;
    
#endif
}

// ============================================================================================== //

void SpinLock::Unlock()
{
#if defined(VD_TARGET_WINDOWS)
    
    ::InterlockedExchange((LONG*)&m_Handle, 0);
    
#elif defined(VD_USE_GCC_ATOMICS)
    
    __sync_lock_release(&m_Handle);

#elif defined(VD_USE_POSIX)
    
    ::pthread_spinlock_t* spin = reinterpret_cast<pthread_spinlock_t*>(m_Handle);
    if(::pthread_spin_unlock(spin) != 0) 
	{
		vdGlobalException("pthread_spin_unlock", "Failed to unlock spinlock!");
	}

#endif
}

// ============================================================================================== //

ScopedSpinLock::ScopedSpinLock(
    SpinLock* spinlock
) : m_SpinLock(spinlock)
{
    vdGlobalAssert(spinlock);
    m_SpinLock->Lock();
}

// ============================================================================================== //

ScopedSpinLock::~ScopedSpinLock()
{
    vdGlobalAssert(true);
    m_SpinLock->Unlock();
}

// ============================================================================================== //

struct SlottedSpinLockCore
{
#if (defined(VD_TARGET_WINDOWS) || defined(VD_USE_GCC_ATOMICS))

    vd::u32*                locks;                     
    size_t                  capacity;                        	

#elif defined(VD_USE_POSIX)

    ::pthread_spinlock_t*   spins;           	
    size_t                  capacity;                        	

#endif
};

// ============================================================================================== //

SlottedSpinLock::SlottedSpinLock(size_t capacity) : m_Handle(NULL)
{
#if (defined(VD_TARGET_WINDOWS) || defined(VD_USE_GCC_ATOMICS))
    
    SlottedSpinLockCore* core = VD_NEW( SlottedSpinLockCore );
    vd::u32* locks = VD_NEW_ARRAY( vd::u32, capacity );

    for(size_t i = 0; i < capacity; i++)
    {
        locks[i] = 0;
    }

    core->locks = locks;
    core->capacity = capacity;
    m_Handle = reinterpret_cast<void*>(core);

#elif defined(VD_USE_POSIX)
    
    SlottedSpinLockCore* core = VD_NEW( SlottedSpinLockCore );
    ::pthread_spinlock_t* spins = VD_NEW_ARRAY( pthread_spinlock_t, capacity );

    for(size_t i = 0; i < capacity; i++)
    {
        if(::pthread_spin_init(spins + i, PTHREAD_PROCESS_PRIVATE) != 0)
		{
			vdGlobalException("pthread_spin_init", "Failed to initialize slotted spinlock!");
		}
    }

    core->spins = spins;
    core->capacity = capacity;
    m_Handle = reinterpret_cast<void*>(core);
    
#endif
}

// ============================================================================================== //

SlottedSpinLock::~SlottedSpinLock()
{
#if (defined(VD_TARGET_WINDOWS) || defined(VD_USE_GCC_ATOMICS))
    
    SlottedSpinLockCore* core = reinterpret_cast<SlottedSpinLockCore*>(m_Handle);
    VD_DELETE_ARRAY(core->locks);
    VD_DELETE(core);
    
#elif defined(VD_USE_POSIX)
    
    SlottedSpinLockCore* core = reinterpret_cast<SlottedSpinLockCore*>(m_Handle);
    ::pthread_spinlock_t* spins = core->spins;
    size_t capacity = core->capacity;

    for(size_t i = 0; i < capacity; i++)
    {
        ::pthread_spin_destroy(spins + i);
    }

    VD_DELETE_ARRAY(spins);
    VD_DELETE(core);

#endif
}

// ============================================================================================== //

void SlottedSpinLock::Lock(size_t idx)
{
#if defined(VD_TARGET_WINDOWS)
    
    SlottedSpinLockCore* core = reinterpret_cast<SlottedSpinLockCore*>(m_Handle);
    vd::u32* lock = core->locks + idx;
    vd::u32 wait_count = 0;

    while(::InterlockedCompareExchange((LONG*)lock, 1, 0) != 0)
    {
        if(wait_count >= VD_THREAD_SPINLOCK_THRESHOLD)
        {
            Thread::Nap();
        }
        else
        {
            Thread::Yield();
            wait_count++;
        }
    }

#elif defined(VD_USE_GCC_ATOMICS)
    
    SlottedSpinLockCore* core = reinterpret_cast<SlottedSpinLockCore*>(m_Handle);
    vd::u32* lock = core->locks + idx;
    vd::u32 wait_count = 0;

    while(!__sync_bool_compare_and_swap(lock, 0, 1))
    {
        if(wait_count >= VD_THREAD_SPINLOCK_THRESHOLD)
        {
            Thread::Nap();
        }
        else
        {
            Thread::Yield();
            wait_count++;
        }
    }

#elif defined(VD_USE_POSIX)
    
    SlottedSpinLockCore* core = reinterpret_cast<SlottedSpinLockCore*>(m_Handle);
    if(::pthread_spin_lock(core->spins + idx) != 0)
	{
		vdGlobalException("pthread_spin_lock", "Failed to lock slotted spinlock!");
	}

#endif
}

// ============================================================================================== //

void SlottedSpinLock::Unlock(size_t idx)
{
#if defined(VD_TARGET_WINDOWS)
    
    SlottedSpinLockCore* core = reinterpret_cast<SlottedSpinLockCore*>(m_Handle);
    vd::u32* lock = core->locks + idx;
    ::InterlockedExchange((LONG*)lock, 0);

#elif defined(VD_USE_GCC_ATOMICS)
    
    SlottedSpinLockCore* core = reinterpret_cast<SlottedSpinLockCore*>(m_Handle);
    vd::u32* lock = core->locks + idx;
    __sync_lock_release(lock);

#elif defined(VD_USE_POSIX)
    
    SlottedSpinLockCore* core = reinterpret_cast<SlottedSpinLockCore*>(m_Handle);
    if(::pthread_spin_unlock(core->spins + idx) != 0)
	{
		vdGlobalException("pthread_spin_unlock", "Failed to unlock slotted spinlock!");
	}

#endif
}

// ============================================================================================== //

void SlottedSpinLock::LockAll()
{
#if defined(VD_TARGET_WINDOWS)
    
    SlottedSpinLockCore* core = reinterpret_cast<SlottedSpinLockCore*>(m_Handle);
    vd::u32* locks = core->locks;
    size_t capacity = core->capacity;

    for(size_t i = 0; i < capacity; i++)
    {
        vd::u32* lock = locks + i;
        vd::u32 wait_count = 0;

        while(::InterlockedCompareExchange((LONG*)lock, 1, 0) != 0)
        {
            if(wait_count >= VD_THREAD_SPINLOCK_THRESHOLD)
            {
                Thread::Nap();
            }
            else
            {
                Thread::Yield();
                wait_count++;
            }
        }
    }

#elif defined(VD_USE_GCC_ATOMICS)
    
    SlottedSpinLockCore* core = reinterpret_cast<SlottedSpinLockCore*>(m_Handle);
    vd::u32* locks = core->locks;
    size_t capacity = core->capacity;

    for(size_t i = 0; i < capacity; i++)
    {
        vd::u32* lock = locks + i;
        vd::u32 wait_count = 0;

        while(!__sync_bool_compare_and_swap(lock, 0, 1))
        {
            if(wait_count >= VD_THREAD_SPINLOCK_THRESHOLD)
            {
                Thread::Nap();
            }
            else
            {
                Thread::Yield();
                wait_count++;
            }
        }
    }

#elif defined(VD_USE_POSIX)
    
    SlottedSpinLockCore* core = reinterpret_cast<SlottedSpinLockCore*>(m_Handle);
    ::pthread_spinlock_t* spins = core->spins;
    size_t capacity = core->capacity;

    for(size_t i = 0; i < capacity; i++)
    {
        if(::pthread_spin_lock(spins + i) != 0)
		{
			vdGlobalException("pthread_spin_lock", "Failed to lock slotted spinlock!");
		}
    }

#endif
}

// ============================================================================================== //

void SlottedSpinLock::UnlockAll()
{
#if defined(VD_TARGET_WINDOWS)
    
    SlottedSpinLockCore* core = reinterpret_cast<SlottedSpinLockCore*>(m_Handle);
    vd::u32* locks = core->locks;
    size_t capacity = core->capacity;

    for(size_t i = 0; i < capacity; i++)
    {
        vd::u32* lock = locks + i;
        ::InterlockedExchange((LONG*)lock, 0);
    }

#elif defined(VD_USE_GCC_ATOMICS)
    
    SlottedSpinLockCore* core = reinterpret_cast<SlottedSpinLockCore*>(m_Handle);
    vd::u32* locks = core->locks;
    size_t capacity = core->capacity;

    for(size_t i = 0; i < capacity; i++)
    {
        vd::u32* lock = locks + i;
        __sync_lock_release(lock);
    }

#elif defined(VD_USE_POSIX)
    
    SlottedSpinLockCore* core = reinterpret_cast<SlottedSpinLockCore*>(m_Handle);
    ::pthread_spinlock_t* spins = core->spins;
    size_t capacity = core->capacity;

    for(size_t i = 0; i < capacity; i++)
    {
        if(::pthread_spin_unlock(spins + i) != 0)
		{
			vdGlobalException("pthread_spin_unlock", "Failed to unlock slotted spinlock!");
		}
    }

#endif
}

// ============================================================================================== //

RWLock::RWLock() : m_Handle(NULL)
{
#if defined(VD_TARGET_WINDOWS)
    
    SpinRWLock* rwlock = VD_NEW( SpinRWLock );
    m_Handle = reinterpret_cast<void*>(rwlock);

#elif defined(VD_USE_POSIX)
    
    ::pthread_rwlock_t* rwlock = VD_NEW( pthread_rwlock_t );
    if(::pthread_rwlock_init(rwlock, NULL) != 0) 
	{
		vdGlobalException("pthread_rwlock_init", "Failed to init read-write lock!");
	}
    m_Handle = reinterpret_cast<void*>(rwlock);

#endif
}

// ============================================================================================== //

RWLock::~RWLock()
{
#if defined(VD_TARGET_WINDOWS)
    
    SpinRWLock* rwlock = reinterpret_cast<SpinRWLock*>(m_Handle);
    VD_DELETE(rwlock);

#elif defined(VD_USE_POSIX)
    
    ::pthread_rwlock_t* rwlock = reinterpret_cast<pthread_rwlock_t*>(m_Handle);
    ::pthread_rwlock_destroy(rwlock);
    VD_DELETE(rwlock);

#endif
}

// ============================================================================================== //

void RWLock::LockWriter()
{
#if defined(VD_TARGET_WINDOWS)
    
    SpinRWLock* rwlock = reinterpret_cast<SpinRWLock*>(m_Handle);
    rwlock->LockWriter();

#elif defined(VD_USE_POSIX)
    
    ::pthread_rwlock_t* rwlock = reinterpret_cast<pthread_rwlock_t*>(m_Handle);
    if(::pthread_rwlock_wrlock(rwlock) != 0) 
	{
		vdGlobalException("pthread_rwlock_wrlock", "Failed to enable write lock for read-write lock!");
	}

#endif
}

// ============================================================================================== //

bool RWLock::TryToLockWriter()
{
#if defined(VD_TARGET_WINDOWS)
    
    SpinRWLock* rwlock = reinterpret_cast<SpinRWLock*>(m_Handle);
    return rwlock->TryToLockWriter();

#elif defined(VD_USE_POSIX)
    
    ::pthread_rwlock_t* rwlock = reinterpret_cast<pthread_rwlock_t*>(m_Handle);
    int32_t ecode = ::pthread_rwlock_trywrlock(rwlock);

    if(ecode == 0) 
    	return true;

    if(ecode != EBUSY) 
	{
		vdGlobalException("pthread_rwlock_trywrlock", "Failed to check status of write lock for read-write lock!");
	}

    return false;
#endif
}

// ============================================================================================== //

void RWLock::LockReader()
{
#if defined(VD_TARGET_WINDOWS)
    
    SpinRWLock* rwlock = reinterpret_cast<SpinRWLock*>(m_Handle);
    rwlock->LockReader();
    
#elif defined(VD_USE_POSIX)
    
    ::pthread_rwlock_t* rwlock = reinterpret_cast<pthread_rwlock_t*>(m_Handle);
    if(::pthread_rwlock_rdlock(rwlock) != 0) 
	{
		vdGlobalException("pthread_rwlock_rdlock", "Failed to enable read lock for read-write lock!");
	}

#endif
}

// ============================================================================================== //

bool RWLock::TryToLockReader()
{
#if defined(VD_TARGET_WINDOWS)
    
    SpinRWLock* rwlock = reinterpret_cast<SpinRWLock*>(m_Handle);
    return rwlock->TryToLockReader();

#elif defined(VD_USE_POSIX)
    
    ::pthread_rwlock_t* rwlock = reinterpret_cast<pthread_rwlock_t*>(m_Handle);
    int32_t ecode = ::pthread_rwlock_tryrdlock(rwlock);

    if(ecode == 0) return true;

    if(ecode != EBUSY)
	{
		vdGlobalException("pthread_rwlock_tryrdlock", "Failed to check read lock status for read-write lock!");
	}

    return false;
#endif
}

// ============================================================================================== //

void RWLock::Unlock()
{
#if defined(VD_TARGET_WINDOWS)
    
    SpinRWLock* rwlock = reinterpret_cast<SpinRWLock*>(m_Handle);
    rwlock->Unlock();

#elif defined(VD_USE_POSIX)
    
    ::pthread_rwlock_t* rwlock = reinterpret_cast<pthread_rwlock_t*>(m_Handle);
    if(::pthread_rwlock_unlock(rwlock) != 0) 
	{
		vdGlobalException("pthread_rwlock_unlock", "Failed to unlock read-write lock!");
	}

#endif
}

// ============================================================================================== //

ScopedRWLock::ScopedRWLock(
    RWLock* rwlock, bool writer
) : m_RWLock(rwlock)
{
    vdGlobalAssert(rwlock);

    if(writer)
    {
        m_RWLock->LockWriter();
    }
    else
    {
        m_RWLock->LockReader();
    }
}

ScopedRWLock::~ScopedRWLock()
{
    vdGlobalAssert(true);
    m_RWLock->Unlock();
}

// ============================================================================================== //

struct SlottedRWLockCore
{
#if defined(VD_TARGET_WINDOWS)

    RWLock*             rwlocks;                      
    size_t              capacity;                       

#elif defined(VD_USE_POSIX)

    ::pthread_rwlock_t* rwlocks;           
    size_t              capacity;                      

#endif
};

// ============================================================================================== //

SlottedRWLock::SlottedRWLock(size_t capacity) : m_Handle(NULL)
{
#if defined(VD_TARGET_WINDOWS)
    
    SlottedRWLockCore* core = VD_NEW( SlottedRWLockCore );
    RWLock* rwlocks = VD_NEW_ARRAY( RWLock, capacity );
    core->rwlocks = rwlocks;
    core->capacity = capacity;
    m_Handle = reinterpret_cast<void*>(core);

#elif defined(VD_USE_POSIX)
    
    SlottedRWLockCore* core = VD_NEW( SlottedRWLockCore );
    ::pthread_rwlock_t* rwlocks = VD_NEW_ARRAY( pthread_rwlock_t, capacity );

    for(size_t i = 0; i < capacity; i++)
    {
        if(::pthread_rwlock_init(rwlocks + i, NULL) != 0)
		{
			vdGlobalException("pthread_rwlock_init", "Failed to initialize slotted read-write lock!");
		}
    }

    core->rwlocks = rwlocks;
    core->capacity = capacity;
    m_Handle = reinterpret_cast<void*>(core);
    
#endif
}

// ============================================================================================== //

SlottedRWLock::~SlottedRWLock()
{
#if defined(VD_TARGET_WINDOWS)
    
    SlottedRWLockCore* core = reinterpret_cast<SlottedRWLockCore*>(m_Handle);
    VD_DELETE_ARRAY(core->rwlocks);
    VD_DELETE(core);

#elif defined(VD_USE_POSIX)
    
    SlottedRWLockCore* core = reinterpret_cast<SlottedRWLockCore*>(m_Handle);
    ::pthread_rwlock_t* rwlocks = core->rwlocks;
    size_t capacity = core->capacity;

    for(size_t i = 0; i < capacity; i++)
    {
        ::pthread_rwlock_destroy(rwlocks + i);
    }

    VD_DELETE_ARRAY(rwlocks);
    VD_DELETE(core);

#endif
}

// ============================================================================================== //

void SlottedRWLock::LockWriter(size_t idx)
{
#if defined(VD_TARGET_WINDOWS)
    
    SlottedRWLockCore* core = reinterpret_cast<SlottedRWLockCore*>(m_Handle);
    core->rwlocks[idx].LockWriter();
#else
    
    SlottedRWLockCore* core = reinterpret_cast<SlottedRWLockCore*>(m_Handle);

    if(::pthread_rwlock_wrlock(core->rwlocks + idx) != 0)
	{
		vdGlobalException("pthread_rwlock_wrlock", "Failed to enable write lock for slotted read-write lock!");
	}

#endif
}

// ============================================================================================== //

void SlottedRWLock::LockReader(size_t idx)
{
#if defined(VD_TARGET_WINDOWS)
    SlottedRWLockCore* core = reinterpret_cast<SlottedRWLockCore*>(m_Handle);
    core->rwlocks[idx].LockReader();
#else
    
    SlottedRWLockCore* core = reinterpret_cast<SlottedRWLockCore*>(m_Handle);

    if(::pthread_rwlock_rdlock(core->rwlocks + idx) != 0)
	{
		vdGlobalException("pthread_rwlock_rdlock", "Failed to enable read lock for slotted read-write lock!");
	}

#endif
}

// ============================================================================================== //

void SlottedRWLock::Unlock(size_t idx)
{
#if defined(VD_TARGET_WINDOWS)
    SlottedRWLockCore* core = reinterpret_cast<SlottedRWLockCore*>(m_Handle);
    core->rwlocks[idx].Unlock();
#else
    
    SlottedRWLockCore* core = reinterpret_cast<SlottedRWLockCore*>(m_Handle);

    if(::pthread_rwlock_unlock(core->rwlocks + idx) != 0)
	{
		vdGlobalException("pthread_rwlock_unlock", "Failed to disable lock for slotted read-write lock!");
	}

#endif
}

// ============================================================================================== //

void SlottedRWLock::LockAllWriters()
{
#if defined(VD_TARGET_WINDOWS)
    
    SlottedRWLockCore* core = reinterpret_cast<SlottedRWLockCore*>(m_Handle);
    RWLock* rwlocks = core->rwlocks;
    size_t capacity = core->capacity;

    for(size_t i = 0; i < capacity; i++)
    {
        rwlocks[i].LockWriter();
    }

#else
    
    SlottedRWLockCore* core = reinterpret_cast<SlottedRWLockCore*>(m_Handle);
    ::pthread_rwlock_t* rwlocks = core->rwlocks;
    size_t capacity = core->capacity;

    for(size_t i = 0; i < capacity; i++)
    {
        if(::pthread_rwlock_wrlock(rwlocks + i) != 0)
		{
			vdGlobalException("pthread_rwlock_wrlock", "Failed to enable write lock for slotted read-write lock!");
		}
    }

#endif
}

// ============================================================================================== //

void SlottedRWLock::LockAllReaders()
{
#if defined(VD_TARGET_WINDOWS)
    
    SlottedRWLockCore* core = reinterpret_cast<SlottedRWLockCore*>(m_Handle);
    RWLock* rwlocks = core->rwlocks;
    size_t capacity = core->capacity;

    for(size_t i = 0; i < capacity; i++)
    {
        rwlocks[i].LockReader();
    }

#else
    
    SlottedRWLockCore* core = reinterpret_cast<SlottedRWLockCore*>(m_Handle);
    ::pthread_rwlock_t* rwlocks = core->rwlocks;
    size_t capacity = core->capacity;

    for(size_t i = 0; i < capacity; i++)
    {
        if(::pthread_rwlock_rdlock(rwlocks + i) != 0)
		{
			vdGlobalException("pthread_rwlock_rdlock", "Failed to enable read lock for slotted read-write lock!");
		}
    }

#endif
}

// ============================================================================================== //

void SlottedRWLock::UnlockAll()
{
#if defined(VD_TARGET_WINDOWS)
    
    SlottedRWLockCore* core = reinterpret_cast<SlottedRWLockCore*>(m_Handle);
    RWLock* rwlocks = core->rwlocks;
    size_t capacity = core->capacity;

    for(size_t i = 0; i < capacity; i++)
    {
        rwlocks[i].Unlock();
    }

#else
    
    SlottedRWLockCore* core = reinterpret_cast<SlottedRWLockCore*>(m_Handle);
    ::pthread_rwlock_t* rwlocks = core->rwlocks;
    size_t capacity = core->capacity;

    for(size_t i = 0; i < capacity; i++)
    {
        if(::pthread_rwlock_unlock(rwlocks + i) != 0)
		{
			vdGlobalException("pthread_rwlock_unlock", "Failed to disable lock for slotted read-write lock!");
		}
    }

#endif
}

// ============================================================================================== //

struct SpinRWLockCore
{
#if defined(VD_TARGET_WINDOWS)
    LONG                    semaphore;                           
    vd::u32                 counter;                         
#elif defined(VD_USE_GCC_ATOMICS)
    int32_t                 semaphore;                         
    vd::u32                 counter;                        
#else
    ::pthread_spinlock_t    semaphore;             
    vd::u32                 counter;                         
#endif
};

// ============================================================================================== //

static void DoLockForSpinRWLock(SpinRWLockCore* core);
static void DoUnlockForSpinRWLock(SpinRWLockCore* core);

// ============================================================================================== //

SpinRWLock::SpinRWLock() : m_Handle(NULL)
{
#if (defined(VD_TARGET_WINDOWS) || defined(VD_USE_GCC_ATOMICS))
    
    SpinRWLockCore* core = VD_NEW( SpinRWLockCore );
    core->semaphore = 0;
    core->counter = 0;
    m_Handle = reinterpret_cast<void*>(core);
    
#else
    
    SpinRWLockCore* core = VD_NEW( SpinRWLockCore );

    if(::pthread_spin_init(&core->semaphore, PTHREAD_PROCESS_PRIVATE) != 0)
	{
		vdGlobalException("pthread_spin_init", "Failed to initialize spinlock!");
	}

    core->counter = 0;
    m_Handle = reinterpret_cast<void*>(core);

#endif
}

// ============================================================================================== //

SpinRWLock::~SpinRWLock()
{
#if (defined(VD_TARGET_WINDOWS) || defined(VD_USE_GCC_ATOMICS))
    
    SpinRWLockCore* core = reinterpret_cast<SpinRWLockCore*>(m_Handle);
    VD_DELETE(core);

#else
    
    SpinRWLockCore* core = reinterpret_cast<SpinRWLockCore*>(m_Handle);
    ::pthread_spin_destroy(&core->semaphore);
    VD_DELETE(core);

#endif
}

// ============================================================================================== //

void SpinRWLock::LockWriter()
{
    
    vd::u32 wait_count = 0;
    SpinRWLockCore* core = reinterpret_cast<SpinRWLockCore*>(m_Handle);

    DoLockForSpinRWLock(core);
    while(core->counter > 0)
    {
        DoUnlockForSpinRWLock(core);

        if(wait_count >= VD_THREAD_SPINLOCK_THRESHOLD)
        {
            Thread::Nap();
        }
        else
        {
            Thread::Yield();
            wait_count++;
        }

        DoLockForSpinRWLock(core);
    }

    core->counter = VD_I32_MAX;
    DoUnlockForSpinRWLock(core);
}

// ============================================================================================== //

bool SpinRWLock::TryToLockWriter()
{
    
    SpinRWLockCore* core = reinterpret_cast<SpinRWLockCore*>(m_Handle);
    DoLockForSpinRWLock(core);

    if(core->counter > 0)
    {
        DoUnlockForSpinRWLock(core);
        return false;
    }

    core->counter = VD_I32_MAX;
    DoUnlockForSpinRWLock(core);
    return true;
}

// ============================================================================================== //

void SpinRWLock::LockReader()
{
    
    SpinRWLockCore* core = reinterpret_cast<SpinRWLockCore*>(m_Handle);
    DoLockForSpinRWLock(core);

    vd::u32 wait_count = 0;
    while(core->counter >= (vd::u32)VD_I32_MAX)
    {
        DoUnlockForSpinRWLock(core);

        if(wait_count >= VD_THREAD_SPINLOCK_THRESHOLD)
        {
            Thread::Nap();
        }
        else
        {
            Thread::Yield();
            wait_count++;
        }

        DoLockForSpinRWLock(core);
    }

    core->counter++;
    DoUnlockForSpinRWLock(core);
}

// ============================================================================================== //

bool SpinRWLock::TryToLockReader()
{  
    SpinRWLockCore* core = reinterpret_cast<SpinRWLockCore*>(m_Handle);
    DoLockForSpinRWLock(core);

    if(core->counter >= (vd::u32)VD_I32_MAX)
    {
        DoUnlockForSpinRWLock(core);
        return false;
    }

    core->counter++;
    DoUnlockForSpinRWLock(core);
    return true;
}

// ============================================================================================== //

void SpinRWLock::Unlock()
{
    SpinRWLockCore* core = reinterpret_cast<SpinRWLockCore*>(m_Handle);
    DoLockForSpinRWLock(core);

    if(core->counter >= (vd::u32)VD_I32_MAX)
    {
        core->counter = 0;
    }
    else
    {
        core->counter--;
    }

    DoUnlockForSpinRWLock(core);
}

// ============================================================================================== //

bool SpinRWLock::Promote()
{    
    SpinRWLockCore* core = reinterpret_cast<SpinRWLockCore*>(m_Handle);
    DoLockForSpinRWLock(core);

    if(core->counter > 1)
    {
        DoUnlockForSpinRWLock(core);
        return false;
    }

    core->counter = VD_I32_MAX;
    DoUnlockForSpinRWLock(core);
    return true;
}

// ============================================================================================== //

void SpinRWLock::Demote()
{
    SpinRWLockCore* core = reinterpret_cast<SpinRWLockCore*>(m_Handle);
    DoLockForSpinRWLock(core);
    core->counter = 1;
    DoUnlockForSpinRWLock(core);
}

// ============================================================================================== //

static void DoLockForSpinRWLock(SpinRWLockCore* core)
{
    vdGlobalAssert(core != NULL);

#if defined(VD_TARGET_WINDOWS)

    while(::InterlockedCompareExchange(&core->semaphore, 1, 0) != 0)
    {
        ::Sleep(0);
    }

#elif defined(VD_USE_GCC_ATOMICS)

    while(!__sync_bool_compare_and_swap(&core->semaphore, 0, 1))
    {
        ::sched_yield();
    }

#elif defined(VD_USE_POSIX)

    if(::pthread_spin_lock(&core->semaphore) != 0) 
	{
		vdGlobalException("pthread_spin_lock", "Failed to initialize spinlock for spin-rw-lock!");
	}

#endif
}

// ============================================================================================== //

static void DoUnlockForSpinRWLock(SpinRWLockCore* core)
{
    vdGlobalAssert(core != NULL);

#if defined(VD_TARGET_WINDOWS)

    ::InterlockedExchange(&core->semaphore, 0);

#elif defined(VD_USE_GCC_ATOMICS)

    __sync_lock_release(&core->semaphore);

#elif defined(VD_USE_POSIX)

    if(::pthread_spin_unlock(&core->semaphore) != 0) 
	{
		vdGlobalException("pthread_spin_unlock", "Failed to unlock spinlock for spin-rw-lock!");
	}

#endif
}

// ============================================================================================== //

ScopedSpinRWLock::ScopedSpinRWLock(
    SpinRWLock* srwlock, bool writer
) : m_SpinRWLock(srwlock)
{
    vdGlobalAssert(srwlock);

    if(writer)
    {
        m_SpinRWLock->LockWriter();
    }
    else
    {
        m_SpinRWLock->LockReader();
    }
}

// ============================================================================================== //

ScopedSpinRWLock::~ScopedSpinRWLock()
{
    vdGlobalAssert(true);
    m_SpinRWLock->Unlock();
}

// ============================================================================================== //

struct SlottedSpinRWLockCore
{
#if defined(VD_TARGET_WINDOWS)
    LONG                        semaphores[VD_THREAD_LOCK_SEMA_COUNT];       
#elif defined(VD_USE_GCC_ATOMICS)
    int32_t                     semaphores[VD_THREAD_LOCK_SEMA_COUNT];         
#else
    ::pthread_spinlock_t        semaphores[VD_THREAD_LOCK_SEMA_COUNT];
#endif
    vd::u32*                    counters;                        
    size_t                      capacity;                       
};

// ============================================================================================== //

static void DoLockForSlottedSpinRWLock(SlottedSpinRWLockCore* core, size_t idx);
static void DoUnlockForSlottedSpinRWLock(SlottedSpinRWLockCore* core, size_t idx);

// ============================================================================================== //

SlottedSpinRWLock::SlottedSpinRWLock(size_t capacity) : m_Handle(NULL)
{
#if defined(VD_TARGET_WINDOWS)

    SlottedSpinRWLockCore* core = VD_NEW( SlottedSpinRWLockCore );
    LONG* semaphores = core->semaphores;
    vd::u32* counters = VD_NEW_ARRAY( vd::u32, capacity );

    for(size_t i = 0; i < VD_THREAD_LOCK_SEMA_COUNT; i++)
    {
        semaphores[i] = 0;
    }

    for(size_t i = 0; i < capacity; i++)
    {
        counters[i] = 0;
    }

    core->counters = counters;
    core->capacity = capacity;
    m_Handle = reinterpret_cast<void*>(core);

#elif defined(VD_USE_GCC_ATOMICS)

    SlottedSpinRWLockCore* core = VD_NEW( SlottedSpinRWLockCore );
    int32_t* semaphores = core->semaphores;
    vd::u32* counters = VD_NEW_ARRAY( vd::u32, capacity );

    for(size_t i = 0; i < VD_THREAD_LOCK_SEMA_COUNT; i++)
    {
        semaphores[i] = 0;
    }

    for(size_t i = 0; i < capacity; i++)
    {
        counters[i] = 0;
    }

    core->counters = counters;
    core->capacity = capacity;
    m_Handle = reinterpret_cast<void*>(core);

#elif defined(VD_USE_POSIX)
    
    SlottedSpinRWLockCore* core = VD_NEW( SlottedSpinRWLockCore );
    ::pthread_spinlock_t* semaphores = core->semaphores;
    vd::u32* counters = VD_NEW_ARRAY( vd::u32, capacity );

    for(size_t i = 0; i < VD_THREAD_LOCK_SEMA_COUNT; i++)
    {
        if(::pthread_spin_init(semaphores + i, PTHREAD_PROCESS_PRIVATE) != 0)
		{
			vdGlobalException("pthread_spin_unlock", "Failed to initialize spinlock for spin-rw-lock!");
		}
    }

    for(size_t i = 0; i < capacity; i++)
    {
        counters[i] = 0;
    }

    core->counters = counters;
    core->capacity = capacity;
    m_Handle = reinterpret_cast<void*>(core);

#endif
}

// ============================================================================================== //

SlottedSpinRWLock::~SlottedSpinRWLock()
{
#if defined(VD_TARGET_WINDOWS) || defined(VD_USE_GCC_ATOMICS)
    
    SlottedSpinRWLockCore* core = reinterpret_cast<SlottedSpinRWLockCore*>(m_Handle);
    VD_DELETE_ARRAY(core->counters);
    VD_DELETE(core);

#elif defined(VD_USE_POSIX)
    
    SlottedSpinRWLockCore* core = reinterpret_cast<SlottedSpinRWLockCore*>(m_Handle);
    ::pthread_spinlock_t* semaphores = core->semaphores;

    for(size_t i = 0; i < VD_THREAD_LOCK_SEMA_COUNT; i++)
    {
        ::pthread_spin_destroy(semaphores + i);
    }

    VD_DELETE_ARRAY(core->counters);
    VD_DELETE(core);

#endif

	m_Handle = NULL;
}

// ============================================================================================== //

void SlottedSpinRWLock::LockWriter(size_t idx)
{
    SlottedSpinRWLockCore* core = reinterpret_cast<SlottedSpinRWLockCore*>(m_Handle);
    size_t semidx = idx % VD_THREAD_LOCK_SEMA_COUNT;
    DoLockForSlottedSpinRWLock(core, semidx);
    vd::u32 wait_count = 0;

    while(core->counters[idx] > 0)
    {
        DoUnlockForSlottedSpinRWLock(core, semidx);

        if(wait_count >= VD_THREAD_SPINLOCK_THRESHOLD)
        {
            Thread::Nap();
        }
        else
        {
            Thread::Yield();
            wait_count++;
        }

        DoLockForSlottedSpinRWLock(core, semidx);
    }

    core->counters[idx] = VD_I32_MAX;
    DoUnlockForSlottedSpinRWLock(core, semidx);
}

// ============================================================================================== //

void SlottedSpinRWLock::LockReader(size_t idx)
{   
    SlottedSpinRWLockCore* core = reinterpret_cast<SlottedSpinRWLockCore*>(m_Handle);
    size_t semidx = idx % VD_THREAD_LOCK_SEMA_COUNT;
    DoLockForSlottedSpinRWLock(core, semidx);
    vd::u32 wait_count = 0;

    while(core->counters[idx] >= (vd::u32)VD_I32_MAX)
    {
        DoUnlockForSlottedSpinRWLock(core, semidx);

        if(wait_count >= VD_THREAD_SPINLOCK_THRESHOLD)
        {
            Thread::Nap();
        }
        else
        {
            Thread::Yield();
            wait_count++;
        }

        DoLockForSlottedSpinRWLock(core, semidx);
    }

    core->counters[idx]++;
    DoUnlockForSlottedSpinRWLock(core, semidx);
}

// ============================================================================================== //

void SlottedSpinRWLock::Unlock(size_t idx)
{   
    SlottedSpinRWLockCore* core = reinterpret_cast<SlottedSpinRWLockCore*>(m_Handle);
    size_t semidx = idx % VD_THREAD_LOCK_SEMA_COUNT;
    DoLockForSlottedSpinRWLock(core, semidx);

    if(core->counters[idx] >= (vd::u32)VD_I32_MAX)
    {
        core->counters[idx] = 0;
    }
    else
    {
        core->counters[idx]--;
    }

    DoUnlockForSlottedSpinRWLock(core, semidx);
}

// ============================================================================================== //

void SlottedSpinRWLock::LockAllWriters()
{
    SlottedSpinRWLockCore* core = reinterpret_cast<SlottedSpinRWLockCore*>(m_Handle);
    vd::u32* counters = core->counters;
    size_t capacity = core->capacity;

    for(size_t i = 0; i < capacity; i++)
    {
        size_t semidx = i % VD_THREAD_LOCK_SEMA_COUNT;
        DoLockForSlottedSpinRWLock(core, semidx);
        vd::u32 wait_count = 0;

        while(counters[i] > 0)
        {
            DoUnlockForSlottedSpinRWLock(core, semidx);

            if(wait_count >= VD_THREAD_SPINLOCK_THRESHOLD)
            {
                Thread::Nap();
            }
            else
            {
                Thread::Yield();
                wait_count++;
            }

            DoLockForSlottedSpinRWLock(core, semidx);
        }

        counters[i] = VD_I32_MAX;
        DoUnlockForSlottedSpinRWLock(core, semidx);
    }
}

// ============================================================================================== //

void SlottedSpinRWLock::LockAllReaders()
{
    SlottedSpinRWLockCore* core = reinterpret_cast<SlottedSpinRWLockCore*>(m_Handle);
    vd::u32* counters = core->counters;
    size_t capacity = core->capacity;

    for(size_t i = 0; i < capacity; i++)
    {
        size_t semidx = i % VD_THREAD_LOCK_SEMA_COUNT;
        DoLockForSlottedSpinRWLock(core, semidx);
        vd::u32 wait_count = 0;

        while(counters[i] >= (vd::u32)VD_I32_MAX)
        {
            DoUnlockForSlottedSpinRWLock(core, semidx);

            if(wait_count >= VD_THREAD_SPINLOCK_THRESHOLD)
            {
                Thread::Nap();
            }
            else
            {
                Thread::Yield();
                wait_count++;
            }

            DoLockForSlottedSpinRWLock(core, semidx);
        }

        counters[i]++;
        DoUnlockForSlottedSpinRWLock(core, semidx);
    }
}

// ============================================================================================== //

void SlottedSpinRWLock::UnlockAll()
{
    SlottedSpinRWLockCore* core = reinterpret_cast<SlottedSpinRWLockCore*>(m_Handle);
    vd::u32* counters = core->counters;
    size_t capacity = core->capacity;

    for(size_t i = 0; i < capacity; i++)
    {
        size_t semidx = i % VD_THREAD_LOCK_SEMA_COUNT;
        DoLockForSlottedSpinRWLock(core, semidx);

        if(counters[i] >= (vd::u32)VD_I32_MAX)
        {
            counters[i] = 0;
        }
        else
        {
            counters[i]--;
        }

        DoUnlockForSlottedSpinRWLock(core, semidx);
    }
}

// ============================================================================================== //

static void 
DoLockForSlottedSpinRWLock(
	SlottedSpinRWLockCore* core, size_t idx)
{
    vdGlobalAssert(core != NULL);

#if defined(VD_TARGET_WINDOWS)

    while(::InterlockedCompareExchange(core->semaphores + idx, 1, 0) != 0)
    {
        ::Sleep(0);
    }

#elif defined(VD_USE_GCC_ATOMICS)

    while(!__sync_bool_compare_and_swap(core->semaphores + idx, 0, 1))
    {
        ::sched_yield();
    }

#elif defined(VD_USE_POSIX)

    if(::pthread_spin_lock(core->semaphores + idx) != 0) 
	{
		vdGlobalException("pthread_spin_lock", "Failed to lock spinlock for slotted-spin-rw-lock!");
	}

#endif
}

// ============================================================================================== //

static void 
DoUnlockForSlottedSpinRWLock(
	SlottedSpinRWLockCore* core, size_t idx)
{
    vdGlobalAssert(core != NULL);

#if defined(VD_TARGET_WINDOWS)

    ::InterlockedExchange(core->semaphores + idx, 0);

#elif defined(VD_USE_GCC_ATOMICS)

    __sync_lock_release(core->semaphores + idx);

#elif defined(VD_USE_POSIX)

    if(::pthread_spin_unlock(core->semaphores + idx) != 0)
	{
		vdGlobalException("pthread_spin_unlock", "Failed to unlock spinlock for slotted-spin-rw-lock!");
	}

#endif
}

// ============================================================================================== //

ConditionVariable::ConditionVariable() : m_Handle(NULL)
{
#if defined(VD_TARGET_WINDOWS)
    
    ConditionVariableCore* core = VD_NEW( ConditionVariableCore );
    ::InitializeCriticalSection(&core->mutex);
    core->wait = 0;
    core->wake = 0;
    core->emitter = ::CreateEvent(NULL, true, false, NULL);
    if(!core->emitter) 
	{
		vdGlobalException("CreateEvent", "Failed to create event for condition variable!");
	}

    core->finisher = ::CreateEvent(NULL, false, false, NULL);
    if(!core->finisher) 
	{
		vdGlobalException("CreateEvent", "Failed to create event for condition variable!");
	}

    m_Handle = reinterpret_cast<void*>(core);

#elif defined(VD_USE_POSIX)
    
    ConditionVariableCore* core = VD_NEW( ConditionVariableCore );

    if(::pthread_cond_init(&core->condition, NULL) != 0)
	{
		vdGlobalException("pthread_cond_init", "Failed to initialize condition variable!");
	}

    m_Handle = reinterpret_cast<void*>(core);
    
#endif
}

// ============================================================================================== //

ConditionVariable::~ConditionVariable()
{
#if defined(VD_TARGET_WINDOWS)
    
    ConditionVariableCore* core = reinterpret_cast<ConditionVariableCore*>(m_Handle);
    ::CloseHandle(core->finisher);
    ::CloseHandle(core->emitter);
    ::DeleteCriticalSection(&core->mutex);
    VD_DELETE(core);

#elif defined(VD_USE_POSIX)
    
    ConditionVariableCore* core = reinterpret_cast<ConditionVariableCore*>(m_Handle);
    ::pthread_cond_destroy(&core->condition);
    VD_DELETE(core);

#endif

	m_Handle = NULL;
}

// ============================================================================================== //

void ConditionVariable::Wait(Mutex* mutex)
{
    vdGlobalAssert(mutex != NULL);

#if defined(VD_TARGET_WINDOWS)

    ConditionVariableCore* core = reinterpret_cast<ConditionVariableCore*>(m_Handle);
    ::CRITICAL_SECTION* self_mutex = (::CRITICAL_SECTION*)mutex->m_Handle;
    ::EnterCriticalSection(&core->mutex);
    core->wait++;
    ::LeaveCriticalSection(&core->mutex);
    ::LeaveCriticalSection(self_mutex);

    while(true)
    {
        ::WaitForSingleObject(core->emitter, INFINITE);
        ::EnterCriticalSection(&core->mutex);

        if(core->wake > 0)
        {
            core->wait--;
            core->wake--;

            if(core->wake < 1)
            {
                ::ResetEvent(core->emitter);
                ::SetEvent(core->finisher);
            }

            ::LeaveCriticalSection(&core->mutex);
            break;
        }

        ::LeaveCriticalSection(&core->mutex);
    }

    ::EnterCriticalSection(self_mutex);

#elif defined(VD_USE_POSIX)

    ConditionVariableCore* core = reinterpret_cast<ConditionVariableCore*>(m_Handle);
    ::pthread_mutex_t* self_mutex = (::pthread_mutex_t*)mutex->m_Handle;

    if(::pthread_cond_wait(&core->condition, self_mutex) != 0)
        vdGlobalException("pthread_cond_wait", "Condition variable wait failed!");

#endif
}

// ============================================================================================== //

bool ConditionVariable::Wait(
    Mutex* mutex, vd::f64 sec)
{
    vdGlobalAssert(mutex != NULL && sec >= 0.0);

#if defined(VD_TARGET_WINDOWS)

    if(sec <= 0) return false;

    ConditionVariableCore* core = reinterpret_cast<ConditionVariableCore*>(m_Handle);
    ::CRITICAL_SECTION* self_mutex = (::CRITICAL_SECTION*)mutex->m_Handle;
    ::EnterCriticalSection(&core->mutex);
    core->wait++;
    ::LeaveCriticalSection(&core->mutex);
    ::LeaveCriticalSection(self_mutex);

    while(true)
    {
        if(::WaitForSingleObject(core->emitter, sec * 1000) == WAIT_TIMEOUT)
        {
            ::EnterCriticalSection(&core->mutex);
            core->wait--;
            ::SetEvent(core->finisher);
            ::LeaveCriticalSection(&core->mutex);
            ::EnterCriticalSection(self_mutex);
            return false;
        }

        ::EnterCriticalSection(&core->mutex);

        if(core->wake > 0)
        {
            core->wait--;
            core->wake--;

            if(core->wake < 1)
            {
                ::ResetEvent(core->emitter);
                ::SetEvent(core->finisher);
            }

            ::LeaveCriticalSection(&core->mutex);
            break;
        }

        ::LeaveCriticalSection(&core->mutex);
    }

    ::EnterCriticalSection(self_mutex);
    return true;

#elif defined(VD_USE_POSIX)

    if(sec <= 0) return false;

    ConditionVariableCore* core = reinterpret_cast<ConditionVariableCore*>(m_Handle);
    ::pthread_mutex_t* self_mutex = (::pthread_mutex_t*)mutex->m_Handle;
    struct ::timeval tv;
    struct ::timespec ts;

    if(::gettimeofday(&tv, NULL) == 0)
    {
        vd::f64 integ;
        vd::f64 fract = std::modf(sec, &integ);
        ts.tv_sec = tv.tv_sec + (time_t)integ;
        ts.tv_nsec = (long)(tv.tv_usec * 1000.0 + fract * 999999000);

        if(ts.tv_nsec >= 1000000000)
        {
            ts.tv_nsec -= 1000000000;
            ts.tv_sec++;
        }
    }
    else
    {
        ts.tv_sec = ::time(NULL) + 1;
        ts.tv_nsec = 0;
    }

    int32_t ecode = ::pthread_cond_timedwait(&core->condition, self_mutex, &ts);

    if(ecode == 0) 
    	return true;

    if(ecode != ETIMEDOUT && ecode != EINTR)
        vdGlobalException("pthread_cond_timedwait", "Condition variable timed wait failed!");

    return false;

#endif
}

// ============================================================================================== //

void ConditionVariable::Signal()
{
#if defined(VD_TARGET_WINDOWS)
    
    ConditionVariableCore* core = reinterpret_cast<ConditionVariableCore*>(m_Handle);
    ::EnterCriticalSection(&core->mutex);

    if(core->wait > 0)
    {
        core->wake = 1;
        ::SetEvent(core->emitter);
        ::LeaveCriticalSection(&core->mutex);
        ::WaitForSingleObject(core->finisher, INFINITE);
    }
    else
    {
        ::LeaveCriticalSection(&core->mutex);
    }

#elif defined(VD_USE_POSIX)
    
    ConditionVariableCore* core = reinterpret_cast<ConditionVariableCore*>(m_Handle);
    if(::pthread_cond_signal(&core->condition) != 0)
        vdGlobalException("pthread_cond_signal", "Condition variable signal failed!");

#endif
}

// ============================================================================================== //

void ConditionVariable::Broadcast()
{
#if defined(VD_TARGET_WINDOWS)
    
    ConditionVariableCore* core = reinterpret_cast<ConditionVariableCore*>(m_Handle);
    ::EnterCriticalSection(&core->mutex);

    if(core->wait > 0)
    {
        core->wake = core->wait;
        ::SetEvent(core->emitter);
        ::LeaveCriticalSection(&core->mutex);
        ::WaitForSingleObject(core->finisher, INFINITE);
    }
    else
    {
        ::LeaveCriticalSection(&core->mutex);
    }

#elif defined(VD_USE_POSIX)
    
    ConditionVariableCore* core = reinterpret_cast<ConditionVariableCore*>(m_Handle);
    if(::pthread_cond_broadcast(&core->condition) != 0)
        vdGlobalException("pthread_cond_broadcast", "Condition variable broadcast failed!");

#endif
}

// ============================================================================================== //

ConditionMap::ConditionMap() : m_Slots()
{
    vdGlobalAssert(true);
}
    
// ============================================================================================== //

ConditionMap::~ConditionMap()
{
    vdGlobalAssert(true);
}

// ============================================================================================== //

bool ConditionMap::Wait(
    size_t key, vd::f64 sec)
{
    vdGlobalAssert(true);
    vd::f64 invtime = sec < 0 ? 1.0 : sec;
    vd::f64 curtime = Process::GetTimeInSeconds();
    vd::f64 endtime = curtime + (sec < 0 ? VD_U32_MAX : sec);
    Slot* slot = FindSlot(key);

    while(curtime < endtime)
    {
        ScopedMutex lock(&slot->Lock);
        CountMap::iterator cit = slot->Counters.find(key);

        if(cit == slot->Counters.end())
        {
            Count cnt = { 1, false };
            slot->Counters[key] = cnt;
        }
        else
        {
            cit->second.Queued++;
        }

        slot->Condition.Wait(&slot->Lock, invtime);
        cit = slot->Counters.find(key);
        cit->second.Queued--;

        if(cit->second.Running > 0)
        {
            cit->second.Running--;

            if(cit->second.Queued < 1) slot->Counters.erase(cit);

            return true;
        }

        if(cit->second.Queued < 1) slot->Counters.erase(cit);

        curtime = Process::GetTimeInSeconds();
    }

    return false;
}

// ============================================================================================== //

size_t ConditionMap::Signal(
    size_t key)
{
    vdGlobalAssert(true);
    Slot* slot = FindSlot(key);
    ScopedMutex lock(&slot->Lock);
    CountMap::iterator cit = slot->Counters.find(key);

    if(cit == slot->Counters.end() || cit->second.Queued < 1) return 0;

    if(cit->second.Running < cit->second.Queued) cit->second.Running++;

    slot->Condition.Broadcast();
    return cit->second.Queued;
}

// ============================================================================================== //

size_t ConditionMap::Broadcast(
    size_t key)
{
    vdGlobalAssert(true);
    Slot* slot = FindSlot(key);
    ScopedMutex lock(&slot->Lock);
    CountMap::iterator cit = slot->Counters.find(key);

    if(cit == slot->Counters.end() || cit->second.Queued < 1) return 0;

    cit->second.Running = cit->second.Queued;
    slot->Condition.Broadcast();
    return cit->second.Queued;
}

// ============================================================================================== //

size_t ConditionMap::BroadcastToAll()
{
    vdGlobalAssert(true);
    size_t sum = 0;

    for(size_t i = 0; i < ConditionMap::SlotCount; i++)
    {
        Slot* slot = m_Slots + i;
        ScopedMutex lock(&slot->Lock);
        CountMap::iterator cit = slot->Counters.begin();
        CountMap::iterator citend = slot->Counters.end();

        while(cit != citend)
        {
            if(cit->second.Queued > 0)
            {
                cit->second.Running = cit->second.Queued;
                sum += cit->second.Queued;
            }

            slot->Condition.Broadcast();
            ++cit;
        }
    }

    return sum;
}

// ============================================================================================== //

size_t ConditionMap::GetWaitCount()
{
    vdGlobalAssert(true);
    size_t sum = 0;

    for(size_t i = 0; i < ConditionMap::SlotCount; i++)
    {
        Slot* slot = m_Slots + i;
        ScopedMutex lock(&slot->Lock);
        CountMap::iterator cit = slot->Counters.begin();
        CountMap::iterator citend = slot->Counters.end();

        while(cit != citend)
        {
            sum += cit->second.Queued;
            ++cit;
        }
    }

    return sum;
}

// ============================================================================================== //

ConditionMap::Slot* 
ConditionMap::FindSlot(size_t key)
{
    return m_Slots + key % ConditionMap::SlotCount;
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

