//
// C++ Interface: MovableObjectRenderer
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
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
#ifndef EMBEROGREMOVABLEOBJECTRENDERER_H
#define EMBEROGREMOVABLEOBJECTRENDERER_H

#include <CEGUIEvent.h>

#include "components/ogre/EmberOgrePrerequisites.h"
#include "services/input/IInputAdapter.h"
#include "../SimpleRenderContext.h"
#include <OgreFrameListener.h>
namespace CEGUI
{
class GUISheet;
}

namespace EmberOgre {
namespace Gui {

class EntityCEGUITexture;

/**
Class used for rendering a single Ogre::MovableObject to a EntityCEGUITexture

@author Erik Hjortsberg
*/
class MovableObjectRenderer : public Ember::IInputAdapter, public Ogre::FrameListener
{
public:
    MovableObjectRenderer(CEGUI::Window* image);

    virtual ~MovableObjectRenderer();

    /**
     * Adapts the position of the camera so that the whole scene is shown.
     */
    void showFull();

    /**
     * Sets the distance of the camera from the Model.
     * @param distance
     */
    void setCameraDistance(float distance);

    /**
     * Gets the distance of the camera from the Model in reltive terms with 1.0 being the optimal distance to show the full model.
     * @return
     */
    float getCameraDistance();

	/**
	 *    Gets the distance of the camera from the model in world units.
	 * @return
	 */
	float getAbsoluteCameraDistance();




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
     * Gets whether the camera should be repositioned so that the full scene is shown each time the content of the scene node updates
     * Defaults to true.
     * @return
     */
    void setAutoShowFull(bool showFull);

    /**
     * Sets whether the camera should be repositioned so that the full scene is shown each time the content of the scene node updates
     * @return
     */
    bool getAutoShowFull() const;

    bool getActive() const {return mActive;}
    void setActive(bool isActive) {mActive = isActive;}

/**
---------Methods implemented from IInputAdapter
@see IInputAdapter
*/
	virtual bool injectMouseMove(const Ember::MouseMotion& motion, bool& freezeMouse);
	virtual bool injectMouseButtonUp(const Ember::Input::MouseButton& button);
	virtual bool injectMouseButtonDown(const Ember::Input::MouseButton& button);
	virtual bool injectChar(char character);
	virtual bool injectKeyDown(const SDLKey& key);
	virtual bool injectKeyUp(const SDLKey& key);


	/**
	 * Methods from Ogre::FrameListener
	 */
	virtual bool frameStarted(const Ogre::FrameEvent& event);


	/**
	 *    Gets the rotation of the entity.
	 * @return
	 */
	Ogre::Quaternion getEntityRotation();


	/**
	 *    Resets the orientation of the camera.
	 */
	void resetCameraOrientation();

	/**
	 *    Pitches the camera.
	 * @param degrees The amount of degrees to pitch.
	 */
	void pitch(Ogre::Degree degrees);

	/**
	 *    Yaws the camera.
	 * @param degrees The amount of degree to yaw.
	 */
	void yaw(Ogre::Degree degrees);

	/**
	 *    Rolls the camera.
	 * @param degrees The amount of degree to roll.
	 */
    void roll(Ogre::Degree degrees);

    /**
     * Updates the texture by rendering one frame manually.
     */
    void updateRender();

    /**
     * Sets the background colour.
     * @param colour
     */
    void setBackgroundColour(const Ogre::ColourValue& colour);
    /**
     * Sets the background colour.
     * @param red
     * @param green
     * @param blue
     * @param
     */
    void setBackgroundColour(float red, float green, float blue, float alpha);

    /**
     * Shows an axis mesh in the middle of the scene. Useful for authoring.
     */
    void showAxis();

    /**
     * Hides the axis mesh.
     */
    void hideAxis();

	/**
	 *    Gets the current camera positioning mode. The default is CPM_OBJECTCENTER which centers the camera on the current displayed object.
	 * @return
	 */
	SimpleRenderContext::CameraPositioningMode getCameraPositionMode() const;

	/**
	 *    Sets the camera positioning mode. This determines how the camera behaves.
	 * @param mode
	 */
	void setCameraPositionMode(SimpleRenderContext::CameraPositioningMode mode);

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


	/**CEGUI::StaticImage* image
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

    EntityCEGUITexture* mTexture;
    /**
    If true, the input will be caught when the user clicks on the image, allowing for rotation of the model.
    */
    bool mIsInputCatchingAllowed;

	/**
	used to decide if the camera should be repositioned so that the full scene is shown each time the content of the scene node updates
	*/
	bool mAutoShowFull;

	CEGUI::Window* mImage;

	virtual Ogre::MovableObject* getMovableObject() = 0;

	bool mActive;

	Ogre::Entity* mAxisEntity;
	Ogre::SceneNode* mAxesNode;

};
}
}

#endif
