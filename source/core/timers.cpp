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

#include "core/timers.h"
#include "core/system.h"
#include "core/process.h"

#include "constants/constants.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

Timer::Timer()
{
    Reset();
}

Timer::~Timer()
{
}

void Timer::Reset()
{
	m_Start = Process::GetTimeInSeconds();
}

vd::u64 
Timer::GetMicroseconds() const
{
	vd::f64 current = Process::GetTimeInSeconds();
	return vd::u64((current - m_Start) * 1000000.0);
}

vd::u64 
Timer::GetMilliseconds() const
{
	vd::f64 current = Process::GetTimeInSeconds();
	return vd::u64((current - m_Start) * 1000.0);
}

vd::string 
Timer::ToString() const
{
    std::ostringstream oss;
    oss << "Timer[ms=" << GetMilliseconds() << "]";
    return oss.str();
}

// ============================================================================================== //

VD_IMPLEMENT_OBJECT(Timer, vd_sym(Timer), vd_sym(Object));

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

