/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#ifndef MODELBONEPROVIDER_H_
#define MODELBONEPROVIDER_H_

#include "components/ogre/INodeProvider.h"
#include "components/ogre/model/ModelDefinition.h"
#include <string>

namespace Ogre
{
class TagPoint;
class MovableObject;
}

namespace EmberOgre
{
namespace Model
{
class Model;

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Provides nodes for skeleton bones.
 *
 * When a MovableObject is attached to a skeleton the resulting Node is a TagPoint. This differs some from a SceneNode, which is the normal node we deal with.
 * The largest difference is that a SceneNode can be nestled in a hierarchy of SceneNodes: not so with the TagPoint. A TagPoint can have no children.
 * Furthermore, a TagPoint can only have one MovableObject attached to it.
 * These restrictions make it so that we need to provide our own hierarchy, so that child ModelBoneProvider instances are registered with the instance that they are children of. When the position or orientation of the parent changes, so must the children also be updated.
 */
class ModelBoneProvider: public INodeProvider
{
public:
	/**
	 * @brief Ctor.
	 * @param parentModel The model to which the bone will be attached.
	 * @param attachPoint A named attach point on the model.
	 * @param movableObject An optional movable object to attach. Note that if no object is submitted, no TagPoint will be created.
	 */
	ModelBoneProvider(Model& parentModel, const std::string& attachPointName, Ogre::MovableObject* movableObject);

	/**
	 * @brief Dtor.
	 * At destruction the movable object will be detached from the model.
	 */
	virtual ~ModelBoneProvider();
	virtual Ogre::Node& getNode() const;
	virtual Ogre::Node* getParentNode() const;
	virtual INodeProvider* createChildProvider(Ogre::MovableObject* attachedObject = 0);
	virtual void setVisible(bool visible);

	/**
	 * General method for turning on and off debug visualizations. Subclasses might support more types of visualizations than the ones defined here.
	 * @param visualization The type of visualization. Currently supports "OgreBBox" and "ErisBBox".
	 * @param visualize Whether to visualize or not.
	 */
	virtual void setVisualize(const std::string& visualization, bool visualize);

	/**
	 * @brief Gets whether a certain visualization is turned on or off.
	 * @param visualization The type of visualization. Currently supports "OgreBBox" and "ErisBBox".
	 * @return true if visualization is turned on, else false
	 */
	virtual bool getVisualize(const std::string& visualization) const;

	/**
	 * @brief Sets the position and orientation of the node.
	 * @param position The position.
	 * @param orientation The orientation.
	 */
	virtual void setPositionAndOrientation(const Ogre::Vector3& position, const Ogre::Quaternion& orientation);

protected:

	typedef std::vector<ModelBoneProvider*> ModelBoneProviderStore;

	/**
	 * @brief Ctor.
	 * @param parentModel The model to which the bone will be attached.
	 * @param attachPoint A named attach point on the model.
	 * @param movableObject An optional movable object to attach. Note that if no object is submitted, no TagPoint will be created.
	 * @param parent The parent provider.
	 */
	ModelBoneProvider(Model& parentModel, const std::string& attachPointName, Ogre::MovableObject* movableObject, ModelBoneProvider* parent);

	/**
	 * @brief The model to which this TagPoint is attached.
	 */
	Model& mParentModel;

	/**
	 * @brief The name of the attach point used by this instance.
	 */
	std::string mAttachPointName;

	/**
	 * @brief The TagPoint created by this instance.
	 * @note This can be null if there's no MovableObject available, since one can only create a TagPoint if there's a MovableObject which can be attached.
	 */
	Ogre::TagPoint* mNode;

	/**
	 * @brief An optional movable object which is attached to the Model.
	 * @note If there's no MovableObject, there's also no TagPoint available.
	 */
	Ogre::MovableObject* mAttachedObject;

	/**
	 * @brief The attach point definition.
	 */
	AttachPointDefinition mAttachPointDefinition;

	/**
	 * @brief The parent provider, if any.
	 * This is mainly used for getting derived position and orientation.
	 */
	ModelBoneProvider* mParent;

	/**
	 * @brief A store of any child providers.
	 * Whenever the positioning or orientation of this instance changes, all children needs to be updated.
	 * The reason for this is that unlike the SceneNodes, TagPoints aren't in a hierarchy, so changing the position of one won't affect any others.
	 */
	ModelBoneProviderStore mChildren;

	/**
	 * @brief The local position of this instance.
	 * When positioning the TagPoint, the derived position, which takes into account the positions of the parent entities, is used however.
	 */
	Ogre::Vector3 mPosition;

	/**
	 * @brief The local orientation of this instance.
	 * When positioning the TagPoint, the derived orientation, which takes into account the orientation of the parent entities, is used however.
	 */
	Ogre::Quaternion mOrientation;

	/**
	 * @brief Updates the position and orientation of the entity, taking parent model attachments into account.
	 * Call this whenever a parent attachment has changed. Since bone nodes can't be put in a hierarchy like scene nodes, we need to cascade any update on a parent node down to all the child nodes.
	 */
	void updatePositionAndOrientation();

	/**
	 * @brief Gets the derived position of this instance in relation to the model it's attached to.
	 * @return The derived position in relation to the model it's attached to.
	 */
	Ogre::Vector3 getDerivedPosition() const;

	/**
	 * @brief Gets the derived orientation of this instance in relation to the model it's attached to.
	 * @return The derived orientation in relation to the model it's attached to.
	 */
	Ogre::Quaternion getDerivedOrientation() const;

private:

	/**
	 * @brief Initializes the instance.
	 * This is called by the constructor only.
	 */
	void init();


};

}
}

#endif /* MODELBONEPROVIDER_H_ */
