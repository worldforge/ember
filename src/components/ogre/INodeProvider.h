/*
 Copyright (C) 2009 Erik Ogenvik <erik@ogenvik.org>

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

#ifndef INODEPROVIDER_H_
#define INODEPROVIDER_H_

#include "domain/IVisualizable.h"

namespace Ogre
{
class Node;
class MovableObject;
class Vector3;
class Quaternion;
}

namespace Ember
{
namespace OgreView
{
namespace Model {
    class Model;
}

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief A provider of Ogre Node instances.
 *
 * There are two kinds of nodes used in the system: Ogre::SceneNode and Ogre::TagPoint. However, most of the classes which deals with nodes use the base class Ogre::Node instead of the final classes.
 * Subclasses of this are therefore used for providing concrete instances.
 * A typical use is as a binding between a parent and child node.
 */
class INodeProvider: public IVisualizable
{
public:
	/**
	 * @brief Dtor.
	 */
	virtual ~INodeProvider()
	{
	}

	/**
	 * @brief Gets the main node instance, as created and managed by this instance.
	 * @return A node instance.
	 */
	virtual Ogre::Node* getNode() const = 0;

	/**
	 * @brief Gets the parent node, if there is any.
	 * @return The parent node, or null if no such exists.
	 */
	virtual Ogre::Node* getParentNode() const = 0;

	/**
	 * @brief Creates a new INodeProvider which is a child of this instance.
	 * @param attachedObject An optional movable object which will be attached to the node created by new provider.
	 * @return A new provider, which is a child of this instance.
	 */
	virtual INodeProvider* createChildProvider(const std::string& name) = 0;

	/**
	 * @brief Sets the visiblity of the node.
	 * @param visible Whether the node should be visible or not.
	 */
	virtual void setVisible(bool visible) = 0;

	/**
	 * @brief Sets the position and orientation of the node.
	 * @param position The position.
	 * @param orientation The orientation.
	 */
	virtual void setPositionAndOrientation(const Ogre::Vector3& position, const Ogre::Quaternion& orientation) = 0;

	virtual void setOffsets(const Ogre::Vector3& translate, const Ogre::Quaternion& rotate) = 0;

	virtual void setScale(const Ogre::Vector3& scale) = 0;

	virtual void detachObject(Ogre::MovableObject* movable) = 0;

	virtual void attachObject(Ogre::MovableObject* movable) = 0;
};
}

}

#endif /* INODEPROVIDER_H_ */
