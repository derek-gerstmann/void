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

#include "vd.h"
#include "core/core.h"
#include "core/process.h"
#include "core/symbol.h"
#include "core/asserts.h"
#include "core/resources.h"
#include "core/logging.h"
#include "core/locking.h"
#include "core/threading.h"

// ============================================================================================== //

#if defined(VD_TARGET_OSX)

    #include <time.h>
    #include <sys/sysctl.h>
    #include <sys/time.h>
    #include <mach/mach_time.h>

#elif defined(VD_TARGET_LINUX)
    
    #include <time.h>
    #include <unistd.h>
    #include <sys/sysctl.h>
    #include <sys/time.h>

#elif defined(VD_TARGET_WINDOWS)

    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #if (_MSC_VER >= 1400)
        #include <stdio.h>
        #define snprintf _snprintf
    #endif
    
#else
    #error "Process methods not implemented for this platform!"
#endif

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

vd::f64 Process::m_StartupTime = 0.0;
vd::f64 Process::m_ShutdownTime = 0.0;

// ============================================================================================== //

vd::f64
Process::GetStartupTime(void)
{
    return m_StartupTime;
}

vd::f64
Process::GetShutdownTime(void)
{
    return m_ShutdownTime;
}

bool
Process::Startup(void)
{
    Process::m_StartupTime = GetTimeInSeconds();
    return true;
}

bool
Process::Shutdown(void)
{
    Process::m_ShutdownTime = GetTimeInSeconds();
    return true;
}

// ============================================================================================== //

vd::string 
Process::GetExecutableFileName(void) 
{
    char filename[4096];
    static const vd::string invalid;
    vd::u64 length = sizeof(filename) - 1;
    
#if defined(VD_TARGET_OSX)

    vd::u32 nslength = length;
    if (_NSGetExecutablePath(filename, &nslength) != 0) 
        return invalid;

#elif defined(VD_TARGET_LINUX)

    char pid[128]; 
    snprintf(pid, sizeof(pid)-1, "/proc/%d/exe", getpid());
    int bytes = readlink(pid, filename, length);
    if(bytes <= 0)
        return invalid;
    filename[bytes] = '\0';

#elif defined(VD_TARGET_WINDOWS)

    if (!GetModuleFileName(NULL, filename, length)) 
        return invalid;

#else

    #error "GetExecutableFileName() needs to be implemented for this platform!"

#endif

    vdLogGlobalDebug("Process: Executable is '%s'\n", filename);
    return vd::string(filename);
}

vd::bytesize 
Process::GetPhysicalMemorySize(void)
{
    vd::bytesize bytes = 0;
    
#if defined(VD_TARGET_WINDOWS)

    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    bytes = status.ullTotalPhys;

#elif defined(VD_TARGET_OSX)

    vd::bytesize rv = sizeof(bytes);
    if (sysctlbyname("hw.physmem",&bytes,&rv,NULL,0)) 
    {
        vdGlobalException("hw.physmem", "Process: Failed to get physical memory size!\n");
        return 0;
    }

#elif defined(VD_TARGET_LINUX)

    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    return pages * page_size;
    
#else

    #error "GetPhysicalMemorySize needs to be implemented for this platform!"

#endif

    return bytes; 
}

vd::i32 
Process::GetProcessorCount(void)
{
    vd::i32 count = 1;

#if defined(VD_TARGET_OSX)

    size_t size = sizeof(count);
    if (sysctlbyname("hw.ncpu",&count,&size,NULL,0)) 
    {
        vdLogGlobalWarning("Process: Failed to get cpu core count. Assuming single core!\n");
        count = 1;
    }

#elif defined(VD_TARGET_LINUX)

    count = sysconf(_SC_NPROCESSORS_CONF);

#elif defined(VD_TARGET_WINDOWS)

    SYSTEM_INFO sysinfo;
    GetProcessInfo(&sysinfo);
    count = sysinfo.dwNumberOfProcessors;

#else

    #error "GetMaxThreadCount needs to be implemented for this platform!"

#endif

    vdLogGlobalDebug("Process: Processor Count is '%d'\n", count);
    return count; 
}

vd::i32 
Process::GetTimeResolution(void)
{
#if defined(VD_TARGET_WINDOWS)
    static const vd::i32 value = 100;
#elif defined(VD_USE_POSIX)
    static const vd::i32 value = sysconf(_SC_CLK_TCK);
#endif
    vdLogGlobalDebug("Process: Time Resolution is '%d'\n", value);
    return value;
}

#if defined(VD_TARGET_WINDOWS)
static vd::bytesize 
GetWinPageSize(void) 
{
    ::SYSTEM_INFO ibuf;
    ::GetProcessInfo(&ibuf);
    return ibuf.dwPageSize;
}
#endif

vd::bytesize
Process::GetPageSize(void)
{
#if defined(VD_TARGET_WINDOWS)
    static const vd::bytesize value = GetWinPageSize();
#else
    static const vd::bytesize value = sysconf(_SC_PAGESIZE);
#endif
    vdLogGlobalDebug("Process: Page Size is '%d'\n", (int)value);
    return value;
}

vd::f64 
Process::GetTimeScaleFactor(void)
{
    static vd::f64 scale = 0.0;
    if(scale > 0.0)
        return scale;
    
#if defined(VD_TARGET_OSX)
    
    mach_timebase_info_data_t base;
    mach_timebase_info(&(base));
    scale = 1e-9 * (vd::f64) base.numer / (vd::f64)base.denom;
    
#elif defined(VD_TARGET_LINUX)
    
    scale = 1.0 / 1000000.0;
    
#elif defined(VD_TARGET_WINDOWS)
    
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency( &(frequency) );
    scale = 1.0/((vd::f64)frequency.QuadPart);    

#else

    #error "GetTimeScaleFactor() needs to be implemented for this platform!"

#endif
    return scale;
}

vd::f64
Process::GetTimeInSeconds(void)
{
    static vd::f64 scale = 0.0f;
    if(scale == 0.0f)
        scale = GetTimeScaleFactor();
        
    vd::f64 current;

#if defined(VD_TARGET_OSX)

    uint64_t value;
    value = mach_absolute_time();
    current = value * scale;

#elif defined(VD_TARGET_LINUX)

    struct timeval value;
    gettimeofday(&value, NULL);
    current =  value.tv_sec + value.tv_usec * scale;    

#elif defined(VD_TARGET_WINDOWS)

    LARGE_INTEGER value;
    QueryPerformanceCounter( &value );
    current =  (vd::f64) value.QuadPart * scale;

#else

    #error "GetTimeInSeconds() needs to be implemented for this platform!"

#endif

    return current;
}

vd::i64
Process::GetProcessId(void)
{
#if defined(VD_TARGET_WINDOWS)
    return ::GetCurrentProcessId();
#elif defined(VD_USE_POSIX)
    return ::getpid();
#else
    #error "GetProcessId() needs to be implemented for this platform!"
#endif
}

const std::string& 
Process::GetPlatformName() 
{
#if defined(__CYGWIN__) && !defined(__X86_64__)
    static const std::string name( "Cygwin (32-bit)" );
#elif defined(__CYGWIN__) && defined(__X86_64__)
    static const std::string name( "Cygwin (64-bit)" );
#elif defined(__FREEBSD__) && !defined(__X86_64__)
    static const std::string name( "FreeBSD (32-bit)" );
#elif defined(__FREEBSD__) && defined(__X86_64__)
    static const std::string name( "FreeBSD (64-bit)" );
#elif defined(__LINUX__) && !defined(__X86_64__)
    static const std::string name( "Linux (32-bit)" );
#elif defined(__LINUX__) && defined(__X86_64__)
    static const std::string name( "Linux (64-bit)" );
#elif defined(__MACOSX__) && !defined(__X86_64__)
    static const std::string name( "Mac OSX (32-bit)" );
#elif defined(__MACOSX__) && defined(__X86_64__)
    static const std::string name( "Mac OSX (64-bit)" );
#elif defined(__UNIX__) && !defined(__X86_64__)
    static const std::string name( "Unix (32-bit)" );
#elif defined(__UNIX__) && defined(__X86_64__)
    static const std::string name( "Unix (64-bit)" );
#elif defined(__WIN32__) && !defined(__X86_64__)
    static const std::string name( "Windows (32-bit)" );
#elif defined(__WIN32__) && defined(__X86_64__)
    static const std::string name( "Windows (64-bit)" );
#else
    static const std::string name( "Unknown Platform" );
#endif

    vdLogGlobalDebug("Process: Platform is '%s'\n", name.c_str());
    return name;
}

bool 
Process::Is64bit()
{
#if defined(__X86_64__)
    return true;
#else
    return false;
#endif
}

bool 
Process::Is32bit()
{
#if defined(__X86_64__)
    return false;
#else
    return true;
#endif
}

bool
Process::IsLittleEndian()
{
    static unsigned long x(1);
    static bool little(reinterpret_cast<unsigned char*>(&x)[0] != 0);
    return little;
}
    
// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //



