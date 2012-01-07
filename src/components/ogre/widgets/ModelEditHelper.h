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

#ifndef MODELEDITHELPER_H_
#define MODELEDITHELPER_H_

#include "components/ogre/model/Model.h"
#include "components/ogre/OgreIncludes.h"
#include "services/input/IInputAdapter.h"
#include <string>
#include <memory>

namespace Ember
{
namespace OgreView
{
namespace Gui
{

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 *
 * @brief Helper class for model editing.
 *
 * This is a helper class which is meant to be used from Lua, to handle those things that are very hard to do in Lua.
 *
 * It currently mainly helps with attach point editing. As such, it requires a Model instance (even though it mainly deals with ModelDefinition editing).
 */
class ModelEditHelper: public IInputAdapter
{
public:

	/**
	 * @brief Ctor.
	 * @param model The model which will be edited.
	 */
	ModelEditHelper(Model::Model* model);

	/**
	 * @brief Dtor.
	 */
	~ModelEditHelper();

	/**
	 * @brief Shows a helper model attached to the specified attach point, to help when authoring.
	 * @param attachPointName The attach point name to show a helper for.
	 */
	void showAttachPointHelper(const std::string& attachPointName);

	/**
	 * @brief Hides any helper attach point model.
	 */
	void hideAttachPointHelper();

	/**
	 * @brief Starts catching input to instead allow all mouse motion to rotate the currently shown attach point.
	 *
	 * Input will be restored when the user releases the mouse button.
	 */
	void startInputRotate();

	/**
	 ---------Methods implemented from IInputAdapter
	 @see IInputAdapter
	 */
	virtual bool injectMouseMove(const MouseMotion& motion, bool& freezeMouse);
	virtual bool injectMouseButtonUp(const Input::MouseButton& button);
	virtual bool injectMouseButtonDown(const Input::MouseButton& button);
	virtual bool injectChar(char character);
	virtual bool injectKeyDown(const SDLKey& key);
	virtual bool injectKeyUp(const SDLKey& key);

private:

	typedef std::pair<std::string, Model::Model::AttachPointWrapper> AttachPointHelperType;

	/**
	 * @brief The model which is being authored.
	 */
	Model::Model* mModel;

	/**
	 * @brief Keeps track of any attach point helper model being shown.
	 */
	AttachPointHelperType* mAttachPointHelper;

	/**
	 * @brief A marker entity used to provide graphical representation of attach points being edited.
	 */
	Ogre::Entity* mAttachPointMarker;

	/**
	 * @brief Catches all future input to direct it to alter the currently shown attach point.
	 */
	void catchInput();

	/**
	 * @brief Releases input previously caught by catchInput().
	 */
	void releaseInput();

};

}
}
}
#endif /* MODELEDITHELPER_H_ */
