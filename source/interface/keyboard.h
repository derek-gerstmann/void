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

#ifndef VD_INTERFACE_KEYBOARD_INCLUDED
#define VD_INTERFACE_KEYBOARD_INCLUDED

// ============================================================================================== //

#include "interface/interface.h"

// ============================================================================================== //

VD_INTERFACE_NAMESPACE_BEGIN();

// ============================================================================================== //

class Keyboard
{
public :

	VD_DECLARE_ENUM(KeyCode,	
		Invalid,				
		A, 
		B, 
		C, 
		D, 
		E, 
		F,
		G, 
		H, 			
		I, 
		J, 
		K, 
		L, 
		M, 
		N, 
		O, 
		P, 			
		Q, 
		R, 
		S, 
		T,	
		U, 
		V, 
		W, 
		X, 			
		Y, 
		Z,					
		Num0, 
		Num1, 
		Num2, 
		Num3, 
		Num4,
		Num5,
		Num6, 
		Num7, 
		Num8, 
		Num9,
		Up, 
		Down, 
		Left, 
		Right, 
		PageUp, 
		PageDown, 
		End, 
		Home,
		Tab,
		Escape, 
		Insert, 
		Delete, 
		Pause,
		Backspace, 
		Return, 
		System,
		Shift, 
		Control, 
		Alt,
		LeftBracket, 
		RightBracket,
		LeftBrace, 
		RightBrace,
		LeftParen, 
		RightParen,
		EqualSign, 
		PlusSign, 
		MinusSign,
		Tilde,
		AtSign,
		NumSign,
		DollarSign,
		PercentSign,
		UpCarat,
		Ampersand,
		Asterisk,
		F1, 
		F2, 
		F3, 
		F4, 
		F5, 
		F6, 
		F7, 
		F8, 
		F9, 
		F10, 
		F11, 
		F12);
};

// ============================================================================================== //

VD_INTERFACE_NAMESPACE_END();

// ============================================================================================== //

#endif 
