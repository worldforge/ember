/*
 Copyright (C) 2009 Erik Hjortsberg

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

#ifndef AUTHORINGVISUALIZATION_H_
#define AUTHORINGVISUALIZATION_H_
#include "components/ogre/OgreIncludes.h"
#include <sigc++/trackable.h>

namespace EmberOgre
{

class EmberEntity;

namespace Authoring
{

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief An authoring visualization for an entity.
 * The main use of this is for entities which doesn't have a graphical visualization (like an area) or entities for which the graphical representation is hard to graphically select.
 * The entity is represented by three axes, which are clickable.
 */
class AuthoringVisualization: public virtual sigc::trackable
{
public:
	/**
	 * @brief Ctor.
	 * @param entity The entity this visualization is attached to.
	 * @param sceneNode A precreated scene node to which we'll attach our visualization. Ownership is transferred to this instance.
	 */
	AuthoringVisualization(EmberEntity& entity, Ogre::SceneNode* sceneNode);

	/**
	 * @brief Dtor.
	 * At destruction the entity and scene node will be destroyed.
	 */
	virtual ~AuthoringVisualization();

	/**
	 * @brief Gets the Ogre scene node to which this visualization is attached.
	 * @returns An Ogre scene node.
	 */
	Ogre::SceneNode* getSceneNode() const;

	/**
	 * @brief Updates the position of the visualization to fit with the entity.
	 */
	void updatePositionAndOrientation();

protected:

	/**
	 * @brief The entity which this visualization represents.
	 */
	EmberEntity& mEntity;

	/**
	 * @brief The scene node used by this visualization.
	 */
	Ogre::SceneNode* mSceneNode;

	/**
	 * @brief The entity which represents the entity in the world.
	 */
	Ogre::Entity* mGraphicalRepresentation;

	/**
	 * @brief When the entity moves, update the position and orientation of the scene node.
	 */
	void entity_Moved();

	/**
	 * @brief Creates the entity.
	 */
	void createGraphicalRepresentation();

	/**
	 * @brief Destroys the entity and the scene node to which it's attached.
	 */
	void removeGraphicalRepresentation();
};

}
}
#endif /* AUTHORINGVISUALIZATION_H_ */
