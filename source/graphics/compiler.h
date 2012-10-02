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
#include "graphics/context.h"
#include "graphics/graphics.h"

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_USING(Core, Symbol);
VD_USING(Core, ParamSet);
VD_USING(Core, Map);

// ============================================================================================== //

class Compiler : public Object
{
public:

    Compiler(Graphics::Context* context=NULL);
    ~Compiler();

    virtual vd::status Acquire();
    virtual vd::status Release();
    virtual vd::status Destroy();
    virtual vd::status Reset();

    Graphics::Shader* Build(const vd::string& vp, const vd::string& gp, const vd::string& fp);
    Graphics::Shader* Load(const vd::string& vp, const vd::string& gp, const vd::string& fp);

	VD_DECLARE_OBJECT(Compiler);
	
protected:

	vd::status LocateUniforms(Graphics::Shader* shader);
    vd::status LocateAttributes(Graphics::Shader* shader);

    void CheckStatus(const char* acMessage);

private:

	VD_DISABLE_COPY_CONSTRUCTORS(Compiler);

    Context*    m_Context;    
};

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_END();

// ============================================================================================== //

#endif