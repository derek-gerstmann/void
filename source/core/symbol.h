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
#include "core/hashing.h"

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
	explicit Symbol(const vd::uid key, const char* str);
    Symbol(const Symbol& other);
    Symbol(const vd::symbol& other);
	virtual ~Symbol();

	Symbol& operator=(const Symbol& other);

	bool operator==(const Symbol& other) const;
	bool operator!=(const Symbol& other) const;
	bool operator<(const Symbol& other) const;

//	operator const char* () const { return Lookup(m_Key); }
//	const char* c_str() const { return Lookup(m_Key); }
	const char* GetStr() const { return Lookup(m_Key); }
	vd::uid GetKey() const { return m_Key; }
	vd::u64 GetId() const { return m_Id; }
	bool IsValid() const;

    VD_FORCE_INLINE
    bool operator< (const Symbol& b)
    {
        if(this->GetKey() != b.GetKey() ) 
            return this->GetKey().GetUpper() < b.GetKey().GetUpper() && 
                   this->GetKey().GetLower() < b.GetKey().GetLower();

        return false;
    }

    static Symbol GetInvalid();
	static bool IsValid(const Symbol symbol);

    static const vd::u64 Resolve(const vd::uid key);   
    static const char* Lookup(const vd::uid key);	
	static Symbol Retrieve(const vd::uid key);
    static Symbol Register(const char* bytes);
	static Symbol Register(const vd::uid key, const char* bytes);

	static void DestroyRegistry();
	static vd::u64 CreateRegistry();
	
	static const char* 
	ToString(const Symbol symbol);

	static vd::u64
	ToId(const Symbol symbol);

    static vd::uid
    ToKey(const Symbol symbol);

private:

    static SymbolRegistry* GetRegistry(void);

	vd::u64 m_Id;
	vd::uid m_Key;
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

#if defined(VD_USE_EXTRACTED_SYMBOLS)

#define vd_sym( X ) VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE::Table[ ( VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE::Id ## X ) ]
#define vd_str( I ) VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE::Strings[ ( I ) ]
#define vd_uid( X ) VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE::Ids[ ( VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE::Id ## X ) ]

#else

#if defined(VD_SYMEX_CLIENT)
#define vd_sym( X )
#define vd_str( I )
#define vd_uid( X )
#else
#include "core/hashing.h"
#warning "Using inline symbols (run SymEx to extract symbols into static data)"
#define vd_sym( X )  vd::symbol(Core::Hashing::StringHashFnv( #X ), (const char *)( #X ))
#define vd_str( I )  (const char *)( I )
#define vd_uid( X )  ((vd::uid)Core::Hashing::StringHashFnv( #X ))
#endif

#endif

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_SYMBOL_INCLUDED
