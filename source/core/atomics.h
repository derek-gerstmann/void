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

#ifndef VD_CORE_ATOMICS_INCLUDED
#define VD_CORE_ATOMICS_INCLUDED

// ============================================================================================== //

#include "core/core.h"

// ============================================================================================== //

#if 	defined(VD_TARGET_OSX)
#include <libkern/OSAtomic.h>
#elif 	defined(VD_TARGET_WINDOWS)
#include <intrin.h>
#endif

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //
// The following atomic operations are based on Matt Pharr, et al's PBRT implementation
// ============================================================================================== //
    
template <typename T> inline T*
AtomicExchangePtr(
	T** oldptr,
	T* newptr)
{
#if defined(VD_TARGET_WINDOWS)

	return (T*) InterlockedExchangePointer(
					reinterpret_cast<volatile PVOID*>(oldptr),  
					newptr);
#else

#ifndef __clang__

    return __sync_bool_compare_and_swap(oldptr, (*oldptr), newptr);

#else

#if (__SIZEOF_POINTER__ == 8)

    return __sync_bool_compare_and_swap(
		reinterpret_cast<long long volatile*>(oldptr), 
		reinterpret_cast<long long>(*oldptr),
		reinterpret_cast<long long>(newptr));
#else

    return __sync_bool_compare_and_swap(
		reinterpret_cast<long volatile*>(oldptr), 
		reinterpret_cast<long>(*oldptr),
		reinterpret_cast<long>(newptr));
#endif
#endif
#endif
}

template <typename T> inline bool 
AtomicCompareAndExchangePtr(
	T** ptr, 
	T* newptr, 
	T* oldptr)
{
#if defined(VD_TARGET_WINDOWS)

    return InterlockedCompareExchangePointer(
    			reinterpret_cast<volatile PVOID*>(ptr), 
    			newptr, oldptr) == oldptr;
#else

#ifndef __clang__

    return __sync_bool_compare_and_swap(ptr, oldptr, newptr);

#else

#if (__SIZEOF_POINTER__ == 8)

    return __sync_bool_compare_and_swap(
		reinterpret_cast<long long volatile*>(ptr), 
		reinterpret_cast<long long>(oldptr),
		reinterpret_cast<long long>(newptr));
#else

    return __sync_bool_compare_and_swap(
		reinterpret_cast<long volatile*>(ptr), 
		reinterpret_cast<long>(oldptr),
		reinterpret_cast<long>(newptr));
#endif
#endif
#endif
}

inline bool 
AtomicCompareAndExchange(
	volatile vd::i32* v, 
	vd::i32 newValue, 
	vd::i32 oldValue)
{
#if defined(VD_TARGET_WINDOWS)
    return InterlockedCompareExchange(
		reinterpret_cast<volatile LONG*>(v), 
		newValue, oldValue) == oldValue;
#else
    return __sync_bool_compare_and_swap(v, oldValue, newValue);
#endif
}

inline bool 
AtomicCompareAndExchange(
	volatile vd::i64* v, 
	vd::i64 newValue, 
	vd::i64 oldValue)
{
#if defined(VD_TARGET_WINDOWS)
    return _InterlockedCompareExchange64(
		reinterpret_cast<volatile LONGLONG*>(v), 
		newValue, oldValue) == oldValue;
#else
    return __sync_bool_compare_and_swap(v, oldValue, newValue);
#endif
}
    
inline float 
AtomicAdd(
	volatile float* dst, 
	float delta)
{
    union bits { float f; vd::i32 i; };
    bits oldVal, newVal;    
    do
    {
#if (defined(__i386__) || defined(__amd64__))
        __asm__ __volatile__("pause\n");
#endif
        oldVal.f = *dst;
        newVal.f = oldVal.f + delta;
    }
    while(!AtomicCompareAndExchange((volatile vd::i32*) dst, newVal.i, oldVal.i));
    
    return newVal.f;
}
    
inline double 
AtomicAdd(
	volatile double* dst, 
	double delta)
{
    union bits { double f; vd::i64 i; };
    bits oldVal, newVal;    
    do
    {
#if (defined(__i386__) || defined(__amd64__))
        __asm__ __volatile__("pause\n");
#endif
        oldVal.f = *dst;
        newVal.f = oldVal.f + delta;
    }
    while(!AtomicCompareAndExchange((volatile vd::i64*) dst, newVal.i, oldVal.i));
    
    return newVal.f;
}
    
inline vd::i32 
AtomicAdd(
	volatile vd::i32* dst, 
	vd::i32 delta)
{
#if defined(VD_TARGET_WINDOWS)
    return InterlockedExchangeAdd(reinterpret_cast<volatile LONG*>(dst), delta) + delta;
#else
    return __sync_add_and_fetch(dst, delta);
#endif
}
    
inline vd::i64 
AtomicAdd(
	volatile vd::i64* dst, 
	vd::i64 delta)
{
#if defined(WIN64)
    return _InterlockedExchangeAdd64(reinterpret_cast<volatile LONGLONG*>(dst), delta) + delta;
#elif defined(VD_TARGET_WINDOWS)
    vdLogGlobalError("AtomicAdd() cannot handle 64-bit integers on WIN32");
    return 0;
#else
    return __sync_add_and_fetch(dst, delta);
#endif
}

// ============================================================================================== //

template <typename T>
class Atomic
{
public:
    explicit Atomic(const T& other);
    Atomic(const Atomic& other);

    const T value() const;
    void Set(const T& other);

    Atomic& operator=(const T& other);
    Atomic& operator=(const Atomic& other);

    bool operator<(const T& other) const;
    bool operator<(const Atomic& other) const;
    bool operator<=(const vd::i32& other) const;
    bool operator<=(const T& other) const;
    bool operator>(const T& other) const;
    bool operator>(const Atomic& other) const;
    bool operator>=(const T& other) const;
    bool operator>=(const Atomic& other) const;

    bool operator==(const T& other) const;
    bool operator==(const Atomic& other) const;

    bool operator!=(const T& other) const;
    bool operator!=(const Atomic& other) const;

    const T& operator++();
    const T& operator--();
    T operator++(int);
    T operator--(int);

    Atomic& operator+=(const T& other);
    Atomic& operator+=(const Atomic& other);
    Atomic& operator-=(const T& other);
    Atomic& operator-=(const Atomic& other);
    Atomic& operator&=(const T& other);
    Atomic& operator&=(const Atomic& other);
    Atomic& operator|=(const T& other);
    Atomic& operator|=(const Atomic& other);
    Atomic& operator^=(const T& other);
    Atomic& operator^=(const Atomic& other);

    bool cas(const T& existing, const T& updated);
    
    T swap(const T& other) 
    {
        T rv;
        while (true) 
        {
            rv = value();
            if (cas(rv, other)) 
            {
                break;
            }
        }
        return rv;
    }    
};

// ============================================================================================== //

template <>
class Atomic<vd::i32>
{
public:
    Atomic()
    {
    }

    explicit Atomic(vd::i32 other)
    {
        this->Set(other);
    }

    const vd::i32 value() const
    {
        static const vd::i32 zero = 0;
        return AtomicAdd(&m_Value, zero);
    }

    bool Set(const volatile vd::i32& other)
    {
        return AtomicCompareAndExchange((volatile vd::i32*) &m_Value, other, m_Value);
    }

    Atomic& operator=(const vd::i32& other)
    {
        this->Set(other);
        return *this;
    }
    
    Atomic& operator=(const Atomic& other)
    {
        this->Set(other.m_Value);
        return *this;
    }

    bool cas(const vd::i32& oldval, const vd::i32& newval)
    {
        return AtomicCompareAndExchange((volatile vd::i32*) &m_Value, newval, oldval);
    }

    bool operator<(const vd::i32& other) const
    {
        return this->value() < other;
    }
    
    bool operator<(const Atomic& other) const
    {
        return this->value() < other.value();
    }
    
    bool operator<=(const vd::i32& other) const
    {
        return this->value() <= other;
    }
    
    bool operator<=(const Atomic& other) const
    {
        return this->value() <= other.value();
    }
    
    bool operator>(const vd::i32& other) const
    {
        return this->value() > other;
    }
    
    bool operator>(const Atomic& other) const
    {
        return this->value() > other.value();
    }
    
    bool operator>=(const vd::i32& other) const
    {
        return this->value() >= other;
    }

    bool operator>=(const Atomic& other) const
    {
        return this->value() >= other.value();
    }

    bool operator==(const vd::i32& other) const
    {
        return this->value() == other;
    }
    
    bool operator==(const Atomic& other) const
    {
        return this->value() == other.value();
    }

    bool operator!=(const vd::i32& other) const
    {
        return this->value() != other;
    }
    
    bool operator!=(const Atomic& other) const
    {
        return this->value() != other.value();
    }

    Atomic& operator+=(const vd::i32& other)
    {
        AtomicAdd(&m_Value, other);
        return *this;
    }

    Atomic& operator+=(const Atomic& other)
    {
        vd::i32 old = other.value();
        AtomicAdd(&m_Value, old);
        return *this;
    }

    Atomic& operator-=(const vd::i32& other)
    {
        AtomicAdd(&m_Value, -other);
        return *this;
    }

    Atomic& operator-=(const Atomic& other)
    {
        vd::i32 old = other.value();
        AtomicAdd(&m_Value, -old);
        return *this;
    }

    Atomic& operator*=(const vd::i32& other)
    {
        vd::i32 comperand;
        vd::i32 exchange;

        do
        {
            comperand = m_Value;
            exchange = comperand * other;
        }
        while(!this->cas(comperand, exchange));

        return *this;
    }

    Atomic& operator/=(const vd::i32& other)
    {
        vd::i32 comperand;
        vd::i32 exchange;

        do
        {
            comperand = m_Value;
            exchange = comperand / other;
        }
        while(!this->cas(comperand, exchange));

        return *this;
    }

    Atomic& operator%=(const vd::i32& other)
    {
        vd::i32 comperand;
        vd::i32 exchange;

        do
        {
            comperand = m_Value;
            exchange = comperand % other;
        }
        while(!this->cas(comperand, exchange));

        return *this;
    }

    Atomic& operator&=(const vd::i32& other)
    {
        vd::i32 comperand;
        vd::i32 exchange;

        do
        {
            comperand = m_Value;
            exchange = comperand & other;
        }
        while(!this->cas(comperand, exchange));

        return *this;
    }

    Atomic& operator|=(const vd::i32& other)
    {
        vd::i32 comperand;
        vd::i32 exchange;

        do
        {
            comperand = m_Value;
            exchange = comperand | other;
        }
        while(!this->cas(comperand, exchange));

        return *this;
    }

    Atomic& operator^=(const vd::i32& other)
    {
        vd::i32 comperand;
        vd::i32 exchange;

        do
        {
            comperand = m_Value;
            exchange = comperand ^ other;
        }
        while(!this->cas(comperand, exchange));

        return *this;
    }

    const volatile vd::i32& operator++()
    {
        this->operator+=((volatile vd::i32)1);
        return m_Value;
    }

    const volatile vd::i32& operator--()
    {
        this->operator-=((volatile vd::i32)1);
        return m_Value;
    }

    volatile vd::i32 operator++(int)
    {
        this->operator+=(1);
        return m_Value;
    }

    volatile vd::i32 operator--(int)
    {
        this->operator-=(1);
        return m_Value;
    }

private:

    volatile mutable vd::i32 m_Value;
};

// ============================================================================================== //

#if defined( VD_64BIT )

// ============================================================================================== //

template <>
class Atomic<vd::i64>
{
public:
    Atomic()
    {
    }
    
    explicit Atomic(vd::i64 other)
    {
        this->Set(other);
    }
    
    const vd::i64 value() const
    {
        static const vd::i64 zero = 0;
        return AtomicAdd(&m_Value, zero);
    }
    
    bool Set(const volatile vd::i64& other)
    {
        return AtomicCompareAndExchange((volatile vd::i64*) &m_Value, other, m_Value);
    }
    
    Atomic& operator=(const vd::i64& other)
    {
        this->Set(other);
        return *this;
    }
    
    Atomic& operator=(const Atomic& other)
    {
        this->Set(other.m_Value);
        return *this;
    }
    
    bool cas(const vd::i64& oldval, const vd::i64& newval)
    { 
        return AtomicCompareAndExchange((volatile vd::i64*) &m_Value, newval, oldval);
    }
    
    bool operator<(const vd::i64& other) const
    {
        return this->value() < other;
    }
    
    bool operator<(const Atomic& other) const
    {
        return this->value() < other.value();
    }
    
    bool operator<=(const vd::i64& other) const
    {
        return this->value() <= other;
    }
    
    bool operator<=(const Atomic& other) const
    {
        return this->value() <= other.value();
    }
    
    bool operator>(const vd::i64& other) const
    {
        return this->value() > other;
    }
    
    bool operator>(const Atomic& other) const
    {
        return this->value() > other.value();
    }
    
    bool operator>=(const vd::i64& other) const
    {
        return this->value() >= other;
    }
    
    bool operator>=(const Atomic& other) const
    {
        return this->value() >= other.value();
    }
    
    bool operator==(const vd::i64& other) const
    {
        return this->value() == other;
    }
    
    bool operator==(const Atomic& other) const
    {
        return this->value() == other.value();
    }
    
    bool operator!=(const vd::i64& other) const
    {
        return this->value() != other;
    }
    
    bool operator!=(const Atomic& other) const
    {
        return this->value() != other.value();
    }
    
    Atomic& operator+=(const vd::i64& other)
    {
        AtomicAdd(&m_Value, other);
        return *this;
    }
    
    Atomic& operator+=(const Atomic& other)
    {
        vd::i64 old = other.value();
        AtomicAdd(&m_Value, old);
        return *this;
    }
    
    Atomic& operator-=(const vd::i64& other)
    {
        AtomicAdd(&m_Value, -other);
        return *this;
    }
    
    Atomic& operator-=(const Atomic& other)
    {
        vd::i64 old = other.value();
        AtomicAdd(&m_Value, -old);
        return *this;
    }
    
    Atomic& operator*=(const vd::i64& other)
    {
        vd::i64 comperand;
        vd::i64 exchange;
        
        do
        {
            comperand = m_Value;
            exchange = comperand * other;
        }
        while(!this->cas(comperand, exchange));
        
        return *this;
    }
    
    Atomic& operator/=(const vd::i64& other)
    {
        vd::i64 comperand;
        vd::i64 exchange;
        
        do
        {
            comperand = m_Value;
            exchange = comperand / other;
        }
        while(!this->cas(comperand, exchange));
        
        return *this;
    }
    
    Atomic& operator%=(const vd::i64& other)
    {
        vd::i64 comperand;
        vd::i64 exchange;
        
        do
        {
            comperand = m_Value;
            exchange = comperand % other;
        }
        while(!this->cas(comperand, exchange));
        
        return *this;
    }
    
    Atomic& operator&=(const vd::i64& other)
    {
        vd::i64 comperand;
        vd::i64 exchange;
        
        do
        {
            comperand = m_Value;
            exchange = comperand & other;
        }
        while(!this->cas(comperand, exchange));
        
        return *this;
    }
    
    Atomic& operator|=(const vd::i64& other)
    {
        vd::i64 comperand;
        vd::i64 exchange;
        
        do
        {
            comperand = m_Value;
            exchange = comperand | other;
        }
        while(!this->cas(comperand, exchange));
        
        return *this;
    }
    
    Atomic& operator^=(const vd::i64& other)
    {
        vd::i64 comperand;
        vd::i64 exchange;
        
        do
        {
            comperand = m_Value;
            exchange = comperand ^ other;
        }
        while(!this->cas(comperand, exchange));
        
        return *this;
    }
    
    const volatile vd::i64& operator++()
    {
        this->operator+=((volatile vd::i64)1);
        return m_Value;
    }
    
    const volatile vd::i64& operator--()
    {
        this->operator-=((volatile vd::i64)1);
        return m_Value;
    }
    
    volatile vd::i64 operator++(int)
    {
        this->operator+=((volatile vd::i64)1);
        return m_Value;
    }
    
    volatile vd::i64 operator--(int)
    {
        this->operator-=((volatile vd::i64)1);
        return m_Value;
    }
    
private:
    
    volatile mutable vd::i64 m_Value;
};

// ============================================================================================== //

#endif

// ============================================================================================== //

class AtomicCounter
{
public:

	explicit inline 
	AtomicCounter () : m_Counter(0)
	{
		// EMPTY!
	}

	explicit inline 
	AtomicCounter (int value)
	{
		m_Counter = value;
	}

	inline ~AtomicCounter ()
	{
		// EMPTY!
	}

	inline bool Set(int v) 
	{
        return AtomicCompareAndExchange((volatile vd::i32*) &m_Counter, v, m_Counter);
	}

	inline int Get() 
	{
	    return AtomicAdd(&m_Counter, 0);
	}
	
	inline operator int ()  
	{
        return AtomicAdd(&m_Counter, 0);
	}
	
	inline int operator++(int)
    {
        return Increment();        
    }
    
	inline int operator--(int)
    {
        return Decrement();        
    }
	
	int Decrement(void) 
	{	
	#if defined(VD_TARGET_WINDOWS)
		int count = InterlockedDecrement(&m_Counter);
	#else
		int count = __sync_sub_and_fetch(&m_Counter, 1);
	#endif
		return count;
	} 

	int Increment(void) 
	{	
	#if defined(VD_TARGET_WINDOWS)
		int count = InterlockedIncrement(&m_Counter);
	#else
		int count = __sync_fetch_and_add(&m_Counter, 1);
	#endif
		return count;
	} 

protected:

#if defined(VD_TARGET_WINDOWS)
    volatile mutable LONG m_Counter;
#else
    volatile mutable int m_Counter;
#endif
	
};

// ============================================================================================== //

template <typename T> 
class AtomicPtr
{
public:

	explicit inline 
	AtomicPtr () : m_Ptr(NULL)
	{
		// EMPTY!
	}

	explicit inline 
	AtomicPtr (T* replace)
	{
		Set(replace);
	}

	inline ~AtomicPtr ()
	{
		// EMPTY!
	}

	inline void Set(T* replace)
	{
		m_Ptr = replace;
	}

    const T* Get() const
    {
#if   defined(VD_TARGET_WINDOWS)
        return m_Ptr;
#elif defined(VD_USE_GCC_ATOMICS)
        return __sync_fetch_and_add(&m_Ptr, 0);
#endif
    }

	inline T* Exchange(T* replace)
	{

#if (defined(VD_TARGET_WINDOWS) || defined(VD_TARGET_OSX) || defined(VD_TARGET_LINUX))

		return AtomicExchangePtr(&m_Ptr, replace);
		
#else
		m_SpinLock.Lock();
		T* old = (T*) m_Ptr;
		m_Ptr = replace;
		m_SpinLock.Unlock();
		return old;
#endif
	}

	inline T* CompareAndSwap(T* newptr, T* oldptr)
	{

#if (defined(VD_TARGET_WINDOWS) || defined(VD_TARGET_OSX) || defined(VD_TARGET_LINUX))

		return AtomicCompareAndExchangePtr(&m_Ptr, newptr, oldptr);
#else

		m_SpinLock.Lock();
		T* old = (T*) m_Ptr;
		if (m_Ptr == existing)
			m_Ptr = replace;
		m_SpinLock.Unlock();
		return old;

#endif
	}

private:

	volatile mutable T* m_Ptr;

#if (defined(VD_TARGET_WINDOWS) || defined(VD_TARGET_OSX) || defined(VD_TARGET_LINUX))

#else
	#warning "AtomicPtr is implemented with a SpinLock for this platform!"
	SpinLock m_SpinLock;
#endif

	VD_DISABLE_COPY_CONSTRUCTORS(AtomicPtr);
};

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_ATOMICS_INCLUDED

