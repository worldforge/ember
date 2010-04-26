/*
 Copyright (C) 2010 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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


#include "ExclusiveImposterPage.h"

namespace EmberOgre
{

namespace Environment
{

ExclusiveImposterPage::~ExclusiveImposterPage()
{
}

void ExclusiveImposterPage::addEntity(Ogre::Entity *ent, const Ogre::Vector3 &position, const Ogre::Quaternion &rotation, const Ogre::Vector3 &scale, const Ogre::ColourValue &color)
{
	ImpostorPage::addEntity(ent, position, rotation, scale, color);
	mEntities.push_back(ent);
}

void ExclusiveImposterPage::setVisible(bool visible)
{
	ImpostorPage::setVisible(visible);
	if (visible) {
		for (EntityStore::const_iterator I(mEntities.begin()); I != mEntities.end(); ++I) {
			(*I)->setVisible(false);
		}
	} else {
		for (EntityStore::const_iterator I(mEntities.begin()); I != mEntities.end(); ++I) {
			(*I)->setVisible(true);
		}
	}
}

void ExclusiveImposterPage::removeEntities()
{
	ImpostorPage::removeEntities();
	mEntities.clear();
}

}

}
