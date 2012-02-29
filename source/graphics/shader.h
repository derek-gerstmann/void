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

#ifndef VD_GRAPHICS_SHADER_INCLUDED
#define VD_GRAPHICS_SHADER_INCLUDED

#include "core/core.h"
#include "core/memory.h"
#include "containers/paramset.h"
#include "containers/containers.h"
#include "graphics/graphics.h"

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_IMPORT(Core, Symbol);
VD_IMPORT(Containers, ParamSet);
VD_IMPORT(Containers, Map);

// ============================================================================================== //

class Shader : public Object 
{
public:
	static const vd::i32 InvalidSlot;
	typedef Map< vd::uid, vd::i32 >::type BindingMap;
	
public:

    Shader(Graphics::Context* context=NULL);
    Shader(Graphics::Context* context, const vd::string& name);
    Shader(const vd::string& name);
    ~Shader();

    virtual vd::status Destroy();

    bool Bind(bool dirty=false);
    bool Unbind();
    
    bool Compile(const char* vp, const char* gp, const char* fp);
    bool Load(const vd::string& vp, const vd::string& gp, const vd::string& fp);

	bool SetUniform(const Symbol& name, vd::i32 value);
	bool SetUniform(const Symbol& name, vd::f32 value);
	bool SetUniform(const Symbol& name, vd::f32 x, vd::f32 y);
	bool SetUniform(const Symbol& name, vd::f32 x, vd::f32 y, vd::f32 z);
	bool SetUniform(const Symbol& name, vd::f32 x, vd::f32 y, vd::f32 z, vd::f32 w);
	bool SetUniform(const Symbol& name, const vd::v2f32& value);
	bool SetUniform(const Symbol& name, const vd::v3f32& value);
	bool SetUniform(const Symbol& name, const vd::v4f32& value);
	bool SetUniform(const Symbol& name, const vd::m3f32& value);
	bool SetUniform(const Symbol& name, const vd::m4f32& value);
	
	vd::i32 GetShaderId(){ return m_Handle; }
	
	vd::i32 GetUniformSlot(const Symbol& name);
	vd::i32 GetSamplerSlot(const Symbol& name);
    vd::i32 GetAttributeSlot(const Symbol& name);
    
    bool IsValidUniform(const Symbol& name);
    bool IsActive();

	static const Symbol& GetTypeName(vd::i32 type);
	static vd::i32 GetSamplerTextureTarget(vd::i32 type);

	bool BindTexture(const Symbol& name, vd::i32 texture);
	bool UnbindTexture(const Symbol& name, vd::i32 texture);

    bool BindAttribute(const Symbol& name, vd::i32 slot);
    bool UnbindAttribute(const Symbol& name, vd::i32 slot);

    void SetName(const vd::string& name) {m_Name = name;}

public:

	static Shader* Create(const vd::string& name, const vd::string& vp, const vd::string& gp, const vd::string& fp);
	
	VD_DECLARE_OBJECT(Shader);
	
protected:

	bool AddUniform(const Symbol& name, vd::i32 type);
	void LocateUniforms();
	void SubmitUniforms(bool force);

	void BindSamplers();
	void UnbindSamplers();

    void LocateAttributes();
    void BindAttributes();
    void UnbindAttributes();

    void CheckStatus(const char* acMessage);

private:

	VD_DISABLE_COPY_CONSTRUCTORS(Shader);
    
    vd::i32 m_Handle;
	bool m_Active;

	vd::string m_Name;
	ParamSet m_Uniforms;
	BindingMap m_UniformSlots;
	BindingMap m_UniformTypes;
	BindingMap m_SamplerSlots;
	BindingMap m_SamplerBindings;
    BindingMap m_AttributeSlots;
    Graphics::Context* m_Graphics;
};

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_END();

// ============================================================================================== //

#endif