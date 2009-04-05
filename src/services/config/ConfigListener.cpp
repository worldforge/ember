//
// C++ Implementation: ConfigListener
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ConfigListener.h"
#include "../EmberServices.h"
#include "ConfigService.h"

namespace Ember {

ConfigListener::ConfigListener(const std::string& section, const std::string& key, SettingChangedSlot slot)
: mSection(section)
, mKey(key)
, mSlot(slot)
{
	mInternalSlot = sigc::mem_fun(*this, &ConfigListener::ConfigService_EventChangedConfigItem);
	Ember::EmberServices::getSingletonPtr()->getConfigService()->EventChangedConfigItem.connect(mInternalSlot);
}


ConfigListener::~ConfigListener()
{
	mSlot.disconnect();
	mInternalSlot.disconnect();
}

void ConfigListener::ConfigService_EventChangedConfigItem(const std::string& section, const std::string& key)
{
	if (section == mSection && key == mKey) {
		varconf::Variable variable = Ember::EmberServices::getSingleton().getConfigService()->getValue(section, key);
		mSlot(section, key, variable);
	}
}

void ConfigListener::evaluate()
{
	if (Ember::EmberServices::getSingleton().getConfigService()->itemExists(mSection, mKey)) {
		varconf::Variable variable = Ember::EmberServices::getSingleton().getConfigService()->getValue(mSection, mKey);
		mSlot(mSection, mKey, variable);
	}
}


ConfigListenerContainer::~ConfigListenerContainer() {
	for (std::vector<Ember::ConfigListener*>::iterator I = mConfigListeners.begin(); I != mConfigListeners.end(); ++I) {
		delete *I;
	}
}

ConfigListener* ConfigListenerContainer::registerConfigListener(const std::string& section, const std::string& key, ConfigListener::SettingChangedSlot slot, bool evaluateNow)
{
	ConfigListener* listener = new ConfigListener(section, key, slot);
	mConfigListeners.push_back(listener);
	if (evaluateNow) {
		listener->evaluate();
	}
	return listener;
}



}
