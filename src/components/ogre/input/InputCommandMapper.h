//
// C++ Interface: InputCommandMapper
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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
#ifndef EMBEROGREINPUTCOMMANDMAPPER_H
#define EMBEROGREINPUTCOMMANDMAPPER_H

#include "../EmberOgrePrerequisites.h"
#include <SDL.h>
#include "Input.h"

namespace EmberOgre {


/**

	Listens for input (key presses, mouse movements etc) and maps this input to commands that are then executed (such as +move_forward etc.)
	
	It's expected that more than one instance of this should exists, one for each different game "state", such as "movement mode", "general" etc.

	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class InputCommandMapper : public sigc::trackable
{
public:
	typedef std::map<std::string, std::string> KeyCommandStore;
	typedef std::map<SDLKey, std::string> KeyMapStore;
	typedef std::vector<Input::InputMode> InputModeStore;
   
	InputCommandMapper(const std::string& state);
	InputCommandMapper(const std::string& state, const std::string& configSection);

    ~InputCommandMapper();
    
    void bindCommand(const std::string& key, const std::string& command);
    void unbindCommand(const std::string& key);
    
    inline const std::string& getState() const;
    
	void readFromConfigSection(const std::string& sectionName);
    
    inline void setEnabled(bool enabled);
    inline bool getEnabled() const;
    
    void bindToInput(Input& input);

	inline void restrictToInputModes(InputModeStore modes);
	inline void restrictToInputMode(Input::InputMode mode);
	    
protected:

	void Input_EventKeyPressed(const SDL_keysym& key, Input::InputMode inputMode);
	void Input_EventKeyReleased(const SDL_keysym& key, Input::InputMode inputMode);


	KeyCommandStore mKeyCommands;
	
	std::string mState;
	
	const std::string& getCommandForKey(SDLKey key);
	
	KeyMapStore mKeymap;
	
	void initKeyMap();
	
	bool mEnabled;
	
	InputModeStore mInputModesRestriction;
	
	bool isActiveForInputMode(Input::InputMode mode) const;
};

const std::string& InputCommandMapper::getState() const {return mState;}

void InputCommandMapper::setEnabled(bool enabled) {mEnabled = enabled;}
bool InputCommandMapper::getEnabled() const {return mEnabled; }

void InputCommandMapper::restrictToInputModes(InputModeStore modes) {mInputModesRestriction = modes; }
void InputCommandMapper::restrictToInputMode(Input::InputMode mode) {
InputModeStore modes;
modes.push_back(mode);
restrictToInputModes(modes); 
}


}

#endif
