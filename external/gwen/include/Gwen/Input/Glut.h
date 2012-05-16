/*
	GWEN
	Copyright (c) 2011 Facepunch Studios
	See license in Gwen.h
*/

#pragma once
#ifndef GWEN_INPUT_Glut_H
#define GWEN_INPUT_Glut_H

#include "Gwen/InputHandler.h"
#include "Gwen/Gwen.h"
#include "Gwen/Controls/Canvas.h"

#include <GL/glew.h>
#include <OpenGL/gl.h>
#include <Glut/glut.h>

namespace Gwen
{
	namespace Input
	{
		class Glut
		{
			public:
			
				struct Event
				{
					struct SizeEvent
					{
						unsigned int Width;  ///< New width, in pixels
						unsigned int Height; ///< New height, in pixels
					};
				 
					struct KeyEvent
					{
						int			  Code;    ///< Code of the key that has been pressed
						bool          Alt;     ///< Is the Alt key pressed?
						bool          Control; ///< Is the Control key pressed?
						bool          Shift;   ///< Is the Shift key pressed?
						bool          System;  ///< Is the System key pressed?
					};
				
					struct MouseMoveEvent
					{
						int X; ///< X position of the mouse pointer, relative to the left of the owner window
						int Y; ///< Y position of the mouse pointer, relative to the top of the owner window
					};
				
					struct MouseButtonEvent
					{
						int			  Button; ///< Code of the button that has been pressed
						int           X;      ///< X position of the mouse pointer, relative to the left of the owner window
						int           Y;      ///< Y position of the mouse pointer, relative to the top of the owner window
					};
				
					enum EventType
					{
						Closed,                 ///< The window requested to be closed
						Resized,                ///< The window was resized
						LostFocus,              ///< The window lost the focus
						GainedFocus,            ///< The window gained the focus
						KeyPressed,             ///< A key was pressed
						KeyReleased,            ///< A key was released
						MouseButtonPressed,     ///< A mouse button was pressed
						MouseButtonReleased,    ///< A mouse button was released
						MouseMoved,             ///< The mouse cursor moved
						MouseEntered,           ///< The mouse cursor entered the area of the window
						MouseLeft,              ///< The mouse cursor left the area of the window
						Count                   ///< Keep last -- the total number of event types
					};
				
					EventType Type; ///< Type of the event
				
					union
					{
						SizeEvent            Size;            ///< Size event parameters
						KeyEvent             Key;             ///< Key event parameters
						MouseMoveEvent       MouseMove;       ///< Mouse move event parameters
						MouseButtonEvent     MouseButton;     ///< Mouse button event parameters
					};
				};
			
			public:

				Glut()
				{
					m_Canvas = NULL;
					m_MouseX = 0;
					m_MouseY = 0;
				}

				void Initialize( Gwen::Controls::Canvas* c )
				{
					m_Canvas = c;
				}
				
				unsigned char TranslateKeyCode( int iKeyCode )
				{
					switch ( iKeyCode )
					{
						case GLUT_KEY_LEFT:                   return Gwen::Key::Left;
						case GLUT_KEY_UP:                     return Gwen::Key::Up;
						case GLUT_KEY_RIGHT:                  return Gwen::Key::Right;
						case GLUT_KEY_DOWN:                   return Gwen::Key::Down;
//						case GLUT_KEY_PAGE_UP:                return Gwen::Key::PageUp;
//						case GLUT_KEY_PAGE_DOWN:              return Gwen::Key::PageDown;
						case GLUT_KEY_HOME:                   return Gwen::Key::Home;
						case GLUT_KEY_END:                    return Gwen::Key::End;
//						case GLUT_KEY_INSERT:                 return Gwen::Key::Insert;
					};

					return Gwen::Key::Invalid;
				}

				bool ProcessMessage( Glut::Event& event )
				{
					if ( !m_Canvas ) return false;
					switch(event.Type)
					{
						case Glut::Event::MouseMoved:
							{
								printf("Processing Mouse Move Event (%d, %d)...\n", event.MouseMove.X, event.MouseMove.Y);

								int dx = event.MouseMove.X - m_MouseX;
								int dy = event.MouseMove.Y - m_MouseY;

								m_MouseX = event.MouseMove.X;
								m_MouseY = event.MouseMove.Y;

								return m_Canvas->InputMouseMoved( m_MouseX, m_MouseY, dx, dy );
							}


						case Glut::Event::MouseButtonPressed:
						case Glut::Event::MouseButtonReleased:
							{
								printf("Processing Mouse Button Event (%d, %d)...\n", event.MouseButton.Button, event.Type == Glut::Event::MouseButtonPressed ? 1 : 0);
								return m_Canvas->InputMouseButton( event.MouseButton.Button, event.Type == Glut::Event::MouseButtonPressed );
							}
							
						case Glut::Event::KeyPressed:
						case Glut::Event::KeyReleased:
							{
								bool bPressed = (event.Type == Glut::Event::KeyPressed);

								printf("Processing Keyboard Event (%d, %d)...\n", event.Key.Code, bPressed ? 1 : 0);

								if ( event.Key.Control && bPressed && event.Key.Code >= 'a' && event.Key.Code <= 'z' )
								{
									return m_Canvas->InputCharacter( event.Key.Code );
								}

								unsigned char iKey = TranslateKeyCode( event.Key.Code );

								return m_Canvas->InputKey( iKey, bPressed );

							}
					}

					return false;
				}

				protected:

					Gwen::Controls::Canvas*	m_Canvas;
					int m_MouseX;
					int m_MouseY;

		};
	}
}
#endif
