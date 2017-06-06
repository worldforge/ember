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
#include <OgreSceneManager.h>
#include <OgreTagPoint.h>
#include <components/ogre/model/ModelDefinitionManager.h>

namespace Ember
{
namespace OgreView
{
namespace Gui
{

AttachPointMouseMover::AttachPointMouseMover(AttachPointHelper& attachPointHelper, Model::ModelDefinitionPtr modelDefinition, SimpleRenderContext& renderContext) :
		mAttachPointHelper(attachPointHelper), mModelDefinition(modelDefinition), mRenderContext(renderContext)
{
}

RotateMouseMover::RotateMouseMover(AttachPointHelper& attachPointHelper, Model::ModelDefinitionPtr modelDefinition, SimpleRenderContext& renderContext) :
		AttachPointMouseMover(attachPointHelper, modelDefinition, renderContext)
{
}

TranslateMouseMover::TranslateMouseMover(AttachPointHelper& attachPointHelper, Model::ModelDefinitionPtr modelDefinition, SimpleRenderContext& renderContext) :
		AttachPointMouseMover(attachPointHelper, modelDefinition, renderContext)
{
}

void RotateMouseMover::injectMouseMove(const MouseMotion& motion, bool& freezeMouse)
{
	Ogre::TagPoint* tagPoint = mAttachPointHelper.getTagPoint();

	Ogre::Quaternion rotate;
	//rotate the modelnode
	if (Input::getSingleton().isKeyDown(SDL_SCANCODE_RCTRL) || Input::getSingleton().isKeyDown(SDL_SCANCODE_LCTRL)) {
		rotate.FromAngleAxis(Ogre::Degree(motion.xRelativeMovement * 180), mRenderContext.getCameraOrientation().zAxis());
	} else {
		rotate.FromAngleAxis(Ogre::Degree(-motion.xRelativeMovement * 180), mRenderContext.getCameraOrientation().yAxis());
		Ogre::Quaternion q1;
		q1.FromAngleAxis(Ogre::Degree(-motion.yRelativeMovement * 180), mRenderContext.getCameraOrientation().xAxis());
		rotate = rotate * q1;
	}
	tagPoint->rotate(rotate);
	//we don't want to move the cursor
	freezeMouse = true;
}

bool RotateMouseMover::injectMouseButtonUp(const Input::MouseButton& button)
{
	if (button == Input::MouseButtonLeft) {
		for (Model::AttachPointDefinitionStore::const_iterator I = mModelDefinition->getAttachPointsDefinitions().begin(); I != mModelDefinition->getAttachPointsDefinitions().end(); ++I) {
			if (I->Name == mAttachPointHelper.getAttachPointName()) {
				Model::AttachPointDefinition definition = *I;
				definition.Rotation = mAttachPointHelper.getOrientation();
				mModelDefinition->addAttachPointDefinition(definition);
				break;
			}
		}

		return true;
	}
	return false;
}

void TranslateMouseMover::injectMouseMove(const MouseMotion& motion, bool& freezeMouse)
{
	Ogre::TagPoint* tagPoint = mAttachPointHelper.getTagPoint();

	Ogre::Vector3 translate;
	if (Input::getSingleton().isKeyDown(SDL_SCANCODE_RCTRL) || Input::getSingleton().isKeyDown(SDL_SCANCODE_LCTRL)) {
		translate = Ogre::Vector3(-motion.xRelativeMovement, 0, -motion.yRelativeMovement);
	} else {
		translate = Ogre::Vector3(-motion.xRelativeMovement, motion.yRelativeMovement, 0);
	}
	translate = mRenderContext.getCameraOrientation() * translate;
	tagPoint->translate(translate);
	//we don't want to move the cursor
	freezeMouse = true;
}

bool TranslateMouseMover::injectMouseButtonUp(const Input::MouseButton& button)
{
	if (button == Input::MouseButtonLeft) {
		for (Model::AttachPointDefinitionStore::const_iterator I = mModelDefinition->getAttachPointsDefinitions().begin(); I != mModelDefinition->getAttachPointsDefinitions().end(); ++I) {
			if (I->Name == mAttachPointHelper.getAttachPointName()) {
				Model::AttachPointDefinition definition = *I;
				definition.Translation = mAttachPointHelper.getTagPoint()->getPosition();
				mModelDefinition->addAttachPointDefinition(definition);
				break;
			}
		}

		return true;
	}
	return false;
}

AttachPointHelper::AttachPointHelper(Model::Model& model, const std::string& attachPointName) :
		mModel(model), mAttachPointName(attachPointName), mTagPoint(nullptr)
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
	return mTagPoint;
}

EntityAttachPointHelper::EntityAttachPointHelper(Model::Model& model, const std::string& attachPointName, const std::string& meshName) :
		AttachPointHelper(model, attachPointName)
{
	mEntity = mModel.getManager().createEntity(meshName);
	mTagPoint = mModel.attachObject(attachPointName, mEntity);
}

EntityAttachPointHelper::~EntityAttachPointHelper()
{
	mModel.detachObject(mEntity);
	mEntity->_getManager()->destroyMovableObject(mEntity);
}

Ogre::Quaternion EntityAttachPointHelper::getOrientation() const
{
	return getTagPoint()->getOrientation();
}

ModelAttachPointHelper::ModelAttachPointHelper(Model::Model& model, const std::string& attachPointName, const std::string& modelName) :
		AttachPointHelper(model, attachPointName), mMount(0)
{
	auto definition = Model::ModelDefinitionManager::getSingleton().getByName(modelName);
	mAttachedModel = new Model::Model(mModel.getManager(), definition, modelName);
	Model::ModelBoneProvider* boneProvider = new Model::ModelBoneProvider(mModel, attachPointName, mAttachedModel);

	std::string pose = "";
	const Model::AttachPointDefinitionStore& attachpoints = model.getDefinition()->getAttachPointsDefinitions();
	for (Model::AttachPointDefinitionStore::const_iterator I = attachpoints.begin(); I != attachpoints.end(); ++I) {
		if (I->Name == attachPointName) {
			pose = I->Pose;
			break;
		}
	}

	mMount = new Model::ModelMount(*mAttachedModel, boneProvider, pose);
	mMount->reset();
	mTagPoint = boneProvider->getAttachPointWrapper()->TagPoint;
}

ModelAttachPointHelper::~ModelAttachPointHelper()
{
	delete mMount;
	delete mAttachedModel;
}

Ogre::Quaternion ModelAttachPointHelper::getOrientation() const
{
	//Since the model is rotated 90 degrees by the model mount we need to re-orient it.
	return getTagPoint()->getOrientation() * Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_Y);
}

ModelEditHelper::ModelEditHelper(Model::Model* model, SimpleRenderContext& renderContext) :
		mModel(model), mRenderContext(renderContext), mAttachPointHelper(0), mAttachPointMarker(0), mMouseMover(0)
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
	delete mMouseMover;
	mMouseMover = 0;
	releaseInput();
	if (mAttachPointHelper && mModel) {
		mMouseMover = new RotateMouseMover(*mAttachPointHelper, mModel->getDefinition(), mRenderContext);
		catchInput();
	}
}
void ModelEditHelper::startInputTranslate()
{
	delete mMouseMover;
	mMouseMover = 0;
	releaseInput();
	if (mAttachPointHelper && mModel) {
		mMouseMover = new TranslateMouseMover(*mAttachPointHelper, mModel->getDefinition(), mRenderContext);
		catchInput();
	}
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
	if (mMouseMover) {
		mMouseMover->injectMouseMove(motion, freezeMouse);
	}
	return false;
}

bool ModelEditHelper::injectMouseButtonUp(const Input::MouseButton& button)
{
	if (mMouseMover) {
		if (mMouseMover->injectMouseButtonUp(button)) {
			delete mMouseMover;
			mMouseMover = 0;
			releaseInput();
		}
	}
	return true;
}

bool ModelEditHelper::injectMouseButtonDown(const Input::MouseButton& button)
{
	return true;
}

bool ModelEditHelper::injectChar(int character)
{
	return true;
}

bool ModelEditHelper::injectKeyDown(const SDL_Scancode&)
{
	return true;
}

bool ModelEditHelper::injectKeyUp(const SDL_Scancode&)
{
	return true;
}

}
}
}
