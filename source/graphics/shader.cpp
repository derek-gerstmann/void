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
#include "containers/containers.h"

#include "core/asserts.h"
#include "core/resources.h"
#include "core/symbol.h"
#include "core/logging.h"
#include "core/filesystem.h"
#include "core/file.h"

#include <cstring>
#include <cmath>
#include <string>

// ============================================================================================== //

// #define VD_DEBUG_SHADERS (1)

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_BEGIN();

// ============================================================================================== //

const vd::i32 Shader::InvalidSlot = -1;
typedef Containers::Vector< vd::symbol >::type SymbolList;

// ============================================================================================== //

Shader::Shader(
	Graphics::Context* context
) :
	Object(), 
    m_Handle(0),
    m_Active(false),
    m_Graphics(context)
{
	// EMPTY!
}

Shader::Shader(
	Graphics::Context* context,
	const vd::string& name)
:
	Object(), 
	m_Handle(0),
	m_Active(false),
	m_Name(name),
    m_Graphics(context)
{

}

Shader::Shader(
	const vd::string& name)
:
	Object(), 
	m_Handle(0),
	m_Active(false),
	m_Name(name),
    m_Graphics(NULL)
{

}

Shader::~Shader()
{
    Destroy();
}

vd::status
Shader::Destroy()
{
	Unbind();
    if(m_Handle)
        glDeleteProgram(m_Handle);
    m_Handle = 0;
    return Status::Code::Success;
}

bool
Shader::Bind(bool dirty)
{
	if(m_Active)
		return true;
		
	if(m_Handle)
	{
		BindAttributes();
		BindSamplers();
		glUseProgram(m_Handle);   
		SubmitUniforms(dirty);
		m_Active = true;
		return true;
	}
	return false;
}

bool 
Shader::Unbind()
{
	if(m_Active)
	{
		UnbindSamplers();
		UnbindAttributes();
		glUseProgram(0);    	
		m_Active = false;
		return true;
	}
	return false;
}

bool
Shader::IsActive()
{
	return m_Active;
}

Shader*
Shader::Create(
	const vd::string& name,
	const vd::string& vp, const vd::string& gp, const vd::string& fp)
{
	Shader* shader = VD_NEW(Shader, name);
	shader->Load(
		vp.size() ? vp.c_str() : NULL,
		gp.size() ? gp.c_str() : NULL, 
		fp.size() ? fp.c_str() : NULL);
	return shader;
}

bool
Shader::Compile(
	const char* vp, const char* gp, const char* fp)
{    
    GLint status;
    GLchar msglog[1024] = {0};
    
    m_Handle = glCreateProgram();
		  
	const char* vsSource = vp;
	GLuint vsHandle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vsHandle, 1, &vsSource, 0);
    glCompileShader(vsHandle);
    glGetShaderiv(vsHandle, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(vsHandle, sizeof(msglog), 0, msglog);
    if(status == GL_FALSE)  
    	vdLogError("Failed to compile vertex shader:\n%s\n--\n%s", vsSource, msglog);
	vdAssert(status == GL_TRUE);
	
    glAttachShader(m_Handle, vsHandle);

    GLuint gsHandle;
    if (gp != NULL  && strlen(gp)) 
    {
		const char* gsSource = gp;
        gsHandle = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gsHandle, 1, &gsSource, 0);
        glCompileShader(gsHandle);
		glGetShaderiv(gsHandle, GL_COMPILE_STATUS, &status);
		glGetShaderInfoLog(gsHandle, sizeof(msglog), 0, msglog);

		if(status == GL_FALSE)  
			vdLogError("Failed to compile geometry shader:\n%s\n--\n%s", gsSource, msglog);
		vdAssert(status == GL_TRUE);
	
        glAttachShader(m_Handle, gsHandle);

#if defined(VD_TARGET_OSX)
        glProgramParameteri(m_Handle, GL_GEOMETRY_OUTPUT_TYPE, GL_TRIANGLE_STRIP);
        glProgramParameteri(m_Handle, GL_GEOMETRY_INPUT_TYPE, GL_POINTS);
        glProgramParameteri(m_Handle, GL_GEOMETRY_VERTICES_OUT, 24);
#else
        glProgramParameteriEXT(m_Handle, GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLE_STRIP);
        glProgramParameteriEXT(m_Handle, GL_GEOMETRY_INPUT_TYPE_EXT, GL_POINTS);
        glProgramParameteriEXT(m_Handle, GL_GEOMETRY_VERTICES_OUT_EXT, 24);
#endif
    }
    
    GLuint fsHandle;
    if (fp != NULL && strlen(fp)) 
    {
		const char* fsSource = fp;
        fsHandle = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fsHandle, 1, &fsSource, 0);
        glCompileShader(fsHandle);
		glGetShaderiv(fsHandle, GL_COMPILE_STATUS, &status);
		glGetShaderInfoLog(fsHandle, sizeof(msglog), 0, msglog);
		if(status == GL_FALSE)  
			vdLogError("Failed to compile fragment shader:\n%s\n--\n%s", fsSource, msglog);
		vdAssert(status == GL_TRUE);

        glAttachShader(m_Handle, fsHandle);
    }

    glLinkProgram(m_Handle);
    
    glGetProgramiv(m_Handle, GL_LINK_STATUS, &status);
    glGetProgramInfoLog(m_Handle, sizeof(msglog), 0, msglog);

    if (status == GL_FALSE) 
    {
        vdLogWarning("%s\b", "Failed to link shader!");
        vdLogWarning("%s", msglog);
        return false;
    }
    
    LocateUniforms();
    LocateAttributes();
    return true;
}

bool
Shader::Load(
	const vd::string& vp, const vd::string& gp, const vd::string& fp)
{
	if(m_Graphics == NULL)
		return false;

	Runtime::Context* rt = m_Graphics->GetRuntime();
	Core::FileSystem* fs = rt->GetFileSystem();

	vd::string vs_file = vp.size() ? fs->FindInSearchPath(vp) : vp;
	vd::string gs_file = gp.size() ? fs->FindInSearchPath(gp) : gp;
	vd::string fs_file = fp.size() ? fs->FindInSearchPath(fp) : fp;

	int vs_bytes = 0;
	int gs_bytes = 0;
	int fs_bytes = 0;

	char* vs_src = NULL;
	char* gs_src = NULL;
	char* fs_src = NULL;

	if(vs_file.size())
	{
		bool loaded = Core::FileAccess::Load (vs_file, false, vs_bytes, vs_src);
		if(loaded)
		{
//			vs_file = vd::string("// #line 1 \"") + vd::string(vs_key) + vd::string("\"\n") + vd::string(vs_src);
			vs_file = vd::string(vs_src);
			VD_SAFE_DELETE_ARRAY(vs_src);
		}
		else
		{
			vs_file = vd::string("");
		}
	}

	if(gs_file.size())
	{
		bool loaded = Core::FileAccess::Load (gs_file, false, gs_bytes, gs_src);
		if(loaded)
		{
//			gs_file = vd::string("// #line 1 \"") + vd::string(gs_key) + vd::string("\"\n") + 
			gs_file = vd::string(gs_src);
			VD_SAFE_DELETE_ARRAY(gs_src);
		}
		else
		{
			gs_file = vd::string("");
		}
	}

	if(fs_file.size())
	{
		bool loaded = Core::FileAccess::Load(fs_file, false, fs_bytes, fs_src);
		if(loaded)
		{
//			fs_file = vd::string("// #line 1 \"") + vd::string(fs_key) + vd::string("\"\n") + vd::string(fs_src);
			fs_file = vd::string(fs_src);
			VD_SAFE_DELETE_ARRAY(fs_src);
		}
		else
		{
			fs_file = vd::string("");
		}

	}

	bool success = Compile(
		vs_file.size() ? vs_file.c_str() : NULL,
		gs_file.size() ? gs_file.c_str() : NULL, 
		fs_file.size() ? fs_file.c_str() : NULL);

	return success;
}

const vd::symbol&
Shader::GetTypeIdentifier(
	GLint type)
{
	switch (type)
	{
		case GL_SAMPLER_1D:				return vd_sym(GL_SAMPLER_1D); 
		case GL_SAMPLER_2D:				return vd_sym(GL_SAMPLER_2D); 
		case GL_SAMPLER_3D:				return vd_sym(GL_SAMPLER_3D); 
		case GL_SAMPLER_1D_SHADOW_ARB:	return vd_sym(GL_SAMPLER_1D_SHADOW_ARB); 
		case GL_SAMPLER_2D_SHADOW_ARB:	return vd_sym(GL_SAMPLER_2D_SHADOW_ARB); 
		case GL_FLOAT:					return vd_sym(GL_FLOAT); 
		case GL_FLOAT_VEC2_ARB: 		return vd_sym(GL_FLOAT_VEC2_ARB); 
		case GL_FLOAT_VEC3_ARB: 		return vd_sym(GL_FLOAT_VEC3_ARB); 
		case GL_FLOAT_VEC4_ARB: 		return vd_sym(GL_FLOAT_VEC4_ARB); 
		case GL_INT:            		return vd_sym(GL_INT); 
		case GL_INT_VEC2_ARB:   		return vd_sym(GL_INT_VEC2_ARB); 
		case GL_INT_VEC3_ARB:   		return vd_sym(GL_INT_VEC3_ARB); 
		case GL_INT_VEC4_ARB:   		return vd_sym(GL_INT_VEC4_ARB); 
		case GL_BOOL_ARB:       		return vd_sym(GL_BOOL_ARB); 
		case GL_BOOL_VEC2_ARB:  		return vd_sym(GL_BOOL_VEC2_ARB); 
		case GL_BOOL_VEC3_ARB:  		return vd_sym(GL_BOOL_VEC3_ARB); 
		case GL_BOOL_VEC4_ARB:  		return vd_sym(GL_BOOL_VEC4_ARB); 
		case GL_FLOAT_MAT2_ARB: 		return vd_sym(GL_FLOAT_MAT2_ARB); 
		case GL_FLOAT_MAT3_ARB: 		return vd_sym(GL_FLOAT_MAT3_ARB); 
		case GL_FLOAT_MAT4_ARB: 		return vd_sym(GL_FLOAT_MAT4_ARB); 
	}
	return vd_sym(GL_INVALID);
}

vd::i32
Shader::GetSamplerTextureTarget(
	GLint type)
{
	switch(type)
	{
		case GL_SAMPLER_1D:
		case GL_SAMPLER_1D_SHADOW_ARB:
			return GL_TEXTURE_1D;
		case GL_SAMPLER_2D:
		case GL_SAMPLER_2D_SHADOW_ARB:
			return GL_TEXTURE_2D;
		case GL_SAMPLER_3D:
			return GL_TEXTURE_3D;
		default:
			return 0;
	};
	return 0;
}

bool
Shader::AddUniform(
	const vd::symbol& name, GLint type)
{
	switch (type)
	{
		case GL_FLOAT: 
			m_Uniforms.Add1f(name, (vd::f32)0.0f);
			return true;
		case GL_FLOAT_VEC2_ARB: 
			m_Uniforms.Add2f(name, vd::v2f32(0.0f, 0.0f));
			return true;
		case GL_FLOAT_VEC3_ARB: 
			m_Uniforms.Add3f(name, vd::v3f32(0.0f, 0.0f, 0.0f));
			return true;
		case GL_FLOAT_VEC4_ARB: 
			m_Uniforms.Add4f(name, vd::v4f32(0.0f, 0.0f, 0.0f, 1.0f));
			return true;
		case GL_SAMPLER_1D:
		case GL_SAMPLER_2D:
		case GL_SAMPLER_3D:
		case GL_SAMPLER_1D_SHADOW_ARB:
		case GL_SAMPLER_2D_SHADOW_ARB:
		case GL_INT:            
			m_Uniforms.Add1i(name, (vd::i32)0);
			return true;
		case GL_INT_VEC2_ARB:   			return false;
		case GL_INT_VEC3_ARB:   			return false;
		case GL_INT_VEC4_ARB:   			return false;
		case GL_BOOL_ARB:       			return false;
		case GL_BOOL_VEC2_ARB:  			return false;
		case GL_BOOL_VEC3_ARB:  			return false;
		case GL_BOOL_VEC4_ARB:  			return false;
		case GL_FLOAT_MAT2_ARB: 			return false;
		case GL_FLOAT_MAT3_ARB: 
		{
			vd::m3f32 m = vd::m3f32(Constants::One);
			m_Uniforms.Add3mf(name, m);
			return true;
		}
		case GL_FLOAT_MAT4_ARB: 
		{
			vd::m4f32 m = vd::m4f32(Constants::One);
			m_Uniforms.Add4mf(name, m);
			return true;
		}
	}
	return false;
}

bool
Shader::IsValidUniform(
	const vd::symbol& name)
{
	return m_Uniforms.IsUsed(name);
}

void
Shader::LocateUniforms()
{
	GLint uniform_count;
	GLint uniform_max_length;
	int sampler_count = 0;

	glUseProgram(m_Handle);
	glGetProgramiv(m_Handle, GL_ACTIVE_UNIFORMS, &uniform_count);
	glGetProgramiv(m_Handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniform_max_length);

	if(uniform_count < 1 || uniform_max_length < 1)
		return;
		
	char* uniform_string = VD_NEW_ARRAY(char, uniform_max_length);
	for (GLint i = 0; i < uniform_count; i++)
	{
		GLenum type = 0;
		GLint length = 0;
		GLint size = 0;
		uniform_string[0] = '\0';
		
		glGetActiveUniform(m_Handle, i, uniform_max_length, &length, &size, &type, uniform_string);
		if (type >= GL_SAMPLER_1D && type <= GL_SAMPLER_2D_RECT_SHADOW_ARB)
		{
			GLint location = glGetUniformLocation(m_Handle, uniform_string);
			if(location < 0)
				continue;
				
            Core::Symbol name = Symbol::Register(uniform_string);
			vd::uid key = name.GetKey();

#if defined(VD_DEBUG_SHADERS)			
			vdLogInfo("Shader[%s] : Adding uniform '%s' [%02d] : [%llx] '%s' as [%s] -> [%02d] ", 
				m_Name.c_str(), uniform_string, location, index, Symbol::ToString(name), 
				Symbol::ToString(Shader::GetTypeIdentifier(type)), sampler_count);
#endif

			m_UniformSlots[key] = location;
			m_UniformTypes[key] = type;
			m_Uniforms.Add1i(name, (vd::i32)sampler_count);
			m_SamplerSlots[key] = sampler_count++;
		} 
		else 
		{
			if (strncmp(uniform_string, "gl_", 3) != 0)
			{
				char *bracket = strchr(uniform_string, '[');
				if (bracket == NULL || (bracket[1] == '0' && bracket[2] == ']'))
				{
					if (bracket)
					{
						*bracket = '\0';
						length = (GLint) (bracket - uniform_string);
					}

					GLint location = glGetUniformLocation(m_Handle, uniform_string);
					if(location < 0)
						continue;

                    Core::Symbol name = Symbol::Register(uniform_string);
					vd::uid key = name.GetKey();
					m_UniformSlots[key] = location;
					m_UniformTypes[key] = type;
					AddUniform(name, type);
	
#if defined(VD_DEBUG_SHADERS)			
					vdLogInfo("Shader[%s] : Adding uniform '%s' [%02d] : [%llx] '%s' as [%s] -> [%02d] ", 
						m_Name.c_str(), uniform_string, location, index, Symbol::ToString(name), 
						Symbol::ToString(Shader::GetTypeIdentifier(type)), sampler_count);
#endif

				} 
			}
		}
	}
	VD_DELETE_ARRAY(uniform_string);

	glUseProgram(0);
}

void
Shader::LocateAttributes()
{
	GLint attrib_count;
	GLint attrib_max_length;
	int attrib_index = 0;

	glUseProgram(m_Handle);
	glGetProgramiv(m_Handle, GL_ACTIVE_ATTRIBUTES, &attrib_count);
	glGetProgramiv(m_Handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attrib_max_length);

	if(attrib_count < 1 || attrib_max_length < 1)
		return;
		
	char* attrib_string = VD_NEW_ARRAY(char, attrib_max_length);
	for (GLint i = 0; i < attrib_count; i++)
	{
		GLenum type = 0;
		GLint length = 0;
		GLint size = 0;
		attrib_string[0] = '\0';
		
		glGetActiveAttrib(m_Handle, i, attrib_max_length, &length, &size, &type, attrib_string);
		if (strncmp(attrib_string, "gl_", 3) != 0)
		{
			char *bracket = strchr(attrib_string, '[');
			if (bracket == NULL || (bracket[1] == '0' && bracket[2] == ']'))
			{
				if (bracket)
				{
					*bracket = '\0';
					length = (GLint) (bracket - attrib_string);
				}

				GLint location = glGetAttribLocation(m_Handle, attrib_string);
				if(location < 0)
					continue;

                Core::Symbol name = Symbol::Register(attrib_string);
				vd::uid key = name.GetKey();
				m_AttributeSlots[key] = location;

#if defined(VD_DEBUG_SHADERS)			
				vd::symbol name = Symbol::Retrieve(key);
				vdLogInfo("Shader[%s] : Adding attrib '%s' [%02d] : [%llx] '%s' as [%s] -> [%02d] ", 
					m_Name.c_str(), attrib_string, location, index, Symbol::ToString(name), 
					Symbol::ToString(Shader::GetTypeIdentifier(type)), attrib_index);
#endif
				attrib_index++;
			} 
		}
	}
	VD_DELETE_ARRAY(attrib_string);

	glUseProgram(0);
}

void 
Shader::BindSamplers()
{
	BindingMap::const_iterator it;
	for(it = m_SamplerSlots.begin(); it != m_SamplerSlots.end(); ++it)
	{
		GLint texture = m_SamplerBindings[it->first];
		GLint type = m_UniformTypes[it->first];
		GLint slot = m_SamplerSlots[it->first];
		const char* name = Symbol::Lookup(it->first);
		vd::uid key = it->first;

		if(m_SamplerBindings.count(it->first) < 1)
		{
#if defined(VD_DEBUG_SHADERS)			
			vdLogInfo("Shader[%s] : MISSING Sampler [%02d] %s [%s] : %s !!", 
				m_Name.c_str(), slot, name ? name : "<NULL>", key.ToString().c_str(), 
				Symbol::ToString(Shader::GetTypeIdentifier(m_UniformTypes[key])));
#endif
			continue;
		}

		GLint target = Shader::GetSamplerTextureTarget(type);

#if defined(VD_DEBUG_SHADERS)			
		vdLogInfo("Shader[%s] : Binding Sampler [%02d] %s [%s] : %s [%08d]", 
			m_Name.c_str(), slot, name ? name : "<NULL>", key.ToString().c_str(), 
			Symbol::ToString(Shader::GetTypeIdentifier(m_UniformTypes[name.ToKey()])),
			texture);
#else
    	VD_IGNORE_UNUSED(name);
#endif
		glActiveTexture(GL_TEXTURE0 + slot);

    	glEnable(target);		
    	glBindTexture(target, texture);		
	}
}

void
Shader::UnbindSamplers()
{
	BindingMap::const_iterator it;
	for(it = m_SamplerSlots.begin(); it != m_SamplerSlots.end(); ++it)
	{
		if(m_SamplerBindings.count(it->first) < 1)
			continue;

		const char* name = Symbol::Lookup(it->first);
		GLint type = m_UniformTypes[it->first];
		GLint texture = m_SamplerBindings[it->first];
		GLint target = GetSamplerTextureTarget(type);
		GLint slot = m_SamplerSlots[it->first];
		vd::uid key = it->first;

#if defined(VD_DEBUG_SHADERS)			
		vdLogInfo("Shader[%s] : Unbinding Sampler [%02d] %s [%s] : %s [%08d]", 
			m_Name.c_str(), slot, name ? name : "<NULL>", key.ToString().c_str(), 
			Symbol::ToString(Shader::GetTypeIdentifier(m_UniformTypes[key])),
			texture);
#else
    	VD_IGNORE_UNUSED(name);
    	VD_IGNORE_UNUSED(texture);
#endif

		glActiveTexture(GL_TEXTURE0 + slot);
    	glBindTexture(target, 0);		
    	glDisable(target);		
	}
	glActiveTexture(GL_TEXTURE0);
}

void
Shader::BindAttributes()
{
/*
	BindingMap::const_iterator it;
	for(it = m_AttributeSlots.begin(); it != m_AttributeSlots.end(); ++it)
	{
		const vd::symbol& name = Symbol::Retrieve(it->first);
		GLint slot = m_AttributeSlots[it->first];
		Geometry::AttributeSlot::Value attrib = (Geometry::AttributeSlot::Value)slot;

		vdLogInfo("Shader[%s] : Binding Attribute [%02d] %s [%s] : %s", 
			m_Name.c_str(), slot, Symbol::ToString(name), name.ToKey().ToString().c_str(), 
			Geometry::AttributeSlot::ToString(attrib));

	    glBindAttribLocation(m_Handle, slot, Symbol::ToString(name));
	}
*/
}

void
Shader::UnbindAttributes()
{

}

void 
Shader::SubmitUniforms(bool force)
{
	vd::i64 i = 0;
	ParamSet::ParamVector changes;

	if(force)
	{
		m_Uniforms.GetAllSymbols(changes);
	}
	else
	{
		m_Uniforms.GetChanges(changes);
	}

	if(changes.size() < 1)
	{
		return;
	}
	else
	{
#if defined(VD_DEBUG_SHADERS)			
		vdLogDebug("Shader[%s] : Updating %d uniforms...", m_Name.c_str(), (int)changes.size());
#endif
	}

	ParamSet::ParamVector::const_iterator it;
	for(it = changes.begin(); it != changes.end(); ++it)
	{
#if defined(VD_DEBUG_SHADERS)			
		vd::uid key = *it;
		vd::symbol name = Symbol::Retrieve(key);
		vdLogInfo("Shader[%s] : Updating '%s' ... ", 
			m_Name.c_str(), Symbol::ToString(name));
#endif
	}

	for(i = 0, it = changes.begin(); it != changes.end(); ++it)
	{
		vd::uid key = *it;
		vd::symbol name = Symbol::Retrieve(key);

		if(m_Uniforms.IsA(ParamSet::I32, name) && m_UniformSlots.count(key))
		{
			GLint slot = m_UniformSlots[key];
			vd::i32 value = m_Uniforms.Get1i(name);
			glUniform1i(slot, (GLint)value);
			m_Uniforms.ClearChanges(name);

#if defined(VD_DEBUG_SHADERS)			
			vdLogInfo("Shader[%s] : Submitting '%s' -> '%d'", 
				m_Name.c_str(), Symbol::ToString(name), value);
#endif
		}
	}

	for(i = 0, it = changes.begin(); it != changes.end(); ++it)
	{
		vd::uid key = *it;
		vd::symbol name = Symbol::Retrieve(key);

		if(m_Uniforms.IsA(ParamSet::F32, name) && m_UniformSlots.count(key))
		{
			GLint slot = m_UniformSlots[key];
			vd::f32 value = m_Uniforms.Get1f(name);
			glUniform1f(slot, (GLfloat)value);
			m_Uniforms.ClearChanges(name);

#if defined(VD_DEBUG_SHADERS)			
			vdLogInfo("Shader[%s] : Submitting '%s' -> '%8.3f'", 
				m_Name.c_str(), Symbol::ToString(name), value);
#endif
		}
	}

	for(i = 0, it = changes.begin(); it != changes.end(); ++it)
	{
		vd::uid key = *it;
		vd::symbol name = Symbol::Retrieve(key);

		if(m_Uniforms.IsA(ParamSet::V2F32, name) && m_UniformSlots.count(key))
		{
			GLint slot = m_UniformSlots[key];
			const vd::v2f32& value = m_Uniforms.Get2f(name);
			glUniform2f(slot, (GLfloat)value.x, (GLfloat)value.y );
			m_Uniforms.ClearChanges(name);

#if defined(VD_DEBUG_SHADERS)			
			vdLogInfo("Shader[%s] : Submitting '%s' -> [%8.3f %8.3f]", 
				m_Name.c_str(), Symbol::ToString(name), (GLfloat)value.x, (GLfloat)value.y);
#endif
		}
	}

	for(i = 0, it = changes.begin(); it != changes.end(); ++it)
	{
		vd::uid key = *it;
		vd::symbol name = Symbol::Retrieve(key);

		if(m_Uniforms.IsA(ParamSet::V3F32, name) && m_UniformSlots.count(key))
		{
			GLint slot = m_UniformSlots[key];
			const vd::v3f32& value = m_Uniforms.Get3f(name);
			glUniform3f(slot, (GLfloat)value.x, (GLfloat)value.y, (GLfloat)value.z );
			m_Uniforms.ClearChanges(name);

#if defined(VD_DEBUG_SHADERS)			
			vdLogInfo("Shader[%s] : Submitting '%s' -> [%8.3f %8.3f %8.3f]", 
				m_Name.c_str(), name, (GLfloat)value.x, (GLfloat)value.y, (GLfloat)value.z);
#endif
		}
	}

	for(i = 0, it = changes.begin(); it != changes.end(); ++it)
	{
		vd::uid key = *it;
		vd::symbol name = Symbol::Retrieve(key);

		if(m_Uniforms.IsA(ParamSet::V4F32, name) && m_UniformSlots.count(key))
		{
			GLint slot = m_UniformSlots[key];
			const vd::v4f32& value = m_Uniforms.Get4f(name);
		    glUniform4f(slot, (GLfloat)value.x, (GLfloat)value.y, (GLfloat)value.z, (GLfloat)value.w );
			m_Uniforms.ClearChanges(name);

#if defined(VD_DEBUG_SHADERS)			
			vdLogInfo("Shader[%s] : Submitting '%s' -> [%8.3f %8.3f %8.3f %8.3f]", 
				m_Name.c_str(), Symbol::ToString(name), (GLfloat)value.x, (GLfloat)value.y, (GLfloat)value.z, (GLfloat)value.w );
#endif
		}
	}

	for(i = 0, it = changes.begin(); it != changes.end(); ++it)
	{
		vd::uid key = *it;
		vd::symbol name = Symbol::Retrieve(key);

		if(m_Uniforms.IsA(ParamSet::M3F32, name) && m_UniformSlots.count(key))
		{
			GLint slot = m_UniformSlots[key];
			const vd::m3f32& m = m_Uniforms.Get3mf(name);

			GLfloat packed[3*3] = {
				(GLfloat)m[0][0], (GLfloat)m[0][1], (GLfloat)m[0][2], 
				(GLfloat)m[1][0], (GLfloat)m[1][1], (GLfloat)m[1][2], 
				(GLfloat)m[2][0], (GLfloat)m[2][1], (GLfloat)m[2][2]
			};
			
			glUniformMatrix3fv(slot, 1, 0, packed);
			m_Uniforms.ClearChanges(name);
			
#if defined(VD_DEBUG_SHADERS)			
			vdLogInfo("Shader[%s] : Submitting '%s' -> [%8.3f %8.3f %8.3f] [%8.3f %8.3f %8.3f] [%8.3f %8.3f %8.3f]", 
				m_Name.c_str(), Symbol::ToString(name), 
				(GLfloat)m[0][0], (GLfloat)m[0][1], (GLfloat)m[0][2], 
				(GLfloat)m[1][0], (GLfloat)m[1][1], (GLfloat)m[1][2], 
				(GLfloat)m[2][0], (GLfloat)m[2][1], (GLfloat)m[2][2]);
#endif
		}
	}

	for(i = 0, it = changes.begin(); it != changes.end(); ++it)
	{
		vd::uid key = *it;
		vd::symbol name = Symbol::Retrieve(key);

		if(m_Uniforms.IsA(ParamSet::M4F32, name) && m_UniformSlots.count(key))
		{
			GLint slot = m_UniformSlots[key];
			const vd::m4f32& m = m_Uniforms.Get4mf(name);

			GLfloat packed[4*4] = {
				(GLfloat)m[0][0], (GLfloat)m[0][1], (GLfloat)m[0][2], (GLfloat)m[0][3], 
				(GLfloat)m[1][0], (GLfloat)m[1][1], (GLfloat)m[1][2], (GLfloat)m[1][3], 
				(GLfloat)m[2][0], (GLfloat)m[2][1], (GLfloat)m[2][2], (GLfloat)m[2][3], 
				(GLfloat)m[3][0], (GLfloat)m[3][1], (GLfloat)m[3][2], (GLfloat)m[3][3]
			};
			
			glUniformMatrix4fv(slot, 1, 0, packed);
			m_Uniforms.ClearChanges(name);

#if defined(VD_DEBUG_SHADERS)			
			vdLogInfo("Shader[%s] : Submitting '%s' -> [%8.3f %8.3f %8.3f %8.3f] [%8.3f %8.3f %8.3f %8.3f] [%8.3f %8.3f %8.3f %8.3f] [%8.3f %8.3f %8.3f %8.3f]", 
				m_Name.c_str(), Symbol::ToString(name), 
				(GLfloat)m[0][0], (GLfloat)m[0][1], (GLfloat)m[0][2], (GLfloat)m[0][3], 
				(GLfloat)m[1][0], (GLfloat)m[1][1], (GLfloat)m[1][2], (GLfloat)m[1][3], 
				(GLfloat)m[2][0], (GLfloat)m[2][1], (GLfloat)m[2][2], (GLfloat)m[2][3], 
				(GLfloat)m[3][0], (GLfloat)m[3][1], (GLfloat)m[3][2], (GLfloat)m[3][3]);
#endif
		}
	}
}

bool 
Shader::SetUniform(
	const vd::symbol& name, const vd::v2f32& value)
{
	if(m_Uniforms.IsUsed(name))
	{
		m_Uniforms.Set2f(name, value);
		return true;
	}
#if defined(VD_DEBUG_SHADERS)			
	vdLogInfo("Uniform '%s' not used!", Symbol::ToString(name));
#endif
	return false;
}

bool 
Shader::SetUniform(
	const vd::symbol& name, const vd::v3f32& value)
{
	if(m_Uniforms.IsUsed(name))
	{
		m_Uniforms.Set3f(name, value);
		return true;
	}
#if defined(VD_DEBUG_SHADERS)			
	vdLogInfo("Uniform '%s' not used!", Symbol::ToString(name));
#endif
	return false;
}

bool 
Shader::SetUniform(
	const vd::symbol& name, const vd::v4f32& value)
{
	if(m_Uniforms.IsUsed(name))
	{
		m_Uniforms.Set4f(name, value);
		return true;
	}
#if defined(VD_DEBUG_SHADERS)			
	vdLogInfo("Uniform '%s' not used!", Symbol::ToString(name));
#endif
	return false;
}

bool
Shader::SetUniform(
	const vd::symbol& name, vd::i32 value)
{
	if(m_Uniforms.IsUsed(name))
	{
		m_Uniforms.Set1i(name, (vd::i32)value);
		return true;
	}
#if defined(VD_DEBUG_SHADERS)			
	vdLogInfo("Uniform '%s' not used!", Symbol::ToString(name));
#endif
	return false;
}

bool 
Shader::SetUniform(
	const vd::symbol& name, vd::f32 x)
{
	if(m_Uniforms.IsUsed(name))
	{
		m_Uniforms.Set1f(name, (vd::f32)x);
		return true;
	}
#if defined(VD_DEBUG_SHADERS)			
	vdLogInfo("Uniform '%s' not used!", Symbol::ToString(name));
#endif
	return false;
}

bool 
Shader::SetUniform(
	const vd::symbol& name, vd::f32 x, vd::f32 y)
{
	if(m_Uniforms.IsUsed(name))
	{
		m_Uniforms.Set2f(name, vd::v2f32(x,y));
		return true;
	}
#if defined(VD_DEBUG_SHADERS)			
	vdLogInfo("Uniform '%s' not used!", Symbol::ToString(name));
#endif
	return false;
}

bool 
Shader::SetUniform(
	const vd::symbol& name, vd::f32 x, vd::f32 y, vd::f32 z)
{
	if(m_Uniforms.IsUsed(name))
	{
		m_Uniforms.Set3f(name, vd::v3f32(x,y,z));
		return true;
	}
#if defined(VD_DEBUG_SHADERS)			
	vdLogInfo("Uniform '%s' not used!", Symbol::ToString(name));
#endif
	return false;
}

bool 
Shader::SetUniform(
	const vd::symbol& name, vd::f32 x, vd::f32 y, vd::f32 z, vd::f32 w)
{
	if(m_Uniforms.IsUsed(name))
	{
		m_Uniforms.Set4f(name, vd::v4f32(x,y,z,w));
		return true;
	}
#if defined(VD_DEBUG_SHADERS)			
	vdLogInfo("Uniform '%s' not used!", Symbol::ToString(name));
#endif
	return false;
}

bool
Shader::SetUniform(
	const vd::symbol& name, const vd::m3f32& value)
{
	if(m_Uniforms.IsUsed(name))
	{
		m_Uniforms.Set3mf(name, (vd::m3f32)value);
		return true;
	}
#if defined(VD_DEBUG_SHADERS)			
	vdLogInfo("Uniform '%s' not used!", Symbol::ToString(name));
#endif
	return false;
}

bool 
Shader::SetUniform(
	const vd::symbol& name, const vd::m4f32& value)
{
	if(m_Uniforms.IsUsed(name))
	{
		m_Uniforms.Set4mf(name, (vd::m4f32)value);
		return true;
	}
#if defined(VD_DEBUG_SHADERS)			
	vdLogInfo("Uniform '%s' not used!", Symbol::ToString(name));
#endif
	return false;
}

vd::i32
Shader::GetSamplerSlot(
	const vd::symbol& name)
{
	if(m_SamplerSlots.count(name.ToKey()))
		return m_SamplerSlots[name.ToKey()];
	return Shader::InvalidSlot;
}

vd::i32
Shader::GetUniformSlot(
	const vd::symbol& name)
{
	if(m_UniformSlots.count(name.ToKey()))
		return m_UniformSlots[name.ToKey()];
	return Shader::InvalidSlot;
}

vd::i32
Shader::GetAttributeSlot(
	const vd::symbol& name)
{
	if(m_AttributeSlots.count(name.ToKey()))
		return m_AttributeSlots[name.ToKey()];
	return Shader::InvalidSlot;
}

bool
Shader::BindTexture(
	const vd::symbol& name,
	vd::i32 texture)
{
	if(m_SamplerSlots.count(name.ToKey()))
	{
		m_SamplerBindings[name.ToKey()] = texture;
		return true;
	}
	return false;
}

bool
Shader::UnbindTexture(
	const vd::symbol& name,
	vd::i32 texture)
{
	if(m_SamplerSlots.count(name.ToKey()))
	{
		m_SamplerBindings.erase(name.ToKey());
		return true;
	}
	return false;
}


bool
Shader::BindAttribute(
	const vd::symbol& name,
	vd::i32 texture)
{
	if(m_AttributeSlots.count(name.ToKey()))
	{
		m_AttributeSlots[name.ToKey()] = texture;
		return true;
	}
	return false;
}

bool
Shader::UnbindAttribute(
	const vd::symbol& name,
	vd::i32 texture)
{
	if(m_AttributeSlots.count(name.ToKey()))
	{
		m_AttributeSlots.erase(name.ToKey());
		return true;
	}
	return false;
}
// ============================================================================================== //

VD_IMPLEMENT_OBJECT(Shader, vd_sym(Shader), vd_sym(Object));

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_END();

// ============================================================================================== //
