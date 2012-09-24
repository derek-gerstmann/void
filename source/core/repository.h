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

#ifndef VD_CORE_REPOSITORY_INCLUDED
#define VD_CORE_REPOSITORY_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/object.h"
#include "core/collections.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class VD_API Serializable : public Core::Object 
{
public:
	Serializable(Stream *stream, Repository* repo);
	virtual void Serialize(Stream *stream, Repository *repo) const = 0;
	VD_DECLARE_OBJECT(Serializable);

protected:
	inline Serializable() { }
	virtual ~Serializable() { }
};

// ============================================================================================== //

class VD_API Repository : public Core::Object 
{
	friend class Serializable;
    typedef Core::Vector< Serializable* >::type 				AllocList;
    typedef Core::Map<vd::u32, Serializable* >::type 			IdToObjectMap;
    typedef Core::Map<const Serializable*, vd::u32 >::type 		ObjectToIdMap;

public:
	Repository();
	Serializable* Retrieve(Stream* stream);
	void Serialize(Stream* stream, const Serializable* object);

	VD_DECLARE_OBJECT(Repository);

private:

	virtual ~Repository();
	void Register(Serializable* object);

private:
	vd::u32 		m_Counter;
	vd::u32 		m_LastId;
	AllocList 		m_Allocated;
	IdToObjectMap 	m_IdToObjectMap;
	ObjectToIdMap 	m_ObjectToIdMap;
};

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_REPOSITORY_INCLUDED
