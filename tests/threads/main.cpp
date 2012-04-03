#if 0

#include "core/framework.h"

int RunTest()
{
	typedef vd::core::Delegate< vd::uid () > TestDelegate;
	typedef vd::core::DelegateThread< TestDelegate > TestThread;

	TestDelegate::Function callback = VD_BIND_FUNCTION(&vd::core::Thread::GetId);
	TestDelegate* delegate = VD_NEW(TestDelegate, callback);
	vd::core::Handle<TestThread> thread = VD_NEW(TestThread, delegate);

//	vdGlobalAssert(thread.GetCallCount() == 0);

	thread->Start();
	thread->Join();	

//	vdGlobalAssert(thread.GetCallCount() > 0);

//	vd::uid tid = thread->GetDelegate()->GetResult();
//    vdGlobalAssert(tid != 0);
	return 0;
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

#endif