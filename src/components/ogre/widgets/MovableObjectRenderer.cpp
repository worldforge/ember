//
// C++ Implementation: MovableObjectRenderer
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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
#include "MovableObjectRenderer.h"

#include "EntityCEGUITexture.h"

#include <elements/CEGUIGUISheet.h>

#include "framework/Exception.h"

#include "Widget.h"
#include "../GUIManager.h"

namespace EmberOgre {

MovableObjectRenderer::MovableObjectRenderer(CEGUI::Window* image)
: mImage(image), mTexture(0), mIsInputCatchingAllowed(true), mAutoShowFull(true), mActive(true)
{
	std::string name(image->getName().c_str());
	int width = static_cast<int>(image->getPixelRect().getWidth());
	int height = static_cast<int>(image->getPixelRect().getHeight());
	if (width != 0 && height != 0) {
		mTexture = new EntityCEGUITexture(name, width, height);
		///most models are rotated away from the camera, so as a convenience we'll rotate the node
		//mTexture->getSceneNode()->rotate(Ogre::Vector3::UNIT_Y,(Ogre::Degree)180);
		std::string imageName = std::string("set:") + mTexture->getImage()->getImagesetName().c_str() + " image:" + mTexture->getImage()->getName().c_str();
		mImage->setProperty("Image", imageName);
		//mImage->setImageColours(CEGUI::colour(1.0f, 1.0f, 1.0f));
		BIND_CEGUI_EVENT(mImage, CEGUI::Window::EventMouseButtonDown, MovableObjectRenderer::image_MouseButtonDown);
		BIND_CEGUI_EVENT(mImage, CEGUI::Window::EventMouseWheel, MovableObjectRenderer::image_MouseWheel);
		
		
		/// Register this as a frame listener
		Ogre::Root::getSingleton().addFrameListener(this);
	} else {
		throw Ember::Exception("Image dimension cannot be 0.");
	}
}


MovableObjectRenderer::~MovableObjectRenderer()
{
	if (mTexture) {
		delete mTexture;
	}
	/// Register this as a frame listener
	Ogre::Root::getSingleton().removeFrameListener(this);

}

bool MovableObjectRenderer::injectMouseMove(const MouseMotion& motion, bool& freezeMouse)
{
	///rotate the modelnode
	Ogre::SceneNode* node = mTexture->getSceneNode();
	
	if (GUIManager::getSingleton().getInput().isKeyDown(SDLK_RCTRL) || GUIManager::getSingleton().getInput().isKeyDown(SDLK_LCTRL)) {
		mTexture->roll(Ogre::Degree(motion.xRelativeMovement * 180));
	} else {
		mTexture->yaw(Ogre::Degree(motion.xRelativeMovement * 180));
		mTexture->pitch(Ogre::Degree(motion.yRelativeMovement * 180));
	}
//	node->rotate(Ogre::Vector3::UNIT_Y,(Ogre::Degree)(motion.xRelativeMovement * 180));
	///we don't want to move the cursor
	freezeMouse = true;
	return false;
}

Ogre::Quaternion MovableObjectRenderer::getEntityRotation()
{
	return mTexture->getEntityRotation();
}

void MovableObjectRenderer::resetCameraOrientation()
{
	mTexture->resetCameraOrientation();
}

void MovableObjectRenderer::pitch(Ogre::Degree degrees)
{
	mTexture->pitch(degrees);
}

void MovableObjectRenderer::yaw(Ogre::Degree degrees)
{
	mTexture->yaw(degrees);
}

void MovableObjectRenderer::roll(Ogre::Degree degrees)
{
	mTexture->roll(degrees);
}


bool MovableObjectRenderer::injectMouseButtonUp(const Input::MouseButton& button)
{
	if (button == Input::MouseButtonLeft) {
		releaseInput();
	}
	return true;
}

bool MovableObjectRenderer::injectMouseButtonDown(const Input::MouseButton& button)
{
	return true;
}

bool MovableObjectRenderer::injectChar(char character)
{
	return true;
}

bool MovableObjectRenderer::injectKeyDown(const SDLKey& key)
{
	return true;
}

bool MovableObjectRenderer::injectKeyUp(const SDLKey& key)
{
	return true;
}

bool MovableObjectRenderer::getIsInputCatchingAllowed() const
{
	return mIsInputCatchingAllowed;
}
    
void MovableObjectRenderer::setIsInputCatchingAllowed(bool allowed)
{
	mIsInputCatchingAllowed = allowed;
}

bool MovableObjectRenderer::getAutoShowFull() const
{
	return mAutoShowFull;
}

void MovableObjectRenderer::setAutoShowFull(bool showFull)
{
	mAutoShowFull = showFull;
}

void MovableObjectRenderer::showFull()
{
//	if (mModel) {
		mTexture->showFull(getMovableObject());
//	}
}

void MovableObjectRenderer::setCameraDistance(float distance)
{
	
	mTexture->setCameraDistance(mTexture->getDefaultCameraDistance() * distance);
/*	Ogre::Vector3 position = mTexture->getDefaultCameraPosition();
	position.z *= distance;
	mTexture->getCamera()->setPosition(position);*/
}

float MovableObjectRenderer::getCameraDistance()
{
	return  mTexture->getCameraDistance();
}

void MovableObjectRenderer::catchInput()
{
	GUIManager::getSingleton().getInput().addAdapter(this);
}

void MovableObjectRenderer::releaseInput()
{
	GUIManager::getSingleton().getInput().removeAdapter(this);
}

bool MovableObjectRenderer::image_MouseWheel(const CEGUI::EventArgs& args)
{
	const CEGUI::MouseEventArgs& mouseArgs = static_cast<const CEGUI::MouseEventArgs&>(args);
	
	if (mTexture) {
		if (mouseArgs.wheelChange != 0.0f) {
			float distance = mTexture->getCameraDistance();
			distance += (mouseArgs.wheelChange * 0.1);
			setCameraDistance(distance);
		}
	}
	
	return true;
}


bool MovableObjectRenderer::image_MouseButtonDown(const CEGUI::EventArgs& args)
{
	const CEGUI::MouseEventArgs& mouseArgs = static_cast<const CEGUI::MouseEventArgs&>(args);
	if (mouseArgs.button == CEGUI::LeftButton) {
		///only catch input if it's allowed
		if (getIsInputCatchingAllowed()) {
			catchInput();
		}
	}
	return true;
}

bool MovableObjectRenderer::frameStarted(const Ogre::FrameEvent& event)
{
//	S_LOG_VERBOSE(mImage->getName().c_str() << " visible: " << (mActive && mImage->isVisible()));
	///if the window isn't shown, don't update the render texture
	mTexture->setActive(mActive && mImage->isVisible());
	return true;
}



}
