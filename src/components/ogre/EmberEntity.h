/*
    Copyright (C) 2004  Erik Hjortsberg

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

#ifndef DIMEENTITY_H
#define DIMEENTITY_H

#include <Atlas/Objects/Entity/GameEntity.h>

#include <Ogre.h>
#include <OgreException.h>
#include <Eris/Entity.h>
#include <Eris/World.h>
#include <Eris/PollDefault.h>
#include <Eris/Log.h>
#include <Eris/TypeInfo.h>

#include <wfmath/axisbox.h>
#include <wfmath/const.h>


#if SIGC_MAJOR_VERSION == 1 && SIGC_MINOR_VERSION == 0
#include <sigc++/signal_system.h>
#else
#include <sigc++/object.h>
#include <sigc++/signal.h>
#include <sigc++/slot.h>
#include <sigc++/bind.h>
#include <sigc++/object_slot.h>
#endif

#include "MathConverter.h"

namespace DimeOgre {
class DimeEntityFactory;

class DimeEntity : public Ogre::UserDefinedObject, public Eris::Entity {
	friend class DimeEntityFactory;
public:

/*eris 1.3
	DimeEntity(const Atlas::Objects::Entity::GameEntity &ge, Eris::TypeInfo* ty, Eris::View* vw, Ogre::Entity* ogreEntity); 
*/
	DimeEntity(const Atlas::Objects::Entity::GameEntity &ge, Eris::World* vw, Ogre::SceneManager* sceneManager);
//	, Ogre::Entity* ogreEntity); 
	virtual ~DimeEntity();
	
	/*
	 * return the scenenode to which this entity belongs
	 */
	Ogre::SceneNode* getSceneNode();
	
	/*
	 * return the Ogre::Entity of this object
	 */
	Ogre::Entity* getOgreEntity();
	
	/* from eris 1.2 */
	virtual void handleMove();
	virtual void handleTalk(const std::string &msg);
// not needed as we have handleMove() virtual void setPosition(const WFMath::Point<3>& pt);
	/// update the container of this entity (may be NULL)
	virtual void setContainer(Entity *pr);
	
//	virtual void setContents(const Atlas::Message::Element::ListType &contents);
	
	/// add a contained entity to this object (sets container)
	//virtual void addMember(Entity *e);
	
	/// remove an contained entity
	/** remove a contained entity; throws InvalidOperation if not found. Note
	that the container of e is <i>not<i/> reset */
	//virtual void rmvMember(Entity *e);
	
	/** called by World in response to Appearance/Disappearance messages : note that
	after a disappearance (vis = false), the server will not send any futher messages to the
	entity. At some point, invisible entities get flushed by Eris using an LRU scheme. */
	virtual void setVisible(bool vis);
	

protected: 

	virtual void scaleNode(Ogre::Real scalingFactor);
	virtual void createOgreEntity(Ogre::SceneManager* sceneManager);
	Ogre::Entity* mOgreEntity;
	Ogre::SceneNode* mOgreNode;
	Ogre::SceneManager* mSceneManager;
	
	/*
	 * this will hold the animation for the "Walk" movement
	 */
	Ogre::AnimationState* mAnimationState_Walk;
};

}

#endif // DIMEENTITY_H
