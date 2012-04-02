// ============================================================================================== //
//
// License:   The Lesser GNU Public License (LGPL) v3.0.
// 
// Author(s):   Derek Gerstmann. The University of Western Australia (UWA). 
//        As well as the shoulders of many giants...
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

#ifndef VD_TEST_MACROS_INCLUDED
#define VD_TEST_MACROS_INCLUDED

// ============================================================================================== //

#define VD_DEFINE_TEST(test_case_name, test_name) \
    TEST(test_case_name, test_name)

#define VD_DEFINE_TEST_FOR_FIXTURE(test_fixture, test_name) \
    TEST_F(test_fixture, test_name)

#define VD_DEFINE_TEST_WITH_PARAM(test_case_name, test_name) \
    TEST_P(test_case_name, test_name)

// ============================================================================================== //

// Use this macro in main() to run all tests.  It returns 0 if all tests are successful, or 1 otherwise.
//
// RUN_ALL_TESTS() should be invoked after the command line has been parsed by InitGoogleTest().

#define VD_RUN_ALL_TESTS() \
    RUN_ALL_TESTS()

#define VD_DECLARE_TEST_FRIEND(test_case_name, test_name) \
    FRIEND_TEST(test_case_name, test_name)

#define VD_GENERATE_TEST_WITH_PARAM(prefix, test_case_name, generator) \
    INSTANTIATE_TEST_CASE_P(prefix, test_case_name, generator)

// ============================================================================================== //

// The 'Types' template argument below must have spaces around it
// since some compilers may choke on '>>' when passing a template
// instance (e.g. Types<int>)

#define VD_DECLARE_TYPED_TEST_CASE(CaseName, Types) \
    TYPED_TEST_CASE(CaseName, Types)

#define VD_DECLARE_TYPED_TEST(CaseName, TestName) \
    TYPED_TEST(CaseName, TestName)

// ============================================================================================== //

// Macros for indicating success/failure in test code.

// ADD_FAILURE unconditionally adds a failure to the current test.
// SUCCEED generates a success - it doesn't automatically make the
// current test successful, as a test is only successful when it has
// no failure.
//
// EXPECT_* verifies that a certain condition is satisfied.  If not,
// it behaves like ADD_FAILURE.  In particular:
//
//   EXPECT_TRUE  verifies that a Boolean condition is true.
//   EXPECT_FALSE verifies that a Boolean condition is false.
//
// FAIL and ASSERT_* are similar to ADD_FAILURE and EXPECT_*, except
// that they will also abort the current function on failure.  People
// usually want the fail-fast behavior of FAIL and ASSERT_*, but those
// writing data-driven tests often find themselves using ADD_FAILURE
// and EXPECT_* more.

// Generates a nonfatal failure with a generic message.
#define ADD_FAILURE() GTEST_NONFATAL_FAILURE_("Failed")

// Generates a nonfatal failure at the given source file location with
// a generic message.
#define ADD_FAILURE_AT(file, line) \
  GTEST_MESSAGE_AT_(file, line, "Failed", \
                    ::testing::TestPartResult::kNonFatalFailure)

// Generates a fatal failure with a generic message.
#define GTEST_FAIL() GTEST_FATAL_FAILURE_("Failed")

// Define this macro to 1 to omit the definition of FAIL(), which is a
// generic name and clashes with some other libraries.
#if !GTEST_DONT_DEFINE_FAIL
# define FAIL() GTEST_FAIL()
#endif

// Generates a success with a generic message.
#define GTEST_SUCCEED() GTEST_SUCCESS_("Succeeded")

// Define this macro to 1 to omit the definition of SUCCEED(), which
// is a generic name and clashes with some other libraries.
#if !GTEST_DONT_DEFINE_SUCCEED
# define SUCCEED() GTEST_SUCCEED()
#endif

// Macros for testing exceptions.
//
//    * {ASSERT|EXPECT}_THROW(statement, expected_exception):
//         Tests that the statement throws the expected exception.
//    * {ASSERT|EXPECT}_NO_THROW(statement):
//         Tests that the statement doesn't throw any exception.
//    * {ASSERT|EXPECT}_ANY_THROW(statement):
//         Tests that the statement throws an exception.

#define EXPECT_THROW(statement, expected_exception) \
  GTEST_TEST_THROW_(statement, expected_exception, GTEST_NONFATAL_FAILURE_)
#define EXPECT_NO_THROW(statement) \
  GTEST_TEST_NO_THROW_(statement, GTEST_NONFATAL_FAILURE_)
#define EXPECT_ANY_THROW(statement) \
  GTEST_TEST_ANY_THROW_(statement, GTEST_NONFATAL_FAILURE_)
#define ASSERT_THROW(statement, expected_exception) \
  GTEST_TEST_THROW_(statement, expected_exception, GTEST_FATAL_FAILURE_)
#define ASSERT_NO_THROW(statement) \
  GTEST_TEST_NO_THROW_(statement, GTEST_FATAL_FAILURE_)
#define ASSERT_ANY_THROW(statement) \
  GTEST_TEST_ANY_THROW_(statement, GTEST_FATAL_FAILURE_)

// Boolean assertions. Condition can be either a Boolean expression or an
// AssertionResult. For more information on how to use AssertionResult with
// these macros see comments on that class.
#define EXPECT_TRUE(condition) \
  GTEST_TEST_BOOLEAN_(condition, #condition, false, true, \
                      GTEST_NONFATAL_FAILURE_)
#define EXPECT_FALSE(condition) \
  GTEST_TEST_BOOLEAN_(!(condition), #condition, true, false, \
                      GTEST_NONFATAL_FAILURE_)
#define ASSERT_TRUE(condition) \
  GTEST_TEST_BOOLEAN_(condition, #condition, false, true, \
                      GTEST_FATAL_FAILURE_)
#define ASSERT_FALSE(condition) \
  GTEST_TEST_BOOLEAN_(!(condition), #condition, true, false, \
                      GTEST_FATAL_FAILURE_)

// Includes the auto-generated header that implements a family of
// generic predicate assertion macros.
#include "gtest/gtest_pred_impl.h"

// Macros for testing equalities and inequalities.
//
//    * {ASSERT|EXPECT}_EQ(expected, actual): Tests that expected == actual
//    * {ASSERT|EXPECT}_NE(v1, v2):           Tests that v1 != v2
//    * {ASSERT|EXPECT}_LT(v1, v2):           Tests that v1 < v2
//    * {ASSERT|EXPECT}_LE(v1, v2):           Tests that v1 <= v2
//    * {ASSERT|EXPECT}_GT(v1, v2):           Tests that v1 > v2
//    * {ASSERT|EXPECT}_GE(v1, v2):           Tests that v1 >= v2
//
// When they are not, Google Test prints both the tested expressions and
// their actual values.  The values must be compatible built-in types,
// or you will get a compiler error.  By "compatible" we mean that the
// values can be compared by the respective operator.
//
// Note:
//
//   1. It is possible to make a user-defined type work with
//   {ASSERT|EXPECT}_??(), but that requires overloading the
//   comparison operators and is thus discouraged by the Google C++
//   Usage Guide.  Therefore, you are advised to use the
//   {ASSERT|EXPECT}_TRUE() macro to assert that two objects are
//   equal.
//
//   2. The {ASSERT|EXPECT}_??() macros do pointer comparisons on
//   pointers (in particular, C strings).  Therefore, if you use it
//   with two C strings, you are testing how their locations in memory
//   are related, not how their content is related.  To compare two C
//   strings by content, use {ASSERT|EXPECT}_STR*().
//
//   3. {ASSERT|EXPECT}_EQ(expected, actual) is preferred to
//   {ASSERT|EXPECT}_TRUE(expected == actual), as the former tells you
//   what the actual value is when it fails, and similarly for the
//   other comparisons.
//
//   4. Do not depend on the order in which {ASSERT|EXPECT}_??()
//   evaluate their arguments, which is undefined.
//
//   5. These macros evaluate their arguments exactly once.
//
// Examples:
//
//   EXPECT_NE(5, Foo());
//   EXPECT_EQ(NULL, a_pointer);
//   ASSERT_LT(i, array_size);
//   ASSERT_GT(records.size(), 0) << "There is no record left.";

#define VD_TEST_EXPECT_EQ(expected, actual) \
    EXPECT_EQ(expected, actual) 

#define VD_TEST_EXPECT_NE(expected, actual) \
    EXPECT_NE(expected, actual)

#define VD_TEST_EXPECT_LE(val1, val2) \
    EXPECT_LE(val1, val2) 

#define VD_TEST_EXPECT_LT(val1, val2) \
    EXPECT_LT(val1, val2)

#define VD_TEST_EXPECT_GE(val1, val2) \
    EXPECT_GE(val1, val2)

#define VD_TEST_EXPECT_GT(val1, val2) \
    EXPECT_GT(val1, val2) 

#define VD_TEST_ASSERT_EQ(val1, val2) \
    GTEST_ASSERT_EQ(val1, val2)

#define VD_TEST_ASSERT_NE(val1, val2) \
    GTEST_ASSERT_NE(val1, val2)

#define VD_TEST_ASSERT_LE(val1, val2) \
    GTEST_ASSERT_LE(val1, val2)

#define VD_TEST_ASSERT_LT(val1, val2) \
    GTEST_ASSERT_LT(val1, val2)

#define VD_TEST_ASSERT_GE(val1, val2) \
    GTEST_ASSERT_GE(val1, val2)

#define VD_TEST_ASSERT_GT(val1, val2) \
    GTEST_ASSERT_GT(val1, val2)

// ============================================================================================== //
// C String Comparisons.  All tests treat NULL and any non-NULL string
// as different.  Two NULLs are equal.
//
//    * {ASSERT|EXPECT}_STREQ(s1, s2):     Tests that s1 == s2
//    * {ASSERT|EXPECT}_STRNE(s1, s2):     Tests that s1 != s2
//    * {ASSERT|EXPECT}_STRCASEEQ(s1, s2): Tests that s1 == s2, ignoring case
//    * {ASSERT|EXPECT}_STRCASENE(s1, s2): Tests that s1 != s2, ignoring case
//
// For wide or narrow string objects, you can use the
// {ASSERT|EXPECT}_??() macros.
//
// Don't depend on the order in which the arguments are evaluated,
// which is undefined.
//
// These macros evaluate their arguments exactly once.
// ============================================================================================== //

#define VD_TEST_EXPECT_STR_EQ(expected, actual) \
  EXPECT_STREQ(expected, actual)
  
#define VD_TEST_EXPECT_STR_NE(s1, s2) \
  EXPECT_STRNE(s1, s2)

#define VD_TEST_EXPECT_STR_EQ_IGNORE_CASE(expected, actual) \
  EXPECT_STRCASEEQ(expected, actual)
  
#define VD_TEST_EXPECT_STR_NE_IGNORE_CASE(s1, s2) \
  EXPECT_STRCASENE(s1, s2)
  
#define VD_TEST_ASSERT_STR_EQ(expected, actual) \
  ASSERT_STREQ(expected, actual) 

#define VD_TEST_ASSERT_STR_NE(s1, s2) \
  ASSERT_STRNE(s1, s2) 

#define VD_TEST_ASSERT_STR_EQ_IGNORE_CASE(expected, actual) \
  ASSERT_STRCASEEQ(expected, actual) 

#define VD_TEST_ASSERT_STR_NE_IGNORE_CASE(s1, s2) \
  ASSERT_STRCASENE(s1, s2)
  
// ============================================================================================== //
// Macros for comparing floating-point numbers.
//
//    * {ASSERT|EXPECT}_FLOAT_EQ(expected, actual):
//         Tests that two float values are almost equal.
//    * {ASSERT|EXPECT}_DOUBLE_EQ(expected, actual):
//         Tests that two double values are almost equal.
//    * {ASSERT|EXPECT}_NEAR(v1, v2, abs_error):
//         Tests that v1 and v2 are within the given distance to each other.
//
// Google Test uses ULP-based comparison to automatically pick a default
// error bound that is appropriate for the operands.  See the
// FloatingPoint template class in gtest-internal.h if you are
// interested in the implementation details.
// ============================================================================================== //

#define VD_TEST_EXPECT_F32_ALMOST_EQ(expected, actual) \
  EXPECT_FLOAT_EQ(expected, actual)
  
#define VD_TEST_EXPECT_F64_ALMOST_EQ(expected, actual) \
  EXPECT_DOUBLE_EQ(expected, actual)
  
#define VD_TEST_ASSERT_F32_ALMOST_EQ(expected, actual) \
  ASSERT_FLOAT_EQ(expected, actual)
  
#define VD_TEST_ASSERT_F64_ALMOST_EQ(expected, actual) \
  ASSERT_DOUBLE_EQ(expected, actual)
  
#define VD_TEST_EXPECT_VALUE_WITHIN_ERROR(val1, val2, abs_error) \
  EXPECT_NEAR(val1, val2, abs_error)

#define VD_TEST_ASSERT_VALUE_WITHIN_ERROR(val1, val2, abs_error) \
  ASSERT_NEAR(val1, val2, abs_error)

// ============================================================================================== //
// Macros that execute statement and check that it doesn't generate new fatal
// failures in the current thread.
//
//   * VD_TEST_{ASSERT|EXPECT}_NO_FATAL_FAILURE(statement);
//
// Examples:
//
//   VD_TEST_EXPECT_NO_FAILURE(Process());
//   VD_TEST_ASSERT_NO_FAILURE(Process()) << "Process() failed";
//
// ============================================================================================== //

#define VD_TEST_ASSERT_NO_FAILURE(statement) \
    ASSERT_NO_FATAL_FAILURE(statement)

#define VD_TEST_EXPECT_NO_FAILURE(statement) \
    EXPECT_NO_FATAL_FAILURE(statement) 

// ============================================================================================== //
// Causes a trace (including the source file path, the current line
// number, and the given message) to be included in every test failure
// message generated by code in the current scope.  The effect is
// undone when the control leaves the current scope.
//
// The message argument can be anything streamable to std::ostream.
//
// In the implementation, we include the current line number as part
// of the dummy variable name, thus allowing multiple SCOPED_TRACE()s
// to appear in the same block - as long as they are on different
// lines.
// ============================================================================================== //

#define VD_TEST_TRACE_SCOPE(message) \
    SCOPED_TRACE(message)

#define VD_TEST_EXPECT_FAILURE(statement, substr) \
    EXPECT_FATAL_FAILURE(statement, substr)

#define VD_TEST_EXPECT_FAILURE_ON_ALL_THREADS(statement, substr) \
    EXPECT_FATAL_FAILURE_ON_ALL_THREADS(statement, substr)

#define VD_TEST_ASSERT_EXIT(statement, predicate, regex) \
    ASSERT_EXIT(statement, predicate, regex)

// Like ASSERT_EXIT, but continues on to successive tests in the test case, if any:

#define VD_TEST_EXPECT_EXIT(statement, predicate, regex) \
    EXPECT_EXIT(statement, predicate, regex)

// Asserts that a given statement causes the program to exit, either by
// explicitly exiting with a nonzero exit code or being killed by a
// signal, and emitting error output that matches regex.

#define VD_TEST_ASSERT_DEATH(statement, regex) \
    ASSERT_DEATH(statement, regex)

// Like ASSERT_DEATH, but continues on to successive tests in the test case, if any:
#define VD_TEST_EXPECT_DEATH(statement, regex) \
    EXPECT_DEATH(statement, regex)

// ============================================================================================== //

#endif // VD_TEST_MACROS_INCLUDED

