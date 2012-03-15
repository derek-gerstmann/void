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

#include "testing/namespace.h"

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
typedef ::testing::WithParamInterface WithParam;

// ============================================================================================== //

VD_TEST_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_TEST_TYPES_INCLUDED

