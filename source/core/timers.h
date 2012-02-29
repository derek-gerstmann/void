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

#ifndef VD_CORE_TIMERS_INCLUDED
#define VD_CORE_TIMERS_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/object.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class Timer : public Object
{
public:
    Timer();
    void Reset();

    vd::u64 GetMilliseconds() const;
    vd::u64 GetMicroseconds() const;
	vd::f64 GetSeconds() const;
    vd::string ToString() const;

    VD_DECLARE_OBJECT(Timer);
    
protected:
    virtual ~Timer();

private:
	vd::f64 m_Start;
};

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_TIMERS_INCLUDED



