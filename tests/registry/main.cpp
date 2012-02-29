#include <ctime>
#include <cstdio>
#include <vector>

#include "core/framework.h"

// ============================================================================================== //

using vd::core::Symbol;
using vd::core::Registry;
using namespace vd::constants;

// ============================================================================================== //

class TestValue
{
public:

	TestValue()
		: m_Value(0), m_Id(0)
	{
	
	}

	TestValue(vd::f32 v) 
		: m_Value(v), m_Id(0)
	{
	
	}

	TestValue(const TestValue& other) 
		: m_Value(other.m_Value), m_Id(other.m_Id)
	{
	
	}

	TestValue& operator=(const TestValue& other)
	{
		m_Value = other.m_Value;
		return *this;
	}

	vd::f32 GetValue() const { return m_Value; }
	void SetValue(vd::f32 v) { m_Value = v; }

	vd::u32 GetId() const { return m_Id; }
	void SetId(vd::u32 v) { m_Id = v; }
	
protected:

	vd::f32 m_Value;
	vd::u32 m_Id;

};

typedef Registry<TestValue> TestRegistry;
typedef Registry<Symbol> TestSymbolRegistry;

// ============================================================================================== //

void RunRegistryTest(unsigned N)
{
	TestRegistry registry;
	vd::u32 index[N];
	vd::f32 values[N];
		
	unsigned X1 = 0, X2 = 0, X3 = 0;
	registry.Clear();

	for(int j=0; j<8; ++j)
	{
		clock_t c0 = clock();

		for(unsigned i = 0; i < N; ++i)
		{
			vd::uid uid = registry.Reserve();
			registry.Retrieve(uid).SetValue(i);
			index[i] = uid;
			values[i] = i;  
		}

		clock_t c1 = clock();

		for(int k=0; k<100; ++k)
		{
			unsigned ai = registry.Size();
			for(unsigned ei = 0; ei < ai; ei++)
			{
				vd::uid uid = index[ei];
				vd::f32 v = values[ei];
				VD_FATAL_IF(registry.Retrieve(uid).GetValue() != v, "Failed!");
			}
		}

		clock_t c2 = clock();
		registry.Clear();
		clock_t c3 = clock();
		
		X1 += (unsigned)(c1 - c0);
		X2 += (unsigned)(c2 - c1);
		X3 += (unsigned)(c3 - c2);
	}
	printf("%8u\t%06u\t%06u\t%06u\n", N, X1, X2, X3);
}

// ============================================================================================== //

void RunSymbolTest(unsigned N)
{
	TestSymbolRegistry registry;
	vd::u32 index[N];
	Symbol values[N];
		
	unsigned X1 = 0, X2 = 0, X3 = 0;
	registry.Clear();

	for(int j=0; j<8; ++j)
	{
		clock_t c0 = clock();

		for(unsigned i = 0; i < N; ++i)
		{
			vd::string value = vd::core::ToString(i);
			index[i] = Symbol::Register(value.c_str());
			values[i] = Symbol::Retrieve(index[i]);
		}

		clock_t c1 = clock();

		for(int k=0; k<100; ++k)
		{
			unsigned ai = registry.Size();
			for(unsigned ei = 0; ei < ai; ei++)
			{
				vd::uid uid = index[ei];
				Symbol s = values[ei];
				
				VD_FATAL_IF(registry.Retrieve(uid) != s, "Failed!");
			}
		}

		clock_t c2 = clock();
		registry.Clear();
		clock_t c3 = clock();
		
		X1 += (unsigned)(c1 - c0);
		X2 += (unsigned)(c2 - c1);
		X3 += (unsigned)(c3 - c2);
	}
	printf("%8u\t%06u\t%06u\t%06u\n", N, X1, X2, X3);
}

void RunTest()
{
	printf("Registry speed test\n");
	printf("  Repetitions   Insert Replace Clear\n");
	for(unsigned a = 1024; a < 8 * 1024; )
	{
		unsigned x = a;
		for(unsigned i=0; i<10; ++i)
		{
			RunRegistryTest(x);
			x += a;
		}
		a = x;
	}

	printf("Symbol Registry speed test\n");
	printf("  Repetitions   Insert Replace Clear\n");
	for(unsigned a = 1024; a < 8 * 1024;  )
	{
		unsigned x = a;
		for(unsigned i=0; i<10; ++i)
		{
			RunSymbolTest(x);
			x += a;
		}
		a = x;
	}
}

int 
main(int argc, char** argv)
{
	vd::core::System::Startup();
	vdLogGlobalInfo("Starting test '%s'...", argv[0]);
	vd::f64 start = vd::core::System::GetTimeInSeconds();
	RunTest();
	vd::f64 end = vd::core::System::GetTimeInSeconds();
	vdLogGlobalInfo("Test '%s' completed in '%f' sec!\n", argv[0], end - start);
	vd::core::System::Shutdown();
	return 0;
}
