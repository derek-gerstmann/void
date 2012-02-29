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

#ifndef VD_RENDER_SPHPBR_HEADER_INCLUDED
#define VD_RENDER_SPHPBR_HEADER_INCLUDED

// ============================================================================================== //

#include "render/render.h"
#include "graphics/shader.h"

// ============================================================================================== //

VD_RENDER_NAMESPACE_BEGIN();

// ============================================================================================== //

using VD_GRAPHICS_SCOPE::Shader;

// ============================================================================================== //

class SphPointBasedRenderer : public Object
{

public:
    
	VD_DECLARE_ENUM(DisplayMode,
		Invalid,
		Points,
		Splats);

    SphPointBasedRenderer();
    ~SphPointBasedRenderer();

    bool Setup();
    virtual vd::status Destroy();
    
    void SetVertexBuffer(vd::u32 uiBufferId, int iParticleCount, int vecsize);
	vd::u32 GetVertexBuffer(void) const		     { return m_VertexBufferId; }

    void SetColorBuffer(vd::u32 uiBufferId, int iParticleCount, int vecsize);
	vd::u32 GetColorBuffer(void) const		     { return m_ColorBufferId; }

    void SetColorRange(vd::f32 minval, vd::f32 maxval);
    vd::v2f32 GetColorRange(void) const          { return m_ColorRange; }

    void SetDensityBuffer(vd::u32 uiBufferId, int iParticleCount, int vecsize);
    vd::u32 GetDensityBuffer(void) const         { return m_DensityBufferId; }

    void SetDensityRange(vd::f32 minval, vd::f32 maxval);
    vd::v2f32 GetDensityRange(void) const          { return m_DensityRange; }

    void Render(bool dirty = false, DisplayMode::Value mode = DisplayMode::Splats);
    void RenderBoundary(vd::f32 x, vd::f32 y, vd::f32 z, vd::f32 w, vd::f32 h, vd::f32 d);

    void SetPointScale(vd::f32 scale);
    vd::f32 GetPointScale(void) const   		  { return m_PointScale; }

    void SetPointSize(vd::f32 v);
    vd::f32 GetPointSize(void) const   			  { return m_PointSize; }

    void SetSmoothingRadius(vd::f32 v);
    vd::f32 GetSmoothingRadius(void) const   	  { return m_SmoothingRadius; }

    void SetSmoothingScale(vd::f32 v);
    vd::f32 GetSmoothingScale(void) const   	  { return m_SmoothingScale; }

    void SetParticleRadius(vd::f32 r);
    vd::f32 GetParticleRadius(void) const         { return m_ParticleRadius; }

    void SetDensityScale(vd::f32 v);
    vd::f32 GetDensityScale(void) const   	      { return m_DensityScale; }

    void SetExposureScale(vd::f32 v);
    vd::f32 GetExposureScale(void) const          { return m_ExposureScale; }

    void SetBoxSize(vd::f32 v);
    vd::f32 GetBoxSize(void) const                { return m_BoxSize; }

    void SetAlphaScale(vd::f32 v);
    vd::f32 GetAlphaScale(void) const             { return m_AlphaScale; }

    void SetIntensityScale(vd::f32 v);
    vd::f32 GetIntensityScale(void) const         { return m_IntensityScale; }

    void SetCameraFov(vd::f32 fFov);
    vd::f32 GetCameraFov(void) const  			  { return m_CameraFov; }

    void SetScreenSize(vd::u32 w, vd::u32 h);
    void SetUseCustomProjection(bool v);
    void SetCameraPosition(const vd::v3f32& v);
    void SetCameraRotation(const vd::v3f32& v);
    void SetLightPosition(const vd::v3f32& v);
    
	VD_DECLARE_OBJECT(SphPointBasedRenderer);
	
protected:

	VD_DISABLE_COPY_CONSTRUCTORS(SphPointBasedRenderer);
	
    void DrawPoints();
    
protected:

	void EnableProjection();
	void DisableProjection();
	
    bool m_IsStale;
	bool m_UseCustomProjection;

    vd::u32 m_ParticleCount;

	vd::v3f32 m_LightPosition;
	vd::v3f32 m_CameraPosition;
	vd::v3f32 m_CameraRotation;
    vd::f32 m_CameraFov;
    vd::f32 m_CameraFocalLength;
	
    vd::u32 m_ScreenWidth;
    vd::u32 m_ScreenHeight;

	vd::f32 m_PointScale;
    vd::f32 m_PointSize;
    vd::f32 m_ParticleRadius;
	vd::f32 m_SmoothingRadius;
    vd::f32 m_SmoothingScale;
    vd::f32 m_DensityScale;
    vd::f32 m_ExposureScale;
    vd::f32 m_IntensityScale;
    vd::f32 m_AlphaScale;
	vd::f32 m_WdC;
    vd::f32 m_BoxSize;

    vd::u32 m_VertexBufferId;
    vd::i32 m_VertexComponents;

    vd::u32 m_ColorBufferId;
    vd::i32 m_ColorComponents;
    vd::v2f32 m_ColorRange;

    vd::u32 m_DensityBufferId;
    vd::i32 m_DensityComponents;
    vd::v2f32 m_DensityRange;

    Shader m_Shader;
};

// ============================================================================================== //

VD_RENDER_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_RENDER_SPHPBR_HEADER_INCLUDED
