//
// C++ Interface: ModelRenderer
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
#ifndef EMBEROGREMODELRENDERER_H
#define EMBEROGREMODELRENDERER_H

#include "MovableObjectRenderer.h"
#include <OgreVector3.h>
#include <OgreQuaternion.h>

#include <sigc++/connection.h>

namespace CEGUI
{
class GUISheet;

}

namespace Ember
{
namespace OgreView
{

namespace Model
{
class Model;
class ModelMount;
}

namespace Gui
{

/**

 Renders a single Model to a CEGUI texture. Useful for stuff like inventory or preview of models.

 @author Erik Ogenvik
 */
class ModelRenderer: public MovableObjectRenderer
{
public:

	/**
	 *    Creates a one shot image. This is rather inexpensive, since there's no need for rendering each frame, but it disallows updates to the image.
	 * @param image
	 */
	//static void OneShotRender(CEGUI::Window* image);
	/**
	 * Constructir
	 * @param image A valid CEGUI::StaticImage, to which the Model will be rendered.
	 * @param name A unique name.
	 * @return
	 */
	ModelRenderer(CEGUI::Window* image, const std::string& name);

	virtual ~ModelRenderer();

	/**
	 * @brief Renders the submitted Model.
	 * @param modelName A valid Model
	 */
	void showModel(const std::string& modelName, const Ogre::Vector3& translation = Ogre::Vector3::ZERO, const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY);

	/**
	 * @brief Returns the current rendered Model, or null if none is set.
	 * @return The current model instance.
	 */
	Model::Model* getModel();

	/**
	 * @brief Repositions the scene node using the settings of the Model instance.
	 */
	void repositionSceneNode();

	/**
	 * @brief Updates the texture by rendering one frame manually.
	 */
	void updateRender() override;

protected:

	void showModel();

	float getMovableBoundingRadius() override;

	void model_Reloaded();

	void delayedUpdateRender();

	Model::Model* mModel;

	Model::ModelMount* mModelMount;

	sigc::connection mModelReloadedConnection;
	sigc::connection mModelDelayedUpdateConnection;

	/**
	 * @brief The default translation for the model.
	 *
	 * This is set when a model is shown.
	 */
	Ogre::Vector3 mDefaultTranslation;

	/**
	 * @brief The default rotation for the model.
	 *
	 * This is set when a model is shown.
	 */
	Ogre::Quaternion mDefaultRotation;

};
}
}

}

#endif
