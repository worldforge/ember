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

#ifndef EMBEROGRENodeAttachment_H_
#define EMBEROGRENodeAttachment_H_

#include "components/ogre/AttachmentBase.h"
#include <sigc++/trackable.h>

namespace Ogre
{
class Node;
}

namespace Ember
{
namespace OgreView
{

class INodeProvider;
class NodeController;

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 *
 * @brief An attachment which uses an instance of Ogre::Node.
 *
 * The actual concrete type of node (basically either a normal Ogre::SceneNode or an Ogre::TagPoint) is determined by the INodeProvider instance used.
 * Most entities in the game are expected to be represented by this class, or any subclass of it.
 * The node will be moved and rotated either by using the values from the entity to which it's attached, or through an instance of IEntityControlDelegate set through the setControlDelegate() method.
 */
class NodeAttachment: public AttachmentBase, public virtual sigc::trackable
{
public:

	/**
	 * @brief Ctor.
	 *
	 * You must call init() after an instance has been created.
	 *
	 * @param parentEntity The parent entity.
	 * @param childEntity The child entity.
	 * @param nodeProvider A node provider instance, not null. Ownership will be passed to this instance.
	 */
	NodeAttachment(EmberEntity& parentEntity, EmberEntity& childEntity, INodeProvider* nodeProvider);

	/**
	 * @brief Dtor.
	 * During destruction the INodeProvider instance will be destroyed.
	 */
	virtual ~NodeAttachment();

	/**
	 * @brief Initializes the attachment. This must be called after an instance has been created.
	 */
	virtual void init();

	virtual void setControlDelegate(IEntityControlDelegate* controllerDelegate);

	virtual IEntityControlDelegate* getControlDelegate() const;

	virtual IEntityAttachment* attachEntity(EmberEntity& entity);

	virtual void updatePosition();

	/**
	 * General method for turning on and off debug visualizations. Subclasses might support more types of visualizations than the ones defined here.
	 * @param visualization The type of visualization. Currently supports "OgreBBox".
	 * @param visualize Whether to visualize or not.
	 */
	virtual void setVisualize(const std::string& visualization, bool visualize);

	/**
	 * @brief Gets whether a certain visualization is turned on or off.
	 * @param visualization The type of visualization. Currently supports "OgreBBox".
	 * @return true if visualization is turned on, else false
	 */
	virtual bool getVisualize(const std::string& visualization) const;

	/**
	 * @brief Sets the position and orientation of the node.
	 * @param position The position.
	 * @param orientation The orientation.
	 * @param velocity The velocity.
	 */
	virtual void setPosition(const WFMath::Point<3>& position, const WFMath::Quaternion& orientation, const WFMath::Vector<3>& velocity);

	/**
	 * @brief Gets the node to which the graphical representation of the child entity is attached.
	 */
	Ogre::Node* getNode() const;


protected:

	/**
	 * @brief A node provider instance, from which the mNode instance is created.
	 * Also used when child attachment are created.
	 */
	INodeProvider* mNodeProvider;

	/**
	 * @brief The controller used for the attachment.
	 * This is normally either a NodeController instance, which will use the position and orientation of the entity directly, or a DelegatingNodeController which will use an instance IEntityControlDelegate.
	 */
	NodeController* mAttachmentController;

	/**
	 * @brief Setup listening of entity events.
	 */
	void setupListeners();

	/**
	 * @brief When the visibility of the entity is changed, we need to alter the visibility of the node.
	 * @param visible Whether the entity is visible or not.
	 */
	void entity_VisibilityChanged(bool visible);

	/**
	 * @brief Sets the visibility of the attachment.
	 * The implementation in NodeAttachment will ask the node provider to set the visibility, but there's an option for any subclass to do additional work.
	 */
	virtual void setVisible(bool visible);

};

}
}
#endif /* EMBEROGRENodeAttachment_H_ */
