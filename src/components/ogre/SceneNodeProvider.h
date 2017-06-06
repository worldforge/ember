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

#ifndef SCENENODEPROVIDER_H_
#define SCENENODEPROVIDER_H_

#include "INodeProvider.h"

namespace Ogre {
class SceneNode;

class MovableObject;
}

namespace Ember {
namespace OgreView {

class SceneNodeProvider : public INodeProvider {
public:
	SceneNodeProvider(Ogre::SceneNode* node, Ogre::SceneNode* parentNode, bool transferNodeOwnership = true);

	virtual ~SceneNodeProvider();

	Ogre::Node& getNode() const override;

	Ogre::Node* getParentNode() const override;

	INodeProvider* createChildProvider(const std::string& name) override;

	void setVisible(bool visible) override;

	/**
	 * General method for turning on and off debug visualizations. Subclasses might support more types of visualizations than the ones defined here.
	 * @param visualization The type of visualization. Currently supports "OgreBBox".
	 * @param visualize Whether to visualize or not.
	 */
	void setVisualize(const std::string& visualization, bool visualize) override;

	/**
	 * @brief Gets whether a certain visualization is turned on or off.
	 * @param visualization The type of visualization. Currently supports "OgreBBox".
	 * @return true if visualization is turned on, else false
	 */
	bool getVisualize(const std::string& visualization) const override;

	/**
	 * @brief Sets the position and orientation of the node.
	 * @param position The position.
	 * @param orientation The orientation.
	 */
	void setPositionAndOrientation(const Ogre::Vector3& position, const Ogre::Quaternion& orientation) override;

	void detachObject(Ogre::MovableObject* movable) override;

	void attachObject(Ogre::MovableObject* movable) override;

protected:

	Ogre::SceneNode* mNode;
	Ogre::SceneNode* mParentNode;
	bool mOwnsNode;
};

}

}

#endif /* SCENENODEPROVIDER_H_ */
