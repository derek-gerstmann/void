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

#ifndef VD_CORE_GEOMETRY_INCLUDED
#define VD_CORE_GEOMETRY_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/builtins.h"
#include "core/numerics.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

inline vd::v3f32 
ProjectVector(
	const vd::v3f32& p, 
	const vd::v3f32& a, 
	const vd::v3f32& b)
{
	vd::v3f32 ba;
	vd::v3f32 pa;
	
	ba = b - a;
	pa = p - a;
	
	vd::f32 t = Dot(ba, pa) / LengthSqr(ba);
	return a + t * ba;
}

inline void
OrientationFromUnitVectors( 
	const vd::v3f32& unitVec0, const vd::v3f32& unitVec1,
	vd::v3f32& axis, vd::f32& angle)
{
    vd::f32 cosHalfAngleX2;
    vd::f32 recipCosHalfAngleX2;
    cosHalfAngleX2 = Sqrt( 2.0f * ( 1.0f + ( Dot(unitVec0, unitVec1 ) ) ) );
    recipCosHalfAngleX2 = ( 1.0f / cosHalfAngleX2 );
    axis = ( Cross(unitVec0, unitVec1 ) * recipCosHalfAngleX2 );
    angle = cosHalfAngleX2 * 0.5f;
}

/*
inline vd::m4f32 
PickBox(
	float centerX, 
	float centerY, 
	float width, 
	float height, 
	int viewport[4])
{
	float sx = viewport[2] / width;
	float sy = viewport[3] / height;
	float tx = (viewport[2] + 2.0f * (viewport[0] - centerX)) / width;
	float ty = (viewport[3] + 2.0f * (viewport[1] - centerY)) / height;

	vd::v4f32 c0(sx, 0, 0, tx);
	vd::v4f32 c1(0, sy, 0, ty);
	vd::v4f32 c2(0, 0, 1, 0);
	vd::v4f32 c3(0, 0, 0, 1);

	return transpose(vd::m4f32(c0, c1, c2, c3));
}
*/

inline vd::v3f32 
Perspective(const vd::v4f32& v)
{
	return v3f32(v.x, v.y, v.z) / v.w;
}

inline vd::v3f32 
Perpendicular(const vd::v3f32& a)
{
	vd::v3f32 c = vd::v3f32(1, 0, 0);
	vd::v3f32 b = Cross(a, c);
	if (LengthSqr(b) < 0.01f)
	{
		c = vd::v3f32(0, 1, 0);
		b = Cross(a, c);
	}
	return b;
}

inline const vd::q4f32
OrientationFromUnitVectors( 
	const vd::v3f32& unitVec0, const vd::v3f32& unitVec1 )
{
    float cosHalfAngleX2, recipCosHalfAngleX2;
    cosHalfAngleX2 = Sqrt( 2.0f * ( 1.0f + ( Dot(unitVec0, unitVec1 ) ) ) );
    recipCosHalfAngleX2 = ( 1.0f / cosHalfAngleX2 );
    vd::v3f32 axis = ( Cross(unitVec0, unitVec1 ) * recipCosHalfAngleX2 );
    vd::f32 angle = cosHalfAngleX2 * 0.5f;
    return vd::q4f32::AxisAngle(axis, angle);
}

inline const vd::q4f32
OrientationFromAngleAxis(
	float angle, const vd::v3f32& axis)
{
    return vd::q4f32::AxisAngle(axis, angle);
}

template<class V>
typename V::ScalarType OrthoProjectedArea(
	V view_dir, V box_min, V box_max)
{
    V delta = box_max - box_min;
    V dyzx = V(delta.y, delta.z, delta.x);
    V dzxy = V(delta.z, delta.x, delta.y);
    return Dot( Abs( view_dir ), dyzx * dzxy );
}

template<class V>
typename V::ScalarType LevelOfDetail(
	V view_pos, V object_pos, typename V::ScalarType object_area)
{
    return object_area * Rcp( DistanceSqr(view_pos, object_pos) );
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_GEOMETRY_INCLUDED 

