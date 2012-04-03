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
#include "core/delegates.h"
#include "test/test.h"
#include "test/speed.h"
#include "containers/containers.h"
#include "containers/types.h"

// ============================================================================================== //

VD_TEST_NAMESPACE_BEGIN();

// ============================================================================================== //

float 
StaticTestMethod( float args )
{
	args += args;
	return args;
}

// ============================================================================================== //

class SerialDelegateSpeedTest : public Test::Speed
{
public:

	typedef Core::Delegate<float (float)> 			TestDelegate;
	typedef Core::DelegateThread< TestDelegate > 	TestDelegateThread;

	vd::f64 GetConnectTime(void) const { return m_ConnectTime; }
	vd::f64 GetInvokeTime(void) const { return m_InvokeTime; }
	vd::f64 GetDisconnectTime(void) const { return m_DisconnectTime; }
	
	void RunN(int N)
	{
		vd::f64 start;
		vd::f64 end;

		std::vector<TestDelegate> delegates;	
		for(int j=0; j<8; ++j)
		{
			float value = 3.14f;
			start = Core::Process::GetTimeInSeconds();
			for(int i = 0; i< N; ++i)
			{
				TestDelegate::FunctionType cb = VD_BIND_FUNCTION(StaticTestMethod);
				TestDelegate dg(cb, value);
				delegates.push_back(dg);
				value += value;
			}
			end = Core::Process::GetTimeInSeconds();
	
			m_ConnectTime += (end - start) / N;	
			
			start = Core::Process::GetTimeInSeconds();
			for(int k=0; k<100; ++k)
			{
				for(int ei = 0; ei < (int)delegates.size(); ei++)
				{
					delegates[ei]();
				}
			}
			end = Core::Process::GetTimeInSeconds();

			m_InvokeTime += (end - start) / 100 * N;	
	
			start = Core::Process::GetTimeInSeconds();
			delegates.clear();
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

class ParallelDelegateSpeedTest : public Test::Speed
{
public:

	typedef Core::Delegate<float (float)> 			TestDelegate;
	typedef Core::DelegateThread< TestDelegate > 	TestDelegateThread;

	vd::f64 GetConnectTime(void) const { return m_ConnectTime; }
	vd::f64 GetInvokeTime(void) const { return m_InvokeTime; }
	vd::f64 GetDisconnectTime(void) const { return m_DisconnectTime; }
	
	void RunN(int N)
	{
		vd::f64 start;
		vd::f64 end;

		TestDelegate::FunctionType callback = VD_BIND_FUNCTION(StaticTestMethod);
	
		std::vector< Core::Handle<TestDelegateThread> > threads;
	
		for(int j=0; j<8; ++j)
		{
			float value = 3.14f;
			start = Core::Process::GetTimeInSeconds();
			for(int i = 0; i < N; ++i)
			{
				TestDelegateThread::DelegateType* delegate = VD_NEW(TestDelegateThread::DelegateType, callback, value);
				TestDelegateThread* thread  = VD_NEW(TestDelegateThread, delegate);
				threads.push_back(thread);
				value += value;
			}
			end = Core::Process::GetTimeInSeconds();	
	
			m_ConnectTime += (end - start) / N;	
			
			start = Core::Process::GetTimeInSeconds();	
			for(int h = 0; h < (int)threads.size(); ++h)
			{
				threads[h]->Setup();
				threads[h]->Start();
			}
			end = Core::Process::GetTimeInSeconds();

			m_InvokeTime += (end - start) / N;	
	
			bool more = true;
			while(more)
			{
				Core::Thread::Yield();
				more = false;
				for(int h = 0; h < (int)threads.size(); ++h)
				{
					TestDelegateThread* t = threads[h];
					if(t && t->GetDelegate() && t->GetDelegate()->IsReady() == false)
						more = true;
				}
			}
	
			start = Core::Process::GetTimeInSeconds();	
			for(int h = 0; h < (int)threads.size(); ++h)
			{
				TestDelegateThread* t = threads[h];
				t->Join();
			}
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

VD_DEFINE_TEST_WITH_PARAM(SerialDelegateSpeedTest, RunN) 
{
	RunN(GetParam());
	RecordProperty("Iterations", GetIterationCount());
	RecordProperty("ConnectTime", GetConnectTime());
	RecordProperty("InvokeTime", GetInvokeTime());
	RecordProperty("DisconnectTime", GetDisconnectTime());
}

// ============================================================================================== //

VD_DEFINE_TEST_WITH_PARAM(ParallelDelegateSpeedTest, RunN) 
{
	RunN(GetParam());
	RecordProperty("Iterations", GetIterationCount());
	RecordProperty("ConnectTime", GetConnectTime());
	RecordProperty("InvokeTime", GetInvokeTime());
	RecordProperty("DisconnectTime", GetDisconnectTime());
}

// ============================================================================================== //

VD_GENERATE_TEST_WITH_PARAM(SerialDelegateSpeedTestN, SerialDelegateSpeedTest,
	Test::Range(1000, 100000, 10000));

VD_GENERATE_TEST_WITH_PARAM(ParallelDelegateSpeedTestN, ParallelDelegateSpeedTest,
	Test::Range(1, 8, 1));

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

