
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

#include "serialise.h"

// ---------------------------------------------------------------------------------------------- //

void WriteBytes(const char* bytes, const long& length)
{
    std::cout << std::hex;
    int column = 0;

    for(long i = 0; i < length; ++i)
    {
        std::cout << std::setw(2) << std::setfill('0') << std::right
                  << int((unsigned char)(bytes[i])) << ' ';
        ++column;

        if(column % 4 == 0 && column != 16)
            std::cout << "| ";

        if(column == 16 && i != length - 1)
        {
            std::cout << std::endl;
            column = 0;
        }
    }

    std::cout << std::endl;
}

// ---------------------------------------------------------------------------------------------- //

class SimpleClass
{
public:
    SimpleClass(const int& c, const std::string& n)
        : count(c)
        , name(n)
    {

    }

    bool operator==(const SimpleClass& that) const
    {
        return count == that.count && name == that.name;
    }

    VD_BEGIN_MEMBERS(SimpleClass)
    VD_ADD_MEMBER(private, int, count)
    VD_ADD_MEMBER(private, std::string, name)
    VD_END_MEMBERS()
};

// ---------------------------------------------------------------------------------------------- //

void TestSimple()
{
    std::cout << "TestSimple" << std::endl;
    std::cout << "----------" << std::endl;

    SimpleClass obj(3, "Gerrit");

    long length;
    char* bytes = vd::Serialise(obj, length);
    WriteBytes(bytes, length);
    SimpleClass* newObj = vd::Deserialise<SimpleClass>(bytes);

    if(*newObj == obj)
        std::cout << "SUCCESS" << std::endl;
    else
        std::cout << "FAILURE" << std::endl;

    delete newObj;
    delete[] bytes;
}

// ---------------------------------------------------------------------------------------------- //

class CompositeClass
{
public:
    CompositeClass(const SimpleClass& obj, const int& v)
        : object(obj)
        , value(v)
    {

    }

    bool operator==(const CompositeClass& that) const
    {
        return object == that.object && value == that.value;
    }

    VD_BEGIN_MEMBERS(CompositeClass)
    VD_ADD_MEMBER(private, SimpleClass, object)
    VD_ADD_MEMBER(private, int, value)
    VD_END_MEMBERS()
};

// ---------------------------------------------------------------------------------------------- //

void TestComposite()
{
    std::cout << "TestComposite" << std::endl;
    std::cout << "-------------" << std::endl;

    CompositeClass obj(SimpleClass(3, "Alex"), 42);

    long length;
    char* bytes = vd::Serialise(obj, length);
    WriteBytes(bytes, length);
    CompositeClass* newObj = vd::Deserialise<CompositeClass>(bytes);

    if(*newObj == obj)
        std::cout << "SUCCESS" << std::endl;
    else
        std::cout << "FAILURE" << std::endl;

    delete newObj;
    delete[] bytes;
}

// ---------------------------------------------------------------------------------------------- //

class DerivedClass : public CompositeClass
{
public:
    DerivedClass(const SimpleClass& obj, const int& v, const std::string& t)
        : CompositeClass(obj, v)
        , text(t)
    {

    }

    bool operator==(const DerivedClass& that) const
    {
        return CompositeClass::operator==(that) && text == that.text;
    }

    VD_BEGIN_MEMBERS(DerivedClass)
    VD_BASE_CLASS(CompositeClass)
    VD_ADD_MEMBER(private, std::string, text)
    VD_END_MEMBERS()
};

// ---------------------------------------------------------------------------------------------- //

void TestDerived()
{
    std::cout << "TestDerived" << std::endl;
    std::cout << "-----------" << std::endl;

    DerivedClass obj(SimpleClass(3, "Gerrit Daniels"), 42, "Mostly harmless");

    long length;
    char* bytes = vd::Serialise(obj, length);
    WriteBytes(bytes, length);
    DerivedClass* newObj = vd::Deserialise<DerivedClass>(bytes);

    if(*newObj == obj)
        std::cout << "SUCCESS" << std::endl;
    else
        std::cout << "FAILURE" << std::endl;

    delete newObj;
    delete[] bytes;
}

// ---------------------------------------------------------------------------------------------- //

class SimpleClassVector
{
public:
    void add(const SimpleClass& obj)
    {
        elements.push_back(obj);
    }

    bool operator==(const SimpleClassVector& that)
    {
        return elements == that.elements;
    }

    VD_BEGIN_MEMBERS(SimpleClassVector)
    VD_ADD_MEMBER(private, std::vector<SimpleClass>, elements)
    VD_END_MEMBERS()
};

// ---------------------------------------------------------------------------------------------- //

void TestVector()
{
    std::cout << "TestVector" << std::endl;
    std::cout << "----------" << std::endl;

    SimpleClassVector obj;
    obj.add(SimpleClass(5, "Gerrit"));
    obj.add(SimpleClass(9, "Alex"));
    obj.add(SimpleClass(11, "Kermit"));

    long length;
    char* bytes = vd::Serialise(obj, length);
    WriteBytes(bytes, length);
    SimpleClassVector* newObj = vd::Deserialise<SimpleClassVector>(bytes);

    if(*newObj == obj)
        std::cout << "SUCCESS" << std::endl;
    else
        std::cout << "FAILURE" << std::endl;

    delete newObj;
    delete[] bytes;
}

// ---------------------------------------------------------------------------------------------- //

class SimpleClassList
{
public:
    void add(const SimpleClass& obj)
    {
        elements.push_back(obj);
    }

    bool operator==(const SimpleClassList& that)
    {
        return elements == that.elements;
    }

    VD_BEGIN_MEMBERS(SimpleClassList)
    VD_ADD_MEMBER(private, std::list<SimpleClass>, elements)
    VD_END_MEMBERS()
};

// ---------------------------------------------------------------------------------------------- //

void TestList()
{
    std::cout << "TestList" << std::endl;
    std::cout << "--------" << std::endl;

    SimpleClassList obj;
    obj.add(SimpleClass(5, "Gerrit"));
    obj.add(SimpleClass(9, "Alex"));
    obj.add(SimpleClass(11, "Kermit"));

    long length;
    char* bytes = vd::Serialise(obj, length);
    WriteBytes(bytes, length);
    SimpleClassList* newObj = vd::Deserialise<SimpleClassList>(bytes);

    if(*newObj == obj)
        std::cout << "SUCCESS" << std::endl;
    else
        std::cout << "FAILURE" << std::endl;

    delete newObj;
    delete[] bytes;
}

// ---------------------------------------------------------------------------------------------- //

class SimpleMap
{
public:
    void add(const std::string& key, const SimpleClass& obj)
    {
        m_map[key] = obj;
    }

    bool operator==(const SimpleMap& that) const
    {
        return m_map == that.m_map;
    }

    typedef std::map<std::string, SimpleClass> MapType;
    VD_BEGIN_MEMBERS(SimpleMap)
    VD_ADD_MEMBER(private, MapType, m_map)
    VD_END_MEMBERS()
};

// ---------------------------------------------------------------------------------------------- //

void TestMap()
{
    std::cout << "TestMap" << std::endl;
    std::cout << "-------" << std::endl;
    SimpleMap obj;
    obj.add("G", SimpleClass(0, "Gerrit"));
    obj.add("A", SimpleClass(1, "Alex"));
    obj.add("K", SimpleClass(2, "Kermit"));

    long length;
    char* bytes = vd::Serialise(obj, length);
    WriteBytes(bytes, length);
    SimpleMap* newObj = vd::Deserialise<SimpleMap>(bytes);

    if(*newObj == obj)
        std::cout << "SUCCESS" << std::endl;
    else
        std::cout << "FAILURE" << std::endl;

    delete newObj;
    delete[] bytes;
}

// ---------------------------------------------------------------------------------------------- //

class SimpleClassPtrList
{
public:
    void add(const boost::shared_ptr<SimpleClass>& obj)
    {
        elements.push_back(obj);
    }

    boost::shared_ptr<SimpleClass> get(int index) { return elements[index]; }

    bool operator==(const SimpleClassPtrList& that)
    {
        if(elements.size() != that.elements.size())
            return false;

        for(int i = 0; i < (int)elements.size(); ++i)
        {
            if(elements[i].get() == 0 || that.elements[i].get() == 0)
            {
                if(elements[i].get() != that.elements[i].get())
                    return false;
            }
            else if(!(*(elements[i]) == *(that.elements[i])))
                return false;
        }

        return true;
    }

    VD_BEGIN_MEMBERS(SimpleClassPtrList)
    VD_ADD_MEMBER(private, std::vector< boost::shared_ptr<SimpleClass> >, elements)
    VD_END_MEMBERS()
};

void TestSharedPtr()
{
    std::cout << "TestSharedPtr" << std::endl;
    std::cout << "-------------" << std::endl;
    SimpleClassPtrList obj;
    boost::shared_ptr<SimpleClass> ptr(new SimpleClass(0, "Gerrit"));
    obj.add(ptr);
    obj.add(boost::shared_ptr<SimpleClass>(new SimpleClass(1, "Alex")));
    obj.add(ptr);
    obj.add(ptr);
    obj.add(boost::shared_ptr<SimpleClass>(new SimpleClass(2, "Kermit")));
    long length;
    char* bytes = vd::Serialise(obj, length);
    WriteBytes(bytes, length);
    SimpleClassPtrList* newObj = vd::Deserialise<SimpleClassPtrList>(bytes);

    boost::shared_ptr<SimpleClass> ptrAfter(newObj->get(0));

    if(ptr.use_count() != ptrAfter.use_count())
        std::cout << "FAILURE use_count incorrect" << std::endl;

    std::cout << ptrAfter.use_count() << std::endl;

    if(*newObj == obj)
        std::cout << "SUCCESS" << std::endl;
    else
        std::cout << "FAILURE" << std::endl;

    delete newObj;
    delete[] bytes;
}

// ---------------------------------------------------------------------------------------------- //

class WeakContainer
{
public:
    void setPointers(boost::shared_ptr<SimpleClass> ptr)
    {
        m_delayedSerialization = ptr;
        m_sharedPtr = ptr;
        m_weakPtr = ptr;
    }

    bool operator==(const WeakContainer& that) const
    {
        return (*m_sharedPtr == *that.m_sharedPtr) && (*m_weakPtr.lock() == *that.m_weakPtr.lock()) &&
               (*m_delayedSerialization.lock() == *that.m_delayedSerialization.lock());
    }

    VD_BEGIN_MEMBERS(WeakContainer)
    VD_ADD_MEMBER(private, boost::weak_ptr<SimpleClass>, m_delayedSerialization)
    VD_ADD_MEMBER(private, boost::shared_ptr<SimpleClass>, m_sharedPtr)
    VD_ADD_MEMBER(private, boost::weak_ptr<SimpleClass>, m_weakPtr)
    VD_END_MEMBERS()
};

// ---------------------------------------------------------------------------------------------- //

void TestWeakPtr()
{
    std::cout << "TestWeakPtr" << std::endl;
    std::cout << "-----------" << std::endl;
    WeakContainer obj;
    boost::shared_ptr<SimpleClass> ptr(new SimpleClass(42, "Gerrit"));
    obj.setPointers(ptr);

    long length;
    char* bytes = vd::Serialise(obj, length);
    WriteBytes(bytes, length);
    WeakContainer* newObj = vd::Deserialise<WeakContainer>(bytes);

    if(*newObj == obj)
        std::cout << "SUCCESS" << std::endl;
    else
        std::cout << "FAILURE" << std::endl;

    delete newObj;
    delete[] bytes;
}

// ---------------------------------------------------------------------------------------------- //

class SomeBase
{
public:
    SomeBase(const bool& fl, const std::string& n)
        : m_flag(fl)
        , m_name(n)
    {

    }

    bool operator==(const SomeBase& that) const
    {
        return m_flag == that.m_flag && m_name == that.m_name;
    }

    VD_BEGIN_MEMBERS(SomeBase)
    VD_ADD_MEMBER(private, bool, m_flag)
    VD_ADD_MEMBER(private, std::string, m_name)
    VD_END_MEMBERS()
};

// ---------------------------------------------------------------------------------------------- //

VD_POLYMORPH(SomeBase)

// ---------------------------------------------------------------------------------------------- //

class SomeClass : public SomeBase
{
public:
    SomeClass(const bool& fl, const std::string& n, const float& f)
        : SomeBase(fl, n)
        , m_float(f)
    {

    }

    bool operator==(const SomeClass& that) const
    {
        return SomeBase::operator==(that) && m_float == that.m_float;
    }

    VD_BEGIN_MEMBERS(SomeClass)
    VD_BASE_CLASS(SomeBase)
    VD_ADD_MEMBER(private, float, m_float)
    VD_END_MEMBERS()
};

// ---------------------------------------------------------------------------------------------- //

VD_POLYMORPH(SomeClass)

// ---------------------------------------------------------------------------------------------- //

class SomeContainer
{
public:
    SomeContainer(std::auto_ptr<SomeBase> pb) : m_autoPtr(pb)
    {

    }

    SomeContainer(boost::shared_ptr<SomeBase> pb) : m_sharedPtr(pb)
    {

    }

    bool operator==(const SomeContainer& that) const
    {
        if(m_autoPtr.get() != 0 && that.m_autoPtr.get() != 0)
        {
            if(!(*m_autoPtr == *that.m_autoPtr))
                return false;
            else if(m_sharedPtr.get() != 0 && that.m_sharedPtr.get() != 0)
            {
                if(!(*m_sharedPtr == *that.m_sharedPtr))
                    return false;
                else
                    return true;
            }
            else if(m_sharedPtr.get() == 0 && that.m_sharedPtr.get() == 0)
                return true;
            else
                return false;
        }
        else if(m_autoPtr.get() == 0 && that.m_autoPtr.get() == 0)
        {
            if(m_sharedPtr.get() != 0 && that.m_sharedPtr.get() != 0)
            {
                if(!(*m_sharedPtr == *that.m_sharedPtr))
                    return false;
                else
                    return true;
            }
            else if(m_sharedPtr.get() == 0 && that.m_sharedPtr.get() == 0)
                return true;
            else
                return false;
        }
        else
            return false;
    }

    VD_BEGIN_MEMBERS(SomeContainer)
    VD_ADD_MEMBER(private, std::auto_ptr<SomeBase>, m_autoPtr)
    VD_ADD_MEMBER(private, boost::shared_ptr<SomeBase>, m_sharedPtr)
    VD_END_MEMBERS()
};

// ---------------------------------------------------------------------------------------------- //

void TestPointers()
{
    std::cout << "TestPointers" << std::endl;
    std::cout << "------------" << std::endl;
    SomeContainer obj(std::auto_ptr<SomeBase>(new SomeClass(true, "Gerrit", 2.25)));

    long length;
    char* bytes = vd::Serialise(obj, length);
    WriteBytes(bytes, length);
    SomeContainer* newObj = vd::Deserialise<SomeContainer>(bytes);

    if(*newObj == obj)
        std::cout << "SUCCESS" << std::endl;
    else
        std::cout << "FAILURE" << std::endl;

    delete newObj;
    delete[] bytes;

    SomeContainer obj2(boost::shared_ptr<SomeBase>(new SomeClass(false, "Gerrit", 2.25)));

    bytes = vd::Serialise(obj2, length);
    WriteBytes(bytes, length);
    newObj = vd::Deserialise<SomeContainer>(bytes);

    if(*newObj == obj2)
        std::cout << "SUCCESS" << std::endl;
    else
        std::cout << "FAILURE" << std::endl;

    delete newObj;
    delete[] bytes;
}

// ---------------------------------------------------------------------------------------------- //

class Color
{
public:
    Color(int r, int g, int b)
        : m_red(r)
        , m_green(g)
        , m_blue(b)
    {

    }

    bool operator==(const Color& that) const
    {
        return m_red == that.m_red && m_green == that.m_green && m_blue == that.m_blue;
    }

    VD_BEGIN_MEMBERS(Color)
    VD_ADD_MEMBER(private, int, m_red)
    VD_ADD_MEMBER(private, int, m_green)
    VD_ADD_MEMBER(private, int, m_blue)
    VD_END_MEMBERS()
};

// ---------------------------------------------------------------------------------------------- //

class Shape
{
public:
    Shape(Color c)
        : m_fill(c)
    {

    }

    virtual ~Shape() { }

    bool operator==(const Shape& that) const
    {
        return m_fill == that.m_fill;
    }

    VD_BEGIN_MEMBERS(Shape)
    VD_ADD_MEMBER(protected, Color, m_fill)
    VD_END_MEMBERS()
};

// ---------------------------------------------------------------------------------------------- //

VD_POLYMORPH(Shape)

// ---------------------------------------------------------------------------------------------- //

class Circle : public Shape
{
public:
    Circle(Color c, float x, float y, float r)
        : Shape(c)
        , m_x(x)
        , m_y(y)
        , m_radius(r)
    {

    }

    virtual ~Circle() { }

    bool operator==(const Circle& that) const
    {
        return Shape::operator==(that) && m_x == that.m_x && m_y == that.m_y &&
               m_radius == that.m_radius;
    }

    VD_BEGIN_MEMBERS(Circle)
    VD_BASE_CLASS(Shape)
    VD_ADD_MEMBER(private, float, m_x)
    VD_ADD_MEMBER(private, float, m_y)
    VD_ADD_MEMBER(private, float, m_radius)
    VD_END_MEMBERS()
};

// ---------------------------------------------------------------------------------------------- //

VD_POLYMORPH(Circle)

// ---------------------------------------------------------------------------------------------- //

class Rectangle : public Shape
{
public:
    Rectangle(Color c, float x, float y, float w, float h)
        : Shape(c)
        , m_x(x)
        , m_y(y)
        , m_width(w)
        , m_height(h)
    {

    }

    virtual ~Rectangle() { }

    bool operator==(const Rectangle& that) const
    {
        return Shape::operator==(that) && m_x == that.m_x && m_y == that.m_y &&
               m_width == that.m_width && m_height == that.m_height;
    }

    VD_BEGIN_MEMBERS(Rectangle)
    VD_BASE_CLASS(Shape)
    VD_ADD_MEMBER(private, float, m_x)
    VD_ADD_MEMBER(private, float, m_y)
    VD_ADD_MEMBER(private, float, m_width)
    VD_ADD_MEMBER(private, float, m_height)
    VD_END_MEMBERS()
};

// ---------------------------------------------------------------------------------------------- //

VD_POLYMORPH(Rectangle)

// ---------------------------------------------------------------------------------------------- //

class Drawing
{
public:
    Drawing(Color bg)
        : m_background(bg)
    {

    }

    void addShape(boost::shared_ptr<Shape> shape)
    {
        m_shapes.push_back(shape);
    }

    virtual bool operator==(const Drawing& that)
    {
        if(!(m_background == that.m_background))
            return false;

        if(m_shapes.size() != that.m_shapes.size())
            return false;

        for(int i = 0; i < (int)m_shapes.size(); ++i)
        {
            if(m_shapes[i].get() == 0 || that.m_shapes[i].get() == 0)
            {
                if(m_shapes[i].get() != that.m_shapes[i].get())
                    return false;
            }
            else if(!(*(m_shapes[i]) == *(that.m_shapes[i])))
                return false;
        }

        return true;
    }

    VD_BEGIN_MEMBERS(Drawing)
    VD_ADD_MEMBER(private, Color, m_background)
    VD_ADD_MEMBER(private, std::vector< boost::shared_ptr<Shape> >, m_shapes)
    VD_END_MEMBERS()
};

// ---------------------------------------------------------------------------------------------- //

void TestPolymorph()
{
    std::cout << "TestPolymorph" << std::endl;
    std::cout << "-------------" << std::endl;

    Drawing drawing(Color(0, 255, 0));
    boost::shared_ptr<Shape> circle(new Circle(Color(255, 255, 0), 1.0, 2.0, 3.0));
    boost::shared_ptr<Shape> rectangle(new Rectangle(Color(127, 127, 127), 2.0, 4.0, 3.0, 3.0));
    drawing.addShape(circle);
    drawing.addShape(rectangle);
    drawing.addShape(circle);

    long length;
    char* bytes = vd::Serialise(drawing, length);
    WriteBytes(bytes, length);
    Drawing* newDrawing = vd::Deserialise<Drawing>(bytes);

    if(*newDrawing == drawing)
        std::cout << "SUCCESS" << std::endl;
    else
        std::cout << "FAILURE" << std::endl;

    delete newDrawing;
    delete[] bytes;
}

// ---------------------------------------------------------------------------------------------- //

class Base0
{
public:
    Base0(const int& v) : m_val0(v) {}
    virtual ~Base0() {}

    bool operator==(const Base0& that) const
    {
        return m_val0 == that.m_val0;
    }

    VD_BEGIN_MEMBERS(Base0)
    VD_ADD_MEMBER(private, int, m_val0)
    VD_END_MEMBERS()
};

// ---------------------------------------------------------------------------------------------- //

VD_POLYMORPH(Base0)

// ---------------------------------------------------------------------------------------------- //

class Base1
{
public:
    Base1(const int& v) : m_val1(v) {}
    virtual ~Base1() {}

    bool operator==(const Base1& that) const
    {
        return m_val1 == that.m_val1;
    }

    VD_BEGIN_MEMBERS(Base1)
    VD_ADD_MEMBER(private, int, m_val1)
    VD_END_MEMBERS()
};

// ---------------------------------------------------------------------------------------------- //

VD_POLYMORPH(Base1)

// ---------------------------------------------------------------------------------------------- //

class Derived0 : public Base0, public Base1
{
public:
    Derived0(const int& v0, const int& v1, const std::string& str)
        : Base0(v0), Base1(v1), m_str(str)
    {}

    virtual ~Derived0() {}

    bool operator==(const Derived0& that)
    {
        return Base0::operator==(that) && Base1::operator==(that) && m_str == that.m_str;
    }

    VD_BEGIN_MEMBERS(Derived0)
    VD_BASE_CLASS(Base0)
    VD_BASE_CLASS(Base1)
    VD_ADD_MEMBER(private, std::string, m_str)
    VD_END_MEMBERS()
};

// ---------------------------------------------------------------------------------------------- //

void TestMultipleInheritance0()
{
    std::cout << "TestMultipleInheritance0" << std::endl;
    std::cout << "------------------------" << std::endl;
    Derived0 obj(1, 2, "abc");

    long length;
    char* bytes = vd::Serialise(obj, length);
    WriteBytes(bytes, length);
    Derived0* newObj = vd::Deserialise<Derived0>(bytes);

    if(*newObj == obj)
        std::cout << "SUCCESS" << std::endl;
    else
        std::cout << "FAILURE" << std::endl;

    delete newObj;
    delete[] bytes;
}

// ---------------------------------------------------------------------------------------------- //

class Derived1 : public Base0, public Base1
{
public:
    Derived1(const int& v0, const int& v1, const std::string& str)
        : Base0(v0), Base1(v1), m_str(str)
    {}

    virtual ~Derived1() {}

    bool operator==(const Derived1& that)
    {
        return Base0::operator==(that) && Base1::operator==(that) && m_str == that.m_str;
    }

    VD_BEGIN_MEMBERS(Derived1)
    VD_BASE_CLASS(Base0)
    VD_BASE_CLASS(Base1)
    VD_ADD_MEMBER(private, std::string, m_str)
    VD_END_MEMBERS()
};

// ---------------------------------------------------------------------------------------------- //

VD_POLYMORPH_MULTIPLE_2(Derived1, Base0, Base1)

// ---------------------------------------------------------------------------------------------- //

class Container
{
public:
    void setDerived(boost::shared_ptr<Derived1> ptr)
    {
        m_ptr = ptr;
    }

    void setBase0(boost::shared_ptr<Base0> ptr)
    {
        m_ptr0 = ptr;
    }

    void setBase1(boost::shared_ptr<Base1> ptr)
    {
        m_ptr1 = ptr;
    }

    bool operator==(const Container& that)
    {
        return (*m_ptr == *that.m_ptr) && (*m_ptr0 == *that.m_ptr0) && (*m_ptr1 == *that.m_ptr1);
    }

    VD_BEGIN_MEMBERS(Container)
    VD_ADD_MEMBER(private, boost::shared_ptr<Base1>, m_ptr1)
    VD_ADD_MEMBER(private, boost::shared_ptr<Derived1>, m_ptr)
    VD_ADD_MEMBER(private, boost::shared_ptr<Base0>, m_ptr0)
    VD_END_MEMBERS()
};

// ---------------------------------------------------------------------------------------------- //

void TestMultipleInheritance1()
{
    std::cout << "TestMultipleInheritance1" << std::endl;
    std::cout << "------------------------" << std::endl;
    Container obj;
    obj.setDerived(boost::shared_ptr<Derived1>(new Derived1(1, 2, "a")));
    obj.setBase0(boost::shared_ptr<Base0>(new Derived1(3, 4, "b")));
    obj.setBase1(boost::shared_ptr<Base1>(new Derived1(5, 6, "c")));

    long length;
    char* bytes = vd::Serialise(obj, length);
    WriteBytes(bytes, length);
    Container* newObj = vd::Deserialise<Container>(bytes);

    if(*newObj == obj)
        std::cout << "SUCCESS" << std::endl;
    else
        std::cout << "FAILURE" << std::endl;

    delete newObj;
    delete[] bytes;
}

// ---------------------------------------------------------------------------------------------- //

void TestMultipleInheritance2()
{
    std::cout << "TestMultipleInheritance2" << std::endl;
    std::cout << "------------------------" << std::endl;
    Container obj;
    boost::shared_ptr<Derived1> ptr(new Derived1(1, 2, "a"));
    obj.setDerived(ptr);
    obj.setBase0(ptr);
    obj.setBase1(ptr);

    long length;
    char* bytes = vd::Serialise(obj, length);
    WriteBytes(bytes, length);
    Container* newObj = vd::Deserialise<Container>(bytes);

    if(*newObj == obj)
        std::cout << "SUCCESS" << std::endl;
    else
        std::cout << "FAILURE" << std::endl;

    delete newObj;
    delete[] bytes;
}

// ---------------------------------------------------------------------------------------------- //

class InitializeBase
{
public:
    InitializeBase(int val0, int val1)
        : m_val0(val0)
        , m_val1(val1)
    {
        m_sum = val0 + val1;
    }

    static void initialize(InitializeBase& obj, int version)
    {
        obj.m_sum = obj.m_val0 + obj.m_val1;
    }

    bool operator==(const InitializeBase& that) const
    {
        return (m_val0 == that.m_val0) && (m_val1 == that.m_val1) && (m_sum == that.m_sum);
    }

    VD_BEGIN_MEMBERS(InitializeBase)
    VD_ADD_MEMBER(protected, int, m_val0)
    VD_ADD_MEMBER(protected, int, m_val1)
    VD_END_MEMBERS()

private:
    int m_sum;
};

// ---------------------------------------------------------------------------------------------- //

class InitializeClass : public InitializeBase
{
public:
    InitializeClass(int val0, int val1, int val2)
        : InitializeBase(val0, val1)
        , m_val2(val2)
    {
        m_product = val0 * val1 * val2;
    }

    static void initialize(InitializeClass& obj, int version)
    {
        obj.m_product = obj.m_val0 * obj.m_val1 * obj.m_val2;
    }

    bool operator==(const InitializeClass& that) const
    {
        return InitializeBase::operator==(that) && (m_val2 == that.m_val2) && (m_product == that.m_product);
    }

    VD_BEGIN_MEMBERS(InitializeClass)
    VD_BASE_CLASS(InitializeBase)
    VD_ADD_MEMBER(private, int, m_val2)
    VD_END_MEMBERS()

private:
    int m_product;
};

// ---------------------------------------------------------------------------------------------- //

void TestInitialize()
{
    std::cout << "TestInitialize" << std::endl;
    std::cout << "--------------" << std::endl;
    InitializeClass obj(5, 7, 3);

    long length;
    char* bytes = vd::Serialise(obj, length);
    WriteBytes(bytes, length);
    InitializeClass* newObj = vd::Deserialise<InitializeClass>(bytes);

    if(*newObj == obj)
        std::cout << "SUCCESS" << std::endl;
    else
        std::cout << "FAILURE" << std::endl;

    delete newObj;
    delete[] bytes;
}

// ---------------------------------------------------------------------------------------------- //

class EndianClass
{
public:
    EndianClass(char c, short s, int i, long l, float f, double d)
        : m_c(c), m_s(s), m_i(i), m_l(l), m_f(f), m_d(d)
    { }

    bool operator==(const EndianClass& that) const
    {
        return m_c == that.m_c && m_s == that.m_s && m_i == that.m_i && m_l == that.m_l && m_f == that.m_f && m_d == that.m_d;
    }

    VD_BEGIN_MEMBERS(EndianClass)
    VD_ADD_MEMBER(private, char, m_c)
    VD_ADD_MEMBER(private, short, m_s)
    VD_ADD_MEMBER(private, int, m_i)
    VD_ADD_MEMBER(private, long, m_l)
    VD_ADD_MEMBER(private, float, m_f)
    VD_ADD_MEMBER(private, double, m_d)
    VD_END_MEMBERS()
};

// ---------------------------------------------------------------------------------------------- //

void TestLittleEndian()
{
    std::cout << "TestLittleEndian" << std::endl;
    std::cout << "----------------" << std::endl;
    EndianClass obj(0x01, 0x0203, 0x04050607, 0x08090a0b0c0d0e0f, 2.0, 3.0);

    long length;
    char* bytes = vd::Serialise<8, VD_LITTLE_ENDIAN>(obj, length);
    WriteBytes(bytes, length);

    char expected[] = { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                        , 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                        , 0x03, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                        , 0x07, 0x06, 0x05, 0x04, 0x00, 0x00, 0x00, 0x00
                        , 0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08
                        , 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00
                        , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x40
                      };

    for(int i = 0; i < length; ++i)
    {
        if(*(bytes + i) != expected[i])
        {
            std::cout << "FAILURE invalid format" << std::endl;
            return;
        }
    }

    EndianClass* newObj = vd::Deserialise<EndianClass, 8, VD_LITTLE_ENDIAN>(bytes);

    if(*newObj == obj)
        std::cout << "SUCCESS" << std::endl;
    else
        std::cout << "FAILURE" << std::endl;

    delete newObj;
    delete[] bytes;
}

// ---------------------------------------------------------------------------------------------- //

void TestBigEndian()
{
    std::cout << "TestBigEndian" << std::endl;
    std::cout << "-------------" << std::endl;
    EndianClass obj(0x01, 0x0203, 0x04050607, 0x08090a0b0c0d0e0f, 2.0, 3.0);

    long length;
    char* bytes = vd::Serialise<8, VD_BIG_ENDIAN>(obj, length);
    WriteBytes(bytes, length);

    char expected[] = { 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00
                        , 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                        , 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                        , 0x04, 0x05, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00
                        , 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
                        , 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                        , 0x40, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                      };

    for(int i = 0; i < length; ++i)
    {
        if(*(bytes + i) != expected[i])
        {
            std::cout << "FAILURE invalid format" << std::endl;
            return;
        }
    }

    EndianClass* newObj = vd::Deserialise<EndianClass, 8, VD_BIG_ENDIAN>(bytes);

    if(*newObj == obj)
        std::cout << "SUCCESS" << std::endl;
    else
        std::cout << "FAILURE" << std::endl;

    delete newObj;
    delete[] bytes;
}

// ---------------------------------------------------------------------------------------------- //

class VersionedClass
{
public:
    VersionedClass(int v1, int v2) : m_v1(v1), m_v2(v2) { }

    int getV1() const { return m_v1; }
    int getV2() const { return m_v2; }

    static void initialize(VersionedClass& obj, int version)
    {
        if(version < 2) obj.m_v2 = 0;
    }

    VD_BEGIN_MEMBERS(VersionedClass)
    VD_ADD_MEMBER(private, int, m_v1)
    VD_ADD_MEMBER_V(private, int, m_v2, 2)
    VD_END_MEMBERS()
};

// ---------------------------------------------------------------------------------------------- //

void TestVersion()
{
    std::cout << "TestVersion" << std::endl;
    std::cout << "-----------" << std::endl;
    VersionedClass obj(1, 2);

    long length;
    char* bytes = vd::Serialise(obj, length, 1);
    WriteBytes(bytes, length);
    VersionedClass* newObj = vd::Deserialise<VersionedClass>(bytes, 1);

    if(obj.getV1() != newObj->getV1() || newObj->getV2() != 0)
        std::cout << "FAILURE" << std::endl;
    else
        std::cout << "SUCCESS" << std::endl;

    delete newObj;
    delete[] bytes;

    bytes = vd::Serialise(obj, length, 2);
    WriteBytes(bytes, length);
    newObj = vd::Deserialise<VersionedClass>(bytes, 2);

    if(obj.getV1() != newObj->getV1() || obj.getV2() != newObj->getV2())
        std::cout << "FAILURE" << std::endl;
    else
        std::cout << "SUCCESS" << std::endl;

    delete newObj;
    delete[] bytes;
}

// ---------------------------------------------------------------------------------------------- //

int main(int, char**)
{
    TestSimple();
    TestComposite();
    TestDerived();
    TestVector();
    TestList();
    TestMap();
    TestSharedPtr();
    TestWeakPtr();
    TestPointers();
    TestPolymorph();
    TestMultipleInheritance0();
    TestMultipleInheritance1();
    TestMultipleInheritance2();
    TestInitialize();
    TestLittleEndian();
    TestBigEndian();
    TestVersion();
}

// ---------------------------------------------------------------------------------------------- //
