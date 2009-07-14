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


#ifndef AVATAREMBERENTITY_H
#define AVATAREMBERENTITY_H

#include "EmberEntity.h"
#include "framework/ConsoleBackend.h"

namespace Eris
{
	class Entity;
	class Avatar;
}

namespace EmberOgre {

namespace Model {
	class Model;
}

class EmberPhysicalEntity;
class EmberEntity;
class Avatar;

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 *
 * @brief This is the main player avatar. We want this one to behave a little different than the other game entities, thus it has it's own subclass.
 *
 * One thing that makes this different from other entities is that we don't always respect position updates from the server as we would do with other entities. The main reason is that the user might have moved the avatar in the interim between sending updates to the server and recieving them, so we don't want the client to constantly relocating the avatar when it moves.
 * TODO: remove this class, so that any entity can be controlled
 */
class AvatarEmberEntity
: public EmberEntity,
public Ember::ConsoleObject
{
public:

	AvatarEmberEntity(const std::string& id, Eris::TypeInfo* type, Eris::View* vw, Ogre::SceneManager* sceneManager, Eris::Avatar* erisAvatar);
	virtual ~AvatarEmberEntity();

	/**
	 * used by the main application to set the EmberOgre::Avatar connected to this instance
	 */
	void setAvatar(Avatar* avatar);
	Avatar* getAvatar();

	/**
	 * returns the Ogre::SceneNode which represents the avatar
	 */
	Ogre::SceneNode* getAvatarSceneNode();

	/**
	 *    Returns the Eris Avatar instance.
	 * @return
	 */
	inline Eris::Avatar* getErisAvatar();

	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command
	 * @param args
	 */
	virtual	void runCommand(const std::string &command, const std::string &args);

	const Ember::ConsoleCommandWrapper SetAttachedOrientation;

protected:

	virtual void onChildAdded(Entity *e);
	virtual void onChildRemoved(Entity *e);

	/**Eris methods, see Eris::Entity.h for documentation */
//	virtual void handleTalk(const std::string &msg);
	virtual void onMoved();
	virtual void onImaginary(const Atlas::Objects::Root& act);
/*	virtual void addMember(Entity *e);
	virtual void rmvMember(Entity *e);*/
	//virtual void setVisible(bool vis);
	//virtual void setContainer(Eris::Entity *pr);
	//virtual void onLocationChanged(Eris::Entity *oldLocation, Eris::Entity *newLocation);
    virtual void init(const Atlas::Objects::Entity::RootEntity &ge, bool fromCreateOp);


	Avatar* mAvatar;
	Eris::Avatar* mErisAvatar;
};

///inline implementations
inline void AvatarEmberEntity::setAvatar(Avatar* avatar)
{
	mAvatar = avatar;
}
inline Avatar* AvatarEmberEntity::getAvatar()
{
	return mAvatar;
}
inline Eris::Avatar* AvatarEmberEntity::getErisAvatar()
{
	return mErisAvatar;
}

}

#endif // AVATAREMBERENTITY_H
