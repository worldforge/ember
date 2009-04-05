//
// C++ Implementation: InputCommandMapper
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2006
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

#include <algorithm>
#include "InputCommandMapper.h"
#include "framework/ConsoleBackend.h"
#include "services/config/ConfigService.h"
#include "framework/LoggingInstance.h"

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
	
	///get the mappings from the config service
	const Ember::ConfigService::SectionMap& section = Ember::EmberServices::getSingleton().getConfigService()->getSection(sectionName);
	
	for (Ember::ConfigService::SectionMap::const_iterator I = section.begin(); I != section.end(); ++I) {
		bindCommand(std::string(I->first), std::string(I->second));
	}
	
}

void InputCommandMapper::Input_EventKeyPressed(const SDL_keysym& key, Input::InputMode inputMode)
{
	if (mEnabled && isActiveForInputMode(inputMode)) {
		///check if we have any key with a matching command
		const std::string& command = getCommandForKey(key.sym);
		if (command != "") {
			Ember::ConsoleBackend* myBackend = Ember::ConsoleBackend::getSingletonPtr();
			myBackend->runCommand(command, false);
		}
	}
}


void InputCommandMapper::Input_EventKeyReleased(const SDL_keysym& key, Input::InputMode inputMode)
{
	if (mEnabled && isActiveForInputMode(inputMode)) {
		///check if we have any key with a matching command
		///only check for commands that start with a "+"
		const std::string& command = getCommandForKey(key.sym);
		if (command != "") {
			Ember::ConsoleBackend* myBackend = Ember::ConsoleBackend::getSingletonPtr();
			if (command[0] == '+') {
				///remove the "+" and replace it with "-"
				myBackend->runCommand("-" + std::string(command).erase(0, 1), false);
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
	///if there's no restriction, return true
	if (mInputModesRestriction.size() == 0) {
		return true;
	} else {
		return std::find(mInputModesRestriction.begin(), mInputModesRestriction.end(), mode) != mInputModesRestriction.end();
	}
}


void InputCommandMapper::bindCommand(const std::string& key, const std::string& command)
{
	S_LOG_INFO("Binding key " << key << " to command " << command << " for state " << getState() << ".");
	mKeyCommands[key] = command;
}

void InputCommandMapper::unbindCommand(const std::string& key)
{
	S_LOG_INFO("Unbinding key " << key << " for state " << getState() << ".");
	mKeyCommands.erase(key);
}

const std::string& InputCommandMapper::getCommandForKey(SDLKey key)
{
	KeyMapStore::const_iterator I = mKeymap.find(key);
	if (I != mKeymap.end()) {
		KeyCommandStore::const_iterator J = mKeyCommands.find(I->second);
		if (J != mKeyCommands.end()) {
			return J->second;
		}
	}
	///if we don't find anything, return an empty string
	static std::string empty("");
	return empty;
}


// std::string Bindings::getBindingForKeysym(const SDL_keysym& key) {
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
  /// Assign keys to textual representation
  mKeymap[SDLK_BACKSPACE] = "backspace";
  mKeymap[SDLK_TAB] = "tab";
  mKeymap[SDLK_CLEAR] = "clear";
  mKeymap[SDLK_RETURN] = "return";
  mKeymap[SDLK_PAUSE] = "pause";
  mKeymap[SDLK_ESCAPE] = "escape";
  mKeymap[SDLK_SPACE] = "space";
  mKeymap[SDLK_EXCLAIM] = "exclaim";
  mKeymap[SDLK_QUOTEDBL] = "dbl_quote";
  mKeymap[SDLK_HASH] = "hash";
  mKeymap[SDLK_DOLLAR] = "dollar";
  mKeymap[SDLK_AMPERSAND] = "ampersand";
  mKeymap[SDLK_QUOTE] = "quote";
  mKeymap[SDLK_LEFTPAREN] = "left_paren";
  mKeymap[SDLK_RIGHTPAREN] = "right_paren";
  mKeymap[SDLK_ASTERISK] = "asterisk";
  mKeymap[SDLK_PLUS] = "plus";
  mKeymap[SDLK_COMMA] = "comma";
  mKeymap[SDLK_MINUS] = "-";
  mKeymap[SDLK_PERIOD] = "period";
  mKeymap[SDLK_SLASH] = "slash";
  mKeymap[SDLK_0] = "0";
  mKeymap[SDLK_1] = "1";
  mKeymap[SDLK_2] = "2";
  mKeymap[SDLK_3] = "3";
  mKeymap[SDLK_4] = "4";
  mKeymap[SDLK_5] = "5";
  mKeymap[SDLK_6] = "6";
  mKeymap[SDLK_7] = "7";
  mKeymap[SDLK_8] = "8";
  mKeymap[SDLK_9] = "9";
  mKeymap[SDLK_COLON] = "colon";
  mKeymap[SDLK_SEMICOLON] = "semi_colon";
  mKeymap[SDLK_LESS] = "less_than";
  mKeymap[SDLK_EQUALS] = "equals";
  mKeymap[SDLK_GREATER] = "greater_then";
  mKeymap[SDLK_QUESTION] = "question";
  mKeymap[SDLK_AT] = "at";
  mKeymap[SDLK_LEFTBRACKET] = "left_brace";
  mKeymap[SDLK_BACKSLASH] = "backslash";
  mKeymap[SDLK_RIGHTBRACKET] = "right_brace";
  mKeymap[SDLK_CARET] = "caret";
  mKeymap[SDLK_UNDERSCORE] = "_";
  mKeymap[SDLK_BACKQUOTE] = "backquote";
  mKeymap[SDLK_a] = "a";
  mKeymap[SDLK_b] = "b";
  mKeymap[SDLK_c] = "c";
  mKeymap[SDLK_d] = "d";
  mKeymap[SDLK_e] = "e";
  mKeymap[SDLK_f] = "f";
  mKeymap[SDLK_g] = "g";
  mKeymap[SDLK_h] = "h";
  mKeymap[SDLK_i] = "i";
  mKeymap[SDLK_j] = "j";
  mKeymap[SDLK_k] = "k";
  mKeymap[SDLK_l] = "l";
  mKeymap[SDLK_m] = "m";
  mKeymap[SDLK_n] = "n";
  mKeymap[SDLK_o] = "o";
  mKeymap[SDLK_p] = "p";
  mKeymap[SDLK_q] = "q";
  mKeymap[SDLK_r] = "r";
  mKeymap[SDLK_s] = "s";
  mKeymap[SDLK_t] = "t";
  mKeymap[SDLK_u] = "u";
  mKeymap[SDLK_v] = "v";
  mKeymap[SDLK_w] = "w";
  mKeymap[SDLK_x] = "x";
  mKeymap[SDLK_y] = "y";
  mKeymap[SDLK_z] = "z";
  mKeymap[SDLK_DELETE] = "delete";
  mKeymap[SDLK_KP0] = "kp_0";
  mKeymap[SDLK_KP1] = "kp_1";
  mKeymap[SDLK_KP2] = "kp_2";
  mKeymap[SDLK_KP3] = "kp_3";
  mKeymap[SDLK_KP4] = "kp_4";
  mKeymap[SDLK_KP5] = "kp_5";
  mKeymap[SDLK_KP6] = "kp_6";
  mKeymap[SDLK_KP7] = "kp_7";
  mKeymap[SDLK_KP8] = "kp_8";
  mKeymap[SDLK_KP9] = "kp_9";
  mKeymap[SDLK_KP_PERIOD] = "kp_period";
  mKeymap[SDLK_KP_DIVIDE] = "kp_divide";
  mKeymap[SDLK_KP_MULTIPLY] = "kp_multi";
  mKeymap[SDLK_KP_MINUS] = "kp_minus";
  mKeymap[SDLK_KP_PLUS] = "kp_plus";
  mKeymap[SDLK_KP_ENTER] = "kp_enter";
  mKeymap[SDLK_KP_EQUALS] = "kp_equals";
  mKeymap[SDLK_UP] = "up";
  mKeymap[SDLK_DOWN] = "down";
  mKeymap[SDLK_RIGHT] = "right";
  mKeymap[SDLK_LEFT] = "left";
  mKeymap[SDLK_INSERT] = "insert";
  mKeymap[SDLK_HOME] = "home";
  mKeymap[SDLK_END] = "end";
  mKeymap[SDLK_PAGEUP] = "page_up";
  mKeymap[SDLK_PAGEDOWN] = "page_down";
  mKeymap[SDLK_F1] = "f1";
  mKeymap[SDLK_F2] = "f2";
  mKeymap[SDLK_F3] = "f3";
  mKeymap[SDLK_F4] = "f4";
  mKeymap[SDLK_F5] = "f5";
  mKeymap[SDLK_F6] = "f6";
  mKeymap[SDLK_F7] = "f7";
  mKeymap[SDLK_F8] = "f8";
  mKeymap[SDLK_F9] = "f9";
  mKeymap[SDLK_F10] = "f10";
  mKeymap[SDLK_F11] = "f11";
  mKeymap[SDLK_F12] = "f12";
  mKeymap[SDLK_F13] = "f13";
  mKeymap[SDLK_F14] = "f14";
  mKeymap[SDLK_F15] = "f15";

  mKeymap[SDLK_NUMLOCK] = "num";
  mKeymap[SDLK_CAPSLOCK] = "caps";
  mKeymap[SDLK_SCROLLOCK] = "srcoll";
  mKeymap[SDLK_RSHIFT] = "right_shift";
  mKeymap[SDLK_LSHIFT] = "left_shift";
  mKeymap[SDLK_RCTRL] = "right_ctrl";
  mKeymap[SDLK_LCTRL] = "left_ctrl";
  mKeymap[SDLK_RALT] = "right_alt";
  mKeymap[SDLK_LALT] = "left_alt";
  mKeymap[SDLK_RMETA] = "right_meta";
  mKeymap[SDLK_LMETA] = "left_meta";
  mKeymap[SDLK_LSUPER] = "left_super";
  mKeymap[SDLK_RSUPER] = "right_super";
  mKeymap[SDLK_MODE]= "mode";
  mKeymap[SDLK_COMPOSE] = "compose";
  mKeymap[SDLK_PRINT] = "print";
  mKeymap[SDLK_SYSREQ] = "sysreq";
  mKeymap[SDLK_BREAK] = "break";
  mKeymap[SDLK_MENU] = "menu";
  mKeymap[SDLK_POWER] = "power";
  mKeymap[SDLK_EURO] = "euro";
}

}
