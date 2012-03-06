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

#ifndef VD_CORE_SYMBOL_INCLUDED
#define VD_CORE_SYMBOL_INCLUDED

// ============================================================================================== //

#include "vd.h"
#include "core/core.h"
#include "core/registry.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class SymbolRegistry;

// ============================================================================================== //

class Symbol
{	
private:
	friend class SymbolRegistry;

public:

	explicit Symbol();
	explicit Symbol(const vd::uid key, const char* str, bool track=true);
	explicit Symbol(const char* str);
	Symbol(const Symbol& other);
	virtual ~Symbol();

	Symbol& operator=(const Symbol& other);

	bool operator==(const Symbol& other) const;
	bool operator!=(const Symbol& other) const;
	bool operator<(const Symbol& other) const;

	operator const char* () const { return m_Str; }
	const char* c_str() const { return m_Str; }
	const char* GetStr() const { return m_Str; }
	vd::uid GetKey() const { return m_Key; }
	vd::u64 GetId() const { return m_Id; }
	bool IsValid() const;

	static bool IsValid(const Symbol& symbol);
	static const Symbol Invalid;
	
	static const Symbol& Retrieve(const vd::uid key);
    static vd::uid Register(const char* bytes);
	static vd::uid Register(const Symbol& symbol);
	static void DestroyRegistry();
	static vd::u64 CreateRegistry();
	
protected:

	void RegisterSelf(void);
	Symbol* m_Next;

private:

	vd::u64 m_Id;
	vd::uid m_Key;
	const char* m_Str;
};

// ============================================================================================== //

struct SymbolEqualTo
    : std::binary_function<Symbol, Symbol, bool>
{
    bool operator()(Symbol const& x, Symbol const& y) const
    {
    	return x.GetKey() == y.GetKey();
    }
};

struct SymbolHash
    : std::unary_function<Symbol, vd::uid>
{
    vd::uid operator()(Symbol const& x) const
    {
    	return x.GetKey();
    }
};

// ============================================================================================== //

const char* 
AsString(const Symbol& symbol);

vd::uid
AsId(const Symbol& symbol);

// ============================================================================================== //

#if defined(VD_USE_EXTRACTED_SYMBOLS)

#define vd_sym( X ) VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE::Table[ ( VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE::Id ## X ) ]
#define vd_str( I ) VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE::Strings[ ( I ) ]
#define vd_uid( X ) VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE::Ids[ ( VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE::Id ## X ) ]

#else

#define vd_sym( X )  (const char *)( #X )
#define vd_str( I )  (const char *)( I )
#define vd_uid( X )  (vd::uid)( AsId( Symbol::Create( #X ) ))

#endif

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_SYMBOL_INCLUDED
