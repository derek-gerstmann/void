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
#include "core/registry.h"
#include "test/test.h"
#include "test/speed.h"

// ============================================================================================== //

VD_TEST_NAMESPACE_BEGIN();

// ============================================================================================== //

class TestValue;
typedef Core::ValueRegistry<TestValue>	 TestRegistry;
typedef Core::ValueRegistry<Core::Symbol> TestSymbolRegistry;

// ============================================================================================== //

class TestValue
{
public:

	TestValue()
		: m_Value(0), m_Id(0)
	{
	
	}

	TestValue(vd::f32 v) 
		: m_Value(v), m_Id(0)
	{
	
	}

	TestValue(const TestValue& other) 
		: m_Value(other.m_Value), m_Id(other.m_Id)
	{
	
	}

	TestValue& operator=(const TestValue& other)
	{
		m_Value = other.m_Value;
		return *this;
	}

	vd::f32 GetValue() const { return m_Value; }
	void SetValue(vd::f32 v) { m_Value = v; }

	vd::u32 GetId() const { return m_Id; }
	void SetId(vd::u32 v) { m_Id = v; }
	
protected:

	vd::f32 m_Value;
	vd::u32 m_Id;

};

// ============================================================================================== //

class RegistrySpeedTest : public Test::Speed
{
public:
	virtual ~RegistrySpeedTest() {}
	
	vd::f64 GetInsertTime(void) const { return m_InsertTime; }
	vd::f64 GetRetrieveTime(void) const { return m_RetrieveTime; }
	vd::f64 GetClearTime(void) const { return m_ClearTime; }
	
	void RunN(unsigned N)
	{
		TestRegistry registry;
		vd::u32 index[N];
		vd::f32 values[N];
			
		registry.Clear();

		vd::f64 start;
		vd::f64 end;
		
		vd::v3f32 v; 
			
		for(int j=0; j<8; ++j)
		{
			start = Core::Process::GetTimeInSeconds();
			for(unsigned i = 0; i< N; ++i)
			{
				vd::u32 id = registry.Reserve();
				registry.Retrieve(id).SetValue(i);
				index[i] = id;
				values[i] = i;  
			}
			end = Core::Process::GetTimeInSeconds();		
			
			m_InsertTime += (end - start) / N;
			
			start = Core::Process::GetTimeInSeconds();
			for(int k=0; k<100; ++k)
			{
				unsigned ai = registry.Size();
				for(unsigned ei = 0; ei < ai; ei++)
				{
					vd::u32 id = index[ei];
					vd::f32 v = values[ei];

					VD_TEST_EXPECT_EQ(registry.Retrieve(id).GetValue(), v);
				}
			}
			end = Core::Process::GetTimeInSeconds();
	
			m_RetrieveTime += (end - start) / 100.0 * N;
	
			start = Core::Process::GetTimeInSeconds();
			registry.Clear();
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

VD_DEFINE_TEST_WITH_PARAM(RegistrySpeedTest, RunN) 
{
	RunN(GetParam());
	Base::RecordProperty("Iterations", GetIterationCount());
	Base::RecordProperty("InsertTime", GetInsertTime());
	Base::RecordProperty("RetrieveTime", GetRetrieveTime());
	Base::RecordProperty("ClearTime", GetClearTime());
}

// ============================================================================================== //

VD_GENERATE_TEST_WITH_PARAM(RegistrySpeedTestN, RegistrySpeedTest,
	Test::Range(1000, 16384, 1000));

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

