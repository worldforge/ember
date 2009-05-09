//
// C++ Interface: ConfigListenerContainer
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2008
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

#ifndef CONFIGLISTENERCONTAINER_H_
#define CONFIGLISTENERCONTAINER_H_

#include <sigc++/slot.h>
#include <vector>
#include <string>

#include <varconf/variable.h>

namespace Ember
{

class ConfigListener;

/**
 @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 All classes that whishes to use the ConfigListener class to listen for configuration changes must inherit from this class.
 Call registerConfigListener to register new listeners.

 All listeners will be automatically destroyed when this class is destroyed.
 */
class ConfigListenerContainer
{
public:
	typedef sigc::slot<void, const std::string&, const std::string&, varconf::Variable&> SettingChangedSlot;

	virtual ~ConfigListenerContainer();

protected:

	/**
	 * @brief Registers a new listener. The listener instance will be owned by this class and automatically deleted when the destructor is called.
	 * @param section The config section to listen to.
	 * @param key The config key to listen to.
	 * @param slot The slot to execute when a change has occurred.
	 * @param evaluateNow If true, the listener will be evaluated instantly, possibly triggering a call to the signal. Defaults to true.
	 * @return A pointer to the newly created listener instance.
	 */
	ConfigListener* registerConfigListener(const std::string& section, const std::string& key, SettingChangedSlot slot, bool evaluateNow = true);

	/**
	 * @brief Registers a new listener. The listener instance will be owned by this class and automatically deleted when the destructor is called. The setting will always be evaluated, and if no setting can be found the default value will be used to trigger a call to the listener method.
	 * @param section The config section to listen to.
	 * @param key The config key to listen to.
	 * @param slot The slot to execute when a change has occurred.
	 * @param defaultValue The default value, to use if no existing setting can be found.
	 * @return A pointer to the newly created listener instance.
	 */
	ConfigListener* registerConfigListenerWithDefaults(const std::string& section, const std::string& key, SettingChangedSlot slot, varconf::Variable defaultValue);

private:
	/**
	 A collection of listeners.
	 */
	std::vector<ConfigListener*> mConfigListeners;

};

}

#endif /* CONFIGLISTENERCONTAINER_H_ */
