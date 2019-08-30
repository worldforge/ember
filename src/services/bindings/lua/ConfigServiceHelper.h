/*
 Copyright (C) 2019 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef EMBER_CONFIGSERVICEHELPER_H
#define EMBER_CONFIGSERVICEHELPER_H

#include "services/config/ConfigService.h"

const std::string& _ConfigService_getHomeDirectory(const Ember::ConfigService* service, Ember::BaseDirType baseDirType) {
	return service->getHomeDirectory(baseDirType).string();
}

const std::string& _ConfigService_getSharedDataDirectory(const Ember::ConfigService* service) {
	return service->getSharedDataDirectory().string();
}

const std::string& _ConfigService_getEmberDataDirectory(const Ember::ConfigService* service) {
	return service->getEmberDataDirectory().string();
}

const std::string& _ConfigService_getEmberMediaDirectory(const Ember::ConfigService* service) {
	return service->getEmberMediaDirectory().string();
}

const std::string& _ConfigService_getUserMediaDirectory(const Ember::ConfigService* service) {
	return service->getUserMediaDirectory().string();
}

const std::string& _ConfigService_getSharedMediaDirectory(const Ember::ConfigService* service) {
	return service->getSharedMediaDirectory().string();
}

const std::string& _ConfigService_getSharedConfigDirectory(const Ember::ConfigService* service) {
	return service->getSharedConfigDirectory().string();
}

#endif //EMBER_CONFIGSERVICEHELPER_H
