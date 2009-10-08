//
// C++ Interface: ModelRenderer
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005
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

#include <sigc++/connection.h>

namespace CEGUI
{
class GUISheet;

}

namespace EmberOgre
{

namespace Model
{
class Model;
}

namespace Gui
{

/**

 Renders a single Model to a CEGUI texture. Useful for stuff like inventory or preview of models.

 @author Erik Hjortsberg
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
	 * @return
	 */
	ModelRenderer(CEGUI::Window* image);

	virtual ~ModelRenderer();

	/**
	 * Renders the submitted Model.
	 * @param modelName A valid Model
	 */
	void showModel(const std::string& modelName);

	/**
	 * Returns the current rendered Model, or null if none is set.
	 * @return
	 */
	Model::Model* getModel();

	/**
	 * @brief Repositions the scene node using the settings of the Model instance.
	 */
	void repositionSceneNode();

	/**
	 * @brief Updates the texture by rendering one frame manually.
	 */
	virtual void updateRender();

protected:

	/**
	 * Sets the Model which should be rendered.
	 * @param model
	 * @return
	 */
	void setModel(Model::Model* model);

	Model::Model* mModel;

	virtual Ogre::MovableObject* getMovableObject();

	void model_Reloaded();

	sigc::connection mModelReloadedConnection;
	sigc::connection mModelDelayedUpdateConnection;

	void delayedUpdateRender();

};
}
}

#endif
