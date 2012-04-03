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

#if 0

#include "core/test.h"
#include "core/framework.h"
#include "core/symbol.h"

// ============================================================================================== //

using vd::core::BasicTest;
using vd::core::SpeedTest;
using vd::core::Symbol;

// ============================================================================================== //

class SymbolRegistrySpeedTest : public SpeedTest
{
public:

	vd::f64 GetInsertTime(void) const { return m_InsertTime; }
	vd::f64 GetRetrieveTime(void) const { return m_RetrieveTime; }
	
	void RunN(unsigned N)
	{
		vd::u32* index = VD_NEW_ARRAY(vd::u32, N);
		char** names = VD_NEW_ARRAY(char*, N);
		
		vd::f64 start;
		vd::f64 end;
		
		for(unsigned i = 0; i< N; ++i)
		{
			char buffer[128] = {0};
			sprintf(buffer, "%d", (int)i);
			names[i] = VD_NEW_ARRAY(char, sizeof(buffer));
			vd::core::Memory::MemCopy(names[i], buffer, sizeof(buffer));
		}
		
		for(int j=0; j<8; ++j)
		{
			start = vd::core::System::GetTimeInSeconds();
			for(unsigned i = 0; i< N; ++i)
			{
				index[i] = Symbol::Register(names[i]);
			}
			end = vd::core::System::GetTimeInSeconds();		
			m_InsertTime += (end - start) / N;

			start = vd::core::System::GetTimeInSeconds();
			for(int k=0; k<100; ++k)
			{
				for(unsigned r = 0; r < N; ++r)
				{
					Symbol s = Symbol::Retrieve(index[r]);
				}
			}
			end = vd::core::System::GetTimeInSeconds();		
			m_RetrieveTime += (end - start) / 100.0 * N;

			for(unsigned r = 0; r < N; ++r)
			{
				Symbol s = Symbol::Retrieve(index[r]);
				EXPECT_STREQ(s.GetStr(), names[r]);
			}
		}
		
		for(unsigned i = 0; i< N; ++i)
		{
			VD_DELETE_ARRAY(names[i]);
		}
		
		VD_DELETE_ARRAY(index);
		VD_DELETE_ARRAY(names);
	}

protected:

	vd::f64 m_InsertTime;
	vd::f64 m_RetrieveTime;

};

// ============================================================================================== //

TEST_P(SymbolRegistrySpeedTest, RunN) 
{
	RunN(GetParam());
	RecordProperty("Iterations", GetIterationCount());
	RecordProperty("InsertTime", GetInsertTime());
	RecordProperty("RetrieveTime", GetRetrieveTime());
}

// ============================================================================================== //

INSTANTIATE_TEST_CASE_P(SymbolRegistrySpeedTestN, SymbolRegistrySpeedTest,
                        ::testing::Range(0, 32*1024, 16));

// ============================================================================================== //

#endif
