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


#ifndef AVATARDIMEENTITY_H
#define AVATARDIMEENTITY_H

#include <Eris/Entity.h>
#include <Eris/Avatar.h>

namespace DimeOgre {

class DimePhysicalEntity;
class DimeEntity;
class Avatar;
class Model;
class PersonDimeEntity;

/*
 * This is the main player avatar. We want this one to behave a little different
 * than the other game entities, thus it has it's own subclass.
 * 
 */
class AvatarDimeEntity: public PersonDimeEntity {
public:

	AvatarDimeEntity(const Atlas::Objects::Entity::GameEntity &ge, Eris::World* vw, Ogre::SceneManager* sceneManager, Ogre::SceneNode* nodeWithModel, Eris::Avatar* erisAvatar);
	virtual ~AvatarDimeEntity();
	
	
	
	/*Eris methods, see Eris::Entity.h for documentation */
	virtual void handleTalk(const std::string &msg);
	virtual void handleMove();
	virtual void addMember(Entity *e);
	virtual void rmvMember(Entity *e);
	//virtual void setVisible(bool vis);	
	virtual void setContainer(Eris::Entity *pr);
	
	/* 
	 * used by the main application to set the DimeOgre::Avatar connected to this instance 
	 */
	void setAvatar(Avatar* avatar) {mAvatar = avatar;}
	Avatar* getAvatar() {return mAvatar;}
	
	/*
	 * returns the Ogre::SceneNode which represents the avatar
	 */
	Ogre::SceneNode* getAvatarSceneNode();
	
	inline Eris::Avatar* getErisAvatar() { return mErisAvatar;	}

/*	SigC::Signal1<void, DimeEntity* > EventAddedEntityToInventory;
	SigC::Signal1<void, DimeEntity* > EventRemovedEntityFromInventory;*/
	SigC::Signal1<void, Eris::Entity* > EventAddedEntityToInventory;
	SigC::Signal1<void, Eris::Entity* > EventRemovedEntityFromInventory;


	
protected: 

	Avatar* mAvatar;
	Eris::Avatar* mErisAvatar;
	
};

}

#endif // AVATARDIMEENTITY_H
