//
// C++ Implementation: ModelRenderer
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2005
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

#include "ModelRenderer.h"

#include "EntityCEGUITexture.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelMount.h"
#include "components/ogre/model/SubModel.h"
#include "components/ogre/model/ModelDefinitionManager.h"

#include <OgreSceneManager.h>
#include <CEGUI/Window.h>

#include <sigc++/bind.h>
#include <components/ogre/SceneNodeProvider.h>

namespace Ember
{
namespace OgreView
{
namespace Gui
{

ModelRenderer::ModelRenderer(CEGUI::Window* image, const std::string& name) :
		MovableObjectRenderer(image, name), mModel(nullptr), mModelMount(nullptr), mDefaultTranslation(Ogre::Vector3::ZERO), mDefaultRotation(Ogre::Quaternion::IDENTITY)
{
}

ModelRenderer::~ModelRenderer()
{
	delete mModelMount;
	delete mModel;
	mModelReloadedConnection.disconnect();
	mModelDelayedUpdateConnection.disconnect();
}

void ModelRenderer::showModel()
{
	if (mModel) {
		repositionSceneNode();
		rescaleAxisMarker();
		mTexture->getRenderContext()->repositionCamera();
		if (mAutoShowFull) {
			mModelReloadedConnection = mModel->Reloaded.connect(sigc::mem_fun(*this, &ModelRenderer::model_Reloaded));
			showFull();
		}
	}

}

void ModelRenderer::repositionSceneNode()
{
	if (mModel) {
		Ogre::SceneNode* node = mTexture->getRenderContext()->getSceneNode();
		if (node) {
			node->setOrientation(mDefaultRotation);
			node->rotate(mModel->getDefinition()->getRotation());

			//translate the scale node according to the translate defined in the model
			node->setPosition(mDefaultTranslation);
			node->translate(mModel->getDefinition()->getTranslate());

		}
	}
}

void ModelRenderer::updateRender()
{
	if (mModel) {
		if (!mModel->isLoaded()) {
			//If it's being loaded in a background thread, listen for reloading and render it then. The "Reload" signal will be emitted in the main thread.
			mModelDelayedUpdateConnection.disconnect();
			mModelDelayedUpdateConnection = mModel->Reloaded.connect(sigc::mem_fun(*this, &ModelRenderer::delayedUpdateRender));
		} else {
			MovableObjectRenderer::updateRender();
		}
	}
}

void ModelRenderer::delayedUpdateRender()
{
	if (mAutoShowFull) {
		showFull();
	}
	repositionSceneNode();
	mTexture->getRenderContext()->repositionCamera();
	updateRender();
	mModelDelayedUpdateConnection.disconnect();
}

Model::Model* ModelRenderer::getModel()
{
	return mModel;
}

void ModelRenderer::showModel(const std::string& modelName, const Ogre::Vector3& translation, const Ogre::Quaternion& orientation)
{
	mDefaultRotation = orientation;
	mDefaultTranslation = translation;
	if (mModel) {
		delete mModelMount;
		mModelMount = nullptr;
		delete mModel;
		mModel = nullptr;
		mModelReloadedConnection.disconnect();
		mModelDelayedUpdateConnection.disconnect();
		//delete mModel;
	}
	auto modelDef = Model::ModelDefinitionManager::getSingleton().getByName(modelName);
	if (!modelDef.isNull()) {
		mModel = new Model::Model(*mTexture->getRenderContext()->getSceneManager(), modelDef, modelName);
		//override the rendering distance from the model; we want to always show it in the preview
		mModel->setRenderingDistance(0);

		mModelMount = new Model::ModelMount(*mModel, new SceneNodeProvider(mTexture->getRenderContext()->getSceneNode(), nullptr, false));

		mModel->load();
		showModel();

		mTexture->getRenderContext()->setActive(true);
	} else {
		mTexture->getRenderContext()->setActive(false);
	}
}

void ModelRenderer::model_Reloaded()
{
	rescaleAxisMarker();
	showFull();
}

float ModelRenderer::getMovableBoundingRadius()
{
	if (mModel) {
		return mModel->getCombinedBoundingRadius();
	}
	return 0;
}
}
}
}
