/*
 Copyright (C) 2004  Erik Ogenvik
 Copyright (c) 2005 The Cataclysmos Team

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

#ifndef MODEL_H
#define MODEL_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include "ModelDefinition.h"
#include "ParticleSystem.h"
#include "ModelAction.h"
#include "ModelPart.h"
#include <OgreEntity.h>
#include <sigc++/signal.h>
#include <memory>
#include <unordered_map>
#include <components/ogre/INodeProvider.h>
#include <components/ogre/EmberEntityUserObject.h>
#include <stack>

namespace Eris {
class EventService;
}

namespace Ember {
namespace OgreView {

/**
 * @brief Namespace for Models, which is the main aggregate object used for representing entities in the world.
 */
namespace Model {

class SubModel;

class SubModelPart;

class ModelDefinition;

class ParticleSystemBinding;

class ParticleSystem;

class Action;

class ModelPart;

class ModelBackgroundLoader;

struct LightInfo {
	Ogre::Light* light;
	Ogre::Vector3 position;
};

typedef std::vector<ParticleSystem*> ParticleSystemSet;
typedef std::vector<ParticleSystemBinding> ParticleSystemBindingsSet;
typedef std::vector<LightInfo> LightSet;

/**
 * @brief An instance of this represents a complete model, comprised of both multiple meshes, particle systems and lights.
 * A model can be made out of different entities, just as long as they share a skeleton.
 * The model consists of different parts, represented by instances of SubModelPart.
 * Typical parts would be a body or a shirt. These parts can be turned on or off.
 * That allows for the use of a single mesh with many different submeshes.
 *
 * A model is typically instantiated from a modeldef.xml file through the use
 * of createFromXML(...)
 */
class Model {

	friend class ModelDefinition;

	friend class ModelFactory;

public:

	typedef std::unordered_map<std::string, Action> ActionStore;

	typedef std::set<std::string> StringSet;
	typedef std::unordered_map<std::string, StringSet> SubModelPartMapping;
	typedef std::unordered_map<std::string, ModelPart> ModelPartStore;

	typedef std::unordered_map<std::string, std::vector<std::string>> PartGroupStore;

	struct AttachPointWrapper {
		Ogre::TagPoint* TagPoint;
		AttachPointDefinition Definition;
		Ogre::MovableObject* Movable;
	};

	/**
	 *    Creates a new Model instance with the specified name.
	 *    Remember to call create(...) to create the actual meshes.
	 * @param name
	 * @return
	 */
	Model(Ogre::SceneManager& manager, Ogre::SharedPtr<ModelDefinition> definition, const std::string& name = "");

	/**
	 * @brief Ctor.
	 */
	virtual ~Model();

	bool load();

	/**
	 * @brief Reloads the model from the modeldefinition.
	 */
	bool reload();

	/**
	 * @brief Emitted when the model is reloaded
	 */
	sigc::signal<void> Reloaded;

	/**
	 * @brief Emitted when the model is about to be resetted.
	 */
	sigc::signal<void> Resetting;

	bool addSubmodel(SubModel* submodel);

	bool removeSubmodel(SubModel* submodel);

	/**
	 * @brief Gets the action with the supplied name. If none is found null is returned.
	 * @param name The name of the action.
	 * @return A pointer to an action, or null.
	 */
	Action* getAction(const std::string& name);

	/**
	 * @brief Gets the action for the supplied type and trigger. If none could be found null is returned.
	 * @param type The type of trigger.
	 * @param trigger The actual trigger.
	 * @return A pointer to an action, or null.
	 */
	Action* getAction(const ActivationDefinition::Type type, const std::string& trigger);

	// 	ModelPart& getPart(const std::string& partName);

	/**
	 * @brief Shows a certain part of the model.
	 * If no part with the supplied name can be found, nothing happens.
	 * @param partName The name of the part to show.
	 * @param hideOtherParts If true, all other parts that share the same group as the part which will be shown will be hidden.
	 */
	void showPart(const std::string& partName, bool hideOtherParts = true);

	/**
	 * @brief Hides a certain part.
	 * @param partName The name of the part to hide.
	 * @param dontChangeVisibility If true, the visibility of the part won't be affected. In addition, any other part which belong to the same group and is set to be visible, won't automatically be shown.
	 */
	void hidePart(const std::string& partName, bool dontChangeVisibility = false);

	/**
	 * @brief Sets the visibility of the model and all of it subparts.
	 * @param visible Whether the model should be visible or not.
	 */
	void setVisible(bool visible);

	/**
	 * if defined in the modeldef, returns a scaler by which the node containing
	 * the model can be scaled
	 */
	Ogre::Real getScale() const;

	/**
	 * Defines how much the model should be rotated.
	 */
	const Ogre::Quaternion& getRotation() const;

	/**
	 * if defined in the modeldef, returns an axis by which the model can be scaled
	 * I.e. when dealing with something such as a fir tree, you want to use the
	 * height of the tree to determine how much it should be scaled, since the
	 * bounding box supplied by eris doesn't take the branches into account
	 */
	ModelDefinition::UseScaleOf getUseScaleOf() const;

	// 	inline const SubModelPartStoreMap& getSubmodelParts() const;

	const std::set<SubModel*>& getSubmodels() const;

	//SubModel* getSubModel(const std::string& name);
	SubModel* getSubModel(size_t index);

	Ogre::AnimationState* getAnimationState(const Ogre::String& name);

	Ogre::AnimationStateSet* getAllAnimationStates();

	Ogre::SkeletonInstance* getSkeleton() const;

	/** @see Ogre::MovableObject::setRenderingDistance(Ogre::Real dist)
	 */
	void setRenderingDistance(Ogre::Real dist);


	void setQueryFlags(unsigned int flags);

	Ogre::TagPoint* attachObject(const std::string& attachPoint, Ogre::MovableObject* movable);

	void detachObject(Ogre::MovableObject* movable);

	bool hasAttachPoint(const std::string& attachPoint) const;

	const ParticleSystemBindingsPtrSet& getAllParticleSystemBindings() const;

	ParticleSystemSet& getParticleSystems();

	bool hasParticles() const;

	LightSet& getLights();

	/**
	 Returns a store of AttachPointWrapper objects, which represents all attached objects.
	 @returns a pointer to an AttachPointWrapperStore instance, or null
	 */
	const std::unique_ptr<std::vector<AttachPointWrapper>>& getAttachedPoints() const;

	/**
	 *    If set to true, Ogre will display each bone in the skeleton with an axis overlay. Defaults to false.
	 * @param display Whether to display the debug bone overlay or not.
	 */
	void setDisplaySkeleton(bool display);

	/**
	 *    Whether the debug bone overlay for the skeleton is shown or not.
	 * @param
	 * @return
	 */
	bool getDisplaySkeleton(void) const;

	/**
	 * @brief Whether the model has been fully loaded.
	 * @return True if all assets that make up the model, i.e. meshes and materials, have been loaded.
	 */
	bool isLoaded() const;

	const Ogre::SharedPtr<ModelDefinition>& getDefinition() const;

	const std::string& getName() const;

	void attachToNode(INodeProvider* nodeProvider);

	void setUserObject(std::shared_ptr<EmberEntityUserObject> mUserObject);

	Ogre::SceneManager& getManager();

	float getCombinedBoundingRadius() const;

	Ogre::AxisAlignedBox getCombinedBoundingBox() const;

	const INodeProvider* getNodeProvider() const;

protected:

	struct AssetCreationContext {
		size_t mCurrentlyLoadingSubModelIndex = 0;
		std::set<SubModel*> mSubmodels;
		std::vector<std::string> showPartVector;
		ModelPartStore mModelParts;
		PartGroupStore mGroupsToPartMap;
	};

	/**
	 * Creates the assets that make up the model. In most cases, multiple calls needs to be made to this method to fully create all assets.
	 * @return If all assets have been created.
	 */
	bool createModelAssets();

	ParticleSystemBindingsPtrSet mAllParticleSystemBindings;
	ParticleSystemSet mParticleSystems;
	LightSet mLights;

	/**
	 *    Clears all the submodels
	 */
	void resetSubmodels();

	void createActions();

	void createParticles();

	void createLights();

	/**
	 * @brief Clears all the particles, removing them from the scene.
	 */
	void resetParticles();

	/**
	 * @brief Clears all lights, removing them from the scene.
	 */
	void resetLights();

	/**
	 * @brief Resets the whole model.
	 */
	void reset();

	/**
	 * @brief Updates the mMovableObjects store with the child movable objects.
	 *
	 * This must be called whenever a SubModel, ParticleSystem or Light is added or removed.
	 */
	//void refreshMovableList();

	void addMovable(Ogre::MovableObject* movable);

	void removeMovable(Ogre::MovableObject* movable);

	Ogre::SceneManager& mManager;

	Ogre::SharedPtr<ModelDefinition> mDefinition;

	INodeProvider* mParentNodeProvider;


	const std::string mName;

	Ogre::Entity::ChildObjectList mChildObjectList;

	/**
	 if the model has a skeleton, it can be shared between many different entities
	 this denotes the "owning" entity
	 */
	Ogre::Entity* mSkeletonOwnerEntity;

	/**
	 all actions belonging to the model
	 */
	ActionStore mActions;

	/**
	 a set of all submodels belonging to the model
	 */
	std::set<SubModel*> mSubmodels;
	/**
	 a set of all submodelparts belonging to the model (in reality they belong to the submodels though)
	 */
	// 	SubModelPartStoreMap mSubModelPartMap;

	ModelPartStore mModelParts;

	PartGroupStore mGroupsToPartMap;

	Ogre::SkeletonInstance* mSkeletonInstance;

	/**
	 how much to scale the model from it's initial size
	 */
	Ogre::Real mScale;
	/**
	 how much the model should be rotated around the Y-axis from it's initial position
	 */
	Ogre::Quaternion mRotation;

	/**
	 set of all animation states
	 */
	Ogre::AnimationStateSet* mAnimationStateSet;

	std::unique_ptr<std::vector<AttachPointWrapper>> mAttachPoints;

	/**
	 * @brief A store of the movable objects which make up this model.
	 *
	 * This is only use for performance reasons, to make iteration over the child movable objects faster.
	 * Since this happens pretty frequently each frame it's important that it's as efficient as possible.
	 *
	 * The contents of the list mirrors the sum of mSubmodels, mParticleSystems and mLight.
	 */
	std::vector<Ogre::MovableObject*> mMovableObjects;

	bool mVisible;

	std::shared_ptr<EmberEntityUserObject> mUserObject;
	Ogre::Real mRenderingDistance;
	uint32_t mQueryFlags;
	bool mLoaded;
	AssetCreationContext mAssetCreationContext;
	//size_t mCurrentlyLoadingPartIndex;
};

inline const std::set<SubModel*>& Model::getSubmodels() const {
	return mSubmodels;
}

inline const std::string& Model::getName() const {
	return mName;
}

}
}
}
#endif // MODEL_H
