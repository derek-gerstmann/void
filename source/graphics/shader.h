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
#include "core/paramset.h"
#include "core/collections.h"
#include "graphics/graphics.h"

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_USING(Core, Symbol);
VD_USING(Core, ParamSet);
VD_USING(Core, Map);

// ============================================================================================== //

class Shader : public Object 
{
public:

    static const vd::i32                   InvalidSlot;
    typedef Map< vd::uid, vd::i32 >::type  BindingMap;

    VD_DECLARE_ENUM(StateId,
        Allocated,
        Compiled,
        Active,
        Inactive);

    struct Data 
    {
        vd::u32                 Id;
        vd::u32                 Index;
        vd::u32                 Usage;
        StateId::Value          State;
    };
	
public:

    Shader(Graphics::Context* context=NULL);
    Shader(Graphics::Context* context, const vd::string& name);
    Shader(const vd::string& name);
    virtual ~Shader();

    virtual vd::status Acquire();
    virtual vd::status Release();
    virtual vd::status Destroy();

    virtual vd::status Reset();
    virtual vd::status Setup(const Data& data);
    virtual vd::status Bind(bool force_update=false);
    virtual vd::status Unbind();

    void SetName(const vd::string& name) {m_Name = name;}

    const Data& GetData() const;
    const Data* GetPtr() const;
    vd::i32 GetShaderId() const;

    vd::status SetUniform(Symbol name, vd::i32 value);
    vd::status SetUniform(Symbol name, vd::f32 value);
    vd::status SetUniform(Symbol name, vd::f32 x, vd::f32 y);
    vd::status SetUniform(Symbol name, vd::f32 x, vd::f32 y, vd::f32 z);
    vd::status SetUniform(Symbol name, vd::f32 x, vd::f32 y, vd::f32 z, vd::f32 w);
    vd::status SetUniform(Symbol name, const vd::v2f32& value);
    vd::status SetUniform(Symbol name, const vd::v3f32& value);
    vd::status SetUniform(Symbol name, const vd::v4f32& value);
    vd::status SetUniform(Symbol name, const vd::m3f32& value);
    vd::status SetUniform(Symbol name, const vd::m4f32& value);

    vd::status BindTexture(Symbol name, vd::i32 texture);
    vd::status UnbindTexture(Symbol name, vd::i32 texture);
    
    vd::status BindAttribute(Symbol name, vd::i32 slot);
    vd::status UnbindAttribute(Symbol name, vd::i32 slot);
    
    vd::i32 GetUniformSlot(Symbol name);
    vd::i32 GetSamplerSlot(Symbol name);
    vd::i32 GetAttributeSlot(Symbol name);
    
    bool IsValidUniform(Symbol name);
    bool IsActive(void);

	VD_DECLARE_OBJECT(Shader);
	
private:

	VD_DISABLE_COPY_CONSTRUCTORS(Shader);
    
protected:

    Shader::Data       m_Data;
	vd::string         m_Name;
    ParamSet           m_Uniforms;
    BindingMap         m_UniformSlots;
    BindingMap         m_UniformTypes;
    BindingMap         m_SamplerSlots;
    BindingMap         m_SamplerBindings;
    BindingMap         m_AttributeSlots;
    Context*           m_Context;
};

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_END();

// ============================================================================================== //

#endif