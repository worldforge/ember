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

#ifndef EMBER_GEOMETRYVISUALIZATION_H
#define EMBER_GEOMETRYVISUALIZATION_H

#include <sigc++/trackable.h>
#include <sigc++/connection.h>
#include <functional>


namespace Ogre {
class SceneNode;
class ManualObject;
class Entity;
class Vector3;
}

namespace Atlas {
namespace Message {
class Element;
}
}


namespace Ember {
class EmberEntity;
namespace OgreView {
namespace Authoring {

/**
 * Shows the collision geometry of the entity, as it's represented on the server.
 */
class GeometryVisualization : public virtual sigc::trackable {
public:

	/**
	 * @brief Ctor.
	 * @param entity The entity this visualization is attached to.
	 * @param sceneNode A precreated scene node to which we'll attach our visualization. Ownership is transferred to this instance.
	 */
	GeometryVisualization(EmberEntity& entity, Ogre::SceneNode* sceneNode);

	/**
	 * @brief Ctor.
	 */
	virtual ~GeometryVisualization();

	/**
	 * @brief Updates the position of the visualization to fit with the entity.
	 */
	void updatePositionAndOrientation();

	/**
	 * @brief Updates the size of the bounding box representing the entity.
	 */
	void updateBbox();

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
	 * The ManualObject is used for all geometries that aren't extracted from assets.
	 */
	Ogre::ManualObject* mManualObject;

	/**
	 * The Entity is used by "asset" geometries, i.e. those that are read from Ogre Mesh data.
	 */
	Ogre::Entity* mOgreEntity;

	/**
	 * @brief The connection for the "bbox" attribute observer.
	 */
	sigc::connection mBboxConnection;

	std::function<void()> mBboxUpdateFn;

	void buildGeometry();

	/**
	 * @brief When the entity moves, update the position and orientation of the scene node.
	 */
	void entity_Moved();


	/**
	 * @brief Called when the bbox attribute changes.
	 * @param attributeValue The new value of the bbox attribute.
	 */
	void entity_BboxChanged(const Atlas::Message::Element& attributeValue);

	void placeSphere(float radius, Ogre::Vector3 offset);
	void placeCylinderX(float radius, float halfHeight, Ogre::Vector3 offset);
	void placeCylinderY(float radius, float halfHeight, Ogre::Vector3 offset);
	void placeCylinderZ(float radius, float halfHeight, Ogre::Vector3 offset);

};
}
}
}


#endif //EMBER_GEOMETRYVISUALIZATION_H
