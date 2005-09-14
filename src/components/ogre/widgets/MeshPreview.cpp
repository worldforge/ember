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


#include <elements/CEGUIListbox.h>
#include <elements/CEGUIListboxItem.h> 
#include <elements/CEGUIListboxTextItem.h> 
#include <elements/CEGUIEditbox.h>
#include <elements/CEGUIPushButton.h> 
#include <elements/CEGUISlider.h> 

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

	loadMainSheet("MeshPreview.widget", "MeshPreview/");
	registerConsoleVisibilityToggleCommand("meshpreview");
	enableCloseButton();
	
	
	//bind buttons
	CEGUI::PushButton* button;
	button  = static_cast<CEGUI::PushButton*>(getWindow("Create"));
	BIND_CEGUI_EVENT(button, CEGUI::ButtonBase::EventMouseClick, MeshPreview::createButton_Click);
	
	button  = static_cast<CEGUI::PushButton*>(getWindow("Remove"));
	BIND_CEGUI_EVENT(button, CEGUI::ButtonBase::EventMouseClick, MeshPreview::removeButton_Click);
	
	
	
	mScaleSlider = static_cast<CEGUI::Slider*>(getWindow("Scale"));
	BIND_CEGUI_EVENT(mScaleSlider, CEGUI::Slider::EventValueChanged, MeshPreview::Scale_ValueChanged);

	

	mNameOfMesh = static_cast<CEGUI::Editbox*>(getWindow("MeshName"));
	mCreatedMeshes = static_cast<CEGUI::Listbox*>(getWindow("CreatedMeshes"));
	hide();
// 	loadAllAvailableMeshes();



}

bool MeshPreview::createButton_Click(const CEGUI::EventArgs& args)
{
	if (mNameOfMesh->getText() != "") {
		createMesh(mNameOfMesh->getText().c_str());
	}
	return true;
}

bool MeshPreview::removeButton_Click(const CEGUI::EventArgs& args)
{
	CEGUI::ListboxItem* item = mCreatedMeshes->getFirstSelectedItem();
	if (item) {
		size_t index = mCreatedMeshes->getItemIndex(item);
		removeMesh(index);
	}
	return true;
}

void MeshPreview::removeMesh(size_t index)
{
	if (index > mEntities.size() - 1) {
		return;
	}
	Ogre::Entity* entity = mEntities[index];
	Ogre::SceneNode* node = entity->getParentSceneNode();
	node->detachObject(entity->getName());
	EmberOgre::getSingleton().getSceneManager()->destroySceneNode(node->getName());
	EmberOgre::getSingleton().getSceneManager()->removeEntity(entity);
	EntityStore::iterator I = mEntities.begin();
	for (size_t i = 0; i < index; ++i) {
		++I;
	}
	mEntities.erase(I);
	removedEntity(index);
	
	
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
	Ogre::Vector3 o_vector(0,0,-5);
	Ogre::Camera* camera = EmberOgre::getSingleton().getMainCamera()->getCamera();
	Ogre::Vector3 o_pos = camera->getWorldPosition() + (camera->getWorldOrientation() * o_vector);
	node->setPosition(o_pos);
	
	node->attachObject(entity);
	mEntities.push_back(entity);
	createdNewEntity(entity, node);
	
	
	
}

// void MeshPreview::loadAllAvailableMeshes()
// {
// 	Ogre::StringVectorPtr names = Ogre::ResourceGroupManager::getSingleton().findResourceNames(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "*.mesh");
// 	for (Ogre::StringVector::iterator I = names->begin(); I != names->end(); ++I) {
// //		Ogre::String name = *I;
// //		Ogre::StringUtil::trim(name, false);
// //		if (Ogre::StringUtil::endsWith(name, ".mesh", true)) {
// 			addMeshToAvailableMeshesList(*I);
// //		}
// 	}
// 
// }
// 
// void MeshPreview::addMeshToAvailableMeshesList(const std::string& name) {
// 	CEGUI::ListboxItem* item = new ColoredListItem(name, 0, 0);
// 	mAvailableMeshes->addItem(item);
// }

void MeshPreview::createdNewEntity(Ogre::Entity* entity, Ogre::SceneNode* sceneNode)
{
	CEGUI::String name(entity->getMesh()->getName());
	CEGUI::ListboxItem* item = new ColoredListItem(name, 0, sceneNode);
	mCreatedMeshes->addItem(item);

}

void MeshPreview::removedEntity(size_t index)
{
	CEGUI::ListboxItem* item = mCreatedMeshes->getListboxItemFromIndex(index);
	mCreatedMeshes->removeItem(item);

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
	
	} else {
		Widget::runCommand(command, args);
	}
}

bool MeshPreview::Scale_ValueChanged(const CEGUI::EventArgs& args)
{
	Ogre::SceneNode* node = getActiveSceneNode();
	if (node) {
		float newScale = mScaleSlider->getCurrentValue();
		node->setScale(newScale, newScale, newScale);
	}
	return true;
}

Ogre::SceneNode* MeshPreview::getActiveSceneNode()
{
	CEGUI::ListboxItem* item = mCreatedMeshes->getFirstSelectedItem();
	if (item) {
		return static_cast<Ogre::SceneNode*>(item->getUserData());
	}
	return 0;
}


}
