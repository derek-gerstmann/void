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

#include "core/core.h"
#include "core/threading.h"
#include "core/locking.h"
#include "core/logging.h"
#include "core/asserts.h"
#include "core/handles.h"
#include "core/process.h"
#include "core/hashing.h"
#include "core/symbol.h"

#include "constants/constants.h"

// ============================================================================================== //

#if  defined(VD_TARGET_WINDOWS)
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
    #include <sys/prctl.h>
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
    const vd::u32   VD_THREAD_NAP_TIME_IN_MILLISEC = 21;
}

// ============================================================================================== //

struct ThreadCore
{
#if defined(VD_TARGET_WINDOWS)
    typedef ::HANDLE                NativeType;                      
#elif defined(VD_USE_POSIX)
    typedef ::pthread_t             NativeType;                  
#endif
    static const NativeType         Invalid;

    NativeType                      Native;
    bool                            IsAlive;                    
    bool                            IsRunning;                    
    bool                            IsCritical;                    
    vd::i32                         Affinity;
    vd::i32                         StackSize;
    Handle<LogContext>              LogCtx;
    Core::Mutex                     Mutex;
    char*                           Name;
};

const ThreadCore::NativeType ThreadCore::Invalid = (ThreadCore::NativeType)NULL;

// ============================================================================================== //

class MainThread : public Thread
{
public:

    MainThread() : Thread("Main", true)
    {
        // EMTPY!
    }

    virtual void Run()
    {
        vdLogError("The main thread is already running!");
    }

    virtual const char* GetName() const
    {
        return "Main";
    }

    VD_DECLARE_OBJECT(MainThread);
    
protected:

    VD_DISABLE_COPY_CONSTRUCTORS(MainThread);

    virtual ~MainThread() 
    { 
        Destroy(); 
    }
};

// ============================================================================================== //

bool 
ThreadEngine::Startup()
{
    MainThread* mt = VD_NEW(MainThread);
    Thread::GetLocal().Set(mt);
	return true;
}

bool 
ThreadEngine::Shutdown()
{
	if(Thread::GetCurrent() && Thread::GetCurrent() != Thread::GetLocal().Get())
		Thread::GetCurrent()->Destroy();
    Thread::GetLocal().Destroy();
	return true;
}

// ============================================================================================== //

#if defined(VD_TARGET_WINDOWS)

// ============================================================================================== //

#define MS_VC_EXCEPTION 0x406D1388

namespace {
	typedef struct _THREADNAME_INFO
	{
	   DWORD 	dwType; 	// Must be 0x1000.
	   LPCSTR 	szName; 	// Pointer to name (in user addr space).
	   DWORD 	dwThreadID; // Thread ID (-1=caller thread).
	   DWORD 	dwFlags; 	// Reserved for future use, must be zero.
	} THREADNAME_INFO;
}

static void 
WinSetThreadName( DWORD dwThreadID, LPCSTR szThreadName)
{
   THREADNAME_INFO info;
   info.dwType = 0x1000;
   info.szName = szThreadName;
   info.dwThreadID = dwThreadID;
   info.dwFlags = 0;

   __try
   {
      RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
   }
   __except(EXCEPTION_CONTINUE_EXECUTION)
   {
   }
}

// ============================================================================================== //

#endif

// ============================================================================================== //

Thread::Thread() :
	Object(),
	m_Handle(NULL)
{

}

Thread::Thread(
    const char* name,
    bool critical,
    vd::i32 affinity,
    vd::bytesize stack_size
) :     
    Object(),
    m_Handle(NULL)
{
    Setup(name, critical, affinity, stack_size);
}

Thread::~Thread()
{
	Destroy();
}

LocalHandle<Thread>&
Thread::GetLocal()
{
    static LocalHandle<Thread> self;
    return self;
}

vd::status
Thread::Setup(
	const char* name,
	bool critical,
	vd::i32 affinity,
	vd::bytesize stack_size)
{
    vdAssert(m_Handle == NULL);	
    ThreadCore* core = VD_NEW(ThreadCore);
    core->Native = ThreadCore::Invalid;
	core->Name = (name != NULL) ? strdup(name) : NULL;
    core->Affinity = affinity;
    core->StackSize = stack_size;
    core->IsAlive = false;
    core->IsRunning = false;
    core->IsCritical = critical;
    m_Handle = reinterpret_cast<void*>(core);
    return Status::Code::Success;
}

vd::status 
Thread::Destroy()
{
//    vdAssert(m_Handle != NULL); 

    if(m_Handle == NULL)
        return Status::Code::Reject;
    
    Exit();

    ThreadCore* core = reinterpret_cast<ThreadCore*>(m_Handle); 
    free(core->Name);   
    VD_SAFE_DELETE(core);
    m_Handle = NULL;
    return Status::Code::Success;
}

// ============================================================================================== //

vd::status 
Thread::Start()
{
	vdAssert(m_Handle != NULL);
    ThreadCore* core = reinterpret_cast<ThreadCore*>(m_Handle);
	
    if(core->IsAlive || core->Native != ThreadCore::Invalid) 
    	vdGlobalException("alive", "Failed to start thread -- already alive!");
	
#if defined(VD_TARGET_WINDOWS)

    ::DWORD id;
    core->Native = ::_beginthreadex(NULL, core->StackSize, (LPTHREAD_START_ROUTINE)&Thread::Launch, this, 0, &id);
    if(core->Native == ThreadCore::Invalid) 
    	vdGlobalException("beginthreadex", "Failed to create thread!");

	if(core->Name != NULL)
		WinSetThreadName(core->Name);
    
    if(core->Affinity >= 0) 
    {
		int group = core->Affinity / 64;
		int thread = core->Affinity % 64;
#if (_WIN32_WINNT >= 0x0601)
		GROUP_AFFINITY aff;
		aff.Group = (WORD)group;
		aff.Mask = (KAFFINITY)(1L << thread);
		SetThreadGroupAffinity(core->Native, &aff, NULL);
#endif
		SetThreadAffinityMask(core->Native, DWORD_PTR(1L << thread));
	}	
	
#elif defined(VD_USE_POSIX)

    pthread_attr_t attr;
    pthread_attr_init(&attr);

 	if(core->StackSize > 0) 
    	pthread_attr_setstacksize(&attr, core->StackSize);
    	
    if(::pthread_create(&core->Native, NULL, &Thread::Launch, this) != 0)
        vdGlobalException("pthread_create", "Failed to create thread!");

#endif

	if(core->Affinity >= 0)
		Thread::SetAffinity(core->Affinity);

    core->IsAlive = true;

	Retain();
    return Status::Code::Success;
}

// ============================================================================================== //

vd::status 
Thread::Join()
{
    vdAssert(m_Handle != NULL);
    ThreadCore* core = reinterpret_cast<ThreadCore*>(m_Handle);
	core->Mutex.Lock();
    if(core->IsAlive == false) 
    {
		core->Mutex.Unlock();
    	return Status::Code::Reject;
	}
	core->Mutex.Unlock();
	
#if defined(VD_TARGET_WINDOWS)

    if(core->Native == ThreadCore::Invalid ) 
    	vdGlobalException("not alive", "Failed to join threads -- not alive!");

    if(::WaitForSingleObject(core->Native, INFINITE) == WAIT_FAILED)
        vdGlobalException("WaitForSingleObject", "Failed to wait for single thread!");

    ::CloseHandle(core->Native);

#elif defined(VD_USE_POSIX)

    int ptstatus = ::pthread_join(core->Native, NULL);
    switch(ptstatus)
    {
        case EINVAL: 
        	vdLogWarning("Thread failed to Join!");
			break;
        case ESRCH:
        	vdLogWarning("Thread does not exist and cannot Join!");
            break;
        case EDEADLK: 
        	vdLogError("Thread failed to Join -- deadlock detected!");
			break;
        default: 
        	break;
    }
  
#endif

    core->Native = ThreadCore::Invalid;
    core->IsAlive = false;
    core->IsRunning = false;
    return Status::Code::Success;
}

// ============================================================================================== //

vd::status 
Thread::Detach()
{
	vdAssert(m_Handle != NULL);

    ThreadCore* core = reinterpret_cast<ThreadCore*>(m_Handle);
    if(core->IsAlive == false)
    	vdGlobalException("not alive", "Failed to detach thread -- not alive!");

#if defined(VD_TARGET_WINDOWS)
	
	// NOP 
	
#elif defined(VD_USE_POSIX)
    
    core->IsAlive = false;
    if(::pthread_detach(core->Native) != 0) 
    	vdGlobalException("pthread_detach", "Failed to detach thread!");

#endif

    return Status::Code::Success;
}

// ============================================================================================== //

vd::status 
Thread::Exit()
{
    vdAssert(m_Handle != NULL);
    ThreadCore* core = reinterpret_cast<ThreadCore*>(m_Handle);
    core->Mutex.Lock();
    if(core->IsAlive == false) 
    {
        core->Mutex.Unlock();
        return Status::Code::Reject;
    }
    core->Mutex.Unlock();

    vdLogDebug("Thread '%s' has finished", (core->Name != NULL) ? core->Name : "<UNKNOWN>");

    core->IsAlive = false;
    core->IsRunning = false;

    Release();
    
    GetLocal().Set(NULL);

#if defined(VD_TARGET_WINDOWS)
    
    ::ExitThread(0);

#elif defined(VD_USE_POSIX)
    
    ::pthread_exit(NULL);

#endif
    return Status::Code::Success;
}

// ============================================================================================== //

vd::status 
Thread::Yield()
{
#if defined(VD_TARGET_WINDOWS)
    
    ::Sleep(0);

#elif defined(VD_USE_POSIX)
    
    ::sched_yield();

#endif
    return Status::Code::Success;
}

// ============================================================================================== //

vd::status 
Thread::Nap()
{
#if defined(VD_TARGET_WINDOWS)
    
    ::Sleep(VD_THREAD_NAP_TIME_IN_MILLISEC);

#elif defined(VD_USE_POSIX)
    
    struct ::timespec req;
    req.tv_sec = 0;
    req.tv_nsec = VD_THREAD_NAP_TIME_IN_MILLISEC * 1000 * 1000;
    ::nanosleep(&req, NULL);

#endif
    return Status::Code::Success;
}

// ============================================================================================== //

bool
Thread::SetAffinity(vd::i32 affinity)
{
#if  defined(VD_TARGET_WINDOWS)

    if(affinity >= 0) 
    {
    	SetThreadAffinityMask(GetCurrentThread(), DWORD_PTR(1L << affinity));
		return true;
	}
	
#elif defined(VD_TARGET_OSX)

    if(affinity >= 0)
    {
        thread_affinity_policy ap;
        ap.affinity_tag = affinity;

        if(thread_policy_set(mach_thread_self(), THREAD_AFFINITY_POLICY, 
        					(integer_t*)&ap, THREAD_AFFINITY_POLICY_COUNT) != KERN_SUCCESS)
        {
            vdLogGlobalWarning("Thread: cannot set affinity!\n");
            return false;
		}
        return true;
    }

#elif defined(VD_USE_POSIX)

    vd::i32 wrap = Process::GetProcessorCount() / 2;
    affinity = (affinity / 2) + wrap * (affinity % 2);

    if(affinity >= 0 && affinity < 64 * 64)
    {
        vd::u64 mask[64] = {0};
        mask[affinity / 64] = vd::u64(1) << (affinity % 64);
        if(pthread_setaffinity_np(pthread_self(), sizeof(mask), (cpu_set_t*)mask) < 0)
        {
            vdLogGlobalWarning("Thread: cannot set affinity!\n");
            return false;
        }
        return true;
    }
    
#endif

	return false;	
}

// ============================================================================================== //

bool 
Thread::Sleep(double sec)
{
    vdGlobalAssert(sec >= 0.0);

    if(sec <= 0.0)
    {
        Yield();
        return true;
    }

    if(sec > VD_I32_MAX) 
    	sec = VD_I32_MAX;

#if defined(VD_TARGET_WINDOWS)

    ::Sleep(sec * 1000);
    return true;

#elif defined(VD_USE_POSIX)

    double integ, fract;
    fract = std::modf(sec, &integ);
    struct ::timespec req, rem;
    req.tv_sec = (time_t)integ;
    req.tv_nsec = (long)(fract * 999999000);

    while(::nanosleep(&req, &rem) != 0)
    {
        if(errno != EINTR) 
        	return false;

        req = rem;
    }

    return true;
    
#endif
}

// ============================================================================================== //

bool
Thread::IsCritical()
{
    vdAssert(m_Handle != NULL);
    ThreadCore* core = reinterpret_cast<ThreadCore*>(m_Handle);
    return core->IsCritical;
}

// ============================================================================================== //

bool
Thread::IsAlive()
{
    vdAssert(m_Handle != NULL);
    ThreadCore* core = reinterpret_cast<ThreadCore*>(m_Handle);
    return core->IsAlive;
}

// ============================================================================================== //

bool
Thread::IsRunning()
{
    vdAssert(m_Handle != NULL);
    ThreadCore* core = reinterpret_cast<ThreadCore*>(m_Handle);
    return core->IsRunning;
}

// ============================================================================================== //

Handle<LogContext>
Thread::GetLogContext()
{
    if(m_Handle == NULL)
        return NULL;

    vdAssert(m_Handle != NULL);
    ThreadCore* core = reinterpret_cast<ThreadCore*>(m_Handle);
    return core->LogCtx;
}

// ============================================================================================== //

void
Thread::SetLogContext(LogContext* context)
{
    vdAssert(m_Handle != NULL);
    ThreadCore* core = reinterpret_cast<ThreadCore*>(m_Handle);
    core->LogCtx = context;
}

// ============================================================================================== //

const char*
Thread::GetName() const
{
    vdAssert(m_Handle != NULL);
    const ThreadCore* core = reinterpret_cast<const ThreadCore*>(m_Handle);
	return ((core->Name != NULL) ? core->Name : "<NULL>");
}

// ============================================================================================== //

vd::uid Thread::GetId()
{
    
	vd::i64 value = 0;
	
#if defined(VD_TARGET_WINDOWS)

    value = ::GetCurrentThreadId();
	return vd::uid(value);
	
#elif defined(VD_TARGET_OSX)

    value = ::mach_thread_self();
    return vd::uid(value);
	
#elif defined(VD_USE_POSIX)

    pthread_t tid = pthread_self();
    if(sizeof(tid) == sizeof(value))
    {
        std::memcpy(&value, &tid, sizeof(value));
    }
    else if(sizeof(tid) == sizeof(vd::i32))
    {
        vd::i32 inum;
        std::memcpy(&inum, &tid, sizeof(inum));
        value = inum;
    }
    else
    {
        char* bytes = reinterpret_cast<char*>(&tid);
        value = Core::Hashing::Murmur(bytes, sizeof(tid));
    }
    return vd::uid(value & VD_I64_MAX);
#else
	#error "Thread::Hash() needs to be implemented on this platform!"
#endif
	
	return vd::uid(value);
	
}

// ============================================================================================== //

#if defined(VD_TARGET_WINDOWS)

::DWORD Thread::Launch(::LPVOID arg)
{    
    Thread* thread = reinterpret_cast<Thread>(arg);
    ThreadCore* core = reinterpret_cast<ThreadCore*>(thread->m_Handle);
    Thread::GetLocal().Set(thread);
	Thread* parent = Thread::GetCurrent();		

	if(!core->LogCtx && parent && parent->GetLogContext())
	    core->LogCtx = parent->GetLogContext();

    try
    {
    	core->IsRunning = true;
    	thread->Run();
    }
    catch(std::exception& e)
    {
        LogLevel::Value warn = thread->GetLogContext()->GetErrorLevel() == LogLevel::Error
                                 ? LogLevel::Warning : LogLevel::Info;
                                 
        vdLog(warn, "Fatal Error: Uncaught exception: \"%s\"", e.what());

        if(thread->IsCritical())
            _exit(-1);
    }
    catch(...)
    {
        LogLevel::Value warn = thread->GetLogContext()->GetErrorLevel() == LogLevel::Error
                                 ? LogLevel::Warning : LogLevel::Info;
                                 
        vdLog(warn, "Fatal Error: Uncaught exception <UNKNOWN>");

        if(thread->IsCritical())
            _exit(-1);
    }

    thread->Join();
    return NULL;
}

#else

void* Thread::Launch(void* arg)
{    
    Thread* thread = reinterpret_cast<Thread*>(arg);
    ThreadCore* core = reinterpret_cast<ThreadCore*>(thread->m_Handle);
    Thread::GetLocal().Set(thread);
	Thread* parent = Thread::GetCurrent();		

	if(!core->LogCtx && parent && parent->GetLogContext())
	    core->LogCtx = parent->GetLogContext();

	if(core->Affinity >= 0)
	    Thread::SetAffinity(core->Affinity);

#if defined(VD_TARGET_OSX)

    if (core->Name != NULL)
        pthread_setname_np(core->Name);

#elif defined(VD_TARGET_LINUX)

    if (core->Name != NULL)
		prctl(PR_SET_NAME, core->Name, 0, 0, 0);
#endif

    try
    {
    	core->IsRunning = true;
    	thread->Run();
    }
    catch(std::exception& e)
    {
        LogLevel::Value warn = thread->GetLogContext()->GetErrorLevel() == LogLevel::Error
                                 ? LogLevel::Warning : LogLevel::Info;
                                 
        vdLogGlobal(warn, "Fatal Error: Uncaught exception: \"%s\"", e.what());

        if(thread->IsCritical())
            _exit(-1);
    }
    catch(...)
    {
        LogLevel::Value warn = thread->GetLogContext()->GetErrorLevel() == LogLevel::Error
                                 ? LogLevel::Warning : LogLevel::Info;
                                 
        vdLogGlobal(warn, "Fatal Error: Uncaught exception <UNKNOWN>");

        if(thread->IsCritical())
            _exit(-1);
    }

    thread->Join();
    return NULL;
}
#endif

// ============================================================================================== //

VD_IMPLEMENT_ABSTRACT_OBJECT(Thread, vd_sym(Thread), vd_sym(Object));
VD_IMPLEMENT_OBJECT(MainThread, vd_sym(MainThread), vd_sym(Thread));

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

