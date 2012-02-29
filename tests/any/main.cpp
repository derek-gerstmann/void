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

#include "core/test.h"
#include "core/framework.h"
#include "core/geometry.h"

#include <boost/any.hpp>
#include <string>

// ============================================================================================== //

using vd::core::BasicTest;
using vd::core::SpeedTest;
using vd::core::Vector3f;

typedef vd::core::AnyMap<vd::uid, vd::core::UidHash> TestAnyMap;
typedef vd::core::AlignedVector<Vector3f>::type Array3f;

/*
class TestPropertySet : public PropertySetBase
{
	VD_DECLARE_PROPERTY_TYPE_SUPPORT(int);
	VD_DECLARE_PROPERTY_TYPE_SUPPORT(float);
	VD_DECLARE_PROPERTY_TYPE_SUPPORT(Vector3f);
};
*/
// ============================================================================================== //

class AnyMapSpeedTest : public SpeedTest
{
public:

	vd::f64 GetInsertTime(void) const { return m_InsertTime; }
	vd::f64 GetRetrieveTime(void) const { return m_RetrieveTime; }
	vd::f64 GetClearTime(void) const { return m_ClearTime; }
	
	void RunN(unsigned N)
	{
		vd::f64 start;
		vd::f64 end;
		
		Vector3f v; 
		TestAnyMap anymap;
		Array3f array(N);
			
		for(int j=0; j<8; ++j)
		{
			start = vd::core::System::GetTimeInSeconds();
			for(unsigned i = 0; i< N; ++i)
			{
				vd::uid id = i;
				anymap.Set<Vector3f>(id) = array[i];
			}
			end = vd::core::System::GetTimeInSeconds();		
			
			m_InsertTime += (end - start) / N;
			
			start = vd::core::System::GetTimeInSeconds();
			for(int k=0; k<100; ++k)
			{
				unsigned ai = anymap.size();
				for(unsigned ei = 0; ei < ai; ei++)
				{
					vd::uid id = ei;
					v = anymap.Get<Vector3f>(id);
				}
			}
			end = vd::core::System::GetTimeInSeconds();
	
			m_RetrieveTime += (end - start) / 100.0 * N;
	
			start = vd::core::System::GetTimeInSeconds();
			anymap.clear();
			end = vd::core::System::GetTimeInSeconds();
	
			m_ClearTime += (end - start) / N;
		}
	}

protected:

	vd::f64 m_InsertTime;
	vd::f64 m_RetrieveTime;
	vd::f64 m_ClearTime;

};

// ============================================================================================== //

TEST_P(AnyMapSpeedTest, RunN) 
{
	RunN(GetParam());
	RecordProperty("Iterations", GetIterationCount());
	RecordProperty("InsertTime", GetInsertTime());
	RecordProperty("RetrieveTime", GetRetrieveTime());
	RecordProperty("ClearTime", GetClearTime());
}

// ============================================================================================== //

INSTANTIATE_TEST_CASE_P(AnyMapSpeedTestN, AnyMapSpeedTest,
                        ::testing::Range(1000, 100000, 10000));

// ============================================================================================== //
