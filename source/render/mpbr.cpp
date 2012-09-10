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

#include "render/mpbr.h"
#include "graphics/graphics.h"
#include "graphics/gl/opengl.h"

#include <cmath>
#include <cstdio>
#include <memory.h>

#if 0
    
    // Blue values are -7 -> -10.
    // Green are -10. - > -12.
    // Red are -12. - > -14.
    if(vc < 7.0)
    {
        base = vec4(1.0, 0.0, 0.0, 0.0);
        alpha = 1.0;
        luma = 0.0;
    }
    else if(vc >= 7.0 && vc < 10.0 )
    { 
        float db = (abs(vc) - 7.0) / 3.0;
//        base = mix(dark, blue,  vec4(db, db, db, db));
        base = blue;
    }
    else if(vc >= 10.0 && vc < 12.0 )
    {  
        float bg = (abs(vc) - 10.0) / 2.0;
//        base = mix(blue, green, vec4(bg, bg, bg, bg));
        base = green;
    }
    else if(vc >= 12.0 && vc < 14.0 )
    {  
        float gp = (abs(vc) - 12.0) / 2.0;
//        base = mix(green, pink, vec4(gp, gp, gp, gp));
        base = pink;
    }
    if(vc      >= 14.0 && vc <= 18.0 )
    {
        float pr = (abs(vc) - 14.0) / 3.0;
//        base = mix(pink, red, vec4(pr, pr, pr, pr));
        base = red;
    }
    else
    {
        base = vec4(0.0, 1.0, 0.0, 0.0);
        alpha = 1.0;
        luma = 0.0;
    }
#endif

// ============================================================================================== //

VD_RENDER_NAMESPACE_BEGIN();

// ============================================================================================== //

const char* 
MotionPointBasedRenderer::VertexShader = VD_PP_STRINGIZE(

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
uniform mat4 MotionTransform;
uniform vec3 CameraPosition;  

uniform vec2 ColorRange;  
uniform vec2 DensityRange;  

varying vec2 vModelPosition;
varying vec3 vEye;
varying vec3 vIncident;
varying vec3 vMotionDir;
varying vec2 vMotionSize;
varying vec4 vMotionPos;
varying vec4 vVertex;
varying vec4 vPosition;
varying float vPointSize;
varying float vMaxPointSize;
varying float vMinPointSize;
varying float vViewDistance;
varying float vDistance;
varying float vDensity;
varying vec4 vColor;

const float MPBR_F32_HUGE = 987654321.0;
const float MPBR_F32_EPSILON = 0.000000001;

float LinearRemap(float x, 
    float a, float b,
    float c, float d)
{
    float xp = ((d - c) * (x - a) / (b - a)) + c;
    return xp;
}
    

float 
Log10(float x)
{
    return log(x)/log(10.0);
}

vec4 project(vec4 vertex)
{
    vec4 result = gl_ModelViewProjectionMatrix * vertex;
    result /= result.w;
    return result;
}

vec2 screen_space(vec4 vertex)
{
    return (clamp(vertex.xy, -2.0, 2.0) + 1.0) * (ScreenSize * 0.5);
}

void main()
{
    float Epsilon = 0.001; // 1.0 / length(ScreenSize);

    mat4 ModelView = gl_ModelViewMatrix;

    vec4 move = vec4(MotionTransform * gl_Vertex);
    vec4 vertex = vec4(gl_Vertex);// - vec4(gl_ModelViewMatrix * gl_Vertex);
    vec3 EyePosition = vec3(ModelView * vec4(gl_Vertex.xyz, 1.0));
    vec4 VertexPosition = vec4(ModelView * gl_Vertex);
    vec4 MotionPosition = vec4(MotionTransform * gl_Vertex);

    // z = log(z / zn) / log(zf / zn)
    vec3 Extents = vec3(ModelView * vec4(BoxSize, BoxSize, BoxSize, 1.0));
    float minr = 0.001 * MinPointScale * ScreenSize.y / FieldOfView;
    float maxr = 0.001 * MaxPointScale * ScreenSize.y / FieldOfView;

    float maxd = length(Extents);
    float dist = length(CameraPosition - VertexPosition.xyz); // / maxd;
    float pdensity = LinearRemap(Log10(ParticleDensity), Log10(DensityRange.x), Log10(DensityRange.y), 0.0, DensityScale);
    float pcolor = ParticleColor; 
    float alpha = 0.0;
    float luma = 0.0;
    
    vec4 base  = vec4(luma, luma, luma, alpha);
    vec4 red   = vec4( 120.0 / 255.0,   60.0 / 255.0,   75.0 / 255.0, 1.0 );
    vec4 pink  = vec4( 150.0 / 255.0,  120.0 / 255.0,  150.0 / 255.0, 1.0 );
    vec4 green = vec4(  23.0 / 255.0,  100.0 / 255.0,   66.0 / 255.0, 1.0 );
    vec4 blue  = vec4( 105.0 / 255.0,  110.0 / 255.0,  150.0 / 255.0, 1.0 ); 
    vec4 dark  = vec4(  32.0 / 255.0,   50.0 / 255.0,   70.0 / 255.0, 1.0 );
    vec4 black = vec4(           0.0,            0.0,            0.0, 0.0 );

   // Blue values are -7 -> -10.
    // Green are -10. - > -12.
    // Red are -12. - > -14.

    if(pcolor < 7.0)
    {
        base = black;
//        alpha = 0.0;
//        VertexPosition = vec4(MPBR_F32_HUGE);
//        MotionPosition = vec4(MPBR_F32_HUGE - MPBR_F32_EPSILON);
    }
    else if (pcolor < 10.0)
    {
        base = mix(dark, blue, (pcolor -  7.0) / 3.0);
    }
    else if (pcolor < 12.0)
    {
        base = mix(blue, green, (pcolor - 10.0) / 2.0);
    }
    else if (pcolor < 14.0) 
    {
        base = mix(green, pink, (pcolor - 12.0) / 2.0);
    }
    else if (pcolor <= 18.0)
    {
        base = mix(pink, red, (pcolor - 14.0) / 4.0);
    }
    else if (pcolor > 18.0)
    {
//        alpha = 0.0;
        base = black;
//        VertexPosition = vec4(MPBR_F32_HUGE);
//        MotionPosition = vec4(MPBR_F32_HUGE - MPBR_F32_EPSILON);
    }
    
    vec3 p0 = gl_Vertex.xyz;
    vec3 p1 = vec3(MotionTransform * gl_Vertex);
    vec3 p2 = vec3(ModelView * gl_Vertex);
    p1 = p0 + normalize(p2 - p1) * MotionSpeed;
    vec3 p = mix(p0, p1, MotionTime);

    float angle = asin(1.0 / dist);
    float pradius = 0.001 * ParticleRadius * angle * ScreenSize.y / FieldOfView;
    float w = pradius * 2.0;
    float h = w * 0.5;
    
    vec3 u = vec3(ModelView * vec4(p2 - p1, 1.0));
    
    vec2 s0 = screen_space(project(vec4(p0.xyz, 1.0)));
    vec2 s1 = screen_space(project(vec4(p1.xyz, 1.0)));

    // Compute screen-space velocity:
    float t = 0.0;
    float nz = abs(normalize(u).z);
    if (nz > 1.0 - Epsilon)
        t = (nz - (1.0 - Epsilon)) / Epsilon;
    else if (dot(u,u) < Epsilon)
        t = (Epsilon - dot(u,u)) / Epsilon;

    // Lerp the orientation vector if screen-space velocity is negligible:
    u.z = 0.0;
    u = normalize(u);
    u = normalize(mix(u, vec3(1.0, 0.0, 0.0), t));
    h = mix(h, w, t);

    float pscale = (h * ScreenSize.y * pdensity) / dist * dist;
    pscale = pscale > maxr ? maxr : pscale;
    pscale = pscale < minr ? minr : pscale;

    vec2 s1s0 = (s1 - s0);

    vMotionDir = u.xyz;
    vMotionSize = vec2(w, h);
    vMotionPos = project(vec4(p.xyz, 1.0));// vec4(u.xyz, 1.0); // project(vec4(p.xyz, 1.0)); // (vec4(p.xyz+N+W,1.0)) - (vec4(p.xyz+S+E,1.0));

    vVertex = gl_Vertex;
    vPosition = vMotionPos;
    vViewDistance = maxd / length(CameraPosition.xyz - EyePosition.xyz) + 0.000001;
    vDistance = (pscale / dist / maxd);
    vPointSize = max(pscale, 3.0);
    vMinPointSize = minr;
    vMaxPointSize = maxr;
    vIncident = normalize(-vec3(VertexPosition.xyz));
    vDensity = pdensity; // ParticleDensity; // (LinearRemap(ParticleDensity, DensityRange.x, DensityRange.y, 0.0, 1.0));
    vColor = base; // ParticleColor;
    vEye = EyePosition;
    vModelPosition = VertexPosition.xy;

    gl_PointSize = vPointSize;
    gl_Position = vPosition;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_FrontColor = gl_BackColor = base;
}

);

// ============================================================================================== //

const char* 
MotionPointBasedRenderer::GeometryShader = VD_PP_STRINGIZE(

// -- Geometry Shader

// layout(points) in;
// layout(triangle_strip, max_vertices = 24) out;

varying in vec4 vVertex[1];
varying out vec2 gCoord;

uniform float ParticleRadius;
uniform float MotionTime;
uniform float MotionSpeed;
uniform mat4 MotionTransform;
uniform mat4 Projection;
uniform mat4 ModelView;

float Epsilon = 0.001;

void main()
{
    vec3 p0 = vVertex[0].xyz;
    vec3 p1 = vec3(MotionTransform * vVertex[0]);
    p1 = p0 + normalize(p1 - p0) * MotionSpeed;
    vec3 p = mix(p0, p1, MotionTime);

    float w = ParticleRadius * 0.5;
    float h = w * 2.0;
    
    vec3 u = vec3(ModelView * vec4(p1 - p0, 1.0));

    // Determine 't', which represents Z-aligned magnitude.
    // By default, t = 0.0.
    // If velocity aligns with Z, increase t towards 1.0.
    // If total speed is negligible, increase t towards 1.0.
    float t = 0.0;
    float nz = abs(normalize(u).z);
    if (nz > 1.0 - Epsilon)
        t = (nz - (1.0 - Epsilon)) / Epsilon;
    else if (dot(u,u) < Epsilon)
        t = (Epsilon - dot(u,u)) / Epsilon;

    // Compute screen-space velocity:
    u.z = 0.0;
    u = normalize(u);

    // Lerp the orientation vector if screen-space velocity is negligible:
    u = normalize(mix(u, vec4(1.0,0.0,0.0,1.0), t));
    h = mix(h, w, t);

    // Compute the change-of-basis matrix for the billboard:
    vec3 v = vec3(-u.y, u.x, 0.0);
    vec3 a = vec3(vec4(u.xyz, 1.0) * ModelView);
    vec3 b = vec3(vec4(v.xyz, 1.0) * ModelView);
    vec3 c = cross(a, b);
    mat3 basis = mat3(a, b, c);

    // Compute the four offset vectors:
    vec3 N = basis * vec3(0.0,w,0.0);
    vec3 S = basis * vec3(0.0,-w,0.0);
    vec3 E = basis * vec3(-h,0.0,0.0);
    vec3 W = basis * vec3(h,0.0,0.0);

    // Emit the quad:
    mat4 mvp = ModelView * Projection;
    gCoord = vec2(+1.0,+1.0); gl_Position = mvp * vec4(p+N+E,1.0); EmitVertex();
    gCoord = vec2(-1.0,+1.0); gl_Position = mvp * vec4(p+N+W,1.0); EmitVertex();
    gCoord = vec2(+1.0,-1.0); gl_Position = mvp * vec4(p+S+E,1.0); EmitVertex();
    gCoord = vec2(-1.0,-1.0); gl_Position = mvp * vec4(p+S+W,1.0); EmitVertex();
    EndPrimitive();
}

);

// ============================================================================================== //

const char* 
MotionPointBasedRenderer::FragmentShader = VD_PP_STRINGIZE(

varying vec2 vModelPosition;
varying float vDensity;
varying vec4 vColor;
varying float vDistance;
varying float vPointSize;
varying float vMinPointSize;
varying float vMaxPointSize;
varying float vViewDistance;
varying vec3 vIncident;
varying vec3 vEye;
varying vec3 vMotionDir;
varying vec2 vMotionSize;
varying vec4 vMotionPos;

uniform float BoxSize;
uniform float SmoothingRadius;
uniform float SmoothingScale;
uniform float DensityScale;
uniform float MotionSpeed;
uniform float ExposureScale;
uniform float IntensityScale;
uniform float IntensityBias;
uniform float AlphaScale;
uniform float AlphaBias;
uniform float WdC;

float fast_exp(float value)
{
    float result = 0.0000006906;
    result *= value;
    result += 0.0000054302;
    result *= value;
    result += 0.0001715620;
    result *= value;
    result += 0.0025913712;
    result *= value;
    result += 0.0312575832;
    result *= value;
    result += 0.2499986842;
    result *= value;
    result += 1.0;
    result *= result;
    result *= result;
    result = (1.0)/result;
    return result;
}


float gabor(float K, float a, float b, float f0, float omega_0, float dx, float dy, float size)
{
    float x = dx * size;
    float y = dy * size;
    float PI = 3.1415926535897932384626433832795;
    float a2 = a * a;
    float b2 = b * b;
    float gamma = size / 5.36;
    float g2 = gamma * gamma;
    float hw = size / 2.0;    
    float F0 = f0; // 0.0625;
    float rx = (x * cos(omega_0)) + (y * sin(omega_0));
    float ry = (-y * sin(omega_0)) + (y * cos(omega_0));
    K = K / ( sqrt( 2.0 * g2 * PI ) );
    float envelope = K * exp(-PI * ((a2 * (rx * rx)) + (b2 * (ry * ry))) / (2.0 * g2) );
    float carrier = cos(2.0 * PI * F0 * (rx));
    return envelope * carrier;
}

float GaborKernel(float a, float x, float y, float angle, float size)
{
//    float k = 1.0;
//    float a = 0.05;
//    float f0 = 0.0625;
//    float o0 = angle;//PI / 4.0;
//    float sz = sqrt(-log(0.05) / PI) / a;
//    return gabor(k, a, f0, o0, x * sz, y * sz);

    float frequency = 0.0625;
    float PI = 3.1415926535897932384626433832795;
    float gamma = size / 5.36;
    float g2 = gamma * gamma;
    float hw = size / 2.0;
    float xc = x * size;
    float yc = y * size;    
    float cost = cos( angle );
    float sint = sin( angle );
    float gaussian = (exp(-(xc * xc + yc * yc) / (2.0 * g2)) / ( sqrt( 2.0 * g2 * PI ) ));
    float sinusoid = (cos( 2.0 * PI * frequency * ( xc * cost + yc * sint ) ) );
    return gaussian * sinusoid;
}

float LinearRemap(float x, 
    float a, float b,
    float c, float d)
{
    float xp = ((d - c) * (x - a) / (b - a)) + c;
    return xp;
}
  
float 
Log10(float x)
{
    return log(x)/log(10.0);
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

float bias(float b, float x)
{
    return pow(x, log(b)/log(0.5));
}

float gain(float g, float x)
{
    if(x < 0.5)
        return bias(1.0-g,2.0*x)/2.0;
    else
        return 1.0 - bias(1.0-g,2.0 - 2.0*x)/2.0;
}

float Atan2(float x, float y)
{
    if (x != 0.0 || y != 0.0)
    {
        return atan(y, x);
    }
    else
    {
        return 0.0;
    }    
}


float ACos (float value)
{
    if (-1.0 < value)
    {
        if (value < 1.0)
        {
            return acos(value);
        }
        else
        {
            return 0.0;
        }
    }
    else
    {
        return 3.1415926535897932384626433832795;
    }
}

void main()
{
    vec4 base = vColor;

    float luma = 1.0;
    
    vec3 normal;
    normal.xy = (gl_TexCoord[0].xy) * vec2(2.0, -2.0) + vec2(-1.0, 1.0);

    float h2 = SmoothingRadius * SmoothingRadius;
    float h = dot(normal.xy, normal.xy);
    float d = clamp(h * SmoothingScale, 0.0, h2);
//    float a = Poly6Kernel(d, SmoothingRadius) * (vDensity);
    // float a = exp(-d) * Log10(vDensity);

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

    float a = NormalizationConstant * exp(r2 * InverseVariance);
    float angle = Atan2(vMotionDir.y, vMotionDir.x);
    float mi = gabor(a, vMotionSize.x, vMotionSize.y, d, angle, normal.x, normal.y, 1.0 / ExposureScale);

    float alpha = clamp((a / (ips * ips)) * AlphaScale + AlphaBias, 0.0, 1.0);
    base *= (mi * IntensityScale); // / (ips * ips);
    vec4 color = vec4(base.rgb, alpha);

    gl_FragColor = color;
    gl_FragDepth = vDistance;
}

);

// ============================================================================================== //

static float 
CWd(float fH)
{
    return (315.0 / (64.0 * 3.1415926535897932384626433832795 * fH * fH * fH));
}

#if 0
static float
GWd(float fH)
{
    return std::sqrt(-std::log(0.05) / 3.1415926535897932384626433832795) / fH;
}
#endif

MotionPointBasedRenderer::MotionPointBasedRenderer() : 
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

MotionPointBasedRenderer::~MotionPointBasedRenderer()
{
	Destroy();
}

vd::status
MotionPointBasedRenderer::Destroy()
{
    vd::status result = Status::Code::Success;
	if(m_Shader.GetShaderId())
	{
		result = m_Shader.Destroy();
	}
    return result;
}

void MotionPointBasedRenderer::SetVertexRange(
    vd::v3f32 minval, vd::v3f32 maxval)
{
}

void MotionPointBasedRenderer::SetMinPointScale(
    vd::f32 scale)            
{ 
    m_MinPointScale = scale;
    m_MinPointScale = m_MinPointScale > m_MaxPointScale ? m_MaxPointScale - 1.0f : m_MinPointScale;
    m_MinPointScale = m_MinPointScale < 0.0 ? 0.0 : m_MinPointScale;
    // vd::f32 ps = m_ScreenHeight * m_CameraFocalLength;
    m_Shader.SetUniform(vd_sym(MinPointScale), m_MinPointScale );
}

void MotionPointBasedRenderer::SetMaxPointScale(
    vd::f32 scale)            
{ 
	m_MaxPointScale = scale;
    m_MaxPointScale = m_MaxPointScale < m_MinPointScale ? m_MinPointScale + 1.0f : m_MaxPointScale;
    m_MaxPointScale = m_MaxPointScale < 0.0 ? 0.0 : m_MaxPointScale;
    // vd::f32 ps = m_ScreenHeight * m_CameraFocalLength;
	m_Shader.SetUniform(vd_sym(MaxPointScale), m_MaxPointScale );
}

void MotionPointBasedRenderer::SetPointSize(
    vd::f32 v)          	 	  
{ 
	m_PointSize = v;
	SetSmoothingRadius(v); 
}

void MotionPointBasedRenderer::SetBoxSize(
    vd::f32 v)            
{ 
    m_BoxSize = v; 
    m_Shader.SetUniform(vd_sym(BoxSize), m_BoxSize);  
}

void MotionPointBasedRenderer::SetSmoothingScale(
    vd::f32 v)			  
{ 
	m_SmoothingScale = v; 
	m_Shader.SetUniform(vd_sym(SmoothingScale), m_SmoothingScale);	
}

void MotionPointBasedRenderer::SetDensityScale(
    vd::f32 size)          	  
{ 
	m_DensityScale = size; 
	m_Shader.SetUniform(vd_sym(DensityScale), m_DensityScale);	
}

void MotionPointBasedRenderer::SetDensityRange(
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

void MotionPointBasedRenderer::SetColorRange(
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

void MotionPointBasedRenderer::SetExposureScale(
    vd::f32 size)             
{ 
    m_ExposureScale = size; 
    m_Shader.SetUniform(vd_sym(ExposureScale), m_ExposureScale);  
}

void MotionPointBasedRenderer::SetIntensityScale(
    vd::f32 size)             
{ 
    m_IntensityScale = size; 
    m_Shader.SetUniform(vd_sym(IntensityScale), m_IntensityScale);  
}

void MotionPointBasedRenderer::SetIntensityBias(
    vd::f32 size)             
{ 
    m_IntensityBias = size; 
    m_Shader.SetUniform(vd_sym(IntensityBias), m_IntensityBias);  
}

void MotionPointBasedRenderer::SetAlphaScale(
    vd::f32 size)             
{ 
    m_AlphaScale = size; 
    m_Shader.SetUniform(vd_sym(AlphaScale), m_AlphaScale);  
}

void MotionPointBasedRenderer::SetAlphaBias(
    vd::f32 size)             
{ 
    m_AlphaBias = size; 
    m_Shader.SetUniform(vd_sym(AlphaBias), m_AlphaBias);  
}

void MotionPointBasedRenderer::SetParticleRadius(
    vd::f32 r)         	  
{ 
	m_ParticleRadius = r; 
    // vd::f32 ps = m_ScreenHeight * m_CameraFocalLength;
    m_Shader.SetUniform(vd_sym(ParticleRadius), m_ParticleRadius);
}

void MotionPointBasedRenderer::SetCameraFov(
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

void MotionPointBasedRenderer::SetScreenSize(
    vd::u32 w, vd::u32 h)      
{ 
    m_ScreenWidth = w; m_ScreenHeight = h; 
    SetMaxPointScale(m_MaxPointScale);
    SetMinPointScale(m_MinPointScale);
    SetParticleRadius(m_ParticleRadius);
    m_Shader.SetUniform(vd_sym(ScreenSize), vd::v2f32(w, h));
}

void MotionPointBasedRenderer::SetUseCustomProjection(
    bool v, bool ortho)			  
{ 
    m_UseCustomProjection = v; 
    m_UseOrthographic = ortho;
}

void MotionPointBasedRenderer::SetCameraPosition(
    vd::v3f32 v)	  
{ 
    m_CameraPosition = v; 
    m_Shader.SetUniform(vd_sym(CameraPosition), m_CameraPosition );
}

void MotionPointBasedRenderer::SetMotionSpeed(vd::f32 v)      
{ 
    m_MotionSpeed = v; 
    m_Shader.SetUniform(vd_sym(MotionSpeed), m_MotionSpeed);
}

void MotionPointBasedRenderer::SetMotionTime(vd::f32 v)      
{ 
    m_MotionTime = v; 
    m_Shader.SetUniform(vd_sym(MotionTime), m_MotionTime);
}

void MotionPointBasedRenderer::SetMotionTransform(
    vd::m4f32 v)      
{ 
    m_MotionTransform = v; 
    m_Shader.SetUniform(vd_sym(MotionTransform), m_MotionTransform);
}

void MotionPointBasedRenderer::SetModelView(
    vd::m4f32 v)      
{ 
    m_ModelView = v; 
    m_Shader.SetUniform(vd_sym(ModelView), m_ModelView);
}

void MotionPointBasedRenderer::SetProjection(
    vd::m4f32 v)      
{ 
    m_Projection = v; 
    m_Shader.SetUniform(vd_sym(Projection), m_Projection);
}

void MotionPointBasedRenderer::SetCameraRotation(
    vd::v3f32 v)	  
{   
    m_CameraRotation = v; 
}

void MotionPointBasedRenderer::SetLightPosition(
    vd::v3f32 v)	  
{ 
    m_LightPosition = v; 
}

void MotionPointBasedRenderer::SetSmoothingRadius(
	vd::f32 v)
{
	m_SmoothingRadius = v;
	m_WdC = CWd(m_SmoothingRadius * m_SmoothingRadius);
	m_Shader.SetUniform(vd_sym(SmoothingRadius), m_SmoothingRadius);
	m_Shader.SetUniform(vd_sym(WdC), m_WdC);
}

void MotionPointBasedRenderer::SetVertexBuffer(
	vd::u32 uiBufferId, int uiParticleCount, int uiVectorSize)
{
    m_VertexBufferId = uiBufferId;
    m_ParticleCount = uiParticleCount;
    m_VertexComponents = uiVectorSize;
}

void MotionPointBasedRenderer::SetColorBuffer(
    vd::u32 uiBufferId, int uiParticleCount, int uiVectorSize)
{
    m_ColorBufferId = uiBufferId;
    m_ColorComponents = uiVectorSize;
}

void MotionPointBasedRenderer::SetDensityBuffer(
    vd::u32 uiBufferId, int uiParticleCount, int uiVectorSize)
{
    m_DensityBufferId = uiBufferId;
    m_DensityComponents = uiVectorSize;
}

void MotionPointBasedRenderer::DrawPoints()
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

void MotionPointBasedRenderer::Render(
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
MotionPointBasedRenderer::EnableProjection()
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

void MotionPointBasedRenderer::DisableProjection()
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
MotionPointBasedRenderer::Setup(
    Graphics::Context* gfx)
{
    m_Shader.SetName("MBPBR");
    m_Shader.Compile(VertexShader, NULL, FragmentShader);
	m_WdC = CWd(m_SmoothingRadius*m_SmoothingRadius);
    SetCameraFov(m_CameraFov);
    // vd::f32 ps = m_ScreenHeight * m_CameraFocalLength;
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

VD_IMPLEMENT_OBJECT(MotionPointBasedRenderer, vd_sym(MotionPointBasedRenderer), vd_sym(Object));

// ============================================================================================== //

VD_RENDER_NAMESPACE_END();

// ============================================================================================== //


