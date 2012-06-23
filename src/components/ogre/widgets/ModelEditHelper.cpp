/*
 Copyright (C) 2012 Erik Ogenvik

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ModelEditHelper.h"
#include "components/ogre/model/ModelBoneProvider.h"
#include "components/ogre/model/ModelMount.h"
#include "framework/LoggingInstance.h"
#include "services/input/Input.h"
#include <OgreSceneManager.h>
#include <OgreTagPoint.h>
#include <OgreMath.h>
namespace Ember
{
namespace OgreView
{
namespace Gui
{

AttachPointHelper::AttachPointHelper(Model::Model& model, const std::string& attachPointName) :
		mModel(model), mAttachPointName(attachPointName)
{
}

AttachPointHelper::~AttachPointHelper()
{
}

const std::string& AttachPointHelper::getAttachPointName() const
{
	return mAttachPointName;
}

Ogre::TagPoint* AttachPointHelper::getTagPoint() const
{
	return mAttachPointWrapper.TagPoint;
}

EntityAttachPointHelper::EntityAttachPointHelper(Model::Model& model, const std::string& attachPointName, const std::string& meshName) :
		AttachPointHelper(model, attachPointName)
{
	mEntity = mModel._getManager()->createEntity(meshName);
	mAttachPointWrapper = mModel.attachObjectToAttachPoint(attachPointName, mEntity);
}

EntityAttachPointHelper::~EntityAttachPointHelper()
{
	mModel.detachObjectFromBone(mEntity->getName());
	mEntity->_getManager()->destroyMovableObject(mEntity);
}

Ogre::Quaternion EntityAttachPointHelper::getOrientation() const
{
	return getTagPoint()->getOrientation();
}

ModelAttachPointHelper::ModelAttachPointHelper(Model::Model& model, const std::string& attachPointName, const std::string& modelName) :
		AttachPointHelper(model, attachPointName), mMount(0)
{
	mAttachedModel = Model::Model::createModel(*mModel._getManager(), modelName);
	Model::ModelBoneProvider* boneProvider = new Model::ModelBoneProvider(mModel, attachPointName, mAttachedModel);
	mMount = new Model::ModelMount(*mAttachedModel, boneProvider);
	mMount->reset();
	mAttachPointWrapper = *boneProvider->getAttachPointWrapper();
}

ModelAttachPointHelper::~ModelAttachPointHelper()
{
	delete mMount;
	mAttachedModel->_getManager()->destroyMovableObject(mAttachedModel);
}

Ogre::Quaternion ModelAttachPointHelper::getOrientation() const
{
	//Since the model is rotated 90 degrees by the model mount we need to re-orient it.
	return getTagPoint()->getOrientation() * Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_Y);
}

ModelEditHelper::ModelEditHelper(Model::Model* model) :
		mModel(model), mAttachPointHelper(0), mAttachPointMarker(0)
{
}

ModelEditHelper::~ModelEditHelper()
{
	hideAttachPointHelper();
	if (mAttachPointMarker) {
		mAttachPointMarker->_getManager()->destroyMovableObject(mAttachPointMarker);
	}
}

void ModelEditHelper::showAttachPointHelperEntity(const std::string& attachPointName, const std::string& meshName)
{
	try {
		hideAttachPointHelper();
		//this could fail with an exception if the mesh isn't found, so we need to set the mAttachPointHelper to 0 first.
		mAttachPointHelper = new EntityAttachPointHelper(*mModel, attachPointName, meshName);
	} catch (const std::exception& e) {
		S_LOG_WARNING("Could not create attach point helper with mesh " << meshName << ".");
		return;
	}
}

void ModelEditHelper::showAttachPointHelperModel(const std::string& attachPointName, const std::string& modelName)
{
	try {
		hideAttachPointHelper();
		//this could fail with an exception if the model isn't found, so we need to set the mAttachPointHelper to 0 first.
		mAttachPointHelper = new ModelAttachPointHelper(*mModel, attachPointName, modelName);
	} catch (const std::exception& e) {
		S_LOG_WARNING("Could not create attach point helper with model " << modelName << ".");
		return;
	}
}

void ModelEditHelper::hideAttachPointHelper()
{
	delete mAttachPointHelper;
	mAttachPointHelper = 0;
}

void ModelEditHelper::startInputRotate()
{
	catchInput();
}

void ModelEditHelper::catchInput()
{
	Input::getSingleton().addAdapter(this);
}

void ModelEditHelper::releaseInput()
{
	Input::getSingleton().removeAdapter(this);
}

bool ModelEditHelper::injectMouseMove(const MouseMotion& motion, bool& freezeMouse)
{
	if (mAttachPointHelper) {
		Ogre::TagPoint* tagPoint = mAttachPointHelper->getTagPoint();
		//rotate the modelnode
		if (Input::getSingleton().isKeyDown(SDLK_RCTRL) || Input::getSingleton().isKeyDown(SDLK_LCTRL)) {
			tagPoint->roll(Ogre::Degree(motion.xRelativeMovement * 180));
		} else {
			tagPoint->yaw(Ogre::Degree(motion.xRelativeMovement * 180));
			tagPoint->pitch(Ogre::Degree(motion.yRelativeMovement * 180));
		}
	}
	//we don't want to move the cursor
	freezeMouse = true;
	return false;
}

bool ModelEditHelper::injectMouseButtonUp(const Input::MouseButton& button)
{
	if (button == Input::MouseButtonLeft) {
		if (mAttachPointHelper) {
			for (Model::AttachPointDefinitionStore::const_iterator I = mModel->getDefinition()->getAttachPointsDefinitions().begin(); I != mModel->getDefinition()->getAttachPointsDefinitions().end(); ++I) {
				if (I->Name == mAttachPointHelper->getAttachPointName()) {
					Model::AttachPointDefinition definition = *I;
					definition.Rotation = mAttachPointHelper->getOrientation();
					mModel->getDefinition()->addAttachPointDefinition(definition);
				}
			}
		}

		releaseInput();
	}
	return true;
}

bool ModelEditHelper::injectMouseButtonDown(const Input::MouseButton& button)
{
	return true;
}

bool ModelEditHelper::injectChar(char character)
{
	return true;
}

bool ModelEditHelper::injectKeyDown(const SDLKey& key)
{
	return true;
}

bool ModelEditHelper::injectKeyUp(const SDLKey& key)
{
	return true;
}

}
}
}
