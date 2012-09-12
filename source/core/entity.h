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

#ifndef VD_CORE_ENTITY_INCLUDED
#define VD_CORE_ENTITY_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/config.h"
#include "core/repository.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class VD_API Entity : public Serializable
{
public:
    virtual void SetParent(Entity* parent);

    inline Entity* GetParent() { return m_Parent; }
    inline const Entity* GetParent() const { return m_Parent; }

    virtual void AddChild(const std::string &name, Entity* child);
    inline void AddChild(Entity *child) { AddChild("", child); }
    virtual void Configure();

    virtual void Serialize(Stream *stream, Repository* repo) const;

    VD_DECLARE_OBJECT(Entity);

protected:

    virtual ~Entity() { }
    inline Entity(const Config& config) 
        : Serializable(), m_Parent(NULL) { }
    
    Entity(Stream *stream, Repository* repo);

protected:
    Entity* m_Parent;
};

// ============================================================================================== //

#define VD_EXPORT_PLUGIN(name, descr)                           \
    extern "C" {                                                \
        void VD_API *CreateEntity(const Config& config) {       \
            return VD_NEW(name, config);                        \
        }                                                       \
        const char VD_API *GetDescription() {                   \
            return descr;                                       \
        }                                                       \
    }

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //
#endif // VD_CORE_ENTITY_INCLUDED

