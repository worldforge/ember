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

#ifndef ENTITYTEXTUREMANIPULATOR_H_
#define ENTITYTEXTUREMANIPULATOR_H_

#include "services/input/IInputAdapter.h"
#include <CEGUIEvent.h>

namespace CEGUI
{
class Window;
}

namespace Ember
{
namespace OgreView
{
namespace Gui
{

class EntityCEGUITexture;

/**
 * @brief Allows for easy manipulation of an EntityCEGUITexture instance being shown by a CEGUI Window.
 */
class EntityTextureManipulator: public IInputAdapter
{
public:
	/**
	 * @brief Ctor.
	 * @param window The window which shows the texture.
	 * @param texture The entity texture, which contains both the CEGUI Image and the Ogre SceneManager.
	 */
	EntityTextureManipulator(CEGUI::Window& window, EntityCEGUITexture& texture);
	virtual ~EntityTextureManipulator();

	/**
	 ---------Methods implemented from IInputAdapter
	 @see IInputAdapter
	 */
	virtual bool injectMouseMove(const MouseMotion& motion, bool& freezeMouse) = 0;
	virtual bool injectMouseButtonUp(const Input::MouseButton& button);
	virtual bool injectMouseButtonDown(const Input::MouseButton& button);
	virtual bool injectChar(char character);
	virtual bool injectKeyDown(const SDLKey& key);
	virtual bool injectKeyUp(const SDLKey& key);

	/**
	 * @brief Emitted when movement of the entity or camera has started.
	 */
	sigc::signal<void> EventMovementStarted;

	/**
	 * @brief Emitted when movement of the entity or camera has stopped.
	 */
	sigc::signal<void> EventMovementStopped;

protected:

	/**
	 *    Catches input and allows for rotation of the Model
	 *    @see releaseInput
	 */
	void catchInput();

	/**
	 *    Releases input caught with catchInput
	 *    @see catchInput
	 */
	void releaseInput();

	/**
	 *    When the mouse button is pressed over the image, catch input and allow for rotation of the model. When the mouse button is releases, also release input.
	 * @param args
	 * @return
	 */
	bool image_MouseButtonDown(const CEGUI::EventArgs& args);

	/**
	 *    Mouse wheel movements will zoom in and out.
	 * @param args
	 * @return
	 */
	bool image_MouseWheel(const CEGUI::EventArgs& args);

	virtual void handleMouseButtonDown(const CEGUI::MouseEventArgs& mouseArgs);

	CEGUI::Window& mWindow;
	EntityCEGUITexture& mTexture;

	CEGUI::Event::Connection mButtonDownConnection;
	CEGUI::Event::Connection mMoveConnection;
};

/**
 * @brief Manipulates the entity directly.
 */
class DirectEntityTextureManipulator: public EntityTextureManipulator
{
public:
	/**
	 * @brief Ctor.
	 * @param window The window which shows the texture.
	 * @param texture The entity texture, which contains both the CEGUI Image and the Ogre SceneManager.
	 */
	DirectEntityTextureManipulator(CEGUI::Window& window, EntityCEGUITexture& texture);
	virtual ~DirectEntityTextureManipulator();

	virtual bool injectMouseMove(const MouseMotion& motion, bool& freezeMouse);

};

/**
 * @brief Manipulates the camera.
 */
class CameraEntityTextureManipulator: public EntityTextureManipulator
{
public:
	/**
	 * @brief Ctor.
	 * @param window The window which shows the texture.
	 * @param texture The entity texture, which contains both the CEGUI Image and the Ogre SceneManager.
	 */
	CameraEntityTextureManipulator(CEGUI::Window& window, EntityCEGUITexture& texture);
	virtual ~CameraEntityTextureManipulator();

	virtual bool injectMouseMove(const MouseMotion& motion, bool& freezeMouse);

};

/**
 * @brief Manipulates the camera and the entity.
 */
class CombinedEntityTextureManipulator: public EntityTextureManipulator
{
public:
	/**
	 * @brief Ctor.
	 * @param window The window which shows the texture.
	 * @param texture The entity texture, which contains both the CEGUI Image and the Ogre SceneManager.
	 */
	CombinedEntityTextureManipulator(CEGUI::Window& window, EntityCEGUITexture& texture);
	virtual ~CombinedEntityTextureManipulator();

	virtual bool injectMouseMove(const MouseMotion& motion, bool& freezeMouse);

	virtual bool injectMouseButtonUp(const Input::MouseButton& button);

protected:

	/**
	 * @brief Keeps track of if the left mouse button was pressed.
	 *
	 */
	bool mLeftMouseButtonDown;

	virtual void handleMouseButtonDown(const CEGUI::MouseEventArgs& mouseArgs);

};
}
}
}
#endif /* ENTITYTEXTUREMANIPULATOR_H_ */
