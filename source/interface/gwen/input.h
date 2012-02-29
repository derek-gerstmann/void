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

#ifndef VD_INTERFACE_GWEN_INPUT_INCLUDED
#define VD_INTERFACE_GWEN_INPUT_INCLUDED

// ============================================================================================== //

#include "interface/event.h"
#include "interface/gwen/gwen.h"
#include "interface/gwen/canvas.h"

// ============================================================================================== //

VD_INTERFACE_GWEN_NAMESPACE_BEGIN();

// ============================================================================================== //

class Input 
{
public:

	Input() 
	:	m_Canvas(NULL),
		m_MouseX(0),
		m_MouseY(0)
	{
		// EMPTY!
	}

	void 
	Initialize( 
		Canvas* c )
	{
		m_Canvas = c;
	}
	
	unsigned char 
	TranslateKeyCode( 
		Keyboard::KeyCode::Value code )
	{
		switch ( code )
		{
			case Keyboard::KeyCode::A:                  	 return 'a';
			case Keyboard::KeyCode::B:                 	  	 return 'b';
			case Keyboard::KeyCode::C:                	  	 return 'c';
			case Keyboard::KeyCode::D:                  	 return 'd';
			case Keyboard::KeyCode::E:                 	 	 return 'e';
			case Keyboard::KeyCode::F:                   	 return 'f';
			case Keyboard::KeyCode::G:                   	 return 'g';
			case Keyboard::KeyCode::H:                   	 return 'h';
			case Keyboard::KeyCode::I:                  	 return 'i';
			case Keyboard::KeyCode::J:                  	 return 'j';
			case Keyboard::KeyCode::K:                 	     return 'k';
			case Keyboard::KeyCode::L:                   	 return 'l';
			case Keyboard::KeyCode::M:                   	 return 'm';
			case Keyboard::KeyCode::N:                  	 return 'n';
			case Keyboard::KeyCode::O:                  	 return 'o';
			case Keyboard::KeyCode::P:                  	 return 'p';
			case Keyboard::KeyCode::Q:                  	 return 'q';
			case Keyboard::KeyCode::R:                  	 return 'r';
			case Keyboard::KeyCode::S:                  	 return 's';
			case Keyboard::KeyCode::T:                  	 return 't';
			case Keyboard::KeyCode::U:                  	 return 'u';
			case Keyboard::KeyCode::V:                  	 return 'v';
			case Keyboard::KeyCode::W:                   	 return 'w';
			case Keyboard::KeyCode::X:      	             return 'x';
			case Keyboard::KeyCode::Y:  	                 return 'y';
			case Keyboard::KeyCode::Z:	                     return 'z';
			case Keyboard::KeyCode::Left:                    return Gwen::Key::Left;
			case Keyboard::KeyCode::Up:                      return Gwen::Key::Up;
			case Keyboard::KeyCode::Right:                   return Gwen::Key::Right;
			case Keyboard::KeyCode::Down:                    return Gwen::Key::Down;
			case Keyboard::KeyCode::Home:                    return Gwen::Key::Home;
			case Keyboard::KeyCode::End:                     return Gwen::Key::End;
			case Keyboard::KeyCode::PageUp:                  return 73;
			case Keyboard::KeyCode::PageDown:                return 81;
			case Keyboard::KeyCode::Insert:					 return 82;
			case Keyboard::KeyCode::Delete:					 return 83;
			default:
				return code;
		};

		return Gwen::Key::Invalid;
	}

	bool OnEvent( const Event& event )
	{
		if ( m_Canvas == NULL ) 
			return false;
			
		switch(event.Kind)
		{
			case Event::Type::MouseMoved:
			{
				int dx = event.MouseMove.X - m_MouseX;
				int dy = event.MouseMove.Y - m_MouseY;

				m_MouseX = event.MouseMove.X;
				m_MouseY = event.MouseMove.Y;

				return m_Canvas->InputMouseMoved( m_MouseX, m_MouseY, dx, dy );
			}

			case Event::Type::MouseDown:
			case Event::Type::MouseUp:
			{
				int button = 1;
				switch(event.MouseButton.Button)
				{
					case Mouse::Button::Left: 	{ button = 0; break; }
					case Mouse::Button::Right:	{ button = 1; break; }
					case Mouse::Button::Middle: { button = 2; break; }
					case Mouse::Button::X1: 	{ button = 3; break; }
					case Mouse::Button::X2: 	{ button = 4; break; }
					case Mouse::Button::X3: 	{ button = 5; break; }
					case Mouse::Button::X4: 	{ button = 6; break; }
					case Mouse::Button::X5: 	{ button = 7; break; }
					case Mouse::Button::X6: 	{ button = 8; break; }
					case Mouse::Button::X7: 	{ button = 9; break; }
					case Mouse::Button::X8: 	{ button = 10; break; }
					default: break;
				}
				return m_Canvas->InputMouseButton( button, event.Kind == Event::Type::MouseDown );
			}
				
			case Event::Type::KeyUp:
			case Event::Type::KeyDown:
			{
				bool bPressed = (event.Kind == Event::Type::KeyDown);
				if ( event.Key.Control && bPressed && event.Key.Code >= 'a' && event.Key.Code <= 'z' )
				{
					return m_Canvas->InputCharacter( event.Key.Code );
				}

				unsigned char iKey = TranslateKeyCode( event.Key.Code );

				return m_Canvas->InputKey( iKey, bPressed );

			}
			default:
				return false;
		}

		return false;
	}

protected:

	Canvas*	m_Canvas;
	int m_MouseX;
	int m_MouseY;

};

// ============================================================================================== //

VD_INTERFACE_GWEN_NAMESPACE_END();

// ============================================================================================== //

#endif	// VD_INTERFACE_GWEN_INPUT_INCLUDED

// ============================================================================================== //
// end file
