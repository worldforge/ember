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

#ifndef EXCLUSIVEIMPOSTERPAGE_H_
#define EXCLUSIVEIMPOSTERPAGE_H_

#include "pagedgeometry/include/ImpostorPage.h"

namespace EmberOgre
{

namespace Environment
{

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 *
 * @brief An exclusive imposter page, which means that it will automatically hide any entities when the imposter variant of them is used.
 *
 * This works very much like the standard imposter page, except that all entities that are added to it are kept track of, and when the imposter variant is shown the real entity is automatically hidden.
 * When the imposter isn't shown anymore (often by the user moving the camera closer to the entity) the entity is restored again.
 * This page mainly of use together with the PassiveEntityPage, where you want the entities to be shown normally when up close, and with imposters far away.
 *
 * @note Any entity added to this page can't be transient as a reference to it is stored.
 */
class ExclusiveImposterPage : public Forests::ImpostorPage
{
public:

	/**
	 * @brief A store of entity pointers.
	 */
	typedef std::vector<Ogre::Entity*> EntityStore;

	virtual ~ExclusiveImposterPage();

	void addEntity(Ogre::Entity *ent, const Ogre::Vector3 &position, const Ogre::Quaternion &rotation, const Ogre::Vector3 &scale, const Ogre::ColourValue &color);

	void setVisible(bool visible);
	void removeEntities();

protected:

	/**
	 * @brief The entities used on this page.
	 */
	EntityStore mEntities;
};

}

}

#endif /* EXCLUSIVEIMPOSTERPAGE_H_ */
