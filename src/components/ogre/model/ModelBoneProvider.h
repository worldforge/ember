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

#ifndef MODELBONEPROVIDER_H_
#define MODELBONEPROVIDER_H_

#include "components/ogre/INodeProvider.h"
#include "components/ogre/model/ModelDefinition.h"
#include "components/ogre/model/Model.h"
#include <string>

namespace Ogre {
class TagPoint;
class SceneNode;
class MovableObject;
}

namespace Ember {
namespace OgreView {
namespace Model {

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Provides nodes for skeleton bones.
 *
 * When a MovableObject is attached to a skeleton the resulting Node is a TagPoint. This differs some from a SceneNode, which is the normal node we deal with.
 * The largest difference is that a SceneNode can be nestled in a hierarchy of SceneNodes: not so with the TagPoint. A TagPoint can have no children.
 * Furthermore, a TagPoint can only have one MovableObject attached to it.
 * These restrictions make it so that we need to provide our own hierarchy, so that child ModelBoneProvider instances are registered with the instance that they are children of. When the position or orientation of the parent changes, so must the children also be updated.
 */
class ModelBoneProvider : public INodeProvider {
public:
	/**
	 * @brief Ctor.
	 * @param parentModel The model to which the bone will be attached.
	 * @param attachPoint A named attach point on the model.
	 * @param movableObject An optional movable object to attach. Note that if no object is submitted, no TagPoint will be created.
	 */
	ModelBoneProvider(Ogre::Node* parentSceneNode, Model& parentModel, const std::string& attachPointName, bool deleteMovableWhenDone = false);

	/**
	 * @brief Dtor.
	 * At destruction the movable object will be detached from the model.
	 */
	virtual ~ModelBoneProvider();

	Ogre::Node* getNode() const override;

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

	void setOffsets(const Ogre::Vector3& translate, const Ogre::Quaternion& rotate) override;

	void setScale(const Ogre::Vector3& scale) override;

	void detachObject(Ogre::MovableObject* movable) override;

	void attachObject(Ogre::MovableObject* movable) override;

protected:

	typedef std::vector<ModelBoneProvider*> ModelBoneProviderStore;

	/**
	 * @brief Ctor.
	 * @param parentModel The model to which the bone will be attached.
	 * @param attachPoint A named attach point on the model.
	 * @param movableObject An optional movable object to attach. Note that if no object is submitted, no TagPoint will be created.
	 * @param parent The parent provider.
	 */
	ModelBoneProvider(Ogre::Node* parentSceneNode, Model& parentModel, const std::string& attachPointName, ModelBoneProvider* parent);


	/**
	 * @brief The scene node to which everything is attached.
	 *
	 */
	Ogre::Node* mNode;

	/**
	 * @brief The model to which this TagPoint is attached.
	 */
	Model& mParentModel;

	/**
	 * @brief The name of the attach point used by this instance.
	 */
	std::string mAttachPointName;

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
	 * @brief The attach point definition for the attach points.
	 *
	 * This should only be set for the top ModelBoneProvider in any such hierarchy. Any child instance should have this set to null.
	 */
	AttachPointDefinition mAttachPointDefinition;

	/**
	 * @brief If true, the mAttachedObject field should be deleted when this instance is destroyed.
	 *
	 * This is mainly here to handle the instance where a temporary Ogre::ManualObject is used when attaching entities which lack a Model.
	 */
	bool mDeleteMovableWhenDone;

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

	std::set<Ogre::TagPoint*> mTagPoints;

	Ogre::Vector3 mScale;
	bool mVisible;
	Ogre::Vector3 mOffsetTranslation;
	Ogre::Quaternion mOffsetRotation;
};

}
}

}

#endif /* MODELBONEPROVIDER_H_ */
