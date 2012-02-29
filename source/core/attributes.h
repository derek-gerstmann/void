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

#ifndef VD_CORE_ATTRIBUTES_INCLUDED
#define VD_CORE_ATTRIBUTES_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/event.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

template< class T >
class Attribute
{
public:
	struct CommitArgs
	{
		CommitArgs( const T& older, const T& newer )
			: existing( older )
			, requested( newer )
		{

		}

		const T& existing;
		const T& requested;
	};

	struct RequestArgs
	{
		RequestArgs( const T& older, const T& newer )
		: 	existing( older ),
			requested( newer ),
			veto( false )
		{

		}

		const T& existing;
		const T& requested;
		mutable bool veto;
	};

	typedef vd::Signature< const RequestArgs& >  RequestSignature;
	typedef vd::Signature< const CommitArgs& >    CommitSignature;

	Attribute()
		: m_Value ()
	{

	}

	Attribute( const T& value )
		: m_Value ( value )
	{

	}

	T& Value()
	{
		return m_Value;
	}

	const T& Get() const
	{
		return m_Value;
	}

	bool Set(const T& value)
	{
		if ( m_Value != value )
		{
			RequestArgs args ( m_Value, value );
			m_RequestEvent.Raise( args );
			if ( !args.veto )
			{
				T previous = m_Value;
				m_Value = value;
				m_CommitEvent.Raise( CommitArgs( previous, m_Value ) );
				return true;
			}
		}

		return false;
	}

	typename RequestSignature::Event& request()
	{
		return m_RequestEvent;
	}

	typename CommitSignature::Event& commit()
	{
		return m_CommitEvent;
	}

	void RaiseChanged( const T& previous = T() )
	{
		m_CommitEvent.Raise( CommitArgs( previous, m_Value ) );
	}

protected:
	T                                  m_Value;
	typename RequestSignature::Event   m_RequestEvent;
	typename CommitSignature::Event    m_CommitEvent;
};

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_ATTRIBUTES_INCLUDED

