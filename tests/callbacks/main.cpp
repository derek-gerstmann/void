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
#include "core/functions.h"
#include "test/test.h"
#include "test/speed.h"
#include "containers/containers.h"
#include "containers/types.h"

// ============================================================================================== //

VD_TEST_NAMESPACE_BEGIN();

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

class CallbackSpeedTest : public Test::Speed
{
public:
	typedef Core::Function< float (float) > TestCallback;

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
			start = Core::Process::GetTimeInSeconds();
			for(unsigned i = 0; i< N; ++i)
			{
				TestClass* tc = VD_NEW(TestClass);
				TestCallback cb = VD_BIND_MEMBER_FUNCTION(tc, &TestClass::TestMethod);
				bindings.push_back(cb);
				pool.push_back(tc);
			}
			end = Core::Process::GetTimeInSeconds();
	
			m_ConnectTime += (end - start) / N;	
			
			float value = 3.14f;
			start = Core::Process::GetTimeInSeconds();
			for(int k=0; k<100; ++k)
			{
				for(unsigned ei = 0; ei < bindings.size(); ei++)
				{
					value = bindings[ei]( value );
				}
			}
			end = Core::Process::GetTimeInSeconds();	

			m_InvokeTime += (end - start) / 100 * N;	
	
			start = Core::Process::GetTimeInSeconds();
			bindings.clear();
			end = Core::Process::GetTimeInSeconds();		

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
			start = Core::Process::GetTimeInSeconds();
			for(unsigned i = 0; i< N; ++i)
			{
				TestCallback cb = VD_BIND_FUNCTION(StaticTestMethod);
				bindings.push_back(cb);
			}
			end = Core::Process::GetTimeInSeconds();	
	
			m_ConnectTime += (end - start) / N;	
			
			float value = 3.14f;
			start = Core::Process::GetTimeInSeconds();
			for(int k=0; k<100; ++k)
			{
				for(unsigned ei = 0; ei < bindings.size(); ei++)
				{
					value = bindings[ei]( value );
				}
			}
			end = Core::Process::GetTimeInSeconds();

			m_InvokeTime += (end - start) / 100 * N;	
	
			start = Core::Process::GetTimeInSeconds();
			bindings.clear();
			end = Core::Process::GetTimeInSeconds();

			m_DisconnectTime += (end - start) / N;	
		}
	}
	

protected:

	vd::f64 m_ConnectTime;
	vd::f64 m_InvokeTime;
	vd::f64 m_DisconnectTime;

};

// ============================================================================================== //

VD_DEFINE_TEST_WITH_PARAM(CallbackSpeedTest, RunStaticN) 
{
	RunStaticN(GetParam());
	RecordProperty("Iterations", GetIterationCount());
	RecordProperty("ConnectTime", GetConnectTime());
	RecordProperty("InvokeTime", GetInvokeTime());
	RecordProperty("DisconnectTime", GetDisconnectTime());
}

// ============================================================================================== //

VD_DEFINE_TEST_WITH_PARAM(CallbackSpeedTest, RunDynamicN) 
{
	RunDynamicN(GetParam());
	RecordProperty("Iterations", GetIterationCount());
	RecordProperty("ConnectTime", GetConnectTime());
	RecordProperty("InvokeTime", GetInvokeTime());
	RecordProperty("DisconnectTime", GetDisconnectTime());
}

// ============================================================================================== //

VD_GENERATE_TEST_WITH_PARAM(CallbackSpeedTestN, CallbackSpeedTest,
	Test::Range(1000, 100000, 10000));

// ============================================================================================== //

VD_TEST_NAMESPACE_END();

// ============================================================================================== //

VD_IMPORT_MODULE(Test);

// ============================================================================================== //

int main(int argc, char **argv) 
{
	Test::System::Startup(&argc, argv);
	int status = Test::System::RunAllTests();
	Test::System::Shutdown();
	return status;
}

// ============================================================================================== //
