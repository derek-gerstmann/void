// ============================================================================================== //
//
// License:     The Lesser GNU Public License (LGPL) v3.0.
// 
// Author(s):   Derek Gerstmann. The University of Western Australia (UWA). 
//              As well as the shoulders of many giants...
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
#include "test/test.h"
#include "test/speed.h"

// ============================================================================================== //

VD_TEST_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_DEFINE_TEST(CoreTest, CreateMetaclass) 
{
	Core::MetaClass meta(vd::symbol(vd::uid(0,0), "Test"), vd::symbol(vd::uid(0,0), "Object"), true);
	VD_TEST_EXPECT_STR_EQ("Test", meta.GetIdentifier().ToString());
}

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
