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

#include <cstring>

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

const Symbol Symbol::Invalid = Symbol(0, "<NULL>");

// ============================================================================================== //

class SymbolRegistry : public Object
{
public:

	typedef Containers::Vector<vd::string>::type 							StringTable;
	typedef Containers::Map<vd::uid, Symbol, UidHash, UidEqualTo>::type 	SymbolMap;

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
		m_SymbolMap.clear();
		m_StringTable.clear();
		return Status::Code::Success;
	}
	
	vd::uid Add(Symbol& symbol)
	{
		if(symbol.IsValid() == false)
			return Constants::Zero;

		if(symbol.m_Str != NULL)
		{
			vd::u64 index = 0;
			vd::uid key = symbol.m_Key;
			vd::string str = vd::string(symbol.m_Str);
			if(key == Constants::Zero)
			{
				key = Hashing::Murmur(str.c_str(), str.length());
			}

			SymbolMap::const_iterator it = m_SymbolMap.find(key);
			if(it == m_SymbolMap.end())
			{
				index = m_StringTable.size();
				m_StringTable.push_back(str);
				symbol.m_Str = m_StringTable[index].c_str();
				symbol.m_Key = key;
				symbol.m_Id = index;
				m_SymbolMap[key] = symbol;

				vdLogInfo("Symbol[%s] : Adding symbol [%02d] : [%s] '%s'", 
					str.c_str(), index, Convert::ToString(key).c_str(), AsString(symbol));
			}
			return key;
		}

		return Constants::Zero;
	}
	
	const Symbol& 
	Find(const vd::uid key) const
	{
		SymbolMap::const_iterator it = m_SymbolMap.find(key);
		if(it != m_SymbolMap.end())
		{
			const Symbol& s = (it->second);
			return s;
		}	
		return Symbol::Invalid;
	}

	const bool 
	Exists(const vd::uid key) const
	{
		SymbolMap::const_iterator it = m_SymbolMap.find(key);
		if(it != m_SymbolMap.end())
		{
			return true;
		}	
		return false;
	}
	
	const bool 
	Exists(const Symbol& symbol) const
	{
		if(symbol.IsValid() == false)
			return false;
			
		vd::uid key = symbol.GetKey();
		return Exists(key);
	}
	
	bool Remove(const Symbol& symbol)
	{
		return Remove(symbol.GetKey());
	}
	
	bool Remove(const vd::uid key)
	{
		SymbolMap::const_iterator it = m_SymbolMap.find(key);
		if(it != m_SymbolMap.end())
		{
			const Symbol& s = (it->second);
			vd::u64 index = s.m_Id;
			m_StringTable[index] = vd::string();
			m_SymbolMap.erase(it);
			return true;
		}

		return false;
	}
    
	VD_DECLARE_OBJECT(SymbolRegistry);
	
private:

	VD_DISABLE_COPY_CONSTRUCTORS(SymbolRegistry);

	SymbolMap 	m_SymbolMap;
	StringTable m_StringTable;
};

// ============================================================================================== //

VD_IMPLEMENT_OBJECT(SymbolRegistry, vd_sym(SymbolRegistry), vd_sym(Object));

// ============================================================================================== //

namespace
{
	static SymbolRegistry GlobalSymbolRegistry;
	static Symbol* SymbolListHead = NULL;
	static Symbol* SymbolListTail = NULL;
}

// ============================================================================================== //

void Symbol::RegisterSelf(void)
{
    if(SymbolListHead != NULL)
    {
        SymbolListTail->m_Next = this;
        SymbolListTail = this;
    }
    else
    {
        SymbolListHead = this;
        SymbolListTail = this;
    }
}

// ============================================================================================== //

Symbol::Symbol(
	const char* str)
{ 
	m_Str = str;
	m_Id = 0;
	m_Key = 0;
}

Symbol::Symbol(
	const vd::uid key, 
	const char* str,
	bool track
) :
	m_Next(NULL),
	m_Id(0),
	m_Key(),
	m_Str("<NULL>")
{ 
	m_Key = key;
	m_Str = str;

	if(track && key && str)
		RegisterSelf();
}

Symbol::Symbol() : 
	m_Next(NULL),
	m_Id(0),
	m_Key(),
	m_Str("<NULL>")
{ 

}

Symbol::~Symbol() 
{ 
	m_Id = 0;
	m_Key = 0;
	m_Str = NULL;
}

Symbol::Symbol(
	const Symbol& other) 
{
	m_Id = other.m_Id;
	m_Key = other.m_Key;
	m_Str = other.m_Str;
}

Symbol& 
Symbol::operator=(const Symbol& other)
{
	if(this == &other)
		return *this;
		
	m_Id = other.m_Id;
	m_Key = other.m_Key;
	m_Str = other.m_Str;
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

bool 
Symbol::IsValid() const
{
	return Symbol::Invalid == *this;
}

vd::uid 
Symbol::Register(const char* bytes)	
{		
	vd::uid key = Constants::Zero;
	if(bytes == NULL)
		return key;
		
	Symbol symbol(key, bytes, true);
	key = Symbol::Register(symbol);
	return key; // Symbol::Retrieve(key);
}

vd::uid 
Symbol::Register(const Symbol& name)
{
	Symbol rv = name;
	return GlobalSymbolRegistry.Add(rv);
	return rv.GetId();	
}

const Symbol&
Symbol::Retrieve(vd::uid key)
{
	return GlobalSymbolRegistry.Find(key);
}

bool 
Symbol::IsValid(
	const Symbol& symbol)
{
	return Symbol::Invalid == symbol;
}

// ============================================================================================== //

vd::u64
Symbol::CreateRegistry(void)
{
	typedef Containers::Map<vd::uid, vd::uid, UidHash, UidEqualTo>::type 	SymbolMap;

	vd::u64 count = 0;
	SymbolMap existing;

	Symbol* sym = SymbolListHead; 
	while(sym != NULL)
	{
		if(existing.find(sym->m_Key) == existing.end())
		{
			vd::uid key = Symbol::Register(*sym);
			// const Symbol& s = Symbol::Retrieve(key);
			existing[key] = 1;
		}
		sym = sym->m_Next;
		count++;
	} 
	
	return count;
}

void
Symbol::DestroyRegistry()
{
	GlobalSymbolRegistry.Destroy();
}

// ============================================================================================== //

const char* 
AsString(const Symbol& symbol)
{
	return symbol.GetStr() == NULL ? "<NULL>" : symbol.GetStr();
}

vd::uid
AsId(const Symbol& symbol)
{
	return symbol.GetKey();
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

