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

#include <Atlas/Objects/Entity.h>

#include "EmberOgrePrerequisites.h"
#include <OgreException.h>
#include <Eris/Entity.h>
#include <Eris/View.h>
#include <Eris/PollDefault.h>
#include <Eris/Log.h>
#include <Eris/TypeInfo.h>

namespace EmberOgre {

class EmberEntity;
class Model;
	
/*
 * Represents a Ember entity with a physical representation in the world.
 * This is represented by a an Ogre::Entity.
 */
class EmberPhysicalEntity : public EmberEntity{
public:

/*eris 1.3
	EmberEntity(const Atlas::Objects::Entity::GameEntity &ge, Eris::TypeInfo* ty, Eris::View* vw, Ogre::Entity* ogreEntity); 
*/
	EmberPhysicalEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw, Ogre::SceneManager* sceneManager, Ogre::SceneNode* nodeWithModel);
//	, Ogre::Entity* ogreEntity); 
	virtual ~EmberPhysicalEntity();
	
	
	/*
	 * return the Ogre::Entity of this object
	 */
//	Ogre::MovableObject* getOgreEntity();

	/*
	 * return the Model of this object
	 */
	inline Model* EmberPhysicalEntity::getModel() const
	{
		return mModel;	
	}

	inline Ogre::SceneNode* EmberPhysicalEntity::getScaleNode() const
	{
		//return EmberOgre::getSingleton().getSceneManager()->getSceneNode(getId() + "_scaleNode");
	
		return mScaleNode;
	}	
	


	/**
	 * Adjust the height of the entity so that it "snaps" to the ground.
	 * This is most often done by making a call to the containing node's
	 * adjustHeightPositionForContainedNode method.
	 */
	virtual void adjustHeightPosition(); 
	
	

protected: 


	/* from eris 1.2 */
	virtual void onMoved();
//	virtual void handleTalk(const std::string &msg);
// not needed as we have handleMove() virtual void setPosition(const WFMath::Point<3>& pt);
	/// update the container of this entity (may be NULL)
//	virtual void setContainer(Entity *pr);
	
//	virtual void setContents(const Atlas::Message::Element::ListType &contents);
	
	/// add a contained entity to this object (sets container)
	//virtual void addMember(Entity *e);
	
	/// remove an contained entity
	/** remove a contained entity; throws InvalidOperation if not found. Note
	that the container of e is <i>not<i/> reset */
	//virtual void rmvMember(Entity *e);
	
	/** called by View in response to Appearance/Disappearance messages : note that
	after a disappearance (vis = false), the server will not send any futher messages to the
	entity. At some point, invisible entities get flushed by Eris using an LRU scheme. */
	//virtual void setVisible(bool vis);

	//virtual void onVisibilityChanged( bool vis);

	virtual void init(const Atlas::Objects::Entity::GameEntity &ge);


	/*
	 * Checks the model for animations and assign registers them with MotionManager
	 */
	void loadAnimationsFromModel();


	/*
	 * Scales the Ogre::SceneNode to the size of the AxisBox defined by Eris::Entity
	 */
	virtual void scaleNode();
	
	void setNodes();
	
	/*
	 * The main Ogre::Entity
	 */
//	Ogre::MovableObject* mOgreEntity;
	
	/*
	 * The model of the entity
	 */
	 Model* mModel;
	
	/*
	 * We need to scale the Ogre::Entity, because the underlying media is not
	 * always correctly scaled.
	 * But since each Eris::Entity can contain child entites, we'll get problems
	 * with scaling if we attach the children to a scaled node.
	 * Thus we use a separate, scaled node.
	 */
	Ogre::SceneNode* mScaleNode;
	
	/*
	 * this will hold the animation for the "Walk" movement
	 */
	Ogre::AnimationState* mAnimationState_Walk;
};

}
#endif // DIMEPHYSICALENTITY_H
