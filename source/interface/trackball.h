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

#ifndef VD_UI_TRACKBALL_INCLUDED
#define VD_UI_TRACKBALL_INCLUDED

// ============================================================================================== //

#include "interface/interface.h"
#include "interface/controls.h"

// ============================================================================================== //

VD_INTERFACE_NAMESPACE_BEGIN();

// ============================================================================================== //

#if 0

class TrackBall : public CameraControlInterface 
{
public:
	TrackBall(const vd::graphics::Viewport& viewport=vd::graphics::Viewport(0,0,0,0), CameraControlInterface::Mode mode=OrbitMode);
	virtual vd::flag Reset();
	virtual vd::flag IsActive() const;
	virtual vd::flag IsDone() const;
	virtual vd::flag SetMode(CameraControlInterface::Mode mode);
	virtual CameraControlInterface::Mode GetMode(void) const { return m_Mode; }
	virtual vd::flag OnMouseDown(vd::i32 x, vd::i32 y, vd::flag ctrl, vd::flag alt, vd::flag sys);
	virtual vd::flag OnMouseUp(vd::i32 x, vd::i32 y, vd::flag ctrl, vd::flag alt, vd::flag sys);
	virtual vd::flag OnMouseMove(vd::i32 x, vd::i32 y, vd::flag ctrl, vd::flag alt, vd::flag sys);
	virtual vd::flag ReturnHome();
	virtual vd::flag Goto(const vd::q4f32& destination);
	virtual vd::f32 GetZoom() const;
	virtual vd::q4f32 GetRotation() const;
	virtual vd::flag Update(vd::u64 Microseconds);
	virtual vd::flag SetViewport(const vd::graphics::Viewport& viewport);

protected:
	void ResetPosition();
	void ResetGoal();
	void ResetInertia();
	vd::flag Track(const vd::v2i32 & point2D);
	vd::flag MapToSphere(const vd::v2i32& p2, vd::v3f32& v3);

	vd::v3f32 MapToSphere(vd::i32 x, vd::i32 y);

private:

	vd::v3f32 m_StartPos;
	vd::v3f32 m_CurrentPos;
	vd::v3f32 m_PreviousPos;
	vd::v3f32 m_DownPos;
	vd::v3f32 m_MovePos;
	vd::v3f32 m_UpPos;
	vd::v3f32 m_Axis;
	vd::v3f32 m_LastPoint3D;
	vd::core::Quaternionf m_Orientation;
	vd::graphics::Viewport m_Viewport;
	
	struct Goal {
		vd::core::Quaternionf StartOrientation;
		vd::core::Quaternionf EndOrientation;
		vd::u64 Microseconds;
		vd::i32 IsActive;
	} m_Goal;

	struct Inertia {
		vd::v3f32 Axis;
		vd::f32 RadiansPerSecond;
		vd::i32 IsActive;
	} m_Inertia;

	Mode m_Mode;

	vd::i32 m_IsDone;
	vd::i32 m_IsActive;
	vd::i32 m_LastPointOk;

	vd::f32 m_Radius;
	vd::f32 m_RadiansPerSec;
	vd::u64 m_CurrentTime;
	vd::u64 m_PreviousTime;
};

#endif

// ============================================================================================== //

VD_INTERFACE_NAMESPACE_END();

// ============================================================================================== //

#endif	// VD_UI_TRACKBALL_INCLUDED

// ============================================================================================== //
// end file
