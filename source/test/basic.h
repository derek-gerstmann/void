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

#ifndef VD_TEST_BASIC_INCLUDED
#define VD_TEST_BASIC_INCLUDED

// ============================================================================================== //

#include "test/test.h"

// ============================================================================================== //

VD_TEST_NAMESPACE_BEGIN();

// ============================================================================================== //

// The fixture for testing class Foo.
class BasicTest : public Test::Base 
{

public:

  static const char* GetTestName() {
	const ::testing::TestInfo* const test_info = ::testing::UnitTest::GetInstance()->current_test_info();
	return test_info ? test_info->name() : "<UNKNOWN>";
  }
  
  static const char* GetTestCaseName() {
	const ::testing::TestInfo* const test_info = ::testing::UnitTest::GetInstance()->current_test_info();
	return test_info ? test_info->test_case_name() : "<UNKNOWN>";
  }
  
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  BasicTest() {
    // You can do set-up work for each test here.
  }

  virtual ~BasicTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
	System::Startup();
	m_StartTime = Process::GetTimeInSeconds();
  }

  virtual void TearDown() {
	
	m_EndTime = Process::GetTimeInSeconds();
	vd::f64 dt = m_EndTime - m_StartTime;
	
	vdLogGlobalInfo("Test '%s/%s' completed in '%f' sec!",
		BasicTest::GetTestName(), BasicTest::GetTestCaseName(), dt);

	System::Shutdown();

    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  vd::f64 m_StartTime;
  vd::f64 m_EndTime;
};

// ============================================================================================== //

class SpeedTest : public ::testing::TestWithParam<int> 
{

public:

  static const char* GetTestName() {
	const ::testing::TestInfo* const test_info = ::testing::UnitTest::GetInstance()->current_test_info();
	return test_info ? test_info->name() : "<UNKNOWN>";
  }
  
  static const char* GetTestCaseName() {
	const ::testing::TestInfo* const test_info = ::testing::UnitTest::GetInstance()->current_test_info();
	return test_info ? test_info->test_case_name() : "<UNKNOWN>";
  }
  
  int GetIterationCount() {
  	return GetParam();
  }
  
protected:
  // You can implement all the usual fixture class members here.
  // To access the test parameter, call GetParam() from class
  // TestWithParam<T>.
  // You can remove any or all of the following functions if its body
  // is empty.

  SpeedTest() {
    // You can do set-up work for each test here.
  }

  virtual ~SpeedTest() 
  {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() 
  {
    // Code here will be called immediately after the constructor (right
    // before each test).
    
	System::Startup();
	m_StartTime = Process::GetTimeInSeconds();
  }

  virtual void TearDown() 
  {
	
	m_EndTime = Process::GetTimeInSeconds();
//	vd::f64 dt = m_EndTime - m_StartTime;
//	vd::i32 count = GetIterationCount();
//	vdLogGlobalInfo("Test '%s/%s' completed '%d' iterations in '%f' sec ('%f' ave sec per iteration)!",
//		SpeedTest::GetTestName(), SpeedTest::GetTestCaseName(), 
//		count, dt, dt / count);

	System::Shutdown();

    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  vd::f64 m_StartTime;
  vd::f64 m_EndTime;
};

// // Tests that the Foo::Bar() method does Abc.
// TEST_F(FooTest, MethodBarDoesAbc) {
//   const string input_filepath = "this/package/testdata/myinputfile.dat";
//   const string output_filepath = "this/package/testdata/myoutputfile.dat";
//   Foo f;
//   EXPECT_EQ(0, f.Bar(input_filepath, output_filepath));
// }
// 
// // Tests that Foo does Xyz.
// TEST_F(FooTest, DoesXyz) {
//   // Exercises the Xyz feature of Foo.
// }
// 
// }  // namespace
// 
// int main(int argc, char **argv) {
//   ::testing::InitGoogleTest(&argc, argv);
//   return RUN_ALL_TESTS();
// }

// ============================================================================================== //

VD_TEST_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_TEST_BASIC_INCLUDED

