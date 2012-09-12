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

#ifndef VD_FRAMEWORK_TYPES_INCLUDED
#define VD_FRAMEWORK_TYPES_INCLUDED

// ============================================================================================== //

#include "vd.h"

// ============================================================================================== //

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================================== //

#if (defined(VD_TARGET_WINDOWS) && defined(_MSC_VER))

// ============================================================================================== //

typedef signed   __int8                      vd_flag;
typedef signed   __int8                      vd_i8;
typedef unsigned __int8                      vd_u8;
typedef signed   __int16                     vd_i16;
typedef unsigned __int16                     vd_u16;
typedef signed   __int32                     vd_i32;
typedef unsigned __int32                     vd_u32;
typedef signed   __int64                     vd_i64;
typedef unsigned __int64                     vd_u64;
typedef unsigned __int16                     vd_f16;
typedef float                                vd_f32;
typedef double                               vd_f64;
typedef unsigned char                        vd_char;
typedef wchar_t                              vd_utf8;
typedef size_t                               vd_size;
typedef vd_u32                               vd_tag;
typedef intptr_t                             vd_address;
typedef void*                                vd_ptr;

// ============================================================================================== //

#else

// ============================================================================================== //

#include <stdint.h>

typedef int8_t                               vd_flag;
typedef int8_t                               vd_i8;
typedef uint8_t                              vd_u8;
typedef int16_t                              vd_i16;
typedef uint16_t                             vd_u16;
typedef int32_t                              vd_i32;
typedef uint32_t                             vd_u32;
typedef int64_t                              vd_i64;
typedef uint64_t                             vd_u64;
typedef uint16_t                             vd_f16;
typedef float                                vd_f32;
typedef double                               vd_f64;
typedef char                                 vd_char;
typedef wchar_t                              vd_utf8;
typedef size_t                               vd_size;
typedef vd_u32                               vd_tag;
typedef intptr_t                             vd_address;
typedef void*                                vd_ptr;

#endif

// ============================================================================================== //

typedef vd_u32                               vd_code;
typedef vd_u32                               vd_status;
typedef vd_u64                               vd_uid[2];

// ============================================================================================== //

#if     defined(VD_USE_DOUBLE_PRECISION)
typedef vd_i64                               vd_int;
typedef vd_f64                               vd_real;
#ifdef VD_USE_SINGLE_PRECISION
#undef VD_USE_SINGLE_PRECISION            
#endif
#elif   defined(VD_USE_SINGLE_PRECISION)
typedef vd_i32                               vd_int;
typedef vd_f32                               vd_real;
#else
#define VD_USE_SINGLE_PRECISION              1
typedef vd_i32                               vd_int;
typedef vd_f32                               vd_real;

// ============================================================================================== //
#warning "No numeric precision flag defined!  Using single precision floating-point for numeric values!"
// ============================================================================================== //

#endif

// ============================================================================================== //

#ifdef __cplusplus
}
#endif

// ============================================================================================== //

VD_NAMESPACE_BEGIN();

// ============================================================================================== //

#if defined(VD_USE_HALF)
#include <half.h>
#endif

// ============================================================================================== //

#if (defined(VD_TARGET_WINDOWS) && defined(_MSC_VER))

// ============================================================================================== //

typedef signed   __int8    	                 flag;
typedef signed   __int8    	                 i8;
typedef unsigned __int8    	                 u8;
typedef signed   __int16   	                 i16;
typedef unsigned __int16   	                 u16;
typedef signed   __int32   	                 i32;
typedef unsigned __int32                     u32;
typedef signed   __int64                     i64;
typedef unsigned __int64                     u64;
#if defined(VD_USE_HALF)
typedef half                                 f16;
#else
typedef unsigned __int16                     f16;
#endif
typedef float                                f32;
typedef double                               f64;
typedef wchar_t                              wchar;
typedef size_t                               bytesize;
typedef u32 				                 tag;
typedef intptr_t                             address;
typedef void*                                ptr;

// ============================================================================================== //

#else // Unix 

// ============================================================================================== //

#include <stdint.h>

#if defined(VD_USE_ALIGNED_TYPES)
typedef int8_t          	                 flag    __attribute__((aligned(1)));
typedef int8_t          	                 i8	  	 __attribute__((aligned(1)));
typedef uint8_t         	                 u8		 __attribute__((aligned(1)));
typedef int16_t         	                 i16     __attribute__((aligned(2)));
typedef uint16_t        	                 u16     __attribute__((aligned(2)));
typedef int32_t         	                 i32     __attribute__((aligned(4)));
typedef uint32_t        	                 u32     __attribute__((aligned(4)));
typedef int64_t         	                 i64     __attribute__((aligned(8)));
typedef uint64_t        	                 u64     __attribute__((aligned(8)));
#if defined(VD_USE_HALF)
typedef half                                 f16     __attribute__((aligned(2)));
#else
typedef u16                                  f16     __attribute__((aligned(2)));
#endif
typedef float           	                 f32     __attribute__((aligned(4)));
typedef double          	                 f64	 __attribute__((aligned(8)));
typedef wchar_t                              wchar_t __attribute__((aligned(sizeof(wchar_t))));
typedef size_t                               bytesize;
typedef u32 				                 tag;
typedef intptr_t                             address;
typedef void*                                ptr;
#else
typedef int8_t                               flag;
typedef int8_t                               i8;
typedef uint8_t                              u8;
typedef int16_t                              i16;
typedef uint16_t                             u16;
typedef int32_t                              i32;
typedef uint32_t                             u32;
typedef int64_t                              i64;
typedef uint64_t                             u64;
#if defined(VD_USE_HALF)
typedef half                                 f16;
#else
typedef uint16_t                             f16;
#endif
typedef float                                f32;
typedef double                               f64;
typedef wchar_t                              wchar;
typedef size_t                               bytesize;
typedef u32                                  tag;
typedef intptr_t                             address;
typedef void*                                ptr;
#endif
#endif

// ============================================================================================== //

#if     defined(VD_USE_DOUBLE_PRECISION)
typedef f64                                  real;
#ifdef VD_USE_SINGLE_PRECISION
#undef VD_USE_SINGLE_PRECISION            
#endif
#elif   defined(VD_USE_SINGLE_PRECISION)
typedef f32                                  real;
#else
#define VD_USE_SINGLE_PRECISION              1
typedef f32                                  real;
#warning "No numeric precision flag defined!  Using single precision floating-point for numeric values!"
#endif

// ============================================================================================== //


template<bool> 
class CompileTimeCheck
{
public:
    CompileTimeCheck(...) {}
};

template<> 
class CompileTimeCheck<false>
{

};

// ============================================================================================== //

#define vdStaticAssert(test, msg)       	                    \
    do {                                                        \
        struct VD_ERROR_##msg {};              		            \
        typedef CompileTimeCheck< (test) != 0 > check;          \
        check c = check(VD_ERROR_##msg());              		\
        size_t s = sizeof(c);                                 	\
        (void)(s);                            					\
    } while (0)

#define VD_STATIC_ASSERT_MSG(test, msg) vdStaticAssert(test, msg)
#define VD_STATIC_ASSERT(test) vdStaticAssert(test, CompileTimeAssertionFailed)

// ============================================================================================== //

template<typename T>
T AsType(void* v){ return reinterpret_cast<T*>(v); }

#define VD_TYPE_CHECK(T, S)                                    \
  while (false) {                                              \
    *(static_cast<T* volatile*>(0)) = static_cast<S*>(0);      \
  }

// ============================================================================================== //

#define VD_FSTR_LENGTH                         256u

typedef wchar*                		  	       cwstr;
typedef wchar                 		  	       fwstr[VD_FSTR_LENGTH];
typedef std::wstring              		       vwstr;
typedef std::wstring                           unicode;

typedef char*                   		  	   cstr;
typedef char                    		  	   fstr[VD_FSTR_LENGTH];
typedef std::string               		       vstr;
typedef std::string                            ascii;

#if defined(VD_USE_UNICODE)
typedef std::ostringstream< wchar_t >          stringstream; 
typedef std::wstring                           string;
#else
typedef std::string 		          	       string;
typedef std::ostringstream        		       stringstream;
#endif

// ============================================================================================== //

struct VD_API Status
{
    VD_DECLARE_ENUM(Code,
        Success,
        FatalError,
        UncaughtException,
        UnknownError,
        RuntimeError,
        ExecError,
        StartupError,
        ShutdownError,
        Cancel,
        Abort,
        Queued,
        Starting,
        Running,
        Exiting,
        Defer,
        Reject,
        Accept,
        Bypass,
        OutOfMemory,
        InvalidRequest,
        InvalidContext,
        InvalidValue,
        InvalidEnum,
        InvalidFile,
        InvalidFileName,
        InvalidDirectoryName,
        InvalidResource,
        InvalidSlot,
        CreateError,
        DestroyError,
        OpenError,
        CloseError,
        LoadError,
        ReadError,
        WriteError,
        SaveError,
        UserStatus);
};

// ============================================================================================== //

typedef Status::Code status;

// ============================================================================================== //

class VD_API uid
{
public:

    explicit uid() 
    {
        m_Data[1] = 0;
        m_Data[0] = 0;
    }

    explicit uid(const u64 low) 
    {
        m_Data[1] = 0;
        m_Data[0] = low;
    }

    explicit uid(const u64 high, const u64 low)
    {
        m_Data[1] = high;
        m_Data[0] = low;
    }

    explicit uid(const char* str)
    {
        *this = FromString(vd::string(str));
    }

    uid& operator = (const uid& rhs)
    {
        m_Data[1] = rhs.m_Data[1];
        m_Data[0] = rhs.m_Data[0];
        return *this;
    }

    uid& operator = (uid& rhs)
    {
        m_Data[1] = rhs.m_Data[1];
        m_Data[0] = rhs.m_Data[0];
        return *this;
    }

    operator bool() const
    {
        return m_Data[0] != 0;        
    }
    
    bool operator == (const uid& rhs) const
    { 
        return m_Data[1] == rhs.m_Data[1] && m_Data[0] == rhs.m_Data[0]; 
    }

    bool operator != (const uid& rhs) const
    { 
        return m_Data[1] != rhs.m_Data[1] || m_Data[0] != rhs.m_Data[0]; 
    }

    bool operator < (const uid& rhs) const
    {
        if(m_Data[1] < rhs.m_Data[1])
            return true;

        if(m_Data[1] > rhs.m_Data[1])
            return false;

        return m_Data[0] < rhs.m_Data[0];
    }

    bool operator > (const uid& rhs) const
    {
        if(m_Data[1] > rhs.m_Data[1])
            return true;

        if(m_Data[1] < rhs.m_Data[1])
            return false;

        return m_Data[0] > rhs.m_Data[0];
    }

    bool operator <= (const uid& rhs) const
    {
        if(m_Data[1] < rhs.m_Data[1])
            return true;

        if(m_Data[1] > rhs.m_Data[1])
            return false;

        return m_Data[0] <= rhs.m_Data[0];
    }

    bool operator >= (const uid& rhs) const
    {
        if(m_Data[1] > rhs.m_Data[1])
            return true;

        if(m_Data[1] < rhs.m_Data[1])
            return false;

        return m_Data[0] >= rhs.m_Data[0];
    }

    uid& operator ++()
    {
        ++m_Data[0];

        if(!m_Data[0])
            ++m_Data[1];

        return *this;
    }

    uid& operator --()
    {
        if(!m_Data[0])
            --m_Data[1];

        --m_Data[0];
        return *this;
    }

    uid& FromString(const vd::string& from);

    const u64& GetLower() const { return m_Data[0]; }
    const u64& GetUpper() const { return m_Data[1]; }

    u64& GetLower() { return m_Data[0]; }
    u64& GetUpper() { return m_Data[1]; }

private:
    u64 m_Data[2];
};

inline std::ostream& operator << (std::ostream& os, const uid& id)
{
    if(id.GetUpper() == 0)
        os << std::hex << id.GetLower() << std::dec;
    else
        os << std::hex << id.GetUpper() << ':' << id.GetLower() << std::dec;

    return os;
}

inline uid operator+ (const uid& a, const u64& b)
{
    uid result = a;
    result.GetLower() += b;

    if(result.GetLower() < a.GetLower())
        ++result.GetUpper();

    return result;
};

inline uid operator- (const uid& a, const u64& b)
{
    uid result = a;
    result.GetLower() -= b;

    if(result.GetLower() > a.GetLower())
        --result.GetUpper();

    return result;
};

inline uid operator & (const uid& a, const uid& b)
{
    uid result = a;
    result.GetLower() &= b.GetLower();
    result.GetUpper() &= b.GetUpper();
    return result;
}

inline uid operator | (const uid& a, const uid& b)
{
    uid result = a;
    result.GetLower() |= b.GetLower();
    result.GetUpper() |= b.GetUpper();
    return result;
}

struct UidEqualTo : std::binary_function<vd::uid, vd::uid, bool>
{
    bool operator()(vd::uid const& x, vd::uid const& y) const
    {
        return x == y;
    }
};

struct UidHash : std::unary_function<vd::uid, std::size_t>
{
    std::size_t operator()(vd::uid const& x) const
    {
        return x.GetLower();
    }
};

// ============================================================================================== //

struct VD_API symbol
{

public:

    VD_FORCE_INLINE 
    symbol()  
        : key(0,0), str(NULL), next(NULL) { }

    VD_FORCE_INLINE 
    symbol(const vd::uid& k, const char* s) 
        : key(k), str(s), next(NULL) { RegisterSelf(); }
    
    VD_FORCE_INLINE 
    symbol(const vd::symbol& s) 
        : key(s.ToKey()), str(s.ToString()), next(NULL) { RegisterSelf(); }

    bool operator==(const symbol& other) const
    {
        return (key == other.key);
    }

    bool operator!=(const symbol& other) const
    {
        return (key != other.key);
    }

    bool operator<(const symbol& other) const
    {
        return (key < other.key);
    }

    operator const char* () const 
    { 
        return str; 
    }
    
    const char* c_str() const 
    { 
        return str; 
    }

    operator const uid& () const 
    { 
        return key; 
    }

    const char* ToString() const 
    {
        return (str == NULL) ? "<null>" : str;
    }

    const uid& ToId() const 
    {
        return key;
    }
    const uid& ToKey() const 
    {
        return key;
    }

    operator bool() const
    {
        return key && (str != NULL);        
    }

    bool IsValid() const 
    {
        return key && (str != NULL); 
    }

    void RegisterSelf(void);

    uid             key;
    const char*     str;
    symbol*         next;
    static symbol*  head;
    static symbol*  tail;

};

// ============================================================================================== //

template <typename T, unsigned int N = 16>
class VD_API array
{
private:
    T       m_Data[N];
    vd::u32 m_Size;

public:

    array() : m_Size(static_cast<vd::u32>(0)) { }
    ~array() { clear(); }

    unsigned int capacity() const { return N; }
    unsigned int size(void) const { return m_Size; }

    void clear()
    {
        while(!empty()) {
            pop_back();
        }
    }

    void push_back (const T& x)
    { 
        if (size() < N) {    
            new (&m_Data[m_Size]) T(x);
            m_Size++;
        } else {
//            vdGlobalException("Failed to append to array -- capacity exceeded!");
        }
    }

    void pop_back(void)
    {
        if (m_Size != 0) {
            --m_Size;
            m_Data[m_Size].~T();
        } else {
//            vdGlobalException("Failed to remove from array -- capacity exceeded!");
        }
    }
  
    array(const array<T, N>& a) : 
        m_Size(a.m_Size)
    {
        if (m_Size != 0) {   
            assign(a.begin(), a.end());
        }
    } 

    array(unsigned int size, const T& val = T()
    ) :
        m_Size(0)
    {
        for (unsigned int i = 0; i < size; i++) 
        {
            push_back(val);
        }
    }

    array<T, N>& operator=(const array<T, N>& rhs)
    {
        if (this == &rhs) {
            return *this;
        }

        if (rhs.m_Size != 0) {   
            assign(rhs.begin(), rhs.end());
        } else {
            clear();
        }
    
        return *this;
    }

    bool operator==(array<T,N> &vec)
    {
        if (size() != vec.size()) {
            return false;
        }

        for( unsigned int i = 0; i < size(); ++i ) {
            if( operator[](i) != vec[i] ) {
                return false;
            }
        }
        return true;
    }
  
    operator T* ()             { return m_Data; }
    operator const T* () const { return m_Data; }
   
    bool empty (void) const
    {
        return m_Size==0;
    }
  
    unsigned int max_size () const
    {
        return N;
    }

    T& operator[](int index)
    {
        return m_Data[index];
    }
  
    const T& operator[](int index) const
    {
        return m_Data[index];
    }
  
    template<class I>
    void assign(I start, I end)
    {
        clear();   
        while(start != end) 
        {
            push_back(*start);
            start++;
        }
    }

    class iterator
    {
    private:
        const array<T,N>*   m_Array;
        vd::i32             m_Index;

        iterator (const array<T,N> &vec, int index) :
            m_Array(&vec)
        {            
            if( !vec.empty() ) {
                m_Index = index;
            } else {
                m_Index = -1;
            }
        }

    public:
        iterator(void) : 
            m_Index(-1),
            m_Array(NULL)
        {
        }

        iterator(const iterator& rhs) :
            m_Array(rhs.m_Array),
            m_Index(rhs.m_Index)
        {
        }

        ~iterator(void) {}

        static iterator begin(const array<T,N> &vec)
        {
            iterator i(vec, 0);

            return i;
        }

        static iterator end(const array<T,N> &vec)
        {
            iterator i(vec, vec.size());

            return i;
        }
    
        bool operator==(iterator i)
        {
            return ((m_Array == i.m_Array) && 
                    (m_Index == i.m_Index));
        }

        bool operator!=(iterator i)
        {
            return (!(*this==i));
        }

        iterator& operator++()
        {
            ++m_Index;
            return *this;
        }

        iterator operator++(int)
        {
            iterator retVal(*this);
            ++m_Index;
            return retVal;
        }

        iterator& operator--()
        {
            --m_Index;
            return *this;
        }

        iterator operator--(int)
        {
            iterator retVal(*this);
            --m_Index;
            return retVal;
        }

        const T& operator *() const
        {
            return (*m_Array)[m_Index];
        }
    };

    iterator begin(void)
    {
        return iterator::begin(*this);
    }

    iterator begin(void) const
    {
        return iterator::begin(*this);
    }

    iterator end(void)
    {
        return iterator::end(*this);
    }

    iterator end(void) const
    {
        return iterator::end(*this);
    }

    T& front(void)
    {
        return m_Data[0];
    }

    T& back(void)
    {
        return m_Data[m_Size];
    }

    const T& front(void) const
    {
        return m_Data[0];
    }

    const T& back(void) const
    {
        return m_Data[m_Size-1];
    }
};  

// ============================================================================================== //

#define vdSetTag(a, b, c, d)                   (((a) << 24) | ((b) << 16) | ((c) << 8) | (d))

// ============================================================================================== //

VD_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_FRAMEWORK_TYPES_INCLUDED

