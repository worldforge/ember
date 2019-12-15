/*
 Copyright (C) 2013 Samuel Kogler

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

#include "OgreTerrainPageProvider.h"

namespace Ember {
namespace OgreView {
namespace Terrain {

OgreTerrainPageProvider::OgreTerrainPageProvider() = default;

OgreTerrainPageProvider::~OgreTerrainPageProvider() = default;

bool OgreTerrainPageProvider::prepareProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) {
	return true;
}

bool OgreTerrainPageProvider::loadProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) {
	return true;
}

bool OgreTerrainPageProvider::unloadProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) {
	return true;
}

bool OgreTerrainPageProvider::unprepareProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) {
	return true;
}

} /* namespace Terrain */
} /* namespace OgreView */
} /* namespace Ember */
