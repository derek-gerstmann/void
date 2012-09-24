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

#include "graphics/shader.h"
#include "graphics/geometry.h"
#include "graphics/gl/opengl.h"
#include "constants/values.h"
#include "core/asserts.h"
#include "core/resources.h"
#include "core/collections.h"
#include "core/symbol.h"
#include "core/logging.h"
#include "core/filesystem.h"
#include "core/file.h"

#include <cstring>
#include <cmath>
#include <string>

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_BEGIN();

// ============================================================================================== //

const vd::i32 Shader::InvalidSlot 			 	  = -1;
typedef Core::Vector< vd::symbol >::type 	SymbolList;

// ============================================================================================== //

Shader::Shader(
    Context* ctx) :
    Object(),
    m_Context(ctx)
{
    Core::Memory::SetBytes(&m_Data, 0, sizeof(m_Data));
    m_Data.Index = Constants::InvalidIndex;
}

Shader::Shader(
	Graphics::Context* context,
	const vd::string& name)
:
	Object(), 
	m_Name(name),
    m_Context(context)
{
    Core::Memory::SetBytes(&m_Data, 0, sizeof(m_Data));
    m_Data.Index = Constants::InvalidIndex;
}

Shader::~Shader()
{
    Destroy();
}

vd::status 
Shader::Reset()
{
    Destroy();
    Core::Memory::SetBytes(&m_Data, 0, sizeof(m_Data));
    m_Data.Index = Constants::InvalidIndex;
    return Status::Code::Success;
}

vd::status 
Shader::Acquire()
{
	return Status::Code::Success;
}

vd::status 
Shader::Release()
{
    m_Data.Index = Constants::InvalidIndex;
	return Status::Code::Success;
}

vd::status
Shader::Setup(
    const Shader::Data& data)
{
    if(&m_Data == &data)
        return Status::Code::Success;

    Destroy();
    Core::Memory::CopyBytes(&m_Data, &data, sizeof(m_Data));
    return Status::Code::Success;
}

vd::status 
Shader::Destroy()
{
	if(m_Context)
		m_Context->Release(this);

    Core::Memory::SetBytes(&m_Data, 0, sizeof(m_Data));
    m_Data.Index = Constants::InvalidIndex;
	return Status::Code::Success;
}

const Shader::Data&
Shader::GetData() const
{
    return m_Data;
}

const Shader::Data*
Shader::GetPtr() const
{
    return &m_Data;
}

vd::i32 
Shader::GetShaderId() const 
{ 
	return m_Data.Id; 
}

vd::status
Shader::Bind(bool)
{
    m_Data.Usage++;
    return Status::Code::Success;
}

vd::status
Shader::Unbind()
{
    vdAssert(m_Data.Usage > 0);
    m_Data.Usage--;
    return Status::Code::Success;
}

bool 
Shader::IsActive()
{
    return m_Data.Usage > 0;
}

bool
Shader::IsValidUniform(
	Symbol name)
{
	return m_Uniforms.IsUsed(name);
}

vd::status 
Shader::SetUniform(
	Symbol name, const vd::v2f32& value)
{
	if(m_Uniforms.IsUsed(name))
	{
		m_Uniforms.Set2f(name, value);
		return Status::Code::Success;
	}
#if defined(VD_DEBUG_SHADERS)			
	vdLogInfo("Uniform '%s' not used!", Symbol::ToString(name));
#endif
	return Status::Code::InvalidSlot;
}

vd::status 
Shader::SetUniform(
	Symbol name, const vd::v3f32& value)
{
	if(m_Uniforms.IsUsed(name))
	{
		m_Uniforms.Set3f(name, value);
		return Status::Code::Success;
	}
#if defined(VD_DEBUG_SHADERS)			
	vdLogInfo("Uniform '%s' not used!", Symbol::ToString(name));
#endif
	return Status::Code::InvalidSlot;
}

vd::status 
Shader::SetUniform(
	Symbol name, const vd::v4f32& value)
{
	if(m_Uniforms.IsUsed(name))
	{
		m_Uniforms.Set4f(name, value);
		return Status::Code::Success;
	}
#if defined(VD_DEBUG_SHADERS)			
	vdLogInfo("Uniform '%s' not used!", Symbol::ToString(name));
#endif
	return Status::Code::InvalidSlot;
}

vd::status
Shader::SetUniform(
	Symbol name, vd::i32 value)
{
	if(m_Uniforms.IsUsed(name))
	{
		m_Uniforms.Set1i(name, (vd::i32)value);
		return Status::Code::Success;
	}
#if defined(VD_DEBUG_SHADERS)			
	vdLogInfo("Uniform '%s' not used!", Symbol::ToString(name));
#endif
	return Status::Code::InvalidSlot;
}

vd::status 
Shader::SetUniform(
	Symbol name, vd::f32 x)
{
	if(m_Uniforms.IsUsed(name))
	{
		m_Uniforms.Set1f(name, (vd::f32)x);
		return Status::Code::Success;
	}
#if defined(VD_DEBUG_SHADERS)			
	vdLogInfo("Uniform '%s' not used!", Symbol::ToString(name));
#endif
	return Status::Code::InvalidSlot;
}

vd::status 
Shader::SetUniform(
	Symbol name, vd::f32 x, vd::f32 y)
{
	if(m_Uniforms.IsUsed(name))
	{
		m_Uniforms.Set2f(name, vd::v2f32(x,y));
		return Status::Code::Success;
	}
#if defined(VD_DEBUG_SHADERS)			
	vdLogInfo("Uniform '%s' not used!", Symbol::ToString(name));
#endif
	return Status::Code::InvalidSlot;
}

vd::status 
Shader::SetUniform(
	Symbol name, vd::f32 x, vd::f32 y, vd::f32 z)
{
	if(m_Uniforms.IsUsed(name))
	{
		m_Uniforms.Set3f(name, vd::v3f32(x,y,z));
		return Status::Code::Success;
	}
#if defined(VD_DEBUG_SHADERS)			
	vdLogInfo("Uniform '%s' not used!", Symbol::ToString(name));
#endif
	return Status::Code::InvalidSlot;
}

vd::status 
Shader::SetUniform(
	Symbol name, vd::f32 x, vd::f32 y, vd::f32 z, vd::f32 w)
{
	if(m_Uniforms.IsUsed(name))
	{
		m_Uniforms.Set4f(name, vd::v4f32(x,y,z,w));
		return Status::Code::Success;
	}
#if defined(VD_DEBUG_SHADERS)			
	vdLogInfo("Uniform '%s' not used!", Symbol::ToString(name));
#endif
	return Status::Code::InvalidSlot;
}

vd::status
Shader::SetUniform(
	Symbol name, const vd::m3f32& value)
{
	if(m_Uniforms.IsUsed(name))
	{
		m_Uniforms.Set3mf(name, (vd::m3f32)value);
		return Status::Code::Success;
	}
#if defined(VD_DEBUG_SHADERS)			
	vdLogInfo("Uniform '%s' not used!", Symbol::ToString(name));
#endif
	return Status::Code::InvalidSlot;
}

vd::status 
Shader::SetUniform(
	Symbol name, const vd::m4f32& value)
{
	if(m_Uniforms.IsUsed(name))
	{
		m_Uniforms.Set4mf(name, (vd::m4f32)value);
		return Status::Code::Success;
	}
#if defined(VD_DEBUG_SHADERS)			
	vdLogInfo("Uniform '%s' not used!", Symbol::ToString(name));
#endif
	return Status::Code::InvalidSlot;
}

vd::i32
Shader::GetSamplerSlot(
	Symbol name)
{
	vd::uid key = name.GetKey();
	if(m_SamplerSlots.count(key))
		return m_SamplerSlots[key];
	return Shader::InvalidSlot;
}

vd::i32
Shader::GetUniformSlot(
	Symbol name)
{
	vd::uid key = name.GetKey();
	if(m_UniformSlots.count(key))
		return m_UniformSlots[key];
	return Shader::InvalidSlot;
}

vd::i32
Shader::GetAttributeSlot(
	Symbol name)
{
	vd::uid key = name.GetKey();
	if(m_AttributeSlots.count(key))
		return m_AttributeSlots[key];
	return Shader::InvalidSlot;
}

vd::status
Shader::BindTexture(
	Symbol name,
	vd::i32 texture)
{
	vd::uid key = name.GetKey();
	if(m_SamplerSlots.count(key))
	{
		m_SamplerBindings[key] = texture;
		return Status::Code::Success;
	}
	return Status::Code::InvalidSlot;
}

vd::status
Shader::UnbindTexture(
	Symbol name,
	vd::i32 texture)
{
	vd::uid key = name.GetKey();
	if(m_SamplerSlots.count(key))
	{
		m_SamplerBindings.erase(key);
		return Status::Code::Success;
	}
	return Status::Code::InvalidSlot;
}

vd::status
Shader::BindAttribute(
	Symbol name,
	vd::i32 texture)
{
	vd::uid key = name.GetKey();
	if(m_AttributeSlots.count(key))
	{
		m_AttributeSlots[key] = texture;
		return Status::Code::Success;
	}
	return Status::Code::InvalidSlot;
}

vd::status
Shader::UnbindAttribute(
	Symbol name,
	vd::i32 texture)
{
	vd::uid key = name.GetKey();
	if(m_AttributeSlots.count(key))
	{
		m_AttributeSlots.erase(key);
		return Status::Code::Success;
	}
	return Status::Code::InvalidSlot;
}

// ============================================================================================== //

VD_IMPLEMENT_ABSTRACT_OBJECT(Shader, vd_sym(GraphicsShader), vd_sym(Object));

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_END();

// ============================================================================================== //
