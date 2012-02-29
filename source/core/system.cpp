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
#include "core/system.h"
#include "core/symbol.h"
#include "core/asserts.h"
#include "core/resources.h"
#include "core/logging.h"
#include "core/locking.h"
#include "core/threading.h"
#include "core/process.h"

// ============================================================================================== //

#if defined(VD_TARGET_OSX)

	#include <time.h>
	#include <sys/sysctl.h>
	#include <sys/time.h>
    #include <mach/mach_time.h>

#elif defined(VD_TARGET_LINUX)

	#include <time.h>
	#include <sys/time.h>

#elif defined(VD_TARGET_WINDOWS)

	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#if (_MSC_VER >= 1400)
		#include <stdio.h>
		#define snprintf _snprintf
	#endif
	
#else
	#error "System methods not implemented for this platform!"
#endif

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

vd::f64 System::m_StartupTime = 0.0;
vd::f64 System::m_ShutdownTime = 0.0;

// ============================================================================================== //

vd::f64
System::GetStartupTime(void)
{
	return m_StartupTime;
}

vd::f64
System::GetShutdownTime(void)
{
	return m_ShutdownTime;
}

bool
System::Startup(void)
{
	Mutex mutex;
	mutex.Lock();
	{
		System::m_StartupTime = Process::GetTimeInSeconds();
		Symbol::CreateRegistry();
		MetaClass::CreateRegistry();
		ResourceEngine::Startup();
		ThreadEngine::Startup();
		LogEngine::Startup();
		Process::Startup();
		vdLogGlobalInfo("Void Framework v%d.%d.%d - %s", 
			VD_VERSION_MAJOR, 
			VD_VERSION_MINOR, 
			VD_VERSION_PATCH, 
			VD_BUILD_INFO);
	}
	mutex.Unlock();
	return true;
}

bool
System::Shutdown(void)
{
	Mutex mutex;
	mutex.Lock();
	{
		Process::Shutdown();
		LogEngine::Shutdown();
		ThreadEngine::Shutdown();
		ResourceEngine::Shutdown();
		Symbol::DestroyRegistry();
		MetaClass::DestroyRegistry();
		System::m_ShutdownTime = Process::GetTimeInSeconds();
	}
	mutex.Unlock();
	return true;
}
    
// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //



