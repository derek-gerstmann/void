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

#ifndef VD_CORE_WORKQUEUE_INCLUDED
#define VD_CORE_WORKQUEUE_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/system.h"
#include "core/threading.h"
#include "core/logging.h"
#include "core/locking.h"
#include "containers/containers.h"

#include <boost/circular_buffer.hpp>

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_DECLARE_ENUM(WorkStatus,
    Invalid,
    Queued,
    Running,
    Complete,
    Cancelled,
    Exited,
    Failed);

// ============================================================================================== //

class WorkItem : public Shared<WorkItem>
{
    friend class WorkQueue;

public:
    explicit WorkItem();
    virtual ~WorkItem();

    vd::u64 GetWorkId() const;
    vd::u32 GetSlotId() const;

    bool IsAborted() const;
    bool IsDone() const;

    WorkStatus::Value GetStatus() const;

    VD_DECLARE_OBJECT(WorkItem);

private:

    vd::u64                 m_WorkId;
    vd::u64                 m_SlotId;
    WorkStatus::Value       m_Status;
};

// ============================================================================================== //

class WorkQueue : public Object
{

private:
    class WorkThread;
//    typedef vd::core::List<WorkItem*>::type WorkList;
    typedef boost::circular_buffer<WorkItem*> WorkList;

public:

    WorkQueue();
    virtual ~WorkQueue();

    virtual void 
    OnRun(WorkItem* work) { }

    virtual void 
    OnStart(const WorkItem* work);

    virtual void 
    OnFinish(const WorkItem* work);

    virtual vd::status 
    Destroy();

    void 
    Start(vd::i32 slots);

    void
    Wait(void);

    void 
    Finish(void);

    void
    Cancel(void);

    void 
    Exit();

    vd::i64 
    Submit(WorkItem* work);

    vd::i64 
    GetSize(void);

	VD_DECLARE_OBJECT(WorkQueue);
	
private:

    class WorkThread : public Thread
    {
        friend class WorkQueue;

    public:
        explicit WorkThread(); 
        VD_DECLARE_OBJECT(WorkThread);

    private:

        virtual void Run(void);        

        vd::u32             m_SlotId;
        WorkQueue*          m_WorkQueue;
        bool                m_IsDone;
    };

    vd::i32                 m_SlotCount;
    vd::i64                 m_Size;
    vd::u64                 m_Seed;

    WorkList                m_Queue;
    WorkThread*             m_Workers;
    Mutex                   m_Mutex;
    ConditionVariable       m_Condition;
    
    VD_DISABLE_COPY_CONSTRUCTORS(WorkQueue);
};

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_WORKQUEUE_INCLUDED

