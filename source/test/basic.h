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
#include "core/system.h"
#include "core/process.h"

// ============================================================================================== //

VD_TEST_NAMESPACE_BEGIN();

// ============================================================================================== //

class Basic : public Test::Base
{

public:

    static const char* GetTestName()
    {
        const Test::Info* const test_info = Unit::GetInstance()->current_test_info();
        return test_info ? test_info->name() : "<UNKNOWN>";
    }

    static const char* GetTestCaseName()
    {
        const Test::Info* const test_info = Unit::GetInstance()->current_test_info();
        return test_info ? test_info->test_case_name() : "<UNKNOWN>";
    }

protected:

    Basic()
    {
        // Do set-up work for each test here.
    }

    virtual ~Basic()
    {
        // Do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:
    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    virtual void SetUp()
    {
        // Code here will be called immediately after the constructor (right
        // before each test).

        Core::System::Startup();
        m_StartTime = Core::Process::GetTimeInSeconds();
    }

    virtual void TearDown()
    {
        // Code here will be called immediately after each test (right
        // before the destructor).

        m_EndTime = Core::Process::GetTimeInSeconds();
        vd::f64 dt = m_EndTime - m_StartTime;

        vdLogGlobalInfo("Test '%s/%s' completed in '%f' sec!",
                        Basic::GetTestName(), Basic::GetTestCaseName(), dt);

        Core::System::Shutdown();
    }

    vd::f64 m_StartTime;
    vd::f64 m_EndTime;
};

// ============================================================================================== //

VD_TEST_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_TEST_BASIC_INCLUDED

