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
namespace Model
{
class ModelMount;
}
namespace Gui
{

/**
 * @brief A helper class for showing attach points.
 */
class AttachPointHelper
{
public:

	/**
	 * @brief Ctor.
	 * @param model The model for which we want to show attach points.
	 * @param attachPointName The name of the attach point to show.
	 */
	AttachPointHelper(Model::Model& model, const std::string& attachPointName);

	/**
	 * @brief Dtor.
	 */
	virtual ~AttachPointHelper();

	/**
	 * @brief Gets the tag point which has been created for the attach point.
	 * @return A tag point.
	 */
	Ogre::TagPoint* getTagPoint() const;

	/**
	 * @brief Gets the name of the attach point.
	 * @return A name.
	 */
	const std::string& getAttachPointName() const;

	/**
	 * @brief Gets the orientation of the attach point.
	 * @return An orientation.
	 */
	virtual Ogre::Quaternion getOrientation() const = 0;

protected:

	/**
	 * @brief The model which is being edited.
	 */
	Model::Model& mModel;

	/**
	 * @brief The name of the attach point.
	 */
	const std::string mAttachPointName;

	/**
	 * @brief Contains data about the attachment.
	 */
	Model::Model::AttachPointWrapper mAttachPointWrapper;

};

/**
 * @brief A helper class for using an Ogre::Entity instance for showing the attach point.
 */
class EntityAttachPointHelper: public AttachPointHelper
{
public:

	/**
	 * @brief Ctor.
	 * @param model The model for which we want to show attach points.
	 * @param attachPointName The name of the attach point to show.
	 * @param meshName The mesh we want to use for preview.
	 */
	EntityAttachPointHelper(Model::Model& model, const std::string& attachPointName, const std::string& meshName);

	/**
	 * @brief Dtor.
	 */
	~EntityAttachPointHelper();
	virtual Ogre::Quaternion getOrientation() const;
private:

	/**
	 * @brief The entity used as preview.
	 */
	Ogre::Entity* mEntity;
};

/**
 * @brief A helper class for using a Model instance for showing the attach point.
 */
class ModelAttachPointHelper: public AttachPointHelper
{
public:
	/**
	 * @brief Ctor.
	 * @param model The model for which we want to show attach points.
	 * @param attachPointName The name of the attach point to show.
	 * @param modelName The model we want to use for preview.
	 */
	ModelAttachPointHelper(Model::Model& model, const std::string& attachPointName, const std::string& modelName);

	/**
	 * @brief Dtor.
	 */
	~ModelAttachPointHelper();

	virtual Ogre::Quaternion getOrientation() const;
private:

	/**
	 * @brief The model used for preview.
	 */
	Model::Model* mAttachedModel;

	/**
	 * @brief The mount used for attaching the preview model.
	 */
	Model::ModelMount* mMount;
};

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
	 * @brief Shows a helper entity attached to the specified attach point, to help when authoring.
	 * @param attachPointName The attach point name to show a helper for.
	 * @param meshName The name of the mesh to attach. Optional; if left out a general arrow mesh will be used.
	 */
	void showAttachPointHelperEntity(const std::string& attachPointName, const std::string& meshName = "3d_objects/primitives/models/arrow.mesh");

	/**
	 * @brief Shows a helper model attached to the specified attach point, to help when authoring.
	 * @param attachPointName The attach point name to show a helper for.
	 * @param meshName The name of the model to attach.
	 */
	void showAttachPointHelperModel(const std::string& attachPointName, const std::string& modelName);

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

	/**
	 * @brief The model which is being authored.
	 */
	Model::Model* mModel;

	/**
	 * @brief Keeps track of any attach point helper model being shown.
	 */
	AttachPointHelper* mAttachPointHelper;

	/**
	 * @brief A marker entity used to provide graphical representation of attach points being edited.
	 */
	Ogre::MovableObject* mAttachPointMarker;

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
