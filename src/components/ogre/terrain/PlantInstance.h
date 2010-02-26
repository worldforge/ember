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


#ifndef EMBEROGRE_TERRAIN_PLANTINSTANCE_H_
#define EMBEROGRE_TERRAIN_PLANTINSTANCE_H_

#include <OgreVector3.h>
#include <OgreVector2.h>

namespace EmberOgre
{

namespace Terrain
{


/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief A simple struct for representing one plant instance in a 2d space.
 * To save space some simplifications are made. The orientation is only stored as a rotation around the vertical axis, and the scale is only stored for a flat plant.
 */
class Plant2DInstance
{
public:
	/**
	 * @brief Ctor.
	 * @param position A position in 2d space.
	 * @param orientation The rotation around the vertical axis for the plant.
	 * @param scale The scale of the plant in width and height.
	 */
	Plant2DInstance(const Ogre::Vector2& position, float orientation, const Ogre::Vector2& scale)
	{
		this->position = position;
		this->orientation = orientation;
		this->scale = scale;
	}

	/**
	 * @brief The position of the plant.
	 */
	Ogre::Vector2 position;

	/**
	 * @brief The rotation of the plant around the vertical axis.
	 */
	float orientation;

	/**
	 * @brief The scale of the plant, for width and height.
	 */
	Ogre::Vector2 scale;
};


/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief A simple struct for representing one plant instance in a 3d space.
 * To save space some simplifications are made. The orientation is only stored as a rotation around the vertical axis, and the scale is only stored for a flat plant.
 */
class PlantInstance
{
public:
	/**
	 * @brief Ctor.
	 * @param position A position in 3d space.
	 * @param orientation The rotation around the vertical axis for the plant.
	 * @param scale The scale of the plant in width and height.
	 */
	PlantInstance(const Ogre::Vector3& position, float orientation, const Ogre::Vector2& scale)
	{
		this->position = position;
		this->orientation = orientation;
		this->scale = scale;
	}

	/**
	 * @brief The position of the plant.
	 */
	Ogre::Vector3 position;

	/**
	 * @brief The rotation of the plant around the vertical axis.
	 */
	float orientation;

	/**
	 * @brief The scale of the plant, for width and height.
	 */
	Ogre::Vector2 scale;
};

}

}
#endif /* EMBEROGRE_TERRAIN_PLANTINSTANCE_H_ */
