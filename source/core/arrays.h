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

#ifndef VD_CORE_ARRAYS_INCLUDED
#define VD_CORE_ARRAYS_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/numerics.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

struct A1True {};
struct A1False {};

template<typename T> struct A1ValueIsInteger
{
    typedef A1False type;
};
template<> struct A1ValueIsInteger<bool>
{
    typedef A1True type;
};
template<> struct A1ValueIsInteger<vd::i8>
{
    typedef A1True type;
};
template<> struct A1ValueIsInteger<vd::u8>
{
    typedef A1True type;
};
template<> struct A1ValueIsInteger<vd::i16>
{
    typedef A1True type;
};
template<> struct A1ValueIsInteger<vd::u16>
{
    typedef A1True type;
};
template<> struct A1ValueIsInteger<vd::i32>
{
    typedef A1True type;
};
template<> struct A1ValueIsInteger<vd::u32>
{
    typedef A1True type;
};
template<> struct A1ValueIsInteger<vd::i64>
{
    typedef A1True type;
};
template<> struct A1ValueIsInteger<vd::u64>
{
    typedef A1True type;
};
template<> struct A1ValueIsInteger<vd::i128>
{
    typedef A1True type;
};
template<> struct A1ValueIsInteger<vd::u128>
{
    typedef A1True type;
};

// ============================================================================================== //

template<typename T>
struct Array1
{
public:
    typedef T*                                              iterator;
    typedef const T*                                        const_iterator;
    typedef vd::u64                                         size_type;
    typedef vd::i64                                         difference_type;
    typedef T&                                              reference;
    typedef const T&                                        const_reference;
    typedef T                                               value_type;
    typedef T*                                              pointer;
    typedef const T*                                        const_pointer;
    typedef std::reverse_iterator<iterator>                 reverse_iterator;
    typedef std::reverse_iterator<const_iterator>           const_reverse_iterator;

public:
    vd::u64 n;
    vd::u64 max_n;
    T* data;

    Array1(void)
        : n(0), max_n(0), data(0)
    {}

    Array1(vd::u64 n_)
        : n(0), max_n(0), data(0)
    {
        if(n_> VD_U64_MAX/sizeof(T)) throw std::bad_alloc();
        data=(T*)std::calloc(n_, sizeof(T));
        if(!data) throw std::bad_alloc();
        n=n_;
        max_n=n_;
    }

    Array1(vd::u64 n_, const T& value)
        : n(0), max_n(0), data(0)
    {
        if(n_> VD_U64_MAX/sizeof(T)) throw std::bad_alloc();
        data=(T*)std::calloc(n_, sizeof(T));
        if(!data) throw std::bad_alloc();
        n=n_;
        max_n=n_;
        for(vd::u64 i=0; i<n; ++i) data[i]=value;
    }

    Array1(vd::u64 n_, const T& value, vd::u64 max_n_)
        : n(0), max_n(0), data(0)
    {
        vdGlobalAssert(n_<=max_n_);
        if(max_n_> VD_U64_MAX/sizeof(T)) throw std::bad_alloc();
        data=(T*)std::calloc(max_n_, sizeof(T));
        if(!data) throw std::bad_alloc();
        n=n_;
        max_n=max_n_;
        for(vd::u64 i=0; i<n; ++i) data[i]=value;
    }

    Array1(vd::u64 n_, const T* data_)
        : n(0), max_n(0), data(0)
    {
        if(n_> VD_U64_MAX/sizeof(T)) throw std::bad_alloc();
        data=(T*)std::calloc(n_, sizeof(T));
        if(!data) throw std::bad_alloc();
        n=n_;
        max_n=n_;
        vdGlobalAssert(data_);
        std::memcpy(data, data_, n*sizeof(T));
    }

    Array1(vd::u64 n_, const T* data_, vd::u64 max_n_)
        : n(0), max_n(0), data(0)
    {
        vdGlobalAssert(n_<=max_n_);
        if(max_n_> VD_U64_MAX/sizeof(T)) throw std::bad_alloc();
        data=(T*)std::calloc(max_n_, sizeof(T));
        if(!data) throw std::bad_alloc();
        max_n=max_n_;
        n=n_;
        vdGlobalAssert(data_);
        std::memcpy(data, data_, n*sizeof(T));
    }

    Array1(const Array1<T> &x)
        : n(0), max_n(0), data(0)
    {
        data=(T*)std::malloc(x.n*sizeof(T));
        if(!data) throw std::bad_alloc();
        n=x.n;
        max_n=x.n;
        std::memcpy(data, x.data, n*sizeof(T));
    }

    ~Array1(void)
    {
        std::free(data);
#ifndef NDEBUG
        data=0;
        n=max_n=0;
#endif
    }

    const T& operator[](vd::u64 i) const
    {
        return data[i];
    }

    T& operator[](vd::u64 i)
    {
        return data[i];
    }

    // these are range-checked (in debug mode) versions of operator[], like at()
    const T& operator()(vd::u64 i) const
    {
        vdGlobalAssert(i<n);
        return data[i];
    }

    T& operator()(vd::u64 i)
    {
        vdGlobalAssert(i<n);
        return data[i];
    }

    Array1<T>& operator=(const Array1<T>& x)
    {
        if(max_n<x.n)
        {
            T* new_data=(T*)std::malloc(x.n*sizeof(T));
            if(!new_data) throw std::bad_alloc();
            std::free(data);
            data=new_data;
            max_n=x.n;
        }
        n=x.n;
        std::memcpy(data, x.data, n*sizeof(T));
        return *this;
    }

    bool operator==(const Array1<T>& x) const
    {
        if(n!=x.n) return false;
        for(vd::u64 i=0; i<n; ++i) if(!(data[i]==x.data[i])) return false;
        return true;
    }

    bool operator!=(const Array1<T>& x) const
    {
        if(n!=x.n) return true;
        for(vd::u64 i=0; i<n; ++i) if(data[i]!=x.data[i]) return true;
        return false;
    }

    bool operator<(const Array1<T>& x) const
    {
        for(vd::u64 i=0; i<n && i<x.n; ++i)
        {
            if(data[i]<x[i]) return true;
            else if(x[i]<data[i]) return false;
        }
        return n<x.n;
    }

    bool operator>(const Array1<T>& x) const
    {
        for(vd::u64 i=0; i<n && i<x.n; ++i)
        {
            if(data[i]>x[i]) return true;
            else if(x[i]>data[i]) return false;
        }
        return n>x.n;
    }

    bool operator<=(const Array1<T>& x) const
    {
        for(vd::u64 i=0; i<n && i<x.n; ++i)
        {
            if(data[i]<x[i]) return true;
            else if(x[i]<data[i]) return false;
        }
        return n<=x.n;
    }

    bool operator>=(const Array1<T>& x) const
    {
        for(vd::u64 i=0; i<n && i<x.n; ++i)
        {
            if(data[i]>x[i]) return true;
            else if(x[i]>data[i]) return false;
        }
        return n>=x.n;
    }

    void add_unique(const T& value)
    {
        for(vd::u64 i=0; i<n; ++i) if(data[i]==value) return;
        if(n==max_n) grow();
        data[n++]=value;
    }

    void assign(const T& value)
    {
        for(vd::u64 i=0; i<n; ++i) data[i]=value;
    }

    void assign(vd::u64 num, const T& value)
    {
        fill(num, value);
    }

    void assign(vd::u64 num, const T* copydata)
    {
        vdGlobalAssert(num==0 || copydata);
        if(num>max_n)
        {
            if(num> VD_U64_MAX/sizeof(T)) throw std::bad_alloc();
            std::free(data);
            data=(T*)std::malloc(num*sizeof(T));
            if(!data) throw std::bad_alloc();
            max_n=num;
        }
        n=num;
        std::memcpy(data, copydata, n*sizeof(T));
    }

    template<typename InputIterator>
    void assign(InputIterator first, InputIterator last)
    {
        assign_(first, last, typename A1ValueIsInteger<InputIterator>::type());
    }

    template<typename InputIterator>
    void assign_(InputIterator first, InputIterator last, A1True check)
    {
        fill(first, last);
    }

    template<typename InputIterator>
    void assign_(InputIterator first, InputIterator last, A1False check)
    {
        vd::u64 i=0;
        InputIterator p=first;
        for(; p!=last; ++p, ++i)
        {
            if(i==max_n) grow();
            data[i]=*p;
        }
        n=i;
    }

    const T& at(vd::u64 i) const
    {
        vdGlobalAssert(i<n);
        return data[i];
    }

    T& at(vd::u64 i)
    {
        vdGlobalAssert(i<n);
        return data[i];
    }

    const T& back(void) const
    {
        vdGlobalAssert(data && n>0);
        return data[n-1];
    }

    T& back(void)
    {
        vdGlobalAssert(data && n>0);
        return data[n-1];
    }

    const T* begin(void) const
    {
        return data;
    }

    T* begin(void)
    {
        return data;
    }

    vd::u64 capacity(void) const
    {
        return max_n;
    }

    void clear(void)
    {
        std::free(data);
        data=0;
        max_n=0;
        n=0;
    }

    bool empty(void) const
    {
        return n==0;
    }

    const T* end(void) const
    {
        return data+n;
    }

    T* end(void)
    {
        return data+n;
    }

    void erase(vd::u64 index)
    {
        vdGlobalAssert(index<n);
        for(vd::u64 i=index; i<n-1; ++i)
            data[i]=data[i-1];
        pop_back();
    }

    void fill(vd::u64 num, const T& value)
    {
        if(num>max_n)
        {
            if(num> VD_U64_MAX/sizeof(T)) throw std::bad_alloc();
            std::free(data);
            data=(T*)std::malloc(num*sizeof(T));
            if(!data) throw std::bad_alloc();
            max_n=num;
        }
        n=num;
        for(vd::u64 i=0; i<n; ++i) data[i]=value;
    }

    const T& front(void) const
    {
        vdGlobalAssert(n>0);
        return *data;
    }

    T& front(void)
    {
        vdGlobalAssert(n>0);
        return *data;
    }

    void grow(void)
    {
        vd::u64 new_size=(max_n*sizeof(T)< VD_U64_MAX/2 ? 2*max_n+1 :  VD_U64_MAX/sizeof(T));
        T *new_data=(T*)std::realloc(data, new_size*sizeof(T));
        if(!new_data) throw std::bad_alloc();
        data=new_data;
        max_n=new_size;
    }

    void insert(vd::u64 index, const T& entry)
    {
        vdGlobalAssert(index<=n);
        push_back(back());
        for(vd::u64 i=n-1; i>index; --i)
            data[i]=data[i-1];
        data[index]=entry;
    }

    vd::u64 max_size(void) const
    {
        return  VD_U64_MAX/sizeof(T);
    }

    void pop_back(void)
    {
        vdGlobalAssert(n>0);
        --n;
    }

    void push_back(const T& value)
    {
        if(n==max_n) grow();
        data[n++]=value;
    }

    reverse_iterator rbegin(void)
    {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin(void) const
    {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend(void)
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend(void) const
    {
        return const_reverse_iterator(begin());
    }

    void reserve(vd::u64 r)
    {
        if(r> VD_U64_MAX/sizeof(T)) throw std::bad_alloc();
        T *new_data=(T*)std::realloc(data, r*sizeof(T));
        if(!new_data) throw std::bad_alloc();
        data=new_data;
        max_n=r;
    }

    void resize(vd::u64 n_)
    {
        if(n_>max_n) reserve(n_);
        n=n_;
    }

    void resize(vd::u64 n_, const T& value)
    {
        if(n_>max_n) reserve(n_);
        if(n<n_) for(vd::u64 i=n; i<n_; ++i) data[i]=value;
        n=n_;
    }

    void set_zero(void)
    {
        std::memset(data, 0, n*sizeof(T));
    }

    vd::u64 size(void) const
    {
        return n;
    }

    void swap(Array1<T>& x)
    {
        std::swap(n, x.n);
        std::swap(max_n, x.max_n);
        std::swap(data, x.data);
    }

    // resize the array to avoid wasted space, without changing contents
    // (Note: realloc, at least on some platforms, will not do the trick)
    void trim(void)
    {
        if(n==max_n) return;
        T *new_data=(T*)std::malloc(n*sizeof(T));
        if(!new_data) return;
        std::memcpy(new_data, data, n*sizeof(T));
        std::free(data);
        data=new_data;
        max_n=n;
    }
};

//============================================================================

template<typename T>
struct ProxyArray1
{
    // STL-friendly typedefs

    typedef T* iterator;
    typedef const T* const_iterator;
    typedef vd::u64 size_type;
    typedef vd::i64 difference_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    // the actual representation

    vd::u64 n;
    vd::u64 max_n;
    T* data;

    // most of STL vector's interface, with a few changes

    ProxyArray1(void)
        : n(0), max_n(0), data(0)
    {}

    ProxyArray1(vd::u64 n_, T* data_)
        : n(n_), max_n(n_), data(data_)
    {
        vdGlobalAssert(data || max_n==0);
    }

    ProxyArray1(vd::u64 n_, T* data_, vd::u64 max_n_)
        : n(n_), max_n(max_n_), data(data_)
    {
        vdGlobalAssert(n<=max_n);
        vdGlobalAssert(data || max_n==0);
    }

    // Allow for simple shallow copies of existing arrays
    // Note that if the underlying arrays change where their data is, the ProxyArray may be screwed up

    ProxyArray1(Array1<T>& a)
        : n(a.n), max_n(a.max_n), data(a.data)
    {}

    ProxyArray1(std::vector<T>& a)
        : n(a.size()), max_n(a.capacity()), data(&a[0])
    {}

    void init(vd::u64 n_, T* data_, vd::u64 max_n_)
    {
        vdGlobalAssert(n_<=max_n_);
        vdGlobalAssert(data_ || max_n_==0);
        n=n_;
        max_n=max_n_;
        data=data_;
    }

    const T& operator[](vd::u64 i) const
    {
        return data[i];
    }

    T& operator[](vd::u64 i)
    {
        return data[i];
    }

    // these are range-checked (in debug mode) versions of operator[], like at()
    const T& operator()(vd::u64 i) const
    {
        vdGlobalAssert(i<n);
        return data[i];
    }

    T& operator()(vd::u64 i)
    {
        vdGlobalAssert(i<n);
        return data[i];
    }

    bool operator==(const ProxyArray1<T>& x) const
    {
        if(n!=x.n) return false;
        for(vd::u64 i=0; i<n; ++i) if(!(data[i]==x.data[i])) return false;
        return true;
    }

    bool operator!=(const ProxyArray1<T>& x) const
    {
        if(n!=x.n) return true;
        for(vd::u64 i=0; i<n; ++i) if(data[i]!=x.data[i]) return true;
        return false;
    }

    bool operator<(const ProxyArray1<T>& x) const
    {
        for(vd::u64 i=0; i<n && i<x.n; ++i)
        {
            if(data[i]<x[i]) return true;
            else if(x[i]<data[i]) return false;
        }
        return n<x.n;
    }

    bool operator>(const ProxyArray1<T>& x) const
    {
        for(vd::u64 i=0; i<n && i<x.n; ++i)
        {
            if(data[i]>x[i]) return true;
            else if(x[i]>data[i]) return false;
        }
        return n>x.n;
    }

    bool operator<=(const ProxyArray1<T>& x) const
    {
        for(vd::u64 i=0; i<n && i<x.n; ++i)
        {
            if(data[i]<x[i]) return true;
            else if(x[i]<data[i]) return false;
        }
        return n<=x.n;
    }

    bool operator>=(const ProxyArray1<T>& x) const
    {
        for(vd::u64 i=0; i<n && i<x.n; ++i)
        {
            if(data[i]>x[i]) return true;
            else if(x[i]>data[i]) return false;
        }
        return n>=x.n;
    }

    void add_unique(const T& value)
    {
        for(vd::u64 i=0; i<n; ++i) if(data[i]==value) return;
        vdGlobalAssert(n<max_n);
        data[n++]=value;
    }

    void assign(const T& value)
    {
        for(vd::u64 i=0; i<n; ++i) data[i]=value;
    }

    void assign(vd::u64 num, const T& value)
    {
        fill(num, value);
    }

    void assign(vd::u64 num, const T* copydata)
    {
        vdGlobalAssert(num==0 || copydata);
        vdGlobalAssert(num<=max_n);
        n=num;
        std::memcpy(data, copydata, n*sizeof(T));
    }

    template<typename InputIterator>
    void assign(InputIterator first, InputIterator last)
    {
        assign_(first, last, typename A1ValueIsInteger<InputIterator>::type());
    }

    template<typename InputIterator>
    void assign_(InputIterator first, InputIterator last, A1True check)
    {
        fill(first, last);
    }

    template<typename InputIterator>
    void assign_(InputIterator first, InputIterator last, A1False check)
    {
        vd::u64 i=0;
        InputIterator p=first;
        for(; p!=last; ++p, ++i)
        {
            vdGlobalAssert(i<max_n);
            data[i]=*p;
        }
        n=i;
    }

    const T& at(vd::u64 i) const
    {
        vdGlobalAssert(i<n);
        return data[i];
    }

    T& at(vd::u64 i)
    {
        vdGlobalAssert(i<n);
        return data[i];
    }

    const T& back(void) const
    {
        vdGlobalAssert(data && n>0);
        return data[n-1];
    }

    T& back(void)
    {
        vdGlobalAssert(data && n>0);
        return data[n-1];
    }

    const T* begin(void) const
    {
        return data;
    }

    T* begin(void)
    {
        return data;
    }

    vd::u64 capacity(void) const
    {
        return max_n;
    }

    void clear(void)
    {
        n=0;
    }

    bool empty(void) const
    {
        return n==0;
    }

    const T* end(void) const
    {
        return data+n;
    }

    T* end(void)
    {
        return data+n;
    }

    void erase(vd::u64 index)
    {
        vdGlobalAssert(index<n);
        for(vd::u64 i=index; i<n-1; ++i)
            data[i]=data[i-1];
        pop_back();
    }

    void fill(vd::u64 num, const T& value)
    {
        vdGlobalAssert(num<=max_n);
        n=num;
        for(vd::u64 i=0; i<n; ++i) data[i]=value;
    }

    const T& front(void) const
    {
        vdGlobalAssert(n>0);
        return *data;
    }

    T& front(void)
    {
        vdGlobalAssert(n>0);
        return *data;
    }

    void insert(vd::u64 index, const T& entry)
    {
        vdGlobalAssert(index<=n);
        push_back(back());
        for(vd::u64 i=n-1; i>index; --i)
            data[i]=data[i-1];
        data[index]=entry;
    }

    vd::u64 max_size(void) const
    {
        return max_n;
    }

    void pop_back(void)
    {
        vdGlobalAssert(n>0);
        --n;
    }

    void push_back(const T& value)
    {
        vdGlobalAssert(n<max_n);
        data[n++]=value;
    }

    reverse_iterator rbegin(void)
    {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin(void) const
    {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend(void)
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend(void) const
    {
        return const_reverse_iterator(begin());
    }

    void reserve(vd::u64 r)
    {
        vdGlobalAssert(r<=max_n);
    }

    void resize(vd::u64 n_)
    {
        vdGlobalAssert(n_<=max_n);
        n=n_;
    }

    void resize(vd::u64 n_, const T& value)
    {
        vdGlobalAssert(n_<=max_n);
        if(n<n_) for(vd::u64 i=n; i<n_; ++i) data[i]=value;
        n=n_;
    }

    void set_zero(void)
    {
        std::memset(data, 0, n*sizeof(T));
    }

    vd::u64 size(void) const
    {
        return n;
    }

    void swap(ProxyArray1<T>& x)
    {
        std::swap(n, x.n);
        std::swap(max_n, x.max_n);
        std::swap(data, x.data);
    }
};

// ============================================================================================== //

template<
    class T, 
    class ArrayT = std::vector<T> 
>
struct Array2
{
public:
   typedef typename ArrayT::iterator                    iterator;
   typedef typename ArrayT::const_iterator              const_iterator;
   typedef typename ArrayT::size_type                   size_type;
   typedef vd::i64                                      difference_type;
   typedef T&                                           reference;
   typedef const T&                                     const_reference;
   typedef T                                            value_type;
   typedef T*                                           pointer;
   typedef const T*                                     const_pointer;
   typedef typename ArrayT::reverse_iterator            reverse_iterator;
   typedef typename ArrayT::const_reverse_iterator      const_reverse_iterator;

public:
   int ni, nj;
   ArrayT a;

public:
   Array2(void)
      : ni(0), nj(0)
   {}

   Array2(int ni_, int nj_)
      : ni(ni_), nj(nj_), a(ni_*nj_)
   { vdGlobalAssert(ni_>=0 && nj>=0); }

   Array2(int ni_, int nj_, ArrayT& a_)
      : ni(ni_), nj(nj_), a(a_)
   { vdGlobalAssert(ni_>=0 && nj>=0); }

   Array2(int ni_, int nj_, const T& value)
      : ni(ni_), nj(nj_), a(ni_*nj_, value)
   { vdGlobalAssert(ni_>=0 && nj>=0); }

   Array2(int ni_, int nj_, const T& value, size_type max_n_)
      : ni(ni_), nj(nj_), a(ni_*nj_, value, max_n_)
   { vdGlobalAssert(ni_>=0 && nj>=0); }

   Array2(int ni_, int nj_, T* data_)
      : ni(ni_), nj(nj_), a(ni_*nj_, data_)
   { vdGlobalAssert(ni_>=0 && nj>=0); }

   Array2(int ni_, int nj_, T* data_, size_type max_n_)
      : ni(ni_), nj(nj_), a(ni_*nj_, data_, max_n_)
   { vdGlobalAssert(ni_>=0 && nj>=0); }

   template<class OtherArrayT>
   Array2(Array2<T, OtherArrayT>& other)
      : ni(other.ni), nj(other.nj), a(other.a)
   {}

   ~Array2(void)
   {
#ifndef NDEBUG
      ni=nj=0;
#endif
   }

   const T& operator()(int i, int j) const
   {
      vdGlobalAssert(i>=0 && i<ni && j>=0 && j<nj);
      return a[i+ni*j];
   }

   T& operator()(int i, int j)
   {
      vdGlobalAssert(i>=0 && i<ni && j>=0 && j<nj);
      return a[i+ni*j];
   }

   bool operator==(const Array2<T>& x) const
   { return ni==x.ni && nj==x.nj && a==x.a; }

   bool operator!=(const Array2<T>& x) const
   { return ni!=x.ni || nj!=x.nj || a!=x.a; }

   bool operator<(const Array2<T>& x) const
   {
      if(ni<x.ni) return true; else if(ni>x.ni) return false;
      if(nj<x.nj) return true; else if(nj>x.nj) return false;
      return a<x.a;
   }

   bool operator>(const Array2<T>& x) const
   {
      if(ni>x.ni) return true; else if(ni<x.ni) return false;
      if(nj>x.nj) return true; else if(nj<x.nj) return false;
      return a>x.a;
   }

   bool operator<=(const Array2<T>& x) const
   {
      if(ni<x.ni) return true; else if(ni>x.ni) return false;
      if(nj<x.nj) return true; else if(nj>x.nj) return false;
      return a<=x.a;
   }

   bool operator>=(const Array2<T>& x) const
   {
      if(ni>x.ni) return true; else if(ni<x.ni) return false;
      if(nj>x.nj) return true; else if(nj<x.nj) return false;
      return a>=x.a;
   }

   void assign(const T& value)
   { a.assign(value); }

   void assign(int ni_, int nj_, const T& value)
   {
      a.assign(ni_*nj_, value);
      ni=ni_;
      nj=nj_;
   }
    
   void assign(int ni_, int nj_, const T* copydata)
   {
      a.assign(ni_*nj_, copydata);
      ni=ni_;
      nj=nj_;
   }
    
   const T& at(int i, int j) const
   {
      vdGlobalAssert(i>=0 && i<ni && j>=0 && j<nj);
      return a[i+ni*j];
   }

   T& at(int i, int j)
   {
      vdGlobalAssert(i>=0 && i<ni && j>=0 && j<nj);
      return a[i+ni*j];
   }

   const T& back(void) const
   { 
      vdGlobalAssert(a.size());
      return a.back();
   }

   T& back(void)
   {
      vdGlobalAssert(a.size());
      return a.back();
   }

   const_iterator begin(void) const
   { return a.begin(); }

   iterator begin(void)
   { return a.begin(); }

   size_type capacity(void) const
   { return a.capacity(); }

   void clear(void)
   {
      a.clear();
      ni=nj=0;
   }

   bool empty(void) const
   { return a.empty(); }

   const_iterator end(void) const
   { return a.end(); }

   iterator end(void)
   { return a.end(); }

   void fill(int ni_, int nj_, const T& value)
   {
      a.fill(ni_*nj_, value);
      ni=ni_;
      nj=nj_;
   }
    
   const T& front(void) const
   {
      vdGlobalAssert(a.size());
      return a.front();
   }

   T& front(void)
   {
      vdGlobalAssert(a.size());
      return a.front();
   }

   size_type max_size(void) const
   { return a.max_size(); }

   reverse_iterator rbegin(void)
   { return reverse_iterator(end()); }

   const_reverse_iterator rbegin(void) const
   { return const_reverse_iterator(end()); }

   reverse_iterator rend(void)
   { return reverse_iterator(begin()); }

   const_reverse_iterator rend(void) const
   { return const_reverse_iterator(begin()); }

   void reserve(int reserve_ni, int reserve_nj)
   { a.reserve(reserve_ni*reserve_nj); }

   void resize(int ni_, int nj_)
   {
      vdGlobalAssert(ni_>=0 && nj_>=0);
      a.resize(ni_*nj_);
      ni=ni_;
      nj=nj_;
   }

   void resize(int ni_, int nj_, const T& value)
   {
      vdGlobalAssert(ni_>=0 && nj_>=0);
      a.resize(ni_*nj_, value);
      ni=ni_;
      nj=nj_;
   }

   void set_zero(void)
   { a.set_zero(); }

   size_type size(void) const
   { return a.size(); }

   void swap(Array2<T>& x)
   {
      std::swap(ni, x.ni);
      std::swap(nj, x.nj);
      a.swap(x.a);
   }

   void trim(void)
   { a.trim(); }
};

// ============================================================================================== //

template<
    class T, 
    class ArrayT = std::vector<T> 
>
struct Array3
{
public:
   typedef typename ArrayT::iterator                iterator;
   typedef typename ArrayT::const_iterator          const_iterator;
   typedef typename ArrayT::size_type               size_type;
   typedef vd::i64                                  difference_type;
   typedef T&                                       reference;
   typedef const T&                                 const_reference;
   typedef T                                        value_type;
   typedef T*                                       pointer;
   typedef const T*                                 const_pointer;
   typedef typename ArrayT::reverse_iterator        reverse_iterator;
   typedef typename ArrayT::const_reverse_iterator  const_reverse_iterator;

public:
   int ni, nj, nk;
   ArrayT a;

public:
   Array3(void)
      : ni(0), nj(0), nk(0)
   {}

   Array3(int ni_, int nj_, int nk_)
      : ni(ni_), nj(nj_), nk(nk_), a(ni_*nj_*nk_)
   { vdGlobalAssert(ni_>=0 && nj_>=0 && nk_>=0); }

   Array3(int ni_, int nj_, int nk_, ArrayT& a_)
      : ni(ni_), nj(nj_), nk(nk_), a(a_)
   { vdGlobalAssert(ni_>=0 && nj_>=0 && nk_>=0); }

   Array3(int ni_, int nj_, int nk_, const T& value)
      : ni(ni_), nj(nj_), nk(nk_), a(ni_*nj_*nk_, value)
   { vdGlobalAssert(ni_>=0 && nj_>=0 && nk_>=0); }

   Array3(int ni_, int nj_, int nk_, const T& value, size_type max_n_)
      : ni(ni_), nj(nj_), nk(nk_), a(ni_*nj_*nk_, value, max_n_)
   { vdGlobalAssert(ni_>=0 && nj_>=0 && nk_>=0); }

   Array3(int ni_, int nj_, int nk_, T* data_)
      : ni(ni_), nj(nj_), nk(nk_), a(ni_*nj_*nk_, data_)
   { vdGlobalAssert(ni_>=0 && nj_>=0 && nk_>=0); }

   Array3(int ni_, int nj_, int nk_, T* data_, size_type max_n_)
      : ni(ni_), nj(nj_), nk(nk_), a(ni_*nj_*nk_, data_, max_n_)
   { vdGlobalAssert(ni_>=0 && nj_>=0 && nk_>=0); }

   ~Array3(void)
   {
#ifndef NDEBUG
      ni=nj=0;
#endif
   }

   const T& operator()(int i, int j, int k) const
   {
      vdGlobalAssert(i>=0 && i<ni && j>=0 && j<nj && k>=0 && k<nk);
      return a[i+ni*(j+nj*k)];
   }

   T& operator()(int i, int j, int k)
   {
      vdGlobalAssert(i>=0 && i<ni && j>=0 && j<nj && k>=0 && k<nk);
      return a[i+ni*(j+nj*k)];
   }

   bool operator==(const Array3<T>& x) const
   { return ni==x.ni && nj==x.nj && nk==x.nk && a==x.a; }

   bool operator!=(const Array3<T>& x) const
   { return ni!=x.ni || nj!=x.nj || nk!=x.nk || a!=x.a; }

   bool operator<(const Array3<T>& x) const
   {
      if(ni<x.ni) return true; else if(ni>x.ni) return false;
      if(nj<x.nj) return true; else if(nj>x.nj) return false;
      if(nk<x.nk) return true; else if(nk>x.nk) return false;
      return a<x.a;
   }

   bool operator>(const Array3<T>& x) const
   {
      if(ni>x.ni) return true; else if(ni<x.ni) return false;
      if(nj>x.nj) return true; else if(nj<x.nj) return false;
      if(nk>x.nk) return true; else if(nk<x.nk) return false;
      return a>x.a;
   }

   bool operator<=(const Array3<T>& x) const
   {
      if(ni<x.ni) return true; else if(ni>x.ni) return false;
      if(nj<x.nj) return true; else if(nj>x.nj) return false;
      if(nk<x.nk) return true; else if(nk>x.nk) return false;
      return a<=x.a;
   }

   bool operator>=(const Array3<T>& x) const
   {
      if(ni>x.ni) return true; else if(ni<x.ni) return false;
      if(nj>x.nj) return true; else if(nj<x.nj) return false;
      if(nk>x.nk) return true; else if(nk<x.nk) return false;
      return a>=x.a;
   }

   void assign(const T& value)
   { a.assign(value); }

   void assign(int ni_, int nj_, int nk_, const T& value)
   {
      a.assign(ni_*nj_*nk_, value);
      ni=ni_;
      nj=nj_;
      nk=nk_;
   }
    
   void assign(int ni_, int nj_, int nk_, const T* copydata)
   {
      a.assign(ni_*nj_*nk_, copydata);
      ni=ni_;
      nj=nj_;
      nk=nk_;
   }
    
   const T& at(int i, int j, int k) const
   {
      vdGlobalAssert(i>=0 && i<ni && j>=0 && j<nj && k>=0 && k<nk);
      return a[i+ni*(j+nj*k)];
   }

   T& at(int i, int j, int k)
   {
      vdGlobalAssert(i>=0 && i<ni && j>=0 && j<nj && k>=0 && k<nk);
      return a[i+ni*(j+nj*k)];
   }

   const T& back(void) const
   { 
      vdGlobalAssert(a.size());
      return a.back();
   }

   T& back(void)
   {
      vdGlobalAssert(a.size());
      return a.back();
   }

   const_iterator begin(void) const
   { return a.begin(); }

   iterator begin(void)
   { return a.begin(); }

   size_type capacity(void) const
   { return a.capacity(); }

   void clear(void)
   {
      a.clear();
      ni=nj=nk=0;
   }

   bool empty(void) const
   { return a.empty(); }

   const_iterator end(void) const
   { return a.end(); }

   iterator end(void)
   { return a.end(); }

   void fill(int ni_, int nj_, int nk_, const T& value)
   {
      a.fill(ni_*nj_*nk_, value);
      ni=ni_;
      nj=nj_;
      nk=nk_;
   }
    
   const T& front(void) const
   {
      vdGlobalAssert(a.size());
      return a.front();
   }

   T& front(void)
   {
      vdGlobalAssert(a.size());
      return a.front();
   }

   size_type max_size(void) const
   { return a.max_size(); }

   reverse_iterator rbegin(void)
   { return reverse_iterator(end()); }

   const_reverse_iterator rbegin(void) const
   { return const_reverse_iterator(end()); }

   reverse_iterator rend(void)
   { return reverse_iterator(begin()); }

   const_reverse_iterator rend(void) const
   { return const_reverse_iterator(begin()); }

   void reserve(int reserve_ni, int reserve_nj, int reserve_nk)
   { a.reserve(reserve_ni*reserve_nj*reserve_nk); }

   void resize(int ni_, int nj_, int nk_)
   {
      vdGlobalAssert(ni_>=0 && nj_>=0 && nk_>=0);
      a.resize(ni_*nj_*nk_);
      ni=ni_;
      nj=nj_;
      nk=nk_;
   }

   void resize(int ni_, int nj_, int nk_, const T& value)
   {
      vdGlobalAssert(ni_>=0 && nj_>=0 && nk_>=0);
      a.resize(ni_*nj_*nk_, value);
      ni=ni_;
      nj=nj_;
      nk=nk_;
   }

   void set_zero(void)
   { a.set_zero(); }

   size_type size(void) const
   { return a.size(); }

   void swap(Array3<T>& x)
   {
      std::swap(ni, x.ni);
      std::swap(nj, x.nj);
      std::swap(nk, x.nk);
      a.swap(x.a);
   }

   void trim(void)
   { a.trim(); }
};

// ============================================================================================== //

typedef Array1<vd::f64>                                                 Array1f64;
typedef Array1<vd::f32>                                                 Array1f32;
typedef Array1<vd::i64>                                                 Array1i64;
typedef Array1<vd::u64> 		                                        Array1u64;
typedef Array1<vd::i32>                                                 Array1i32;
typedef Array1<vd::u32>       	                                        Array1u32;
typedef Array1<vd::i16>                                                 Array1i16;
typedef Array1<vd::u16>     	                                        Array1u16;
typedef Array1<vd::i8>                                                  Array1i8;
typedef Array1<vd::u8>      	                                        Array1u8;

// ============================================================================================== //

typedef ProxyArray1<vd::f64>                                            ProxyArray1f64;
typedef ProxyArray1<vd::f32>                                            ProxyArray1f32;
typedef ProxyArray1<vd::i64>                                            ProxyArray1i64;
typedef ProxyArray1<vd::u64> 		                                    ProxyArray1u64;
typedef ProxyArray1<vd::i32>                                            ProxyArray1i32;
typedef ProxyArray1<vd::u32>       	                                    ProxyArray1u32;
typedef ProxyArray1<vd::i16>                                            ProxyArray1i16;
typedef ProxyArray1<vd::u16>     	                                    ProxyArray1u16;
typedef ProxyArray1<vd::i8>                                             ProxyArray1i8;
typedef ProxyArray1<vd::u8>      	                                    ProxyArray1u8;

// ============================================================================================== //

typedef Array2<vd::f64, Array1<vd::f64> >                               Array2f64;
typedef Array2<vd::f32, Array1<vd::f32> >                               Array2f32;
typedef Array2<vd::i64, Array1<vd::i64> >                               Array2i64;
typedef Array2<vd::u64, Array1<vd::u64> >                               Array2u64;
typedef Array2<vd::i32, Array1<vd::i32> >                               Array2i32;
typedef Array2<vd::u32, Array1<vd::u32> >                               Array2u32;
typedef Array2<vd::i16, Array1<vd::i16> >                               Array2i16;
typedef Array2<vd::u16, Array1<vd::u16> >                               Array2u16;
typedef Array2<vd::i8,  Array1<vd::i8> >                                Array2i8;
typedef Array2<vd::u8,  Array1<vd::u8> >                                Array2u8;

// ============================================================================================== //

typedef Array2<vd::f64, ProxyArray1<vd::f64> >                          ProxyArray2f64;
typedef Array2<vd::f32, ProxyArray1<vd::f32> >                          ProxyArray2f32;
typedef Array2<vd::i64, ProxyArray1<vd::i64> >                          ProxyArray2i64;
typedef Array2<vd::u64, ProxyArray1<vd::u64> >                          ProxyArray2u64;
typedef Array2<vd::i32, ProxyArray1<vd::i32> >                          ProxyArray2i32;
typedef Array2<vd::u32, ProxyArray1<vd::u32> >                          ProxyArray2u32;
typedef Array2<vd::i16, ProxyArray1<vd::i16> >                          ProxyArray2i16;
typedef Array2<vd::u16, ProxyArray1<vd::u16> >                          ProxyArray2u16;
typedef Array2<vd::i8,  ProxyArray1<vd::i8> >                           ProxyArray2i8;
typedef Array2<vd::u8,  ProxyArray1<vd::u8> >                           ProxyArray2u8;

// ============================================================================================== //

typedef Array3<vd::f64, Array1<vd::f64> >                               Array3f64;
typedef Array3<vd::f32, Array1<vd::f32> >                               Array3f32;
typedef Array3<vd::i64, Array1<vd::i64> >                               Array3i64;
typedef Array3<vd::u64, Array1<vd::u64> >                               Array3u64;
typedef Array3<vd::i32, Array1<vd::i32> >                               Array3i32;
typedef Array3<vd::u32, Array1<vd::u32> >                               Array3u32;
typedef Array3<vd::i16, Array1<vd::i16> >                               Array3i16;
typedef Array3<vd::u16, Array1<vd::u16> >                               Array3u16;
typedef Array3<vd::i8,  Array1<vd::i8> >                                Array3i8;
typedef Array3<vd::u8,  Array1<vd::u8> >                                Array3u8;

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif	// VD_CORE_ARRAYS_INCLUDED
