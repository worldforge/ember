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
#include <OgreMovableObject.h>
#include "ModelDefinition.h"

namespace EmberOgre {

class SubModel;
class SubModelPart;
class ModelDefinition;
/**
 * This is the standard model used in Ember.
 * A model can be made out of different entities, just as long as they share a skeleton.
 * The model consists of different parts, represented by instances of SubModelPart. 
 * Typical parts would be a body or a shirt. These parts can be turned on or off.
 * That allows for the use of a single mesh with many different submeshes.
 * 
 * A model is typically instanciated from a modeldef.xml file through the use
 * of createFromXML(...)
 */

struct AnimationPart
{
	Ogre::AnimationState* state;
	Ogre::Real weight;
};


typedef std::vector<AnimationPart> AnimationPartSet;


class AnimationSet
{
public:
	void addTime(Ogre::Real timeSlice);
	void addAnimationPart(AnimationPart part);
	void setEnabled(bool state);
	/**
	 *    returns true if all animation parts have been played 
	 * @return 
	 */
	bool hasCompleted() const;
	
protected:
	AnimationPartSet mAnimations;
};



class Action
{
public:
	inline AnimationSet* getAnimations() { return &mAnimations; }
	inline void setName(const std::string& name) { mName = name; }
	inline const std::string& getName() { return mName; }
	

protected:
	std::string mName;
	AnimationSet mAnimations;
};

class ParticleSystem;
class ParticleSystemBinding
{
public:
	ParticleSystemBinding(ParticleSystem* parentSystem, const std::string& emitterVal, const std::string& variableName);
	void scaleValue(Ogre::Real scaler);
private:
	std::string mEmitterVal;
	std::string mVariableName;
	ParticleSystem* mParticleSystem;
	Ogre::Real mOriginalValue;
public:
	inline const std::string& getEmitterVal() const {return mEmitterVal;}
	inline const std::string& getVariableName() const {return mVariableName;}

};
typedef std::vector<ParticleSystemBinding> ParticleSystemBindingsSet;
typedef std::vector<ParticleSystemBinding*> ParticleSystemBindingsPtrSet;

class ParticleSystem
{
public:
	ParticleSystem(Ogre::ParticleSystem* ogreParticleSystem);
	ParticleSystem::~ParticleSystem();
	Ogre::ParticleSystem* getOgreParticleSystem();
	
	/**
	 *        Adds a new binding.
	 * @param emitterVal the name of the particle emitter
	 * @param variableName the name of the atlas attribute
	 * @return a pointer to the newly created binding
	 */
	ParticleSystemBinding* addBinding(const std::string& emitterVal, const std::string& variableName);
	ParticleSystemBindingsPtrSet& getBindings();
protected:
	Ogre::ParticleSystem* mOgreParticleSystem;
	ParticleSystemBindingsPtrSet mBindings;

};
typedef std::vector<ParticleSystem*> ParticleSystemSet;
 
 
class Model : public Ogre::MovableObject
{

friend class ModelDefinition;



public:



	
	
	
	typedef std::map<std::string, Action> ActionStore;

	typedef std::set<SubModel*> SubModelSet;
	typedef std::set<std::string> StringSet;
	typedef std::map<std::string, StringSet > SubModelPartMapping;
	typedef std::map<std::string, SubModelPart*> SubModelPartMap;

	Model(const std::string& name);
	bool create(const std::string& modelType); // create model of specific type

	virtual ~Model();

	bool addSubmodel(SubModel* submodel);
 	bool removeSubmodel(SubModel* submodel);
 	
	Action* getAction(const std::string& name);	
	
	ParticleSystemBindingsSet& getAllBindings();

	/**
	 * hides and shows a certain part of the model
	 */
	void showPart(const std::string& partName);
	void hidePart(const std::string& partName);
	void setVisible(bool visible);
	
	/**
	 * if defined in the modeldef, returns a scaler by which the node containing 
	 * the model can be scaled
	 */
	const Ogre::Real getScale() const;

	/**
	 * if defined in the modeldef, returns an amount of degrees by which the node containing 
	 * the model can be rotated
	 */
	const Ogre::Real getRotation() const;
	
	/**
	 * if defined in the modeldef, returns an axis by which the model can be scaled
	 * I.e. when dealing with something such as a fir tree, you want to use the
	 * height of the tree to determine how much it should be scaled, since the 
	 * bounding box supplied by eris doesn't take the branches into account
	 */
	const ModelDefinition::UseScaleOf getUseScaleOf() const;
	
	
	inline const SubModelPartMap& getSubmodelParts() const
	{
		return mSubModelPartMap;
	}
	
	inline const SubModelSet& getSubmodels() const
	{
		return mSubmodels;
	}
	
	

	Ogre::AnimationState* getAnimationState(const Ogre::String& name);
	Ogre::AnimationStateSet* getAllAnimationStates();
	Ogre::SkeletonInstance * getSkeleton ();
	void attachObjectToBone (const Ogre::String &boneName, Ogre::MovableObject *pMovable, const Ogre::Quaternion &offsetOrientation=Ogre::Quaternion::IDENTITY, const Ogre::Vector3 &offsetPosition=Ogre::Vector3::ZERO);
	Ogre::MovableObject * detachObjectFromBone (const Ogre::String &movableName);
	void detachAllObjectsFromBone(void);
	
	//inline bool isAnimated() { return mAnimationPartMap.size(); }


        /** Overridden - see MovableObject.
        */
        virtual void _notifyCurrentCamera(Ogre::Camera* cam);

        /** Overridden - see MovableObject.
        */
		virtual void setUserObject (Ogre::UserDefinedObject *obj);

        /// Overridden - see MovableObject.
        virtual void setRenderQueueGroup(Ogre::RenderQueueGroupID queueID);
        
        /** Overridden - see MovableObject.
        */
        virtual const Ogre::AxisAlignedBox& getBoundingBox(void) const;

        /** Overridden - see MovableObject.
        */
		virtual const Ogre::AxisAlignedBox& Model::getWorldBoundingBox(bool derive) const;
		
		virtual Ogre::Real getBoundingRadius() const;

        /** Overridden - see MovableObject.
        */
        virtual void _updateRenderQueue(Ogre::RenderQueue* queue);

        /** Overridden from MovableObject */
        virtual const Ogre::String& getName(void) const;

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
		
		
	/**
	 *    returns a pointer to the defintion of the Model
	 * @return 
	 */
	inline ModelDefnPtr getDefinition() const { return _masterModel; }
	
	void attachObjectToBone(const Ogre::String &boneName, Ogre::MovableObject *pMovable, const Ogre::Quaternion &offsetOrientation=Ogre::Quaternion::IDENTITY, const Ogre::Vector3 &offsetPosition=Ogre::Vector3::ZERO, const Ogre::Vector3 &scale = Ogre::Vector3::UNIT_SCALE);
	void attachObjectToAttachPoint(const Ogre::String &boneName, Ogre::MovableObject *pMovable,  const Ogre::Vector3 &scale = Ogre::Vector3::UNIT_SCALE, const Ogre::Quaternion &offsetOrientation=Ogre::Quaternion::IDENTITY, const Ogre::Vector3 &offsetPosition=Ogre::Vector3::ZERO);
	//void attachObjectToAttachPoint(const Ogre::String &attachPointName, Ogre::MovableObject *pMovable, const Ogre::Quaternion &offsetOrientation=Ogre::Quaternion::IDENTITY, const Ogre::Vector3 &offsetPosition=Ogre::Vector3::ZERO);
	
	bool hasAttachPoint(const std::string& attachPoint) const; 

	const ParticleSystemBindingsPtrSet& getAllParticleSystemBindings() const;

	ParticleSystemSet& getParticleSystems();
	
	bool hasParticles() const;
	
protected:

	ParticleSystemBindingsPtrSet mAllParticleSystemBindings;
	ParticleSystemSet mParticleSystems;
	/**
	 *    Clears all the submodels
	 */
	void Model::resetSubmodels();
	
	
	
	/**
	 *    Clears all the particles
	 */
	void Model::resetParticles();
	
	/**
	 *    Resets the whole model.
	 */
	void Model::reset();
	
	
	Ogre::Entity::ChildObjectList mChildObjectList;
	
	void attachObjectImpl(Ogre::MovableObject *pObject, Ogre::TagPoint *pAttachingPoint);
	void detachObjectImpl(MovableObject* pObject);
	void detachAllObjectsImpl(void);
	
	mutable Ogre::AxisAlignedBox mFull_aa_box;
	mutable Ogre::AxisAlignedBox mWorldFull_aa_box;

// 	//set of all animations currently running
// 	std::set< std::string > mRunningAnimations;
// 	//set of all animation currently paused
// 	std::set< std::string > mPausedAnimations;

	static Ogre::String msMovableType;
	
	
	/**
	all actions belonging to the model
	*/
	ActionStore mActions;
	
	
	/**
	 the name of the model
	*/
	std::string mName;
	
	/**
	modeldef this was copied from
	*/
	ModelDefnPtr _masterModel; 
	
	/**
	a set of all submodels belonging to the model
	*/
	SubModelSet mSubmodels;
	/**
	a set of all submodelparts belonging to the model (in reality they belong to the submodels though)
	*/
	SubModelPartMap mSubModelPartMap;
	
	Ogre::SkeletonInstance* mSkeletonInstance;
	
	/**
	how much to scale the model from it's initial size
	*/
	Ogre::Real mScale;
	/**
	how much the model should be rotated around the Y-axis from it's initial position
	*/
	Ogre::Real mRotation;
	
	
	/**
	set of all animation states
	*/
	Ogre::AnimationStateSet* mAnimationStateSet;

	
	bool createFromDefn();
	//void enableAnimation(const std::string& nameOfAnimation,bool enable);
};

}
#endif // MODEL_H
