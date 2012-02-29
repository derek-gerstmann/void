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

#ifndef VD_CORE_PROFILING_INCLUDED
#define VD_CORE_PROFILING_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/logging.h"
#include "core/timers.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class AutoProfile 
{
public:
    AutoProfile(const char label[]);
    ~AutoProfile();
private:
    const char* m_Label;
    vd::f64 m_Start;
};

// ============================================================================================== //

class AutoTrace 
{
public:
    AutoTrace(const char label[]);
    ~AutoTrace();
private:
    const char* m_Label;
};

// ============================================================================================== //

class Profiler 
{
public:
	explicit Profiler(
		const vd::string& cat, 
		const vd::string& n,
		const vd::string& v = vd::string("Time"),
		const vd::string& u = vd::string("sec")
	) :	category(cat), 
		name(n),
		value(v),
		units(u),
		running(false),
		begin(0.0),
		end(0.0),
		scale(1.0),
		count(0)
	{
		// EMPTY!
	}

	inline const vd::string& GetCategory() const { return category; }
	inline const vd::string& GetName() const { return name; }
	inline const vd::string& GetValue() const { return value; }
	inline const vd::string& GetUnits() const { return units; }
	inline vd::f64 GetStartTime() const { return begin; }
	inline vd::f64 GetEndTime() const { return end; }
	inline vd::u64 GetCount() const { return count; }
	inline bool IsRunning() const { return running; }

	vd::f64 Start();
	vd::f64 Stop(); 
	vd::u32 Add(vd::u32 v=1); 
	vd::f64 GetElapsedTime();
	vd::f64 GetAverage();
	void Reset();
	
private:

	vd::string category;
	vd::string name;
	vd::string value;
	vd::string units;
	
	bool running;
	vd::f64 begin;
	vd::f64 end;
	vd::f64 scale;
	vd::u64 count;
};

// ============================================================================================== //

static Profiler* 
vdAddProfiler(const vd::string& category, const vd::string& name) VD_OPTIONAL;

static Profiler*
vdGetProfiler(const vd::string& category, const vd::string& name) VD_OPTIONAL;

static void
vdRemoveProfiler(const vd::string& category, const vd::string& name) VD_OPTIONAL;

static void
vdRemoveProfiler(const Profiler* profiler) VD_OPTIONAL;

static void
vdOutputProfilerSummary(const vd::string& filename) VD_OPTIONAL;

static void
vdDestroyProfilers(void) VD_OPTIONAL;

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_PROFILING_INCLUDED

