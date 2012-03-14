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

#ifndef VD_CORE_DELEGATES_INCLUDED
#define VD_CORE_DELEGATES_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/atomics.h"
#include "core/functions.h"
#include "core/threading.h"
#include "core/shared.h"
#include "core/symbol.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

template<typename FuncSignature> class Delegate;

struct NullDelegate {};

// ============================================================================================== //

template<typename R>
class Delegate<R ()> : public Shared< Delegate<R ()> >
{
public:
	typedef R ResultType;
	typedef Delegate<R ()> DelegateType;
	typedef Function<R ()> FunctionType;
	typedef AtomicCounter CallCounter;

	Delegate() : Shared< Delegate<R ()> >(), m_Calls(0)  {}
	Delegate(const DelegateType& dg) : Shared< Delegate<R ()> >(dg), m_Fn(dg.m_Fn), m_Result(dg.m_Result), m_Calls(0) { }
	Delegate(const FunctionType& cb) : Shared< Delegate<R ()> >(), m_Fn(cb), m_Calls(0)  {}
	virtual ~Delegate() { m_Calls.Set(0); }
	
    DelegateType& operator=(const DelegateType& rhs)
        { *this = DelegateType(rhs); return *this; }
	
	inline void Invoke()
	{
		m_Calls.Increment();
		m_Result = m_Fn();
	}
	
	inline void operator()() 			{ Invoke(); }
	inline void Reset() 				{ m_Calls.Set(0); }
	inline bool IsReady(void)			{ return m_Calls > 0; }
	inline vd::i32 GetCallCount()		{ return m_Calls; }
	inline const R& GetResult() const	{ return m_Result;	}
	inline R CopyResult() const			{ return m_Result;	}
	
private:

	FunctionType m_Fn;
	ResultType 	 m_Result;
	CallCounter  m_Calls;
};

// ============================================================================================== //

template<typename R, typename P1>
class Delegate<R (P1)> : public Shared< Delegate<R (P1)> >
{
public:
	typedef R ResultType;
	typedef Function<R (P1)> FunctionType;
	typedef Delegate<R (P1)> DelegateType;
	typedef AtomicCounter CallCounter;

	Delegate() : Shared< Delegate<R (P1)> >(), m_Calls(0)  {}
	
	Delegate(
		const DelegateType& dg
	) :
		Shared< Delegate<R (P1)> >(dg), 
		m_P1(dg.m_P1), 
		m_Fn(dg.m_Fn), 
		m_Result(dg.m_Result), 
		m_Calls(0) 
	{ 
		// EMPTY!
	}
	
	Delegate(const FunctionType& cb, const P1& p1) : Shared< Delegate<R (P1)> >(), m_P1(p1), m_Fn(cb), m_Calls(0)  {}
	virtual ~Delegate() { m_Calls.Set(0); }
	
    DelegateType& operator=(const DelegateType& rhs)
        { *this = DelegateType(rhs); return *this; }
	
	inline void Invoke()
	{
		m_Calls.Increment();
		m_Result = m_Fn(m_P1);
	}
	
	inline void operator()() 			{ Invoke(); }
	inline void Reset() 				{ m_Calls.Set(0); }
	inline bool IsReady(void)			{ return m_Calls > 0; }
	inline vd::i32 GetCallCount()		{ return m_Calls; }
	inline const R& GetResult() const	{ return m_Result;	}
	inline R CopyResult() const			{ return m_Result;	}
	
private:

	P1 				m_P1;
	FunctionType 	m_Fn;
	ResultType		m_Result;
	CallCounter 	m_Calls;
};


// ============================================================================================== //

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
class Delegate<R (P1, P2, P3, P4, P5, P6)> : public Shared< Delegate<R (P1, P2, P3, P4, P5, P6)> >
{
public:
	typedef R ResultType;
	typedef Delegate<R (P1, P2, P3, P4, P5, P6)> DelegateType;
	typedef Function<R (P1, P2, P3, P4, P5, P6)> FunctionType;
	typedef AtomicCounter CallCounter;

	Delegate(
		const FunctionType& cb,
		const P1& p1, 
		const P2& p2, 
		const P3& p3, 
		const P4& p4, 
		const P5& p5, 
		const P6& p6
	) : Shared< Delegate<R (P1, P2, P3, P4, P5, P6)> >(),
		m_P1(p1),
		m_P2(p2),
		m_P3(p3),
		m_P4(p4),
		m_P5(p5),
		m_P6(p6),
		m_Fn(cb),
		m_Result(),
		m_Calls(0)
	{ 
		// EMPTY!
	}

	Delegate(
		const DelegateType& dg
	)  : Shared< Delegate<R (P1, P2, P3, P4, P5, P6)> >(),
		m_P1(dg.m_P1),
		m_P2(dg.m_P2),
		m_P3(dg.m_P3),
		m_P4(dg.m_P4),
		m_P5(dg.m_P5),
		m_P6(dg.m_P6),
		m_Fn(dg.m_Fn),
		m_Result(dg.m_Result),
		m_Calls(0)
	{ 
		// EMPTY!
	}


	virtual ~Delegate() { m_Calls.Set(0); }

    DelegateType& operator=(const DelegateType& rhs)
        { *this = DelegateType(rhs); return *this; }
	
	inline void Invoke()
	{
		m_Calls.Increment();
		m_Result = m_Fn(m_P1, m_P2, m_P3, m_P4, m_P5, m_P6);
	}

	inline void operator()()			 	{ Invoke(); }
	inline void Reset() 					{ m_Calls.Set(0); }
	inline bool IsReady(void)				{ return m_Calls > 0; }
	inline vd::i32 GetCallCount()			{ return m_Calls; }
	inline const R& GetResult() const		{ return m_Result;	}
	inline R CopyResult() const				{ return m_Result;	}
	
private:

	P1 			 m_P1;
	P2 			 m_P2;
	P3 			 m_P3;
	P4 			 m_P4;
	P5 			 m_P5;
	P6 			 m_P6;
	FunctionType m_Fn;
	ResultType 	 m_Result;
	CallCounter  m_Calls;
};

// ============================================================================================== //

template<typename DT>
class DelegateThread : public Thread
{

public:

	typedef DT DelegateType;

	static DelegateThread* Create(
		DT* delegate, 
		const Symbol& name = Symbol())
	{
		DelegateThread* self = VD_NEW(DelegateThread<DT>, delegate, name);
		return self;
	}
	
public:

    DelegateThread(
		DT* delegate, 
		const Symbol& name = Symbol()
	) :
		Thread(),
		m_Name(name),
		m_Delegate(delegate)
	{
		// EMPTY!
	}

	void Setup(
		bool critical = false,
		size_t stack_size = 0, 
		int affinity = -1)
	{
		Thread::Setup(Symbol::ToString(m_Name), critical, stack_size, affinity);
	}
	
    virtual void Run()
    {
		m_Delegate->Invoke();
    }
    
    inline const Symbol& GetName() 			{ return m_Name; }
    inline vd::i32 GetCallCount() 			{ return m_Delegate->GetCallCount(); }
	inline const DT* GetDelegate() const 	{ return m_Delegate; }
	inline DT* GetDelegate() 				{ return m_Delegate; }

protected:

	virtual ~DelegateThread() { }

	VD_DISABLE_COPY_CONSTRUCTORS(DelegateThread);

private:
    
	Symbol m_Name;
	Handle<DT> m_Delegate;
		
};

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_DELEGATES_INCLUDED
