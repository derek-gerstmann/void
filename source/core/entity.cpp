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
#include "core/traits.h"
#include "core/threading.h"
#include "core/logging.h"
#include "core/asserts.h"
#include "core/symbol.h"

#include "constants/constants.h"

#include <sstream>
#include <string>

#define VD_DEBUG_REFCOUNTS (1)

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

Entity::Entity(const Entity*) : 
    Shared<Entity>()
{
    // EMPTY!
}

Entity::~Entity()
{
    vd::i32 count = GetRefCount();
    vdGlobalAssertMsg( count == 0, "Deleting %s with reference count %i!", ToString().c_str(), count);
}

vd::status
Entity::Destroy()
{
    return Status::Code::Success;
}

void
Entity::SetId(vd::uid id)
{
    m_Id = id;
}

vd::uid
Entity::GetId() const
{
    return m_Id;
}

vd::string 
Entity::ToString() const
{
    std::ostringstream oss;
    oss << GetTraits()->GetName().GetStr();
    oss << " [unknown]";
    return oss.str();
}

// ============================================================================================== //

VD_IMPLEMENT_ABSTRACT_ENTITY(Entity, vd_sym(Entity), vd::symbol());

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

