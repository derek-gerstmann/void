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
#include "core/shared.h"
#include "core/framework.h"

// ============================================================================================== //

using vd::core::Function;
using vd::core::SpeedTest;

// ============================================================================================== //

class TestClass 
{

public:
	float TestMethod( float args )
	{
		args += args;
		return args;
	}
};

// ============================================================================================== //

float 
StaticTestMethod( float args )
{
	args += args;
	return args;
}

// ============================================================================================== //

class CallbackSpeedTest : public SpeedTest
{
public:
	typedef Function< float (float) > TestCallback;

	vd::f64 GetConnectTime(void) const { return m_ConnectTime; }
	vd::f64 GetInvokeTime(void) const { return m_InvokeTime; }
	vd::f64 GetDisconnectTime(void) const { return m_DisconnectTime; }
	
	void RunDynamicN(unsigned N)
	{
		vd::f64 start;
		vd::f64 end;

		std::vector<TestCallback> bindings;
		std::vector<TestClass*> pool;
	
		for(int j=0; j<8; ++j)
		{
			start = vd::core::System::GetTimeInSeconds();
			for(unsigned i = 0; i< N; ++i)
			{
				TestClass* tc = VD_NEW(TestClass);
				TestCallback cb = VD_BIND_MEMBER_FUNCTION(tc, &TestClass::TestMethod);
				bindings.push_back(cb);
				pool.push_back(tc);
			}
			end = vd::core::System::GetTimeInSeconds();		
	
			m_ConnectTime += (end - start) / N;	
			
			float value = 3.14f;
			start = vd::core::System::GetTimeInSeconds();	
			for(int k=0; k<100; ++k)
			{
				for(unsigned ei = 0; ei < bindings.size(); ei++)
				{
					value = bindings[ei]( value );
				}
			}
			end = vd::core::System::GetTimeInSeconds();		

			m_InvokeTime += (end - start) / 100 * N;	
	
			start = vd::core::System::GetTimeInSeconds();	
			bindings.clear();
			end = vd::core::System::GetTimeInSeconds();		

			for(unsigned p = 0; p < pool.size(); p++)
			{
				TestClass* tc = pool[p];
				VD_DELETE(tc);
			}
			pool.clear();
			
			m_DisconnectTime += (end - start) / N;	
		}
	}

	void RunStaticN(unsigned N)
	{
		vd::f64 start;
		vd::f64 end;

		std::vector<TestCallback> bindings;	
		for(int j=0; j<8; ++j)
		{
			start = vd::core::System::GetTimeInSeconds();
			for(unsigned i = 0; i< N; ++i)
			{
				TestCallback cb = VD_BIND_FUNCTION(StaticTestMethod);
				bindings.push_back(cb);
			}
			end = vd::core::System::GetTimeInSeconds();		
	
			m_ConnectTime += (end - start) / N;	
			
			float value = 3.14f;
			start = vd::core::System::GetTimeInSeconds();	
			for(int k=0; k<100; ++k)
			{
				for(unsigned ei = 0; ei < bindings.size(); ei++)
				{
					value = bindings[ei]( value );
				}
			}
			end = vd::core::System::GetTimeInSeconds();		

			m_InvokeTime += (end - start) / 100 * N;	
	
			start = vd::core::System::GetTimeInSeconds();	
			bindings.clear();
			end = vd::core::System::GetTimeInSeconds();		

			m_DisconnectTime += (end - start) / N;	
		}
	}
	

protected:

	vd::f64 m_ConnectTime;
	vd::f64 m_InvokeTime;
	vd::f64 m_DisconnectTime;

};

// ============================================================================================== //

TEST_P(CallbackSpeedTest, RunStaticN) 
{
	RunStaticN(GetParam());
	RecordProperty("Iterations", GetIterationCount());
	RecordProperty("ConnectTime", GetConnectTime());
	RecordProperty("InvokeTime", GetInvokeTime());
	RecordProperty("DisconnectTime", GetDisconnectTime());
}

// ============================================================================================== //

TEST_P(CallbackSpeedTest, RunDynamicN) 
{
	RunDynamicN(GetParam());
	RecordProperty("Iterations", GetIterationCount());
	RecordProperty("ConnectTime", GetConnectTime());
	RecordProperty("InvokeTime", GetInvokeTime());
	RecordProperty("DisconnectTime", GetDisconnectTime());
}

// ============================================================================================== //

INSTANTIATE_TEST_CASE_P(CallbackSpeedTestN, CallbackSpeedTest,
                        ::testing::Range(1000, 100000, 10000));

// ============================================================================================== //

