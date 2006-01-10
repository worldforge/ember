//
// C++ Implementation: GUICEGUIAdapter
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#include "GUICEGUIAdapter.h"

#include <CEGUIExceptions.h>

namespace EmberOgre {

GUICEGUIAdapter::GUICEGUIAdapter(CEGUI::System *system, CEGUI::OgreCEGUIRenderer *renderer): 
mGuiSystem(system)
, mGuiRenderer(renderer)
{

	//lookup table for sdl scancodes and CEGUI keys
	mKeyMap[SDLK_BACKSPACE] = CEGUI::Key::Backspace;
	mKeyMap[SDLK_TAB] = CEGUI::Key::Tab;
/*	mKeyMap[SDLK_CLEAR] = CEGUI::Key::Clear;*/
	mKeyMap[SDLK_RETURN] = CEGUI::Key::Return;
	mKeyMap[SDLK_PAUSE] = CEGUI::Key::Pause;
	mKeyMap[SDLK_ESCAPE] = CEGUI::Key::Escape;
	mKeyMap[SDLK_SPACE] = CEGUI::Key::Space;
/*	mKeyMap[SDLK_EXCLAIM] = CEGUI::Key::Exclaim;*/
/*	mKeyMap[SDLK_QUOTEDBL] = CEGUI::Key::;
	mKeyMap[SDLK_HASH] = CEGUI::Key::;
	mKeyMap[SDLK_DOLLAR] = CEGUI::Key::;
	mKeyMap[SDLK_AMPERSAND] = CEGUI::Key::;
	mKeyMap[SDLK_QUOTE] = CEGUI::Key::;
	mKeyMap[SDLK_LEFTPAREN] = CEGUI::Key::;
	mKeyMap[SDLK_RIGHTPAREN] = CEGUI::Key::;
	mKeyMap[SDLK_ASTERISK] = CEGUI::Key::;*/
	mKeyMap[SDLK_PLUS] = CEGUI::Key::Add;
/*	mKeyMap[SDLK_COMMA] = CEGUI::Key::;*/
	mKeyMap[SDLK_MINUS] = CEGUI::Key::Minus;
	mKeyMap[SDLK_PERIOD] = CEGUI::Key::Period;
/*	mKeyMap[SDLK_SLASH] = CEGUI::Key::;*/
	mKeyMap[SDLK_0] = CEGUI::Key::One;
	mKeyMap[SDLK_1] = CEGUI::Key::Two;
	mKeyMap[SDLK_2] = CEGUI::Key::Two;
	mKeyMap[SDLK_3] = CEGUI::Key::Three;
	mKeyMap[SDLK_4] = CEGUI::Key::Four;
	mKeyMap[SDLK_5] = CEGUI::Key::Five;
	mKeyMap[SDLK_6] = CEGUI::Key::Six;
	mKeyMap[SDLK_7] = CEGUI::Key::Seven;
	mKeyMap[SDLK_8] = CEGUI::Key::Eight;
	mKeyMap[SDLK_9] = CEGUI::Key::Nine;
	mKeyMap[SDLK_COLON] = CEGUI::Key::Colon;
	mKeyMap[SDLK_SEMICOLON] = CEGUI::Key::Semicolon;
/*	mKeyMap[SDLK_LESS] = CEGUI::Key::;*/
/*	mKeyMap[SDLK_EQUALS] = CEGUI::Key::;
	mKeyMap[SDLK_GREATER] = CEGUI::Key::;
	mKeyMap[SDLK_QUESTION] = CEGUI::Key::;*/
/*	mKeyMap[SDLK_AT] = CEGUI::Key::;*/
/*	mKeyMap[SDLK_LEFTBRACKET] = CEGUI::Key::;*/
	mKeyMap[SDLK_BACKSLASH] = CEGUI::Key::Backslash;
/*	mKeyMap[SDLK_RIGHTBRACKET] = CEGUI::Key::;*/
/*	mKeyMap[SDLK_CARET] = CEGUI::Key::;
	mKeyMap[SDLK_UNDERSCORE] = CEGUI::Key::;
	mKeyMap[SDLK_BACKQUOTE] = CEGUI::Key::;*/
	mKeyMap[SDLK_a] = CEGUI::Key::A;
	mKeyMap[SDLK_b] = CEGUI::Key::B;
	mKeyMap[SDLK_c] = CEGUI::Key::C;
	mKeyMap[SDLK_d] = CEGUI::Key::D;
	mKeyMap[SDLK_e] = CEGUI::Key::E;
	mKeyMap[SDLK_f] = CEGUI::Key::F;
	mKeyMap[SDLK_g] = CEGUI::Key::G;
	mKeyMap[SDLK_h] = CEGUI::Key::H;
	mKeyMap[SDLK_i] = CEGUI::Key::I;
	mKeyMap[SDLK_j] = CEGUI::Key::J;
	mKeyMap[SDLK_k] = CEGUI::Key::K;
	mKeyMap[SDLK_l] = CEGUI::Key::L;
	mKeyMap[SDLK_m] = CEGUI::Key::M;
	mKeyMap[SDLK_n] = CEGUI::Key::N;
	mKeyMap[SDLK_o] = CEGUI::Key::O;
	mKeyMap[SDLK_p] = CEGUI::Key::P;
	mKeyMap[SDLK_q] = CEGUI::Key::Q;
	mKeyMap[SDLK_r] = CEGUI::Key::R;
	mKeyMap[SDLK_s] = CEGUI::Key::S;
	mKeyMap[SDLK_t] = CEGUI::Key::T;
	mKeyMap[SDLK_u] = CEGUI::Key::U;
	mKeyMap[SDLK_v] = CEGUI::Key::V;
	mKeyMap[SDLK_w] = CEGUI::Key::W;
	mKeyMap[SDLK_x] = CEGUI::Key::X;
	mKeyMap[SDLK_y] = CEGUI::Key::Y;
	mKeyMap[SDLK_z] = CEGUI::Key::Z;
	mKeyMap[SDLK_DELETE] = CEGUI::Key::Delete;
	mKeyMap[SDLK_UP] = CEGUI::Key::ArrowUp;
	mKeyMap[SDLK_DOWN] = CEGUI::Key::ArrowDown;
	mKeyMap[SDLK_RIGHT] = CEGUI::Key::ArrowRight;
	mKeyMap[SDLK_LEFT] = CEGUI::Key::ArrowLeft;
	mKeyMap[SDLK_INSERT] = CEGUI::Key::Insert;
	mKeyMap[SDLK_HOME] = CEGUI::Key::Home;
	mKeyMap[SDLK_END] = CEGUI::Key::End;
	mKeyMap[SDLK_PAGEUP] = CEGUI::Key::PageUp;
	mKeyMap[SDLK_PAGEDOWN] = CEGUI::Key::PageDown;
	mKeyMap[SDLK_F1] = CEGUI::Key::F1;
	mKeyMap[SDLK_F2] = CEGUI::Key::F2;
	mKeyMap[SDLK_F3] = CEGUI::Key::F3;
	mKeyMap[SDLK_F4] = CEGUI::Key::F4;
	mKeyMap[SDLK_F5] = CEGUI::Key::F5;
	mKeyMap[SDLK_F6] = CEGUI::Key::F6;
	mKeyMap[SDLK_F7] = CEGUI::Key::F7;
	mKeyMap[SDLK_F8] = CEGUI::Key::F8;
	mKeyMap[SDLK_F9] = CEGUI::Key::F9;
	mKeyMap[SDLK_F10] = CEGUI::Key::F10;
	mKeyMap[SDLK_F11] = CEGUI::Key::F11;
	mKeyMap[SDLK_F12] = CEGUI::Key::F12;
	
	
}


GUICEGUIAdapter::~GUICEGUIAdapter()
{
}

bool GUICEGUIAdapter::injectMouseMove(const MouseMotion& motion, bool& freezeMouse)
{
	try {
		CEGUI::MouseCursor::getSingleton().setPosition(CEGUI::Point((motion.xPosition), (motion.yPosition))); 
		mGuiSystem->injectMouseMove(0.0f, 0.0f);
	} catch (const CEGUI::Exception& ex) {
		S_LOG_WARNING("Error in CEGUI: " << ex.getMessage().c_str());
	}
	return true;
}

bool GUICEGUIAdapter::injectMouseButtonUp(const Input::MouseButton& button)
{
	CEGUI::MouseButton ceguiButton;
	if (button == Input::MouseButtonLeft) {
		ceguiButton = CEGUI::LeftButton;
	} else if(button == Input::MouseButtonMiddle) {
		ceguiButton = CEGUI::MiddleButton;
	} else if(button == Input::MouseButtonRight) {
		ceguiButton = CEGUI::RightButton;
	} else {
		return true;
	}
	
	try {
		mGuiSystem->injectMouseButtonUp(ceguiButton);
	} catch (const CEGUI::Exception& ex) {
		S_LOG_WARNING("Error in CEGUI: " << ex.getMessage().c_str());
	}
	return true;
}

bool GUICEGUIAdapter::injectMouseButtonDown(const Input::MouseButton& button)
{
	CEGUI::MouseButton ceguiButton;
	if (button == Input::MouseButtonLeft) {
		ceguiButton = CEGUI::LeftButton;
	} else if(button == Input::MouseButtonMiddle) {
		ceguiButton = CEGUI::MiddleButton;
	} else if(button == Input::MouseButtonRight) {
		ceguiButton = CEGUI::RightButton;
	} else {
		return true;
	}
	
	try {
		mGuiSystem->injectMouseButtonDown(ceguiButton);
	} catch (const CEGUI::Exception& ex) {
		S_LOG_WARNING("Error in CEGUI: " << ex.getMessage().c_str());
	}
	return true;
}

bool GUICEGUIAdapter::injectChar(char character)
{
	try {
		//cegui can't handle tabs, so we have to convert it to a couple of spaces
		if (character == '\t') {
			mGuiSystem->injectChar(' ');
			mGuiSystem->injectChar(' ');
			mGuiSystem->injectChar(' ');
			mGuiSystem->injectChar(' ');
		//can't handle CR either really, iosert a line break (0x0a) instead
		} else if (character == '\r') {
			mGuiSystem->injectChar(0x0a);
		} else {
			mGuiSystem->injectChar(character);
		}
	} catch (const CEGUI::Exception& ex) {
		S_LOG_WARNING("Error in CEGUI: " << ex.getMessage().c_str());
	}
	return true;

}

bool GUICEGUIAdapter::injectKeyDown(const SDLKey& key)
{
	try {
		mGuiSystem->injectKeyDown(mKeyMap[key]);
	} catch (const CEGUI::Exception& ex) {
		S_LOG_WARNING("Error in CEGUI: " << ex.getMessage().c_str());
	}
	return true;

}

bool GUICEGUIAdapter::injectKeyUp(const SDLKey& key)
{
	try {
		mGuiSystem->injectKeyUp(mKeyMap[key]);
	} catch (const CEGUI::Exception& ex) {
		S_LOG_WARNING("Error in CEGUI: " << ex.getMessage().c_str());
	}
	return true;
	
}



}
