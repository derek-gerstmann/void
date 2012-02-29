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

#ifndef VD_INTERFACE_GWEN_SKIN_INCLUDED
#define VD_INTERFACE_GWEN_SKIN_INCLUDED

// ============================================================================================== //

#include "interface/gwen/gwen.h"
#include "Gwen/Skins/Simple.h"

// ============================================================================================== //

VD_INTERFACE_GWEN_NAMESPACE_BEGIN();

// ============================================================================================== //

enum SkinColors
{
    BackgroundColor,
    BackgroundColorDark,
    BaseColor,
    BaseDarkColor,
    BaseDarkerColor,
    BaseFillColor,
    BaseHoverDarkColor,
    BaseLightColor,
    BaseOutlineColor,
    BaseOutlineLightColor,
    BaseOutlineLighterColor,
    ButtonColor,
    ButtonDarkColor,
    ButtonDarkerColor,
    ButtonHoverDarkColor,
    ButtonLightColor,
    ButtonOutlineColor,
    ButtonOutlineLightColor,
    ButtonOutlineLighterColor,
    ButtonPushedColor,
    BorderColor,
    BorderHighlight,
    CheckBoxColor,
    HighlightColor,
    HighlightOutlineColor,
    HoverColor,
    HoverDarkColor,
    HoverLightColor,
    HoverOutlineColor,
    ListBoxColor,
    MenuColor,
    MenuLightColor,
    ModalColor,
    ProgressBarColor,
    RadioButtonColor,
    ScrollButtonColor,
    ScrollBarColor,
    ScrollBarDarkColor,
    ScrollBarLightColor,
    ScrollBarOutlineColor,
    SelectedColor,
    SelectedOutlineColor,
    GlossColor,
    GlossLightColor,
    TabColor,
    TextBoxColor,
    ToolTipBackgroundColor,
    ToolTipOutlineColor,
    WindowActiveColor,
    WindowBorderColor,
    WindowColor,
    SkinColorCount
};

class Skin : public Skins::Simple
{
public:
#define LU(x) (x), (x), (x)
#define RD(x) 255, 0, 0, 255
#define GN(x)   0, 255, 0, 255
#define BL(x)   0, 0, 255, 255

    Gwen::Color m_Colors[SkinColorCount];

    Skin() : Skins::Simple()
    {
        char alpha = 128;
        char light = 0.03f * alpha;
        char hover = 0.2f * alpha;

        hover = hover < 50 ? 50 : hover;
        light = light < 10 ? 10 : light;


        m_Colors[BorderColor]				= Gwen::Color(LU(28), alpha);
        m_Colors[BackgroundColor]			= Gwen::Color(LU(5), 0);
        m_Colors[BackgroundColorDark]		= Gwen::Color(LU(0), 0);

        m_Colors[BaseColor]					= Gwen::Color(LU(55), alpha);
        m_Colors[BaseFillColor]				= Gwen::Color(LU(46), alpha);
        m_Colors[BaseLightColor]			= Gwen::Color(LU(60), light);
        m_Colors[BaseDarkColor]				= Gwen::Color(LU(42), alpha);
        m_Colors[BaseDarkerColor]			= Gwen::Color(LU(36), alpha);
        m_Colors[BaseOutlineColor]			= Gwen::Color(LU(8), alpha);
        m_Colors[BaseOutlineLightColor]		= Gwen::Color(LU(16), alpha);
        m_Colors[BaseOutlineLighterColor] 	= Gwen::Color(LU(40), alpha);
        m_Colors[TabColor]					= Gwen::Color(LU(46), alpha);

        m_Colors[ButtonColor]				= Gwen::Color(LU(66), alpha);
        m_Colors[ButtonLightColor]			= Gwen::Color(LU(72), light);
        m_Colors[ButtonDarkColor]			= Gwen::Color(LU(44), alpha);
        m_Colors[ButtonDarkerColor]			= Gwen::Color(LU(32), alpha);
        m_Colors[ButtonOutlineColor]		= Gwen::Color(LU(4), alpha);
        m_Colors[ButtonOutlineLightColor]	= Gwen::Color(LU(8), light);
        m_Colors[ButtonOutlineLighterColor] = Gwen::Color(LU(16), light);
        m_Colors[ButtonPushedColor]			= Gwen::Color(46, 115, 215, hover);

        m_Colors[ScrollBarColor]			= Gwen::Color(LU(46), alpha);
        m_Colors[ScrollBarLightColor]		= Gwen::Color(LU(52), light);
        m_Colors[ScrollBarDarkColor]		= Gwen::Color(LU(36), alpha);
        m_Colors[ScrollBarOutlineColor]		= Gwen::Color(LU(28), alpha);

        m_Colors[GlossColor]				= Gwen::Color(255, 255, 255, 10);
        m_Colors[GlossLightColor]			= Gwen::Color(255, 255, 255, 20);
        m_Colors[HighlightColor]			= Gwen::Color(46, 115, 215, hover);
        m_Colors[HighlightOutlineColor]		= Gwen::Color(46, 115, 215, alpha);

        m_Colors[ToolTipBackgroundColor]	= Gwen::Color(46, 115, 215, alpha);
        m_Colors[ToolTipOutlineColor]		= Gwen::Color(46, 115, 215, alpha);
        m_Colors[ScrollButtonColor]			= Gwen::Color(0,   0,   0, light);
        m_Colors[ModalColor] 				= Gwen::Color(25,  25,  25, alpha);
        m_Colors[SelectedColor]				= Gwen::Color(46, 115, 215, hover);
        m_Colors[SelectedOutlineColor]		= Gwen::Color(46, 115, 215, alpha);
        m_Colors[HoverColor]				= Gwen::Color(46, 115, 215, hover);
        m_Colors[HoverDarkColor]			= Gwen::Color(46 * 0.2f, 115 * 0.2f, 215 * 0.3f, hover);
        m_Colors[HoverLightColor]			= Gwen::Color(46 * 1.2f, 115 * 1.2f, 255, hover);
        m_Colors[HoverOutlineColor] 		= Gwen::Color(46 * 0.5f, 115 * 0.5f, 215 * 0.5f, hover);
        m_Colors[RadioButtonColor]			= Gwen::Color(46, 115, 215,  hover);
        m_Colors[MenuLightColor]			= Gwen::Color(LU(8), light);
        m_Colors[MenuColor]					= Gwen::Color(LU(4), hover);
        m_Colors[ProgressBarColor]			= Gwen::Color(46, 115, 215, hover);
        m_Colors[WindowActiveColor]			= Gwen::Color(LU(16), alpha);
        m_Colors[WindowColor]				= Gwen::Color(LU(32), alpha);
        m_Colors[WindowBorderColor]			= Gwen::Color(LU(15), alpha);
        m_Colors[TextBoxColor]				= Gwen::Color(LU(34), 255);
        m_Colors[ListBoxColor]				= Gwen::Color(LU(34), 255);
        m_Colors[CheckBoxColor]				= Gwen::Color(LU(34), 255);
        
		m_colBorderColor			= m_Colors[BaseOutlineColor];
		m_colBG						= m_Colors[BackgroundColor];
		m_colBGDark					= m_Colors[BackgroundColorDark];

		m_colControl				= m_Colors[BaseColor];
		m_colControlBright			= m_Colors[BaseLightColor];
		m_colControlDark			= m_Colors[BaseDarkColor];
		m_colControlDarker			= m_Colors[BaseDarkerColor];

		m_colControlOutlineNormal	= m_Colors[BaseOutlineColor];
		m_colControlOutlineLight	= m_Colors[BaseOutlineLightColor];
		m_colControlOutlineLighter	= m_Colors[BaseOutlineLighterColor];

		m_colHighlightBG			= m_Colors[HighlightColor];
		m_colHighlightBorder		= m_Colors[HighlightOutlineColor];

		m_colToolTipBackground		= m_Colors[ToolTipBackgroundColor];
		m_colToolTipBorder			= m_Colors[HighlightOutlineColor];

		m_colModal = Gwen::Color( 25, 25, 25, 150 );

        m_DefaultFont.facename	= L"Vera.ttf";
        m_DefaultFont.size		= 12;
    }

    virtual void Init(const Gwen::TextObject& TextureName)
    {
/*
        char alpha = 128;
        char light = 0.03f * alpha;
        char hover = 0.2f * alpha;
        char text = 255 * 0.8f;

        hover = hover < 50 ? 50 : hover;
        light = light < 10 ? 10 : light;

        m_DefaultFont.facename	= L"titillium.otf";
        m_DefaultFont.size		= 11;

        m_Texture.Load(TextureName, GetRender());

        Colors.Window.TitleActive 		= Gwen::Color(LU(200), text);
        Colors.Window.TitleInactive		= Gwen::Color(LU(120), text);
        Colors.Button.Normal			= Gwen::Color(LU(132), alpha);
        Colors.Button.Hover				= Gwen::Color(LU(255), hover);
        Colors.Button.Down				= Gwen::Color(LU(148), hover);
        Colors.Button.Disabled			= Gwen::Color(LU(144), alpha);

        Colors.Tab.Active.Normal		= Gwen::Color(LU(146), hover);
        Colors.Tab.Active.Hover			= Gwen::Color(LU(255), hover);
        Colors.Tab.Active.Down			= Gwen::Color(LU(180), hover);
        Colors.Tab.Active.Disabled		= Gwen::Color(LU(128), hover);
        Colors.Tab.Inactive.Normal		= Gwen::Color(LU(132), hover);
        Colors.Tab.Inactive.Hover		= Gwen::Color(LU(255), hover);
        Colors.Tab.Inactive.Down		= Gwen::Color(LU(116), hover);
        Colors.Tab.Inactive.Disabled	= Gwen::Color(LU(8), alpha);

        Colors.Label.Default			= Gwen::Color(LU(164), text);
        Colors.Label.Bright				= Gwen::Color(LU(200), text);
        Colors.Label.Dark				= Gwen::Color(LU(132), text);
        Colors.Label.Highlight			= Gwen::Color(LU(255), text);

        Colors.Tree.Lines				= Gwen::Color(46, 115, 215, hover);
        Colors.Tree.Normal				= Gwen::Color(LU(128), alpha);
        Colors.Tree.Hover				= Gwen::Color(46 * 0.2f, 115 * 0.2f, 215 * 0.3f, hover);
        Colors.Tree.Selected			= Gwen::Color(46, 115, 215, hover);

        Colors.Properties.Line_Normal		= Gwen::Color(LU(32), alpha);
        Colors.Properties.Line_Selected		= Gwen::Color(LU(46), hover);
        Colors.Properties.Line_Hover		= Gwen::Color(LU(48), hover);
        Colors.Properties.Title				= Gwen::Color(LU(164), alpha);

        Colors.Properties.Column_Normal		= Gwen::Color(LU(72), alpha);
        Colors.Properties.Column_Selected	= Gwen::Color(LU(80), hover);
        Colors.Properties.Column_Hover		= Gwen::Color(LU(128), hover);
        Colors.Properties.Border			= Gwen::Color(LU(8), alpha);

        Colors.Properties.Label_Normal		= Gwen::Color(LU(148), hover);
        Colors.Properties.Label_Selected	= Gwen::Color(LU(164), hover);
        Colors.Properties.Label_Hover		= Gwen::Color(LU(132), hover);

        Colors.ModalBackground				= Gwen::Color(LU(32), alpha);
        Colors.TooltipText					= Gwen::Color(LU(255), text);
        Colors.Category.Header				= Gwen::Color(LU(164), alpha);
        Colors.Category.Header_Closed		= Gwen::Color(LU(132), alpha);

        Colors.Category.Line.Text			= Gwen::Color(LU(200), text);
        Colors.Category.Line.Text_Hover		= Gwen::Color(LU(225), text);
        Colors.Category.Line.Text_Selected	= Gwen::Color(LU(255), text);
        Colors.Category.Line.Button			= Gwen::Color(LU(32), alpha);
        Colors.Category.Line.Button_Hover	= Gwen::Color(LU(48), alpha);
        Colors.Category.Line.Button_Selected = Gwen::Color(LU(128), alpha);
        
        Colors.Category.LineAlt.Text			= Gwen::Color(LU(200), text);
        Colors.Category.LineAlt.Text_Hover		= Gwen::Color(LU(225), text);
        Colors.Category.LineAlt.Text_Selected	= Gwen::Color(LU(132), text);
        Colors.Category.LineAlt.Button			= Gwen::Color(LU(37), alpha);
        Colors.Category.LineAlt.Button_Hover	= Gwen::Color(LU(53), alpha);
        Colors.Category.LineAlt.Button_Selected	= Gwen::Color(LU(133), alpha);

        Textures.Shadow.Init(&m_Texture, 448, 0,	31,		31,		Gwen::Margin(8, 8, 8, 8));
        Textures.Tooltip.Init(&m_Texture, 128, 320,	127,	31,		Gwen::Margin(8, 8, 8, 8));
        Textures.StatusBar.Init(&m_Texture, 128, 288, 127, 31, Gwen::Margin(8, 8, 8, 8));
        Textures.Selection.Init(&m_Texture, 384, 32, 31, 31, Gwen::Margin(4, 4, 4, 4));

        Textures.Panel.Normal.Init(&m_Texture, 256,		0,	63,	63,		Gwen::Margin(16,	16, 16, 16));
        Textures.Panel.Bright.Init(&m_Texture, 256 + 64,	0,	63,	63,		Gwen::Margin(16,	16, 16, 16));
        Textures.Panel.Dark.Init(&m_Texture, 256,		64,	63,	63,		Gwen::Margin(16,	16, 16, 16));
        Textures.Panel.Highlight.Init(&m_Texture, 256 + 64,	64,	63,	63,		Gwen::Margin(16,	16, 16, 16));

        Textures.Window.Normal.Init(&m_Texture, 0, 0, 127,		127,	Gwen::Margin(8, 32, 8, 8));
        Textures.Window.Inactive.Init(&m_Texture, 128, 0, 127,		127,	Gwen::Margin(8, 32, 8, 8));

        Textures.Checkbox.Active.Checked.Init(&m_Texture, 448, 32, 15, 15);
        Textures.Checkbox.Active.Normal.Init(&m_Texture, 464, 32, 15, 15);
        Textures.Checkbox.Disabled.Normal.Init(&m_Texture, 448, 48, 15, 15);
        Textures.Checkbox.Disabled.Normal.Init(&m_Texture, 464, 48, 15, 15);

        Textures.RadioButton.Active.Checked.Init(&m_Texture, 448, 64, 15, 15);
        Textures.RadioButton.Active.Normal.Init(&m_Texture, 464, 64, 15, 15);
        Textures.RadioButton.Disabled.Normal.Init(&m_Texture, 448, 80, 15, 15);
        Textures.RadioButton.Disabled.Normal.Init(&m_Texture, 464, 80, 15, 15);

        Textures.TextBox.Normal.Init(&m_Texture, 0, 150, 127, 21, Gwen::Margin(4, 4, 4, 4));
        Textures.TextBox.Focus.Init(&m_Texture, 0, 172, 127, 21, Gwen::Margin(4, 4, 4, 4));
        Textures.TextBox.Disabled.Init(&m_Texture, 0, 193, 127, 21, Gwen::Margin(4, 4, 4, 4));

        Textures.Menu.Strip.Init(&m_Texture, 0, 128, 127, 21, Gwen::Margin(1, 1, 1, 1));
        Textures.Menu.BackgroundWithMargin.Init(&m_Texture, 128, 128, 127, 63, Gwen::Margin(24, 8, 8, 8));
        Textures.Menu.Background.Init(&m_Texture, 128, 192, 127, 63, Gwen::Margin(8, 8, 8, 8));
        Textures.Menu.Hover.Init(&m_Texture, 128, 256, 127, 31, Gwen::Margin(8, 8, 8, 8));
        Textures.Menu.RightArrow.Init(&m_Texture, 464, 112, 15, 15);
        Textures.Menu.Check.Init(&m_Texture, 448, 112, 15, 15);

        Textures.Tab.Control.Init(&m_Texture, 0, 256, 127, 127, Gwen::Margin(8, 8, 8, 8));
        Textures.Tab.Bottom.Active.Init(&m_Texture, 0,		416, 63, 31, Gwen::Margin(8, 8, 8, 8));
        Textures.Tab.Bottom.Inactive.Init(&m_Texture, 0 + 128,	416, 63, 31, Gwen::Margin(8, 8, 8, 8));
        Textures.Tab.Top.Active.Init(&m_Texture, 0,		384, 63, 31, Gwen::Margin(8, 8, 8, 8));
        Textures.Tab.Top.Inactive.Init(&m_Texture, 0 + 128,	384, 63, 31, Gwen::Margin(8, 8, 8, 8));
        Textures.Tab.Left.Active.Init(&m_Texture, 64,		384, 31, 63, Gwen::Margin(8, 8, 8, 8));
        Textures.Tab.Left.Inactive.Init(&m_Texture, 64 + 128,	384, 31, 63, Gwen::Margin(8, 8, 8, 8));
        Textures.Tab.Right.Active.Init(&m_Texture, 96,		384, 31, 63, Gwen::Margin(8, 8, 8, 8));
        Textures.Tab.Right.Inactive.Init(&m_Texture, 96 + 128,	384, 31, 63, Gwen::Margin(8, 8, 8, 8));
        Textures.Tab.HeaderBar.Init(&m_Texture, 128, 352, 127, 31, Gwen::Margin(4, 4, 4, 4));

        Textures.Window.Close.Init(&m_Texture, 0, 224, 24, 24);
        Textures.Window.Close_Hover.Init(&m_Texture, 32, 224, 24, 24);
        Textures.Window.Close_Down.Init(&m_Texture, 64, 224, 24, 24);
        Textures.Window.Close_Disabled.Init(&m_Texture, 96, 224, 24, 24);

        Textures.Tree.Background.Init(&m_Texture, 256, 128, 127,	127,	Gwen::Margin(16, 16, 16, 16));
        Textures.Tree.Plus.Init(&m_Texture, 448, 96, 15, 15);
        Textures.Tree.Minus.Init(&m_Texture, 464, 96, 15, 15);

        Textures.Input.Button.Normal.Init(&m_Texture, 480, 0,	31,		31,		Gwen::Margin(8, 8, 8, 8));
        Textures.Input.Button.Hovered.Init(&m_Texture, 480, 32,	31,		31,		Gwen::Margin(8, 8, 8, 8));
        Textures.Input.Button.Disabled.Init(&m_Texture, 480, 64,	31,		31,		Gwen::Margin(8, 8, 8, 8));
        Textures.Input.Button.Pressed.Init(&m_Texture, 480, 96,	31,		31,		Gwen::Margin(8, 8, 8, 8));

        for(int i = 0; i < 4; i++)
        {
            Textures.Scroller.Button.Normal[i].Init(&m_Texture, 464 + 0, 208 + i * 16, 15, 15, Gwen::Margin(2, 2, 2, 2));
            Textures.Scroller.Button.Hover[i].Init(&m_Texture, 480, 208 + i * 16, 15, 15, Gwen::Margin(2, 2, 2, 2));
            Textures.Scroller.Button.Down[i].Init(&m_Texture, 464, 272 + i * 16, 15, 15, Gwen::Margin(2, 2, 2, 2));
            Textures.Scroller.Button.Disabled[i].Init(&m_Texture, 480 + 48, 272 + i * 16, 15, 15, Gwen::Margin(2, 2, 2, 2));
        }

        Textures.Scroller.TrackV.Init(&m_Texture, 384,			208, 15, 127, Gwen::Margin(4, 4, 4, 4));
        Textures.Scroller.ButtonV_Normal.Init(&m_Texture, 384 + 16,		208, 15, 127, Gwen::Margin(4, 4, 4, 4));
        Textures.Scroller.ButtonV_Hover.Init(&m_Texture, 384 + 32,		208, 15, 127, Gwen::Margin(4, 4, 4, 4));
        Textures.Scroller.ButtonV_Down.Init(&m_Texture, 384 + 48,		208, 15, 127, Gwen::Margin(4, 4, 4, 4));
        Textures.Scroller.ButtonV_Disabled.Init(&m_Texture, 384 + 64,		208, 15, 127, Gwen::Margin(4, 4, 4, 4));

        Textures.Scroller.TrackH.Init(&m_Texture, 384,	128,		127, 15, Gwen::Margin(4, 4, 4, 4));
        Textures.Scroller.ButtonH_Normal.Init(&m_Texture, 384,	128 + 16,	127, 15, Gwen::Margin(4, 4, 4, 4));
        Textures.Scroller.ButtonH_Hover.Init(&m_Texture, 384,	128 + 32,	127, 15, Gwen::Margin(4, 4, 4, 4));
        Textures.Scroller.ButtonH_Down.Init(&m_Texture, 384,	128 + 48,	127, 15, Gwen::Margin(4, 4, 4, 4));
        Textures.Scroller.ButtonH_Disabled.Init(&m_Texture, 384,	128 + 64,	127, 15, Gwen::Margin(4, 4, 4, 4));



        Textures.Input.ListBox.Background.Init(&m_Texture, 256,	256, 63, 127, Gwen::Margin(8, 8, 8, 8));
        Textures.Input.ListBox.Hovered.Init(&m_Texture, 320,	320, 31, 31, Gwen::Margin(8, 8, 8, 8));
        Textures.Input.ListBox.EvenLine.Init(&m_Texture, 352,  256, 31, 31, Gwen::Margin(8, 8, 8, 8));
        Textures.Input.ListBox.OddLine.Init(&m_Texture, 352,  288, 31, 31, Gwen::Margin(8, 8, 8, 8));
        Textures.Input.ListBox.EvenLineSelected.Init(&m_Texture, 320,	270, 31, 31, Gwen::Margin(8, 8, 8, 8));
        Textures.Input.ListBox.OddLineSelected.Init(&m_Texture, 320,	288, 31, 31, Gwen::Margin(8, 8, 8, 8));

        Textures.Input.ComboBox.Normal.Init(&m_Texture, 384,	336,	127, 31, Gwen::Margin(8, 8, 32, 8));
        Textures.Input.ComboBox.Hover.Init(&m_Texture, 384,	336 + 32, 127, 31, Gwen::Margin(8, 8, 32, 8));
        Textures.Input.ComboBox.Down.Init(&m_Texture, 384,	336 + 64, 127, 31, Gwen::Margin(8, 8, 32, 8));
        Textures.Input.ComboBox.Disabled.Init(&m_Texture, 384,	336 + 96, 127, 31, Gwen::Margin(8, 8, 32, 8));

        Textures.Input.ComboBox.Button.Normal.Init(&m_Texture, 496,	272,	15, 15);
        Textures.Input.ComboBox.Button.Hover.Init(&m_Texture, 496,	272 + 16, 15, 15);
        Textures.Input.ComboBox.Button.Down.Init(&m_Texture, 496,	272 + 32, 15, 15);
        Textures.Input.ComboBox.Button.Disabled.Init(&m_Texture, 496,	272 + 48, 15, 15);

        Textures.Input.UpDown.Up.Normal.Init(&m_Texture, 384,		112,	7, 7);
        Textures.Input.UpDown.Up.Hover.Init(&m_Texture, 384 + 8,	112,	7, 7);
        Textures.Input.UpDown.Up.Down.Init(&m_Texture, 384 + 16,	112,	7, 7);
        Textures.Input.UpDown.Up.Disabled.Init(&m_Texture, 384 + 24,	112,	7, 7);

        Textures.Input.UpDown.Down.Normal.Init(&m_Texture, 384,		120,	7, 7);
        Textures.Input.UpDown.Down.Hover.Init(&m_Texture, 384 + 8,	120,	7, 7);
        Textures.Input.UpDown.Down.Down.Init(&m_Texture, 384 + 16,	120,	7, 7);
        Textures.Input.UpDown.Down.Disabled.Init(&m_Texture, 384 + 24,	120,	7, 7);

        Textures.ProgressBar.Back.Init(&m_Texture, 384,	0, 31, 31, Gwen::Margin(8, 8, 8, 8));
        Textures.ProgressBar.Front.Init(&m_Texture, 384 + 32,	0, 31, 31, Gwen::Margin(8, 8, 8, 8));

        Textures.Input.Slider.H.Normal.Init(&m_Texture, 416,	32,	15, 15);
        Textures.Input.Slider.H.Hover.Init(&m_Texture, 416,	32 + 16, 15, 15);
        Textures.Input.Slider.H.Down.Init(&m_Texture, 416,	32 + 32, 15, 15);
        Textures.Input.Slider.H.Disabled.Init(&m_Texture, 416,	32 + 48, 15, 15);

        Textures.Input.Slider.V.Normal.Init(&m_Texture, 416 + 16,	32,	15, 15);
        Textures.Input.Slider.V.Hover.Init(&m_Texture, 416 + 16,	32 + 16, 15, 15);
        Textures.Input.Slider.V.Down.Init(&m_Texture, 416 + 16,	32 + 32, 15, 15);
        Textures.Input.Slider.V.Disabled.Init(&m_Texture, 416 + 16,	32 + 48, 15, 15);

        Textures.CategoryList.Outer.Init(&m_Texture, 256,			384, 63, 63, Gwen::Margin(8, 8, 8, 8));
        Textures.CategoryList.Inner.Init(&m_Texture, 256 + 64,	384, 63, 63, Gwen::Margin(8, 21, 8, 8));
        Textures.CategoryList.Header.Init(&m_Texture, 320,			352, 63, 31, Gwen::Margin(8, 8, 8, 8));
*/
    }

        virtual void DrawButton(Gwen::Controls::Base * control, bool bDepressed, bool bHovered)
        {
            int w = control->Width();
            int h = control->Height();

            DrawButton(w, h, bDepressed, bHovered);
        }

        virtual void DrawButton(int w, int h, bool bDepressed, bool bHovered, bool bSquared = false)
        {
            if(bDepressed)	m_Render->SetDrawColor(m_Colors[ButtonPushedColor]);
            else if(bHovered)m_Render->SetDrawColor(m_Colors[HoverLightColor]);
            else				m_Render->SetDrawColor(m_Colors[ButtonColor]);

            m_Render->DrawFilledRect(Gwen::Rect(1, 1, w - 2, h - 2));

            if(bDepressed)	m_Render->SetDrawColor(m_Colors[ButtonPushedColor]);
            else if(bHovered)m_Render->SetDrawColor(m_Colors[HoverColor]);
            else				m_Render->SetDrawColor(m_Colors[ButtonDarkColor]);

            m_Render->DrawFilledRect(Gwen::Rect(1, h * 0.5, w - 2, h * 0.5 - 2));

            if(!bDepressed)
            {
                m_Render->SetDrawColor(m_Colors[ButtonLightColor]);
                m_Render->DrawShavedCornerRect(Gwen::Rect(1, 1, w - 2, h - 2), bSquared);
            }
            else
            {
                m_Render->SetDrawColor(m_Colors[ButtonPushedColor]);
                m_Render->DrawShavedCornerRect(Gwen::Rect(1, 1, w - 2, h - 2), bSquared);
            }

            // Border
            m_Render->SetDrawColor(m_Colors[ButtonOutlineColor]);
            m_Render->DrawShavedCornerRect(Gwen::Rect(0, 0, w, h), bSquared);
        }

        virtual void DrawMenuItem(Gwen::Controls::Base * control, bool bSubmenuOpen, bool bChecked)
        {
            if(bSubmenuOpen || control->IsHovered())
            {
                m_Render->SetDrawColor(m_Colors[HighlightColor]);
                m_Render->DrawFilledRect(control->GetRenderBounds());

                m_Render->SetDrawColor(m_Colors[HighlightOutlineColor]);
                m_Render->DrawLinedRect(control->GetRenderBounds());
            }

            Gwen::Rect rect = control->GetRenderBounds();

            if(bChecked)
            {
                m_Render->SetDrawColor(Gwen::Color(0, 0, 0, 255));

                Gwen::Rect r(control->Width() / 2 - 2, control->Height() / 2 - 2, 5, 5);
                DrawCheck(r);
            }
        }

        virtual void DrawMenuStrip(Gwen::Controls::Base * control)
        {
            int w = control->Width();
            int h = control->Height();

            m_Render->SetDrawColor(m_Colors[MenuLightColor]);
            m_Render->DrawFilledRect(Gwen::Rect(0, 0, w, h));

            m_Render->SetDrawColor(m_Colors[MenuColor]);

            m_Render->DrawFilledRect(Gwen::Rect(0, h * 0.4f, w, h * 0.6f));
            m_Render->DrawFilledRect(Gwen::Rect(0, h * 0.5f, w, h * 0.5f));

        }

        virtual void DrawMenu(Gwen::Controls::Base * control, bool bPaddingDisabled)
        {
            int w = control->Width();
            int h = control->Height();

            m_Render->SetDrawColor(m_Colors[MenuColor]);
            m_Render->DrawFilledRect(Gwen::Rect(0, 0, w, h));

            if(!bPaddingDisabled)
            {
                m_Render->SetDrawColor(m_Colors[MenuLightColor]);
                m_Render->DrawFilledRect(Gwen::Rect(1, 0, 22, h));
            }

            m_Render->SetDrawColor(m_Colors[BaseOutlineColor]);
            m_Render->DrawLinedRect(Gwen::Rect(0, 0, w, h));
        }

        virtual void DrawShadow(Gwen::Controls::Base * control)
        {
            int w = control->Width();
            int h = control->Height();

            int x = 4;
            int y = 6;

            m_Render->SetDrawColor(Gwen::Color(0, 0, 0, 10));

            m_Render->DrawFilledRect(Gwen::Rect(x, y, w, h));
            x += 2;
            m_Render->DrawFilledRect(Gwen::Rect(x, y, w, h));
            y += 2;
            m_Render->DrawFilledRect(Gwen::Rect(x, y, w, h));

        }

/*
        virtual void DrawButton(int w, int h, bool bDepressed, bool bHovered, bool bSquared = false)
        {
            if(bDepressed)	m_Render->SetDrawColor(m_Colors[ButtonPushedColor]);
            else if(bHovered)m_Render->SetDrawColor(m_Colors[HoverLightColor]);
            else				m_Render->SetDrawColor(m_Colors[ButtonColor]);

            m_Render->DrawFilledRect(Gwen::Rect(1, 1, w - 2, h - 2));

            if(bDepressed)	m_Render->SetDrawColor(m_Colors[ButtonPushedColor]);
            else if(bHovered)m_Render->SetDrawColor(m_Colors[HoverColor]);
            else				m_Render->SetDrawColor(m_Colors[ButtonDarkColor]);

            m_Render->DrawFilledRect(Gwen::Rect(1, h * 0.5, w - 2, h * 0.5 - 2));

            if(!bDepressed)
            {
                m_Render->SetDrawColor(m_Colors[ButtonLightColor]);
                m_Render->DrawShavedCornerRect(Gwen::Rect(1, 1, w - 2, h - 2), bSquared);
            }
            else
            {
                m_Render->SetDrawColor(m_Colors[ButtonPushedColor]);
                m_Render->DrawShavedCornerRect(Gwen::Rect(1, 1, w - 2, h - 2), bSquared);
            }

            // Border
            m_Render->SetDrawColor(m_Colors[ButtonOutlineColor]);
            m_Render->DrawShavedCornerRect(Gwen::Rect(0, 0, w, h), bSquared);
        }
*/
        virtual void DrawRadioButton(Gwen::Controls::Base * control, bool bSelected, bool bDepressed)
        {
            Gwen::Rect rect = control->GetRenderBounds();

            // Inside colour
            if(control->IsHovered())	m_Render->SetDrawColor(m_Colors[HoverColor]);
            else m_Render->SetDrawColor(m_Colors[ButtonLightColor]);

            m_Render->DrawFilledRect(Gwen::Rect(1, 1, rect.w - 2, rect.h - 2));

            // Border
            if(control->IsHovered())	m_Render->SetDrawColor(m_Colors[HoverOutlineColor]);
            else m_Render->SetDrawColor(m_Colors[ButtonOutlineLightColor]);

            m_Render->DrawShavedCornerRect(rect);

            m_Render->SetDrawColor(m_Colors[RadioButtonColor]);
            m_Render->DrawFilledRect(Gwen::Rect(rect.x + 2, rect.y + 2, rect.w - 4, rect.h - 4));
            m_Render->DrawFilledRect(Gwen::Rect(rect.x + 2, rect.y + 2, rect.w * 0.3f, rect.h - 4));
            m_Render->DrawFilledRect(Gwen::Rect(rect.x + 2, rect.y + 2, rect.w - 4, rect.h * 0.3f));

            if(control->IsHovered())	m_Render->SetDrawColor(m_Colors[HoverDarkColor]);
            else m_Render->SetDrawColor(m_Colors[RadioButtonColor]);

            m_Render->DrawFilledRect(Gwen::Rect(rect.x + 2, rect.y + 3, 1, rect.h - 5));
            m_Render->DrawFilledRect(Gwen::Rect(rect.x + 3, rect.y + 2, rect.w - 5, 1));


            if(bSelected)
            {
                m_Render->SetDrawColor(m_Colors[SelectedColor]);
                m_Render->DrawFilledRect(Gwen::Rect(rect.x + 2, rect.y + 2, rect.w - 4, rect.h - 4));
            }
        }


        virtual void DrawCheckBox(Gwen::Controls::Base * control, bool bSelected, bool bDepressed)
        {
            Gwen::Rect rect = control->GetRenderBounds();

            // Inside colour
            if(control->IsHovered())	m_Render->SetDrawColor(m_Colors[HoverColor]);
            else m_Render->SetDrawColor(m_Colors[ButtonLightColor]);

            m_Render->DrawFilledRect(rect);

            // Border
            if(control->IsHovered())	m_Render->SetDrawColor(m_Colors[HoverOutlineColor]);
            else m_Render->SetDrawColor(m_Colors[ButtonOutlineLightColor]);

            m_Render->DrawLinedRect(rect);

            m_Render->SetDrawColor(m_Colors[CheckBoxColor]);
            m_Render->DrawFilledRect(Gwen::Rect(rect.x + 2, rect.y + 2, rect.w - 4, rect.h - 4));
            m_Render->DrawFilledRect(Gwen::Rect(rect.x + 2, rect.y + 2, rect.w * 0.3f, rect.h - 4));
            m_Render->DrawFilledRect(Gwen::Rect(rect.x + 2, rect.y + 2, rect.w - 4, rect.h * 0.3f));

            if(control->IsHovered())	m_Render->SetDrawColor(m_Colors[HoverDarkColor]);
            else m_Render->SetDrawColor(m_Colors[CheckBoxColor]);

            m_Render->DrawFilledRect(Gwen::Rect(rect.x + 2, rect.y + 2, 1, rect.h - 4));
            m_Render->DrawFilledRect(Gwen::Rect(rect.x + 2, rect.y + 2, rect.w - 4, 1));



            if(bDepressed)
            {
                m_Render->SetDrawColor(m_Colors[ButtonPushedColor]);
                Gwen::Rect r(control->Width() / 2 - 2, control->Height() / 2 - 2, 5, 5);
                DrawCheck(r);
            }
            else if(bSelected)
            {
                m_Render->SetDrawColor(m_Colors[SelectedColor]);
                Gwen::Rect r(control->Width() / 2 - 2, control->Height() / 2 - 2, 5, 5);
                DrawCheck(r);
            }
        }

        virtual void DrawGroupBox(Gwen::Controls::Base * control, int textStart, int textHeight, int textWidth)
        {
            Gwen::Rect rect = control->GetRenderBounds();

            rect.y += textHeight * 0.5f;
            rect.h -= textHeight * 0.5f;

            m_Render->SetDrawColor(m_Colors[BaseLightColor]);

            m_Render->DrawFilledRect(Gwen::Rect(rect.x + 1, rect.y + 1, textStart - 3, 1));
            m_Render->DrawFilledRect(Gwen::Rect(rect.x + 1 + textStart + textWidth, rect.y + 1, rect.w - textStart + textWidth - 2, 1));
            m_Render->DrawFilledRect(Gwen::Rect(rect.x + 1, (rect.y + rect.h) - 1, rect.w - 2, 1));

            m_Render->DrawFilledRect(Gwen::Rect(rect.x + 1, rect.y + 1, 1, rect.h));
            m_Render->DrawFilledRect(Gwen::Rect((rect.x + rect.w) - 2, rect.y + 1, 1, rect.h - 1));

            m_Render->SetDrawColor(m_Colors[BaseDarkColor]);

            m_Render->DrawFilledRect(Gwen::Rect(rect.x + 1, rect.y, textStart - 3, 1));
            m_Render->DrawFilledRect(Gwen::Rect(rect.x + 1 + textStart + textWidth, rect.y, rect.w - textStart - textWidth - 2, 1));
            m_Render->DrawFilledRect(Gwen::Rect(rect.x + 1, (rect.y + rect.h) - 1, rect.w - 2, 1));

            m_Render->DrawFilledRect(Gwen::Rect(rect.x, rect.y + 1, 1, rect.h - 1));
            m_Render->DrawFilledRect(Gwen::Rect((rect.x + rect.w) - 1, rect.y + 1, 1, rect.h - 1));
        }

        virtual void DrawTextBox(Gwen::Controls::Base * control)
        {
            Gwen::Rect rect = control->GetRenderBounds();
            bool bHasFocus = control->HasFocus();

            // Box inside
            m_Render->SetDrawColor(m_Colors[TextBoxColor]);
            m_Render->DrawFilledRect(Gwen::Rect(1, 1, rect.w - 2, rect.h - 2));

            m_Render->SetDrawColor(m_Colors[BaseOutlineLightColor]);
            m_Render->DrawFilledRect(Gwen::Rect(rect.x + 1, rect.y, rect.w - 2, 1));
            m_Render->DrawFilledRect(Gwen::Rect(rect.x, rect.y + 1, 1, rect.h - 2));

            m_Render->SetDrawColor(m_Colors[BaseOutlineLighterColor]);
            m_Render->DrawFilledRect(Gwen::Rect(rect.x + 1, (rect.y + rect.h) - 1, rect.w - 2, 1));
            m_Render->DrawFilledRect(Gwen::Rect((rect.x + rect.w) - 1, rect.y + 1, 1, rect.h - 2));

            if(bHasFocus)
            {
                m_Render->SetDrawColor(m_Colors[GlossColor]);
                m_Render->DrawLinedRect(rect);
            }
        }

        virtual void DrawTabButton(Gwen::Controls::Base * control, bool bActive)
        {
            Gwen::Rect rect = control->GetRenderBounds();
            bool bHovered = control->IsHovered();

            if(bActive)
            {
                m_Render->SetDrawColor(m_Colors[BaseDarkColor]);
                m_Render->DrawFilledRect(Gwen::Rect(rect.x + 1, rect.y + 1, rect.w - 2, rect.h - 1));
            }
            else
            {
                if(bHovered)m_Render->SetDrawColor(m_Colors[TabColor]);
                else		   m_Render->SetDrawColor(m_Colors[TabColor]);

                m_Render->DrawFilledRect(Gwen::Rect(1, 1, rect.w - 2, rect.h - 1));

                if(bHovered)m_Render->SetDrawColor(m_Colors[BaseDarkerColor]);
                else		   m_Render->SetDrawColor(m_Colors[BaseDarkColor]);

                m_Render->DrawFilledRect(Gwen::Rect(1, rect.h * 0.5, rect.w - 2, rect.h * 0.5 - 1));

                m_Render->SetDrawColor(m_Colors[TabColor]);
                m_Render->DrawShavedCornerRect(Gwen::Rect(1, 1, rect.w - 2, rect.h));
            }

            m_Render->SetDrawColor(m_Colors[BorderColor]);

            m_Render->DrawShavedCornerRect(Gwen::Rect(0, 0, rect.w, rect.h));
        }

        virtual void DrawTabControl(Gwen::Controls::Base * control, Gwen::Rect CurrentButtonRect)
        {
            Gwen::Rect rect = control->GetRenderBounds();

            m_Render->SetDrawColor(m_Colors[BaseFillColor]);
            m_Render->DrawFilledRect(rect);

            m_Render->SetDrawColor(m_Colors[BorderColor]);
            m_Render->DrawLinedRect(rect);

            m_Render->SetDrawColor(m_Colors[ButtonDarkColor]);
            m_Render->DrawFilledRect(CurrentButtonRect);
        }

        virtual void DrawWindow(Gwen::Controls::Base * control, int topHeight, bool inFocus)
        {
            Gwen::Rect rect = control->GetRenderBounds();

            // Titlebar
            if(inFocus)
                m_Render->SetDrawColor(m_Colors[WindowActiveColor]);
            else
                m_Render->SetDrawColor(m_Colors[WindowColor]);

            int iBorderSize = 5;
            m_Render->DrawFilledRect(Gwen::Rect(rect.x + 1, rect.y + 1, rect.w - 2, topHeight - 1));
            m_Render->DrawFilledRect(Gwen::Rect(rect.x + 1, rect.y + topHeight - 1, iBorderSize, rect.h - 2 - topHeight));
            m_Render->DrawFilledRect(Gwen::Rect(rect.x + rect.w - iBorderSize, rect.y + topHeight - 1, iBorderSize, rect.h - 2 - topHeight));
            m_Render->DrawFilledRect(Gwen::Rect(rect.x + 1, rect.y + rect.h - iBorderSize, rect.w - 2, iBorderSize));

            // Main inner
            m_Render->SetDrawColor(Gwen::Color(m_Colors[BaseDarkColor].r, m_Colors[BaseDarkColor].g, m_Colors[BaseDarkColor].b, 230));
            m_Render->DrawFilledRect(Gwen::Rect(rect.x + iBorderSize + 1, rect.y + topHeight, rect.w - iBorderSize * 2 - 2, rect.h - topHeight - iBorderSize - 1));

            // Light inner border
            m_Render->SetDrawColor(m_Colors[WindowBorderColor]);
            m_Render->DrawShavedCornerRect(Gwen::Rect(rect.x + 1, rect.y + 1, rect.w - 2, rect.h - 2));

            // Dark line between titlebar and main
            m_Render->SetDrawColor(m_Colors[BorderColor]);

            // Inside border
            m_Render->SetDrawColor(m_Colors[BaseOutlineColor]);
            m_Render->DrawLinedRect(Gwen::Rect(rect.x + iBorderSize, rect.y + topHeight - 1, rect.w - 10, rect.h - topHeight - (iBorderSize - 1)));

            // Dark outer border
            m_Render->SetDrawColor(m_Colors[BorderColor]);
            m_Render->DrawShavedCornerRect(Gwen::Rect(rect.x, rect.y, rect.w, rect.h));
        }

        virtual void DrawHighlight(Gwen::Controls::Base * control)
        {
            Gwen::Rect rect = control->GetRenderBounds();
            m_Render->SetDrawColor(m_Colors[HighlightColor]);
            m_Render->DrawFilledRect(rect);
        }

        virtual void DrawScrollBar(Gwen::Controls::Base * control, bool isHorizontal, bool bDepressed)
        {
            Gwen::Rect rect = control->GetRenderBounds();

            if(bDepressed)
                m_Render->SetDrawColor(m_Colors[ScrollBarDarkColor]);
            else
                m_Render->SetDrawColor(m_Colors[ScrollBarLightColor]);

            m_Render->DrawFilledRect(rect);
        }

        virtual void DrawScrollBarButton(int w, int h, bool bDepressed, bool bHovered, bool bSquared = false)
        {
            if(!bDepressed && !bHovered)
                return;

            if(bDepressed)	m_Render->SetDrawColor(m_Colors[ButtonPushedColor]);
            else if(bHovered)m_Render->SetDrawColor(m_Colors[HoverLightColor]);
            else				m_Render->SetDrawColor(m_Colors[ScrollBarColor]);

            m_Render->DrawFilledRect(Gwen::Rect(1, 1, w - 2, h - 2));

            if(bDepressed)	m_Render->SetDrawColor(m_Colors[ButtonPushedColor]);
            else if(bHovered)m_Render->SetDrawColor(m_Colors[HoverColor]);
            else				m_Render->SetDrawColor(m_Colors[ScrollBarDarkColor]);

            m_Render->DrawFilledRect(Gwen::Rect(1, h * 0.5, w - 2, h * 0.5 - 2));

            if(!bDepressed)
            {
                m_Render->SetDrawColor(m_Colors[ScrollBarLightColor]);
                m_Render->DrawShavedCornerRect(Gwen::Rect(1, 1, w - 2, h - 2), bSquared);
            }
            else
            {
                m_Render->SetDrawColor(m_Colors[ButtonPushedColor]);
                m_Render->DrawShavedCornerRect(Gwen::Rect(1, 1, w - 2, h - 2), bSquared);
            }

            // Border
            m_Render->SetDrawColor(m_Colors[ScrollBarOutlineColor]);
            m_Render->DrawShavedCornerRect(Gwen::Rect(0, 0, w, h), bSquared);
        }

        virtual void DrawScrollBarBar(Gwen::Controls::Base * control, bool bDepressed, bool isHovered, bool isHorizontal)
        {
            int w = control->Width();
            int h = control->Height();

            DrawScrollBarButton(w, h, bDepressed, isHovered);
        }

        virtual void DrawTabTitleBar(Gwen::Controls::Base * control)
        {
            Gwen::Rect rect = control->GetRenderBounds();

            m_Render->SetDrawColor(m_Colors[TabColor]);
            m_Render->DrawFilledRect(rect);

            m_Render->SetDrawColor(m_Colors[BorderColor]);
            rect.h += 1;
            m_Render->DrawLinedRect(rect);
        }

        virtual void DrawProgressBar(Gwen::Controls::Base * control, bool isHorizontal, float progress)
        {
            Gwen::Rect rect = control->GetRenderBounds();

            if(isHorizontal)
            {
                //BackgroundColor
                m_Render->SetDrawColor(m_Colors[ButtonDarkColor]);
                m_Render->DrawFilledRect(Gwen::Rect(1, 1, rect.w - 2, rect.h - 2));

                //Right half
                m_Render->SetDrawColor(m_Colors[ProgressBarColor]);
                m_Render->DrawFilledRect(Gwen::Rect(1, 1, rect.w * progress - 2, rect.h - 2));

                m_Render->SetDrawColor(m_Colors[GlossColor]);
                m_Render->DrawFilledRect(Gwen::Rect(1, 1, rect.w - 2, rect.h * 0.45f));
            }
            else
            {
                //BackgroundColor
                m_Render->SetDrawColor(m_Colors[ButtonDarkColor]);
                m_Render->DrawFilledRect(Gwen::Rect(1, 1, rect.w - 2, rect.h - 2));

                //Top half
                m_Render->SetDrawColor(m_Colors[ProgressBarColor]);
                m_Render->DrawFilledRect(Gwen::Rect(1, 1 + (rect.h * (1 - progress)), rect.w - 2, rect.h * progress - 2));

                m_Render->SetDrawColor(m_Colors[GlossColor]);
                m_Render->DrawFilledRect(Gwen::Rect(1, 1, rect.w * 0.45f, rect.h - 2));
            }

            m_Render->SetDrawColor(m_Colors[GlossColor]);
            m_Render->DrawShavedCornerRect(Gwen::Rect(1, 1, rect.w - 2, rect.h - 2));

            m_Render->SetDrawColor(m_Colors[GlossLightColor]);
            m_Render->DrawShavedCornerRect(Gwen::Rect(2, 2, rect.w - 4, rect.h - 4));

            m_Render->SetDrawColor(m_Colors[BorderColor]);
            m_Render->DrawShavedCornerRect(rect);
        }

        virtual void DrawListBox(Gwen::Controls::Base * control)
        {
            Gwen::Rect rect = control->GetRenderBounds();

            m_Render->SetDrawColor(m_Colors[ListBoxColor]);
            m_Render->DrawFilledRect(rect);

            m_Render->SetDrawColor(m_Colors[BorderColor]);
            m_Render->DrawLinedRect(rect);
        }

        virtual void DrawListBoxLine(Gwen::Controls::Base * control, bool bSelected)
        {
            Gwen::Rect rect = control->GetRenderBounds();

            if(bSelected)
            {
                m_Render->SetDrawColor(Gwen::Color(m_Colors[BaseDarkColor].r, m_Colors[BaseDarkColor].g, m_Colors[BaseDarkColor].b, 230)); //m_Colors[HighlightOutlineColor]);
                m_Render->DrawFilledRect(rect);
            }
            else if(control->IsHovered())
            {
                m_Render->SetDrawColor(m_Colors[HoverColor]);
                m_Render->DrawFilledRect(rect);
            }
        }


        virtual void DrawSlider(Gwen::Controls::Base * control, bool bIsHorizontal, int numNotches, int barSize)
        {
            Gwen::Rect rect = control->GetRenderBounds();
            Gwen::Rect notchRect = rect;

            if(bIsHorizontal)
            {
                rect.y += rect.h * 0.4;
                rect.h -= rect.h * 0.8;
            }
            else
            {
                rect.x += rect.w * 0.4;
                rect.w -= rect.w * 0.8;
            }

            m_Render->SetDrawColor(m_Colors[ButtonOutlineColor]);
            m_Render->DrawFilledRect(rect);

            m_Render->SetDrawColor(m_Colors[ButtonDarkerColor]);
            m_Render->DrawFilledRect(rect);
        }

        virtual void DrawComboBox(Gwen::Controls::Base * control)
        {
            DrawTextBox(control);
        }


        virtual void DrawBackground(Gwen::Controls::Base * control)
        {
            Gwen::Rect rect = control->GetRenderBounds();
            m_Render->SetDrawColor(m_Colors[BackgroundColorDark]);
            m_Render->DrawFilledRect(rect);
            m_Render->SetDrawColor(m_Colors[BaseDarkerColor]);
            m_Render->DrawLinedRect(rect);
        }

        virtual void DrawKeyboardHighlight(Gwen::Controls::Base * control, const Gwen::Rect & r, int iOffset)
        {
            Gwen::Rect rect = r;

            rect.x += iOffset;
            rect.y += iOffset;
            rect.w -= iOffset * 2;
            rect.h -= iOffset * 2;

            //draw the top and bottom
            bool skip = true;

            for(int i = 0; i < rect.w * 0.5; i++)
            {
                m_Render->SetDrawColor(Gwen::Color(0, 0, 0, 255));

                if(!skip)
                {
                    m_Render->DrawPixel(rect.x + (i * 2), rect.y);
                    m_Render->DrawPixel(rect.x + (i * 2), rect.y + rect.h - 1);
                }
                else
                    skip = !skip;
            }

            skip = false;

            for(int i = 0; i < rect.h * 0.5; i++)
            {
                m_Render->SetDrawColor(Gwen::Color(0, 0, 0, 255));

                if(!skip)
                {
                    m_Render->DrawPixel(rect.x , rect.y + i * 2);
                    m_Render->DrawPixel(rect.x + rect.w - 1, rect.y + i * 2);
                }
                else
                    skip = !skip;
            }
        }

        virtual void DrawToolTip(Gwen::Controls::Base * control)
        {
            Gwen::Rect rct = control->GetRenderBounds();
            rct.x -= 3;
            rct.y -= 3;
            rct.w += 6;
            rct.h += 6;

            m_Render->SetDrawColor(m_Colors[ToolTipBackgroundColor]);
            m_Render->DrawFilledRect(rct);

            m_Render->SetDrawColor(m_Colors[ToolTipOutlineColor]);
            m_Render->DrawLinedRect(rct);
        }

        virtual void DrawScrollButton(Gwen::Controls::Base * control, int iDirection, bool bDepressed)
        {
            //					DrawButton( control, bDepressed, false );

            m_Render->SetDrawColor(m_Colors[ScrollButtonColor]);

            Gwen::Rect r(control->Width() / 2 - 2, control->Height() / 2 - 2, 5, 5);

            if(bDepressed) m_Render->SetDrawColor(m_Colors[SelectedColor]);
            else m_Render->SetDrawColor(m_Colors[ScrollBarDarkColor]);

            if(iDirection == Gwen::Pos::Top) DrawArrowUp(r);
            else if(iDirection == Gwen::Pos::Bottom) DrawArrowDown(r);
            else if(iDirection == Gwen::Pos::Left) DrawArrowLeft(r);
            else DrawArrowRight(r);
        }

        virtual void DrawComboBoxButton(Gwen::Controls::Base * control, bool bDepressed)
        {
            //DrawButton( control->Width(), control->Height(), bDepressed, false, true );

            m_Render->SetDrawColor(Gwen::Color(0, 0, 0, 240));

            Gwen::Rect r(control->Width() / 2 - 2, control->Height() / 2 - 2, 5, 5);
            DrawArrowDown(r);
        }

        virtual void DrawNumericUpDownButton(Gwen::Controls::Base * control, bool bDepressed, bool bUp)
        {
            //DrawButton( control->Width(), control->Height(), bDepressed, false, true );

            m_Render->SetDrawColor(Gwen::Color(0, 0, 0, 240));

            Gwen::Rect r(control->Width() / 2 - 2, control->Height() / 2 - 2, 5, 5);

            if(bUp) DrawArrowUp(r);
            else DrawArrowDown(r);

        }

        virtual void DrawTreeButton(Gwen::Controls::Base * control, bool bOpen)
        {
            Gwen::Rect rect = control->GetRenderBounds();
            rect.x += 2;
            rect.y += 2;
            rect.w -= 4;
            rect.h -= 4;

            m_Render->SetDrawColor(m_Colors[BaseLightColor]);
            m_Render->DrawFilledRect(rect);

            m_Render->SetDrawColor(m_Colors[BorderColor]);
            m_Render->DrawLinedRect(rect);

            m_Render->SetDrawColor(m_Colors[BorderColor]);

            if(!bOpen)    // ! because the button shows intention, not the current state
                m_Render->DrawFilledRect(Gwen::Rect(rect.x + rect.w / 2,	rect.y + 2,				1,			rect.h - 4));

            m_Render->DrawFilledRect(Gwen::Rect(rect.x + 2,			rect.y + rect.h / 2,		rect.w - 4,	1));

        }

        virtual void DrawTreeControl(Gwen::Controls::Base * control)
        {
            Gwen::Rect rect = control->GetRenderBounds();

            m_Render->SetDrawColor(m_Colors[BaseLightColor]);
            m_Render->DrawFilledRect(rect);

            m_Render->SetDrawColor(m_Colors[BorderColor]);
            m_Render->DrawLinedRect(rect);
        }

        void DrawTreeNode(Gwen::Controls::Base * ctrl, bool bOpen, bool bSelected, int iLabelHeight, int iLabelWidth, int iHalfWay, int iLastBranch, bool bIsRoot)
        {
            if(bSelected)
            {
                m_Render->SetDrawColor(m_Colors[SelectedColor]);

                m_Render->DrawFilledRect(Gwen::Rect(17, 0, iLabelWidth + 2, iLabelHeight - 1));
                m_Render->SetDrawColor(m_Colors[SelectedOutlineColor]);
                m_Render->DrawLinedRect(Gwen::Rect(17, 0, iLabelWidth + 2, iLabelHeight - 1));
            }

            m_Render->SetDrawColor(Gwen::Color(0, 0, 0, 50));

            if(!bIsRoot)
                m_Render->DrawFilledRect(Gwen::Rect(9, iHalfWay, 16 - 9, 1));

            if(!bOpen) return;

            m_Render->DrawFilledRect(Gwen::Rect(14 + 8, iLabelHeight, 1, iLastBranch + iHalfWay - iLabelHeight));
        }

        virtual void DrawStatusBar(Gwen::Controls::Base * control)
        {
            DrawBackground(control);
        }

        virtual void DrawPropertyRow(Gwen::Controls::Base * control, int iWidth, bool bBeingEdited)
        {
            Gwen::Rect rect = control->GetRenderBounds();

            if(bBeingEdited)
            {
                m_Render->SetDrawColor(m_Colors[SelectedColor]);
                m_Render->DrawFilledRect(Gwen::Rect(0, rect.y, iWidth, rect.h));
            }

            m_Render->SetDrawColor(m_Colors[BaseOutlineLighterColor]);

            m_Render->DrawFilledRect(Gwen::Rect(iWidth, rect.y, 2, rect.h));

            rect.y += rect.h - 2;
            rect.h = 2;


            m_Render->DrawFilledRect(rect);
        }

        virtual void DrawPropertyTreeNode(Gwen::Controls::Base * control, int BorderLeft, int BorderTop)
        {
            Gwen::Rect rect = control->GetRenderBounds();

            m_Render->SetDrawColor(m_Colors[BaseOutlineLighterColor]);

            m_Render->DrawFilledRect(Gwen::Rect(rect.x, rect.y, BorderLeft, rect.h));
            m_Render->DrawFilledRect(Gwen::Rect(rect.x + BorderLeft, rect.y, rect.w - BorderLeft, BorderTop));
        }

        void DrawColorDisplay(Gwen::Controls::Base * control, Gwen::Color color)
        {
            Gwen::Rect rect = control->GetRenderBounds();

            if(color.a != 255)
            {
                GetRender()->SetDrawColor(Gwen::Color(255, 255, 255, 255));
                GetRender()->DrawFilledRect(rect);

                GetRender()->SetDrawColor(Gwen::Color(128, 128, 128, 128));

                GetRender()->DrawFilledRect(Gwen::Rect(0, 0, rect.w * 0.5, rect.h * 0.5));
                GetRender()->DrawFilledRect(Gwen::Rect(rect.w * 0.5, rect.h * 0.5, rect.w * 0.5, rect.h * 0.5));
            }

            GetRender()->SetDrawColor(color);
            GetRender()->DrawFilledRect(rect);

            GetRender()->SetDrawColor(Gwen::Color(0, 0, 0, 255));
            GetRender()->DrawLinedRect(rect);
        }

        virtual void DrawModalControl(Gwen::Controls::Base * control)
        {
            if(control->ShouldDrawBackground())
            {
                Gwen::Rect rect = control->GetRenderBounds();
                GetRender()->SetDrawColor(m_Colors[ModalColor]);
                GetRender()->DrawFilledRect(rect);
            }
        }

        virtual void DrawMenuDivider(Gwen::Controls::Base * control)
        {
            Gwen::Rect rect = control->GetRenderBounds();
            GetRender()->SetDrawColor(m_Colors[BackgroundColorDark]);
            GetRender()->DrawFilledRect(rect);
            GetRender()->SetDrawColor(m_Colors[BaseDarkerColor]);
            GetRender()->DrawLinedRect(rect);
        }
};

// ============================================================================================== //

VD_INTERFACE_GWEN_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_INTERFACE_GWEN_SKIN_INCLUDED

// ============================================================================================== //
// end file
