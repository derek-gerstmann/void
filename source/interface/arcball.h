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

#ifndef VD_INTERFACE_ARCBALL_INCLUDED
#define VD_INTERFACE_ARCBALL_INCLUDED

// ============================================================================================== //

#include "graphics/graphics.h"
#include "core/geometry.h"

// ============================================================================================== //

VD_INTERFACE_NAMESPACE_BEGIN();

// ============================================================================================== //

#if 0

class Arcball 
{
public:
	Arcball()
		: m_WindowSizeX( 0 ),
		  m_WindowSizeY( 0 ) 
	{
		DisableConstraints();
		ResetOrientation();
	}
	
	Arcball( int w, int h )
		: m_WindowSizeX( w ),
		  m_WindowSizeY( h ) 
	{	
		SetCenter(m_WindowSizeX / 2.0f, m_WindowSizeY / 2.0f );
		m_Radius = std::min( (float)m_WindowSizeX / 2, (float)m_WindowSizeY / 2 );
		m_IsChanged = true;
		DisableConstraints();
		ResetOrientation();
	}
	
	void OnMouseDown( int x, int y  )
	{
		m_IsChanged = true;
		m_StartMousePosX = x;
		m_StartMousePosY = y;		
		m_StartOrientation = m_CurrentOrientation;
	}

	void OnMouseUp( int x, int y  )
	{
		m_IsChanged = false;
	}
	
	void OnMouseMotion( int x, int y )
	{
		if(!m_IsChanged)
			return;
			
		Vector3f from = MapToSphere( m_StartMousePosX, m_StartMousePosY );
		Vector3f to = MapToSphere( x, y );
		
		if( m_UseConstraint ) 
		{
			from = Constrain( from, m_ConstraintAxis );
			to = Constrain( to, m_ConstraintAxis );
		}
		
		Vector3f axis = from.cross(to);
		m_CurrentOrientation = m_StartOrientation * vd::core::OrientationFromAngleAxis( from.dot(to), axis );
		m_CurrentOrientation.normalize();
	}
	
	vd::flag 	IsChanged(){ return m_IsChanged; }
	void	ResetOrientation() { m_CurrentOrientation = m_StartOrientation = vd::core::OrientationFromAngleAxis(0.0f, Vector3f(0.0f, 1.0f, 0.0f) ); }
	const Quaternionf& GetOrientation() { return m_CurrentOrientation; }
	void	SetOrientation( const Quaternionf &q ) { m_CurrentOrientation = q; }
	
	void	SetWindowSize( int w, int h ) { m_WindowSizeX = w; m_WindowSizeY = h; }
	void	SetCenter( float x, float y ) { m_Center = Vector3f( x, y, 0); }
	void	SetRadius( float aRadius ) { m_Radius = aRadius; }
	float	GetRadius() const { return m_Radius; }
	void	SetConstraintAxis( const Vector3f &aConstraintAxis ) { m_ConstraintAxis = aConstraintAxis; m_UseConstraint = true; }
	void	DisableConstraints() { m_UseConstraint = false; }
	vd::flag	IsUsingConstraint() const { return m_UseConstraint; }
	Vector3f	GetConstraintAxis() const { return m_ConstraintAxis; }
	
	Vector3f MapToSphere( int x, int y ) 
	{
		Vector3f result;
		Vector3f point = Vector3f(x, y, 0.0f);
		result = Vector3f(( point.x() - m_Center.x() ) / ( m_Radius * 2.0f ), 
						  ( point.y() - m_Center.y() ) / ( m_Radius * 2.0f ), 0.0f);

		float mag = result.squaredNorm();
		if( mag > 1.0f ) 
		{
			result.normalize();
		}
		else 
		{
			result.z() = sqrt( 1.0f - mag );
			result.normalize();
		}

		return result;
	}
	
 private:

	Vector3f Constrain( const Vector3f &loose, const Vector3f &axis )
	{
		float norm;
		Vector3f onPlane = loose - axis * axis.dot(loose);
		norm = onPlane.squaredNorm();
		if( norm > 0.0f ) 
		{
			if( onPlane.z() < 0.0f )
				onPlane = -onPlane;
				
			return ( onPlane * ( 1.0f / sqrtf( norm ) ) );
		}
		
		if( axis.dot(Vector3f::UnitZ()) < 0.0001f ) 
		{
			onPlane = Vector3f::UnitY();
		}
		else 
		{
			onPlane = Vector3f( -axis.y(), axis.x(), 0.0f );
			onPlane.normalize();
		}
		
		return onPlane;
	}
	
	vd::flag			m_IsChanged;
	int				m_WindowSizeX;
	int				m_WindowSizeY;
	int				m_StartMousePosX;
	int				m_StartMousePosY;
	Vector3f		m_Center;
	Quaternionf		m_CurrentOrientation;
	Quaternionf  	m_StartOrientation;
	float			m_Radius;
	Vector3f		m_ConstraintAxis;
	vd::flag			m_UseConstraint;
};

#endif

// ============================================================================================== //

VD_INTERFACE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_INTERFACE_ARCBALL_INCLUDED 

// ============================================================================================== //
// end file
