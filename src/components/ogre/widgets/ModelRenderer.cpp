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

#include "Widget.h"
#include "../GUIManager.h"


namespace EmberOgre {

ModelRenderer::ModelRenderer(CEGUI::StaticImage* image)
: mImage(image), mModel(0), mTexture(0), mIsInputCatchingAllowed(true)
{
	std::string name(image->getName().c_str());
	int width = static_cast<int>(image->getPixelRect().getWidth());
	int height = static_cast<int>(image->getPixelRect().getHeight());
	if (width != 0 && height != 0) {
		mTexture = new EntityCEGUITexture(name, width, height);
		///most models are rotated away from the camera, so as a convenience we'll rotate the node
		mTexture->getSceneNode()->rotate(Ogre::Vector3::UNIT_Y,(Ogre::Degree)180);
		mImage->setImage(mTexture->getImage());
		BIND_CEGUI_EVENT(mImage, CEGUI::Window::EventMouseButtonDown, ModelRenderer::image_MouseButtonDown);

	} else {
		throw Ember::Exception("Image dimension cannot be 0.");
	}
}


ModelRenderer::~ModelRenderer()
{
	if (mTexture) {
		delete mTexture;
	}
}

void ModelRenderer::setModel(Model::Model* model)
{
	Ogre::SceneNode* node = mTexture->getSceneNode();
	
	node->detachAllObjects();
	node->attachObject(model);
	mTexture->repositionCamera();
	
}

Model::Model* ModelRenderer::getModel()
{
	return mModel;
}
    
void ModelRenderer::showModel(const std::string& modelName)
{
	if (mModel) {
		delete mModel;
	}
	mModel = new Model::Model();
	mModel->create(modelName);
	setModel(mModel);
	mTexture->setActive(true);
}

void ModelRenderer::showFullModel()
{
//	if (mModel) {
		mTexture->showFull(mModel);
//	}
}

void ModelRenderer::setCameraDistance(float distance)
{
	
	mTexture->setCameraDistance(mTexture->getDefaultCameraDistance() * distance);
/*	Ogre::Vector3 position = mTexture->getDefaultCameraPosition();
	position.z *= distance;
	mTexture->getCamera()->setPosition(position);*/
}

float ModelRenderer::getCameraDistance()
{
	return  mTexture->getCamera()->getPosition().z;
}

void ModelRenderer::catchInput()
{
	GUIManager::getSingleton().getInput()->addAdapter(this);
}

void ModelRenderer::releaseInput()
{
	GUIManager::getSingleton().getInput()->removeAdapter(this);
}

bool ModelRenderer::image_MouseButtonDown(const CEGUI::EventArgs& args)
{
	///only catch input if it's allowed
	if (getIsInputCatchingAllowed()) {
		catchInput();
	}
	return true;
}




bool ModelRenderer::injectMouseMove(const MouseMotion& motion, bool& freezeMouse)
{
	///rotate the modelnode
	Ogre::SceneNode* node = mTexture->getSceneNode();
	mTexture->yaw(Ogre::Degree(motion.xRelativeMovement * 180));
	mTexture->pitch(Ogre::Degree(motion.yRelativeMovement * 180));
//	node->rotate(Ogre::Vector3::UNIT_Y,(Ogre::Degree)(motion.xRelativeMovement * 180));
	///we don't want to move the cursor
	freezeMouse = true;
	return false;
}

bool ModelRenderer::injectMouseButtonUp(const Input::MouseButton& button)
{
	if (button == Input::MouseButtonLeft) {
		releaseInput();
	}
	return true;
}

bool ModelRenderer::injectMouseButtonDown(const Input::MouseButton& button)
{
	return true;
}

bool ModelRenderer::injectChar(char character)
{
	return true;
}

bool ModelRenderer::injectKeyDown(const SDLKey& key)
{
	return true;
}

bool ModelRenderer::injectKeyUp(const SDLKey& key)
{
	return true;
}

bool ModelRenderer::getIsInputCatchingAllowed() const
{
	return mIsInputCatchingAllowed;
}
    
void ModelRenderer::setIsInputCatchingAllowed(bool allowed)
{
	mIsInputCatchingAllowed = allowed;
}


}
