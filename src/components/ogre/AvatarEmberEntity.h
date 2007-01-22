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
 * This is the main player avatar. We want this one to behave a little different
 * than the other game entities, thus it has it's own subclass.
 * 
 */
class AvatarEmberEntity: public EmberPhysicalEntity {
public:

	AvatarEmberEntity(const std::string& id, Eris::TypeInfo* type, Eris::View* vw, Ogre::SceneManager* sceneManager, Eris::Avatar* erisAvatar);
	virtual ~AvatarEmberEntity();
	
	
	
	
	/**
	 * used by the main application to set the EmberOgre::Avatar connected to this instance 
	 */
	inline void setAvatar(Avatar* avatar);
	inline Avatar* getAvatar();
	
	/**
	 * returns the Ogre::SceneNode which represents the avatar
	 */
	Ogre::SceneNode* getAvatarSceneNode();
	
	/**
	 *    Returns the Eris Avatar instance.
	 * @return 
	 */
	inline Eris::Avatar* getErisAvatar();



	
protected: 

	virtual void onChildAdded(Entity *e);
	virtual void onChildRemoved(Entity *e);

	/*Eris methods, see Eris::Entity.h for documentation */
//	virtual void handleTalk(const std::string &msg);
	virtual void onMoved();
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
	void AvatarEmberEntity::setAvatar(Avatar* avatar) 
	{
		mAvatar = avatar;
	}
	Avatar* AvatarEmberEntity::getAvatar() 
	{
		return mAvatar;
	}
	Eris::Avatar* AvatarEmberEntity::getErisAvatar() 
	{
		return mErisAvatar;	
	}

}

#endif // AVATARDIMEENTITY_H
