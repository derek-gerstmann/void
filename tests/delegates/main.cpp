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
#include "core/delegates.h"
#include "core/framework.h"

// ============================================================================================== //

using vd::core::Delegate;
using vd::core::DelegateThread;
using vd::core::SpeedTest;

// ============================================================================================== //

float 
StaticTestMethod( float args )
{
	args += args;
	return args;
}

// ============================================================================================== //

class SerialDelegateSpeedTest : public SpeedTest
{
public:

	typedef Delegate<float (float)> TestDelegate;
	typedef DelegateThread< TestDelegate > TestDelegateThread;

	vd::f64 GetConnectTime(void) const { return m_ConnectTime; }
	vd::f64 GetInvokeTime(void) const { return m_InvokeTime; }
	vd::f64 GetDisconnectTime(void) const { return m_DisconnectTime; }
	
	void RunN(int N)
	{
		vd::f64 start;
		vd::f64 end;

		typedef Delegate<float (float)> TestDelegate;
		std::vector<TestDelegate> delegates;	
		for(int j=0; j<8; ++j)
		{
			float value = 3.14f;
			start = vd::core::System::GetTimeInSeconds();
			for(int i = 0; i< N; ++i)
			{
				TestDelegate::Function cb = VD_BIND_FUNCTION(StaticTestMethod);
				TestDelegate dg(cb, value);
				delegates.push_back(dg);
				value += value;
			}
			end = vd::core::System::GetTimeInSeconds();		
	
			m_ConnectTime += (end - start) / N;	
			
			start = vd::core::System::GetTimeInSeconds();	
			for(int k=0; k<100; ++k)
			{
				for(int ei = 0; ei < (int)delegates.size(); ei++)
				{
					delegates[ei]();
				}
			}
			end = vd::core::System::GetTimeInSeconds();		

			m_InvokeTime += (end - start) / 100 * N;	
	
			start = vd::core::System::GetTimeInSeconds();	
			delegates.clear();
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

class ParallelDelegateSpeedTest : public SpeedTest
{
public:

	typedef Delegate<float (float)> TestDelegate;
	typedef DelegateThread< TestDelegate > TestDelegateThread;

	vd::f64 GetConnectTime(void) const { return m_ConnectTime; }
	vd::f64 GetInvokeTime(void) const { return m_InvokeTime; }
	vd::f64 GetDisconnectTime(void) const { return m_DisconnectTime; }
	
	void RunN(int N)
	{
		vd::f64 start;
		vd::f64 end;

		TestDelegate::Function callback = VD_BIND_FUNCTION(StaticTestMethod);
	
		std::vector< vd::core::Handle<TestDelegateThread> > threads;
	
		for(int j=0; j<8; ++j)
		{
			float value = 3.14f;
			start = vd::core::System::GetTimeInSeconds();
			for(int i = 0; i < N; ++i)
			{
				TestDelegateThread::DelegateType* delegate = VD_NEW(TestDelegateThread::DelegateType, callback, value);
				TestDelegateThread* thread  = VD_NEW(TestDelegateThread, delegate);
				threads.push_back(thread);
				value += value;
			}
			end = vd::core::System::GetTimeInSeconds();		
	
			m_ConnectTime += (end - start) / N;	
			
			start = vd::core::System::GetTimeInSeconds();	
			for(int h = 0; h < (int)threads.size(); ++h)
			{
				threads[h]->Setup();
				threads[h]->Start();
			}
			end = vd::core::System::GetTimeInSeconds();		

			m_InvokeTime += (end - start) / N;	
	
			bool more = true;
			while(more)
			{
				vd::core::Thread::Yield();
				more = false;
				for(int h = 0; h < (int)threads.size(); ++h)
				{
					TestDelegateThread* t = threads[h];
					if(t && t->GetDelegate() && t->GetDelegate()->IsReady() == false)
						more = true;
				}
			}
	
			start = vd::core::System::GetTimeInSeconds();	
			for(int h = 0; h < (int)threads.size(); ++h)
			{
				TestDelegateThread* t = threads[h];
				t->Join();
			}
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

TEST_P(SerialDelegateSpeedTest, RunN) 
{
	RunN(GetParam());
	RecordProperty("Iterations", GetIterationCount());
	RecordProperty("ConnectTime", GetConnectTime());
	RecordProperty("InvokeTime", GetInvokeTime());
	RecordProperty("DisconnectTime", GetDisconnectTime());
}

// ============================================================================================== //

TEST_P(ParallelDelegateSpeedTest, RunN) 
{
	RunN(GetParam());
	RecordProperty("Iterations", GetIterationCount());
	RecordProperty("ConnectTime", GetConnectTime());
	RecordProperty("InvokeTime", GetInvokeTime());
	RecordProperty("DisconnectTime", GetDisconnectTime());
}

// ============================================================================================== //

INSTANTIATE_TEST_CASE_P(SerialDelegateSpeedTestN, SerialDelegateSpeedTest,
                        ::testing::Range(1000, 100000, 10000));

INSTANTIATE_TEST_CASE_P(ParallelDelegateSpeedTestN, ParallelDelegateSpeedTest,
                        ::testing::Range(1, 8, 1));

// ============================================================================================== //

