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

#include "core/logging.h"
#include "core/symbol.h"
#include "core/threading.h"
#include "core/locking.h"
#include "core/asserts.h"
#include "core/text.h"

#include "constants/constants.h"

#include <stdarg.h>
#include <fstream>
#include <iostream>

#if defined(VD_TARGET_WINDOWS)
#include <io.h>
#endif

#if defined(VD_TARGET_OSX)
#include <sys/sysctl.h>
#endif

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

static Handle<LogContext> DefaultLogContext;

// ============================================================================================== //

void 
LogEngine::Startup()
{
    Handle<LogContext> context = GetDefaultLogContext();
    if(Thread::GetCurrent())
        Thread::GetCurrent()->SetLogContext(context);
}

void 
LogEngine::Shutdown()
{
    if(Thread::GetCurrent())
        Thread::GetCurrent()->SetLogContext(NULL);
    
    DefaultLogContext->Release();		
}

// ============================================================================================== //
    
Handle<LogContext>
LogEngine::GetDefaultLogContext(void)
{
    if(!DefaultLogContext)
    {	
    	DefaultLogContext = VD_NEW(LogContext, LogLevel::Info);
		Handle<LogChannel> channel = VD_NEW(StreamLog, &std::cout);
		Handle<LogFormat> format = VD_NEW(DefaultLogFormat);
		DefaultLogContext->AddChannel(channel);
		DefaultLogContext->SetFormat(format);	
		DefaultLogContext->Retain();		
	}	
	return DefaultLogContext;
}

Handle<LogContext>
LogEngine::GetCurrentLogContext(void)
{
	Thread* self = Thread::GetCurrent();
	if(self && self->GetLogContext())
		return self->GetLogContext();
	return GetDefaultLogContext();
}

// ============================================================================================== //

LogContext::LogContext(
	LogLevel::Value level)
: 
	m_LogLevel(level), 
	m_ErrorLevel(LogLevel::Error), 
	m_WarningCount(0)
{
    m_Mutex = VD_NEW(Mutex);
}

LogContext::~LogContext()
{
    for(size_t i = 0; i < m_Channels.size(); ++i)
        m_Channels[i]->Release();
}

void 
LogContext::SetFormat(
	LogFormat* formatter)
{
    m_Mutex->Lock();
    m_LogFormat = formatter;
    m_Mutex->Unlock();
}

void 
LogContext::SetLogLevel(
	LogLevel::Value level)
{
    m_LogLevel = level;
}

void 
LogContext::SetErrorLevel(
	LogLevel::Value level)
{
    vdGlobalAssert(m_ErrorLevel <= LogLevel::Error);
    m_ErrorLevel = level;
}

void 
LogContext::Log(
	LogLevel::Value level, 
	const MetaClass* metaclass,
    const char* file, int line, 
    const char* fmt, ...)
{
    if(level < m_LogLevel)
        return;

    char tmp[512] = {0};
    char *msg = tmp;
    va_list iterator;

#if defined(VD_TARGET_WINDOWS)

    va_start(iterator, fmt);
    size_t size = _vscprintf(fmt, iterator) + 1;

    if(size >= sizeof(tmp))
    {
        msg = VD_NEW_ARRAY(char, size + 1);
        Memory::MemSet(msg, 0, size);
    }
    vsnprintf_s(msg, size, size - 1, fmt, iterator);
    va_end(iterator);

#else

    va_start(iterator, fmt);
    size_t size = vsnprintf(tmp, sizeof(tmp), fmt, iterator);
    va_end(iterator);

    if(size >= sizeof(tmp))
    {
        // Temp buffer overflow! -- dynamically allocate memory
        msg = VD_NEW_ARRAY(char, size + 1);
        Memory::MemSet(msg, 0, size);
        va_start(iterator, fmt);
        vsnprintf(msg, size + 1, fmt, iterator);
        va_end(iterator);
    }

#endif

    if(m_LogFormat == NULL)
    {
        std::cerr << "PANIC: Logging has not been properly initialized!" << std::endl;
        exit(-1);
    }

    vd::string text = m_LogFormat->Format(level, metaclass, Thread::GetCurrent(), msg, file, line);

    if(msg != tmp)
        VD_DELETE_ARRAY(msg);

    if(level < m_ErrorLevel)
    {
        m_Mutex->Lock();

        if(level >= LogLevel::Warning)
            m_WarningCount++;

        for(size_t i = 0; i < m_Channels.size(); ++i)
            m_Channels[i]->Log(level, text);

        m_Mutex->Unlock();
    }
    else
    {
    
        // Catch any critical errors (and trap if we're running under a debugger)
#if !defined(VD_USE_EXCEPTIONS)
        fprintf(stdout, "%s\n", text.c_str());
        fflush(stdout);
#endif
        fprintf(stdout, "PANIC: Critical error! Halting execution!\n");
        fflush(stdout);

#if defined(VD_TARGET_LINUX)

        char exe_path[VD_MAX_PATH];
        pid_t ppid = getppid();
        Memory::MemSet(exe_path, 0, sizeof(exe_path));

        vd::string pid_name = Text::Format("/proc/%i/exe", ppid);
        if(readlink(pid_name.c_str(), exe_path, sizeof(exe_path)) != -1)
        {
            if(!strcmp(exe_path, "/usr/bin/gdb"))
            {
                __asm__("int $3");
            }
        }

#elif defined(VD_TARGET_OSX)

        int                 mib[4];
        struct kinfo_proc   info;
        size_t              size;
        info.kp_proc.p_flag = 0;
        mib[0] = CTL_KERN;
        mib[1] = KERN_PROC;
        mib[2] = KERN_PROC_PID;
        mib[3] = getpid();
        size = sizeof(info);
        sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, NULL, 0);
        bool debugging = (info.kp_proc.p_flag & P_TRACED) != 0;

        if(debugging)
            __asm__("int $3");

#elif defined(VD_TARGET_WINDOWS)

        if(IsDebuggerPresent())
            __debugbreak();

#endif

#if defined(VD_USE_EXCEPTIONS)
        throw std::runtime_error(text);
#else
        exit(-1);
#endif
    }
}

void 
LogContext::LogProgress(
	vd::real progress, 
	const vd::string& name,
    const vd::string& formatted, 
    const vd::string& eta, 
    const void* ptr)
{
    m_Mutex->Lock();

    for(size_t i = 0; i < m_Channels.size(); ++i)
        m_Channels[i]->LogProgress(progress, name, formatted, eta, ptr);

    m_Mutex->Unlock();
}

void 
LogContext::AddChannel(LogChannel* channel)
{
    channel->Retain();
    m_Mutex->Lock();
    m_Channels.push_back(channel);
    m_Mutex->Unlock();
}

void 
LogContext::RemoveChannel(LogChannel* channel)
{
    m_Mutex->Lock();
    m_Channels.erase(std::remove(m_Channels.begin(), m_Channels.end(), channel), m_Channels.end());
    m_Mutex->Unlock();
    channel->Release();
}

void 
LogContext::ClearChannels()
{
    m_Mutex->Lock();

    for(size_t i = 0; i < m_Channels.size(); ++i)
        m_Channels[i]->Release();

    m_Channels.clear();
    m_Mutex->Unlock();
}

// ============================================================================================== //

DefaultLogFormat::DefaultLogFormat() :
	m_ShowDate(true), 
	m_ShowLogLevel(true), 
	m_ShowThread(true), 
	m_ShowClass(true)
{
	// EMPTY!
}

vd::string 
DefaultLogFormat::Format(
	LogLevel::Value  level, 
	const MetaClass* metaclass,
	const Thread* thread, 
	const vd::string& text, 
	const char* file, int line)
{
    std::ostringstream oss;
    if(m_ShowDate)
    {
    	char buffer[256] = {0};
        time_t timestamp = ::time(NULL);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S ", ::localtime(&timestamp));
        oss << buffer;
    }

    if(m_ShowLogLevel)
    {
        switch(level)
        {
            case LogLevel::Trace: 		oss << "[TRACE]   "; break;
            case LogLevel::Debug: 		oss << "[DEBUG]   "; break;
            case LogLevel::Info:  		oss << "[INFO]    "; break;
            case LogLevel::Warning:  	oss << "[WARNING] "; break;
            case LogLevel::Error: 		oss << "[ERROR]	  "; break;
            default:     				oss << "[USER]    "; break;
        }
    }

    if(m_ShowThread && thread && thread->GetName() != NULL)
    {
    	vd::string name = thread->GetName();
        oss << name;

        for(int i = 0; i < (5 - (int) name.size()); i++)
            oss << ' ';
    }

    if(m_ShowClass)
    {
        if(metaclass)
            oss << " [" << metaclass->GetIdentifier().ToString() << "] ";
        else if(line != -1 && file)
            oss << " [" << file << ":" << line << "] ";
//            oss << "[" << fs::path(file).filename().string() << ":" << line << "] ";
    }

    oss << text;

    return oss.str();
}

// ============================================================================================== //

StreamLog::StreamLog(
	std::ostream* stream)
: 
	m_Stream(stream), 
	m_IsFile(false)
{
    m_Flushed = false;
}

StreamLog::StreamLog(
	const vd::string& filename)
: 
	m_FileName(filename), 
	m_IsFile(true)
{
    m_Stream = new std::ofstream();
    ((std::ofstream*) m_Stream)->open(filename.c_str());
    m_Flushed = false;
}

void 
StreamLog::Log(
	LogLevel::Value level, 
	const vd::string& text)
{
    if(m_Flushed && !m_IsFile)
        std::cout << std::endl;

    (*m_Stream) << text << std::endl;
    m_Flushed = false;
}

void 
StreamLog::LogProgress(
	vd::real progress, 
	const vd::string& name,
	const vd::string& formatted, 
	const vd::string& eta, 
	const void* ptr)
{
    if(!m_IsFile)
    {
        std::cout << formatted;
        std::cout.flush();
    }

    m_Flushed = true;
}

vd::string 
StreamLog::ToString() const
{
    std::ostringstream oss;

    oss << "StreamLog[stream=";

    if(m_IsFile)
    {
        oss << "\"" << m_FileName << "\"";
    }
    else
    {
        oss << "<std::ostream>";
    }

    oss << "]";

    return oss.str();
}

StreamLog::~StreamLog()
{
    if(m_IsFile)
    {
        ((std::ofstream*) m_Stream)->close();
        delete m_Stream;
    }
}

// ============================================================================================== //

RawFileLog::RawFileLog(
	int fd)
: 
	m_Fd(fd)
{
    m_Flushed = false;
}

void 
RawFileLog::Log(
	LogLevel::Value level, 
	const vd::string& text)
{
    vd::string value = text + vd::string("\n");
    
#if defined(VD_TARGET_WINDOWS)

    write(m_Fd, value.c_str(), (unsigned int) value.length());

#else

    if(write(m_Fd, value.c_str(), value.length()) != (ssize_t) value.length())
        vdLogError("Log message truncated! Write incomplete!");
#endif

}

void 
RawFileLog::LogProgress(
	vd::real progress, 
	const vd::string& name,
    const vd::string& formatted, 
    const vd::string& eta, 
    const void* ptr)
{
    // NOP
}

vd::string 
RawFileLog::ToString() const
{
    return "RawFileLog[]";
}

RawFileLog::~RawFileLog()
{
    close(m_Fd);
}

// ============================================================================================== //

VD_IMPLEMENT_ABSTRACT_OBJECT(LogFormat, vd_sym(LogFormat), vd_sym(Object));
VD_IMPLEMENT_ABSTRACT_OBJECT(LogChannel, vd_sym(LogChannel), vd_sym(Object));

VD_IMPLEMENT_OBJECT(LogContext, vd_sym(LogContext), vd_sym(Object));
VD_IMPLEMENT_OBJECT(StreamLog, vd_sym(StreamLog), vd_sym(LogChannel));
VD_IMPLEMENT_OBJECT(RawFileLog, vd_sym(RawFileLog), vd_sym(LogChannel));
VD_IMPLEMENT_OBJECT(DefaultLogFormat, vd_sym(DefaultLogFormat), vd_sym(LogFormat));

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //
