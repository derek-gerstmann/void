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

#ifndef VD_UID_INCLUDED
#define VD_UID_INCLUDED

// ============================================================================================== //

#include "core/core.h"

// ============================================================================================== //

VD_NAMESPACE_BEGIN();

// ============================================================================================== //

class uid
{
public:

    uid(const u64 low = 0) 
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
    	*this = vd::string(str);
	}

    uid& operator = (const uid& rhs)
    {
        m_Data[1] = rhs.m_Data[1];
        m_Data[0] = rhs.m_Data[0];
        return *this;
    }

    uid& operator = (const u64 rhs)
    {
        m_Data[1] = 0;
        m_Data[0] = rhs;
        return *this;
    }

    uid& operator = (const vd::string& from);

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

    const u64& low() const { return m_Data[0]; }
    const u64& high() const { return m_Data[1]; }

    u64& low() { return m_Data[0]; }
    u64& high() { return m_Data[1]; }

    vd::string ToString() const
    {
        std::stringstream stream;
    	if(high() == 0)
        	stream << std::hex << low() << std::dec;
	    else
    	    stream << std::hex << high() << ':' << low() << std::dec;

        const std::string str = stream.str();
        return str;
    }

    static const uid Zero;

private:
    u64 m_Data[2];
};

inline std::ostream& operator << (std::ostream& os, const uid& id)
{
    if(id.high() == 0)
        os << std::hex << id.low() << std::dec;
    else
        os << std::hex << id.high() << ':' << id.low() << std::dec;

    return os;
}

inline uid operator+ (const uid& a, const u64& b)
{
    uid result = a;
    result.low() += b;

    if(result.low() < a.low())
        ++result.high();

    return result;
};

inline uid operator- (const uid& a, const u64& b)
{
    uid result = a;
    result.low() -= b;

    if(result.low() > a.low())
        --result.high();

    return result;
};

inline uid operator & (const uid& a, const uid& b)
{
    uid result = a;
    result.low() &= b.low();
    result.high() &= b.high();
    return result;
}

inline uid operator | (const uid& a, const uid& b)
{
    uid result = a;
    result.low() |= b.low();
    result.high() |= b.high();
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
        return x.low();
    }
};

// ============================================================================================== //

VD_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_UID_INCLUDED

// ============================================================================================== //
// END FILE

