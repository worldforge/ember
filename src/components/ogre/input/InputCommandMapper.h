//
// C++ Interface: InputCommandMapper
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
#ifndef EMBEROGREINPUTCOMMANDMAPPER_H
#define EMBEROGREINPUTCOMMANDMAPPER_H

#include <SDL.h>
#include "Input.h"
#include <vector>

namespace EmberOgre {


/**

	@brief Listens for input (key presses, mouse movements etc) and maps this input to commands that are then executed (such as +move_forward etc.)
	
	It's expected that more than one instance of this should exists, one for each different game "state", such as "movement mode", "general" etc.

	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class InputCommandMapper : public sigc::trackable
{
public:
	typedef std::map<std::string, std::string> KeyCommandStore;
	typedef std::map<SDLKey, std::string> KeyMapStore;
	typedef std::vector<Input::InputMode> InputModeStore;
   
	/**
	 *    Constructor. 
	 * @param state The state which this instance should apply to, such as "general" or "gui"
	 * @return 
	 */
	InputCommandMapper(const std::string& state);
	
	/**
	 *    Constructor which also reads values from the config and sets up bindings.
	 * @param state The state which this instance should apply to, such as "general" or "gui"
	 * @param configSection The name of the config section to read values from.
	 * @return 
	 */
	InputCommandMapper(const std::string& state, const std::string& configSection);

    /**
     * At destruction the instance is also deregistered from the Input instance.
     * @return 
     */
    virtual ~InputCommandMapper();
    
    /**
     * Binds a command to a key.
     * @param key The name of the key.
     * @param command The full command to execute, such as "/show_inventory" or "+run"
     */
    void bindCommand(const std::string& key, const std::string& command);
    
    /**
     * Removes a binding for a certain key.
     * @param key The name of the key to remove bindings for.
     */
    void unbindCommand(const std::string& key);
    
    /**
     * The name of the state that this instance applies to.
     * @return 
     */
    inline const std::string& getState() const;
    
	/**
	 *    Reads a section from the config and sets up bindings accordingly.
	 * @param sectionName The name of the section, such as "key_bindings_general".
	 */
	void readFromConfigSection(const std::string& sectionName);
    
    /**
     * Sets whether this instance is enabled or not.
     * @param enabled 
     */
    inline void setEnabled(bool enabled);
    
    /**
     * Returns whether this instance is enabled or not.
     * @return 
     */
    inline bool getEnabled() const;
    
    /**
     * Binds the instance to a Input object. Call this to start recieving input.
     * @param input 
     */
    void bindToInput(Input& input);

	/**
	 *    Sets which modes this instance should be restricted to. An empty list will make it listen for all modes.
	 * @param modes 
	 */
	inline void restrictToInputModes(InputModeStore modes);
	
	/**
	 *    Sets which mode this instance should be restricted to.
	 * @param mode 
	 */
	inline void restrictToInputMode(Input::InputMode mode);
	    
protected:

	/**
	 *    At keypress time, see what command to run.
	 * @param key 
	 * @param inputMode 
	 */
	void Input_EventKeyPressed(const SDL_keysym& key, Input::InputMode inputMode);
	/**
	 *    At keyrelease time, see if there's a command prefixed with "+", such as "+run", which should have its "-" twin command sent out.
	 * @param key 
	 * @param inputMode 
	 */
	void Input_EventKeyReleased(const SDL_keysym& key, Input::InputMode inputMode);

	/**
	A store of the mapping between keys and commands.
	*/
	KeyCommandStore mKeyCommands;
	
	/**
	The state which this instance applies to.
	*/
	std::string mState;
	
	/**
	 *    Gets the name of the command registered to the key, or an empty string if no could be found.
	 * @param key 
	 * @return 
	 */
	const std::string& getCommandForKey(SDLKey key);
	
	/**
	Mappings between SDLKeys and their names.
	*/
	KeyMapStore mKeymap;
	
	/**
	 *    Creates the mapping between SDLKeys and their names.
	 */
	void initKeyMap();
	
	/**
	Whether this is enabled or not.
	*/
	bool mEnabled;
	
	/**
	Stores the InputModes that this instance is restricted to listen to, if any.
	*/
	InputModeStore mInputModesRestriction;
	
	/**
	 *    Returns true if this instance is active for the current input mode.
	 * @param mode 
	 * @return 
	 */
	bool isActiveForInputMode(Input::InputMode mode) const;
	
	/**
	We'll keep a reference to the Input object which we will use at dispose time.
	*/
	Input* mInput;
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
