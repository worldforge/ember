/*
 Copyright (C) 2004  Erik Hjortsberg
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
#include <OgreEntity.h>
#include "ModelDefinition.h"
#include "ParticleSystem.h"
#include <sigc++/signal.h>
#include <memory>

namespace EmberOgre
{
namespace Model
{

class SubModel;
class SubModelPart;
class ModelDefinition;
class ParticleSystemBinding;
class ParticleSystem;
class Action;
class ModelPart;
class ModelBackgroundLoader;

struct LightInfo
{
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
class Model: public Ogre::MovableObject
{

	friend class ModelDefinition;
	friend class ModelFactory;

public:

	typedef std::map<std::string, Action> ActionStore;

	typedef std::set<SubModel*> SubModelSet;
	typedef std::set<std::string> StringSet;
	typedef std::map<std::string, StringSet> SubModelPartMapping;
	typedef std::map<std::string, ModelPart> ModelPartStore;

	typedef std::map<std::string, std::vector<std::string> > PartGroupStore;

	struct AttachPointWrapper
	{
		Ogre::TagPoint* TagPoint;
		AttachPointDefinition Definition;
		Ogre::MovableObject *Movable;
	};
	typedef std::vector<AttachPointWrapper> AttachPointWrapperStore;

	static const Ogre::String sMovableType;

	/**
	 * @brief Ctor.
	 */
	virtual ~Model();

	/**
	 * @brief Notify the object of it's manager (internal use only)
	 */
	virtual void _notifyManager(Ogre::SceneManager* man);

	static Model* createModel(Ogre::SceneManager& sceneManager, const std::string& modelType, const std::string& name = "");

	/**
	 * @brief Reloads the model from the modeldefinition.
	 */
	void reload();

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
	const Ogre::Real getScale() const;

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
	const ModelDefinition::UseScaleOf getUseScaleOf() const;

	// 	inline const SubModelPartStoreMap& getSubmodelParts() const;

	const SubModelSet& getSubmodels() const;

	//SubModel* getSubModel(const std::string& name);
	SubModel* getSubModel(size_t index);

	Ogre::AnimationState* getAnimationState(const Ogre::String& name);
	Ogre::AnimationStateSet* getAllAnimationStates();
	Ogre::SkeletonInstance * getSkeleton();
	Ogre::TagPoint* attachObjectToBone(const Ogre::String &boneName, Ogre::MovableObject *pMovable, const Ogre::Quaternion &offsetOrientation = Ogre::Quaternion::IDENTITY, const Ogre::Vector3 &offsetPosition = Ogre::Vector3::ZERO);
	Ogre::MovableObject * detachObjectFromBone(const Ogre::String &movableName);
	void detachAllObjectsFromBone(void);

	/** @see Ogre::MovableObject::setRenderingDistance(Ogre::Real dist)
	 */
	virtual void setRenderingDistance(Ogre::Real dist);

	/** Overridden - see MovableObject.
	 */
	// 		inline virtual Ogre::Real getRenderingDistance   (   void    )    const;

	/** Overridden - see MovableObject.
	 */
	virtual void _notifyCurrentCamera(Ogre::Camera* cam);

	virtual void _notifyMoved();

	/** Overridden - see MovableObject.
	 */
	virtual void setUserObject(Ogre::UserDefinedObject *obj);

	/// Overridden - see MovableObject.
	virtual void setRenderQueueGroup(Ogre::RenderQueueGroupID queueID);

	/** Overridden - see MovableObject.
	 */
	virtual const Ogre::AxisAlignedBox& getBoundingBox(void) const;

	/** Overridden - see MovableObject.
	 */
	virtual const Ogre::AxisAlignedBox& getWorldBoundingBox(bool derive) const;

	virtual Ogre::Real getBoundingRadius() const;

	/** Overridden - see MovableObject.
	 */
	virtual void _updateRenderQueue(Ogre::RenderQueue* queue);

	/** Overridden from MovableObject */
	//	virtual const Ogre::String& getName(void) const;

	/** Overridden from MovableObject */
	virtual const Ogre::String& getMovableType(void) const;

	/** Overridden from MovableObject */
	virtual void setQueryFlags(unsigned long flags);

	/** Overridden from MovableObject */
	virtual void addQueryFlags(unsigned long flags);

	/** Overridden from MovableObject */
	virtual void removeQueryFlags(unsigned long flags);

	/** Overridden from MovableObject */
	virtual void _notifyAttached(Ogre::Node* parent, bool isTagPoint = false);

	/** Overridden from MovableObject */
	virtual bool isVisible(void) const;

	/**
	 * @copydoc Ogre::MovableObject::visitRenderables()
	 */
	virtual void visitRenderables(Ogre::Renderable::Visitor* visitor, bool debugRenderables);

	/**
	 *    returns a pointer to the defintion of the Model
	 * @return
	 */
	ModelDefnPtr getDefinition() const;

	Ogre::TagPoint* attachObjectToBone(const Ogre::String &boneName, Ogre::MovableObject *pMovable, const Ogre::Quaternion &offsetOrientation = Ogre::Quaternion::IDENTITY, const Ogre::Vector3 &offsetPosition = Ogre::Vector3::ZERO, const Ogre::Vector3 &scale = Ogre::Vector3::UNIT_SCALE);
	AttachPointWrapper attachObjectToAttachPoint(const Ogre::String &boneName, Ogre::MovableObject *pMovable, const Ogre::Vector3 &scale = Ogre::Vector3::UNIT_SCALE, const Ogre::Quaternion &offsetOrientation = Ogre::Quaternion::IDENTITY, const Ogre::Vector3 &offsetPosition = Ogre::Vector3::ZERO);
	//void attachObjectToAttachPoint(const Ogre::String &attachPointName, Ogre::MovableObject *pMovable, const Ogre::Quaternion &offsetOrientation=Ogre::Quaternion::IDENTITY, const Ogre::Vector3 &offsetPosition=Ogre::Vector3::ZERO);

	bool hasAttachPoint(const std::string& attachPoint) const;

	const ParticleSystemBindingsPtrSet& getAllParticleSystemBindings() const;

	ParticleSystemSet& getParticleSystems();

	bool hasParticles() const;

	LightSet& getLights();

	/**
	 Returns a store of AttachPointWrapper objects, which represents all attached objects.
	 @returns a pointer to an AttachPointWrapperStore instance, or null
	 */
	const AttachPointWrapperStore* getAttachedPoints() const;

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

protected:

	/**
	 *    Creates a new Model instance with the specified name.
	 *    Remember to call create(...) to create the actual meshes.
	 * @param name
	 * @return
	 */
	Model(const std::string& name);

	/**
	 * Try to create the needed meshes from the specified modeldef.
	 * Returns true if successful, else false.
	 * @param modelType
	 * @return
	 */
	bool create(const std::string& modelType); // create model of specific type

	bool createActualModel();

	static unsigned long msAutoGenId;
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

	Ogre::Entity::ChildObjectList mChildObjectList;

	mutable Ogre::AxisAlignedBox mFull_aa_box;
	mutable Ogre::AxisAlignedBox mWorldFull_aa_box;

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
	 modeldef this was copied from
	 */
	ModelDefnPtr mMasterModel;

	/**
	 a set of all submodels belonging to the model
	 */
	SubModelSet mSubmodels;
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

	bool createFromDefn();

	std::auto_ptr<AttachPointWrapperStore> mAttachPoints;

	ModelBackgroundLoader* mBackgroundLoader;

};

// Ogre::Real Model::getRenderingDistance   (   void    )    const
// {
// 	return _masterModel->getRenderingDistance();
// }

inline ModelDefnPtr Model::getDefinition() const
{
	return mMasterModel;
}

// // const Model::SubModelPartStoreMap& Model::getSubmodelParts() const
// // {
// // 	return mSubModelPartMap;
// // }

inline const Model::SubModelSet& Model::getSubmodels() const
{
	return mSubmodels;
}

/** Factory object for creating Model instances */
class ModelFactory: public Ogre::MovableObjectFactory
{
protected:
	Ogre::MovableObject* createInstanceImpl(const Ogre::String& name, const Ogre::NameValuePairList* params);
public:
	ModelFactory()
	{
	}
	~ModelFactory()
	{
	}

	static Ogre::String FACTORY_TYPE_NAME;

	const Ogre::String& getType(void) const;
	void destroyInstance(Ogre::MovableObject* obj);

};

}
}
#endif // MODEL_H
