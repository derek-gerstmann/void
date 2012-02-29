#include "core/framework.h"

enum
{
    VD_INVALID_VALUE
};

using vd::core::WorkQueue;

class TestWorkQueue : public WorkQueue
{
public:
	class TestTask : public Task
	{
		friend class TestWorkQueue;
		
	public:
		explicit TestTask(vd::string echo) { m_Echo = echo; }
		
	private:
		vd::string m_Echo;	
	};
	
	
public:

    void SetParams(vd::i32 threads, double interval)
    {
        m_ThreadCount = threads;
        m_Interval = interval;
        m_DoneCount.Set(0);
    }

    virtual void
    OnRunTask(WorkQueue::Task* wqtask)
    {
    	TestTask* task = (TestTask*)wqtask;
		printf("[%d] %s\n", (int)task->GetSlotId(), task->m_Echo.c_str());
		
        m_DoneCount.Increment();

        if(m_Interval > 0)
        {
            vd::core::Thread::Sleep(m_Interval * m_ThreadCount);
        }
        else if(m_Interval < 0)
        {
            vd::core::Thread::Yield();
        }

        VD_DELETE(task);
    }

    vd::i64 DoneCount()
    {
        return m_DoneCount.Get();
    }

	// VD_DECLARE_OBJECT(TestWorkQueue);
	
private:
    vd::i32 m_ThreadCount;
    double m_Interval;
    vd::core::AtomicCounter m_DoneCount;
};

// VD_IMPLEMENT_OBJECT(TestWorkQueue, false, WorkQueue);

void RunTest()
{
    vd::u64 randseed = 987654321;
    vd::i64 rnum = 10; // 1000 * 10;
    vd::i32 thnum = vd::core::System::GetProcessorCount();
    double interval = 0.0001; // 10.0 / rnum;
    
    vdLogGlobalInfo("Task Queue: seed=%u  rnum=%lld  thnum=%d  interval=%.3f",
           randseed, (long long)rnum, thnum, interval);

    double start = vd::core::System::GetTimeInSeconds();
    TestWorkQueue queue;
    queue.SetParams(thnum, interval);
    queue.Start(thnum);

    for(int64_t i = 1; i <= rnum; i++)
    {
        TestWorkQueue::TestTask* task = VD_NEW(TestWorkQueue::TestTask, vd::core::ToString(i));
        queue.AddTask(task);

        if(interval > 0)
        {
            vd::core::Thread::Sleep(interval);
        }
        else if(interval < 0)
        {
            vd::core::Thread::Yield();
        }

        if(rnum > 250 && i % (rnum / 250) == 0)
        {
            printf(".");

            if(i == rnum || i % (rnum / 10) == 0) printf(" (%08lld)", (long long)i);
        }
    }

    vdLogGlobalInfo("count: %lld", queue.ActiveCount());
    vdLogGlobalInfo("done: %lld", queue.DoneCount());
    queue.Finish();

    bool err = false;

    if(queue.ActiveCount() != 0)
    {
        vdLogGlobalError("WorkQueue ActiveCount > 0!");
        err = true;
    }

    if(queue.DoneCount() != rnum)
    {
        vdLogGlobalError("WorkQueue DoneCount > 0!");
        err = true;
    }

    //  double etime = kc::time();
    double end = vd::core::System::GetTimeInSeconds();
    vdLogGlobalInfo("Time: %.3f sec", end - start);
}

int 
main(int argc, char** argv)
{
	vd::core::System::Startup();
	vdLogGlobalInfo("Starting test '%s'...", argv[0]);
	vd::f64 start = vd::core::System::GetTimeInSeconds();
	RunTest();
	vd::f64 end = vd::core::System::GetTimeInSeconds();
	vdLogGlobalInfo("Test '%s' completed in '%f' sec!", argv[0], end - start);
	vd::core::System::Shutdown();
	return 0;
}

