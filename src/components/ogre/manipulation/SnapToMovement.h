//
// C++ Interface: SnapToMovement
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifndef EMBEROGRE_MANIPULATIONSNAPTOMOVEMENT_H
#define EMBEROGRE_MANIPULATIONSNAPTOMOVEMENT_H

#include <wfmath/point.h>
#include <wfmath/vector.h>
#include <vector>

namespace Ogre
{
class SceneNode;
class Node;
class SceneManager;
}
namespace Eris
{
class Entity;
}

namespace EmberOgre
{

class EmberEntity;

namespace Manipulation
{

/**
 * @brief Provides snap-to functionality for moved entities, i.e. it allows them to snap to other entities.
 *
 * This can be very useful when you want to align two entities with another.
 * The snapping is done through the corners of the bounding boxes.
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 */
class SnapToMovement
{
public:
	/**
	 * @brief Ctor.
	 * @param entity The entity which is moved.
	 * @param node The node to which the entity is attached.
	 * @param snapThreshold The max distance, in world units, when looking for suitable snap targets.
	 * @param sceneManager The scene manager in which the movement occurs.
	 * @param showDebugOverlay Whether to show a debug overlay consisting of dots for the snapping.
	 */
	SnapToMovement(Eris::Entity& entity, Ogre::Node& node, float snapThreshold, Ogre::SceneManager& sceneManager, bool showDebugOverlay = false);

	/**
	 * @brief Dtor.
	 */
	virtual ~SnapToMovement();

	/**
	 * @brief Tests whether the entity can be snapped to another entity.
	 * @param position The position of the entity.
	 * @param orientation The orientation of the entity.
	 * @param adjustment If snapping can happen, this is the adjustment needed for the entity to be in the new position.
	 * @param snappedToEntity If snapping can happen, this is the entity to which it can be snapped.
	 * @returns True if it could be snapped.
	 */
	bool testSnapTo(const WFMath::Point<3>& position, const WFMath::Quaternion& orientation, WFMath::Vector<3>& adjustment, EmberEntity* snappedToEntity);

protected:
	/**
	 * @brief A simple struct for snapping candidates.
	 */
	struct SnapPointCandidate
	{
		WFMath::CoordType distance;
		WFMath::Vector<3> adjustment;
		EmberEntity* entity;
	};

	/**
	 * @brief The entity which is being moved.
	 */
	Eris::Entity& mEntity;

	/**
	 * @brief The node to which the entity is attached.
	 */
	Ogre::Node& mNode;

	/**
	 * @brief The max distance for when looking for entities to snap to.
	 */
	float mSnapThreshold;

	Ogre::SceneManager& mSceneManager;

	/**
	 * @brief If debug overlay is shown, the scene nodes which contains the nodes will be stored here.
	 */
	std::vector<Ogre::SceneNode*> mDebugNodes;

};

}

}

#endif
