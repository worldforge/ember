/*
    Copyright (C) 2004  Erik Hjortsberg
    some parts Copyright (C) 2004 bad_camel at Ogre3d forums

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

#ifndef EMBEROGRE_EMBERPHYSICALENTITY_H
#define EMBEROGRE_EMBERPHYSICALENTITY_H

#include "EmberOgrePrerequisites.h"

#include "EmberEntity.h"
#include "services/sound/SoundAction.h"
#include "services/sound/SoundGroup.h"
#include "services/sound/SoundEntity.h"

namespace EmberOgre {

namespace Model {
	class ActionDefinition;
	class SoundDefinition;
	class Model;
	class Action;
	namespace Mapping {
		class ModelMapping;
	}
};



class EmberEntity;

typedef std::list<Model::Action*> ActionStore;
typedef std::vector<Model::ActionDefinition*> ActionDefinitionsStore;
typedef std::vector<Model::SoundDefinition*> SoundDefinitionsStore;
	
/**
 * Represents a Ember entity with a physical representation in the world.
 * This is presented by an instance of Model::Model, available through getModel().
 * The Model instance is attached to an instance of Ogre::SceneNode which can be accessed through getScaleNode(). This in turn is attached to the Ogre::SceneNode held by EmberEntity. The reason for this is that we want to be able to scale the Model without affecting child nodes. As an example, a character holding a sword might be scaled without the sword being scaled.
 * This means that child nodes will be children of the node held in EmberEntity, not children of the "scale node".
 *
 * When the node is created we use the Model::ModelMapping framework to determine what Model to show. This happens in the init(...) method. As data is updated the Model::ModelMapping framework might trigger certain parts of the Model to be hidden or shown, as well as a new model to be shown.
 *
 * This class also takes care of updating all actions that needs to be executed (for example animations and sounds) as the entity moves or performs actions.
 */
class EmberPhysicalEntity : public EmberEntity
{
friend class EmberEntityModelAction;
friend class EmberEntityPartAction;
friend class EmberEntityHideModelAction;
public:

	/**
	Keep static references to the names of the special actions connected to movements.
	*/
	static const char * const ACTION_STAND;
	static const char * const ACTION_RUN;
	static const char * const ACTION_WALK;
	static const char * const ACTION_SWIM;
	static const char * const ACTION_FLOAT;




	/**
	 *    Ctor.
	 * @param id 
	 * @param ty 
	 * @param vw 
	 * @param sceneManager 
	 */
	EmberPhysicalEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw, Ogre::SceneManager* sceneManager);
	
	/**
	 *    Dtor.
	 */
	virtual ~EmberPhysicalEntity();
	
	

	/**
	 * return the Model of this object
	 */
	inline Model::Model* getModel() const;

	/**
	 *    Returns the "scale node", which is the Ogre::SceneNode to which the Model instance is attached. This is separate from the Ogre::SceneNode in EmberEntity since we want to be able to scale the node without also scaling the attached nodes (such as any wielded entity).
	 * @return An Ogre::SceneNode, to which the Model::Model instance is attached.
	 */
	inline Ogre::SceneNode* getScaleNode() const;
	


	
	
	/**
	 *    Override the default implementation to also handle different rendering techniques (like "forest" rendering).
	 * @param visible 
	 */
// 	virtual void setClientVisible(bool visible);

	virtual void attachToPointOnModel(const std::string& point, Model::Model* model);
	virtual void detachFromModel();
	
	virtual void updateMotion(Ogre::Real timeSlice);
	
	/**
	 *    Updates the animation. This is normally called by MotionManager.
	 * @param timeSlice time to update with.
	 */
	void updateAnimation(Ogre::Real timeSlice);

	/**
	 *    Sets whether the ogre axis aligned bounding box should be shown or not.
	 * @param show whether to show the ogre bounding box
	 */
	virtual void showOgreBoundingBox(bool show);
// 	virtual void showErisBoundingBox(bool show);

	/**
	 * Gets whether the ogre axis aligned bounding box should be shown or not.
	 * @return true if the bounding box is shown
	 */
	virtual bool getShowOgreBoundingBox() const;
// 	virtual bool getShowErisBoundingBox();
	
	
	/**
	 *    Returns the entity that's attched to the specified point, if there is such 
	 * @param attachPoint 
	 * @return a pointer to the EmberEntity, or 0 if none found
	 */
	EmberEntity* getEntityAttachedToPoint(const std::string& attachPoint);

	virtual const Ogre::AxisAlignedBox& getWorldBoundingBox(bool derive = true) const;
	virtual const Ogre::Sphere & getWorldBoundingSphere (bool derive=true) const;


	/**
	 * Called by a contained member to see if the member is allowed to be shown.
	 * This can be reimplemented in a subclass such as AvatarEmberEntity to 
	 * disallow things that belongs to a characters inventory to be shown.
	 */
	virtual bool allowVisibilityOfMember(EmberEntity* entity);
	
	
	/**
	 * General method for turning on and off debug visualizations. Subclasses might support more types of visualizations than the ones defined here.
	 * @param visualization The type of visualization. Currently supports "OgreBBox" and "ErisBBox".
	 * @param visualize Whether to visualize or not.
	 */
	virtual void setVisualize(const std::string& visualization, bool visualize);
	
	
	/**
	 *    Gets whether a certain visualization is turned on or off.
	 * @param visualization The type of visualization. Currently supports "OgreBBox" and "ErisBBox".
	 * @return true if visualization is turned on, else false
	 */
	virtual bool getVisualize(const std::string& visualization) const;

protected: 

	/**
	 *    Tells the entity to use the model with the supplied name. This will trigger a creation of a Model::Model instance.
	 *    This method is normally called by an instance of EmberEntityModelAction.
	 * @param modelName The name of the model to use.
	 */
	void setModel(const std::string& modelName);

	/**
	 * Tells the entity to retrieve it sound actions from
	 * the model definition manager
	 */
	void setSounds();
	bool needSoundEntity();
	
	/**
	 *    Shows a certain part of the model.
	 *    This method is normally called by an instance of EmberEntityPartAction.
	 * @param partName The part to show.
	 */
	void showModelPart(const std::string& partName);
	
	/**
	 *    Hides a certain part of the model.
	 *    This method is normally called by an instance of EmberEntityPartAction.
	 * @param partName The part to hide.
	 */
	void hideModelPart(const std::string& partName);

	virtual const Ogre::Vector3& getOffsetForContainedNode(const Ogre::Vector3& position, EmberEntity* const entity);
	
	/**
	 *   creates EmberEntityUserObjects, connects them and sets up the collision detection system
	 * @return 
	 */
	void connectEntities();
	
	/**
	 *    Creates the model mapping for this entity. Call this once when initializing the entity.
	 */
	void createModelMapping();
	
	/**
	 * Creates the scale node to which the Model instance will be attached. Call this once when initializing the entity.
	 */
	void createScaleNode();
		
		
	/**
	 *    Called when the bounding box has changed.
	 */
	virtual void onBboxChanged();
	
	/**
	 *    Called when the movement mode has changed. We might want to update the animation of the entity, for example if it's a human.
	 * @param newMode 
	 */
	virtual void onModeChanged(MovementMode newMode);

	/**
	The current movement action of the entity, for example a walk action or a run action.
	*/
	Model::Action* mCurrentMovementAction;

	/**
	All the active actions, except the movement action (since it's stored in mCurrentMovementAction). 
	These actions will be updated each frame.
	NOTE: we currently don't allow for multiple actions playing at the same time
	*/
	//ActionStore mActiveActions;
	Model::Action* mActiveAction;

	/**
	If the entity is attached to another entity, this is the model to which it is attached to.
	This will be 0 if the entity isn't attached.
	*/
	Model::Model* mModelAttachedTo;
	
	/**
	We can't do attachments until the entity has been properly initialized, so sometimes we need to do delayed attachments. This will then hold the model to which this entity should be attached. Once the entity has been initialized we'll use mModelAttachedTo instead.
	*/
	Model::Model* mModelMarkedToAttachTo;
	
	/**
	We can't do attachments until the entity has been properly initialized, so sometimes we need to do delayed attachments. This will then hold the name of the attach point to which this entity should be attached. Once the entity has been initialized we'll use mModelAttachedTo instead.
	*/
	std::string mAttachPointMarkedToAttachTo;

	virtual void onChildAdded(Entity *e);
	virtual void onChildRemoved(Entity *e);

	

	/**
	 *    Detaches an entity which is already wielded.
	 * @param entityId 
	 */
	void detachEntity(const std::string & entityId);
	
	/**
	 *    Attaches an entity to a certain attach point
	 * @param attachPoint the name of the attachpoint to attach to
	 * @param entityId the id of the entity to attach to
	 */
	void attachEntity(const std::string & attachPoint, const std::string & entityId);
	
	/**
	Detaches all currently attached entities. Call this before the Model is resetted.
	*/
	void detachAllEntities();
	
	/**
	Attaches all entities that aren't currently attached.
	*/
	void attachAllEntities();
	
	
	/**
	 *    Process wield ops, which means wielding and unwielding entities. This methos will in turn call the appropriate attachEntity and detachEntity methods.
	 * @param wieldName the attachpoint to update
	 * @param idElement the id of the entity to wield
	 */
	void processWield(const std::string& wieldName, const Atlas::Message::Element& idElement);

	
	/**
	 *    Processes the outfit map and updates the appearance.
	 * @param outfitMap 
	 */
	void processOutfit(const Atlas::Message::MapType & outfitMap);

	/**
	 *    Overridden from Eris::Entity
	 * @param str 
	 * @param v 
	 */
	virtual void onAttrChanged(const std::string& str, const Atlas::Message::Element& v);
    /**
	 *    Overridden from Eris::Entity
     * @param act 
     */
    virtual void onAction(const Atlas::Objects::Operation::RootOperation& act);

	typedef std::map<std::string, std::string> AttachedEntitiesStore;
	/**
	A store of all attached entities, indexed by their id.
	*/
	AttachedEntitiesStore mAttachedEntities;	

//	virtual void onMoved();
	/**
	 *    Overridden from Eris::Entity
	 * @param moving 
	 */
	virtual void setMoving(bool moving);

	/**
	 *    Overridden from Eris::Entity
	 * @param ge 
	 */
    virtual void init(const Atlas::Objects::Entity::RootEntity &ge, bool fromCreateOp);


	/**
	The default size of the ogre bounding box, before any scaling is done.
	*/
	Ogre::AxisAlignedBox mDefaultOgreBoundingBox;


	/**
	 * Scales the Ogre::SceneNode to the size of the AxisBox defined by Eris::Entity
	 */
	virtual void scaleNode();
	
	//void setNodes();
	

	/**
	 * The model of the entity
	 */
	 Model::Model* mModel;

	/**
	 * The sound entity this entitiy is connected to.
	 */
	 Ember::SoundEntity* mSoundEntity;
	
	/**
	 * We need to scale the Ogre::Entity, because the underlying media is not
	 * always correctly scaled.
	 * But since each Eris::Entity can contain child entites, we'll get problems
	 * with scaling if we attach the children to a scaled node.
	 * Thus we use a separate, scaled node.
	 */
	Ogre::SceneNode* mScaleNode;
	
	
	/**
	 *    When the Model is reloaded we need to update with the new values.
	 */
	void Model_Reloaded();
	
	/**
	 *    When the Model is reset we need to clean up and remove all attachments from it.
	 */
	void Model_Resetting();
	
	
	/**
	 *    Initialize position and scaling of the scale node with values from the Model, as well as set up any alternative rendering techniques.
	 */
	void initFromModel();
	
	/**
	The model mapping used for this entity.
	*/
	Model::Mapping::ModelMapping* mModelMapping;
	
	
};

Ogre::SceneNode* EmberPhysicalEntity::getScaleNode() const
{
	return mScaleNode;
}	

Model::Model* EmberPhysicalEntity::getModel() const
{
	return mModel;	
}

}
#endif // EMBEROGRE_EMBERPHYSICALENTITY_H
