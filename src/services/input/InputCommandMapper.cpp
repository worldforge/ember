//
// C++ Implementation: InputCommandMapper
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
// Copyright (C) 2001 - 2005 Simon Goodall, University of Southampton
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "InputCommandMapper.h"
#include "framework/ConsoleBackend.h"
#include "services/config/ConfigService.h"
#include "framework/LoggingInstance.h"

#include <SDL_keyboard.h>

#include <algorithm>

namespace Ember {


InputCommandMapper::InputCommandMapper(const std::string& state) : mState(state), mEnabled(true), mInput(0)
{
	initKeyMap();
}

InputCommandMapper::InputCommandMapper(const std::string& state, const std::string& configSection) : mState(state), mEnabled(true), mInput(0)
{
	initKeyMap();
	readFromConfigSection(configSection);
}

InputCommandMapper::~InputCommandMapper()
{
	if (mInput) {
		mInput->deregisterCommandMapper(this);
	}
}

void InputCommandMapper::readFromConfigSection(const std::string& sectionName)
{

	//get the mappings from the config service
	ConfigService::SectionMap section = EmberServices::getSingleton().getConfigService().getSection(sectionName);

	for (ConfigService::SectionMap::const_iterator I = section.begin(); I != section.end(); ++I) {
		bindCommand(std::string(I->first), std::string(I->second));
	}

}

void InputCommandMapper::Input_EventKeyPressed(const SDL_Keysym& key, Input::InputMode inputMode)
{
	if (mEnabled && isActiveForInputMode(inputMode)) {
		//check if we have any key with a matching command
		KeyMapStore::const_iterator keyI = mKeymap.find(key.scancode);
		if (keyI != mKeymap.end()) {
			std::pair<KeyCommandStore::iterator, KeyCommandStore::iterator> commandsI = mKeyCommands.equal_range(keyI->second);
			for (KeyCommandStore::iterator I = commandsI.first; I != commandsI.second; ++I) {
				const std::string& command(I->second);
				if (command != "") {
					ConsoleBackend& myBackend = ConsoleBackend::getSingleton();
					myBackend.runCommand(command, false);
				}
			}
		}
	}
}


void InputCommandMapper::Input_EventKeyReleased(const SDL_Keysym& key, Input::InputMode inputMode)
{
	if (mEnabled && isActiveForInputMode(inputMode)) {
		//check if we have any key with a matching command
		//only check for commands that start with a "+"
		KeyMapStore::const_iterator keyI = mKeymap.find(key.scancode);
		if (keyI != mKeymap.end()) {
			std::pair<KeyCommandStore::iterator, KeyCommandStore::iterator> commandsI = mKeyCommands.equal_range(keyI->second);
			for (KeyCommandStore::iterator I = commandsI.first; I != commandsI.second; ++I) {
				const std::string& command(I->second);
				if (command != "") {
					if (command[0] == '+') {
						ConsoleBackend& myBackend = ConsoleBackend::getSingleton();
						//remove the "+" and replace it with "-"
						myBackend.runCommand("-" + std::string(command).erase(0, 1), false);
					}
				}
			}
		}
	}
}

void InputCommandMapper::bindToInput(Input& input)
{
	input.EventKeyPressed.connect(sigc::mem_fun(*this, &InputCommandMapper::Input_EventKeyPressed));
	input.EventKeyReleased.connect(sigc::mem_fun(*this, &InputCommandMapper::Input_EventKeyReleased));
	input.registerCommandMapper(this);
	mInput = &input;
}

bool InputCommandMapper::isActiveForInputMode(Input::InputMode mode) const
{
	//if there's no restriction, return true
	if (mInputModesRestriction.size() == 0) {
		return true;
	} else {
		return std::find(mInputModesRestriction.begin(), mInputModesRestriction.end(), mode) != mInputModesRestriction.end();
	}
}


void InputCommandMapper::bindCommand(const std::string& key, const std::string& command)
{
	S_LOG_INFO("Binding key " << key << " to command " << command << " for state " << getState() << ".");
	mKeyCommands.insert(KeyCommandStore::value_type(key, command));
}

void InputCommandMapper::unbindCommand(const std::string& key)
{
	S_LOG_INFO("Unbinding key " << key << " for state " << getState() << ".");
	mKeyCommands.erase(key);
}

void InputCommandMapper::unbindCommand(const std::string& key, const std::string& command)
{
	std::pair<KeyCommandStore::iterator, KeyCommandStore::iterator> commandsI = mKeyCommands.equal_range(key);
	for (KeyCommandStore::iterator I = commandsI.first; I != commandsI.second; ++I) {
		if (I->second == command) {
			mKeyCommands.erase(I);
		}
	}
}


//const std::string& InputCommandMapper::getCommandForKey(SDLKey key)
//{
//	KeyMapStore::const_iterator I = mKeymap.find(key);
//	if (I != mKeymap.end()) {
//		KeyCommandStore::const_iterator J = mKeyCommands.find(I->second);
//		if (J != mKeyCommands.end()) {
//			return J->second;
//		}
//	}
//	//if we don't find anything, return an empty string
//	static std::string empty("");
//	return empty;
//}


// std::string Bindings::getBindingForKeysym(const SDL_Keysym& key) {
//
//   std::map<int, std::string>::const_iterator I = m_keymap.find(key.sym);
//   if (I == m_keymap.end()) return ""; // un-mapped basic keysym
//   const std::string & plainName = I->second;
//   std::string decoratedName = plainName;
//
//   if (key.mod & KMOD_SHIFT)
//     decoratedName = "shift_" + decoratedName;
//
//   if (key.mod & KMOD_ALT)
//     decoratedName = "alt_" + decoratedName;
//
//   if (key.mod & KMOD_CTRL)
//     decoratedName = "ctrl_" + decoratedName;
//
//   m_bindings->clean(decoratedName);
//   if (m_bindings->findItem("key_bindings", decoratedName))
//     return m_bindings->getItem("key_bindings", decoratedName);
//
//   if (m_bindings->findItem("key_bindings", plainName))
//     return m_bindings->getItem("key_bindings", plainName);
//
//   std::cout << "no binding specified for key " << decoratedName << std::endl;
//   return "";
// }

void InputCommandMapper::initKeyMap() {
  // Assign keys to textual representation
  mKeymap[SDL_SCANCODE_BACKSPACE] = "backspace";
  mKeymap[SDL_SCANCODE_TAB] = "tab";
  mKeymap[SDL_SCANCODE_CLEAR] = "clear";
  mKeymap[SDL_SCANCODE_RETURN] = "return";
  mKeymap[SDL_SCANCODE_PAUSE] = "pause";
  mKeymap[SDL_SCANCODE_ESCAPE] = "escape";
  mKeymap[SDL_SCANCODE_SPACE] = "space";
  mKeymap[SDL_SCANCODE_KP_HASH] = "hash";
  mKeymap[SDL_SCANCODE_KP_AMPERSAND] = "ampersand";
  mKeymap[SDL_SCANCODE_KP_LEFTPAREN] = "left_paren";
  mKeymap[SDL_SCANCODE_KP_RIGHTPAREN] = "right_paren";
  mKeymap[SDL_SCANCODE_KP_PLUS] = "plus";
  mKeymap[SDL_SCANCODE_COMMA] = "comma";
  mKeymap[SDL_SCANCODE_MINUS] = "-";
  mKeymap[SDL_SCANCODE_PERIOD] = "period";
  mKeymap[SDL_SCANCODE_SLASH] = "slash";
  mKeymap[SDL_SCANCODE_0] = "0";
  mKeymap[SDL_SCANCODE_1] = "1";
  mKeymap[SDL_SCANCODE_2] = "2";
  mKeymap[SDL_SCANCODE_3] = "3";
  mKeymap[SDL_SCANCODE_4] = "4";
  mKeymap[SDL_SCANCODE_5] = "5";
  mKeymap[SDL_SCANCODE_6] = "6";
  mKeymap[SDL_SCANCODE_7] = "7";
  mKeymap[SDL_SCANCODE_8] = "8";
  mKeymap[SDL_SCANCODE_9] = "9";
  mKeymap[SDL_SCANCODE_KP_COLON] = "colon";
  mKeymap[SDL_SCANCODE_SEMICOLON] = "semi_colon";
  mKeymap[SDL_SCANCODE_KP_LESS] = "less_than";
  mKeymap[SDL_SCANCODE_EQUALS] = "equals";
  mKeymap[SDL_SCANCODE_KP_GREATER] = "greater_then";
  mKeymap[SDL_SCANCODE_KP_AT] = "at";
  mKeymap[SDL_SCANCODE_LEFTBRACKET] = "left_brace";
  mKeymap[SDL_SCANCODE_BACKSLASH] = "backslash";
  mKeymap[SDL_SCANCODE_RIGHTBRACKET] = "right_brace";
  mKeymap[SDL_SCANCODE_A] = "a";
  mKeymap[SDL_SCANCODE_B] = "b";
  mKeymap[SDL_SCANCODE_C] = "c";
  mKeymap[SDL_SCANCODE_D] = "d";
  mKeymap[SDL_SCANCODE_E] = "e";
  mKeymap[SDL_SCANCODE_F] = "f";
  mKeymap[SDL_SCANCODE_G] = "g";
  mKeymap[SDL_SCANCODE_H] = "h";
  mKeymap[SDL_SCANCODE_I] = "i";
  mKeymap[SDL_SCANCODE_J] = "j";
  mKeymap[SDL_SCANCODE_K] = "k";
  mKeymap[SDL_SCANCODE_L] = "l";
  mKeymap[SDL_SCANCODE_M] = "m";
  mKeymap[SDL_SCANCODE_N] = "n";
  mKeymap[SDL_SCANCODE_O] = "o";
  mKeymap[SDL_SCANCODE_P] = "p";
  mKeymap[SDL_SCANCODE_Q] = "q";
  mKeymap[SDL_SCANCODE_R] = "r";
  mKeymap[SDL_SCANCODE_S] = "s";
  mKeymap[SDL_SCANCODE_T] = "t";
  mKeymap[SDL_SCANCODE_U] = "u";
  mKeymap[SDL_SCANCODE_V] = "v";
  mKeymap[SDL_SCANCODE_W] = "w";
  mKeymap[SDL_SCANCODE_X] = "x";
  mKeymap[SDL_SCANCODE_Y] = "y";
  mKeymap[SDL_SCANCODE_Z] = "z";
  mKeymap[SDL_SCANCODE_DELETE] = "delete";
  mKeymap[SDL_SCANCODE_KP_0] = "kp_0";
  mKeymap[SDL_SCANCODE_KP_1] = "kp_1";
  mKeymap[SDL_SCANCODE_KP_2] = "kp_2";
  mKeymap[SDL_SCANCODE_KP_3] = "kp_3";
  mKeymap[SDL_SCANCODE_KP_4] = "kp_4";
  mKeymap[SDL_SCANCODE_KP_5] = "kp_5";
  mKeymap[SDL_SCANCODE_KP_6] = "kp_6";
  mKeymap[SDL_SCANCODE_KP_7] = "kp_7";
  mKeymap[SDL_SCANCODE_KP_8] = "kp_8";
  mKeymap[SDL_SCANCODE_KP_9] = "kp_9";
  mKeymap[SDL_SCANCODE_KP_PERIOD] = "kp_period";
  mKeymap[SDL_SCANCODE_KP_DIVIDE] = "kp_divide";
  mKeymap[SDL_SCANCODE_KP_MULTIPLY] = "kp_multi";
  mKeymap[SDL_SCANCODE_KP_MINUS] = "kp_minus";
  mKeymap[SDL_SCANCODE_KP_PLUS] = "kp_plus";
  mKeymap[SDL_SCANCODE_KP_ENTER] = "kp_enter";
  mKeymap[SDL_SCANCODE_KP_EQUALS] = "kp_equals";
  mKeymap[SDL_SCANCODE_UP] = "up";
  mKeymap[SDL_SCANCODE_DOWN] = "down";
  mKeymap[SDL_SCANCODE_RIGHT] = "right";
  mKeymap[SDL_SCANCODE_LEFT] = "left";
  mKeymap[SDL_SCANCODE_INSERT] = "insert";
  mKeymap[SDL_SCANCODE_HOME] = "home";
  mKeymap[SDL_SCANCODE_END] = "end";
  mKeymap[SDL_SCANCODE_PAGEUP] = "page_up";
  mKeymap[SDL_SCANCODE_PAGEDOWN] = "page_down";
  mKeymap[SDL_SCANCODE_F1] = "f1";
  mKeymap[SDL_SCANCODE_F2] = "f2";
  mKeymap[SDL_SCANCODE_F3] = "f3";
  mKeymap[SDL_SCANCODE_F4] = "f4";
  mKeymap[SDL_SCANCODE_F5] = "f5";
  mKeymap[SDL_SCANCODE_F6] = "f6";
  mKeymap[SDL_SCANCODE_F7] = "f7";
  mKeymap[SDL_SCANCODE_F8] = "f8";
  mKeymap[SDL_SCANCODE_F9] = "f9";
  mKeymap[SDL_SCANCODE_F10] = "f10";
  mKeymap[SDL_SCANCODE_F11] = "f11";
  mKeymap[SDL_SCANCODE_F12] = "f12";
  mKeymap[SDL_SCANCODE_F13] = "f13";
  mKeymap[SDL_SCANCODE_F14] = "f14";
  mKeymap[SDL_SCANCODE_F15] = "f15";

  mKeymap[SDL_SCANCODE_NUMLOCKCLEAR] = "num";
  mKeymap[SDL_SCANCODE_CAPSLOCK] = "caps";
  mKeymap[SDL_SCANCODE_SCROLLLOCK] = "srcoll";
  mKeymap[SDL_SCANCODE_RSHIFT] = "right_shift";
  mKeymap[SDL_SCANCODE_LSHIFT] = "left_shift";
  mKeymap[SDL_SCANCODE_RCTRL] = "right_ctrl";
  mKeymap[SDL_SCANCODE_LCTRL] = "left_ctrl";
  mKeymap[SDL_SCANCODE_RALT] = "right_alt";
  mKeymap[SDL_SCANCODE_LALT] = "left_alt";
  mKeymap[SDL_SCANCODE_RGUI] = "right_meta";
  mKeymap[SDL_SCANCODE_LGUI] = "left_meta";
  mKeymap[SDL_SCANCODE_MODE]= "mode";
  mKeymap[SDL_SCANCODE_APPLICATION] = "compose";
  mKeymap[SDL_SCANCODE_PRINTSCREEN] = "print";
  mKeymap[SDL_SCANCODE_SYSREQ] = "sysreq";
  mKeymap[SDL_SCANCODE_MENU] = "menu";
  mKeymap[SDL_SCANCODE_POWER] = "power";
}

}
