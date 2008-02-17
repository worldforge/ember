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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "MeshPreview.h"
#include "framework/Tokeniser.h"
#include "ColouredListItem.h"

#include "framework/ConsoleBackend.h"
#include "../EmberOgre.h"
#include "../AvatarCamera.h"


#include <elements/CEGUIListbox.h>
#include <elements/CEGUIListboxItem.h> 
#include <elements/CEGUIListboxTextItem.h> 
#include <elements/CEGUIEditbox.h>
#include <elements/CEGUIPushButton.h> 
#include <elements/CEGUISlider.h> 

#include "framework/Exception.h"

namespace EmberOgre {
namespace Gui {


MeshPreviewHandler::MeshPreviewHandler() : mEntityCounter(0)
{
}

size_t MeshPreviewHandler::createInstance(const std::string& meshName)
{
	Ogre::Entity* entity = 0;
	try {
		entity = EmberOgre::getSingleton().getSceneManager()->createEntity(std::string("meshPreview_" + mEntityCounter++ ), meshName);
	} catch (const Ogre::Exception&) {
		return 0;
	}
	if (!entity) {
		return 0;
	}
	
	
	Ogre::SceneNode* node = EmberOgre::getSingleton().getWorldSceneNode()->createChildSceneNode();
	
	//place it five meters in front of the camera
	Ogre::Vector3 o_vector(0,0,-5);
	Ogre::Camera* camera = EmberOgre::getSingleton().getMainCamera()->getCamera();
	Ogre::Vector3 o_pos = camera->getDerivedPosition() + (camera->getDerivedOrientation() * o_vector);
	node->setPosition(o_pos);
	
	node->attachObject(entity);
	MeshPreviewMeshInstance instance(entity);
	mInstances.push_back(instance);
	//mEntities.push_back(entity);
	//createdNewEntity(entity, node);
	EventCreatedInstance.emit(mInstances.size() - 1);
	return mInstances.size();
	
	
	
}

void MeshPreviewHandler::removeInstance(size_t index)
{
	if (index > mInstances.size() - 1) {
		return;
	}
	EventRemoveInstance.emit(index);
	MeshPreviewMeshInstance instance = getInstance(index);
	Ogre::Entity* entity = instance.getEntity();
	Ogre::SceneNode* node = entity->getParentSceneNode();
	node->detachObject(entity->getName());
	EmberOgre::getSingleton().getSceneManager()->destroySceneNode(node->getName());
	EmberOgre::getSingleton().getSceneManager()->destroyEntity(entity);
	InstanceStore::iterator I = mInstances.begin();
	for (size_t i = 0; i < index; ++i) {
		++I;
	}
	mInstances.erase(I);
	
	
}
 	
void MeshPreviewHandler::updateAnimation(Ogre::Real elapsedTime)
{
	InstanceStore::iterator I = mInstances.begin();
	for (;I != mInstances.end(); ++I) {
		I->updateAnimation(elapsedTime);
	}
}


MeshPreviewMeshInstance& MeshPreviewHandler::getInstance(size_t position) 
{
	if (mInstances.size() < position) {
		throw Ember::Exception("Not that many instances in the store.");
	}
	return mInstances[position];
}



MeshPreviewMeshInstance::MeshPreviewMeshInstance(Ogre::Entity* entity): mEntity(entity)
{
}

void MeshPreviewMeshInstance::startAnimation(const std::string& name)
{
	Ogre::AnimationState* state = mEntity->getAnimationState(name);
	state->setLoop(true);
	if (state == 0) { 
		return;
	}
	state->setEnabled(true);
	mActiveAnimations.insert(AnimationStore::value_type(name, state));
	
}

void MeshPreviewMeshInstance::stopAnimation(const std::string& name)
{
	AnimationStore::iterator I = mActiveAnimations.find(name);
	if (I == mActiveAnimations.end()) {
		return;
	}
	mActiveAnimations.erase(I);
	
}

void MeshPreviewMeshInstance::toggleAnimation(const std::string& name)
{
	if (mActiveAnimations.find(name) != mActiveAnimations.end()) {
		stopAnimation(name);
	} else {
		startAnimation(name);
	}
}

void MeshPreviewMeshInstance::resetAnimations()
{
	Ogre::AnimationStateSet* states = getEntity()->getAllAnimationStates();
	if (states != 0) {
		Ogre::AnimationStateIterator I = states->getAnimationStateIterator ();
		while (I.hasMoreElements()) {
			I.getNext()->setEnabled(false);
		}
	}

	mActiveAnimations.clear();
	getEntity()->getSkeleton()->reset();
	
}

void MeshPreviewMeshInstance::updateAnimation(Ogre::Real elapsedTime)
{
//	S_LOG_INFO("Updating animations. Size: " << mActiveAnimations.size());
	AnimationStore::iterator I = mActiveAnimations.begin();
	for (;I != mActiveAnimations.end(); ++I) {
//		S_LOG_INFO("Updating " << I->second->getAnimationName() << " with: " << elapsedTime);
		I->second->addTime(elapsedTime);
	}
}

bool MeshPreviewMeshInstance::isAnimationPlaying(const std::string& name)
{
	return mActiveAnimations.find(name) != mActiveAnimations.end();
}

bool MeshPreviewMeshInstance::isAnimationEnabled(const std::string& name)
{
	Ogre::AnimationState* state = mEntity->getAnimationState(name);
	if (state == 0) { 
		return false;
	}
	return state->getEnabled();
	
}

Ogre::Entity* MeshPreviewMeshInstance::getEntity() const
{
	return mEntity;
}

Ogre::SceneNode* MeshPreviewMeshInstance::getSceneNode() const
{
	return getEntity()->getParentSceneNode();
}



MeshPreview::MeshPreview() : CreateMesh("createmesh", this, "Create a mesh.")
{
	mHandler.EventCreatedInstance.connect(sigc::mem_fun(*this, &MeshPreview::createdNewEntity));
	mHandler.EventRemoveInstance.connect(sigc::mem_fun(*this, &MeshPreview::removedEntity));

}


MeshPreview::~MeshPreview()
{
}

void MeshPreview::buildWidget()
{

	loadMainSheet("MeshPreview.layout", "MeshPreview/");
	registerConsoleVisibilityToggleCommand("meshpreview");
	enableCloseButton();
	
	
	//bind buttons
	CEGUI::PushButton* button;
	button  = static_cast<CEGUI::PushButton*>(getWindow("Create"));
	BIND_CEGUI_EVENT(button, CEGUI::ButtonBase::EventMouseClick, MeshPreview::createButton_Click);
	
	button  = static_cast<CEGUI::PushButton*>(getWindow("Remove"));
	BIND_CEGUI_EVENT(button, CEGUI::ButtonBase::EventMouseClick, MeshPreview::removeButton_Click);
	
	CEGUI::Window* meshNameBox = getWindow("MeshName");
	BIND_CEGUI_EVENT(meshNameBox, CEGUI::Editbox::EventTextAccepted, MeshPreview::createButton_Click)
	
	
	
	mScaleSlider = static_cast<CEGUI::Slider*>(getWindow("Scale"));
	BIND_CEGUI_EVENT(mScaleSlider, CEGUI::Slider::EventValueChanged, MeshPreview::Scale_ValueChanged);


	mNameOfMesh = static_cast<CEGUI::Editbox*>(getWindow("MeshName"));
	mCreatedMeshes = static_cast<CEGUI::Listbox*>(getWindow("CreatedMeshes"));
	BIND_CEGUI_EVENT(mCreatedMeshes, CEGUI::Listbox::EventSelectionChanged, MeshPreview::createdMeshes_EventSelectionChanged);
	
	mAnimations = static_cast<CEGUI::Listbox*>(getWindow("Animations"));
	
	mPlayAnimation = static_cast<CEGUI::PushButton*>(getWindow("PlayAnimation"));
	BIND_CEGUI_EVENT(mPlayAnimation, CEGUI::ButtonBase::EventMouseClick, MeshPreview::playAnimation_MouseClick);
	
	CEGUI::Window* resetButton = getWindow("ResetAnimation");
	BIND_CEGUI_EVENT(resetButton, CEGUI::ButtonBase::EventMouseClick, MeshPreview::resetAnimation_MouseClick);
	
	
	hide();
// 	loadAllAvailableMeshes();



}

void MeshPreview::frameStarted(const Ogre::FrameEvent& evt)
{
	mHandler.updateAnimation(evt.timeSinceLastFrame);
}


bool MeshPreview::createButton_Click(const CEGUI::EventArgs& args)
{
	if (mNameOfMesh->getText() != "") {
		mHandler.createInstance(mNameOfMesh->getText().c_str());
	}
	return true;
}

bool MeshPreview::removeButton_Click(const CEGUI::EventArgs& args)
{
	CEGUI::ListboxItem* item = mCreatedMeshes->getFirstSelectedItem();
	if (item) {
		size_t index = mCreatedMeshes->getItemIndex(item);
		mHandler.removeInstance(index);
	}
	return true;
}


bool MeshPreview::playAnimation_MouseClick(const CEGUI::EventArgs& args)
{
	try {
		MeshPreviewMeshInstance& instance = getActiveInstance();
		CEGUI::ListboxItem* item = mAnimations->getFirstSelectedItem();
		if (item) {
			std::string animationName = mAnimationNames[item->getID()];
			instance.toggleAnimation(animationName);
		}
		fillAnimationList(instance);
	} catch (const Ember::Exception&) {
		return true;
	}
	return true;
}

bool MeshPreview::resetAnimation_MouseClick(const CEGUI::EventArgs& args)
{
	try {
		MeshPreviewMeshInstance& instance = getActiveInstance();
		instance.resetAnimations();
		fillAnimationList(instance);
	} catch (const Ember::Exception&) {
		return true;
	}
	
	return true;
}




void MeshPreview::createdNewEntity(size_t index)
{
	try {
		MeshPreviewMeshInstance instance = mHandler.getInstance(index);
		Ogre::Entity* entity = instance.getEntity();
		CEGUI::String name(entity->getMesh()->getName());
	//	CEGUI::ListboxItem* item = new ColouredListItem(name, 0, index);
		CEGUI::ListboxItem* item = new Gui::ColouredListItem(name, index);
		mCreatedMeshes->addItem(item);
		
	} catch (const Ember::Exception&)
	{
		return;
	}

}

void MeshPreview::removedEntity(size_t index)
{
//	MeshPreviewMeshInstance instance = mHandler.getInstance(index);
	CEGUI::ListboxItem* item = mCreatedMeshes->getListboxItemFromIndex(index);
	if (item) {
		mCreatedMeshes->removeItem(item);
	}

}
		
void MeshPreview::fillAnimationList(MeshPreviewMeshInstance& instance )
{
	mAnimations->resetList();
	mAnimationNames.clear();
	Ogre::AnimationStateSet* states = instance.getEntity()->getAllAnimationStates();
	if (states != 0) {
		uint i = 0;
		Ogre::AnimationStateIterator I = states->getAnimationStateIterator ();
		while (I.hasMoreElements()) {
			Ogre::AnimationState* state = I.getNext();
			mAnimationNames.push_back(state->getAnimationName());
			std::string name(state->getAnimationName());
			if (instance.isAnimationPlaying(state->getAnimationName())) {
				name += " (playing)";
			} else if (instance.isAnimationEnabled(state->getAnimationName())) {
				name += " (paused)";
			}
			CEGUI::ListboxItem* item = new Gui::ColouredListItem(name, i++);
			mAnimations->addItem(item);
//			instance.startAnimation(I->first);
		}
	}
}

void MeshPreview::runCommand(const std::string &command, const std::string &args)
{
	if (CreateMesh == command) {
		Ember::Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string meshName = tokeniser.nextToken();
		if (meshName != "") {
			mHandler.createInstance(meshName);
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
		size_t index = mCreatedMeshes->getItemIndex(item);
/*		size_t index = item->getUserData(); */
		try {
			const MeshPreviewMeshInstance instance = mHandler.getInstance(index);
			return instance.getSceneNode();
		} catch (const Ember::Exception&) {
			return 0;
		}
	}
	return 0;
}

MeshPreviewMeshInstance& MeshPreview::getActiveInstance()
{
	CEGUI::ListboxItem* item = mCreatedMeshes->getFirstSelectedItem();
	if (item) {
		size_t index = mCreatedMeshes->getItemIndex(item);
/*		size_t index = item->getUserData(); */
	
		return mHandler.getInstance(index);
	} 
	throw Ember::Exception("No selected item.");
}

bool MeshPreview::createdMeshes_EventSelectionChanged(const CEGUI::EventArgs& args)
{
	try {
		MeshPreviewMeshInstance& instance = getActiveInstance();
		fillAnimationList(instance);
	} catch (const Ember::Exception&) {
	}
	return true;

}
}

}
