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

#include <Atlas/Objects/Entity.h>

#include <Eris/Entity.h>
#include <Eris/View.h>
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

#include <Ogre.h>
#include <OgreException.h>

#include "MathConverter.h"

namespace DimeOgre {
class DimeEntityFactory;

/*
 * A representation of an Eris::Entity, ie. a world entity.
 * Note that most entities in the game world will be of type DimePhysicalEntity
 * as they will have some sort of physical representation.
 * For things such as boundaries and weather, this is a nice class.
 */
class DimeEntity : public Ogre::UserDefinedObject, public Eris::Entity {
	friend class DimeEntityFactory;
public:

	enum ClickMasks
	{
		CM_AVATAR = 1<<0,
		CM_PERSONS = 1<<1,
		CM_CREATURES = 1<<2,
		CM_NATURE = 1<<3,
		CM_UNDEFINED = 1<<4,
		CM_TERRAIN = 1<<5
	};

	DimeEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw,Ogre::SceneManager* sceneManager);
	virtual ~DimeEntity();
	

	/**
	 * Called by contained entites to determine how they snap to the ground.
	 * For instance a house entitiy containing a player entity.
	 * This should of course be extended to a more dynamic physics simulation
	 * in the future
	 */
	virtual void adjustHeightPositionForContainedNode(DimeEntity* const entity);

	/*
	 * return the scenenode to which this entity belongs
	 */
	Ogre::SceneNode* getSceneNode();

	/*
	 * Called by a contained member to see if the member is allowed to be shown.
	 * This can be reimplemented in a subclass such as AvatarDimeEntity to 
	 * disallow things that belongs to a characters inventory to be shown.
	 */
	virtual bool allowVisibilityOfMember(DimeEntity* entity);



protected: 

	/*
	 * Overridden from Eris::Entity
	 * @see Eris::Entity
	 */
	virtual void onMoved();
	virtual void onTalk(const Atlas::Objects::Root& obj);
//	virtual void setContainer(Entity *pr);
	virtual void onVisibilityChanged(bool vis);
	virtual void onLocationChanged(Eris::Entity *newLocation, Eris::Entity *oldLocation);
	
	/* 
	 * Creates the main scene node which holds the entity.
	 */
	void DimeEntity::createSceneNode();
	
	virtual void init(const Atlas::Objects::Entity::GameEntity &ge);



	/*
	 * The main SceneNode which holds the entity in the ogre world space.
	 */
	Ogre::SceneNode* mOgreNode;
	
	Ogre::SceneManager* mSceneManager;
	
	Eris::View* mView;

};

}

#endif // DIMEENTITY_H
