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

#ifndef VD_CORE_FUNCTIONS_INCLUDED
#define VD_CORE_FUNCTIONS_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/memory.h"

// ============================================================================================== //

#define VD_GET_FUNCTION_FACTORY_BIND(method) \
    (vdGetFunctionFactory(method).Bind<method>())
#define VD_BIND_FUNCTION VD_GET_FUNCTION_FACTORY_BIND

#define VD_GET_FUNCTION_FACTORY_BIND_MEMBER(instance, member) \
    (vdGetFunctionFactory(member).Bind<member>(instance))
#define VD_BIND_MEMBER_FUNCTION VD_GET_FUNCTION_FACTORY_BIND_MEMBER

#define VD_GET_FUNCTION_FACTORY_BIND_ANY(method, ...) \
    (vdGetFunctionFactory(method).Bind<method>(## __VA_ARGS__))
#define VD_BIND VD_GET_FUNCTION_FACTORY_BIND_ANY

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

template<typename FuncSignature>
class Function;

struct NullFunction {};
typedef void* Void;

// ============================================================================================== //

// 0 parameter version

template<typename R>
class Function<R ()>
{
public:
    static const int Arity = 0;
    typedef R ReturnType;

    Function()                    : m_Fn(0), m_Obj(0) {}
    Function(NullFunction)        : m_Fn(0), m_Obj(0) {}
    Function(const Function& rhs) : m_Fn(rhs.m_Fn), m_Obj(rhs.m_Obj) {}
    ~Function() {} 

    Function& operator=(NullFunction)
        { m_Obj = 0; m_Fn = 0; return *this; }

    Function& operator=(const Function& rhs)
        { m_Obj = rhs.m_Obj; m_Fn = rhs.m_Fn; return *this; }

    inline R operator()() const
    {
        return (*m_Fn)(m_Obj);
    }

private:
    typedef const void* Function::*SafeBoolType;
public:
    inline operator SafeBoolType() const
        { return m_Fn != 0 ? &Function::m_Obj : 0; }
    inline bool operator!() const
        { return m_Fn == 0; }

private:
    typedef R (*FuncType)(const void*);
    Function(FuncType f, const void* o) : m_Fn(f), m_Obj(o) {}

private:
    FuncType m_Fn;
    const void* m_Obj;

    template<typename FR>
    friend class MethodFunctionFactory0;
    template<typename FR, class FT>
    friend class MemberFunctionFactory0;
    template<typename FR, class FT>
    friend class ConstMemberFunctionFactory0;
};

// ============================================================================================== //

template<typename R>
void operator==(const Function<R ()>&, const Function<R ()>&);

template<typename R>
void operator!=(const Function<R ()>&, const Function<R ()>&);

// ============================================================================================== //

template<typename R>
struct FunctionWrapper0
{
	typedef R (*FuncType)();

    template<FuncType>
	static R Wrapper(const void*)
	{
		return FuncType();
	}
};

template<typename R>
class MethodFunctionFactory0
{
public:

    template<R (*Func)()>
	static R Wrapper(const void*)
	{
		return (*Func)();
	}
    
    template<R (*Func)()>
    inline static Function<R ()> Bind()
    {
        return Function<R ()>(&Wrapper<Func>, 0);
    }
};

// ============================================================================================== //

template<typename R>
inline MethodFunctionFactory0<R>
GetFunctionFactory(R (*)())
{
    return MethodFunctionFactory0<R>();
}

// ============================================================================================== //

template<typename R, class T>
class MemberFunctionFactory0
{
private:
    template<R (T::*Func)()>
    static R Wrapper(const void* o)
    {
        T* m_Obj = const_cast<T*>(static_cast<const T*>(o));
        return (m_Obj->*Func)();
    }

public:

    template<R (T::*Func)()>
    inline static Function<R ()> BindTo(T* o)
    {
        T* m_Obj = const_cast<T*>(static_cast<const T*>(o));
        return Function<R ()>( &((m_Obj->*Func)()), static_cast<const void*>(o) );
    }

    template<R (T::*Func)()>
    inline static Function<R ()> Bind(T* o)
    {
        return Function<R ()>(&Wrapper<Func>, static_cast<const void*>(o));
    }
};

// ============================================================================================== //

template<typename R, class T>
inline MemberFunctionFactory0<R, T>
GetFunctionFactory(R (T::*)())
{
    return MemberFunctionFactory0<R, T>();
}

template<typename R, class T>
class ConstMemberFunctionFactory0
{
private:
    template<R (T::*Func)() const>
    static R Wrapper(const void* o)
    {
        const T* m_Obj = static_cast<const T*>(o);
        return (m_Obj->*Func)();
    }

public:
    template<R (T::*Func)() const>
    inline static Function<R ()> Bind(const T* o)
    {
        return Function<R ()>(&Wrapper<Func>, static_cast<const void*>(o));
    }
};

template<typename R, class T>
inline ConstMemberFunctionFactory0<R, T>
GetFunctionFactory(R (T::*)() const)
{
    return ConstMemberFunctionFactory0<R, T>();
}

// ============================================================================================== //

// 1 parameter version

template<typename R, typename P1>
class Function<R (P1)>
{
public:
    static const int Arity = 1;
    typedef R ReturnType;
    typedef P1 Param1Type;

    Function()                    : m_Fn(0), m_Obj(0) {}
    Function(NullFunction)        : m_Fn(0), m_Obj(0) {}
    Function(const Function& rhs) : m_Fn(rhs.m_Fn), m_Obj(rhs.m_Obj) {}
    ~Function() {} 

    Function& operator=(NullFunction)
        { m_Obj = 0; m_Fn = 0; return *this; }
    Function& operator=(const Function& rhs)
        { m_Obj = rhs.m_Obj; m_Fn = rhs.m_Fn; return *this; }

    inline R operator()(P1 a1) const
    {
        return (*m_Fn)(m_Obj, a1);
    }

private:
    typedef const void* Function::*SafeBoolType;
public:
    inline operator SafeBoolType() const
        { return m_Fn != 0 ? &Function::m_Obj : 0; }
    inline bool operator!() const
        { return m_Fn == 0; }

private:
    typedef R (*FuncType)(const void*, P1);
    Function(FuncType f, const void* o) : m_Fn(f), m_Obj(o) {}

private:
    FuncType m_Fn;
    const void* m_Obj;

    template<typename FR, typename FP1>
    friend class MethodFunctionFactory1;
    
    template<typename FR, class FT, typename FP1>
    friend class MemberFunctionFactory1;
    
    template<typename FR, class FT, typename FP1>
    friend class ConstMemberFunctionFactory1;
};

// ============================================================================================== //

template<typename R, typename P1>
void operator==(const Function<R (P1)>&, const Function<R (P1)>&);

template<typename R, typename P1>
void operator!=(const Function<R (P1)>&, const Function<R (P1)>&);

// ============================================================================================== //

template<typename R, typename P1>
class MethodFunctionFactory1
{
private:
    template<R (*Func)(P1)>
    static R Wrapper(const void*, P1 a1)
    {
        return (*Func)(a1);
    }

public:
    template<R (*Func)(P1)>
    inline static Function<R (P1)> Bind()
    {
        return Function<R (P1)>(&Wrapper<Func>, 0);
    }
};

// ============================================================================================== //

template<typename R, typename P1>
inline MethodFunctionFactory1<R, P1>
vdGetFunctionFactory(R (*)(P1))
{
    return MethodFunctionFactory1<R, P1>();
}

// ============================================================================================== //

template<typename R, class T, typename P1>
class MemberFunctionFactory1
{
private:
    template<R (T::*Func)(P1)>
    static R Wrapper(const void* o, P1 a1)
    {
        T* m_Obj = const_cast<T*>(static_cast<const T*>(o));
        return (m_Obj->*Func)(a1);
    }

public:
    template<R (T::*Func)(P1)>
    inline static Function<R (P1)> Bind(T* o)
    {
        return Function<R (P1)>(&Wrapper<Func>, static_cast<const void*>(o));
    }
};

// ============================================================================================== //

template<typename R, class T, typename P1>
inline MemberFunctionFactory1<R, T, P1>
vdGetFunctionFactory(R (T::*)(P1))
{
    return MemberFunctionFactory1<R, T, P1>();
}

// ============================================================================================== //

template<typename R, class T, typename P1>
class ConstMemberFunctionFactory1
{
private:
    template<R (T::*Func)(P1) const>
    static R Wrapper(const void* o, P1 a1)
    {
        const T* m_Obj = static_cast<const T*>(o);
        return (m_Obj->*Func)(a1);
    }

public:
    template<R (T::*Func)(P1) const>
    inline static Function<R (P1)> Bind(const T* o)
    {
        return Function<R (P1)>(&Wrapper<Func>, static_cast<const void*>(o));
    }
};

// ============================================================================================== //

template<typename R, class T, typename P1>
inline ConstMemberFunctionFactory1<R, T, P1>
vdGetFunctionFactory(R (T::*)(P1) const)
{
    return ConstMemberFunctionFactory1<R, T, P1>();
}

// ============================================================================================== //

// 2 parameter version

template<typename R, typename P1, typename P2>
class Function<R (P1, P2)>
{
public:
    static const int Arity = 2;
    typedef R ReturnType;
    typedef P1 Param1Type;
    typedef P2 Param2Type;

    Function()                    : m_Fn(0), m_Obj(0) {}
    Function(NullFunction)        : m_Fn(0), m_Obj(0) {}
    Function(const Function& rhs) : m_Fn(rhs.m_Fn), m_Obj(rhs.m_Obj) {}
    ~Function() {} 

    Function& operator=(NullFunction)
        { m_Obj = 0; m_Fn = 0; return *this; }
    Function& operator=(const Function& rhs)
        { m_Obj = rhs.m_Obj; m_Fn = rhs.m_Fn; return *this; }

    inline R operator()(P1 a1, P2 a2) const
    {
        return (*m_Fn)(m_Obj, a1, a2);
    }

private:
    typedef const void* Function::*SafeBoolType;
public:
    inline operator SafeBoolType() const
        { return m_Fn != 0 ? &Function::m_Obj : 0; }
    inline bool operator!() const
        { return m_Fn == 0; }

private:
    typedef R (*FuncType)(const void*, P1, P2);
    Function(FuncType f, const void* o) : m_Fn(f), m_Obj(o) {}

private:
    FuncType m_Fn;
    const void* m_Obj;

    template<typename FR, typename FP1, typename FP2>
    friend class MethodFunctionFactory2;
    
    template<typename FR, class FT, typename FP1, typename FP2>
    friend class MemberFunctionFactory2;
    
    template<typename FR, class FT, typename FP1, typename FP2>
    friend class ConstMemberFunctionFactory2;
};

// ============================================================================================== //

template<typename R, typename P1, typename P2>
void operator==(const Function<R (P1, P2)>&, const Function<R (P1, P2)>&);

template<typename R, typename P1, typename P2>
void operator!=(const Function<R (P1, P2)>&, const Function<R (P1, P2)>&);

// ============================================================================================== //

template<typename R, typename P1, typename P2>
class MethodFunctionFactory2
{
private:
    template<R (*Func)(P1, P2)>
    static R Wrapper(const void*, P1 a1, P2 a2)
    {
        return (*Func)(a1, a2);
    }

public:
    template<R (*Func)(P1, P2)>
    inline static Function<R (P1, P2)> Bind()
    {
        return Function<R (P1, P2)>(&Wrapper<Func>, 0);
    }
};

// ============================================================================================== //

template<typename R, typename P1, typename P2>
inline MethodFunctionFactory2<R, P1, P2>
vdGetFunctionFactory(R (*)(P1, P2))
{
    return MethodFunctionFactory2<R, P1, P2>();
}

// ============================================================================================== //

template<typename R, class T, typename P1, typename P2>
class MemberFunctionFactory2
{
private:
    template<R (T::*Func)(P1, P2)>
    static R Wrapper(const void* o, P1 a1, P2 a2)
    {
        T* m_Obj = const_cast<T*>(static_cast<const T*>(o));
        return (m_Obj->*Func)(a1, a2);
    }

public:
    template<R (T::*Func)(P1, P2)>
    inline static Function<R (P1, P2)> Bind(T* o)
    {
        return Function<R (P1, P2)>(&Wrapper<Func>, static_cast<const void*>(o));
    }
};

// ============================================================================================== //

template<typename R, class T, typename P1, typename P2>
inline MemberFunctionFactory2<R, T, P1, P2>
vdGetFunctionFactory(R (T::*)(P1, P2))
{
    return MemberFunctionFactory2<R, T, P1, P2>();
}

// ============================================================================================== //

template<typename R, class T, typename P1, typename P2>
class ConstMemberFunctionFactory2
{
private:
    template<R (T::*Func)(P1, P2) const>
    static R Wrapper(const void* o, P1 a1, P2 a2)
    {
        const T* m_Obj = static_cast<const T*>(o);
        return (m_Obj->*Func)(a1, a2);
    }

public:
    template<R (T::*Func)(P1, P2) const>
    inline static Function<R (P1, P2)> Bind(const T* o)
    {
        return Function<R (P1, P2)>(&Wrapper<Func>, static_cast<const void*>(o));
    }
};

// ============================================================================================== //

template<typename R, class T, typename P1, typename P2>
inline ConstMemberFunctionFactory2<R, T, P1, P2>
vdGetFunctionFactory(R (T::*)(P1, P2) const)
{
    return ConstMemberFunctionFactory2<R, T, P1, P2>();
}

// ============================================================================================== //

// 3 parameter version

template<typename R, typename P1, typename P2, typename P3>
class Function<R (P1, P2, P3)>
{
public:
    static const int Arity = 3;
    typedef R ReturnType;
    typedef P1 Param1Type;
    typedef P2 Param2Type;
    typedef P3 Param3Type;

    Function()                    : m_Fn(0), m_Obj(0) {}
    Function(NullFunction)        : m_Fn(0), m_Obj(0) {}
    Function(const Function& rhs) : m_Fn(rhs.m_Fn), m_Obj(rhs.m_Obj) {}
    ~Function() {} 

    Function& operator=(NullFunction)
        { m_Obj = 0; m_Fn = 0; return *this; }
    Function& operator=(const Function& rhs)
        { m_Obj = rhs.m_Obj; m_Fn = rhs.m_Fn; return *this; }

    inline R operator()(P1 a1, P2 a2, P3 a3) const
    {
        return (*m_Fn)(m_Obj, a1, a2, a3);
    }

private:
    typedef const void* Function::*SafeBoolType;
public:
    inline operator SafeBoolType() const
        { return m_Fn != 0 ? &Function::m_Obj : 0; }
    inline bool operator!() const
        { return m_Fn == 0; }

private:
    typedef R (*FuncType)(const void*, P1, P2, P3);
    Function(FuncType f, const void* o) : m_Fn(f), m_Obj(o) {}

private:
    FuncType m_Fn;
    const void* m_Obj;

    template<typename FR, typename FP1, typename FP2, typename FP3>
    friend class MethodFunctionFactory3;
    template<typename FR, class FT, typename FP1, typename FP2, typename FP3>
    friend class MemberFunctionFactory3;
    template<typename FR, class FT, typename FP1, typename FP2, typename FP3>
    friend class ConstMemberFunctionFactory3;
};

// ============================================================================================== //

template<typename R, typename P1, typename P2, typename P3>
void operator==(const Function<R (P1, P2, P3)>&, const Function<R (P1, P2, P3)>&);

template<typename R, typename P1, typename P2, typename P3>
void operator!=(const Function<R (P1, P2, P3)>&, const Function<R (P1, P2, P3)>&);

// ============================================================================================== //

template<typename R, typename P1, typename P2, typename P3>
class MethodFunctionFactory3
{
private:
    template<R (*Func)(P1, P2, P3)>
    static R Wrapper(const void*, P1 a1, P2 a2, P3 a3)
    {
        return (*Func)(a1, a2, a3);
    }

public:
    template<R (*Func)(P1, P2, P3)>
    inline static Function<R (P1, P2, P3)> Bind()
    {
        return Function<R (P1, P2, P3)>(&Wrapper<Func>, 0);
    }
};

// ============================================================================================== //

template<typename R, typename P1, typename P2, typename P3>
inline MethodFunctionFactory3<R, P1, P2, P3>
vdGetFunctionFactory(R (*)(P1, P2, P3))
{
    return MethodFunctionFactory3<R, P1, P2, P3>();
}

// ============================================================================================== //

template<typename R, class T, typename P1, typename P2, typename P3>
class MemberFunctionFactory3
{
private:
    template<R (T::*Func)(P1, P2, P3)>
    static R Wrapper(const void* o, P1 a1, P2 a2, P3 a3)
    {
        T* m_Obj = const_cast<T*>(static_cast<const T*>(o));
        return (m_Obj->*Func)(a1, a2, a3);
    }

public:
    template<R (T::*Func)(P1, P2, P3)>
    inline static Function<R (P1, P2, P3)> Bind(T* o)
    {
        return Function<R (P1, P2, P3)>(&Wrapper<Func>, static_cast<const void*>(o));
    }
};

// ============================================================================================== //

template<typename R, class T, typename P1, typename P2, typename P3>
inline MemberFunctionFactory3<R, T, P1, P2, P3>
vdGetFunctionFactory(R (T::*)(P1, P2, P3))
{
    return MemberFunctionFactory3<R, T, P1, P2, P3>();
}

// ============================================================================================== //

template<typename R, class T, typename P1, typename P2, typename P3>
class ConstMemberFunctionFactory3
{
private:
    template<R (T::*Func)(P1, P2, P3) const>
    static R Wrapper(const void* o, P1 a1, P2 a2, P3 a3)
    {
        const T* m_Obj = static_cast<const T*>(o);
        return (m_Obj->*Func)(a1, a2, a3);
    }

public:
    template<R (T::*Func)(P1, P2, P3) const>
    inline static Function<R (P1, P2, P3)> Bind(const T* o)
    {
        return Function<R (P1, P2, P3)>(&Wrapper<Func>, static_cast<const void*>(o));
    }
};

// ============================================================================================== //

template<typename R, class T, typename P1, typename P2, typename P3>
inline ConstMemberFunctionFactory3<R, T, P1, P2, P3>
vdGetFunctionFactory(R (T::*)(P1, P2, P3) const)
{
    return ConstMemberFunctionFactory3<R, T, P1, P2, P3>();
}

// ============================================================================================== //

// 4 parameter version

template<typename R, typename P1, typename P2, typename P3, typename P4>
class Function<R (P1, P2, P3, P4)>
{
public:
    static const int Arity = 4;
    typedef R ReturnType;
    typedef P1 Param1Type;
    typedef P2 Param2Type;
    typedef P3 Param3Type;
    typedef P4 Param4Type;

    Function()                    : m_Fn(0), m_Obj(0) {}
    Function(NullFunction)        : m_Fn(0), m_Obj(0) {}
    Function(const Function& rhs) : m_Fn(rhs.m_Fn), m_Obj(rhs.m_Obj) {}
    ~Function() {} 

    Function& operator=(NullFunction)
        { m_Obj = 0; m_Fn = 0; return *this; }
    Function& operator=(const Function& rhs)
        { m_Obj = rhs.m_Obj; m_Fn = rhs.m_Fn; return *this; }

    inline R operator()(P1 a1, P2 a2, P3 a3, P4 a4) const
    {
        return (*m_Fn)(m_Obj, a1, a2, a3, a4);
    }

private:
    typedef const void* Function::*SafeBoolType;
public:
    inline operator SafeBoolType() const
        { return m_Fn != 0 ? &Function::m_Obj : 0; }
    inline bool operator!() const
        { return m_Fn == 0; }

private:
    typedef R (*FuncType)(const void*, P1, P2, P3, P4);
    Function(FuncType f, const void* o) : m_Fn(f), m_Obj(o) {}

private:
    FuncType m_Fn;
    const void* m_Obj;

    template<typename FR, typename FP1, typename FP2, typename FP3,
                          typename FP4>
    friend class MethodFunctionFactory4;
    template<typename FR, class FT, typename FP1, typename FP2, typename FP3,
                                    typename FP4>
    friend class MemberFunctionFactory4;
    template<typename FR, class FT, typename FP1, typename FP2, typename FP3,
                                    typename FP4>
    friend class ConstMemberFunctionFactory4;
};

// ============================================================================================== //

template<typename R, typename P1, typename P2, typename P3, typename P4>
void operator==(const Function<R (P1, P2, P3, P4)>&, const Function<R (P1, P2, P3, P4)>&);

// ============================================================================================== //

template<typename R, typename P1, typename P2, typename P3, typename P4>
void operator!=(const Function<R (P1, P2, P3, P4)>&, const Function<R (P1, P2, P3, P4)>&);

// ============================================================================================== //

template<typename R, typename P1, typename P2, typename P3, typename P4>
class MethodFunctionFactory4
{
private:
    template<R (*Func)(P1, P2, P3, P4)>
    static R Wrapper(const void*, P1 a1, P2 a2, P3 a3, P4 a4)
    {
        return (*Func)(a1, a2, a3, a4);
    }

public:
    template<R (*Func)(P1, P2, P3, P4)>
    inline static Function<R (P1, P2, P3, P4)> Bind()
    {
        return Function<R (P1, P2, P3, P4)>(&Wrapper<Func>, 0);
    }
};

// ============================================================================================== //

template<typename R, typename P1, typename P2, typename P3, typename P4>
inline MethodFunctionFactory4<R, P1, P2, P3, P4>
vdGetFunctionFactory(R (*)(P1, P2, P3, P4))
{
    return MethodFunctionFactory4<R, P1, P2, P3, P4>();
}

// ============================================================================================== //

template<typename R, class T, typename P1, typename P2, typename P3, typename P4>
class MemberFunctionFactory4
{
private:
    template<R (T::*Func)(P1, P2, P3, P4)>
    static R Wrapper(const void* o, P1 a1, P2 a2, P3 a3, P4 a4)
    {
        T* m_Obj = const_cast<T*>(static_cast<const T*>(o));
        return (m_Obj->*Func)(a1, a2, a3, a4);
    }

public:
    template<R (T::*Func)(P1, P2, P3, P4)>
    inline static Function<R (P1, P2, P3, P4)> Bind(T* o)
    {
        return Function<R (P1, P2, P3, P4)>(&Wrapper<Func>, static_cast<const void*>(o));
    }
};

// ============================================================================================== //

template<typename R, class T, typename P1, typename P2, typename P3, typename P4>
inline MemberFunctionFactory4<R, T, P1, P2, P3, P4>
vdGetFunctionFactory(R (T::*)(P1, P2, P3, P4))
{
    return MemberFunctionFactory4<R, T, P1, P2, P3, P4>();
}

// ============================================================================================== //

template<typename R, class T, typename P1, typename P2, typename P3, typename P4>
class ConstMemberFunctionFactory4
{
private:
    template<R (T::*Func)(P1, P2, P3, P4) const>
    static R Wrapper(const void* o, P1 a1, P2 a2, P3 a3, P4 a4)
    {
        const T* m_Obj = static_cast<const T*>(o);
        return (m_Obj->*Func)(a1, a2, a3, a4);
    }

public:
    template<R (T::*Func)(P1, P2, P3, P4) const>
    inline static Function<R (P1, P2, P3, P4)> Bind(const T* o)
    {
        return Function<R (P1, P2, P3, P4)>(&Wrapper<Func>, static_cast<const void*>(o));
    }
};

// ============================================================================================== //

template<typename R, class T, typename P1, typename P2, typename P3, typename P4>
inline ConstMemberFunctionFactory4<R, T, P1, P2, P3, P4>
vdGetFunctionFactory(R (T::*)(P1, P2, P3, P4) const)
{
    return ConstMemberFunctionFactory4<R, T, P1, P2, P3, P4>();
}

// ============================================================================================== //

// 5 parameter version

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5>
class Function<R (P1, P2, P3, P4, P5)>
{
public:
    static const int Arity = 5;
    typedef R ReturnType;
    typedef P1 Param1Type;
    typedef P2 Param2Type;
    typedef P3 Param3Type;
    typedef P4 Param4Type;
    typedef P5 Param5Type;

    Function()                    : m_Fn(0), m_Obj(0) {}
    Function(NullFunction)        : m_Fn(0), m_Obj(0) {}
    Function(const Function& rhs) : m_Fn(rhs.m_Fn), m_Obj(rhs.m_Obj) {}
    ~Function() {} 

    Function& operator=(NullFunction)
        { m_Obj = 0; m_Fn = 0; return *this; }
        
    Function& operator=(const Function& rhs)
        { m_Obj = rhs.m_Obj; m_Fn = rhs.m_Fn; return *this; }

    inline R operator()(P1 a1, P2 a2, P3 a3, P4 a4, P5 a5) const
    {
        return (*m_Fn)(m_Obj, a1, a2, a3, a4, a5);
    }

private:
    typedef const void* Function::*SafeBoolType;

public:

    inline operator SafeBoolType() const
        { return m_Fn != 0 ? &Function::m_Obj : 0; }

    inline bool operator!() const
        { return m_Fn == 0; }

private:
    typedef R (*FuncType)(const void*, P1, P2, P3, P4, P5);
    Function(FuncType f, const void* o) : m_Fn(f), m_Obj(o) {}

private:
    FuncType m_Fn;
    const void* m_Obj;

    template<typename FR, typename FP1, typename FP2, typename FP3, typename FP4, typename FP5>
    friend class MethodFunctionFactory5;

    template<typename FR, class FT, typename FP1, typename FP2, typename FP3, typename FP4, typename FP5>
    friend class MemberFunctionFactory5;
    
    template<typename FR, class FT, typename FP1, typename FP2, typename FP3, typename FP4, typename FP5>
    friend class ConstMemberFunctionFactory5;
};

// ============================================================================================== //

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5>
void operator==(const Function<R (P1, P2, P3, P4, P5)>&, const Function<R (P1, P2, P3, P4, P5)>&);
                
template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5>
void operator!=(const Function<R (P1, P2, P3, P4, P5)>&, const Function<R (P1, P2, P3, P4, P5)>&);

// ============================================================================================== //

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5>
class MethodFunctionFactory5
{
private:
    template<R (*Func)(P1, P2, P3, P4, P5)>
    static R Wrapper(const void*, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5)
    {
        return (*Func)(a1, a2, a3, a4, a5);
    }

public:
    template<R (*Func)(P1, P2, P3, P4, P5)>
    inline static Function<R (P1, P2, P3, P4, P5)> Bind()
    {
        return Function<R (P1, P2, P3, P4, P5)>(&Wrapper<Func>, 0);
    }
};

// ============================================================================================== //

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5>
inline MethodFunctionFactory5<R, P1, P2, P3, P4, P5>
vdGetFunctionFactory(R (*)(P1, P2, P3, P4, P5))
{
    return MethodFunctionFactory5<R, P1, P2, P3, P4, P5>();
}

// ============================================================================================== //

template<typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5>
class MemberFunctionFactory5
{
private:
    template<R (T::*Func)(P1, P2, P3, P4, P5)>
    static R Wrapper(const void* o, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5)
    {
        T* m_Obj = const_cast<T*>(static_cast<const T*>(o));
        return (m_Obj->*Func)(a1, a2, a3, a4, a5);
    }

public:
    template<R (T::*Func)(P1, P2, P3, P4, P5)>
    inline static Function<R (P1, P2, P3, P4, P5)> Bind(T* o)
    {
        return Function<R (P1, P2, P3, P4, P5)>(&Wrapper<Func>, static_cast<const void*>(o));
    }
};

// ============================================================================================== //

template<typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5>
inline MemberFunctionFactory5<R, T, P1, P2, P3, P4, P5>
vdGetFunctionFactory(R (T::*)(P1, P2, P3, P4, P5))
{
    return MemberFunctionFactory5<R, T, P1, P2, P3, P4, P5>();
}

// ============================================================================================== //

template<typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5>
class ConstMemberFunctionFactory5
{
private:
    template<R (T::*Func)(P1, P2, P3, P4, P5) const>
    static R Wrapper(const void* o, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5)
    {
        const T* m_Obj = static_cast<const T*>(o);
        return (m_Obj->*Func)(a1, a2, a3, a4, a5);
    }

public:
    template<R (T::*Func)(P1, P2, P3, P4, P5) const>
    inline static Function<R (P1, P2, P3, P4, P5)> Bind(const T* o)
    {
        return Function<R (P1, P2, P3, P4, P5)>(&Wrapper<Func>, static_cast<const void*>(o));
    }
};

// ============================================================================================== //

template<typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5>
inline ConstMemberFunctionFactory5<R, T, P1, P2, P3, P4, P5>
vdGetFunctionFactory(R (T::*)(P1, P2, P3, P4, P5) const)
{
    return ConstMemberFunctionFactory5<R, T, P1, P2, P3, P4, P5>();
}

// ============================================================================================== //

// 6 parameter version

template<typename R, typename P1, typename P2, typename P3,
                     typename P4, typename P5, typename P6>
class Function<R (P1, P2, P3, P4, P5, P6)>
{
public:
    static const int Arity = 6;
    typedef R ReturnType;
    typedef P1 Param1Type;
    typedef P2 Param2Type;
    typedef P3 Param3Type;
    typedef P4 Param4Type;
    typedef P5 Param5Type;
    typedef P6 Param6Type;

    Function()                    : m_Fn(0), m_Obj(0) {}
    Function(NullFunction)        : m_Fn(0), m_Obj(0) {}
    Function(const Function& rhs) : m_Fn(rhs.m_Fn), m_Obj(rhs.m_Obj) {}
    ~Function() {} 

    Function& operator=(NullFunction)
        { m_Obj = 0; m_Fn = 0; return *this; }
        
    Function& operator=(const Function& rhs)
        { m_Obj = rhs.m_Obj; m_Fn = rhs.m_Fn; return *this; }

    inline R operator()(P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6) const
    {
        return (*m_Fn)(m_Obj, a1, a2, a3, a4, a5, a6);
    }

private:
    typedef const void* Function::*SafeBoolType;
public:
    inline operator SafeBoolType() const
        { return m_Fn != 0 ? &Function::m_Obj : 0; }
    inline bool operator!() const
        { return m_Fn == 0; }

private:
    typedef R (*FuncType)(const void*, P1, P2, P3, P4, P5, P6);
    Function(FuncType f, const void* o) : m_Fn(f), m_Obj(o) {}

private:
    FuncType m_Fn;
    const void* m_Obj;

    template<typename FR, typename FP1, typename FP2, typename FP3, typename FP4, typename FP5, typename FP6>
    friend class MethodFunctionFactory6;

    template<typename FR, class FT, typename FP1, typename FP2, typename FP3, typename FP4, typename FP5, typename FP6>
    friend class MemberFunctionFactory6;

    template<typename FR, class FT, typename FP1, typename FP2, typename FP3, typename FP4, typename FP5, typename FP6>
    friend class ConstMemberFunctionFactory6;
};

// ============================================================================================== //

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
void operator==(const Function<R (P1, P2, P3, P4, P5, P6)>&, const Function<R (P1, P2, P3, P4, P5, P6)>&);
                              
template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
void operator!=(const Function<R (P1, P2, P3, P4, P5, P6)>&, const Function<R (P1, P2, P3, P4, P5, P6)>&);

// ============================================================================================== //

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
class MethodFunctionFactory6
{
private:
    template<R (*Func)(P1, P2, P3, P4, P5, P6)>
    static R Wrapper(const void*, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6)
    {
        return (*Func)(a1, a2, a3, a4, a5, a6);
    }

public:
    template<R (*Func)(P1, P2, P3, P4, P5, P6)>
    inline static Function<R (P1, P2, P3, P4, P5, P6)> Bind()
    {
        return Function<R (P1, P2, P3, P4, P5, P6)>(&Wrapper<Func>, 0);
    }
};

// ============================================================================================== //

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
inline MethodFunctionFactory6<R, P1, P2, P3, P4, P5, P6>
vdGetFunctionFactory(R (*)(P1, P2, P3, P4, P5, P6))
{
    return MethodFunctionFactory6<R, P1, P2, P3, P4, P5, P6>();
}

// ============================================================================================== //

template<typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
class MemberFunctionFactory6
{
private:
    template<R (T::*Func)(P1, P2, P3, P4, P5, P6)>
    static R Wrapper(const void* o, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6)
    {
        T* m_Obj = const_cast<T*>(static_cast<const T*>(o));
        return (m_Obj->*Func)(a1, a2, a3, a4, a5, a6);
    }

public:
    template<R (T::*Func)(P1, P2, P3, P4, P5, P6)>
    inline static Function<R (P1, P2, P3, P4, P5, P6)> Bind(T* o)
    {
        return Function<R (P1, P2, P3, P4, P5, P6)>(&Wrapper<Func>, static_cast<const void*>(o));
    }
};

// ============================================================================================== //

template<typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
inline MemberFunctionFactory6<R, T, P1, P2, P3, P4, P5, P6>
vdGetFunctionFactory(R (T::*)(P1, P2, P3, P4, P5, P6))
{
    return MemberFunctionFactory6<R, T, P1, P2, P3, P4, P5, P6>();
}

// ============================================================================================== //

template<typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
class ConstMemberFunctionFactory6
{
private:
    template<R (T::*Func)(P1, P2, P3, P4, P5, P6) const>
    static R Wrapper(const void* o, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6)
    {
        const T* m_Obj = static_cast<const T*>(o);
        return (m_Obj->*Func)(a1, a2, a3, a4, a5, a6);
    }

public:
    template<R (T::*Func)(P1, P2, P3, P4, P5, P6) const>
    inline static Function<R (P1, P2, P3, P4, P5, P6)> Bind(const T* o)
    {
        return Function<R (P1, P2, P3, P4, P5, P6)>(&Wrapper<Func>, static_cast<const void*>(o));
    }
};

// ============================================================================================== //

template<typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
inline ConstMemberFunctionFactory6<R, T, P1, P2, P3, P4, P5, P6>
vdGetFunctionFactory(R (T::*)(P1, P2, P3, P4, P5, P6) const)
{
    return ConstMemberFunctionFactory6<R, T, P1, P2, P3, P4, P5, P6>();
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_FUNCTIONS_INCLUDED

