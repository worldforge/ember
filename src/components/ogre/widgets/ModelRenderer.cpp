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
#include "ModelRenderer.h"

#include "EntityCEGUITexture.h"

#include <elements/CEGUIStaticImage.h>

#include "components/ogre/model/Model.h"
#include "framework/Exception.h"

// #include "Widget.h"
// #include "../GUIManager.h"


namespace EmberOgre {

ModelRenderer::ModelRenderer(CEGUI::StaticImage* image) 
: mModel(0), MovableObjectRenderer(image)
{

}


ModelRenderer::~ModelRenderer()
{

}

void ModelRenderer::setModel(Model::Model* model)
{
	Ogre::SceneNode* node = mTexture->getSceneNode();
	
	node->detachAllObjects();
	if (model) {
		node->attachObject(model);
		mTexture->repositionCamera();
		if (mAutoShowFull) {
			showFull();
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
		mTexture->getSceneManager()->destroyMovableObject(mModel);
		//delete mModel;
	}
	if (modelName != "") {
		mModel = Model::Model::createModel(mTexture->getSceneManager(), modelName);
// 		mModel->create(modelName);
		///override the rendering distance from the model; we want to always show it in the preview
		mModel->setRenderingDistance(0);
		setModel(mModel);
		mTexture->setActive(true);
	} else {
		setModel(0);
		mTexture->setActive(false);
	}
}


Ogre::MovableObject* ModelRenderer::getMovableObject()
{
	return mModel;
}







}
