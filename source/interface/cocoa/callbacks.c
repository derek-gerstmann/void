// ============================================================================================== //
//
// License:     The Lesser GNU Public License (LGPL) v3.0.
// 
// Author(s):   Derek Gerstmann. The University of Western Australia (UWA). 
//              As well as the shoulders of many giants...
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

#include "interface/cocoa/callbacks.h"

// ============================================================================================== //

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================================== //

Keyboard::KeyCode::Value
vdCocoaTranslateAsciiKey(int code)
{
    switch(code)
    {
        case 8:             { return Keyboard::KeyCode::Delete;  }
        case 9:             { return Keyboard::KeyCode::Tab;  }
        case 10:            { return Keyboard::KeyCode::Return;  }
        case 13:            { return Keyboard::KeyCode::Return;  }
        case 27:            { return Keyboard::KeyCode::Escape;  }
        case '0':           { return Keyboard::KeyCode::Num0;  }
        case '1':           { return Keyboard::KeyCode::Num1;  }
        case '2':           { return Keyboard::KeyCode::Num2;  }
        case '3':           { return Keyboard::KeyCode::Num3;  }
        case '4':           { return Keyboard::KeyCode::Num4;  }
        case '5':           { return Keyboard::KeyCode::Num5;  }
        case '6':           { return Keyboard::KeyCode::Num6;  }
        case '7':           { return Keyboard::KeyCode::Num7;  }
        case '8':           { return Keyboard::KeyCode::Num8;  }
        case '9':           { return Keyboard::KeyCode::Num9;  }
        case 'a':           { return Keyboard::KeyCode::A;  }
        case 'A':           { return Keyboard::KeyCode::A;  }
        case 'b':
        case 'B':           { return Keyboard::KeyCode::B;  }
        case 'c':
        case 'C':           { return Keyboard::KeyCode::C;  }
        case 'd':
        case 'D':           { return Keyboard::KeyCode::D;  }
        case 'e':
        case 'E':           { return Keyboard::KeyCode::E;  }
        case 'f':
        case 'F':           { return Keyboard::KeyCode::F;  }
        case 'g':
        case 'G':           { return Keyboard::KeyCode::G;  }
        case 'h':
        case 'H':           { return Keyboard::KeyCode::H;  }
        case 'i':
        case 'I':           { return Keyboard::KeyCode::I;  }
        case 'j':
        case 'J':           { return Keyboard::KeyCode::J;  }
        case 'k':           { return Keyboard::KeyCode::K;  }
        case 'K':           { return Keyboard::KeyCode::K;  }
        case 'l':           { return Keyboard::KeyCode::L;  }
        case 'L':           { return Keyboard::KeyCode::L;  }
        case 'm':
        case 'M':           { return Keyboard::KeyCode::M;  }
        case 'n':
        case 'N':           { return Keyboard::KeyCode::N;  }
        case 'o':
        case 'O':           { return Keyboard::KeyCode::O;  }
        case 'p':
        case 'P':           { return Keyboard::KeyCode::P;  }
        case 'q':
        case 'Q':           { return Keyboard::KeyCode::Q;  }
        case 'r':
        case 'R':           { return Keyboard::KeyCode::R;  }
        case 's':
        case 'S':           { return Keyboard::KeyCode::S;  }
        case 't':
        case 'T':           { return Keyboard::KeyCode::T;  }
        case 'u':
        case 'U':           { return Keyboard::KeyCode::U;  }
        case 'v':
        case 'V':           { return Keyboard::KeyCode::V;  }
        case 'w':
        case 'W':           { return Keyboard::KeyCode::W;  }
        case 'x':
        case 'X':           { return Keyboard::KeyCode::X;  }
        case 'y':
        case 'Y':           { return Keyboard::KeyCode::Y;  }
        case 'z':
        case 'Z':           { return Keyboard::KeyCode::Z;  }
        case '~':           { return Keyboard::KeyCode::Tilde;  }
        case '@':           { return Keyboard::KeyCode::AtSign;  }
        case '#':           { return Keyboard::KeyCode::NumSign;  }
        case '$':           { return Keyboard::KeyCode::DollarSign;  }
        case '%':           { return Keyboard::KeyCode::PercentSign;  }
        case '^':           { return Keyboard::KeyCode::UpCarat;  }
        case '&':           { return Keyboard::KeyCode::Ampersand;  }
        case '*':           { return Keyboard::KeyCode::Asterisk;  }
        case '(':           { return Keyboard::KeyCode::LeftParen;  }
        case ')':           { return Keyboard::KeyCode::RightParen;  }
        case '=':           { return Keyboard::KeyCode::EqualSign;  }
        case '-':           { return Keyboard::KeyCode::MinusSign;  }
        case '+':           { return Keyboard::KeyCode::PlusSign;  }
        case '[':           { return Keyboard::KeyCode::LeftBracket;  }
        case ']':           { return Keyboard::KeyCode::RightBracket;  }
        case '{':           { return Keyboard::KeyCode::LeftBrace;  }
        case '}':           { return Keyboard::KeyCode::RightBrace;  }
        default:            { return Keyboard::KeyCode::FromInteger(code);  }
    };

    return Keyboard::KeyCode::FromInteger(code);
}

Keyboard::KeyCode::Value
vdCocoaTranslateSpecialKey(int code)
{
    switch(code)
    {
        case COCOA_KEY_F1:      { return Keyboard::KeyCode::F1;  }
        case COCOA_KEY_F2:      { return Keyboard::KeyCode::F2;  }
        case COCOA_KEY_F3:      { return Keyboard::KeyCode::F3;  }
        case COCOA_KEY_F4:      { return Keyboard::KeyCode::F4;  }
        case COCOA_KEY_F5:      { return Keyboard::KeyCode::F5;  }
        case COCOA_KEY_F6:      { return Keyboard::KeyCode::F6;  }
        case COCOA_KEY_F7:      { return Keyboard::KeyCode::F7;  }
        case COCOA_KEY_F8:      { return Keyboard::KeyCode::F8;  }
        case COCOA_KEY_F9:      { return Keyboard::KeyCode::F9;  }
        case COCOA_KEY_F10:     { return Keyboard::KeyCode::F10;  }
        case COCOA_KEY_F11:     { return Keyboard::KeyCode::F11;  }
        case COCOA_KEY_F12:     { return Keyboard::KeyCode::F12;  }
        case COCOA_KEY_UP:      { return Keyboard::KeyCode::Up;  }
        case COCOA_KEY_DOWN:        { return Keyboard::KeyCode::Down;  }
        case COCOA_KEY_LEFT:        { return Keyboard::KeyCode::Left;  }
        case COCOA_KEY_RIGHT:   { return Keyboard::KeyCode::Right;  }
        case COCOA_KEY_HOME:        { return Keyboard::KeyCode::Home;  }
        case COCOA_KEY_END:     { return Keyboard::KeyCode::End;  }
        case COCOA_KEY_INSERT:  { return Keyboard::KeyCode::Insert;  }
        default:                { return Keyboard::KeyCode::Invalid;  }
    };

    return Keyboard::KeyCode::Invalid;
}

void
vdCocoaShutdown()
{
/*
    glutDisplayFunc(vdNullDisplayCallback);
    glutReshapeFunc(vdNullReshapeCallback);
    glutMouseFunc(vdNullMouseCallback);
    glutPassiveMotionFunc(vdNullMotionCallback);
    glutMotionFunc(vdNullMotionCallback);
    glutKeyboardFunc(vdNullKeyboardCallback);
    glutKeyboardUpFunc(vdNullKeyboardCallback);
    glutSpecialFunc(vdNullSpecialKeyCallback);
    glutSpecialUpFunc(vdNullSpecialKeyCallback);
    glutVisibilityFunc(vdNullEntryCallback);
    glutEntryFunc(vdNullEntryCallback);
    glutIdleFunc(vdNullCallback);
*/
    if(Global::ActiveWindow)
    {   
        Event event;
        event.Kind = Event::Type::Shutdown;
        event.Update.Time = Core::Process::GetTimeInSeconds();
        Global::ActiveWindow->ProcessEvent( event );
        Global::ActiveWindow = NULL;
    }
}

void 
vdCocoaMotionCallback(int x, int y)
{
    int dx = 0;
    int dy = 0;
    
    dx = x - dx;
    dy = y - dy;
    
    Event event;
    event.MouseMove.X = x;
    event.MouseMove.Y = y;
    event.Kind = Event::Type::MouseMoved;
    
    dx = x;
    dy = y;
    
    if(Global::ActiveWindow) 
        Global::ActiveWindow->ProcessEvent( event );
}

void 
vdCocoaMouseCallback(int button, int state, int x, int y)
{
    Event event;
    Mouse::Button::Value value = Mouse::Button::Left;
    
    switch(button)
    {
        case 0:     { value = Mouse::Button::Left; break; }
        case 1:     { value = Mouse::Button::Right; break; }
        case 2:     { value = Mouse::Button::Middle; break; }
        case 3:     { value = Mouse::Button::X1; break; }
        case 4:     { value = Mouse::Button::X2; break; }
        case 5:     { value = Mouse::Button::X3; break; }
        case 6:     { value = Mouse::Button::X4; break; }
        case 7:     { value = Mouse::Button::X5; break; }
        case 8:     { value = Mouse::Button::X6; break; }
        case 9:     { value = Mouse::Button::X7; break; }
        case 10:    { value = Mouse::Button::X8; break; }
        default:    break;
    };

    event.MouseButton.Button = value;
    event.MouseButton.X = x;
    event.MouseButton.Y = y;

    if (state == COCOA_DOWN)
        event.Kind = Event::Type::MouseDown;
    else
        event.Kind = Event::Type::MouseUp;

    if(Global::ActiveWindow) 
        Global::ActiveWindow->ProcessEvent( event );
}

void 
vdCocoaDisplayCallback(void)
{
    if(Global::IsExiting)
        return;

    if(Global::ActiveWindow) 
    {
        Event event;
        event.Kind = Event::Type::Display;
        event.Update.Time = Core::Process::GetTimeInSeconds();
        Global::ActiveWindow->ProcessEvent( event );
    }   
}

void 
vdCocoaReshapeCallback(int width, int height)
{
    Event event;
    event.Size.Width = width;
    event.Size.Height = height;
    event.Kind = Event::Type::Resize;
    if(Global::ActiveWindow) 
        Global::ActiveWindow->ProcessEvent( event );
}

void 
vdCocoaKeyboardCallback(unsigned char key, int x, int y)
{
    Event event;
/*
    bool ctrl = glutGetModifiers() & COCOA_ACTIVE_CTRL ? true : false;
    bool shift = glutGetModifiers() & COCOA_ACTIVE_SHIFT ? true : false;
    bool alt = glutGetModifiers() & COCOA_ACTIVE_ALT ? true : false;
*/  
    event.Kind = Event::Type::KeyDown;
    event.Key.Control = ctrl;
    event.Key.Alt = alt;
    event.Key.Shift = shift;
    event.Key.Code = vdCocoaTranslateAsciiKey(key);
    
    if(key >= 'A' && key <= 'Z')
        event.Key.Shift = true;
    else if(key >= 'a' && key <= 'z')
        event.Key.Shift = false;
    
    if(Global::ActiveWindow) 
        Global::ActiveWindow->ProcessEvent( event );
}

void 
vdCocoaKeyboardUpCallback(unsigned char key, int x, int y)
{
    Event event;
/*
    bool ctrl = glutGetModifiers() & COCOA_ACTIVE_CTRL ? true : false;
    bool shift = glutGetModifiers() & COCOA_ACTIVE_SHIFT ? true : false;
    bool alt = glutGetModifiers() & COCOA_ACTIVE_ALT ? true : false;
*/  
    event.Kind = Event::Type::KeyUp;
    event.Key.Control = ctrl;
    event.Key.Alt = alt;
    event.Key.Shift = shift;
    event.Key.Code = vdCocoaTranslateAsciiKey(key);

    if(key >= 'A' && key <= 'Z')
        event.Key.Shift = true;
    else if(key >= 'a' && key <= 'z')
        event.Key.Shift = false;

    if(Global::ActiveWindow) 
        Global::ActiveWindow->ProcessEvent( event );
}

void 
vdCocoaPassiveMotionCallback(int x, int y)
{
    Event event;
    event.Kind = Event::Type::MouseMoved;
    event.MouseMove.X = x;
    event.MouseMove.Y = y;
    if(Global::ActiveWindow) 
        Global::ActiveWindow->ProcessEvent( event );
}

void 
vdCocoaEntryCallback(int state)
{
    Event event;
    if(state == COCOA_ENTERED)
        event.Kind = Event::Type::MouseEntered;
    else if(state == COCOA_LEFT)
        event.Kind = Event::Type::MouseExited;
    if(Global::ActiveWindow) 
        Global::ActiveWindow->ProcessEvent( event );
}

void
vdCocoaSpecialKeyDownCallback(int key, int x, int y)
{
    Event event;
/*
    bool ctrl = glutGetModifiers() & COCOA_ACTIVE_CTRL ? true : false;
    bool shift = glutGetModifiers() & COCOA_ACTIVE_SHIFT ? true : false;
    bool alt = glutGetModifiers() & COCOA_ACTIVE_ALT ? true : false;
*/  
    event.Kind = Event::Type::KeyDown;
    event.Key.Control = ctrl;
    event.Key.Alt = alt;
    event.Key.Shift = shift;
    event.Key.Code = vdCocoaTranslateSpecialKey(key);

    if(Global::ActiveWindow) Global::ActiveWindow->ProcessEvent( event );
}

void
vdCocoaSpecialKeyUpCallback(int key, int x, int y)
{
    Event event;
/*
    bool ctrl = glutGetModifiers() & COCOA_ACTIVE_CTRL ? true : false;
    bool shift = glutGetModifiers() & COCOA_ACTIVE_SHIFT ? true : false;
    bool alt = glutGetModifiers() & COCOA_ACTIVE_ALT ? true : false;
*/
    event.Kind = Event::Type::KeyDown;
    event.Key.Control = ctrl;
    event.Key.Alt = alt;
    event.Key.Shift = shift;
    event.Key.Code = vdCocoaTranslateSpecialKey(key);

    if(Global::ActiveWindow) Global::ActiveWindow->ProcessEvent( event );
}

void 
vdCocoaVisibilityCallback(int state)
{
    Event event;

    if(state == COCOA_NOT_VISIBLE)
    {
        event.Kind = Event::Type::LostFocus;
    }
    else if(state == COCOA_VISIBLE)
    {
        event.Kind = Event::Type::GainedFocus;
    
    }
    if(Global::ActiveWindow) Global::ActiveWindow->ProcessEvent( event );
}

void
vdCocoaIdleCallback(void)
{   
    vd::u64 IdleCallbackCount = 0;
    double LastCallbackTime = 0;
    
    if(Global::IsExiting)
    {
        vdCocoaShutdown();
        exit(0);
        return;
    }
        
    if(Global::ActiveWindow->GetFrameRateLimit() < 1 || LastCallbackTime < Core::Process::GetTimeInSeconds())
    {
        Event event;
        event.Kind = Event::Type::Update;
        event.Update.Time = Core::Process::GetTimeInSeconds();
        if(Global::ActiveWindow)
        {
            Global::ActiveWindow->ProcessEvent( event );
            LastCallbackTime = Core::Process::GetTimeInSeconds() + 1.0f / Global::ActiveWindow->GetFrameRateLimit();
            IdleCallbackCount = 0;
        }
    }
}

// ============================================================================================== //

#ifdef __cplusplus
}
#endif

// ============================================================================================== //
