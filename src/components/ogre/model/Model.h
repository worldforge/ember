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
#include "ModelDefinition.h"
#include "ParticleSystem.h"
#include <sigc++/signal.h>

namespace EmberOgre {
namespace Model {

class SubModel;
class SubModelPart;
class ModelDefinition;
class ParticleSystemBinding;
class ParticleSystem;
class Action;

struct LightInfo
{
	Ogre::Light* light;
	Ogre::Vector3 position;
};

typedef std::vector<ParticleSystem*> ParticleSystemSet;
typedef std::vector<ParticleSystemBinding> ParticleSystemBindingsSet;
typedef std::vector<LightInfo> LightSet;
/**
@brief A part of a model, which can be turned on and off.

Each model is separated into one or many "parts" which consists of one or many sub entities using specific materials. By showing or hiding certain parts you can simulate concepts such as different clothes or attached objects. A part can belong to a "group", which is just an arbitrary name. Parts belonging to the same group are mutually exlusive and only one part can be shown at once. When a part which belongs to a certain group is shown, all other parts in the same group are automatically hidden.
A typical example of usage of this would be a human model where there's ten different parts for different shirt combinations. Each shirt part points towards the same mesh, but with different materials. They all belong to the same group, "shirt". Whenever one shirt is shown, all of the others are automatically hidden.
@author Erik Hjortsberg <erik@worldforge.org>
*/
class ModelPart
{
public:
	typedef std::vector<SubModelPart*> SubModelPartStore;
	/**
	 * @brief Default ctor.
	 */
	ModelPart();
	
	/**
	 * @brief Adds a sub model part to this model part.
	 * @param part The sub model part to add.
	 */
	void addSubModelPart(SubModelPart* part);
	
	/**
	 * @brief Hides this part.
	 */
	void hide();
	
	/**
	 * @brief Shows this part.
	 */
	void show();
	
	/**
	 * @brief Gets whether the part is visible or not.
	 * @return True if the part is visible.
	 */
	bool getVisible() const;
	
	/**
	 * @brief Sets whether the part is visible or not.
	 * @param visible True if the part should be visible.
	 */
	void setVisible(bool visible);
	
	/**
	 * @brief Gets the name of the group which this part belongs to.
	 * @return The name of the group which this part belongs to.
	 */
	const std::string& getGroupName() const;
	
	/**
	 * @brief Sets the name of the group to which this part belongs to.
	 * @param groupName The name of the group which this part should belong to.
	 */
	void setGroupName(const std::string& groupName);
	
protected:
	/**
	 * @brief Whether the part is shown or not.
	 */
	bool mVisible;
	
	/**
	 * @brief A store of the sub model parts belonging to this part.
	 */
	SubModelPartStore mSubModelParts;
	
	/**
	 * @brief The name of the group to which this part belongs.
	 */
	std::string mGroupName;
};

/**
 * @brief An instance of this represents a complete model, comprised of both multiple meshes, particle systems and lights.
 * A model can be made out of different entities, just as long as they share a skeleton.
 * The model consists of different parts, represented by instances of SubModelPart. 
 * Typical parts would be a body or a shirt. These parts can be turned on or off.
 * That allows for the use of a single mesh with many different submeshes.
 * 
 * A model is typically instanciated from a modeldef.xml file through the use
 * of createFromXML(...)
 */
class Model : public Ogre::MovableObject
{

friend class ModelDefinition;
friend class ModelFactory;

public:
	
	
	typedef std::map<std::string, Action> ActionStore;

	typedef std::set<SubModel*> SubModelSet;
	typedef std::set<std::string> StringSet;
	typedef std::map<std::string, StringSet > SubModelPartMapping;
	typedef std::map<std::string, ModelPart> ModelPartStore;

	typedef std::map<std::string, std::vector<std::string > > PartGroupStore;
	
	struct AttachPointWrapper
	{
		Ogre::TagPoint* TagPoint;
		std::string AttachPointName;
		Ogre::MovableObject *Movable;
	};
	typedef std::vector<AttachPointWrapper> AttachPointWrapperStore;
	
	static const Ogre::String sMovableType;
	
	/**
	 *    Creates a new Model instance.
	 *    The name of the model will be autogenerated.
	 *    Remember to call create(...) to create the actual meshes.
	 * @return 
	 */
// 	Model();
	
	virtual ~Model();
	
	

	/** Notify the object of it's manager (internal use only) */
	virtual void _notifyManager(Ogre::SceneManager* man);

	static Model* createModel(Ogre::SceneManager* sceneManager, const std::string& modelType, const std::string& name = "");
	
	/**
	 *    Reloads the model from the modeldefinition.
	 */
	void reload();
	
	
	/**
	* Emitted when the model is reloaded
	*/
	sigc::signal<void> Reloaded;
	
	/**
	* Emitted when the model is about to be resetted.
	*/
	sigc::signal<void> Resetting;
	

	bool addSubmodel(SubModel* submodel);
 	bool removeSubmodel(SubModel* submodel);
 	
	Action* getAction(const std::string& name);	
	

	ModelPart& getPart(const std::string& partName);

	/**
	 * hides and shows a certain part of the model
	 */
	void showPart(const std::string& partName, bool hideOtherParts = true);
	void hidePart(const std::string& partName, bool dontChangeVisibility = false);
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
	
	inline const SubModelSet& getSubmodels() const;
	
	//SubModel* getSubModel(const std::string& name);
	SubModel* getSubModel(size_t index);
	

	Ogre::AnimationState* getAnimationState(const Ogre::String& name);
	Ogre::AnimationStateSet* getAllAnimationStates();
	Ogre::SkeletonInstance * getSkeleton ();
	Ogre::TagPoint* attachObjectToBone (const Ogre::String &boneName, Ogre::MovableObject *pMovable, const Ogre::Quaternion &offsetOrientation=Ogre::Quaternion::IDENTITY, const Ogre::Vector3 &offsetPosition=Ogre::Vector3::ZERO);
	Ogre::MovableObject * detachObjectFromBone (const Ogre::String &movableName);
	void detachAllObjectsFromBone(void);
	
	/** @see Ogre::MovableObject::setRenderingDistance(Ogre::Real dist)
	*/
	virtual void setRenderingDistance (Ogre::Real dist);
	//inline bool isAnimated() { return mAnimationPartMap.size(); }

        /** Overridden - see MovableObject.
        */
// 		inline virtual Ogre::Real getRenderingDistance   (   void    )    const;

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
		virtual const Ogre::AxisAlignedBox& getWorldBoundingBox(bool derive) const;
		
		virtual Ogre::Real getBoundingRadius() const;

        /** Overridden - see MovableObject.
        */
        virtual void _updateRenderQueue(Ogre::RenderQueue* queue);

        /** Overridden from MovableObject */
//         virtual const Ogre::String& getName(void) const;

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
		virtual bool   isVisible (void) const;
		
		/**
		 * @copydoc Ogre::MovableObject::visitRenderables()
		 */
		virtual void visitRenderables(Ogre::Renderable::Visitor* visitor, bool debugRenderables);

		
	/**
	 *    returns a pointer to the defintion of the Model
	 * @return 
	 */
	inline ModelDefnPtr getDefinition() const;
	
	
	Ogre::TagPoint* attachObjectToBone(const Ogre::String &boneName, Ogre::MovableObject *pMovable, const Ogre::Quaternion &offsetOrientation=Ogre::Quaternion::IDENTITY, const Ogre::Vector3 &offsetPosition=Ogre::Vector3::ZERO, const Ogre::Vector3 &scale = Ogre::Vector3::UNIT_SCALE);
	Ogre::TagPoint* attachObjectToAttachPoint(const Ogre::String &boneName, Ogre::MovableObject *pMovable,  const Ogre::Vector3 &scale = Ogre::Vector3::UNIT_SCALE, const Ogre::Quaternion &offsetOrientation=Ogre::Quaternion::IDENTITY, const Ogre::Vector3 &offsetPosition=Ogre::Vector3::ZERO);
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
	 *    Clears all the particles
	 */
	void resetParticles();
	
	/**
	 *    Resets the whole model.
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
	 the name of the model
	*/
// 	std::string mName;
	
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
	//void enableAnimation(const std::string& nameOfAnimation,bool enable);
};

// Ogre::Real Model::getRenderingDistance   (   void    )    const
// {
// 	return _masterModel->getRenderingDistance();
// }

ModelDefnPtr Model::getDefinition() const { return mMasterModel; }

// // const Model::SubModelPartStoreMap& Model::getSubmodelParts() const
// // {
// // 	return mSubModelPartMap;
// // }

const Model::SubModelSet& Model::getSubmodels() const
{
	return mSubmodels;
}







	/** Factory object for creating Model instances */
	class  ModelFactory : public Ogre::MovableObjectFactory
	{
	protected:
		Ogre::MovableObject* createInstanceImpl( const Ogre::String& name, const Ogre::NameValuePairList* params);
	public:
		ModelFactory() {}
		~ModelFactory() {}

		static Ogre::String FACTORY_TYPE_NAME;

		const Ogre::String& getType(void) const;
		void destroyInstance( Ogre::MovableObject* obj);  

	};

}
}
#endif // MODEL_H
