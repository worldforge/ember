//
// C++ Implementation: MeshPreview
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#include "MeshPreview.h"
#include "framework/Tokeniser.h"

#include "framework/ConsoleBackend.h"
#include "../EmberOgre.h"
#include "../AvatarCamera.h"
namespace EmberOgre {

const std::string MeshPreview::CREATEMESH("createmesh");

MeshPreview::MeshPreview() : mEntityCounter(0)
{
	Ember::ConsoleBackend::getMainConsole()->registerCommand(CREATEMESH,this);

}


MeshPreview::~MeshPreview()
{
}

void MeshPreview::buildWidget()
{
}

void MeshPreview::createMesh(const std::string& meshName)
{
	Ogre::Entity* entity = 0;
	try {
		entity = EmberOgre::getSingleton().getSceneManager()->createEntity(std::string("meshPreview_" + mEntityCounter++ ), meshName);
	} catch (const Ogre::Exception&) {
		return;
	}
	if (!entity) {
		return;
	}
	
	
	Ogre::SceneNode* node = EmberOgre::getSingleton().getWorldSceneNode()->createChildSceneNode();
	
	//place it five meters in front of the camera
	Ogre::Vector3 o_vector(5,0,0);
	Ogre::Camera* camera = EmberOgre::getSingleton().getMainCamera()->getCamera();
	Ogre::Vector3 o_pos = camera->getPosition() + (camera->getOrientation() * o_vector);
	node->setPosition(o_pos);
	
	node->attachObject(entity);
	
	
	
}

void MeshPreview::runCommand(const std::string &command, const std::string &args)
{
	if (command == CREATEMESH) {
		Ember::Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string meshName = tokeniser.nextToken();
		if (meshName != "") {
			createMesh(meshName);
		}
	
	}
}



}
