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

#include "render/sphpbr.h"
#include "graphics/graphics.h"
#include "graphics/gl/opengl.h"
#if defined(VD_USE_GLUT)
#include "graphics/gl/glut.h"
#endif

#include <cmath>
#include <cstdio>
#include <memory.h>

// ============================================================================================== //

VD_RENDER_NAMESPACE_BEGIN();

// ============================================================================================== //

const char* 
SphPointBasedRenderer::VertexShader = VD_PP_STRINGIZE(

attribute float ParticleDensity;
attribute float ParticleColor;

uniform float ParticleRadius;  
uniform float SmoothingScale;
uniform float MaxPointScale;   
uniform float MinPointScale;   
uniform float BoxSize;
uniform float DensityScale;
uniform float MotionTime;
uniform float MotionSpeed;
uniform float FieldOfView;
uniform vec2 ScreenSize;
uniform vec3 CameraPosition;  

uniform vec2 ColorRange;  
uniform vec2 DensityRange;  

varying vec2 vModelPosition;
varying vec3 vEye;
varying vec3 vIncident;
varying vec4 vVertex;
varying vec4 vPosition;
varying float vPointSize;
varying float vMaxPointSize;
varying float vMinPointSize;
varying float vViewDistance;
varying float vDistance;
varying float vDensity;
varying vec4 vColor;

float 
Log10(float x)
{
    return log(x)/log(10.0);
}

float LinearRemap(float x, 
    float a, float b,
    float c, float d)
{
    float xp = ((d - c) * (x - a) / (b - a)) + c;
    return xp;
}
    
void main()
{
    float Epsilon = 0.001; // 1.0 / length(ScreenSize);
    vec4 base = vec4( 105.0 / 255.0,  110.0 / 255.0,  150.0 / 255.0, 0.0 ); 

    mat4 ModelView = gl_ModelViewMatrix;
    vec4 vertex = vec4(gl_Vertex);// - vec4(gl_ModelViewMatrix * gl_Vertex);
    vec3 EyePosition = vec3(ModelView * vec4(gl_Vertex.xyz, 1.0));
    vec4 VertexPosition = vec4(ModelView * gl_Vertex);

    // z = log(z / zn) / log(zf / zn)
    vec3 Extents = vec3(ModelView * vec4(BoxSize, BoxSize, BoxSize, 1.0));
    float minr = 0.001 * MinPointScale * ScreenSize.y / FieldOfView;
    float maxr = 0.001 * MaxPointScale * ScreenSize.y / FieldOfView;

    float maxd = length(Extents);
    float dist = clamp(length(CameraPosition - VertexPosition.xyz), 0.0, maxd) / maxd;
    float pdensity = LinearRemap((ParticleDensity), (DensityRange.x), (DensityRange.y), 0.0, DensityScale);
    float angle = asin(dist);
    float pradius = 0.001 * ParticleRadius * angle * ScreenSize.y / FieldOfView;
    float pscale = (pradius * ScreenSize.y * pdensity) / dist * dist;
    pscale = pscale > maxr ? maxr : pscale;
    pscale = pscale < minr ? minr : pscale;

    vVertex = gl_Vertex;
    vPosition = VertexPosition;
    vViewDistance = maxd / length(CameraPosition.xyz - EyePosition.xyz) + 0.000001;
    vDistance = (pscale / dist / maxd);
    vPointSize = max(pscale, 3.0);
    vMinPointSize = minr;
    vMaxPointSize = maxr;
    vIncident = normalize(-vec3(VertexPosition.xyz));
    vDensity = pdensity; // ParticleDensity; // (LinearRemap(ParticleDensity, DensityRange.x, DensityRange.y, 0.0, 1.0));
    vColor = vec4(ParticleColor, ParticleColor, ParticleColor, 0.0);
    vEye = EyePosition;
    vModelPosition = VertexPosition.xy;

    gl_PointSize = vPointSize;
    gl_Position = ftransform();
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_FrontColor = gl_BackColor = base;
}

);

// ============================================================================================== //

const char* 
SphPointBasedRenderer::FragmentShader = VD_PP_STRINGIZE(

varying vec2 vModelPosition;
varying vec3 vEye;
varying vec3 vIncident;
varying vec4 vVertex;
varying vec4 vPosition;
varying float vPointSize;
varying float vMaxPointSize;
varying float vMinPointSize;
varying float vViewDistance;
varying float vDistance;
varying float vDensity;
varying vec4 vColor;

uniform float SmoothingRadius;
uniform float SmoothingScale;
uniform float DensityScale;
uniform float ExposureScale;
uniform float IntensityScale;
uniform float AlphaScale;
uniform float AlphaBias;
uniform float WdC;

float 
Log10(float x)
{
    return log(x)/log(10.0);
}

float 
DensityKernel(float r, float h)
{
    float value = 0.0;
    float q = r / h;
    float a = 1.0 / 3.1415926535897932384626433832795 / (h * h * h);
    if (q < 1.0)
    {
        value = a * (1.0 - 1.5 * q * q + 0.75 * q * q * q);
    }
    else if (q < 2.0)
    {
        value = a * 0.25 * (2.0 - q) * (2.0 - q) * (2.0 - q);
    }
    return ( value );
}

float 
Poly6Kernel(float r, float h)
{
    float w = 0.0;
    if ( 0.0 <= r && r <= h )
    {
        float h2 = h * h;
        float r2 = r * r;
        float h2r2 = h2 - r2;

        float fa = (h2r2 * h2r2 * h2r2);

        float fn = 315.0;
        float fd = 64.0 * 3.1415926535897932384626433832795 * h * h * h * h * h * h * h * h * h;

        w = (fn / fd) * fa;
    }
    return w;
}

float 
GradientSpikyKernel(float r, float h)
{
    float w = 0.0;
    if ( 0.0 <= r && r <= h )
    {
        float hr = h - r;
        float fa = hr * hr * r;
        float fn = -45.0;
        float fd = 3.1415926535897932384626433832795 * h * h * h * h * h * h;

        w = (fn / fd) * fa;
    }
    return w;
}

float 
CWd(float fH)
{
    return (315.0 / (64.0 * 3.1415926535897932384626433832795 * fH * fH * fH));
}

float 
Wd(float fRH, float fC)
{
    float fA = (1.0 - fRH * fRH);
    return fC*fA*fA*fA;
}

void main()
{
    vec3 normal;
    vec4 base = vec4(gl_Color);
/*
    if(vColor      >= 5.0)        // boundary
        base = vec4(0.001, 0.002, 0.002, 0.0);
    else if(vColor >= 4.0)   // stars
        base = vec4(0.01, 0.01, 0.01, 0.0);
    else if(vColor >= 3.0)        // bulge
        base = vec4(0.00, 0.02, 0.0, 0.0);
    else if(vColor >= 2.0)        // disk
        base = vec4(0.00, 0.0,  0.02, 0.0);
    else if(vColor >= 1.0)        // halo
        base = vec4(0.01, 0.0125, 0.025, 0.0);
    else if(vColor >= 0.0)        // gas
        base = vec4(0.015, 0.01, 0.01, 0.0);
    else
        base = base;
*/
    normal.xy = gl_TexCoord[0].xy*vec2(2.0, -2.0) + vec2(-1.0, 1.0);

    float h2 = SmoothingRadius * SmoothingRadius;
    float h = dot(normal.xy, normal.xy);
    float d = clamp(h * SmoothingScale, 0.0, h2);
    float wd = Wd(d, WdC) * vDensity;
    // float a = Poly6Kernel(d, SmoothingRadius) * (vDensity);

    const float InnerScale = 0.4;
    const float RadiusScale = 0.001;
    const float TwoPi = 2.0 * 3.1415926535897932384626433832795;

    float DoubleVariance = 2.0 * InnerScale * InnerScale;
    float NormalizationConstant = 1.0 / pow(sqrt(TwoPi) * InnerScale, 3.0);
    float InverseVariance = -1.0 / (2.0 * InnerScale * InnerScale);

    float r2 = h;
    float vps = (1.0 / vMaxPointSize);
    float ips = (vPointSize / vMaxPointSize);
    float aps = vPointSize;

    float ma = NormalizationConstant * exp(r2 * InverseVariance) * RadiusScale;
    float mi = (wd / ExposureScale);

    float alpha = clamp((ma / (ips * ips)) * AlphaScale + AlphaBias, 0.0, 1.0);
    base *= (mi * IntensityScale); // / (ips * ips);
    vec4 color = vec4(base.rgb, alpha);

    gl_FragColor = color;
    gl_FragDepth = vDistance;
}

);

// XXX

static float 
CWd(float fH)
{
    return (315.0 / (64.0 * 3.1415926535897932384626433832795 * fH * fH * fH));
}

SphPointBasedRenderer::SphPointBasedRenderer() : 
    Object(),
    m_IsStale(false),
    m_UseCustomProjection(true),
    m_UseOrthographic(false),
    m_ParticleCount(0),
    m_CameraFov(60.0f),
    m_CameraFocalLength(1.0f),
    m_ScreenWidth(1024),
    m_ScreenHeight(1024),
    m_MinPointScale(1.0f),
    m_MaxPointScale(100.0f),
    m_PointSize(1.0f),
    m_ParticleRadius(1.0f),
    m_SmoothingRadius(1.0f),
    m_SmoothingScale(1.0f),
    m_DensityScale(1.0f),
    m_ExposureScale(1.0f),
    m_IntensityScale(1.0f),
    m_IntensityBias(0.0f),
    m_AlphaScale(1.0f),
    m_AlphaBias(0.01f),
    m_WdC(1.0f),
    m_BoxSize(2000.0f),
    m_VertexBufferId(0),
    m_VertexComponents(3),
    m_ColorBufferId(0),
    m_ColorComponents(1),
    m_DensityBufferId(0),
    m_DensityComponents(1),
    m_MotionSpeed(10.0f)    
{

}

SphPointBasedRenderer::~SphPointBasedRenderer()
{
    Destroy();
}

vd::status
SphPointBasedRenderer::Destroy()
{
    vd::status result = Status::Code::Success;
    if(m_Shader.GetShaderId())
    {
        result = m_Shader.Destroy();
    }
    return result;
}

void SphPointBasedRenderer::SetVertexRange(
    vd::v3f32 minval, vd::v3f32 maxval)
{
    m_VertexMinValue = minval;
    m_VertexMaxValue = maxval;

//    m_Shader.SetUniform(vd_sym(VertexRange), m_VertexRange);
}

void SphPointBasedRenderer::SetMinPointScale(
    vd::f32 scale)            
{ 
    m_MinPointScale = scale;
    m_MinPointScale = m_MinPointScale > m_MaxPointScale ? m_MaxPointScale - 1.0f : m_MinPointScale;
    m_MinPointScale = m_MinPointScale < 0.0 ? 0.0 : m_MinPointScale;
//    vd::f32 ps = m_ScreenHeight * m_CameraFocalLength;
    m_Shader.SetUniform(vd_sym(MinPointScale), m_MinPointScale );
    m_Shader.SetUniform(vd_sym(PointScale), m_MinPointScale );
}

void SphPointBasedRenderer::SetMaxPointScale(
    vd::f32 scale)            
{ 
    m_MaxPointScale = scale;
    m_MaxPointScale = m_MaxPointScale < m_MinPointScale ? m_MinPointScale + 1.0f : m_MaxPointScale;
    m_MaxPointScale = m_MaxPointScale < 0.0 ? 0.0 : m_MaxPointScale;
//    vd::f32 ps = m_ScreenHeight * m_CameraFocalLength;
    m_Shader.SetUniform(vd_sym(MaxPointScale), m_MaxPointScale );
}

void SphPointBasedRenderer::SetPointSize(
    vd::f32 v)                    
{ 
    m_PointSize = v;
    SetSmoothingRadius(v); 
}

void SphPointBasedRenderer::SetBoxSize(
    vd::f32 v)            
{ 
    m_BoxSize = v; 
    m_Shader.SetUniform(vd_sym(BoxSize), m_BoxSize);  
}

void SphPointBasedRenderer::SetSmoothingScale(
    vd::f32 v)            
{ 
    m_SmoothingScale = v; 
    m_Shader.SetUniform(vd_sym(SmoothingScale), m_SmoothingScale);  
}

void SphPointBasedRenderer::SetDensityScale(
    vd::f32 size)             
{ 
    m_DensityScale = size; 
    m_Shader.SetUniform(vd_sym(DensityScale), m_DensityScale);  
}

void SphPointBasedRenderer::SetDensityRange(
    vd::f32 minval, vd::f32 maxval)
{
    m_DensityRange = v2f32(minval, maxval);
    if(m_DensityRange[0] == m_DensityRange[1])
    {
       m_DensityRange[0] = 0.0f;
       m_DensityRange[1] = 1.0f;
    }    
    m_Shader.SetUniform(vd_sym(DensityRange), m_DensityRange);
}

void SphPointBasedRenderer::SetColorRange(
    vd::f32 minval, vd::f32 maxval)
{
    m_ColorRange = v2f32(minval, maxval);
    if(m_ColorRange[0] == m_ColorRange[1])
    {
       m_ColorRange[0] = 0.0f;
       m_ColorRange[1] = 1.0f;
    }    
    m_Shader.SetUniform(vd_sym(ColorRange), m_ColorRange);
}

void SphPointBasedRenderer::SetExposureScale(
    vd::f32 size)             
{ 
    m_ExposureScale = size; 
    m_Shader.SetUniform(vd_sym(ExposureScale), m_ExposureScale);  
}

void SphPointBasedRenderer::SetIntensityScale(
    vd::f32 size)             
{ 
    m_IntensityScale = size; 
    m_Shader.SetUniform(vd_sym(IntensityScale), m_IntensityScale);  
}

void SphPointBasedRenderer::SetIntensityBias(
    vd::f32 size)             
{ 
    m_IntensityBias = size; 
    m_Shader.SetUniform(vd_sym(IntensityBias), m_IntensityBias);  
}

void SphPointBasedRenderer::SetAlphaScale(
    vd::f32 size)             
{ 
    m_AlphaScale = size; 
    m_Shader.SetUniform(vd_sym(AlphaScale), m_AlphaScale);  
}

void SphPointBasedRenderer::SetAlphaBias(
    vd::f32 size)             
{ 
    m_AlphaBias = size; 
    m_Shader.SetUniform(vd_sym(AlphaBias), m_AlphaBias);  
}

void SphPointBasedRenderer::SetParticleRadius(
    vd::f32 r)            
{ 
    m_ParticleRadius = r; 
//    vd::f32 ps = m_ScreenHeight * m_CameraFocalLength;
    m_Shader.SetUniform(vd_sym(ParticleRadius), m_ParticleRadius);
}

void SphPointBasedRenderer::SetCameraFov(
    vd::f32 fov)              
{ 
    m_CameraFov = Core::Min(fov, 180.0f); 
    vd::f32 radians = Core::Deg2Rad(m_CameraFov / vd::f32(Constants::Two));
    m_CameraFocalLength = vd::f32(Constants::One) / Core::Tan(radians);
    m_Shader.SetUniform(vd_sym(FieldOfView), radians);
    SetMaxPointScale(m_MaxPointScale);
    SetMinPointScale(m_MinPointScale);
    SetParticleRadius(m_ParticleRadius);
}

void SphPointBasedRenderer::SetScreenSize(
    vd::u32 w, vd::u32 h)      
{ 
    m_ScreenWidth = w; m_ScreenHeight = h; 
    SetMaxPointScale(m_MaxPointScale);
    SetMinPointScale(m_MinPointScale);
    SetParticleRadius(m_ParticleRadius);
    m_Shader.SetUniform(vd_sym(ScreenSize), vd::v2f32(w, h));
}

void SphPointBasedRenderer::SetUseCustomProjection(
    bool v, bool ortho)           
{ 
    m_UseCustomProjection = v; 
    m_UseOrthographic = ortho;
}

void SphPointBasedRenderer::SetCameraPosition(
    vd::v3f32 v)      
{ 
    m_CameraPosition = v; 
    m_Shader.SetUniform(vd_sym(CameraPosition), m_CameraPosition );
}

void SphPointBasedRenderer::SetMotionSpeed(vd::f32 v)      
{ 
    m_MotionSpeed = v; 
    m_Shader.SetUniform(vd_sym(MotionSpeed), m_MotionSpeed);
}

void SphPointBasedRenderer::SetMotionTime(vd::f32 v)      
{ 
    m_MotionTime = v; 
    m_Shader.SetUniform(vd_sym(MotionTime), m_MotionTime);
}

void SphPointBasedRenderer::SetMotionTransform(
    vd::m4f32 v)      
{ 
    m_MotionTransform = v; 
    m_Shader.SetUniform(vd_sym(MotionTransform), m_MotionTransform);
}

void SphPointBasedRenderer::SetModelView(
    vd::m4f32 v)      
{ 
    m_ModelView = v; 
    m_Shader.SetUniform(vd_sym(ModelView), m_ModelView);
}

void SphPointBasedRenderer::SetProjection(
    vd::m4f32 v)      
{ 
    m_Projection = v; 
    m_Shader.SetUniform(vd_sym(Projection), m_Projection);
}

void SphPointBasedRenderer::SetCameraRotation(
    vd::v3f32 v)      
{   
    m_CameraRotation = v; 
}

void SphPointBasedRenderer::SetLightPosition(
    vd::v3f32 v)      
{ 
    m_LightPosition = v; 
}

void SphPointBasedRenderer::SetSmoothingRadius(
    vd::f32 v)
{
    m_SmoothingRadius = v;
    m_WdC = CWd(m_SmoothingRadius * m_SmoothingRadius);
    m_Shader.SetUniform(vd_sym(SmoothingRadius), m_SmoothingRadius);
    m_Shader.SetUniform(vd_sym(WdC), m_WdC);
}

void SphPointBasedRenderer::SetVertexBuffer(
    vd::u32 uiBufferId, int uiParticleCount, int uiVectorSize)
{
    m_VertexBufferId = uiBufferId;
    m_ParticleCount = uiParticleCount;
    m_VertexComponents = uiVectorSize;
}

void SphPointBasedRenderer::SetColorBuffer(
    vd::u32 uiBufferId, int uiParticleCount, int uiVectorSize)
{
    m_ColorBufferId = uiBufferId;
    m_ColorComponents = uiVectorSize;
}

void SphPointBasedRenderer::SetDensityBuffer(
    vd::u32 uiBufferId, int uiParticleCount, int uiVectorSize)
{
    m_DensityBufferId = uiBufferId;
    m_DensityComponents = uiVectorSize;
}

void SphPointBasedRenderer::DrawPoints()
{
    if (m_VertexBufferId < 1)
        return;

    glPushMatrix();
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_VertexBufferId);
    glVertexPointer(m_VertexComponents, GL_FLOAT, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);                

    vd::u32 color_slot = 0;
    if (m_ColorBufferId) 
    {
        color_slot = m_Shader.GetAttributeSlot(vd_sym(ParticleColor));
        if(color_slot < VD_U32_MAX)
        {
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_ColorBufferId);
            glVertexAttribPointer(color_slot, m_ColorComponents, GL_FLOAT, GL_FALSE, m_ColorComponents * sizeof(vd::f32), 0);
            glEnableVertexAttribArray(color_slot);    
        }
        else 
        {
            vdLogWarning("Failed to bind particle color attribute!");
            color_slot = 0;
        }
//      glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_ColorBufferId);
//      glColorPointer(m_ColorComponents, GL_FLOAT, 0, 0);
//      glEnableClientState(GL_COLOR_ARRAY);
    }

    vd::u32 density_slot = 0;
    if (m_DensityBufferId) 
    {
        density_slot = m_Shader.GetAttributeSlot(vd_sym(ParticleDensity));
        if(density_slot < VD_U32_MAX)
        {
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_DensityBufferId);
            glVertexAttribPointer(density_slot, m_DensityComponents, GL_FLOAT, GL_FALSE, m_DensityComponents * sizeof(vd::f32), 0);
            glEnableVertexAttribArray(density_slot);    
        }
        else 
        {
            density_slot = 0;
        }
    }

    glDrawArrays(GL_POINTS, 0, m_ParticleCount);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    glDisableClientState(GL_VERTEX_ARRAY); 

    if(color_slot)
        glDisableVertexAttribArray(color_slot);    
    else
        glDisableClientState(GL_COLOR_ARRAY); 

    if(density_slot)
        glDisableVertexAttribArray(density_slot);    

    glPopMatrix();
}

void SphPointBasedRenderer::Render(
    bool dirty, DisplayMode::Value mode)
{
    if (m_VertexBufferId < 1)
        return;

    EnableProjection();

    switch (mode)
    {
    case DisplayMode::Points:
        glColor3f(1, 1, 1);
        glPointSize(m_PointSize);
        DrawPoints();
        break;

    default:
    case DisplayMode::Splats:
    {
        glDisable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);     
    
        glEnable(GL_POINT_SPRITE_ARB);
        glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);

        if(m_ColorBufferId == 0)
            glColor3f(0.01, 0.02, 0.04);
        else
            glColor3f(1.0, 1.0, 1.0);
        
        m_Shader.Bind(m_IsStale);
        DrawPoints();
        m_Shader.Unbind();
        m_IsStale = false;

        glDisable(GL_TEXTURE_2D);
        glActiveTextureARB(GL_TEXTURE0_ARB);
        glDisable(GL_POINT_SPRITE_ARB);
        glDisable(GL_BLEND);
        break;
    }
    };

    DisableProjection();
}

void
SphPointBasedRenderer::EnableProjection()
{
    if(m_UseCustomProjection)
    {
        GLfloat gp[16];
        GLint mode;
        glGetIntegerv(GL_MATRIX_MODE, &mode);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        vd::f32 aspect = vd::f32(m_ScreenWidth) / vd::f32(m_ScreenHeight);
        vd::f32 zmin = 0.001f;
        vd::f32 zmax = 1000.0f;

        vd::f32 ymax = zmin * Core::Tan(Core::Deg2Rad(m_CameraFov / vd::f32(Constants::Two)));
        vd::f32 ymin = -ymax;
        vd::f32 xmin = ymin * aspect;
        vd::f32 xmax = ymax * aspect;

        vd::f32 left_plane = xmin;
        vd::f32 right_plane = xmax;
        vd::f32 top_plane = ymax;
        vd::f32 bottom_plane = ymin;

        vd::f32 frustum_width = (right_plane - left_plane);
        vd::f32 frustum_height = (top_plane - bottom_plane);
        vd::f32 frustum_near = zmin;
        vd::f32 frustum_far = zmax;

        vd::f32 tile_left = left_plane;
        vd::f32 tile_right = tile_left + (frustum_width);
        vd::f32 tile_bottom = bottom_plane;
        vd::f32 tile_top = tile_bottom + (frustum_height);

        if(m_UseOrthographic)
        {
            glOrtho(
                tile_left   * m_ScreenWidth * 0.5f, 
                tile_right  * m_ScreenWidth * 0.5f, 
                tile_bottom * aspect * m_ScreenHeight * 0.5f, 
                tile_top    * aspect * m_ScreenHeight * 0.5f, 
                frustum_near, frustum_far);
        }
        else
        {
            gluPerspective(m_CameraFov, aspect, zmin, zmax);
        }
        glGetFloatv(GL_PROJECTION_MATRIX, gp);

        vd::m4f32 mp = vd::m4f32(
            gp[ 0], gp[ 1], gp[ 2], gp[ 3],
            gp[ 4], gp[ 5], gp[ 6], gp[ 7],
            gp[ 8], gp[ 9], gp[10], gp[11],
            gp[12], gp[13], gp[14], gp[15]);

        SetProjection(mp);
        glMatrixMode(mode);
    }
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(m_CameraPosition[0], m_CameraPosition[1], m_CameraPosition[2]);
    glRotatef(m_CameraRotation[0], 1.0, 0.0, 0.0);
    glRotatef(m_CameraRotation[1], 0.0, 1.0, 0.0);
    glRotatef(m_CameraRotation[2], 0.0, 0.0, 1.0);

    GLfloat gn[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, gn);

    vd::m4f32 mn(
        gn[ 0], gn[ 1], gn[ 2], gn[ 3],
        gn[ 4], gn[ 5], gn[ 6], gn[ 7],
        gn[ 8], gn[ 9], gn[10], gn[11],
        gn[12], gn[13], gn[14], gn[15]);

    SetModelView(mn);
}

void SphPointBasedRenderer::DisableProjection()
{
    if(m_UseCustomProjection)
    {
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
    }
    
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

vd::status
SphPointBasedRenderer::Setup(
    Graphics::Context* gfx)
{
    m_Shader.SetName("SPH-PBR");
    m_Shader.Compile(VertexShader, NULL, FragmentShader);
    m_WdC = CWd(m_SmoothingRadius*m_SmoothingRadius);
    SetCameraFov(m_CameraFov);
//    vd::f32 ps = m_ScreenHeight * m_CameraFocalLength;
    vd::f32 radians = Core::Deg2Rad(m_CameraFov / vd::f32(Constants::Two));
    m_Shader.SetUniform(vd_sym(CameraPosition), m_CameraPosition );
    m_Shader.SetUniform(vd_sym(MinPointScale), m_MinPointScale );
    m_Shader.SetUniform(vd_sym(MaxPointScale), m_MaxPointScale );
    m_Shader.SetUniform(vd_sym(ParticleRadius), m_ParticleRadius );
    m_Shader.SetUniform(vd_sym(FieldOfView), radians );
    m_Shader.SetUniform(vd_sym(MotionSpeed), m_MotionSpeed);
    m_Shader.SetUniform(vd_sym(MotionTime), m_MotionTime);
    m_Shader.SetUniform(vd_sym(MotionTransform), m_MotionTransform);
    m_Shader.SetUniform(vd_sym(SmoothingRadius), m_SmoothingRadius);
    m_Shader.SetUniform(vd_sym(SmoothingScale), m_SmoothingScale);  
    m_Shader.SetUniform(vd_sym(DensityScale), m_DensityScale);  
    m_Shader.SetUniform(vd_sym(DensityRange), m_DensityRange);
    m_Shader.SetUniform(vd_sym(ColorRange), m_ColorRange);
    m_Shader.SetUniform(vd_sym(ExposureScale), m_ExposureScale);  
    m_Shader.SetUniform(vd_sym(IntensityScale), m_IntensityScale);  
    m_Shader.SetUniform(vd_sym(IntensityBias), m_IntensityBias);  
    m_Shader.SetUniform(vd_sym(AlphaScale), m_AlphaScale);  
    m_Shader.SetUniform(vd_sym(AlphaBias), m_AlphaBias);  
    m_Shader.SetUniform(vd_sym(BoxSize), m_BoxSize);  
    m_Shader.SetUniform(vd_sym(WdC), m_WdC);
    m_IsStale = true;
    return Status::Code::Success;
}


// ============================================================================================== //

VD_IMPLEMENT_OBJECT(SphPointBasedRenderer, vd_sym(SphPointBasedRenderer), vd_sym(Object));

// ============================================================================================== //

VD_RENDER_NAMESPACE_END();

// ============================================================================================== //

#if 0

// XXX
// ============================================================================================== //

static float 
CWd(float fH)
{
    return (315.0 / (64.0 * 3.1415926535897932384626433832795 * fH * fH * fH));
}

SphPointBasedRenderer::SphPointBasedRenderer() : 
	Object(),
    m_IsStale(false),
	m_UseCustomProjection(true),
    m_ParticleCount(0),
    m_CameraFov(60.0f),
    m_CameraFocalLength(1.0f),
    m_ScreenWidth(0),
    m_ScreenHeight(0),
    m_PointScale(1.0f),
    m_PointSize(1.0f),
    m_ParticleRadius(1.0f),
    m_SmoothingRadius(1.0f),
    m_SmoothingScale(1.0f),
    m_DensityScale(1.0f),
    m_ExposureScale(1.0f),
    m_IntensityScale(1.0f),
    m_AlphaScale(1.0f),
    m_WdC(1.0f),
    m_BoxSize(150.0f),
    m_VertexBufferId(0),
    m_VertexComponents(3),
    m_ColorBufferId(0),
    m_ColorComponents(1),
    m_DensityBufferId(0),
    m_DensityComponents(1)
{
    m_CameraDepthRange = vd::v2f32(0.01f, 100.0f);
}

SphPointBasedRenderer::~SphPointBasedRenderer()
{
	Destroy();
}

vd::status
SphPointBasedRenderer::Destroy()
{
	if(m_Shader.GetShaderId())
	{
		m_Shader.Destroy();
	}
    return Status::Code::Success;
}

void SphPointBasedRenderer::SetPointScale(
    vd::f32 scale)            
{ 
	m_PointScale = scale;
    m_CameraFocalLength = (1.0f / Core::Tan(Core::Deg2Rad(m_CameraFov)) * 0.5f);
    vd::f32 ps = m_ScreenHeight * m_CameraFocalLength;
//	vd::f32 ps = m_ScreenHeight / tan(m_CameraFov*0.5f*M_PI/180.0f);
	m_Shader.SetUniform(vd_sym(PointScale), m_PointScale * ps );
}

void SphPointBasedRenderer::SetPointSize(
    vd::f32 v)          	 	  
{ 
	m_PointSize = v;
	SetSmoothingRadius(v); 
}

void SphPointBasedRenderer::SetBoxSize(
    vd::f32 v)            
{ 
    m_BoxSize = v; 
    m_Shader.SetUniform(vd_sym(BoxSize), m_BoxSize);  
}

void SphPointBasedRenderer::SetSmoothingScale(
    vd::f32 v)			  
{ 
	m_SmoothingScale = v; 
	m_Shader.SetUniform(vd_sym(SmoothingScale), m_SmoothingScale);	
}

void SphPointBasedRenderer::SetDensityScale(
    vd::f32 size)          	  
{ 
	m_DensityScale = size; 
	m_Shader.SetUniform(vd_sym(DensityScale), m_DensityScale);	
}

void SphPointBasedRenderer::SetDensityRange(
    vd::f32 minval, vd::f32 maxval)
{
    m_DensityRange = v2f32(minval, maxval);
    if(m_DensityRange[0] == m_DensityRange[1])
    {
       m_DensityRange[0] = 0.0f;
       m_DensityRange[1] = 1.0f;
    }    
    m_Shader.SetUniform(vd_sym(DensityRange), m_DensityRange);
}

void SphPointBasedRenderer::SetColorRange(
    vd::f32 minval, vd::f32 maxval)
{
    m_ColorRange = v2f32(minval, maxval);
    if(m_ColorRange[0] == m_ColorRange[1])
    {
       m_ColorRange[0] = 0.0f;
       m_ColorRange[1] = 1.0f;
    }    
    m_Shader.SetUniform(vd_sym(ColorRange), m_ColorRange);
}

void SphPointBasedRenderer::SetExposureScale(
    vd::f32 size)             
{ 
    m_ExposureScale = size; 
    m_Shader.SetUniform(vd_sym(ExposureScale), m_ExposureScale);  
}

void SphPointBasedRenderer::SetIntensityScale(
    vd::f32 size)             
{ 
    m_IntensityScale = size; 
    m_Shader.SetUniform(vd_sym(IntensityScale), m_IntensityScale);  
}

void SphPointBasedRenderer::SetAlphaScale(
    vd::f32 size)             
{ 
    m_AlphaScale = size; 
    m_Shader.SetUniform(vd_sym(AlphaScale), m_AlphaScale);  
}

void SphPointBasedRenderer::SetParticleRadius(
    vd::f32 r)         	  
{ 
	m_ParticleRadius = r; 
	m_Shader.SetUniform(vd_sym(ParticleRadius), m_ParticleRadius);
}

void SphPointBasedRenderer::SetCameraFov(
    vd::f32 fov)           	  
{ 
	m_CameraFov = fov; 
	m_CameraFocalLength = (1.0f / Core::Tan(Core::Deg2Rad(fov)) * 0.5f);
    SetPointScale(m_PointScale);
}


void SphPointBasedRenderer::SetCameraDepthRange(
    vd::f32 minval, vd::f32 maxval)
{
    m_CameraDepthRange = v2f32(minval, maxval);
}

void SphPointBasedRenderer::SetScreenSize(
    vd::u32 w, vd::u32 h)      
{ 
    m_ScreenWidth = w; m_ScreenHeight = h; 
}

void SphPointBasedRenderer::SetUseCustomProjection(bool v)			  
{ 
    m_UseCustomProjection = v; 
}

void SphPointBasedRenderer::SetCameraPosition(const vd::v3f32& v)	  
{ 
    m_CameraPosition = v; 
}

void SphPointBasedRenderer::SetCameraRotation(const vd::v3f32& v)	  
{   
    m_CameraRotation = v; 
}

void SphPointBasedRenderer::SetLightPosition(const vd::v3f32& v)	  
{ 
    m_LightPosition = v; 
}

void SphPointBasedRenderer::SetSmoothingRadius(
	vd::f32 v)
{
	m_SmoothingRadius = v;
	m_WdC = CWd(m_SmoothingRadius);
	m_Shader.SetUniform(vd_sym(SmoothingRadius), m_SmoothingRadius);
	m_Shader.SetUniform(vd_sym(WdC), m_WdC);
}

void SphPointBasedRenderer::SetVertexBuffer(
	vd::u32 uiBufferId, int uiParticleCount, int uiVectorSize)
{
    m_VertexBufferId = uiBufferId;
    m_ParticleCount = uiParticleCount;
    m_VertexComponents = uiVectorSize;
}

void SphPointBasedRenderer::SetColorBuffer(
    vd::u32 uiBufferId, int uiParticleCount, int uiVectorSize)
{
    m_ColorBufferId = uiBufferId;
    m_ColorComponents = uiVectorSize;
}

void SphPointBasedRenderer::SetDensityBuffer(
    vd::u32 uiBufferId, int uiParticleCount, int uiVectorSize)
{
    m_DensityBufferId = uiBufferId;
    m_DensityComponents = uiVectorSize;
}

void SphPointBasedRenderer::DrawPoints()
{
    if (m_VertexBufferId < 1)
    	return;

	glPushMatrix();
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_VertexBufferId);
	glVertexPointer(m_VertexComponents, GL_FLOAT, 0, 0);
	glEnableClientState(GL_VERTEX_ARRAY);                

    vd::u32 color_slot = 0;
	if (m_ColorBufferId) 
	{
        color_slot = m_Shader.GetAttributeSlot(vd_sym(ParticleColor));
        if(color_slot < VD_U32_MAX)
        {
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_ColorBufferId);
            glVertexAttribPointer(color_slot, m_ColorComponents, GL_FLOAT, GL_FALSE, m_ColorComponents * sizeof(vd::f32), 0);
            glEnableVertexAttribArray(color_slot);    
        }
        else 
        {
            color_slot = 0;
        }
//		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_ColorBufferId);
//		glColorPointer(m_ColorComponents, GL_FLOAT, 0, 0);
//		glEnableClientState(GL_COLOR_ARRAY);
	}

    vd::u32 density_slot = 0;
    if (m_DensityBufferId) 
    {
        density_slot = m_Shader.GetAttributeSlot(vd_sym(ParticleDensity));
        if(density_slot < VD_U32_MAX)
        {
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_DensityBufferId);
            glVertexAttribPointer(density_slot, m_DensityComponents, GL_FLOAT, GL_FALSE, m_DensityComponents * sizeof(vd::f32), 0);
            glEnableVertexAttribArray(density_slot);    
        }
        else 
        {
            density_slot = 0;
        }
    }

	glDrawArrays(GL_POINTS, 0, m_ParticleCount);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glDisableClientState(GL_VERTEX_ARRAY); 

    if(color_slot)
        glDisableVertexAttribArray(color_slot);    
	else
        glDisableClientState(GL_COLOR_ARRAY); 

    if(density_slot)
        glDisableVertexAttribArray(density_slot);    

    glPopMatrix();
}

void
SphPointBasedRenderer::RenderBoundary(
	float x, float y, float z,
	float w, float h, float d)
{
    EnableProjection();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
    glPushMatrix();
    {
        glTranslatef(x, y, z);
        glScalef(w, h, d);
        glColor4f(0.1f, 0.1f, 0.1f, 0.5f);
#if defined(VD_USE_GLUT)
        glutWireCube(1.0f);
#endif        
        glColor4f(0.1f, 0.1f, 0.1f, 0.1f);
#if defined(VD_USE_GLUT)
        glutSolidCube(1.0f);
#endif        
    }
    glPopMatrix();
    glDisable(GL_BLEND);

    DisableProjection();
}

void SphPointBasedRenderer::Render(
	bool dirty, DisplayMode::Value mode)
{
    if (m_VertexBufferId < 1)
    	return;

    EnableProjection();

    switch (mode)
    {
    case DisplayMode::Points:
        glColor3f(1, 1, 1);
        glPointSize(m_PointSize);
        DrawPoints();
        break;

    default:
    case DisplayMode::Splats:
	{
		glDisable(GL_DEPTH_TEST);

        glEnable(GL_POINT_SPRITE_ARB);
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);
	    glEnable(GL_BLEND);
        glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);

//      glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);     
//      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
//      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		if(m_ColorBufferId == 0)
			glColor3f(0.01, 0.02, 0.04);
    	else
    		glColor3f(1.0, 1.0, 1.0);
		
        m_Shader.Bind(m_IsStale);
		DrawPoints();
        m_Shader.Unbind();
        m_IsStale = false;

//        glDisable(GL_DEPTH_TEST);
		glDisable(GL_POINT_SPRITE_ARB);
		glDisable(GL_BLEND);
        break;
	}
    };

    DisableProjection();
}

void
SphPointBasedRenderer::EnableProjection()
{
	if(m_UseCustomProjection)
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPerspective(m_CameraFov, (float) m_ScreenWidth / (float) m_ScreenHeight, 0.001, 1000.0f);
	}
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(m_CameraPosition[0], m_CameraPosition[1], m_CameraPosition[2]);
	glRotatef(m_CameraRotation[0], 1.0, 0.0, 0.0);
	glRotatef(m_CameraRotation[1], 0.0, 1.0, 0.0);
}

void SphPointBasedRenderer::DisableProjection()
{
	if(m_UseCustomProjection)
	{
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}
	
	glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

vd::status
SphPointBasedRenderer::Setup(
    Graphics::Context* gfx)
{
    m_Graphics = gfx;
    m_Shader.SetName("SPBR");
    m_Shader.Compile(SphereVS, NULL, SphereFS);
	m_WdC = CWd(m_SmoothingRadius);
    m_CameraFocalLength = (1.0f / Core::Tan(Core::Deg2Rad(m_CameraFov)) * 0.5f);
	vd::f32 ps = m_ScreenHeight * m_CameraFocalLength;
	m_Shader.SetUniform(vd_sym(PointScale), m_PointScale * ps );
	m_Shader.SetUniform(vd_sym(ParticleRadius), m_ParticleRadius);
	m_Shader.SetUniform(vd_sym(SmoothingRadius), m_SmoothingRadius);
    m_Shader.SetUniform(vd_sym(SmoothingScale), m_SmoothingScale);  
    m_Shader.SetUniform(vd_sym(DensityScale), m_DensityScale);  
    m_Shader.SetUniform(vd_sym(DensityRange), m_DensityRange);
    m_Shader.SetUniform(vd_sym(ColorRange), m_ColorRange);
    m_Shader.SetUniform(vd_sym(ExposureScale), m_ExposureScale);  
    m_Shader.SetUniform(vd_sym(IntensityScale), m_IntensityScale);  
    m_Shader.SetUniform(vd_sym(AlphaScale), m_AlphaScale);  
    m_Shader.SetUniform(vd_sym(BoxSize), m_BoxSize);  
	m_Shader.SetUniform(vd_sym(WdC), m_WdC);
    m_IsStale = true;
    return Status::Code::Success;
}

// ============================================================================================== //

VD_IMPLEMENT_OBJECT(SphPointBasedRenderer, vd_sym(SphPointBasedRenderer), vd_sym(Object));

// ============================================================================================== //

VD_RENDER_NAMESPACE_END();

// ============================================================================================== //

#endif
