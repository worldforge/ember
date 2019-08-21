/*
 Copyright (C) 2016 erik

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EntityConsoleEditor.h"

#include "services/EmberServices.h"
#include "framework/Tokeniser.h"
#include <Eris/Avatar.h>
#include <boost/algorithm/string.hpp>
#include <components/ogre/Avatar.h>

namespace Ember {
namespace OgreView {
namespace Authoring {

EntityConsoleEditor::EntityConsoleEditor(Ember::OgreView::Avatar* avatar) :
		SetProperty("set_property", this, "Sets a property on the avatar."),
		mAvatar(avatar) {
}

void EntityConsoleEditor::runCommand(const std::string& command, const std::string& args) {
	if (SetProperty == command) {
		Atlas::Message::MapType map;
		Tokeniser tokeniser(args);
		if (tokeniser.hasRemainingTokens()) {
			std::string property = tokeniser.nextToken();
			if (tokeniser.hasRemainingTokens()) {
				std::string value = tokeniser.remainingTokens();
				if (!value.empty() && value[0] == '"') {
					std::stringstream ss;

					for (size_t i = 1; i < value.size(); ++i) {
						if (value[i] != '"') {
							ss << value[i];
						} else {
							break;
						}
					}
					map[property] = ss.str();
				} else {
					try {
						if (value.find_first_of('.') != std::string::npos) {
							map[property] = std::stof(value);
						} else {
							map[property] = std::stoi(value);
						}
					} catch (const std::exception&) {
						return;
					}
				}

				if (!map.empty()) {
					if (mAvatar) {
						mAvatar->setAttributes(&mAvatar->getEmberEntity(), map);
					}
				}
			}
		}
	}
}

}
}
}
