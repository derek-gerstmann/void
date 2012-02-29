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

#ifndef VD_CORE_PROCESS_INCLUDED
#define VD_CORE_PROCESS_INCLUDED

#include "core/core.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class Process
{
public:
    static bool Startup(void);
    static bool Shutdown(void);

    static const vd::string& GetPlatformName();
    static bool Is64bit();
    static bool Is32bit();
    static bool IsLittleEndian();
    static vd::f64 GetStartupTime(void);
    static vd::f64 GetShutdownTime(void);
    static vd::string GetExecutableFileName(void); 
    static vd::bytesize GetPhysicalMemorySize(void);
    static vd::i32 GetProcessorCount(void);
    static vd::i32 GetTimeResolution(void);
    static vd::f64 GetTimeScaleFactor(void);
    static vd::f64 GetTimeInSeconds(void);
    static vd::bytesize GetPageSize(void);
    static vd::i64 GetProcessId(void);

private:
    static vd::f64 m_StartupTime;
    static vd::f64 m_ShutdownTime;
};

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_SYSTEM_INCLUDED 

