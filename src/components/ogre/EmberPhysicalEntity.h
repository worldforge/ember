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

#ifndef DIMEPHYSICALENTITY_H
#define DIMEPHYSICALENTITY_H

#include "EmberOgrePrerequisites.h"

#include "EmberEntity.h"


namespace EmberOgre {

namespace Model {
	class Model;
	class Action;
};

class EmberEntity;

typedef std::list<Model::Action*> ActionStore;
	
/**
 * Represents a Ember entity with a physical representation in the world.
 * This is represented by a an Ogre::Entity.
 */
class EmberPhysicalEntity : public EmberEntity
{
public:

	static const char * const ACTION_STAND;
	static const char * const ACTION_RUN;
	static const char * const ACTION_WALK;
	static const char * const ACTION_SWIM;
	static const char * const ACTION_FLOAT;




	EmberPhysicalEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw, Ogre::SceneManager* sceneManager, Ogre::SceneNode* nodeWithModel);
	virtual ~EmberPhysicalEntity();
	
	

	/**
	 * return the Model of this object
	 */
	Model::Model* getModel() const;

	inline Ogre::SceneNode* getScaleNode() const;
	


	
	
	virtual void setVisible(bool visible);

	virtual void attachToPointOnModel(const std::string& point, Model::Model* model);
	virtual void detachFromModel();
	
	virtual void updateMotion(Ogre::Real timeSlice);
	
	void updateAnimation(Ogre::Real timeSlice);

	virtual void showOgreBoundingBox(bool show);
// 	virtual void showErisBoundingBox(bool show);

	virtual bool getShowOgreBoundingBox();
// 	virtual bool getShowErisBoundingBox();
	
	
	/**
	 *    Returns the entity that's attched to the specified point, if there is such 
	 * @param attachPoint 
	 * @return a pointer to the EmberEntity, or 0 if none found
	 */
	EmberEntity* getEntityAttachedToPoint(const std::string& attachPoint);

	virtual const Ogre::AxisAlignedBox& getWorldBoundingBox(bool derive = true) const;


	/**
	 * Called by a contained member to see if the member is allowed to be shown.
	 * This can be reimplemented in a subclass such as AvatarEmberEntity to 
	 * disallow things that belongs to a characters inventory to be shown.
	 */
	virtual bool allowVisibilityOfMember(EmberEntity* entity);

protected: 

	virtual const Ogre::Vector3& getOffsetForContainedNode(const Ogre::Vector3& position, EmberEntity* const entity);
	
	/**
	 *   creates EmberEntityUserObjects, connects them and sets up the collision detection system
	 * @return 
	 */
	void connectEntities();
	
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
	
	Model::Model* mModelMarkedToAttachTo;
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
	 * We need to scale the Ogre::Entity, because the underlying media is not
	 * always correctly scaled.
	 * But since each Eris::Entity can contain child entites, we'll get problems
	 * with scaling if we attach the children to a scaled node.
	 * Thus we use a separate, scaled node.
	 */
	Ogre::SceneNode* mScaleNode;
	
	void Model_Reloaded();
	
	void Model_Resetting();
	
	void initFromModel();
	
};

Ogre::SceneNode* EmberPhysicalEntity::getScaleNode() const
{
	return mScaleNode;
}	


}
#endif // DIMEPHYSICALENTITY_H
