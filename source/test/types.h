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

#ifndef VD_TEST_TYPES_INCLUDED
#define VD_TEST_TYPES_INCLUDED

// ============================================================================================== //

#include "test/namespace.h"

#include "core/core.h"
#include "core/logging.h"
#include "core/system.h"
#include "core/process.h"
#include "core/memory.h"

// ============================================================================================== //

#include "gtest/gtest.h" // Using Googles Testing framework

// ============================================================================================== //

VD_TEST_NAMESPACE_BEGIN();

// ============================================================================================== //

typedef ::testing::Test               Base;
typedef ::testing::UnitTest           Unit;
typedef ::testing::TestCase           Case;
typedef ::testing::TestProperty       Property;
typedef ::testing::TestResult         Result;
typedef ::testing::TestInfo           Info;
typedef ::testing::Environment        Environment;
typedef ::testing::AssertionResult    Assertion;
typedef ::testing::TestEventListener  Listener;
typedef ::testing::TestEventListeners ListenerGroup;

template <typename T>
class WithParamInterface {
 public:
  typedef T ParamType;
  virtual ~WithParamInterface() {}

  // The current parameter value. Is also available in the test fixture's
  // constructor. This member function is non-static, even though it only
  // references static data, to reduce the opportunity for incorrect uses
  // like writing 'WithParamInterface<bool>::GetParam()' for a test that
  // uses a fixture whose parameter type is int.
  const ParamType& GetParam() const { return *parameter_; }

 private:
  // Sets parameter value. The caller is responsible for making sure the value
  // remains alive and unchanged throughout the current test.
  static void SetParam(const ParamType* parameter) {
    parameter_ = parameter;
  }

  // Static value used for accessing parameter during a test lifetime.
  static const ParamType* parameter_;

  // TestClass must be a subclass of WithParamInterface<T> and Test.
  template <class TestClass> friend class testing::internal::ParameterizedTestFactory;
};

template <typename T>
const T* WithParamInterface<T>::parameter_ = NULL;

// Most value-parameterized classes can ignore the existence of
// WithParamInterface, and can just inherit from ::testing::TestWithParam.

template <typename T>
class WithParam : public Base, public WithParamInterface<T> 
{

};

template <typename T, typename IncrementT>
testing::internal::ParamGenerator<T> 
Range(T start, T end, IncrementT step) 
{
    return testing::internal::ParamGenerator<T>(
        new testing::internal::RangeGenerator<T, IncrementT>(start, end, step)
    );
}

template <typename T>
testing::internal::ParamGenerator<T> 
Range(T start, T end) {
    return Range(start, end, 1);
}

// ============================================================================================== //

VD_TEST_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_TEST_TYPES_INCLUDED

