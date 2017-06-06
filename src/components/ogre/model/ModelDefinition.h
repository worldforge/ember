//
// C++ Interface: ModelDefinition
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2004
// Copyright (c) 2005 The Cataclysmos Team
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
#ifndef EMBEROGREMODELDEFINITION_H
#define EMBEROGREMODELDEFINITION_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include <OgreQuaternion.h>
#include <OgreVector3.h>
#include <OgreLight.h>
#include <OgreColourValue.h>
#include <OgreResource.h>
#include <OgreSharedPtr.h>
#include <map>
#include <unordered_map>
#include <vector>
#include <unordered_set>

namespace Ember
{
namespace OgreView
{
namespace Model
{

class Model;
class PartDefinition;
class SubModelDefinition;
class ModelDefinition;
class SubEntityDefinition;

class ActionDefinition;
struct SoundDefinition;
class AnimationDefinition;
struct AnimationPartDefinition;
struct ActivationDefinition;
struct AttachPointDefinition;
struct ViewDefinition;
struct BoneGroupDefinition;
struct PoseDefinition;

typedef std::unordered_set<Model*> ModelInstanceStore;

typedef std::vector<SubModelDefinition*> SubModelDefinitionsStore;
typedef std::vector<PartDefinition*> PartDefinitionsStore;
typedef std::vector<SubEntityDefinition*> SubEntityDefinitionsStore;
typedef std::vector<AnimationDefinition*> AnimationDefinitionsStore;
typedef std::vector<AnimationPartDefinition*> AnimationPartDefinitionsStore;
typedef std::vector<SoundDefinition*> SoundDefinitionsStore;
typedef std::vector<ActionDefinition*> ActionDefinitionsStore;
typedef std::vector<AttachPointDefinition> AttachPointDefinitionStore;
typedef std::vector<ActivationDefinition> ActivationDefinitionStore;
typedef std::map<std::string, ViewDefinition*> ViewDefinitionStore;
typedef std::map<std::string, PoseDefinition> PoseDefinitionStore;
typedef std::map<std::string, std::string> StringParamStore;
typedef std::map<std::string, BoneGroupDefinition*> BoneGroupDefinitionStore;

/**
 * @brief A rendering definition.
 *
 * This allows you to specify a different render method than the default one (regular Model).
 * All of this requires that you create functionality for implementing the different schemes that might be specified.
 */
class RenderingDefinition
{
	friend class XMLModelDefinitionSerializer;
public:

	/**
	 * Gets the scheme which will be used.
	 * @return
	 */
	const std::string& getScheme() const;
	/**
	 * Sets the scheme.
	 * @param scheme
	 */
	void setScheme(const std::string& scheme);

	/**
	 * Gets a collection of parameters for the rendering scheme.
	 * @return
	 */
	const StringParamStore& getParameters() const;

private:
	StringParamStore mParams;
	std::string mScheme;
};

/**
 * @brief A definitions of a subentity within a part of a submodel.
 *
 * Each subentity definitions refers to an Ogre::SubEntity (by index in the Ogre::Entity).
 * In addition, a specific material can be defined which overrides the default material of the subentity.
 */
class SubEntityDefinition
{
	friend class PartDefinition;
public:
	const std::string& getSubEntityName() const;
	unsigned int getSubEntityIndex() const;

	const std::string& getMaterialName() const;
	void setMaterialName(const std::string& materialName);

	const PartDefinition& getPartDefinition() const;

private:
	SubEntityDefinition(const std::string& subEntityName, PartDefinition& partdef);
	SubEntityDefinition(unsigned int subEntityIndex, PartDefinition& partdef);
	std::string mSubEntityName;
	std::string mMaterialName;
	unsigned int mSubEntityIndex;
	PartDefinition& mPartDef;
};

/**
 * @brief Defines a part within a Model.
 *
 * Each model can contain many different parts. By turning on and off these parts the presentation of the model can change.
 *
 * A simple example would be a human mesh with different kind of hairs defined.
 * Each hair variant would be represented by a part, and by switching on and off these parts the presentation of the human is changed.
 *
 * Each part can be put into a "group". This mechanisms guarantees that only one part from the same group ever is visible.
 * In our example with hair variants it would be very much suitable to put all parts into the same group since we only ever want one hair variant to be visible at once.
 */
class PartDefinition
{
	friend class SubModelDefinition;
public:
	~PartDefinition();

	void setName(const std::string& name);
	const std::string& getName() const;

	void setGroup(const std::string& group);
	const std::string& getGroup() const;

	void setShow(bool show);
	bool getShow() const;

	SubEntityDefinition* createSubEntityDefinition(const std::string& subEntityName);
	SubEntityDefinition* createSubEntityDefinition(unsigned int subEntityIndex);
	const SubEntityDefinitionsStore& getSubEntityDefinitions() const;
	void removeSubEntityDefinition(SubEntityDefinition* def);

	const SubModelDefinition& getSubModelDefinition() const;

private:
	PartDefinition(const std::string& name, SubModelDefinition& subModelDef);
	std::string mName;
	bool mShow;
	std::string mGroup;
	SubEntityDefinitionsStore mSubEntities;
	SubModelDefinition& mSubModelDef;
};

/**
 * @brief A sub model definition within a Model.
 *
 * This represents one mesh within the Model. Each Model can be made up of multiple meshes, and each mesh is then defined through an instance of this class.
 *
 */
class SubModelDefinition
{
	friend class ModelDefinition;
public:
	~SubModelDefinition();

	const std::string& getMeshName() const;

	PartDefinition* createPartDefinition(const std::string& partname);
	const PartDefinitionsStore& getPartDefinitions() const;
	void removePartDefinition(PartDefinition* def);

	const ModelDefinition& getModelDefinition() const;

private:
	SubModelDefinition(const std::string& meshname, ModelDefinition& modelDef);
	std::string mMeshName;
	PartDefinitionsStore mParts;
	ModelDefinition& mModelDef;
};

/**
 * @brief A simple struct for defining a certain view of the Model.
 *
 * These settings needs to be applied to the camera rendering the Model.
 * The main use of this is when providing previews in icons and similar views.
 */
struct ViewDefinition
{
	/**
	 The name of the view.
	 */
	std::string Name;

	/**
	 The rotation of the camera related to the Model.
	 */
	Ogre::Quaternion Rotation;

	/**
	 The distance of the camera from the Model.
	 */
	float Distance;
};

struct PoseDefinition
{
	/**
	 * @brief The rotation of the model around the translated origin.
	 */
	Ogre::Quaternion Rotate;

	/**
	 * @brief The translation of the original model, to provide a new origin.
	 */
	Ogre::Vector3 Translate;

	/**
	 * @brief If true, the orientation and translation of the entity will be ignored.
	 */
	bool IgnoreEntityData;

};

/**
 * @brief Definition of an attach point.
 *
 * An "attach point" is a place where child entities can be attached. A typical example would be something wielded in the hand of a human.
 * Each attach point has a name, which is used to bind it to the Atlas entity data, and a "bone name" which is used to determine what bone in the Ogre skeleton to bind to.
 */
struct AttachPointDefinition
{
	std::string Name;
	std::string BoneName;
	std::string Pose;

	/**
	 * @brief Rotation of the attach point.
	 */
	Ogre::Quaternion Rotation;

	/**
	 * @brief Translation of the attach point.
	 */
	Ogre::Vector3 Translation;
};

/**
 * @brief A reference to a bone group, with an optional weight.
 */
struct BoneGroupRefDefinition
{
	std::string Name;
	float Weight;
};

/**
 * @brief Definition of an animation part.
 *
 * An animation is comprised of many animation parts, where each part refers to an Ogre::Animation.
 * When the animation is played, the different parts are blended together.
 *
 * In addition, each part can have a weight attached to it which determines how much it should be blended.
 */
struct AnimationPartDefinition
{
	/**
	 * The name of the animation.
	 */
	std::string Name;

	/**
	 * @brief The bone groups, if any, which should affect this animation.
	 */
	std::vector<BoneGroupRefDefinition> BoneGroupRefs;

};

/**
 * @brief A definition of a bone group.
 *
 * A bone group is used to define blend weights to bones, so that different animations can be used together.
 * A typical example would be of one animation which only affects a human character's hands, and another animation which only affects the legs.
 * To make these blend together one would have to add bone groups where one the bones affecting the arms are added to one group, and the bones affecting the legs added to the other group.
 */
struct BoneGroupDefinition
{
	/**
	 * @brief The name of the group.
	 */
	std::string Name;

	/**
	 * @brief The bones which are affected by the group, specified through their indices.
	 */
	std::vector<size_t> Bones;

};

/**
 * @brief A definition of an animation.
 *
 * Each animation is made up of one or many "animation parts" which mainly refers to which instances of Ogre::Animation to play.
 *
 * In addition, each animation can be specified to iterate a certain number of times.
 */
class AnimationDefinition
{
	friend class ActionDefinition;
public:
	~AnimationDefinition();
	AnimationPartDefinition* createAnimationPartDefinition(const std::string& ogreAnimationName);
	const AnimationPartDefinitionsStore& getAnimationPartDefinitions() const;
	void removeAnimationPartDefinition(AnimationPartDefinition* def);

	int getIterations() const;

	/**
	 * @brief Sets the number of iterations for the animation.
	 * @param iterations The number of iterations for the animation.
	 */
	void setIterations(int iterations);

private:
	AnimationDefinition(int iterations);

	AnimationPartDefinitionsStore mAnimationParts;
	std::string mName;
	int mIterations;
};

/**
 * @brief Definition of a sound to play for a certain action.
 */
struct SoundDefinition
{
	std::string groupName;
	unsigned int playOrder;
};

/**
 * @brief A definition for an action activation.
 *
 * This structure is used to determine when an action should be played.
 */
struct ActivationDefinition
{
	/**
	 * @brief The type of activation.
	 */
	enum Type
	{
		/**
		 * @brief Activation through change of movement type.
		 */
		MOVEMENT,

		/**
		 * @brief Activation through an entity action.
		 */
		ACTION,

		/**
		 * @brief Activation through a task being carried out.
		 */
		TASK
	};

	/**
	 * @brief The type of the activation.
	 */
	Type type;

	/**
	 * @brief The trigger for the activation.
	 *
	 * The interpretation of this value is dependent on the type.
	 */
	std::string trigger;
};

/**
 * @brief Definition of an action.
 *
 * An "action" is comprised of both animations and sounds.
 * A typical example would be a "dig" action, which would use both an animation for showing the digging operation as well as some sound (perhaps grunting and the sound of a shovel digging in the ground).
 */
class ActionDefinition
{
	friend class ModelDefinition;
public:
	~ActionDefinition();

	AnimationDefinition* createAnimationDefinition(int iterations = 1);
	const AnimationDefinitionsStore& getAnimationDefinitions() const;
	AnimationDefinitionsStore& getAnimationDefinitions();
	void removeAnimationDefinition(AnimationDefinition* def);

	SoundDefinition* createSoundDefinition(const std::string& groupName, unsigned int play);
	const SoundDefinitionsStore& getSoundDefinitions() const;
	SoundDefinitionsStore& getSoundDefinitions();
	void removeSoundDefinition(SoundDefinition* def);

	void createActivationDefinition(const ActivationDefinition::Type& type, const std::string& trigger);
	const ActivationDefinitionStore& getActivationDefinitions() const;
	ActivationDefinitionStore& getActivationDefinitions();

	const std::string& getName() const;
	Ogre::Real getAnimationSpeed() const;
	void setAnimationSpeed(Ogre::Real speed);

private:
	ActionDefinition(const std::string& name);

	std::string mName;
	AnimationDefinitionsStore mAnimations;
	SoundDefinitionsStore mSounds;
	ActivationDefinitionStore mActivations;
	Ogre::Real mAnimationSpeed;
};

/**
 * @author Erik Ogenvik
 * @brief Definition of a Model.
 *
 * A "Model" is a very broad concept which allows any entity in the world to have a visible and audible representation.
 * It can hold both meshes, sounds, lights and particle effects.
 *
 * The most common use is to show a mesh though.
 */
class ModelDefinition: public Ogre::Resource
{

	friend class XMLModelDefinitionSerializer;
	friend class Model;

public:

	/**
	 * @brief Whether to use a certain axis for scaling.
	 * For example, if you use a model of a human you probably want to scale according to the height.
	 * This might mean that width and depths aren't correct though.
	 */
	enum class UseScaleOf
	{
		/**
		 * @brief Scale in all sizes, so that the bounding box of the model exactly matches the entity bounding box.
		 */
		MODEL_ALL = 0,

		/**
		 * @brief Perform no scaling of the model.
		 */
		MODEL_NONE = 1,

		/**
		 * @brief Scale the model so that it matches the width of the entity bounding box.
		 */
		MODEL_WIDTH = 2,

		/**
		 * @brief Scale the model so that it matches the depth of the entity bounding box.
		 */
		MODEL_DEPTH = 3,

		/**
		 * @brief Scale the model so that it matches the height of the entity bounding box.
		 */
		MODEL_HEIGHT = 4
	};

	ModelDefinition(Ogre::ResourceManager* creator, const Ogre::String& name, Ogre::ResourceHandle handle, const Ogre::String& group, bool isManual = false, Ogre::ManualResourceLoader* loader = 0);

	virtual ~ModelDefinition();

	bool isValid(void) const;
	void setValid(bool valid);

	//Ogre resource virtual functions
	void loadImpl(void);

	void unloadImpl(void);

	size_t calculateSize(void) const;

	/**
	 * @brief Gets the amount of scale that needs to be applied to derived Models.
	 * @return
	 */
	Ogre::Real getScale() const;
	void setScale(Ogre::Real scale);

	/**
	 * @brief Gets how derived Models needs to be scaled.
	 * Defaults to "ALL"
	 * @return
	 */
	UseScaleOf getUseScaleOf() const;
	void setUseScaleOf(const UseScaleOf useScale);

	/**
	 * @brief Gets an optional translation vector which should be applied to derived Models.
	 * @return
	 */
	const Ogre::Vector3& getTranslate() const;
	void setTranslate(const Ogre::Vector3 translate);

	/**
	 *	Whether contained entities should be shown or not.
	 * Defaults to true.
	 * @return true if contained entities should be shown, else false
	 */
	bool getShowContained() const;
	void setShowContained(bool show);

	/**
	 * @brief If set to something else than 0, all models beyond this distance won't be shown.
	 * @return
	 */
	float getRenderingDistance() const;
	void setRenderingDistance(float distance);

	/**
	 * @brief Returns a vector defining how much, if ever, contained entities should be offset.
	 * If they shouldn't, Ogre::Vector3::ZERO will be returned.
	 * @return A offset vector.
	 */
	const Ogre::Vector3& getContentOffset() const;
	void setContentOffset(const Ogre::Vector3&);

	/**
	 * @brief Gets the rotation of the model.
	 * @return
	 */
	const Ogre::Quaternion& getRotation() const;

	/**
	 * @brief Sets the rotation of the model.
	 * @param rotation
	 */
	void setRotation(const Ogre::Quaternion rotation);

	/**
	 * @brief Gets a path to an icon resource, if defined.
	 * @return a path to an image which can be used as an icon for the model
	 */
	const std::string& getIconPath() const;

	/**
	 * @brief Creates and returns a new sub model definition for the supplied mesh name.
	 * @param meshname The name of the mesh to base the new sub model on. Must be a valid mesh.
	 * @return
	 */
	SubModelDefinition* createSubModelDefinition(const std::string& meshname);

	/**
	 * @brief Returns all SubModelDefinitions defined.
	 * @return The SubModelDefinitions store.
	 */
	const SubModelDefinitionsStore& getSubModelDefinitions() const;

	/**
	 * @brief Removes a certain SubModelDefinition.
	 * @param def The definition to remove.
	 */
	void removeSubModelDefinition(SubModelDefinition* def);

	/**
	 * @brief Creates and returns a new ActionDefintion with the given name.
	 * @param actionname The name of the new ActionDefinition.
	 * @return A pointer to the new ActionDefinition.
	 */
	ActionDefinition* createActionDefinition(const std::string& actionname);

	/**
	 * @brief Returns all ActionDefinitions defined.
	 * @return
	 */
	const ActionDefinitionsStore& getActionDefinitions() const;

	/**
	 * @brief Returns all ActionDefinitions defined.
	 * @return
	 */
	ActionDefinitionsStore& getActionDefinitions();

	/**
	 * @brief Removes a certain ActionDefinition.
	 * @param def The definition to remove.
	 */
	void removeActionDefinition(ActionDefinition* def);

	/**
	 * @brief Gets all attach point definitions.
	 * @return All attach point definitions.
	 */
	const AttachPointDefinitionStore& getAttachPointsDefinitions() const;

	/**
	 * @brief Adds a new attach point definition.
	 *
	 * If an definition with the same name already exists it will be updated. Else a new entry will be added.
	 *
	 * @param definition The new definition.
	 */
	void addAttachPointDefinition(const AttachPointDefinition& definition);

	/**
	 * @brief Creates and returns a new ViewDefinition with the supplied name.
	 * @param viewname The name of the view
	 * @return A pointer to the new view.
	 */
	ViewDefinition* createViewDefinition(const std::string& viewname);

	/**
	 * @brief Returns all views defined.
	 * @return
	 */
	const ViewDefinitionStore& getViewDefinitions() const;

	/**
	 * @brief Removed a named view. If no view can be found, no exception will be thrown.
	 * @param name The name of the view to to remove.
	 */
	void removeViewDefinition(const std::string& name);

	/**
	 * @brief Creates and returns a new BoneGroupDefinition with the supplied name.
	 * @param name The name of the definition.
	 * @return A pointer to the new definition.
	 */
	BoneGroupDefinition* createBoneGroupDefinition(const std::string& name);

	/**
	 * @brief Removed a named bone group. If no bone group can be found, no exception will be thrown.
	 * @param name The name of the bone group to to remove.
	 */
	void removeBoneGroupDefinition(const std::string& name);

	/**
	 * @brief Returns all bone groups.
	 * @return All bone groups.
	 */
	const BoneGroupDefinitionStore& getBoneGroupDefinitions() const;

	/**
	 * @brief Utility method for removing a definition from a non-associative stores (vector, list etc.)
	 * @param def The definition to remove.
	 * @param store The store to remove from.
	 */
	template<typename T, typename T1>
	static void removeDefinition(T* def, T1& store);

	/**
	 * @brief Reloads all the Model instances that uses this definition.
	 */
	void reloadAllInstances();

	/**
	 * @brief Gets a pointer to the rendering scheme definition, or null if none specified.
	 * @return
	 */
	const RenderingDefinition* getRenderingDefinition() const;

	const PoseDefinitionStore& getPoseDefinitions() const;

	void addPoseDefinition(const std::string& name, const PoseDefinition& definition);

	void removePoseDefinition(const std::string& name);

private:

	struct BindingDefinition
	{
		std::string EmitterVar;
		std::string AtlasAttribute;
	};

	typedef std::vector<BindingDefinition> BindingSet;

	/**
	 * @brief A definition of a particle system.
	 */
	struct ParticleSystemDefinition
	{
		/**
		 * @brief The script to use for the particle system.
		 *
		 * This must be defined in a separate Ogre .particle script.
		 */
		std::string Script;

		/**
		 * @brief An optional world direction to which the particle system should be locked.
		 *
		 * In most cases this would be up, since most particle systems would want to be emitted upwards (smoke, fire etc.).
		 * If no direction is defined, this will be invalid. In these cases, the particle system will use the local coords of the parent scene node. If the particle system is directional, this will look strange when the model is attached to another model (such as when wielded).
		 */
		Ogre::Vector3 Direction;

		/**
		 * @brief A set of bindings.
		 *
		 * A binding is a connection between an entity attribute and a parameter of the particle system.
		 * This allows for such effects as a fire growing larger as the status of the fire entity increases.
		 */
		BindingSet Bindings;
	};

	typedef std::vector<ParticleSystemDefinition> ParticleSystemSet;

	struct LightDefinition
	{
		Ogre::Light::LightTypes type;
		Ogre::ColourValue diffuseColour, specularColour;
		Ogre::Real range, constant, linear, quadratic;
		Ogre::Vector3 position;
	};

	typedef std::vector<LightDefinition> LightSet;

	/**
	 * @brief Adds a model instance to the internal store of instances. This method should be called from the class Model when a new Model is created.
	 * @param
	 */
	void addModelInstance(Model*);

	/**
	 * @brief Removed a model instance from the internal store of instances. This method should be called from the class Model when a new Model is removed.
	 * @param
	 */
	void removeModelInstance(Model*);

	/**
	 * @brief A store of all model instances of this definition.
	 * This can be used to update all instances at once.
	 */
	ModelInstanceStore mModelInstances;

	/**
	 * @brief The minimum distance at which the model will be shown.
	 */
	float mRenderingDistance;

	SubModelDefinitionsStore mSubModels;
	ActionDefinitionsStore mActions;
	ParticleSystemSet mParticleSystems;
	LightSet mLights;
	BoneGroupDefinitionStore mBoneGroups;
	PoseDefinitionStore mPoseDefinitions;

	AttachPointDefinitionStore mAttachPoints;

	UseScaleOf mUseScaleOf;
	Ogre::Real mScale;
	Ogre::Quaternion mRotation;

	/**
	 * @brief Defines how much contained entities should be offset. ZERO if not.
	 */
	Ogre::Vector3 mContentOffset;

	/**
	 * @brief Whether contained entities should be shown or not.
	 * Defaults to true.
	 */
	bool mShowContained;

	/**
	 * @brief How, if any, to transform the model from the base position.
	 * Defaults to a zeroed vector.
	 */
	Ogre::Vector3 mTranslate;

	bool mIsValid;

	ViewDefinitionStore mViews;

	/**
	 * @brief A path to an image resource which can be shown as an icon for the model.
	 */
	std::string mIconPath;

	RenderingDefinition* mRenderingDef;

};

typedef Ogre::SharedPtr<ModelDefinition> ModelDefinitionPtr;

///implementations

inline const Ogre::Vector3& ModelDefinition::getContentOffset() const
{
	return mContentOffset;
}
inline void ModelDefinition::setContentOffset(const Ogre::Vector3& offset)
{
	mContentOffset = offset;
}

inline void ModelDefinition::setValid(bool valid)
{
	mIsValid = valid;
}

inline size_t ModelDefinition::calculateSize(void) const
{
	//TODO:implement this
	return 0;
}

inline Ogre::Real ModelDefinition::getScale() const
{
	return mScale;
}
inline void ModelDefinition::setScale(Ogre::Real scale)
{
	mScale = scale;
}

inline ModelDefinition::UseScaleOf ModelDefinition::getUseScaleOf() const
{
	return mUseScaleOf;
}
inline void ModelDefinition::setUseScaleOf(const UseScaleOf useScale)
{
	mUseScaleOf = useScale;
}

inline float ModelDefinition::getRenderingDistance() const
{
	return mRenderingDistance;
}

inline void ModelDefinition::setRenderingDistance(float distance)
{
	mRenderingDistance = distance;
}

inline const std::string& ModelDefinition::getIconPath() const
{
	return mIconPath;
}

inline int AnimationDefinition::getIterations() const
{
	return mIterations;
}

inline Ogre::Real ActionDefinition::getAnimationSpeed() const
{
	return mAnimationSpeed;
}
inline void ActionDefinition::setAnimationSpeed(Ogre::Real speed)
{
	mAnimationSpeed = speed;
}

}
}
}
#endif
