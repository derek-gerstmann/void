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
#include "constants/symbols.h"
#include "containers/containers.h"
#include "containers/set.h"

#include "core/symbol.h"
#include "core/macros.h"
#include "core/memory.h"
#include "core/asserts.h"
#include "core/hashing.h"
#include "core/registry.h"
#include "core/logging.h"
#include "core/threading.h"
#include "core/registry.h"
#include "core/utilities.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class SymbolRegistry : public Object
{
public:

	typedef Containers::SetVector<Core::Symbol>	 	 	SymbolSet;
	typedef Containers::Vector<char*>::type 	 		StringTable;
	typedef Containers::Map<vd::uid, vd::u64>::type 	SymbolMap;

public:

    explicit SymbolRegistry(): 
    	Object()
    { 
		// EMPTY!
    }

    virtual ~SymbolRegistry()
    {
    	Destroy();
    }

	vd::status Destroy(void)
	{
		m_SymbolSet.clear();
		m_StringTable.clear();
		return Status::Code::Success;
	}
	
	Symbol
	Add(const vd::uid key, const char* bytes)
	{
		Symbol symbol;
		symbol.m_Key = key;
		if(bytes == NULL)
			return symbol;

		vd::u64 index = VD_U64_MAX;
		size_t length = bytes ? strlen(bytes) : 0;
		if(key == Constants::Zero && length)
		{
			symbol.m_Key = Hashing::Murmur(bytes, length);
		}

		std::pair<SymbolSet::iterator, bool> added = m_SymbolSet.insert(symbol);
		if(added.second)
		{
			index = m_StringTable.size();
			char* str = VD_NEW_ARRAY(char, length + 1);
			Memory::CopyBytes(str, bytes, length);
			m_StringTable.push_back(str);
			added.first->m_Id = index;
		}

        symbol.m_Key = added.first->m_Key;
        symbol.m_Id = added.first->m_Id;
		return symbol;
	}
	
	Symbol
	Retrieve(const vd::uid key) const
	{
		Symbol symbol;
		symbol.m_Key = key;

		SymbolSet::const_iterator it = m_SymbolSet.find(symbol);
		if(it != m_SymbolSet.end())
		{
			return (*it);
		}	
		return Symbol::GetInvalid();
	}

	vd::u64
	Resolve(const vd::uid key) const
	{
		Symbol symbol;
		symbol.m_Key = key;

		SymbolSet::const_iterator it = m_SymbolSet.find(symbol);
		if(it != m_SymbolSet.end())
		{
			return it->m_Id;
		}	
		return VD_U64_MAX;
	}

	const char* 
	Lookup(const vd::u64 index) const
	{
		if(index < m_StringTable.size())
		{
			return m_StringTable[index];
		}	
		return NULL;
	}

	bool 
	Exists(const vd::uid key) const
	{
		Symbol symbol;
		symbol.m_Key = key;
		SymbolSet::const_iterator it = m_SymbolSet.find(symbol);
		if(it != m_SymbolSet.end())
		{
			return true;
		}	
		return false;
	}
	
	bool 
	Exists(const Symbol& symbol) const
	{
		if(symbol.IsValid() == false)
			return false;
			
		vd::uid key = symbol.GetKey();
		return Exists(key);
	}
	
	bool 
	Remove(const Symbol& symbol)
	{
		SymbolSet::const_iterator it = m_SymbolSet.find(symbol);
		if(it != m_SymbolSet.end())
		{
			vd::u64 index = it->m_Id;
			m_SymbolSet.erase(*it);
			char* str = m_StringTable[index];
			m_StringTable[index] = NULL;
			VD_DELETE_ARRAY(str);
			return true;
		}

		return false;
	}
	
	bool 
	Remove(const vd::uid key)
	{
		Symbol symbol;
		symbol.m_Key = key;
		return Remove(symbol);
	}
    
	VD_DECLARE_OBJECT(SymbolRegistry);
	
private:

	VD_DISABLE_COPY_CONSTRUCTORS(SymbolRegistry);

	StringTable m_StringTable;
	SymbolSet	m_SymbolSet;
};

// ============================================================================================== //

namespace Internal
{
	static SymbolRegistry* Registry = NULL;
}

// ============================================================================================== //

VD_IMPLEMENT_OBJECT(SymbolRegistry, vd_sym(SymbolRegistry), vd_sym(Object));

// ============================================================================================== //

Symbol::Symbol() : 
	m_Id(0),
	m_Key(0,0)
{ 

}

Symbol::Symbol(
	const vd::symbol& sym
) :
	m_Id(0),
	m_Key(sym.key)
{ 
	if(sym.key)	
		m_Id = Symbol::Retrieve(sym.key).GetId();
}

Symbol::Symbol(
	const vd::uid key, 
	const char* str
) :
	m_Id(0),
	m_Key(key)
{ 
	if(key && str)	
		*this = Symbol::Register(key, str);
}

Symbol::Symbol(
	const Symbol& other) 
{
	m_Id = other.m_Id;
	m_Key = other.m_Key;
}

Symbol::~Symbol() 
{ 
	m_Id = 0;
	m_Key = Constants::Zero;
}

Symbol& 
Symbol::operator=(const Symbol& other)
{
	if(this == &other)
		return *this;

	m_Id = other.m_Id;
	m_Key = other.m_Key;
	return *this;
}

bool 
Symbol::operator==(const Symbol& other) const
{
	return (m_Key == other.m_Key);
}

bool 
Symbol::operator!=(const Symbol& other) const
{
	return (m_Key != other.m_Key);
}

bool 
Symbol::operator<(const Symbol& other) const
{
	return (m_Key < other.m_Key);
}

Symbol
Symbol::GetInvalid()
{
	static const Symbol invalid;
	return invalid;
}

bool 
Symbol::IsValid() const
{
	return (*this == GetInvalid());
}

Symbol 
Symbol::Register(const char* bytes)	
{		
	vdGlobalAssert(bytes != NULL);
	if(bytes == NULL) return Symbol::GetInvalid();
	SymbolRegistry* registry = Symbol::GetRegistry();
	return registry->Add(Constants::Zero, bytes);
}

Symbol 
Symbol::Register(
	const vd::uid key, const char* bytes)
{
	SymbolRegistry* registry = Symbol::GetRegistry();
	return registry->Add(key, bytes);
}

vd::u64
Symbol::Resolve(vd::uid key)
{
	SymbolRegistry* registry = Symbol::GetRegistry();
	return registry->Resolve(key);
}

const char*
Symbol::Lookup(vd::uid key)
{
	SymbolRegistry* registry = Symbol::GetRegistry();
	vd::u64 index = registry->Resolve(key);
	return registry->Lookup(index);
}

Symbol
Symbol::Retrieve(vd::uid key)
{
	SymbolRegistry* registry = Symbol::GetRegistry();
	return registry->Retrieve(key);
}

bool 
Symbol::IsValid(
	const Symbol symbol)
{
	return (symbol == GetInvalid());
}

// ============================================================================================== //

vd::u64
Symbol::CreateRegistry()
{
	if(Internal::Registry == NULL)
		Internal::Registry = VD_NEW(SymbolRegistry);

	SymbolRegistry* registry = Symbol::GetRegistry();
	vd::symbol* sym = vd::symbol::head;

	vd::u64 count = 0;
	while(sym != NULL)
	{
		if(sym->key && sym->str && !registry->Exists(sym->key))
		{
			registry->Add(sym->key, sym->str);
		}

		sym = sym->next;
		count++;
	} 

	return count;
}

void
Symbol::DestroyRegistry()
{
	vdGlobalAssertMsg((Internal::Registry != NULL), 
		"Symbol registry has not been initialized!");

	VD_DELETE(Internal::Registry);
}

SymbolRegistry*
Symbol::GetRegistry()
{
	vdGlobalAssertMsg((Internal::Registry != NULL), 
		"Symbol registry has not been initialized!");

	return Internal::Registry;
}

// ============================================================================================== //

const char* 
Symbol::ToString(const Symbol symbol)
{
	SymbolRegistry* registry = Symbol::GetRegistry();
	vd::u64 index = symbol.GetId();
	return registry->Lookup(index);
}

vd::u64
Symbol::ToId(const Symbol symbol)
{
	return symbol.GetId();
}

vd::uid
Symbol::ToKey(const Symbol symbol)
{
	return symbol.GetKey();
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

