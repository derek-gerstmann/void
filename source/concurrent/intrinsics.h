#if defined(VD_TARGET_OSX)
#include <libkern/OSAtomic.h>
#endif

#if defined(VD_TARGET_WINDOWS)
#include <intrin.h>
#endif

namespace vd {
namespace memory {

#if defined(VD_TARGET_WINDOWS)

template <class T>
T AtomicDecrement32(volatile T& t)
{
	vdStaticAssert(sizeof(T) == sizeof(LONG), TypeSizeMisMatch);
    return InterlockedDecrement((volatile LONG*)&t);
}

template <class T>
T AtomicIncrement32(volatile T& t)
{
	vdStaticAssert(sizeof(T) == sizeof(LONG), TypeSizeMisMatch);
    return InterlockedIncrement((volatile LONG*)&t);
}

template <class T>
T AtomicAdd32(volatile T& t, T v)
{
	vdStaticAssert32(sizeof(T) == sizeof(LONG), TypeSizeMisMatch);
    return InterlockedExchangeAdd((volatile LONG*)&t, (LONG)v) + v;
}

template <class T>
T AtomicCompareAndSwap32(volatile T& t, T newvalue, T comparand)
{
	vdStaticAssert(sizeof(T) == sizeof(LONG), TypeSizeMisMatch);
    return InterlockedCompareExchange((volatile LONG*)&t, (LONG)newvalue, (LONG)comparand);
}

template <class T>
T AtomicSwap32(volatile T& t, T newvalue)
{
	vdStaticAssert(sizeof(T) == sizeof(LONG), TypeSizeMisMatch);
    return InterlockedExchange((volatile LONG*)&t, (LONG)newvalue);
}

inline bool
AtomicTestAndSet32(volatile void *address, int bit = 0)
{
    return !!InterlockedBitTestAndSet((volatile LONG*)address + (bit >> 5), (LONG)(0x80000000 >> (bit & 31)));
}

inline bool 
AtomicTestAndClear32(volatile void *address, int bit = 0)
{
    return !!InterlockedBitTestAndReset((volatile LONG*)address + (bit >> 5), (LONG)(0x80000000 >> (bit & 31)));
}

#if defined( VD_64BIT )

template <class T>
T AtomicDecrement64(volatile T& t)
{
	vdStaticAssert(sizeof(T) == sizeof(LONGLONG), TypeSizeMisMatch);
    return InterlockedDecrement64((volatile LONGLONG*)&t);
}

template <class T>
T AtomicIncrement64(volatile T& t)
{
	vdStaticAssert(sizeof(T) == sizeof(LONGLONG), TypeSizeMisMatch);
    return InterlockedIncrement64((volatile LONGLONG*)&t);
}

template <class T>
T AtomicAdd64(volatile T& t, T v)
{
	vdStaticAssert(sizeof(T) == sizeof(LONGLONG), TypeSizeMisMatch);
    return InterlockedExchangeAdd64((volatile LONGLONG*)&t, (LONGLONG)v) + v;
}

template <class T>
T AtomicCompareAndSwap64(volatile T& t, T newvalue, T comparand)
{
	vdStaticAssert(sizeof(T) == sizeof(LONGLONG), TypeSizeMisMatch);
    return InterlockedCompareExchange64((volatile LONGLONG*)&t, (LONGLONG)newvalue, (LONGLONG)comparand);
}

template <class T>
T AtomicSwap64(volatile T& t, T newvalue)
{
	vdStaticAssert(sizeof(T) == sizeof(LONGLONG), TypeSizeMisMatch);
    return InterlockedExchange64((volatile LONGLONG*)&t, (LONGLONG)newvalue);
}
#endif

#elif defined(VD_TARGET_OSX)

#include <libkern/OSAtomic.h>

template <class T>
T AtomicDecrement32(volatile T &t)
{
	vdStaticAssert(sizeof(T) == sizeof(int32_t), TypeSizeMisMatch);
    return OSAtomicDecrement32Barrier((volatile int32_t *)&t);
}

template <class T>
T AtomicIncrement32(volatile T &t)
{
	vdStaticAssert(sizeof(T) == sizeof(int32_t), TypeSizeMisMatch);
    return OSAtomicIncrement32Barrier((volatile int32_t *)&t);
}

template <class T>
T AtomicAdd32(volatile T &t, T v)
{
	vdStaticAssert(sizeof(T) == sizeof(int32_t), TypeSizeMisMatch);
    return OSAtomicAdd32Barrier((int32_t)v, (volatile int32_t *)&t);
}

template <class T>
T AtomicCompareAndSwap32(volatile T &t, T newvalue, T comparand)
{
	vdStaticAssert(sizeof(T) == sizeof(int32_t), TypeSizeMisMatch);
    return OSAtomicCompareAndSwap32Barrier((int32_t)comparand, (int32_t)newvalue, (volatile int32_t *)&t) ? comparand : t;
}

template <class T>
T AtomicSwap32(volatile T &t, T newvalue)
{
	vdStaticAssert(sizeof(T) == sizeof(int32_t), TypeSizeMisMatch);

    int32_t comparand = (int32_t)t;
    while (!OSAtomicCompareAndSwap32Barrier((int32_t)comparand, (int32_t)newvalue, (volatile int32_t *) &t))
        comparand = (int32_t)t;
    return comparand;
}

inline bool
AtomicTestAndSet32(volatile void *addr, int bit = 0)
{
    return OSAtomicTestAndSetBarrier((uint32_t)bit, addr);
}

template <class T>
bool AtomicTestAndClear32(volatile void *addr, int bit = 0)
{
    return OSAtomicTestAndClearBarrier((uint32_t)bit, addr);
}

#if defined( VD_64BIT )

template <class T>
T AtomicDecrement64(volatile T &t)
{
	vdStaticAssert(sizeof(T) == sizeof(int64_t), TypeSizeMisMatch);
    return OSAtomicDecrement64Barrier((volatile int64_t *)&t);
}

template <class T>
T AtomicIncrement64(volatile T &t)
{
	vdStaticAssert(sizeof(T) == sizeof(int64_t), TypeSizeMisMatch);
    return OSAtomicIncrement64Barrier((volatile int64_t *)&t);
}

template <class T>
T AtomicAdd64(volatile T &t, T v)
{
	vdStaticAssert(sizeof(T) == sizeof(int64_t), TypeSizeMisMatch);
    return OSAtomicAdd64Barrier((int64_t)v, (volatile int64_t *)&t);
}

template <class T>
T AtomicCompareAndSwap(volatile T &t, T newvalue, T comparand)
{
	vdStaticAssert(sizeof(T) == sizeof(int64_t), TypeSizeMisMatch);
    return OSAtomicCompareAndSwap64Barrier((int64_t)comparand, (int64_t)newvalue, (volatile int64_t *)&t) ? comparand : t;
}

template <class T>
T AtomicSwap64(volatile T &t, T newvalue)
{
	vdStaticAssert(sizeof(T) == sizeof(int64_t), TypeSizeMisMatch);

    int64_t comparand = (int64_t)t;
    while (!OSAtomicCompareAndSwap64Barrier((int64_t)comparand, (int64_t)newvalue, (volatile int64_t *)&t))
        comparand = (int64_t)t;
    return comparand;
}
#endif

#elif (__GNUC__ == 4 && __GNUC_MINOR__ >= 1 && !defined(__arm__))

template <class T>
T AtomicDecrement(volatile T& t) 
{ 
	vdStaticAssert(sizeof(T) <= sizeof(void *), TypeSizeMisMatch);
	return __sync_sub_and_fetch(&t, 1); 
}

template <class T>
T AtomicIncrement(volatile T& t) 
{ 
	vdStaticAssert(sizeof(T) <= sizeof(void *), TypeSizeMisMatch);
	return __sync_add_and_fetch(&t, 1); 
}

template <class T>
T AtomicAdd(volatile T& t, T v) 
{ 
	vdStaticAssert(sizeof(T) <= sizeof(void *), TypeSizeMisMatch);
	return __sync_add_and_fetch(&t, v); 
}

template <class T>
AtomicCompareAndSwap(volatile T &t, T newvalue, T comparand)
{
    vdStaticAssert(sizeof(T) <= sizeof(void *), TypeSizeMisMatch);
    return __sync_val_compare_and_swap((volatile T *)&t, comparand, newvalue); 
}

template <class T>
AtomicSwap(volatile T &t, T newvalue)
{ 
    vdStaticAssert(sizeof(T) <= sizeof(void *), TypeSizeMisMatch);
    return __sync_lock_test_and_set(&t, newvalue); 
}

inline bool
AtomicTestAndSet(volatile void *address, int bit = 0)
{
    int mask = (1 << (sizeof(int) * 8 - 1)) >> (bit & (sizeof(int) * 8 - 1));
    volatile int &target = *(volatile int *)((intptr_t)address >> (sizeof(int) >> 3));
    int oldvalue, newvalue;
    do {
        oldvalue = target;
        newvalue = oldvalue | mask;
    } while (newvalue != AtomicCompareAndSwap(target, newvalue, oldvalue));
    return !!(oldvalue & mask);
}

inline bool
AtomicTestAndClear(volatile void *address, int bit = 0)
{
    int mask = (1 << (sizeof(int) * 8 - 1)) >> (bit & (sizeof(int) * 8 - 1));
    volatile int &target = *(volatile int *)((intptr_t)address >> (sizeof(int) >> 3));
    int oldvalue, newvalue;
    do {
        oldvalue = target;
        newvalue = oldvalue & ~mask;
    } while (newvalue != AtomicCompareAndSwap(target, newvalue, oldvalue));
    return !!(oldvalue & mask);
}

template <typename T>
class Atomic
{
public:
    Atomic(T value = 0) : m_Value(value) { }

    operator T(void) const { return AtomicAdd(m_Value, T(0)); }
    T operator +=(T v) { return AtomicAdd(m_Value, v); }
    T operator -=(T v) { return AtomicAdd(m_Value, -v); }
    T operator ++(void) { return AtomicIncrement(m_Value); }
    T operator --(void) { return AtomicDecrement(m_Value); }
    T operator ++(int) { return AtomicIncrement(m_Value) - 1; }
    T operator --(int) { return AtomicDecrement(m_Value) + 1; }

private:
    mutable T m_Value;
};

#if defined(_MSC_VER)
inline void CompilerReadWriteBarrier()  { _ReadWriteBarrier(); }
inline void CompilerReadBarrier()       { _ReadBarrier(); }
inline void CompilerWriteBarrier()      { _WriteBarrier(); }
#elif defined(__GNUC__)
inline void CompilerReadWriteBarrier()  { __asm__ __volatile__ ("" ::: "memory"); }
inline void CompilerReadBarrier()       { CompilerReadWriteBarrier(); }
inline void CompilerWriteBarrier()      { CompilerReadWriteBarrier(); }
#endif

}

#endif
