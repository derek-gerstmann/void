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
#include "core/profiling.h"
#include "core/logging.h"
#include "core/system.h"
#include "core/process.h"
#include "core/timers.h"
#include "core/utilities.h"

#include <map>
#include <string>
#include <fstream>
#include <iostream>

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

AutoProfile::AutoProfile(const char label[]) : m_Label(label) 
{
    vdLogGlobalInfo("--- profile: %s Start : %f \n", m_Label);
    m_Start = Process::GetTimeInSeconds();
}

AutoProfile::~AutoProfile() 
{
    vd::f64 end = Process::GetTimeInSeconds();
    vd::f64 elapsed = end - m_Start;
    vdLogGlobalInfo("--- profile: %s End : %f - %f sec elapsed \n", 
        m_Label, end, elapsed);
}

// ============================================================================================== //

AutoTrace::AutoTrace(const char label[]) : m_Label(label) 
{
    vdLogGlobalDebug("--- trace: %s Enter\n", m_Label);
}

AutoTrace::~AutoTrace() 
{
    vdLogGlobalDebug("--- trace: %s Leave\n", m_Label);
}

// ============================================================================================== //

vd::f64 Profiler::Start() 
{ 
	running = true; 
	end = begin = Process::GetTimeInSeconds(); 
	count = 0; 
	return begin; 
} 

vd::f64 Profiler::Stop() 
{ 
	running = false; 
	end = Process::GetTimeInSeconds(); 
	count = count < 1 ? 1 : count; 
	return end; 
} 

vd::u32 Profiler::Add(vd::u32 v) 
{ 
	count += v; 
	return count; 
}

vd::f64 Profiler::GetElapsedTime() 
{ 
	return end - begin; 
}

vd::f64 Profiler::GetAverage() 
{ 
	return GetElapsedTime() / (double)count; 
}

void Profiler::Reset() 
{ 
	begin = end = 0.0; 
	running = false; 
	count = 0; 
}

// ============================================================================================== //

typedef std::map<std::pair<vd::string, vd::string>, Profiler *> ProfilerMap;

// ============================================================================================== //

static ProfilerMap GlobalProfilers;

// ============================================================================================== //

Profiler* 
vdAddProfiler(
	const vd::string& category, const vd::string& name) 
{
	Profiler* tracker;
	
	std::pair<vd::string, vd::string> s = std::make_pair(category, name);
	ProfilerMap::iterator iter = GlobalProfilers.find(s);
	if (iter != GlobalProfilers.end()) {
		return iter->second;
	}
	
	tracker = VD_NEW(Profiler, category, name);
	GlobalProfilers[s] = tracker;
	return tracker;
}

Profiler*
vdGetProfiler(
	const vd::string& category, const vd::string& name)
{
	return vdAddProfiler(category, name);
}

void
vdRemoveProfiler(
	const vd::string& category, const vd::string& name) 
{
	std::pair<vd::string, vd::string> s = std::make_pair(category, name);
	ProfilerMap::iterator iter = GlobalProfilers.find(s);
	if (iter != GlobalProfilers.end()) {
		GlobalProfilers.erase(iter);
	}
}

void
vdRemoveProfiler(
	const Profiler* tracker) 
{
	vdRemoveProfiler(tracker->GetCategory(), tracker->GetName());
}

void
vdOutputProfilerSummary(
	const vd::string& filename)
{
    std::ofstream infostream;
	infostream.open(filename.c_str());

	if (infostream.bad()) 
	{
		std::cerr << "Error, file " << filename << " could not be opened for writing" << std::endl;
		return;
	}

	infostream << '"' << "Category" << '"' << ", ";
	infostream << '"' << "Name"  << '"' << ", ";
	infostream << '"' << "Value"  << '"' << ", ";
	infostream << '"' << "Units"  << '"' << ", ";
	infostream << '"' << "Count" << '"' << ", " ;
	infostream << '"' << "Elapsed (sec)"  << '"' << ", ";
	infostream << '"' << "Average (sec)"  << '"' << ", ";
	infostream << '"' << "Start Time"  << '"' << ", ";
	infostream << '"' << "End Time"  << '"' << ", " << std::endl;

	ProfilerMap::iterator iter = GlobalProfilers.begin();
	while (iter != GlobalProfilers.end()) 
	{
		Profiler *tr = iter->second;
		infostream << '"' << tr->GetCategory() << '"' << ", ";
		infostream << '"' << tr->GetName() << '"' << ", ";
		infostream << '"' << tr->GetValue() << '"' << ", ";
		infostream << '"' << tr->GetUnits() << '"' << ", ";
		infostream << Convert::ToString(tr->GetCount()) << ", ";
		infostream << Convert::ToString(tr->GetElapsedTime()) << ", ";
		infostream << Convert::ToString(tr->GetAverage()) << ", ";
		infostream << Convert::ToString(tr->GetStartTime()) << ", ";
		infostream << Convert::ToString(tr->GetEndTime()) << ", "  << std::endl;
		++iter;
	}
	infostream.flush();
}

void
vdDestroyProfilers(void)
{
	ProfilerMap::iterator iter = GlobalProfilers.begin();
	while (iter != GlobalProfilers.end()) 
	{
		Profiler *tr = iter->second;
		delete tr;
		++iter;
	}
	GlobalProfilers.clear();
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

