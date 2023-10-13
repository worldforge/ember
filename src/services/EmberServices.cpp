/*
    Copyright (C) 2002  Hans Häggström
    Copyright (C) 2005	Erik Ogenvik

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software Foundation,
    Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "EmberServices.h"

#include "config/ConfigService.h"
#include "sound/SoundService.h"
#include "metaserver/MetaserverService.h"
#include "server/ServerService.h"
#include "scripting/ScriptingService.h"
#include "serversettings/ServerSettings.h"

namespace Ember {


EmberServices::~EmberServices() = default;

ConfigService& EmberServices::getConfigService() {
	return mConfigService;
}

MetaserverService& EmberServices::getMetaserverService() {
	return *mMetaserverService;
}

ServerService& EmberServices::getServerService() {
	return *mServerService;
}

SoundService& EmberServices::getSoundService() {
	return *mSoundService;
}

ScriptingService& EmberServices::getScriptingService() {
	return *mScriptingService;
}

ServerSettings& EmberServices::getServerSettingsService() {
	return *mServerSettingsService;
}

EmberServices::EmberServices(Session& session, ConfigService& configService)
		: mConfigService(configService),
		  mScriptingService(std::make_unique<ScriptingService>()),
		  mSoundService(std::make_unique<SoundService>(configService)),
		  mServerService(std::make_unique<ServerService>(session)),
		  mMetaserverService(std::make_unique<MetaserverService>(session, configService)),
		  mServerSettingsService(std::make_unique<ServerSettings>()) {
}

}
