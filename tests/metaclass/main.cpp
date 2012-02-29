#include <ctime>
#include <cstdio>
#include <vector>

#include "core/test.h"
#include "core/metaclass.h"

// ============================================================================================== //

TEST(CoreTest, CreateMetaclass) 
{
	vd::core::MetaClass mc(vd::core::Symbol("test"), vd::core::Symbol("none"), true);
	EXPECT_STREQ("test", mc.GetName().c_str());
}

// ============================================================================================== //


