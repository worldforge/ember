/*
	ConsoleObjectImpl.cpp by Miguel Guzman (Aglanor)
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

// config headers
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// system headers

// library headers
#include <Ogre.h>

// local headers
#include "MediaDeployer.h"

//TODO: add a sequence for mediadeployer ids
//TODO: add several methods for adding media with different params

MediaDeployer* MediaDeployer::_mediaDeployerInstance = 0;

MediaDeployer & MediaDeployer::getSingleton(void)
{
	if(_mediaDeployerInstance == 0)
		_mediaDeployerInstance = new MediaDeployer;
	return *_mediaDeployerInstance;
}

MediaDeployer::MediaDeployer(void)
{
	mSceneMgr = Ogre::Root::getSingleton().getSceneManager(Ogre::ST_EXTERIOR_CLOSE);
}

MediaDeployer::~MediaDeployer()
{

}

bool MediaDeployer::addMedia(std::string modelName)
{
	return true;
}

bool MediaDeployer::addMedia(std::string modelName, std::string id, Ogre::Vector3 position)
{

	// create the ogre node and entity
	Ogre::SceneNode* ogreNode = dynamic_cast<Ogre::SceneNode*>(mSceneMgr->getRootSceneNode()->createChild());
	Ogre::Entity* ogreEntity;
	ogreEntity = mSceneMgr->createEntity(id,modelName);
	ogreNode->setPosition(position);
	ogreNode->attachObject(ogreEntity);

	return true;
}
