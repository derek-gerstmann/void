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

#include "core/entity.h"
#include "core/object.h"
#include "core/logging.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

Entity::Entity(
    Stream* stream, Repository* repo
) : 
    Serializable(stream, repo) 
{
    m_Parent = static_cast<Entity*>(repo->Retrieve(stream));
}

void 
Entity::SetParent(
    Entity *parent) 
{
    m_Parent = parent;
}

void 
Entity::Configure() 
{

}

void 
Entity::Serialize(
    Stream *stream, Repository* repo) const 
{
    if (GetMetaClass()->IsSerializable() == false)
    {
        vdLogError(
            "Unable to serialize an instance of type '%s': no serialization support!", 
            GetMetaClass()->GetClassName().c_str()
        );
    }
    repo->Serialize(stream, m_Parent);
}

void 
Entity::AddChild(
    const std::string &name, Entity *child) 
{
    vdLogGlobalError(
        "Entity::AddChild('%s', '%s') not implemented for '%s'", 
        name.c_str(), child->ToString().c_str(), ToString().c_str()
    );
}

// ============================================================================================== //

VD_IMPLEMENT_ABSTRACT_OBJECT(Entity, vd_sym(Entity), vd_sym(Serializable));

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

