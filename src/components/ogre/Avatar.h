/*
	Avatar.h by Miguel Guzman (Aglanor)
	Copyright (C) 2002 Miguel Guzman & The Worldforge Project

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

// TODO: use forward declarations here

#ifndef AVATAR_H
#define AVATAR_H

#include <Ogre.h> // TODO: forward declarations instead

class Avatar
{

    private:

    public:

	Avatar();
	Avatar(Ogre::SceneManager* sceneManager);
	~Avatar();

	Ogre::Camera* getAvatar1pCamera(void);
	Ogre::Camera* getAvatar3pCamera(void);
	Ogre::Camera* getAvatarTopCamera(void);

	private:

	Ogre::SceneManager* mSceneMgr;
	Ogre::Entity* mAvatarEntity;
	Ogre::SceneNode* mAvatarNode;
	Ogre::SceneNode* mAvatar1pCameraNode;
	Ogre::SceneNode* mAvatar3pCameraNode;
	Ogre::SceneNode* mAvatarTopCameraNode;
	Ogre::Camera* mAvatar1pCamera;
	Ogre::Camera* mAvatar3pCamera;
	Ogre::Camera* mAvatarTopCamera;


}; //End of class declaration

#endif // ENTITY_LISTENER_H
