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

#ifndef VD_CORE_CASTS_INCLUDED
#define VD_CORE_CASTS_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/memory.h"

#include <stdexcept>
#include <typeinfo>
#include <sstream>
#include <cassert>
#include <string>

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

namespace Lexical
{
    template <typename Target, typename Source>
    struct GenericCastPolicy
    {
        static 
        Target Apply(const Source &arg)
        {
            Target ret;
            static __thread std::stringstream * ss;
            if (!ss) {
                ss = new std::stringstream;
            }

            ss->clear();
            if(!( *ss << arg &&  *ss >> ret && (*ss >> std::ws).eof() )) 
            {
                ss->str(std::string());
                throw ExceptionBadLexicalCast();
            }
            return ret;
        }
    };

    template <typename Target, typename Source>
    struct ConvertibleCastPolicy
    {
        static 
        Target Apply(const Source &arg)
        {
            return arg;
        };
    };
    
    template <typename T>
    struct NullCastPolicy
    {
        static
        const T& Apply(const T &arg)
        {
            return arg;
        }
    };

    template <typename Target, typename Source, typename Enable = void> 
    struct TraitInfo;

    template <typename Target, typename Source> 
    struct TraitInfo<Target, Source, typename std::enable_if<!std::is_convertible<Source,Target>::value>::type> 
    {
        typedef GenericCastPolicy<Target,Source> Policy; 
    };

    template <typename Target, typename Source> 
    struct TraitInfo<Target, Source, typename std::enable_if<std::is_convertible<Source,Target>::value && 
                                                                 !std::is_same<Source,Target>::value>::type> 
    {
        typedef ConvertibleCastPolicy<Target,Source> Policy; 
    };

    template <typename T> 
    struct TraitInfo<T,T,void>
    {
        typedef NullCastPolicy<T> Policy; 
    };
}

template <typename Target, typename Source> 
Target vd_lexical_cast(const Source &arg)
{
    return Lexical::TraitInfo<Target,Source>::Policy::Apply(arg);
}  

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_CASTS_INCLUDED
