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

#include "vd.h"
#include "core/core.h"
#include "test/test.h"
#include "test/speed.h"

// ============================================================================================== //

VD_TEST_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_DECLARE_ENUM(NumId,
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
	VD_TEST_EXPECT_STR_EQ("Zero", 	NumId::ToString(NumId::Zero));
	VD_TEST_EXPECT_STR_EQ("One",  	NumId::ToString(NumId::One));
	VD_TEST_EXPECT_STR_EQ("Two",  	NumId::ToString(NumId::Two));
	VD_TEST_EXPECT_STR_EQ("Three",  NumId::ToString(NumId::Three));
	VD_TEST_EXPECT_STR_EQ("Four",  	NumId::ToString(NumId::Four));
	VD_TEST_EXPECT_STR_EQ("Five",  	NumId::ToString(NumId::Five));
	VD_TEST_EXPECT_STR_EQ("Six",  	NumId::ToString(NumId::Six));
	VD_TEST_EXPECT_STR_EQ("Seven",  NumId::ToString(NumId::Seven));
	VD_TEST_EXPECT_STR_EQ("Eight",  NumId::ToString(NumId::Eight));
	VD_TEST_EXPECT_STR_EQ("Nine",  	NumId::ToString(NumId::Nine));
	VD_TEST_EXPECT_STR_EQ("Ten",  	NumId::ToString(NumId::Ten));
	VD_TEST_EXPECT_STR_EQ("Eleven", NumId::ToString(NumId::Eleven));
}

// ============================================================================================== //

VD_DEFINE_TEST(EnumTest, EnumFromIntegerTest) 
{
	VD_TEST_EXPECT_EQ(NumId::FromInteger(0), 	NumId::Zero);
	VD_TEST_EXPECT_EQ(NumId::FromInteger(1), 	NumId::One);
	VD_TEST_EXPECT_EQ(NumId::FromInteger(2), 	NumId::Two);
	VD_TEST_EXPECT_EQ(NumId::FromInteger(3), 	NumId::Three);
	VD_TEST_EXPECT_EQ(NumId::FromInteger(4), 	NumId::Four);
	VD_TEST_EXPECT_EQ(NumId::FromInteger(5), 	NumId::Five);
	VD_TEST_EXPECT_EQ(NumId::FromInteger(6), 	NumId::Six);
	VD_TEST_EXPECT_EQ(NumId::FromInteger(7), 	NumId::Seven);
	VD_TEST_EXPECT_EQ(NumId::FromInteger(8), 	NumId::Eight);
	VD_TEST_EXPECT_EQ(NumId::FromInteger(9), 	NumId::Nine);
	VD_TEST_EXPECT_EQ(NumId::FromInteger(10), 	NumId::Ten);
	VD_TEST_EXPECT_EQ(NumId::FromInteger(11), 	NumId::Eleven);
}

// ============================================================================================== //

VD_DEFINE_TEST(EnumTest, EnumToIntegerTest) 
{
	VD_TEST_EXPECT_EQ(0, 	NumId::ToInteger(NumId::Zero));
	VD_TEST_EXPECT_EQ(1, 	NumId::ToInteger(NumId::One));
	VD_TEST_EXPECT_EQ(2, 	NumId::ToInteger(NumId::Two));
	VD_TEST_EXPECT_EQ(3, 	NumId::ToInteger(NumId::Three));
	VD_TEST_EXPECT_EQ(4, 	NumId::ToInteger(NumId::Four));
	VD_TEST_EXPECT_EQ(5, 	NumId::ToInteger(NumId::Five));
	VD_TEST_EXPECT_EQ(6, 	NumId::ToInteger(NumId::Six));
	VD_TEST_EXPECT_EQ(7, 	NumId::ToInteger(NumId::Seven));
	VD_TEST_EXPECT_EQ(8, 	NumId::ToInteger(NumId::Eight));
	VD_TEST_EXPECT_EQ(9, 	NumId::ToInteger(NumId::Nine));
	VD_TEST_EXPECT_EQ(10, 	NumId::ToInteger(NumId::Ten));
	VD_TEST_EXPECT_EQ(11, 	NumId::ToInteger(NumId::Eleven));
}

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
