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

#include "core/plugin.h"
#include "core/local.h"
#include "core/handles.h"
#include "core/logging.h"
#include "core/asserts.h"
#include "core/filesystem.h"

#if !defined(VD_TARGET_WINDOWS)
#include <dlfcn.h>
#endif

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

Plugin::Plugin(
    const std::string& basename, 
    const fs::path& path
) : 
    m_BaseName(basename), 
    m_Path(path)
{
#if defined(VD_TARGET_WINDOWS)
    m_Handle = LoadLibrary(path.native().c_str());
    if (!m_Handle)
    {
        vdLogGlobalError("Error while loading plugin '%s': %s",
             m_Path.native().c_str(), lastErrorText().c_str());
    }
#else
    m_Handle = dlopen(path.native().c_str(), RTLD_LAZY | RTLD_LOCAL);
    if (!m_Handle)
    {
        vdLogGlobalError("Error while loading plugin '%s': %s",
             m_Path.native().c_str(), dlerror());
    }
#endif
    try
    {
        m_GetDescriptionFn = (GetDescriptionFn) Resolve("GetDescription");
    }
    catch (...)
    {
#if defined(WIN32)
        FreeLibrary(m_Handle);
#else
        dlclose(m_Handle);
#endif
        throw;
    }

    m_CreateEntityFn = NULL;
    m_CreateUtilityFn = NULL;
    m_IsUtility = false;

    if (Has("CreateUtility"))
    {
        m_CreateUtilityFn = (CreateUtilityFn) Resolve("CreateUtility");
        m_IsUtility = true;
    }
    else
    {
        m_CreateEntityFn = (CreateEntityFn) Resolve("CreateEntity");
    }
//    Statistics::GetInstance()->LogPlugin(basename, GetDescription());
//    MetaClass::Startup();
}

bool 
Plugin::Has(
    const std::string &sym) const
{
#if defined(WIN32)
    void *ptr = GetProcAddress(m_Handle, sym.c_str());
#else
    void *ptr = dlsym(m_Handle, sym.c_str());
#endif
    return ptr != NULL;
}

void*
Plugin::Resolve(
    const std::string &sym)
{
#if defined(WIN32)
    void *data = GetProcAddress(m_Handle, sym.c_str());
    if (!data)
    {
        vdLogGlobalError("Could not resolve symbol '%s' in '%s': %s",
             sym.c_str(), m_Path.native().c_str(), lastErrorText().c_str());
    }
#else
    void *data = dlsym(m_Handle, sym.c_str());
    if (!data)
    {
        vdLogGlobalError("Could not resolve symbol'%s' in '%s': %s",
             sym.c_str(), m_Path.native().c_str(), dlerror());
    }
#endif
    return data;
}

Entity*
Plugin::CreateEntity(
    const Config& config) const
{
    return (Entity*) m_CreateEntityFn(config);
}

Entity*
Plugin::CreateUtility() const
{
    return (Entity*) m_CreateUtilityFn();
}

std::string 
Plugin::GetDescription() const
{
    return m_GetDescriptionFn();
}

Plugin::~Plugin()
{
#if defined(WIN32)
    FreeLibrary(m_Handle);
#else
    dlclose(m_Handle);
#endif
}

// -----------------------------------------------------------------------

Handle<Module> Module::m_Instance = NULL;

// -----------------------------------------------------------------------

Module::Module()
{
    m_Mutex = VD_NEW(Mutex);
}

Module::~Module()
{
    std::map<std::string, Plugin *>::const_iterator it;
    for (it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
    {
        VD_DELETE( (*it).second );
    }
}

Entity*
Module::CreateEntity(
    const MetaClass* meta,
    const Config& props)
{
    Entity *object;

    m_Mutex->Lock();
    try
    {
        LoadPlugin(props.GetPluginName());
        object = m_Plugins[props.GetPluginName()]->CreateEntity(props);
    }
    catch (std::runtime_error &e)
    {
        m_Mutex->Unlock();
        throw e;
    }
    catch (std::exception &e)
    {
        m_Mutex->Unlock();
        throw e;
    }
    m_Mutex->Unlock();

    if (!object->GetMetaClass()->IsDerivedFrom(meta))
    {
        vdLogError("Type mismatch when loading plugin \"%s\": Expected "
            "an instance of \"%s\"", props.GetPluginName().c_str(), meta->GetClassName().c_str());
    }

    if (object->GetMetaClass()->IsAbstract())
    {
        vdLogError("Error when loading plugin \"%s\": Identifies itself as an abstract class",
            props.GetPluginName().c_str());
    }
    return object;
}

Entity*
Module::CreateUtility(
    const Config& props)
{
    Entity *object;

    m_Mutex->Lock();
    try
    {
        LoadPlugin(props.GetPluginName());
        object = m_Plugins[props.GetPluginName()]->CreateEntity(props);
    }
    catch (std::runtime_error &e)
    {
        m_Mutex->Unlock();
        throw e;
    }
    catch (std::exception &e)
    {
        m_Mutex->Unlock();
        throw e;
    }
    m_Mutex->Unlock();
    if (object->GetMetaClass()->IsAbstract())
    {
        vdLogError("Error when loading plugin '%s': Identifies itself as an abstract class",
            props.GetPluginName().c_str());
    }
    return object;
}

std::vector<std::string> 
Module::GetLoadedPlugins() const
{
    std::vector<std::string> names;
    m_Mutex->Lock();
    std::map<std::string, Plugin *>::const_iterator it;
    for (it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
    {
        names.push_back((*it).first);
    }
    m_Mutex->Unlock();
    return names;
}

void 
Module::LoadPlugin(
    const std::string &name)
{
    if (m_Plugins[name] != NULL)
        return;

#if defined(WIN32)
    std::string basepath = std::string("plugins/") + name + std::string(".dll");
#elif defined(__OSX__)
    std::string basepath = std::string("plugins/") + name + std::string(".dylib");
#else
    std::string basepath = std::string("plugins/") + name + std::string(".so");
#endif
    //const FileResolver *resolver = Thread::GetThread()->GetFileResolver();
    //fs::path path = resolver->resolve(basepath);
    
    Core::FileSystem file_system;
    fs::path path = file_system.FindInSearchPath(basepath);
    if (fs::exists(path))
    {
        vdLogInfo("Loading plugin '%s' ..", basepath.c_str());
        m_Plugins[name] = VD_NEW(Plugin, basepath, path.native());
        return;
    }

    vdLogError("Plugin '%s' not found!", name.c_str());
}

void Module::Startup()
{
    m_Instance = VD_NEW(Module);
}

void Module::Shutdown()
{
    m_Instance = NULL;
}

// ============================================================================================== //

VD_IMPLEMENT_OBJECT(Module, vd_sym(Module), vd_sym(Object));

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //
