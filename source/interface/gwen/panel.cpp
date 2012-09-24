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

#include "panel.h"

#include "core/utilities.h"

#include "Gwen/Platform.h"
#include "Gwen/Controls/Label.h"
#include "Gwen/Controls/Resizer.h"
#include "Gwen/Controls/HorizontalSlider.h"

// ============================================================================================== //

VD_INTERFACE_GWEN_NAMESPACE_BEGIN();

// ============================================================================================== //

#if 0
GWEN_CONTROL_CONSTRUCTOR(ValueSliderWithLabel)
{
    SetSize(250, 19);

    m_Label = new Gwen::Controls::LabelClickable(this);
    m_Label->SetPadding(Gwen::Padding(8, 8, 8, 8));
    m_Label->Dock(Gwen::Pos::Left);
    m_Label->SetAlignment(Gwen::Pos::Right | Gwen::Pos::CenterV);
    m_Label->SetTabable(false);

    m_Slider = new Gwen::Controls::HorizontalSlider(this);
    m_Slider->SetPadding(Gwen::Padding(8, 8, 8, 8));
    m_Slider->Dock(Gwen::Pos::Fill);
    m_Slider->SetRange(0.0f, 1.0f);

    m_TextBox = new Gwen::Controls::TextBoxNumeric(this);
    m_TextBox->SetPadding(Gwen::Padding(8, 8, 8, 8));
    m_TextBox->SetMargin(Gwen::Margin(8, 0, 0, 0));
    m_TextBox->Dock(Gwen::Pos::Right);
    m_TextBox->SetSize(50, 19);

    SetRange(-1.0f, 1.0f);
    SetValue(0.0f);

    m_TextBox->onTextChanged.Add(this, &ValueSliderWithLabel::ValueChanged);
    m_TextBox->onReturnPressed.Add(this, &ValueSliderWithLabel::ValueChanged);
    m_Slider->onValueChanged.Add(this, &ValueSliderWithLabel::SliderMoved);

    SetTabable(false);
}

void ValueSliderWithLabel::SetRange(
    float fMin, float fMax)
{
    fMin = fMin > fMax ? fMax : fMin;
    fMax = fMin > fMax ? fMin : fMax;

    m_Min = fMin;
    m_Max = fMax;
}

void
ValueSliderWithLabel::SetValue(
    float value)
{
    SetValueInternal(value);
}

float
ValueSliderWithLabel::GetValue()
{
    return m_Value;
}

void
ValueSliderWithLabel::UpdateValueSlider(
    bool slider, bool textbox)
{
    if(textbox)
    {
        m_TextBox->SetText(vd::core::ToString(m_Value, 2));
    }

    if(slider)
    {
        m_Slider->SetValue(GetNormalisedValue(m_Value));
    }

    printf("Updating value slider: %f -> %f [%f, %f]\n", m_Value,  GetNormalisedValue(m_Value), m_Min, m_Max);
}

float
ValueSliderWithLabel::GetNormalisedValue(
    float value)
{
    float range = fabs(m_Max - m_Min);
    float offset = m_Min < 0.0f ? -m_Min : m_Min;
    float normalised = (offset + value) / (range);
    return normalised;
}

float
ValueSliderWithLabel::GetRescaledValue(
    float value)
{
    float range = fabs(m_Max - m_Min);
    float offset = m_Min; // < 0.0f ? -m_Min : m_Min;
    float rescaled = offset + (value * (range));
    return rescaled;
}

void
ValueSliderWithLabel::SetValueInternal(
    float value, bool slider, bool textbox)
{
    if(m_Value != value)
    {
        m_Value = value;
        AdjustRange(m_Value);
        onValueChanged.Call(this);
        UpdateValueSlider(slider, textbox);
    }
}

void
ValueSliderWithLabel::AdjustRange(
    float value)
{
    if(value > m_Max) { m_Max = +1.5f * value; }

    if(value < m_Min) { m_Min = -1.5f * value; }
}

void
ValueSliderWithLabel::SliderMoved(
    Base * pControl)
{
    Gwen::Controls::Slider * pSlider = (Gwen::Controls::Slider *)pControl;
    float rescaled = GetRescaledValue(pSlider->GetValue());
    SetValueInternal(rescaled, false, true);
}

void
ValueSliderWithLabel::ValueChanged(
    Base * pControl)
{
    Gwen::Controls::TextBoxNumeric * pTextBox = (Gwen::Controls::TextBoxNumeric *)pControl;
    SetValueInternal(pTextBox->GetFloatFromText(), true, false);
}

// ============================================================================================== //

GWEN_CONTROL_CONSTRUCTOR(Panel)
{
    SetTitle(L"Void");

    SetSize(450, 450);

    m_ScrollControl = new Gwen::Controls::ScrollControl(this);
    m_ScrollControl->Dock(Pos::Fill);
    m_ScrollControl->SetScroll(false, true);
    m_ScrollControl->SetAutoHideBars(true);
    m_ScrollControl->SetMargin(Margin(8, 8, 8, 8));

    m_InnerPanel = m_ScrollControl;

    m_fLastSecond = Gwen::Platform::GetTimeInSeconds();
    m_iFrames = 0;
}

void
Panel::Setup(
    ParamSet * params)
{
    ValueSliderWithLabel * previous = 0;
    m_ParamSet = params;
	#if 0
    	for(int i = 0; i < vd::core::FloatParamCount; ++i)
    	{
    		vd::core::Parameter<float>& fp = params->GetFloatParam((vd::core::FloatParam)i);
    		if(!fp.Name.length())
    			continue;

    		ValueSliderWithLabel* widget = new ValueSliderWithLabel( m_ScrollControl );
    //		widget->SetRange( (fp.Value) - 2.0f * (fp.Value), (fp.Value) + 2.0f * (fp.Value) );
    		widget->SetValue( fp.Value );
    		widget->SetUserData( &fp );
    		widget->Label()->SetText( fp.Name );
    		widget->SetMargin(  Margin( 8, 8, 8, 8 ) );
    		widget->SetSize( this->Width() - m_ScrollControl->Width() * 4 - 8, 19 );
    		widget->onValueChanged.Add( this, &Panel::FloatValueChanged );
    //		widget->UpdateValueSlider();
    		if(previous)
    			Gwen::Align::PlaceBelow( widget, previous, 10 );

    		previous = widget;
    	}
	#endif
}

void
Panel::PrintText(
    const Gwen::UnicodeString & str)
{
    //	m_TextOutput->AddItem( str );
    //	m_TextOutput->Scroller()->ScrollToBottom();
}

void
Panel::Render(
    Gwen::Skin::Base * skin)
{
    m_iFrames++;

    if(m_fLastSecond < Gwen::Platform::GetTimeInSeconds())
    {
        SetTitle(Gwen::Utility::Format(L"Void - %i fps", m_iFrames));

        m_fLastSecond = Gwen::Platform::GetTimeInSeconds() + 1.0f;
        m_iFrames = 0;
    }

    BaseClass::Render(skin);

}

void
Panel::FloatValueChanged(
    Base * pControl)
{
    ValueSliderWithLabel * pSlider = (ValueSliderWithLabel *)pControl;
    float value = pSlider->GetValue();
    Parameter<float>* fp = (Parameter<float>*)pSlider->GetUserData();

    if(fp && fp->Value != value)
    {
        fp->Value = value;
        fp->Changed = true;
        pSlider->UpdateValueSlider();
    }
}

void
Panel::OnResizedInternal(
    Gwen::Controls::Base * pControl)
{
    onResize.Call(this);
    OnResized();
}

#endif

// ============================================================================================== //

VD_INTERFACE_GWEN_NAMESPACE_END();

// ============================================================================================== //
// end file

