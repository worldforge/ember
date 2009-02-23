//
// C++ Implementation: ModelRenderer
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

#include "ModelRenderer.h"

#include "EntityCEGUITexture.h"
#include "../SimpleRenderContext.h"

#include <elements/CEGUIGUISheet.h>

#include "components/ogre/model/Model.h"
#include "framework/Exception.h"

// #include "Widget.h"
// #include "../GUIManager.h"


namespace EmberOgre {
namespace Gui {

ModelRenderer::ModelRenderer(CEGUI::Window* image) 
: MovableObjectRenderer(image), mModel(0) 
{
}


ModelRenderer::~ModelRenderer()
{
	mModelReloadedConnection.disconnect();
}

void ModelRenderer::setModel(Model::Model* model)
{
	Ogre::SceneNode* node = mTexture->getRenderContext()->getSceneNode();
	
	node->detachAllObjects();
	if (model) {
		node->attachObject(model);
		repositionSceneNode();
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
			node->setOrientation(Ogre::Quaternion::IDENTITY);
			///rotate node to fit with WF space
			///perhaps this is something to put in the model spec instead?
// 			node->rotate(Ogre::Vector3::UNIT_Y,(Ogre::Degree)90);
			node->rotate(mModel->getRotation());
			
			///translate the scale node according to the translate defined in the model
			node->setPosition(Ogre::Vector3::ZERO);
			node->translate(mModel->getDefinition()->getTranslate());
			
		}
	}

}

Model::Model* ModelRenderer::getModel()
{
	return mModel;
}
    
void ModelRenderer::showModel(const std::string& modelName)
{
	if (mModel) {
		mModel->_getManager()->destroyMovableObject(mModel);
		mModel = 0;
		mModelReloadedConnection.disconnect();
		//delete mModel;
	}
	if (modelName != "") {
		mModel = Model::Model::createModel(mTexture->getRenderContext()->getSceneManager(), modelName);
		if (mModel) {
	// 		mModel->create(modelName);
			///override the rendering distance from the model; we want to always show it in the preview
			mModel->setRenderingDistance(0);
			setModel(mModel);
			mTexture->getRenderContext()->setActive(true);
		}
	} else {
		setModel(0);
		mTexture->getRenderContext()->setActive(false);
	}
}

void ModelRenderer::model_Reloaded()
{
	showFull();
}


Ogre::MovableObject* ModelRenderer::getMovableObject()
{
	return mModel;
}
}
}
