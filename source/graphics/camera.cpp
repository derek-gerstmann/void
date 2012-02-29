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

#include "graphics/camera.h"

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_BEGIN();

// ============================================================================================== //
/*
Camera::Camera() :
    fovy_(M_PI / 3.0),
    focal_(1.0),
    near_(0.01f),
    far_(1000.0f),
    stale_view_(VD_TRUE),
    stale_projection_(VD_TRUE)
{
    view_.setIdentity();
    projection_.setIdentity();

	SetFovY(fovy_);
    SetPosition(Vector3f(0.0f, 0.0f, -5.0f));
    SetTarget(Vector3f(0.0f, 0.0f, 0.0f));
}

Camera & Camera::operator=(const Camera & other)
{
    stale_view_ = VD_TRUE;
    stale_projection_ = VD_TRUE;

	frame_.position = other.frame_.position;
	frame_.orientation = other.frame_.orientation;

	viewport_ = other.viewport_;
    target_ = other.target_;
    fovy_ = other.fovy_;
    near_ = other.near_;
    far_ = other.far_;

    view_ = other.view_;
    projection_ = other.projection_;

    return *this;
}

Camera::Camera(const Camera & other)
{
    *this = other;
}

Camera::~Camera()
{
	// EMPTY!
}

void Camera::SetViewport(const Viewport & vp)
{
    viewport_ = vp;
    stale_projection_ = VD_TRUE;
}

void Camera::SetFovY(vd::f32 value)
{
    fovy_ = value;
	focal_ = (1.0f / tanf(fovy_) / 2.0);
	stale_projection_ = VD_TRUE;
}

void Camera::SetClipRange(vd::f32 near, vd::f32 far)
{
	near_ = near;
	far_ = far;
//	if(near > far) std::swap(near_, far_);
    stale_projection_ = VD_TRUE;	
}

Vector3f Camera::GetDirection(void) const
{
    return - (GetOrientation() * Vector3f::UnitZ());
}

Vector3f Camera::GetUpVector(void) const
{
    return GetOrientation() * Vector3f::UnitY();
}

Vector3f Camera::GetRightVector(void) const
{
    return GetOrientation() * Vector3f::UnitX();
}

void Camera::SetDirection(const Vector3f & dir)
{
    // TODO implement it computing the rotation between newDirection and current dir ?
    Vector3f up = this->GetUpVector();

    Matrix3f axes;

    axes.col(2) = (-dir).normalized();
    axes.col(0) = up.cross(axes.col(2)).normalized();
    axes.col(1) = axes.col(2).cross(axes.col(0)).normalized();
    SetOrientation(Quaternionf(axes));

    stale_view_ = VD_TRUE;
}

void Camera::SetTarget(const Vector3f & target)
{
    target_ = target;

    if(!target_.isApprox(GetPosition()))
    {
        Vector3f dir = target_ - GetPosition();
        SetDirection(dir.normalized());
    }
}

void Camera::SetPosition(const Vector3f & p)
{
    frame_.position = p;
    stale_view_ = VD_TRUE;
}

void Camera::SetOrientation(const Quaternionf & q)
{
    frame_.orientation = q;
    stale_view_ = VD_TRUE;
}

void Camera::SetFrame(const Frame & f)
{
    frame_ = f;
    stale_view_ = VD_TRUE;
}

void Camera::OrbitAroundTarget(const Quaternionf & q)
{
    Matrix4f mrot, mt, mtm;

    // update the transform matrix
    UpdateViewMatrix();
    Vector3f t = view_ * target_;

    view_ = Translation3f(t)
                   * q
                   * Translation3f(-t)
                   * view_;

    Quaternionf qa(view_.linear());
    qa = qa.conjugate();

    SetOrientation(qa);
    SetPosition(- (qa * view_.translation()));

    stale_view_ = VD_FALSE;
}

void Camera::RotateAroundOrigin(const Quaternionf & q)
{
    UpdateViewMatrix();
    vd::f32 dist = (GetPosition() - target_).norm();
    SetOrientation(GetOrientation() * q);
    target_ = GetPosition() + dist * GetDirection();
    stale_view_ = VD_TRUE;
}

void Camera::Zoom(vd::f32 d)
{
    vd::f32 dist = (GetPosition() - target_).norm();

    if(dist > d)
    {
        SetPosition(GetPosition() + GetDirection() * d);
        stale_view_ = VD_TRUE;
    }
}

void Camera::Translate(const Vector3f & t)
{
    Vector3f trans = GetOrientation() * t;
    SetPosition(GetPosition() + trans);
    SetTarget(target_ + trans);
    stale_view_ = VD_TRUE;
}

void Camera::UpdateViewMatrix(void)
{
    if(stale_view_ == VD_TRUE)
    {
        Quaternionf q = GetOrientation().conjugate();
        view_.linear() = q.toRotationMatrix();
        view_.translation() = - (view_.linear() * GetPosition());
        stale_view_ = VD_FALSE;
    }
}

const Affine3f & Camera::GetViewMatrix(void)
{
    UpdateViewMatrix();
    return view_;
}

void Camera::UpdateProjectionMatrix(void)
{
    if(stale_projection_ == VD_TRUE)
    {
        projection_.setIdentity();
        vd::f32 aspect = AspectRatio(viewport_);
        vd::f32 theta = fovy_ * 0.5f;
        vd::f32 range = far_ - near_;
        vd::f32 invtan = 1.0f / tanf(theta);

        projection_(0, 0) = invtan / aspect;
        projection_(1, 1) = invtan;
        projection_(2, 2) = -(near_ + far_) / range;
        projection_(3, 2) = -1.0f;
        projection_(2, 3) = -2.0f * near_ * far_ / range;
        projection_(3, 3) = 0;

        stale_projection_ = VD_FALSE;
    }
}

const Matrix4f &
Camera::GetProjectionMatrix(void)
{
    UpdateProjectionMatrix();
    return projection_;
}

void Camera::Bind(void)
{
    //  glViewport(vpX(), vpY(), vpWidth(), vpHeight());
    //  gpu.loadMatrix(projectionMatrix(),GL_PROJECTION);
    //  gpu.loadMatrix(viewMatrix().matrix(),GL_MODELVIEW);
}


Vector3f
Camera::Unproject(
    const Vector2f & uv, vd::f32 depth)
{
    Matrix4f inv = view_.inverse().matrix();
    return Unproject(uv, depth, inv);
}

Vector3f
Camera::Unproject(
    const Vector2f & uv, vd::f32 depth, const Matrix4f & invModelview)
{
    UpdateViewMatrix();
    UpdateProjectionMatrix();

    Vector3f a(2.0f * uv.x() / vd::f32(viewport_.Width) - 1.0f, 2.0f * uv.y() / vd::f32(viewport_.Height) - 1.0f, 1.0f);
    a.x() *= depth / projection_(0, 0);
    a.y() *= depth / projection_(1, 1);
    a.z() = -depth;

    Vector4f b = invModelview * Vector4f(a.x(), a.y(), a.z(), 1.0f);
    return Vector3f(b.x(), b.y(), b.z());
}
*/

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_END();

// ============================================================================================== //
