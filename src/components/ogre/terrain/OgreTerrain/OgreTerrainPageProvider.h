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
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef OGRETERRAINPAGEPROVIDER_H_
#define OGRETERRAINPAGEPROVIDER_H_

#include <Paging/OgrePageManager.h>

namespace Ember
{
namespace OgreView
{
namespace Terrain
{

/**
 * @brief Provides a way for the Ogre paging component to request the loading of new terrain pages.
 * In this particular case, it only tells the Ogre terrain component that we generate pages procedurally, so it does not try to load them from the disk.
 */
class OgreTerrainPageProvider: public Ogre::PageProvider
{
public:
	OgreTerrainPageProvider();
	virtual ~OgreTerrainPageProvider();

	bool prepareProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section);
	bool loadProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section);
	bool unloadProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section);
	bool unprepareProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section);

};

} /* namespace Terrain */
} /* namespace OgreView */
} /* namespace Ember */
#endif /* OGRETERRAINPAGEPROVIDER_H_ */
