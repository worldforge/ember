//
// C++ Implementation: MovableObjectRenderer
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006
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

#include "MovableObjectRenderer.h"

#include "EntityCEGUITexture.h"

#include <CEGUI/Window.h>

#include <OgreRoot.h>
#include <OgreEntity.h>
#include <OgreRenderTargetListener.h>

using namespace Ember;
namespace Ember
{
namespace OgreView
{
namespace Gui
{

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Responsible for invalidating a CEGUI Window whenever something is drawn to a viewport which the window is showing.
 *
 * This class shouldn't be needed though as with CEGUI 0.7 there's a way to make CEGUI directly use an Ogre render texture.
 */
class CEGUIWindowUpdater: public Ogre::RenderTargetListener
{
protected:
	CEGUI::Window& mWindow;
public:
	CEGUIWindowUpdater(CEGUI::Window& window) :
			mWindow(window)
	{
	}

	virtual void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
	{
		mWindow.invalidate();
	}
};

MovableObjectRenderer::MovableObjectRenderer(CEGUI::Window* image, const std::string& name) :
		mTexture(0), mAutoShowFull(true), mImage(image), mActive(true), mAxisEntity(0), mAxesNode(0), mWindowUpdater(0)
{
	int width = static_cast<int>(image->getPixelSize().d_width);
	int height = static_cast<int>(image->getPixelSize().d_height);
	if (width != 0 && height != 0) {
		mTexture = new EntityCEGUITexture(name, width, height);

		mImage->setProperty("Image", CEGUI::PropertyHelper<CEGUI::Image*>::toString(mTexture->getImage()));

		image->subscribeEvent(CEGUI::Window::EventSized, CEGUI::Event::Subscriber(&MovableObjectRenderer::image_Sized, this));

		// Register this as a frame listener
		Ogre::Root::getSingleton().addFrameListener(this);
		mWindowUpdater = new CEGUIWindowUpdater(*mImage);
		mTexture->getRenderContext()->getRenderTexture()->addListener(mWindowUpdater);

		//Render a blank scene to start with, else we'll get uninitialized buffer garbage shown.
		mTexture->getRenderContext()->getRenderTexture()->update();
	} else {
		throw Exception("Image dimension cannot be 0.");
	}
}

MovableObjectRenderer::~MovableObjectRenderer()
{
	if (mImage && mImage->isPropertyPresent("Image")) {
		mImage->setProperty("Image", "");
	}
	if (mTexture && mWindowUpdater) {
		mTexture->getRenderContext()->getRenderTexture()->removeListener(mWindowUpdater);
	}

	delete mTexture;
	delete mWindowUpdater;
	// Register this as a frame listener
	Ogre::Root::getSingleton().removeFrameListener(this);

}

Ogre::Quaternion MovableObjectRenderer::getEntityRotation() const
{
	return mTexture->getRenderContext()->getSceneNode()->getOrientation();
}

void MovableObjectRenderer::setEntityRotation(const Ogre::Quaternion& rotation)
{
	mTexture->getRenderContext()->getSceneNode()->setOrientation(rotation);
}

Ogre::Vector3 MovableObjectRenderer::getEntityTranslation() const
{
	return mTexture->getRenderContext()->getSceneNode()->getPosition();
}

void MovableObjectRenderer::setEntityTranslation(const Ogre::Vector3& translation)
{
	mTexture->getRenderContext()->getSceneNode()->setPosition(translation);
}

void MovableObjectRenderer::resetCameraOrientation()
{
	mTexture->getRenderContext()->resetCameraOrientation();
}

void MovableObjectRenderer::pitchCamera(Ogre::Degree degrees)
{
	mTexture->getRenderContext()->pitchCamera(degrees);
}

void MovableObjectRenderer::yawCamera(Ogre::Degree degrees)
{
	mTexture->getRenderContext()->yawCamera(degrees);
}

void MovableObjectRenderer::rollCamera(Ogre::Degree degrees)
{
	mTexture->getRenderContext()->rollCamera(degrees);
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
	mTexture->getRenderContext()->showFull(getMovableBoundingRadius());
}

void MovableObjectRenderer::rescaleAxisMarker()
{
	if (mAxesNode) {
		Ogre::Real axesScale = (getMovableBoundingRadius() / 5.0f);
		mAxesNode->setScale(axesScale, axesScale, axesScale);
	}
}

void MovableObjectRenderer::setCameraDistance(float distance)
{

	mTexture->getRenderContext()->setCameraDistance(mTexture->getRenderContext()->getDefaultCameraDistance() * distance);
	/*	Ogre::Vector3 position = mTexture->getDefaultCameraPosition();
	 position.z *= distance;
	 mTexture->getCamera()->setPosition(position);*/
}

float MovableObjectRenderer::getCameraDistance()
{
	return mTexture->getRenderContext()->getCameraDistance();
}

float MovableObjectRenderer::getAbsoluteCameraDistance()
{
	return mTexture->getRenderContext()->getAbsoluteCameraDistance();
}

bool MovableObjectRenderer::frameStarted(const Ogre::FrameEvent& event)
{
	//if the window isn't shown, don't update the render texture
	mTexture->getRenderContext()->setActive(mActive && mImage->isVisible());
	if (mActive && mImage->isVisible()) {
		updateRender();
	}
	return true;
}

void MovableObjectRenderer::updateRender()
{
	try {
		if (mTexture->getRenderContext()->getRenderTexture()) {
			mTexture->getRenderContext()->getRenderTexture()->update();
		}
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when updating render for MovableObjectRenderer." << ex);
	}
}

void MovableObjectRenderer::setBackgroundColour(const Ogre::ColourValue& colour)
{
	mTexture->getRenderContext()->setBackgroundColour(colour);
}

void MovableObjectRenderer::setBackgroundColour(float red, float green, float blue, float alpha)
{
	mTexture->getRenderContext()->setBackgroundColour(red, green, blue, alpha);
}

void MovableObjectRenderer::showAxis()
{
	if (!mAxesNode) {
		mAxesNode = mTexture->getRenderContext()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	}
	if (!mAxisEntity) {
		std::string name(mImage->getName().c_str());
		try {
			mAxisEntity = mTexture->getRenderContext()->getSceneManager()->createEntity(name + "_axes", "common/primitives/model/axes.mesh");
			if (mAxisEntity) {
				try {
					mAxesNode->attachObject(mAxisEntity);
				} catch (const std::exception& ex) {
					S_LOG_WARNING("Error when attaching axes mesh." << ex);
				}
			}
		} catch (const std::exception& ex) {
			S_LOG_WARNING("Error when loading axes mesh." << ex);
		}
	}
	mAxesNode->setVisible(true);
}

void MovableObjectRenderer::hideAxis()
{
	if (mAxesNode) {
		mAxesNode->setVisible(false);
	}
}

SimpleRenderContext::CameraPositioningMode MovableObjectRenderer::getCameraPositionMode() const
{
	return mTexture->getRenderContext()->getCameraPositionMode();
}

void MovableObjectRenderer::setCameraPositionMode(SimpleRenderContext::CameraPositioningMode mode)
{
	mTexture->getRenderContext()->setCameraPositionMode(mode);
}

EntityCEGUITexture& MovableObjectRenderer::getEntityTexture()
{
	return *mTexture;
}

bool MovableObjectRenderer::image_Sized(const CEGUI::EventArgs& e)
{
	const auto size = mImage->getPixelSize();
	//We'll get a crash in Ogre if the width or height is 0.
	if (size.d_width > 0.0f && size.d_height > 0.0f) {
		mTexture->getRenderContext()->getCamera()->setAspectRatio(size.d_width / size.d_height);
	}
	return true;
}

}
}
}
