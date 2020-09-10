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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
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

#include <utility>

namespace Ember {
namespace OgreView {
namespace Gui {

RotateMouseMover2::RotateMouseMover2(SimpleRenderContext& renderContext,
									 std::function<void(const Ogre::Quaternion&)> rotateCallback,
									 std::function<void()> stopCallback)
		: mRenderContext(renderContext),
		  mRotateCallback(std::move(rotateCallback)),
		  mStopCallback(std::move(stopCallback)) {

}


void RotateMouseMover2::injectMouseMove(const MouseMotion& motion, bool& freezeMouse) {

	Ogre::Quaternion rotate;
	if (Input::getSingleton().isKeyDown(SDL_SCANCODE_RCTRL) || Input::getSingleton().isKeyDown(SDL_SCANCODE_LCTRL)) {
		rotate.FromAngleAxis(Ogre::Degree(motion.xRelativeMovement * 180), mRenderContext.getCameraOrientation().zAxis());
	} else {
		rotate.FromAngleAxis(Ogre::Degree(-motion.xRelativeMovement * 180), mRenderContext.getCameraOrientation().yAxis());
		Ogre::Quaternion q1;
		q1.FromAngleAxis(Ogre::Degree(-motion.yRelativeMovement * 180), mRenderContext.getCameraOrientation().xAxis());
		rotate = rotate * q1;
	}
	freezeMouse = true;
	mRotateCallback(rotate);
	//we don't want to move the cursor

}

bool RotateMouseMover2::injectMouseButtonUp(const Input::MouseButton& button) {
	if (button == Input::MouseButtonLeft) {
		mStopCallback();

		return true;
	}
	return false;
}

TranslateMouseMover2::TranslateMouseMover2(SimpleRenderContext& renderContext,
										   std::function<void(const Ogre::Vector3&)> translateCallback,
										   std::function<void()> stopCallback)
		: mRenderContext(renderContext),
		  mTranslateCallback(std::move(translateCallback)),
		  mStopCallback(std::move(stopCallback)) {}


void TranslateMouseMover2::injectMouseMove(const MouseMotion& motion, bool& freezeMouse) {

	Ogre::Vector3 translate;
	if (Input::getSingleton().isKeyDown(SDL_SCANCODE_RCTRL) || Input::getSingleton().isKeyDown(SDL_SCANCODE_LCTRL)) {
		translate = Ogre::Vector3(-motion.xRelativeMovement, 0, -motion.yRelativeMovement);
	} else {
		translate = Ogre::Vector3(-motion.xRelativeMovement, motion.yRelativeMovement, 0);
	}
	translate = mRenderContext.getCameraOrientation() * translate;
	mTranslateCallback(translate);
	//we don't want to move the cursor
	freezeMouse = true;
}

bool TranslateMouseMover2::injectMouseButtonUp(const Input::MouseButton& button) {
	if (button == Input::MouseButtonLeft) {
		mStopCallback();

		return true;
	}
	return false;
}

ModelAttachPointHelper::ModelAttachPointHelper(Model::Model& model, Model::AttachPointDefinition attachPointDefinition, std::unique_ptr<Model::Model> attachedModel) :
		mAttachPoint(std::move(attachPointDefinition)),
		mModel(model),
		mAttachedModel(std::move(attachedModel)),
		mModelBoneProvider(new Model::ModelBoneProvider(nullptr, mModel)) {

	mAttachedModel->setUseInstancing(false);

	mModelBoneProvider->setAttachPointDefinition(mAttachPoint);

	mMount = std::make_unique<Model::ModelMount>(*mAttachedModel, std::unique_ptr<Model::ModelBoneProvider>(mModelBoneProvider), mAttachPoint.Pose);
	mMount->reset();

	mAttachedModel->load();


}

ModelAttachPointHelper::~ModelAttachPointHelper() = default;

ModelEditHelper::ModelEditHelper(Model::Model* model, SimpleRenderContext& renderContext) :
		mModel(model),
		mRenderContext(renderContext),
		mAttachPointHelper(nullptr),
		mAttachPointMarker(nullptr),
		mMouseMover(nullptr) {
	model->Reloaded.connect([this]() { mAttachPointHelper.reset(); });
}

ModelEditHelper::~ModelEditHelper() {
	hideAttachPointHelper();
	if (mAttachPointMarker) {
		mAttachPointMarker->_getManager()->destroyMovableObject(mAttachPointMarker);
	}
}

void ModelEditHelper::showAttachPointHelperEntity(const std::string& attachPointName, const std::string& meshName) {
	hideAttachPointHelper();
	auto& attachpoints = mModel->getDefinition()->getAttachPointsDefinitions();
	for (const auto& attachpoint : attachpoints) {
		if (attachpoint.Name == attachPointName) {
			//We'll automatically create a model which shows just the specified mesh.
			auto modelDef = std::make_shared<Model::ModelDefinition>();
			modelDef->setOrigin(meshName);
			//Create a single submodel definition using the mesh
			Model::SubModelDefinition subModelDefinition{meshName};
			modelDef->addSubModelDefinition(subModelDefinition);

			auto attachedModel = std::make_unique<Model::Model>(mModel->getManager(), modelDef, meshName);

			try {
				mAttachPointHelper = std::make_unique<ModelAttachPointHelper>(*mModel, attachpoint, std::move(attachedModel));
			} catch (const std::exception& e) {
				S_LOG_WARNING("Could not create attach point helper with mesh " << meshName << ".");
				return;
			}
			break;
		}
	}
}

void ModelEditHelper::showAttachPointHelperModel(const std::string& attachPointName, const std::string& modelName) {
	hideAttachPointHelper();
	auto& attachpoints = mModel->getDefinition()->getAttachPointsDefinitions();
	for (const auto& attachpoint : attachpoints) {
		if (attachpoint.Name == attachPointName) {

			auto definition = Model::ModelDefinitionManager::getSingleton().getByName(modelName);
			if (definition) {
				auto attachedModel = std::make_unique<Model::Model>(mModel->getManager(), definition, modelName);

				try {
					mAttachPointHelper = std::make_unique<ModelAttachPointHelper>(*mModel, attachpoint, std::move(attachedModel));
				} catch (const std::exception& e) {
					S_LOG_WARNING("Could not create attach point helper with model " << modelName << ".");
					return;
				}
			}
		}
	}
}

void ModelEditHelper::hideAttachPointHelper() {
	mAttachPointHelper.reset();
}

void ModelEditHelper::startInputRotate() {
	mMouseMover.reset();
	releaseInput();
	if (mAttachPointHelper && mModel) {
		mMouseMover = std::make_unique<RotateMouseMover2>(mRenderContext, [&](const Ogre::Quaternion& rotate) {
			if (mAttachPointHelper) {
				mAttachPointHelper->mAttachPoint.Rotation = mAttachPointHelper->mAttachPoint.Rotation * rotate;
				mAttachPointHelper->mModelBoneProvider->setAttachPointDefinition(mAttachPointHelper->mAttachPoint);
			}
		}, [&]() {
			if (mAttachPointHelper) {
				mModel->getDefinition()->addAttachPointDefinition(mAttachPointHelper->mAttachPoint);
				mMouseMover.reset();
			}
		});
		catchInput();
	}
}

void ModelEditHelper::startInputTranslate() {
	mMouseMover.reset();
	releaseInput();
	if (mAttachPointHelper && mModel) {
		mMouseMover = std::make_unique<TranslateMouseMover2>(mRenderContext, [&](const Ogre::Vector3& translate) {
			if (mAttachPointHelper) {
				mAttachPointHelper->mAttachPoint.Translation = mAttachPointHelper->mAttachPoint.Translation + translate;
				mAttachPointHelper->mModelBoneProvider->setAttachPointDefinition(mAttachPointHelper->mAttachPoint);
			}
		}, [&]() {
			if (mAttachPointHelper) {
				mModel->getDefinition()->addAttachPointDefinition(mAttachPointHelper->mAttachPoint);
				mMouseMover.reset();
			}
		});
		catchInput();
	}
}

void ModelEditHelper::catchInput() {
	Input::getSingleton().addAdapter(this);
}

void ModelEditHelper::releaseInput() {
	Input::getSingleton().removeAdapter(this);
}

bool ModelEditHelper::injectMouseMove(const MouseMotion& motion, bool& freezeMouse) {
	if (mMouseMover) {
		mMouseMover->injectMouseMove(motion, freezeMouse);
	}
	return false;
}

bool ModelEditHelper::injectMouseButtonUp(Input::MouseButton button) {
	if (mMouseMover) {
		if (mMouseMover->injectMouseButtonUp(button)) {
			mMouseMover.reset();
			releaseInput();
		}
	}
	return true;
}

bool ModelEditHelper::injectMouseButtonDown(Input::MouseButton button) {
	return true;
}

bool ModelEditHelper::injectChar(int character) {
	return true;
}

bool ModelEditHelper::injectKeyDown(const SDL_Scancode&) {
	return true;
}

bool ModelEditHelper::injectKeyUp(const SDL_Scancode&) {
	return true;
}

}
}
}
