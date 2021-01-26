//
// C++ Implementation: ConfigListener
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2008
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

#include "ConfigListener.h"
#include "ConfigService.h"
#include <utility>

namespace Ember {

ConfigListener::ConfigListener(std::string section, std::string key, SettingChangedSlot slot)
		: mSection(std::move(section)),
		  mKey(std::move(key)),
		  mSlot(std::move(slot)),
		  mInternalSlot(sigc::mem_fun(*this, &ConfigListener::ConfigService_EventChangedConfigItem)) {
	mConnection = ConfigService::getSingleton().EventChangedConfigItem.connect(mInternalSlot);
}

ConfigListener::~ConfigListener() {
	mSlot.disconnect();
	mInternalSlot.disconnect();
	mConnection.disconnect();
}

void ConfigListener::ConfigService_EventChangedConfigItem(const std::string& section, const std::string& key) {
	if (section == mSection && key == mKey) {
		varconf::Variable variable = ConfigService::getSingleton().getValue(section, key);
		mSlot(section, key, variable);
	}
}

bool ConfigListener::evaluate() {
	if (ConfigService::getSingleton().itemExists(mSection, mKey)) {
		varconf::Variable variable = ConfigService::getSingleton().getValue(mSection, mKey);
		mSlot(mSection, mKey, variable);
		return true;
	}
	return false;
}

}

