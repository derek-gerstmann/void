// ============================================================================================== //
//
// License:		The Lesser GNU Public License (LGPL) v3.0.
// 
// Author(s): 	Derek Gerstmann. The University of Western Australia (UWA). 
//				As well as the shoulders of many giants...
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

#include "core/workqueue.h"
#include "core/asserts.h"
#include "core/utilities.h"
#include "core/process.h"

#include "constants/constants.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

WorkItem::WorkItem() : 
    m_WorkId(0), 
    m_SlotId(0), 
    m_Status(WorkStatus::Invalid)
{
    vdGlobalAssert(true);
}

WorkItem::~WorkItem()
{
    vdGlobalAssert(true);
}

vd::u64 WorkItem::GetWorkId() const
{
    vdGlobalAssert(true);
    return m_WorkId;
}

vd::u32 WorkItem::GetSlotId() const
{
    vdGlobalAssert(true);
    return m_SlotId;
}

bool WorkItem::IsAborted() const
{
    vdGlobalAssert(true);
    return m_Status == WorkStatus::Cancelled;
}

bool WorkItem::IsDone() const
{
    vdGlobalAssert(true);
    return m_Status == WorkStatus::Complete;
}

WorkStatus::Value WorkItem::GetStatus() const
{
    return m_Status;
}

// ============================================================================================== //

WorkQueue::WorkQueue() : 
    m_SlotCount(0), 
    m_Size(0), 
    m_Seed(0),
    m_Queue(1024), 
    m_Workers(NULL), 
    m_Mutex(), 
    m_Condition()
{
    vdGlobalAssert(true);
}

WorkQueue::~WorkQueue()
{
    Destroy();
}


void 
WorkQueue::OnStart(const WorkItem* work)
{
    vdGlobalAssert(true);
}

void 
WorkQueue::OnFinish(const WorkItem* work)
{
    vdGlobalAssert(true);
}

vd::status
WorkQueue::Destroy()
{
    if(m_Workers)
    {
        VD_DELETE_ARRAY(m_Workers);        
    }
    return Status::Code::Success;
}

void 
WorkQueue::Start(vd::i32 slots)
{
    vdGlobalAssert(slots > 0 && slots <= VD_I32_MAX);
    m_Workers = VD_NEW_ARRAY( WorkThread, slots );

    for(vd::i32 i = 0; i < slots; i++)
    {
        vd::string name = vd::string("WorkQueue[") + Convert::ToString(i) + vd::string("]");
        m_Workers[i].m_SlotId = i;
        m_Workers[i].m_WorkQueue = this;
        m_Workers[i].Setup(name.c_str());
        m_Workers[i].Start();
    }

    m_SlotCount = slots;
}

void
WorkQueue::Wait(void)
{
    for(vd::i32 i = 0; i < m_SlotCount; i++)
    {
        m_Workers[i].Join();
    }        
}

void 
WorkQueue::Finish(void)
{
    Cancel();
    Wait();
    Destroy();
}

void
WorkQueue::Cancel(void)
{
    vdGlobalAssert(true);
    m_Mutex.Lock();
    WorkList::iterator it = m_Queue.begin();
    WorkList::iterator last = m_Queue.end();

    while(it != last)
    {
        WorkItem* work = *it;
        work->m_Status = WorkStatus::Cancelled;
        ++it;
    }

    m_Condition.Broadcast();
    m_Mutex.Unlock();
    
    Thread::Yield();

    vd::i32 res = Process::GetTimeResolution();
    for(vd::f64 wsec = 1.0 / vd::f64(res); true; wsec *= 2)
    {
        m_Mutex.Lock();

        if(m_Queue.empty())
        {
            m_Mutex.Unlock();
            break;
        }

        m_Mutex.Unlock();
        if(wsec > 1.0) wsec = 1.0;
        Thread::Sleep(wsec);
    }

    m_Mutex.Lock();
    {
        for(vd::i32 i = 0; i < m_SlotCount; i++)
        {
            m_Workers[i].m_IsDone = true;
        }
        m_Condition.Broadcast();
    }
    m_Mutex.Unlock();        

    Thread::Yield();

    for(vd::i32 i = 0; i < m_SlotCount; i++)
    {
        m_Workers[i].Join();
    }        
}

void 
WorkQueue::Exit()
{
    Cancel();
    Destroy();
}

vd::i64 
WorkQueue::Submit(WorkItem* work)
{
    vdGlobalAssert(work);
    vd::i64 count; 
    m_Mutex.Lock();
    {
        work->m_WorkId = ++m_Seed;
        m_Queue.push_back(work);
        count = ++m_Size;
        m_Condition.Signal();
    }
    m_Mutex.Unlock();
    return count;
}

vd::i64 
WorkQueue::GetSize(void)
{
    vdGlobalAssert(true);
    m_Mutex.Lock();
    vd::i64 count = m_Size;
    m_Mutex.Unlock();
    return count;
}

WorkQueue::WorkThread::WorkThread() :
    m_SlotId(0), 
    m_WorkQueue(NULL),
    m_IsDone(false)
{
    vdGlobalAssert(true);
}

void 
WorkQueue::WorkThread::Run(void)
{
    vdGlobalAssert(true);
    
    WorkItem* start_work = VD_NEW( WorkItem );
    start_work->m_SlotId = m_SlotId;
    m_WorkQueue->OnStart(start_work);
    VD_DELETE(start_work);
    
    bool empty = false;

    while(true)
    {
        WorkItem* work = NULL;

        m_WorkQueue->m_Mutex.Lock();
        {
            if(m_IsDone)
            {
                m_WorkQueue->m_Mutex.Unlock();
                break;
            }

            if(empty) 
            {
                m_WorkQueue->m_Condition.Wait(&m_WorkQueue->m_Mutex, 1.0);
            }
            
            if(m_WorkQueue->m_Queue.empty())
            {
                empty = true;
            }
            else
            {
                work = m_WorkQueue->m_Queue.front();
                work->m_SlotId = m_SlotId;
                
                m_WorkQueue->m_Queue.pop_front();
                m_WorkQueue->m_Size--;
                
                empty = false;
            }
        }
        m_WorkQueue->m_Mutex.Unlock();

        if(work && work->m_Status != WorkStatus::Cancelled)
        {
            m_WorkQueue->OnRun(work);
        }
    }

    WorkItem* done_work = VD_NEW( WorkItem );
    done_work->m_SlotId = m_SlotId;
    done_work->m_Status = WorkStatus::Complete;
    m_WorkQueue->OnFinish(done_work);
    VD_DELETE(done_work);
}
        
// ============================================================================================== //

VD_IMPLEMENT_ABSTRACT_OBJECT(WorkQueue, vd_sym(WorkQueue), vd_sym(Object));
VD_IMPLEMENT_OBJECT(WorkItem, vd_sym(WorkItem), vd_sym(Object));
VD_IMPLEMENT_OBJECT(WorkQueue::WorkThread, vd_sym(WorkThread), vd_sym(Thread));

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //


