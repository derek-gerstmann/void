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

#ifndef VD_CORE_FACTORY_INCLUDED
#define VD_CORE_FACTORY_INCLUDED

#include <vector>
#include <string>
#include <algorithm>

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

template <class T, class U> struct IsSameType { enum { result = false }; };
template <class T> struct IsSameType<T,T> { enum { result = true }; };

template <bool> class CompileTimeChecker;
template <> class CompileTimeChecker<true> { };

// ============================================================================================== //

template <class Product, typename Identifier = vd::uid>
class Factory
{
public:
    typedef const Product* (*Creator) ();

private:
    Identifier identifier_;
    Creator    creator_;
    bool       registered_;

    typedef Product *const (*DerivedCreator) (const Identifier &);
    typedef std::vector<DerivedCreator> DerivedCreators;
    DerivedCreators derived_;

private:
    explicit Factory() : registered_( false ) { }
    ~Factory() { }

    Factory(const Factory &);
    const Factory &operator =(const Factory &);

private:
    template <class T>
    static Product *const CreateDerived(const Identifier &identifier)
    {
        return Factory<T, Identifier>::Instance().Create( identifier );
    }

public:

    static Factory<Product, Identifier> &Instance()
    {
        static Factory<Product, Identifier> factory;
        return factory;
    }

    static Product *const DefaultCreator()
    {
        return VD_NEW(Product);
    }

    const bool Register(const Identifier &identifier, const Creator creator)
    {
        vdAssert( (!registered_) );
        identifier_  = identifier;
        creator_     = creator;
        registered_ = true;
        return true;
    }

    template <class T>
    const bool RegisterDerived()
    {
        (void)sizeof( CompileTimeChecker< !IsSameType<T, Product>::result > );
        const DerivedCreator derivedCreator = CreateDerived<T>;
        vdAssert( (std::find( derived_.begin(), derived_.end(), derivedCreator ) == derived_.end()) );
        derived_.push_back( derivedCreator );
        return true;
    }

    Product *const Create(const Identifier &identifier)
    {
        if( identifier_ == identifier )
            return (creator_)? (creator_()): (0);

        for(typename DerivedCreators::const_iterator itr = derived_.begin(); 
        	itr != derived_.end(); ++itr)
        {
            Product *const pProduct = (*itr)( identifier );
            if( pProduct )
                return pProduct;
        }

        return 0;
    }
};

// ============================================================================================== //
// Helper functions for Factories with vd::uid identifier types.
// ============================================================================================== //

// Registers an identifier and a creator with a Product
template <class Product>
const bool Register(
	const vd::uid &identifier, 
	const typename Factory<Product>::Creator creator = Factory<Product>::DefaultCreator)
{
    return Factory<Product>::Instance().Register( identifier, creator );
}

// Registers a Product's base class
template <class Product, class ProductBase>
const bool RegisterBase()
{
    return Factory<ProductBase>::Instance().template RegisterDerived<Product>();
}

// Creates an object of type Product, given a product identifier
template <class T>
const T* Create(const vd::uid &identifier)
{
    return Factory<T>::Instance().Create( identifier );
}

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

#endif // VD_CORE_FACTORY_INCLUDED

