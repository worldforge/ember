/*
 Copyright (C) 2018 Erik Ogenvik

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

#ifndef EMBER_EMBERENTITY_HELPER_H
#define EMBER_EMBERENTITY_HELPER_H

#include "EmberEntity.h"
std::vector<std::string> _EmberEntity_getUsages(const Ember::EmberEntity* entity)
{
    std::vector<std::string> usagesNames;
    for (auto& entry : entity->getUsages()) {
        usagesNames.emplace_back(entry.first);
    }
    return usagesNames;
}

const Eris::Usage* _EmberEntity_getUsage(const Ember::EmberEntity* entity, const std::string& usage)
{
    auto I = entity->getUsages().find(usage);
    if (I != entity->getUsages().end()) {
        return &I->second;
    }
    return nullptr;
}

std::vector<std::string> _EmberEntity_getUsagesProtected(const Ember::EmberEntity* entity)
{
    std::vector<std::string> usagesNames;
    for (auto& entry : entity->getUsagesProtected()) {
        usagesNames.emplace_back(entry.first);
    }
    return usagesNames;
}

const Eris::Usage* _EmberEntity_getUsageProtected(const Ember::EmberEntity* entity, const std::string& usage)
{
    auto I = entity->getUsagesProtected().find(usage);
    if (I != entity->getUsagesProtected().end()) {
        return &I->second;
    }
    return nullptr;
}
#endif //EMBER_EMBERENTITY_HELPER_H
