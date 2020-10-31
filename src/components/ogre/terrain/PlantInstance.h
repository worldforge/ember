/*
 Copyright (C) 2010 Erik Ogenvik <erik@ogenvik.org>

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


#ifndef EMBEROGRE_TERRAIN_PLANTINSTANCE_H_
#define EMBEROGRE_TERRAIN_PLANTINSTANCE_H_

#include <OgreVector.h>
#include <OgreVector.h>

namespace Ember {
namespace OgreView {

namespace Terrain {


/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief A simple struct for representing one plant instance in a 2d space.
 * To save space some simplifications are made. The orientation is only stored as a rotation around the vertical axis, and the scale is only stored for a flat plant.
 */
class Plant2DInstance {
public:
	/**
	 * @brief Ctor.
	 * @param position A position in 2d space.
	 * @param orientation The rotation around the vertical axis for the plant.
	 * @param scale The scale of the plant in width and height.
	 */
	Plant2DInstance(const Ogre::Vector2& positionIn, float orientationIn, const Ogre::Vector2& scaleIn) {
		this->position = positionIn;
		this->orientation = orientationIn;
		this->scale = scaleIn;
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
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief A simple struct for representing one plant instance in a 3d space.
 * To save space some simplifications are made. The orientation is only stored as a rotation around the vertical axis, and the scale is only stored for a flat plant.
 */
struct PlantInstance {

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
}
#endif /* EMBEROGRE_TERRAIN_PLANTINSTANCE_H_ */
