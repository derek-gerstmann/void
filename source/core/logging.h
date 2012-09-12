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

#ifndef VD_CORE_LOGGING_INCLUDED
#define VD_CORE_LOGGING_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/threading.h"
#include "core/locking.h"
#include "core/handles.h"
#include "containers/containers.h"

// ============================================================================================== //
        
VD_USING(Containers, Vector);

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_DECLARE_ENUM(LogLevel,
	Trace,
	Debug,
	Info,
	Warning,
	Error);

// ============================================================================================== //

class LogContext;

class LogEngine
{
public:
    static void Startup(int* argc=NULL, char** argv=NULL);
    static void Shutdown();

    static Handle<LogContext> GetDefaultContext();
    static Handle<LogContext> GetCurrentContext();
};

// ============================================================================================== //
	
class LogFormat : public Object
{
	friend class LogEngine;
	
    VD_DECLARE_OBJECT(LogFormat);

public:

    virtual vd::string Format(
    	LogLevel::Value level, 
    	const MetaClass* metaclass,
        const Thread* thread, 
        const vd::string& text,
        const char* file, int line) = 0;


protected:
    virtual ~LogFormat() { }

};

// ============================================================================================== //

class DefaultLogFormat : public LogFormat
{
	friend class LogEngine;

public:

    DefaultLogFormat();

    virtual vd::string Format(
		LogLevel::Value level, 
    	const MetaClass* metaclass,
        const Thread* thread, 
        const vd::string& text,
        const char* file, int line);

    inline void SetShowDate(bool enable) { m_ShowDate = enable; }
    inline void SetShowThread(bool enable) { m_ShowThread = enable; }
    inline void SetShowLogLevel(bool enable) { m_ShowLogLevel = enable; }
    inline void SetShowClass(bool enable) { m_ShowClass = enable; }

    VD_DECLARE_OBJECT(DefaultLogFormat);

protected:
    virtual ~DefaultLogFormat() { }

protected:
    bool m_ShowDate;
    bool m_ShowLogLevel;
    bool m_ShowThread;
    bool m_ShowClass;
};

// ============================================================================================== //

class LogChannel : public Object
{
	friend class LogEngine;

public:

    virtual void Log(
    	LogLevel::Value level,
    	const vd::string& text) = 0;

    virtual void LogProgress(
    	vd::real progress, 
    	const vd::string& name,
		const vd::string& formatted, 
		const vd::string& eta,
		const void* ptr) = 0;

    VD_DECLARE_OBJECT(LogChannel);

protected:
    virtual ~LogChannel() { }
};

// ============================================================================================== //

class StreamLog : public LogChannel
{
	friend class LogEngine;

public:
    StreamLog(std::ostream* pStream);
    StreamLog(const vd::string& pFilename);

    void Log(
        LogLevel::Value level, 
        const vd::string& text);

    void LogProgress(
    	vd::real progress, 
    	const vd::string& name,
		const vd::string& formatted, 
		const vd::string& eta,
		const void* ptr);

    inline bool IsFileLog() const { return m_IsFile; }
    vd::string ToString() const;

    VD_DECLARE_OBJECT(StreamLog);

protected:
    virtual ~StreamLog();

private:
    std::ostream* m_Stream;
    vd::string m_FileName;
    bool m_IsFile;
    bool m_Flushed;
};

// ============================================================================================== //

class RawFileLog : public LogChannel
{
	friend class LogEngine;

public:

    RawFileLog(int fd);
    
    void Log(
         LogLevel::Value level, 
         const vd::string& text);
    
    void LogProgress(
    	vd::real progress, 
    	const vd::string& name,
        const vd::string& formatted, 
        const vd::string& eta,
        const void* ptr);

    vd::string ToString() const;

    VD_DECLARE_OBJECT(RawFileLog);

protected:
    virtual ~RawFileLog();

private:
    int m_Fd;
    bool m_Flushed;
};

// ============================================================================================== //

class LogContext : public Object
{
	friend class LogEngine;

public:
    typedef Vector<LogChannel*>::type ChannelTable;

    LogContext(LogLevel::Value logLevel = LogLevel::Debug);

    void Log(LogLevel::Value level, 
    		 const MetaClass* metaclass,
             const char* fileName, 
             int lineNumber,
             const char* fmt, ...);

    void LogProgress(vd::real progress, 
    		const vd::string& name,
            const vd::string& formatted, 
            const vd::string& eta,
			const void* ptr);

    void SetLogLevel(LogLevel::Value level);
    void SetErrorLevel(LogLevel::Value level);

    inline LogLevel::Value GetLogLevel() const { return m_LogLevel; }
    inline LogLevel::Value GetErrorLevel() const { return m_ErrorLevel; }

    void AddChannel(LogChannel* channel);
    void RemoveChannel(LogChannel* channel);
    void ClearChannels();
    
    inline size_t GetChannelCount() const { return m_Channels.size(); }
    inline LogChannel* GetChannel(size_t index) { return m_Channels[index]; }
    inline const LogChannel* GetLogChannel(size_t index) const { return m_Channels[index]; }

    void SetFormat(LogFormat* format);
    inline LogFormat* GetFormat() { return m_LogFormat; }

    inline size_t GetWarningCount() const { return m_WarningCount; }

    static void Startup();
    static void Shutdown();

    VD_DECLARE_OBJECT(LogContext);

protected:
    
    virtual ~LogContext();

    VD_DISABLE_COPY_CONSTRUCTORS(LogContext);
    
private:

    LogLevel::Value m_LogLevel;
    LogLevel::Value m_ErrorLevel;
    Handle<LogFormat> m_LogFormat;
    Handle<Mutex> m_Mutex;
    ChannelTable m_Channels;
    size_t m_WarningCount;
};

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

// ============================================================================================== //
/// Logging + Assertion Macros
// ============================================================================================== //

#define vdLog(level, fmt, ...) do { \
    Core::LogEngine::GetCurrentContext()->Log( \
        level, GetMetaClass(), \
        __FILE__, __LINE__, fmt, ## __VA_ARGS__); \
    } while (0)
    
#define vdLogTrace(fmt, ...) do { \
    Core::LogEngine::GetCurrentContext()->Log( \
        Core::LogLevel::Trace, GetMetaClass(), \
        __FILE__, __LINE__, fmt, ## __VA_ARGS__); \
    } while (0)
    
#define vdLogDebug(fmt, ...) do { \
    Core::LogEngine::GetCurrentContext()->Log( \
        Core::LogLevel::Debug, GetMetaClass(), \
        __FILE__, __LINE__, fmt, ## __VA_ARGS__); \
    } while (0)

#define vdLogInfo(fmt, ...) do { \
    Core::LogEngine::GetCurrentContext()->Log( \
        Core::LogLevel::Info, GetMetaClass(), \
        __FILE__, __LINE__, fmt, ## __VA_ARGS__); \
    } while (0)
    
#define vdLogWarning(fmt, ...) do { \
    Core::LogEngine::GetCurrentContext()->Log( \
        Core::LogLevel::Warning, GetMetaClass(), \
        __FILE__, __LINE__, fmt, ## __VA_ARGS__); \
    } while (0)

#define vdLogError(fmt, ...) do { \
    Core::LogEngine::GetCurrentContext()->Log( \
        Core::LogLevel::Error, GetMetaClass(), \
        __FILE__, __LINE__, fmt, ## __VA_ARGS__); \
    } while (0)

// ============================================================================================== //

#define vdLogGlobal(level, fmt, ...) do { \
    Core::LogEngine::GetCurrentContext()->Log( \
        level, NULL, \
        __FILE__, __LINE__, fmt, ## __VA_ARGS__);  \
    } while (0)

#define vdLogGlobalTrace(fmt, ...) do { \
    Core::LogEngine::GetCurrentContext()->Log( \
        Core::LogLevel::Trace, NULL, \
        __FILE__, __LINE__, fmt, ## __VA_ARGS__); \
    } while (0)

#define vdLogGlobalDebug(fmt, ...) do { \
    Core::LogEngine::GetCurrentContext()->Log( \
        Core::LogLevel::Debug, NULL, \
        __FILE__, __LINE__, fmt, ## __VA_ARGS__); \
    } while (0)

#define vdLogGlobalInfo(fmt, ...) do { \
    Core::LogEngine::GetCurrentContext()->Log( \
        Core::LogLevel::Info, NULL, \
        __FILE__, __LINE__, fmt, ## __VA_ARGS__); \
    } while (0)

#define vdLogGlobalWarning(fmt, ...) do { \
    Core::LogEngine::GetCurrentContext()->Log( \
        Core::LogLevel::Warning, NULL, \
        __FILE__, __LINE__, fmt, ## __VA_ARGS__); \
    } while (0)

#define vdLogGlobalError(fmt, ...) do { \
    Core::LogEngine::GetCurrentContext()->Log( \
        Core::LogLevel::Error, NULL, \
        __FILE__, __LINE__, fmt, ## __VA_ARGS__); \
    } while (0)

// ============================================================================================== //

#endif // VD_CORE_LOGGING_INCLUDED

// ============================================================================================== //
// END OF FILE

