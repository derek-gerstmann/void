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

#ifndef VD_RENDER_MBPBR_HEADER_INCLUDED
#define VD_RENDER_MBPBR_HEADER_INCLUDED

// ============================================================================================== //

#include "vd.h"
#include "render/render.h"
#include "graphics/graphics.h"
#include "graphics/shader.h"

// ============================================================================================== //

VD_RENDER_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_IMPORT(Graphics, Shader);

// ============================================================================================== //

class MotionPointBasedRenderer : public Object
{

public:
    
	VD_DECLARE_ENUM(DisplayMode,
		Points,
		Splats);

    MotionPointBasedRenderer();
    ~MotionPointBasedRenderer();

    virtual vd::status Setup(Graphics::Context* context);
    virtual vd::status Destroy();
    
    vd::u32 GetParticleCount(void) const         { return m_ParticleCount; }

    void SetVertexBuffer(vd::u32 uiBufferId, int iParticleCount, int vecsize);
	vd::u32 GetVertexBuffer(void) const		     { return m_VertexBufferId; }
    vd::u32 GetVertexComponents(void) const      { return m_VertexComponents; }

    void SetVertexRange(vd::f32 minval, vd::f32 maxval);
    vd::v2f32 GetVertexRange(void) const          { return m_VertexRange; }

    void SetColorBuffer(vd::u32 uiBufferId, int iParticleCount, int vecsize);
	vd::u32 GetColorBuffer(void) const		     { return m_ColorBufferId; }
    vd::u32 GetColorComponents(void) const      { return m_ColorComponents; }

    void SetColorRange(vd::f32 minval, vd::f32 maxval);
    vd::v2f32 GetColorRange(void) const          { return m_ColorRange; }

    void SetDensityBuffer(vd::u32 uiBufferId, int iParticleCount, int vecsize);
    vd::u32 GetDensityBuffer(void) const         { return m_DensityBufferId; }

    void SetDensityRange(vd::f32 minval, vd::f32 maxval);
    vd::v2f32 GetDensityRange(void) const          { return m_DensityRange; }
    vd::u32 GetDensityComponents(void) const      { return m_DensityComponents; }

    void Render(bool dirty = false, DisplayMode::Value mode = DisplayMode::Splats);

    void SetMinPointScale(vd::f32 scale);
    vd::f32 GetMinPointScale(void) const          { return m_MinPointScale; }

    void SetMaxPointScale(vd::f32 scale);
    vd::f32 GetMaxPointScale(void) const   		  { return m_MaxPointScale; }

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

    void SetAlphaBias(vd::f32 v);
    vd::f32 GetAlphaBias(void) const             { return m_AlphaBias; }

    void SetIntensityScale(vd::f32 v);
    vd::f32 GetIntensityScale(void) const         { return m_IntensityScale; }

    void SetIntensityBias(vd::f32 v);
    vd::f32 GetIntensityBias(void) const         { return m_IntensityBias; }

    void SetCameraFov(vd::f32 fFov);
    vd::f32 GetCameraFov(void) const  			  { return m_CameraFov; }

    void SetMotionSpeed(vd::f32 v);
    vd::f32 GetMotionSpeed(void) const            { return m_MotionSpeed; }

    void SetMotionTime(vd::f32 v);
    vd::f32 GetMotionTime(void) const            { return m_MotionTime; }

    void SetScreenSize(vd::u32 w, vd::u32 h);
    void SetUseCustomProjection(bool v, bool ortho);
    void SetCameraPosition(vd::v3f32 v);
    void SetCameraRotation(vd::v3f32 v);
    void SetLightPosition(vd::v3f32 v);
    void SetMotionTransform(vd::m4f32 v);
    void SetModelView(vd::m4f32 v);
    void SetProjection(vd::m4f32 v);

    void EnableProjection();
    void DisableProjection();
    
    
	VD_DECLARE_OBJECT(MotionPointBasedRenderer);
	
protected:

	VD_DISABLE_COPY_CONSTRUCTORS(MotionPointBasedRenderer);
	
    void DrawPoints();
    
protected:
    static const char* VertexShader;
    static const char* FragmentShader;
    static const char* GeometryShader;

    bool m_IsStale;
	bool m_UseCustomProjection;
    bool m_UseOrthographic;

    vd::u32 m_ParticleCount;

	vd::v3f32 m_LightPosition;
	vd::v3f32 m_CameraPosition;
	vd::v3f32 m_CameraRotation;
    vd::f32 m_CameraFov;
    vd::f32 m_CameraFocalLength;
	
    vd::u32 m_ScreenWidth;
    vd::u32 m_ScreenHeight;

    vd::f32 m_MinPointScale;
	vd::f32 m_MaxPointScale;
    vd::f32 m_PointSize;
    vd::f32 m_ParticleRadius;
	vd::f32 m_SmoothingRadius;
    vd::f32 m_SmoothingScale;
    vd::f32 m_DensityScale;
    vd::f32 m_ExposureScale;
    vd::f32 m_IntensityScale;
    vd::f32 m_IntensityBias;
    vd::f32 m_AlphaScale;
    vd::f32 m_AlphaBias;
	vd::f32 m_WdC;
    vd::f32 m_BoxSize;

    vd::u32 m_VertexBufferId;
    vd::i32 m_VertexComponents;
    vd::v2f32 m_VertexRange;

    vd::u32 m_ColorBufferId;
    vd::i32 m_ColorComponents;
    vd::v2f32 m_ColorRange;

    vd::u32 m_DensityBufferId;
    vd::i32 m_DensityComponents;
    vd::v2f32 m_DensityRange;
    
    vd::f32 m_MotionSpeed;
    vd::f32 m_MotionTime;
    vd::m4f32 m_MotionTransform;

    vd::m4f32 m_ModelView;
    vd::m4f32 m_Projection;

    Graphics::Shader m_Shader;
};

// ============================================================================================== //

typedef MotionPointBasedRenderer MPBR;

// ============================================================================================== //

VD_RENDER_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_RENDER_MBPBR_HEADER_INCLUDED
