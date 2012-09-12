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
#include "core/stream.h"
#include "core/repository.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

Repository::Repository() : m_Counter(0) 
{
#if defined(VD_DEBUG_SERIALIZATION)
	vdLogDebug("Creating a repository");
#endif
}

Repository::~Repository() 
{
#if defined(VD_DEBUG_SERIALIZATION)
	vdLogDebug("Destroying a repository");
#endif
	AllocList::iterator it;
	for (it = m_Allocated.begin(); it!= m_Allocated.end(); ++it) 
	{
		(*it)->Release();
	}
}

Serializable*
Repository::Retrieve(Stream* stream) 
{
	m_LastId = stream->ReadU32();
	if (m_LastId == 0) 
	{
		return NULL;
	} 
	else if (m_IdToObjectMap.find(m_LastId) != m_IdToObjectMap.end()) 
	{
		return m_IdToObjectMap[m_LastId];
	} 
	else 
	{
		Serializable *object = NULL;
		vd::string class_name = stream->ReadString();
#if defined(VD_DEBUG_SERIALIZATION)
		vdLogDebug("Loading a class of type '%s'", class_name.c_str());
#endif
		const Core::Symbol& class_symbol = Symbol::Register(class_name.c_str());
		const MetaClass* meta = MetaClass::Retrieve(class_symbol);
		if (meta == NULL)
		{
			vdLogError("Class with name '%s' not found!", class_name.c_str());
		}
		
		try 
		{
			object = static_cast<Serializable*>(meta->Load(stream, this));
		} 
		catch (std::exception &e) 
		{
			vdLogError(
				"Encountered an exception while unserializing an "
				"instance of \"%s\": \"%s\"!", class_name.c_str(), e.what()
			);
		}
		m_Allocated.push_back(object);
		object->Retain();
		return object;
	}
}

void Repository::Register(
	Serializable *object) 
{
	m_IdToObjectMap[m_LastId] = object;
}

void Repository::Serialize(
	Stream* stream, const Serializable* object) 
{
	if (object == NULL) 
	{
		stream->WriteU32(0);
	} 
	else if (m_ObjectToIdMap.find(object) != m_ObjectToIdMap.end()) 
	{
		stream->WriteU32(m_ObjectToIdMap[object]);
	} 
	else 
	{
#if defined(VD_DEBUG_SERIALIZATION)
		vdLogDebug("Serializing a class of type '%s'", object->GetMetaClass()->GetClassName().c_str());
#endif
		stream->WriteU32(++m_Counter);
		stream->WriteString(object->GetMetaClass()->GetClassName().c_str());
		m_ObjectToIdMap[object] = m_Counter;
		object->Serialize(stream, this);
	}
}

Serializable::Serializable(
	Stream *stream, Repository* repo) 
{ 
	repo->Register(this);
}

// ============================================================================================== //

VD_IMPLEMENT_ABSTRACT_OBJECT(Serializable, vd_sym(Serializable), vd_sym(Object));
VD_IMPLEMENT_OBJECT(Repository, vd_sym(Repository), vd_sym(Object));

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //
