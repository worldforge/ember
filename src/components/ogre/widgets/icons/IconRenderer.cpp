//
// C++ Implementation: IconRenderer
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2007
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
#include "IconRenderer.h"
#include "Icon.h"
#include "IconImageStore.h"

#include "../../model/Model.h"
#include "../../model/ModelDefinition.h"
#include "../../SimpleRenderContext.h"

namespace EmberOgre {

namespace Gui {

namespace Icons {

IconRenderer::IconRenderer(const std::string& prefix, int pixelWidth)
: mPixelWidth(pixelWidth), mRenderContext(new SimpleRenderContext(prefix, pixelWidth, pixelWidth))
{

}


IconRenderer::~IconRenderer()
{
}

void IconRenderer::render(Model::Model* model, Icon* icon)
{
	Ogre::SceneNode* node = mRenderContext->getSceneNode();
	
	if (model) {
		node->detachAllObjects();
		node->attachObject(model);
		
		///check for a defined "icon" view and use that if available, else just reposition the camera
		const Model::ViewDefinitionStore::const_iterator I = model->getDefinition()->getViewDefinitions().find("icon");
		if (I != model->getDefinition()->getViewDefinitions().end()) {
			mRenderContext->resetCameraOrientation();
			mRenderContext->repositionCamera();
			mRenderContext->showFull(model);
			if (I->second->Distance) {
				mRenderContext->setCameraDistance(I->second->Distance);
			}
			Ogre::Camera* camera = mRenderContext->getCamera();
			mRenderContext->getCameraRootNode()->setOrientation(I->second->Rotation);
		} else {
			mRenderContext->resetCameraOrientation();
			mRenderContext->repositionCamera();
			mRenderContext->showFull(model);
			mRenderContext->setCameraDistance(mRenderContext->getDefaultCameraDistance() * 0.75);
 		}
		
// 		mRenderContext->setCameraDistance(0.75);
		mRenderContext->getRenderTexture()->update();
		
		Ogre::HardwarePixelBufferSharedPtr srcBuffer = mRenderContext->getTexture()->getBuffer();
		Ogre::HardwarePixelBufferSharedPtr dstBuffer = icon->getImageStoreEntry()->getTexture()->getBuffer();
		
		Ogre::Box sourceBox(0, 0, mRenderContext->getRenderTexture()->getWidth(), mRenderContext->getRenderTexture()->getHeight());
		
		dstBuffer->blit(srcBuffer, sourceBox, icon->getImageStoreEntry()->getBox());
		node->detachAllObjects();
	}
	
}
    
// void IconRenderer::showModel(const std::string& modelName)
// {
// 	if (mModel) {
// 		mModel->_getManager()->destroyMovableObject(mModel);
// 		//delete mModel;
// 	}
// 	if (modelName != "") {
// 		mModel = Model::Model::createModel(mTexture->getRenderContext()->getSceneManager(), modelName);
// // 		mModel->create(modelName);
// 		///override the rendering distance from the model; we want to always show it in the preview
// 		mModel->setRenderingDistance(0);
// 		setModel(mModel);
// 		mTexture->getRenderContext()->setActive(true);
// 	} else {
// 		setModel(0);
// 		mTexture->getRenderContext()->setActive(false);
// 	}
// }

// }
SimpleRenderContext* IconRenderer::getRenderContext()
{
	return mRenderContext.get();
}

}



}

}
