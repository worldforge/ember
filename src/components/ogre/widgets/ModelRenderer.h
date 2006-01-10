//
// C++ Interface: ModelRenderer
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
#ifndef EMBEROGREMODELRENDERER_H
#define EMBEROGREMODELRENDERER_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include "../input/IInputAdapter.h"

#include <CEGUIEvent.h> 

namespace CEGUI
{
	class StaticImage;
	
}

namespace EmberOgre {

namespace Model {
	class Model;
}

class EntityCEGUITexture;

/**

Renders a single Model to a CEGUI texture. Useful for stuff like inventory or preview of models.

@author Erik Hjortsberg
*/
class ModelRenderer : public IInputAdapter
{
public:

	/**
	 *    Creates a one shot image. This is rather inexpensive, since there's no need for rendering each frame, but it disallows updates to the image.
	 * @param image 
	 */
	static void OneShotRender(CEGUI::StaticImage* image);

    /**
     * Constructir
     * @param image A valid CEGUI::StaticImage, to which the Model will be rendered.
     * @return 
     */
    ModelRenderer(CEGUI::StaticImage* image);

    ~ModelRenderer();
    
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
     * Adapts the position of the camera so that the whole Model is shown.
     */
    void showFullModel();
    
    /**
     * Sets the distance of the camera from the Model.
     * @param distance 
     */
    void setCameraDistance(float distance);
    
    /**
     * Gets the distance of the camera from the Model.
     * @return 
     */
    float getCameraDistance();
    
    
    /**
     * Returns whether input catching (and also rotation of the model) is allowed.
     * Defaults to true.
     * @return 
     */
    bool getIsInputCatchingAllowed() const;
    
    /**
     * Sets whether input catching (and also rotation of the model) is allowed.
     * @param allowed 
     */
    void setIsInputCatchingAllowed(bool allowed);
    
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
     * Sets the Model which should be rendered.
     * @param model 
     * @return 
     */
    void setModel(Model::Model* model);
	
	CEGUI::StaticImage* mImage;
    Model::Model* mModel;
    EntityCEGUITexture* mTexture;
    /**
    If true, the input will be caught when the user clicks on the image, allowing for rotation of the model.
    */
    bool mIsInputCatchingAllowed;

};

}

#endif
