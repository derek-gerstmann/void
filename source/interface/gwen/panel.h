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

#ifndef VD_INTERFACE_GWEN_PANEL_INCLUDED
#define VD_INTERFACE_GWEN_PANEL_INCLUDED

// ============================================================================================== //

#include "interface/gwen/gwen.h"
#include "containers/containers.h"

#include "Gwen/Gwen.h"
#include "Gwen/Align.h"
#include "Gwen/Utility.h"
#include "Gwen/Controls/WindowControl.h"
#include "Gwen/Controls/ScrollControl.h"
#include "Gwen/Controls/ListBox.h"
#include "Gwen/Controls/TextBox.h"
#include "Gwen/Controls/LabelClickable.h"
#include "Gwen/Controls/HorizontalSlider.h"
#include "Gwen/Controls/ResizableControl.h"

#include "Gwen/Skins/Simple.h"
#include "Gwen/Skins/TexturedBase.h"
#include "Gwen/UnitTest/UnitTest.h"

// ============================================================================================== //

VD_INTERFACE_GWEN_NAMESPACE_BEGIN();

// ============================================================================================== //

typedef UnitTest Panel;

// ============================================================================================== //

#if 0
class ValueSliderWithLabel : public Gwen::Controls::Base
{
public:

    GWEN_CONTROL(ValueSliderWithLabel, Gwen::Controls::Base);

    virtual void SetRange(float fMin, float fMax);
    virtual void SetValue(float value);
    virtual float GetValue();

    void UpdateValueSlider(bool slider = true, bool textbox = true);

    virtual Gwen::Controls::HorizontalSlider * Slider() { return m_Slider; }
    virtual Gwen::Controls::LabelClickable * Label() { return m_Label; }
    virtual Gwen::Controls::TextBoxNumeric * TextBox() { return m_TextBox; }

    Gwen::Event::Caller	onValueChanged;

protected:

    float GetNormalisedValue(float value);
    float GetRescaledValue(float value);
    void SetValueInternal(float value, bool slider = true, bool textbox = true);
    void AdjustRange(float value);
    void SliderMoved(Base * pControl);
    void ValueChanged(Base * pControl);

private:

    Gwen::Controls::HorizontalSlider * m_Slider;
    Gwen::Controls::LabelClickable	* m_Label;
    Gwen::Controls::TextBoxNumeric * m_TextBox;
    float m_Min;
    float m_Max;
    float m_Value;
};

// ============================================================================================== //

class Panel : public Gwen::Controls::WindowControl
{
public:

    GWEN_CONTROL(Panel, Gwen::Controls::WindowControl);

    void Setup(vd::containers::ParamSet * params);
    void PrintText(const Gwen::UnicodeString & str);

    void Render(Gwen::Skin::Base * skin);

protected:

    void FloatValueChanged(Base * pControl);
    virtual void OnResizedInternal(Gwen::Controls::Base * pControl);

private:

    Gwen::Controls::ScrollControl*	m_ScrollControl;
    Gwen::Controls::ListBox*		m_TextOutput;
    unsigned int					m_iFrames;
    float							m_fLastSecond;
    vd::containers::ParamSet*		m_ParamSet;

};

#endif

// ============================================================================================== //

VD_INTERFACE_GWEN_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_INTERFACE_GWEN_PANEL_INCLUDED 

// ============================================================================================== //
// end file
