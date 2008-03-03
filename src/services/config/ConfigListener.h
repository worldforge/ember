//
// C++ Interface: ConfigListener
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2008
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
#ifndef EMBERCONFIGLISTENER_H
#define EMBERCONFIGLISTENER_H

#include <sigc++/signal.h>
#include <string>

#include <varconf/variable.h>

namespace Ember {

class ConfigListenerContainer;
/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
	
	Use this to listen for changes to a specific key in a specific configuration section. When a change occur, the submitted slot will be executed.
	In your consumer class, bind the slot to a method which will handle the changed value.
	
	You cannot create instances of this class directly, instead your consumer class must inherit from ConfigListenerContainer and call the registerConfigListener in order to create and register instances.
*/
class ConfigListener{
public:
	friend class ConfigListenerContainer;
	typedef sigc::slot<void, const std::string&, const std::string&, varconf::Variable&> SettingChangedSlot;

    ~ConfigListener();
protected:
    ConfigListener(const std::string& section, const std::string& key, SettingChangedSlot slot);

	std::string mSection;
	std::string mKey;
	SettingChangedSlot mSlot;
	sigc::slot<void, const std::string&, const std::string&> mInternalSlot;
	
	void ConfigService_EventChangedConfigItem(const std::string& section, const std::string& key);

};

/**
All classes that whishes to use the ConfigListener class to listen for configuration changes must inherit from this class.
Call registerConfigListener to register new listeners.

All listeners will be automatically destroyed when this class is destroyed.
*/
class ConfigListenerContainer
{
public:

	virtual ~ConfigListenerContainer();
	
protected:

	/**
	 *    Registers a new listener. The listener instance will be owned by this class and automatically deleted when the destructor is called.
	 * @param section The config section to listen to.
	 * @param key The config key to listen to.
	 * @param slot The slot to execute when a change has occurred.
	 * @return A pointer to the newly created listener instance.
	 */
	ConfigListener* registerConfigListener(const std::string& section, const std::string& key, ConfigListener::SettingChangedSlot slot);

private:
	/**
	A collection of listeners.
	*/
	std::vector<ConfigListener*> mConfigListeners;


};

}

#endif
