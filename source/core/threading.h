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

#ifndef VD_CORE_THREADING_INCLUDED
#define VD_CORE_THREADING_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/local.h"
#include "core/handles.h"
#include "core/object.h"
#include "containers/containers.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class LogContext;

// ============================================================================================== //

class ThreadEngine 
{
public:
    static bool Startup();
    static bool Shutdown();
};   

// ============================================================================================== //

VD_DECLARE_ENUM(Priority,
	Idle,
	Lowest,
	Low,
	Normal,
	High,
	Highest,
	Realtime);

// ============================================================================================== //

class Thread : public Object
{
	friend class ThreadEngine;

public:
//    explicit Thread(const char* name=NULL, bool critical=false, vd::i32 affinity=-1, vd::bytesize stack_size=0);

    explicit Thread();
    virtual vd::status Destroy();
    vd::status Setup(const char* name=NULL, bool critical=false, vd::i32 affinity=-1, vd::bytesize stack_size=0);
	vd::status Start();
    vd::status Join();
    vd::status Detach();
    vd::status Exit();
    
    bool IsCritical();
    bool IsRunning();
    bool IsAlive();

    virtual void Run() = 0;
    
    static bool SetAffinity(vd::i32 affinity);
    static vd::status Yield();
    static vd::status Nap();
    static bool Sleep(double sec);
    static vd::uid GetId();

    Handle<LogContext> GetLogContext();
    void SetLogContext(LogContext* context);
    const char* GetName() const;

	static inline Thread* GetCurrent() { return m_Self.Get(); }
    
	VD_DECLARE_OBJECT(Thread);
	
protected:
	virtual ~Thread();
	
private:

	VD_DISABLE_COPY_CONSTRUCTORS(Thread);

#if defined(VD_TARGET_WINDOWS)
	static ::DWORD Launch(::LPVOID arg);
#else
	static void* Launch(void* arg);
#endif

private:

    void* 						m_Handle;
	static LocalHandle<Thread>	m_Self;	
};

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif  // VD_CORE_THREADING_INCLUDED                     

// ============================================================================================== //

// END OF FILE
