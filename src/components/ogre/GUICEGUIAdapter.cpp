//
// C++ Implementation: GUICEGUIAdapter
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2005
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "GUICEGUIAdapter.h"

#include <CEGUI/Exceptions.h>
#include <CEGUI/GlobalEventSet.h>
#include <CEGUI/widgets/Editbox.h>
#include <CEGUI/widgets/MultiLineEditbox.h>

namespace Ember {
namespace OgreView {

GUICEGUIAdapter::GUICEGUIAdapter(CEGUI::System *system, CEGUI::OgreRenderer *renderer):
mGuiSystem(system)
, mGuiRenderer(renderer)
, mGuiContext(system->getDefaultGUIContext())
{

	//lookup table for sdl scancodes and CEGUI keys
	mKeyMap[SDL_SCANCODE_BACKSPACE] = CEGUI::Key::Backspace;
	mKeyMap[SDL_SCANCODE_TAB] = CEGUI::Key::Tab;
/*	mKeyMap[SDL_SCANCODE_CLEAR] = CEGUI::Key::Clear;*/
	mKeyMap[SDL_SCANCODE_RETURN] = CEGUI::Key::Return;
	mKeyMap[SDL_SCANCODE_PAUSE] = CEGUI::Key::Pause;
	mKeyMap[SDL_SCANCODE_ESCAPE] = CEGUI::Key::Escape;
	mKeyMap[SDL_SCANCODE_SPACE] = CEGUI::Key::Space;
/*	mKeyMap[SDL_SCANCODE_EXCLAIM] = CEGUI::Key::Exclaim;*/
/*	mKeyMap[SDL_SCANCODE_QUOTEDBL] = CEGUI::Key::;
	mKeyMap[SDL_SCANCODE_HASH] = CEGUI::Key::;
	mKeyMap[SDL_SCANCODE_DOLLAR] = CEGUI::Key::;
	mKeyMap[SDL_SCANCODE_AMPERSAND] = CEGUI::Key::;
	mKeyMap[SDL_SCANCODE_QUOTE] = CEGUI::Key::;
	mKeyMap[SDL_SCANCODE_LEFTPAREN] = CEGUI::Key::;
	mKeyMap[SDL_SCANCODE_RIGHTPAREN] = CEGUI::Key::;
	mKeyMap[SDL_SCANCODE_ASTERISK] = CEGUI::Key::;*/
	mKeyMap[SDL_SCANCODE_KP_PLUS] = CEGUI::Key::Add;
/*	mKeyMap[SDL_SCANCODE_COMMA] = CEGUI::Key::;*/
	mKeyMap[SDL_SCANCODE_MINUS] = CEGUI::Key::Minus;
	mKeyMap[SDL_SCANCODE_PERIOD] = CEGUI::Key::Period;
/*	mKeyMap[SDL_SCANCODE_SLASH] = CEGUI::Key::;*/
	mKeyMap[SDL_SCANCODE_0] = CEGUI::Key::One;
	mKeyMap[SDL_SCANCODE_1] = CEGUI::Key::Two;
	mKeyMap[SDL_SCANCODE_2] = CEGUI::Key::Two;
	mKeyMap[SDL_SCANCODE_3] = CEGUI::Key::Three;
	mKeyMap[SDL_SCANCODE_4] = CEGUI::Key::Four;
	mKeyMap[SDL_SCANCODE_5] = CEGUI::Key::Five;
	mKeyMap[SDL_SCANCODE_6] = CEGUI::Key::Six;
	mKeyMap[SDL_SCANCODE_7] = CEGUI::Key::Seven;
	mKeyMap[SDL_SCANCODE_8] = CEGUI::Key::Eight;
	mKeyMap[SDL_SCANCODE_9] = CEGUI::Key::Nine;
//	mKeyMap[SDL_SCANCODE_COLON] = CEGUI::Key::Colon;
	mKeyMap[SDL_SCANCODE_SEMICOLON] = CEGUI::Key::Semicolon;
/*	mKeyMap[SDL_SCANCODE_LESS] = CEGUI::Key::;*/
/*	mKeyMap[SDL_SCANCODE_EQUALS] = CEGUI::Key::;
	mKeyMap[SDL_SCANCODE_GREATER] = CEGUI::Key::;
	mKeyMap[SDL_SCANCODE_QUESTION] = CEGUI::Key::;*/
/*	mKeyMap[SDL_SCANCODE_AT] = CEGUI::Key::;*/
/*	mKeyMap[SDL_SCANCODE_LEFTBRACKET] = CEGUI::Key::;*/
	mKeyMap[SDL_SCANCODE_BACKSLASH] = CEGUI::Key::Backslash;
/*	mKeyMap[SDL_SCANCODE_RIGHTBRACKET] = CEGUI::Key::;*/
/*	mKeyMap[SDL_SCANCODE_CARET] = CEGUI::Key::;
	mKeyMap[SDL_SCANCODE_UNDERSCORE] = CEGUI::Key::;
	mKeyMap[SDL_SCANCODE_BACKQUOTE] = CEGUI::Key::;*/
	mKeyMap[SDL_SCANCODE_A] = CEGUI::Key::A;
	mKeyMap[SDL_SCANCODE_B] = CEGUI::Key::B;
	mKeyMap[SDL_SCANCODE_C] = CEGUI::Key::C;
	mKeyMap[SDL_SCANCODE_D] = CEGUI::Key::D;
	mKeyMap[SDL_SCANCODE_E] = CEGUI::Key::E;
	mKeyMap[SDL_SCANCODE_F] = CEGUI::Key::F;
	mKeyMap[SDL_SCANCODE_G] = CEGUI::Key::G;
	mKeyMap[SDL_SCANCODE_H] = CEGUI::Key::H;
	mKeyMap[SDL_SCANCODE_I] = CEGUI::Key::I;
	mKeyMap[SDL_SCANCODE_J] = CEGUI::Key::J;
	mKeyMap[SDL_SCANCODE_K] = CEGUI::Key::K;
	mKeyMap[SDL_SCANCODE_L] = CEGUI::Key::L;
	mKeyMap[SDL_SCANCODE_M] = CEGUI::Key::M;
	mKeyMap[SDL_SCANCODE_N] = CEGUI::Key::N;
	mKeyMap[SDL_SCANCODE_O] = CEGUI::Key::O;
	mKeyMap[SDL_SCANCODE_P] = CEGUI::Key::P;
	mKeyMap[SDL_SCANCODE_Q] = CEGUI::Key::Q;
	mKeyMap[SDL_SCANCODE_R] = CEGUI::Key::R;
	mKeyMap[SDL_SCANCODE_S] = CEGUI::Key::S;
	mKeyMap[SDL_SCANCODE_T] = CEGUI::Key::T;
	mKeyMap[SDL_SCANCODE_U] = CEGUI::Key::U;
	mKeyMap[SDL_SCANCODE_V] = CEGUI::Key::V;
	mKeyMap[SDL_SCANCODE_W] = CEGUI::Key::W;
	mKeyMap[SDL_SCANCODE_X] = CEGUI::Key::X;
	mKeyMap[SDL_SCANCODE_Y] = CEGUI::Key::Y;
	mKeyMap[SDL_SCANCODE_Z] = CEGUI::Key::Z;
	mKeyMap[SDL_SCANCODE_DELETE] = CEGUI::Key::Delete;
	mKeyMap[SDL_SCANCODE_UP] = CEGUI::Key::ArrowUp;
	mKeyMap[SDL_SCANCODE_DOWN] = CEGUI::Key::ArrowDown;
	mKeyMap[SDL_SCANCODE_RIGHT] = CEGUI::Key::ArrowRight;
	mKeyMap[SDL_SCANCODE_LEFT] = CEGUI::Key::ArrowLeft;
	mKeyMap[SDL_SCANCODE_INSERT] = CEGUI::Key::Insert;
	mKeyMap[SDL_SCANCODE_HOME] = CEGUI::Key::Home;
	mKeyMap[SDL_SCANCODE_END] = CEGUI::Key::End;
	mKeyMap[SDL_SCANCODE_PAGEUP] = CEGUI::Key::PageUp;
	mKeyMap[SDL_SCANCODE_PAGEDOWN] = CEGUI::Key::PageDown;
	mKeyMap[SDL_SCANCODE_F1] = CEGUI::Key::F1;
	mKeyMap[SDL_SCANCODE_F2] = CEGUI::Key::F2;
	mKeyMap[SDL_SCANCODE_F3] = CEGUI::Key::F3;
	mKeyMap[SDL_SCANCODE_F4] = CEGUI::Key::F4;
	mKeyMap[SDL_SCANCODE_F5] = CEGUI::Key::F5;
	mKeyMap[SDL_SCANCODE_F6] = CEGUI::Key::F6;
	mKeyMap[SDL_SCANCODE_F7] = CEGUI::Key::F7;
	mKeyMap[SDL_SCANCODE_F8] = CEGUI::Key::F8;
	mKeyMap[SDL_SCANCODE_F9] = CEGUI::Key::F9;
	mKeyMap[SDL_SCANCODE_F10] = CEGUI::Key::F10;
	mKeyMap[SDL_SCANCODE_F11] = CEGUI::Key::F11;
	mKeyMap[SDL_SCANCODE_F12] = CEGUI::Key::F12;
	mKeyMap[SDL_SCANCODE_F13] = CEGUI::Key::F13;
	mKeyMap[SDL_SCANCODE_F14] = CEGUI::Key::F14;
	mKeyMap[SDL_SCANCODE_F15] = CEGUI::Key::F15;
	mKeyMap[SDL_SCANCODE_NUMLOCKCLEAR] = CEGUI::Key::NumLock;
	mKeyMap[SDL_SCANCODE_SCROLLLOCK] = CEGUI::Key::ScrollLock;
	mKeyMap[SDL_SCANCODE_RSHIFT] = CEGUI::Key::RightShift;
	mKeyMap[SDL_SCANCODE_LSHIFT] = CEGUI::Key::LeftShift;
	mKeyMap[SDL_SCANCODE_RCTRL] = CEGUI::Key::RightControl;
	mKeyMap[SDL_SCANCODE_LCTRL] = CEGUI::Key::LeftControl;
	mKeyMap[SDL_SCANCODE_RALT] = CEGUI::Key::RightAlt;
	mKeyMap[SDL_SCANCODE_LALT] = CEGUI::Key::LeftAlt;


}


GUICEGUIAdapter::~GUICEGUIAdapter()
{
}

bool GUICEGUIAdapter::injectMouseMove(const MouseMotion& motion, bool& freezeMouse)
{
	try {
		mGuiContext.injectMousePosition(motion.xPosition, motion.yPosition);
	} catch (const CEGUI::Exception& ex) {
		S_LOG_WARNING("Error in CEGUI." << ex);
	}
	return true;
}

bool GUICEGUIAdapter::injectMouseButtonUp(Input::MouseButton button)
{
	CEGUI::MouseButton ceguiButton;
	if (button == Input::MouseButtonLeft) {
		ceguiButton = CEGUI::LeftButton;
	} else if(button == Input::MouseButtonRight) {
		ceguiButton = CEGUI::RightButton;
	} else if(button == Input::MouseButtonMiddle) {
		ceguiButton = CEGUI::MiddleButton;
	} else {
		return true;
	}

	try {
		mGuiContext.injectMouseButtonUp(ceguiButton);
		return false;
	} catch (const std::exception& e) {
		S_LOG_WARNING("Error in CEGUI." << e);
	} catch (...) {
		S_LOG_WARNING("Unknown error in CEGUI.");
	}
	return true;
}

bool GUICEGUIAdapter::injectMouseButtonDown(Input::MouseButton button)
{
	CEGUI::MouseButton ceguiButton(CEGUI::LeftButton);
	if (button == Input::MouseButtonLeft) {
		ceguiButton = CEGUI::LeftButton;
	} else if(button == Input::MouseButtonRight) {
		ceguiButton = CEGUI::RightButton;
	} else if(button == Input::MouseButtonMiddle) {
		ceguiButton = CEGUI::MiddleButton;
	} else if(button == Input::MouseWheelDown) {
		try {
			mGuiContext.injectMouseWheelChange(-1.0);
		} catch (const CEGUI::Exception& ex) {
			S_LOG_WARNING("Error in CEGUI." << ex);
		}
		return false;
	} else if(button == Input::MouseWheelUp) {
		try {
			mGuiContext.injectMouseWheelChange(1.0);
		} catch (const CEGUI::Exception& ex) {
			S_LOG_WARNING("Error in CEGUI." << ex);
		}
		return false;
	} else {
		return true;
	}

	try {
		mGuiContext.injectMouseButtonDown(ceguiButton);
		return false;
	} catch (const CEGUI::Exception& ex) {
		S_LOG_WARNING("Error in CEGUI." << ex);
	}
	return true;
}

bool GUICEGUIAdapter::injectChar(int character)
{
	try {
		//cegui can't handle tabs, so we have to convert it to a couple of spaces
		if (character == '\t') {
			mGuiContext.injectChar(' ');
			mGuiContext.injectChar(' ');
			mGuiContext.injectChar(' ');
			mGuiContext.injectChar(' ');
		//can't handle CR either really, insert a line break (0x0a) instead
		} else if (character == '\r') {
 			//mGuiContext.injectChar(0x0a);
 			mGuiContext.injectKeyDown(CEGUI::Key::Return);
 			mGuiContext.injectKeyUp(CEGUI::Key::Return);
		} else {
			mGuiContext.injectChar(character);
		}
	} catch (const CEGUI::Exception& ex) {
		S_LOG_WARNING("Error in CEGUI." << ex);
	}
	return true;

}

bool GUICEGUIAdapter::injectKeyDown(const SDL_Scancode& key)
{
	try {
		SDLKeyMap::const_iterator I =  mKeyMap.find(key);
		if (I != mKeyMap.end())  {
			const auto& scanCode = I->second;
			mGuiContext.injectKeyDown(scanCode);
		}
	} catch (const CEGUI::Exception& ex) {
		S_LOG_WARNING("Error in CEGUI." << ex);
	}
	return true;

}

bool GUICEGUIAdapter::injectKeyUp(const SDL_Scancode& key)
{
	try {
		SDLKeyMap::const_iterator I =  mKeyMap.find(key);
		if (I != mKeyMap.end())  {
			const auto& scanCode = I->second;
			mGuiContext.injectKeyUp(scanCode);
		}
	} catch (const CEGUI::Exception& ex) {
		S_LOG_WARNING("Error in CEGUI." << ex);
	}
	return true;

}



}
}
