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

#ifndef VD_GRAPHICS_CAMERA_INCLUDED
#define VD_GRAPHICS_CAMERA_INCLUDED

// ============================================================================================

#include "core/core.h"
#include "core/geometry.h"
#include "graphics/viewport.h"

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_BEGIN();

// ============================================================================================

class Camera
{
/*
    struct Frame
    {
    public:

        Frame(
            const Vector3f & pos = Vector3f::Zero(),
            const Quaternionf & o = Quaternionf()
        )	: orientation(o), position(pos)
        {
            // EMPTY!
        }

        Frame lerp(vd::f32 alpha, const Frame & other) const
        {
            return Frame(
                       (1.0f - alpha) * position + alpha * other.position,
                       orientation.slerp(alpha, other.orientation)
                   );
        }

		Quaternionf orientation;
        Vector3f 	position;

    };

public:
    Camera(void);
    Camera(const Camera & other);

    virtual ~Camera();

    Camera & operator=(const Camera & other);

    void SetViewport(const Viewport & vp);
    inline const Viewport & GetViewport(void) const { return viewport_; }

	void SetClipRange(vd::f32 near, vd::f32 far);
	inline vd::f32 GetNear(void) const { return near_; }
	inline vd::f32 GetFar(void)const { return far_; }
	
    void SetFovY(vd::f32 value);
    inline vd::f32 GetFovY(void) const { return fovy_; }

	inline vd::f32 GetFocalLength(void) const { return focal_; }
	
    void SetPosition(const Vector3f & pos);
    inline const Vector3f & GetPosition(void) const { return frame_.position; }

    void SetOrientation(const Quaternionf & o);
    inline const Quaternionf & GetOrientation(void) const { return frame_.orientation; }

    void SetFrame(const Frame & f);
    const Frame & GetFrame(void) const { return frame_; }

    void SetDirection(const Vector3f & v);
    Vector3f GetDirection(void) const;

    void SetUpVector(const Vector3f & v);
    Vector3f GetUpVector(void) const;

    Vector3f GetRightVector(void) const;

    void SetTarget(const Vector3f & v);
    inline const Vector3f & GetTarget(void) const { return target_; }

	inline vd::flag IsStale(void) const { return (stale_projection_ == VD_FALSE || stale_view_ == VD_FALSE) ? VD_TRUE : VD_FALSE; }
	
    const Affine3f & GetViewMatrix(void);
    const Matrix4f & GetProjectionMatrix(void);

    void RotateAroundOrigin(const Quaternionf & q);
    void OrbitAroundTarget(const Quaternionf & q);

    void Zoom(vd::f32 d);

    void Translate(const vd::core::Vector3f & t);

    void Bind(void);

    Vector3f Unproject(const Vector2f & uv, vd::f32 depth, const vd::core::Matrix4f & invModelview);
    Vector3f Unproject(const Vector2f & uv, vd::f32 depth);

protected:

    void UpdateViewMatrix(void);
    void UpdateProjectionMatrix(void);

protected:

    mutable Affine3f view_;
    mutable Matrix4f projection_;
    mutable Vector3f target_;

    mutable Frame frame_;
    mutable Viewport viewport_;

    vd::f32 fovy_;
	vd::f32 focal_;
    vd::f32 near_;
    vd::f32 far_;

    mutable vd::flag stale_view_;
    mutable vd::flag stale_projection_;
*/
};

// ============================================================================================

VD_GRAPHICS_NAMESPACE_END();

// ============================================================================================

#endif // VD_GRAPHICS_CAMERA_INCLUDED
