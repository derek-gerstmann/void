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

#ifndef VD_CORE_PLUGIN_INCLUDED
#define VD_CORE_PLUGIN_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/config.h"
#include "core/repository.h"
#include "core/entity.h"
#include "core/handles.h"
#include "core/locking.h"

#include <boost/filesystem.hpp>

// ============================================================================================== //

namespace fs = boost::filesystem;

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class VD_API Plugin
{

public:

    typedef void *(*CreateEntityFn)(const Config& config);
    typedef void *(*CreateUtilityFn)();
    typedef char *(*GetDescriptionFn)();

public:

    Plugin(const vd::string& basename, const fs::path &path);
    virtual ~Plugin();
    inline bool IsUtility() const { return m_IsUtility; }
    Entity* CreateEntity(const Config& config) const;
    Entity* CreateUtility() const;
    vd::string GetDescription() const;
    inline const fs::path& GetPath() const { return m_Path; }
    inline const vd::string& GetBaseName() const { return m_BaseName; }

protected:

    void* Resolve(const vd::string &sym);
    bool  Has(const vd::string &sym) const;

private:
#if defined(WIN32)
    HMODULE 				m_Handle;
#else
    void*					m_Handle;
#endif
    vd::string 			    m_BaseName;
    fs::path 				m_Path;
    bool 					m_IsUtility;
    GetDescriptionFn 		m_GetDescriptionFn;
    CreateEntityFn 		    m_CreateEntityFn;
    CreateUtilityFn 		m_CreateUtilityFn;
};

class VD_API Module : public Core::Object
{
public:

    void LoadPlugin(const vd::string &name);

    std::vector<vd::string> GetLoadedPlugins() const;

    Entity* CreateEntity(
        const MetaClass* metaclass,
        const Config& config
    );

    Entity* CreateUtility(
        const Config& config
    );

    static void Startup();
    static void Shutdown();

    inline static Module*
    GetInstance()
    {
        return m_Instance;
    }

    VD_DECLARE_OBJECT(Module);

protected:
    Module();
    ~Module();

private:
    std::map<vd::string, Plugin*> 	    m_Plugins;
    mutable Handle<Mutex> 				m_Mutex;
    static Handle<Module> 	            m_Instance;
};

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_PLUGIN_INCLUDED
