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

/*
 * This is the main player avatar. We want this one to behave a little different
 * than the other game entities, thus it has it's own subclass.
 * 
 */

#ifndef AVATARDIMEENTITY_H
#define AVATARDIMEENTITY_H

#include <Eris/Entity.h>

namespace DimeOgre {
	
class DimeEntity;
class Avatar;

class AvatarDimeEntity: public DimeEntity {
public:

	AvatarDimeEntity(const Atlas::Objects::Entity::GameEntity &ge, Eris::World* vw, Ogre::SceneManager* sceneManager);
	virtual ~AvatarDimeEntity();
	
	
	
	/*Eris methods, see Eris::Entity.h for documentation */
	virtual void handleTalk(const std::string &msg);
	virtual void handleMove();
	//virtual void addMember(Entity *e);
	//virtual void rmvMember(Entity *e);
	virtual void setVisible(bool vis);	
	virtual void setContainer(Entity *pr);
	
	/* 
	 * used by the main application to set the DimeOgre::Avatar connected to this instance 
	 */
	void setAvatar(Avatar* avatar) {mAvatar = avatar;}
	Avatar* getAvatar() {return mAvatar;}

	
protected: 

	virtual void createOgreEntity(Ogre::SceneManager* sceneManager);
	Avatar* mAvatar;
	
};

}

#endif // AVATARDIMEENTITY_H
