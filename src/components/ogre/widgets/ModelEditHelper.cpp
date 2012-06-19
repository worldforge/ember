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
	if (mAttachPointMarker) {
		if (mAttachPointMarker->getMovableType() != Ogre::EntityFactory::FACTORY_TYPE_NAME || static_cast<Ogre::Entity*>(mAttachPointMarker)->getMesh()->getName() != meshName) {
			hideAttachPointHelper();
			mAttachPointMarker->_getManager()->destroyMovableObject(mAttachPointMarker);
			mAttachPointMarker = 0;
		} else if (mAttachPointHelper && mAttachPointHelper->first == attachPointName) {
			return;
		}
	}

	if (!mAttachPointMarker) {
		try {
			mAttachPointMarker = mModel->_getManager()->createEntity("3d_objects/primitives/models/arrow.mesh");
		} catch (const std::exception& e) {
			S_LOG_WARNING("Could not create attach point arrow marker entity.");
			return;
		}
	}
	hideAttachPointHelper();
	Model::Model::AttachPointWrapper wrapper = mModel->attachObjectToAttachPoint(attachPointName, mAttachPointMarker);
	mAttachPointHelper = new AttachPointHelperType(attachPointName, wrapper);
}

void ModelEditHelper::showAttachPointHelperModel(const std::string& attachPointName, const std::string& modelName)
{
	if (mAttachPointMarker) {
		if (mAttachPointMarker->getMovableType() != Model::ModelFactory::FACTORY_TYPE_NAME || static_cast<Model::Model*>(mAttachPointMarker)->getDefinition()->getName() != modelName) {
			hideAttachPointHelper();
			mAttachPointMarker->_getManager()->destroyMovableObject(mAttachPointMarker);
			mAttachPointMarker = 0;
		} else if (mAttachPointHelper && mAttachPointHelper->first == attachPointName) {
			return;
		}
	}

	Model::Model* model = 0;

	if (!mAttachPointMarker) {
		try {
			model = Model::Model::createModel(*mModel->_getManager(), modelName);
			mAttachPointMarker = model;
		} catch (const std::exception& e) {
			S_LOG_WARNING("Could not create attach point marker model.");
			return;
		}
	}
	hideAttachPointHelper();

	if (model) {
		Model::ModelBoneProvider* boneProvider = new Model::ModelBoneProvider(*mModel, attachPointName, model);
		Model::ModelMount* mount = new Model::ModelMount(*model, boneProvider);
		mount->reset();
		Model::Model::AttachPointWrapper wrapper = *boneProvider->getAttachPointWrapper();
		mAttachPointHelper = new AttachPointHelperType(attachPointName, wrapper);
	}
}

void ModelEditHelper::hideAttachPointHelper()
{
	if (mAttachPointHelper) {
		Model::Model::AttachPointWrapper wrapper = mAttachPointHelper->second;
		mModel->detachObjectFromBone(mAttachPointHelper->second.Movable->getName());
		delete mAttachPointHelper;
		mAttachPointHelper = 0;
	}
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
		Ogre::TagPoint* tagPoint = mAttachPointHelper->second.TagPoint;
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
				if (I->Name == mAttachPointHelper->first) {
					Model::AttachPointDefinition definition = *I;
					definition.Rotation = mAttachPointHelper->second.TagPoint->getOrientation() * Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_Y);
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
