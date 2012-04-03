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

VD_DECLARE_ENUM(TestEnum,
	Zero,
	One,
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	Ten,
	Eleven);

// ============================================================================================== //

VD_DEFINE_TEST(EnumTest, EnumToStringTest) 
{
	VD_TEST_EXPECT_STR_EQ("Zero", 	TestEnum::ToString(TestEnum::Zero));
	VD_TEST_EXPECT_STR_EQ("One",  	TestEnum::ToString(TestEnum::One));
	VD_TEST_EXPECT_STR_EQ("Two",  	TestEnum::ToString(TestEnum::Two));
	VD_TEST_EXPECT_STR_EQ("Three",  TestEnum::ToString(TestEnum::Three));
	VD_TEST_EXPECT_STR_EQ("Four",  	TestEnum::ToString(TestEnum::Four));
	VD_TEST_EXPECT_STR_EQ("Five",  	TestEnum::ToString(TestEnum::Five));
	VD_TEST_EXPECT_STR_EQ("Six",  	TestEnum::ToString(TestEnum::Six));
	VD_TEST_EXPECT_STR_EQ("Seven",  TestEnum::ToString(TestEnum::Seven));
	VD_TEST_EXPECT_STR_EQ("Eight",  TestEnum::ToString(TestEnum::Eight));
	VD_TEST_EXPECT_STR_EQ("Nine",  	TestEnum::ToString(TestEnum::Nine));
	VD_TEST_EXPECT_STR_EQ("Ten",  	TestEnum::ToString(TestEnum::Ten));
	VD_TEST_EXPECT_STR_EQ("Eleven", TestEnum::ToString(TestEnum::Eleven));
}

// ============================================================================================== //

VD_DEFINE_TEST(EnumTest, EnumFromIntegerTest) 
{
	VD_TEST_EXPECT_EQ(TestEnum::FromInteger(0), 	TestEnum::Zero);
	VD_TEST_EXPECT_EQ(TestEnum::FromInteger(1), 	TestEnum::One);
	VD_TEST_EXPECT_EQ(TestEnum::FromInteger(2), 	TestEnum::Two);
	VD_TEST_EXPECT_EQ(TestEnum::FromInteger(3), 	TestEnum::Three);
	VD_TEST_EXPECT_EQ(TestEnum::FromInteger(4), 	TestEnum::Four);
	VD_TEST_EXPECT_EQ(TestEnum::FromInteger(5), 	TestEnum::Five);
	VD_TEST_EXPECT_EQ(TestEnum::FromInteger(6), 	TestEnum::Six);
	VD_TEST_EXPECT_EQ(TestEnum::FromInteger(7), 	TestEnum::Seven);
	VD_TEST_EXPECT_EQ(TestEnum::FromInteger(8), 	TestEnum::Eight);
	VD_TEST_EXPECT_EQ(TestEnum::FromInteger(9), 	TestEnum::Nine);
	VD_TEST_EXPECT_EQ(TestEnum::FromInteger(10), 	TestEnum::Ten);
	VD_TEST_EXPECT_EQ(TestEnum::FromInteger(11), 	TestEnum::Eleven);
}

// ============================================================================================== //

VD_DEFINE_TEST(EnumTest, EnumToIntegerTest) 
{
	VD_TEST_EXPECT_EQ(0, 	TestEnum::ToInteger(TestEnum::Zero));
	VD_TEST_EXPECT_EQ(1, 	TestEnum::ToInteger(TestEnum::One));
	VD_TEST_EXPECT_EQ(2, 	TestEnum::ToInteger(TestEnum::Two));
	VD_TEST_EXPECT_EQ(3, 	TestEnum::ToInteger(TestEnum::Three));
	VD_TEST_EXPECT_EQ(4, 	TestEnum::ToInteger(TestEnum::Four));
	VD_TEST_EXPECT_EQ(5, 	TestEnum::ToInteger(TestEnum::Five));
	VD_TEST_EXPECT_EQ(6, 	TestEnum::ToInteger(TestEnum::Six));
	VD_TEST_EXPECT_EQ(7, 	TestEnum::ToInteger(TestEnum::Seven));
	VD_TEST_EXPECT_EQ(8, 	TestEnum::ToInteger(TestEnum::Eight));
	VD_TEST_EXPECT_EQ(9, 	TestEnum::ToInteger(TestEnum::Nine));
	VD_TEST_EXPECT_EQ(10, 	TestEnum::ToInteger(TestEnum::Ten));
	VD_TEST_EXPECT_EQ(11, 	TestEnum::ToInteger(TestEnum::Eleven));
}

// ============================================================================================== //

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
