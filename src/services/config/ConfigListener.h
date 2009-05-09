//
// C++ Interface: ConfigListener
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
#ifndef EMBERCONFIGLISTENER_H
#define EMBERCONFIGLISTENER_H

#include <sigc++/signal.h>
#include <sigc++/trackable.h>
#include <string>

#include <varconf/variable.h>

#include "ConfigListenerContainer.h"

namespace Ember
{

/**
 @author Erik Hjortsberg <erik.hjortsberg@gmail.com>

 Use this to listen for changes to a specific key in a specific configuration section. When a change occur, the submitted slot will be executed.
 In your consumer class, bind the slot to a method which will handle the changed value.

 You cannot create instances of this class directly, instead your consumer class must inherit from ConfigListenerContainer and call the registerConfigListener in order to create and register instances.
 */
class ConfigListener: public sigc::trackable
{
public:
	friend class ConfigListenerContainer;

	~ConfigListener();

	/**
	 * @brief Evaluates the value and calls the listeners if it exitst.
	 * @return True if the setting existed.
	 */
	bool evaluate();
protected:
	ConfigListener(const std::string& section, const std::string& key, ConfigListenerContainer::SettingChangedSlot slot);

	std::string mSection;
	std::string mKey;
	ConfigListenerContainer::SettingChangedSlot mSlot;
	sigc::slot<void, const std::string&, const std::string&> mInternalSlot;

	void ConfigService_EventChangedConfigItem(const std::string& section, const std::string& key);

};

}

#endif
