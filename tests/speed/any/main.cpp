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

#include "vd.h"
#include "core/core.h"
#include "test/test.h"
#include "test/speed.h"
#include "containers/containers.h"
#include "containers/anymap.h"
#include "containers/types.h"

#include <boost/any.hpp>

// ============================================================================================== //

VD_TEST_NAMESPACE_BEGIN();

// ============================================================================================== //

typedef vd::Containers::AnyMap<vd::uid, vd::UidHash> TestAnyMap;
typedef vd::Containers::AlignedVector<vd::v3f32>::type Array3f;

// ============================================================================================== //

class AnyMapSpeedTest : public Test::Speed
{
public:
	virtual ~AnyMapSpeedTest() {}
	
	vd::f64 GetInsertTime(void) const { return m_InsertTime; }
	vd::f64 GetRetrieveTime(void) const { return m_RetrieveTime; }
	vd::f64 GetClearTime(void) const { return m_ClearTime; }
	
	void RunN(unsigned N)
	{
		vd::f64 start;
		vd::f64 end;
		
		vd::v3f32 v; 
		TestAnyMap anymap;
		Array3f array(N);
			
		for(int j=0; j<8; ++j)
		{
			start = Core::Process::GetTimeInSeconds();
			for(unsigned i = 0; i< N; ++i)
			{
				vd::uid id(i);
				anymap.Set<vd::v3f32>(id) = array[i];
			}
			end = Core::Process::GetTimeInSeconds();		
			
			m_InsertTime += (end - start) / N;
			
			start = Core::Process::GetTimeInSeconds();
			for(int k=0; k<100; ++k)
			{
				unsigned ai = anymap.size();
				for(unsigned ei = 0; ei < ai; ei++)
				{
					vd::uid id(ei);
					v = anymap.Get<vd::v3f32>(id);
				}
			}
			end = Core::Process::GetTimeInSeconds();
	
			m_RetrieveTime += (end - start) / 100.0 * N;
	
			start = Core::Process::GetTimeInSeconds();
			anymap.clear();
			end = Core::Process::GetTimeInSeconds();
	
			m_ClearTime += (end - start) / N;
		}
	}

protected:

	vd::f64 m_InsertTime;
	vd::f64 m_RetrieveTime;
	vd::f64 m_ClearTime;

};

// ============================================================================================== //

VD_DEFINE_TEST_WITH_PARAM(AnyMapSpeedTest, RunN) 
{
	RunN(GetParam());
	Base::RecordProperty("Iterations", GetIterationCount());
	Base::RecordProperty("InsertTime", GetInsertTime());
	Base::RecordProperty("RetrieveTime", GetRetrieveTime());
	Base::RecordProperty("ClearTime", GetClearTime());
}

// ============================================================================================== //

VD_GENERATE_TEST_WITH_PARAM(AnyMapSpeedTestN, AnyMapSpeedTest,
	Test::Range(1000, 100000, 10000));

// ============================================================================================== //

VD_TEST_NAMESPACE_END();

// ============================================================================================== //

VD_USING_MODULE(Test);

// ============================================================================================== //

int main(int argc, char **argv) 
{
    Test::System::Startup(&argc, argv);
    int status = Test::System::RunAllTests();
    Test::System::Shutdown();
    return status;
}

// ============================================================================================== //
