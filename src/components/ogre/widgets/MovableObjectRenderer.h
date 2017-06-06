//
// C++ Interface: MovableObjectRenderer
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
#ifndef EMBEROGREMOVABLEOBJECTRENDERER_H
#define EMBEROGREMOVABLEOBJECTRENDERER_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include "components/ogre/SimpleRenderContext.h"
#include <CEGUI/Event.h>
#include <OgreFrameListener.h>
namespace CEGUI
{
class GUISheet;
}

namespace Ember
{
namespace OgreView
{
namespace Gui
{

class EntityCEGUITexture;
class CEGUIWindowUpdater;

/**
 Class used for rendering a single Ogre::MovableObject to a EntityCEGUITexture

 @author Erik Ogenvik
 */
class MovableObjectRenderer: public Ogre::FrameListener
{
public:
	MovableObjectRenderer(CEGUI::Window* image, const std::string& name);

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
	 * Gets the distance of the camera from the Model in relative terms with 1.0 being the optimal distance to show the full model.
	 * @return
	 */
	float getCameraDistance();

	/**
	 *    Gets the distance of the camera from the model in world units.
	 * @return
	 */
	float getAbsoluteCameraDistance();

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

	bool getActive() const
	{
		return mActive;
	}
	void setActive(bool isActive)
	{
		mActive = isActive;
	}

	/**
	 * Methods from Ogre::FrameListener
	 */
	virtual bool frameStarted(const Ogre::FrameEvent& event);

	/**
	 * @brief Gets the rotation of the entity.
	 * @return The rotation of the entity.
	 */
	Ogre::Quaternion getEntityRotation() const;

	/**
	 * @brief Sets the rotation of the entity.
	 * In reality it will set the rotation of the camera.
	 * @param rotation The entity rotation.
	 */
	void setEntityRotation(const Ogre::Quaternion& rotation);

	/**
	 * @brief Gets the translation of the entity.
	 * @return The translation of the entity.
	 */
	Ogre::Vector3 getEntityTranslation() const;

	/**
	 * @brief Sets the translation of the entity.
	 * @param translation The new translation
	 */
	void setEntityTranslation(const Ogre::Vector3& translation);

	/**
	 *    Resets the orientation of the camera.
	 */
	void resetCameraOrientation();

	/**
	 *    Pitches the camera.
	 * @param degrees The amount of degrees to pitch.
	 */
	void pitchCamera(Ogre::Degree degrees);

	/**
	 *    Yaws the camera.
	 * @param degrees The amount of degree to yaw.
	 */
	void yawCamera(Ogre::Degree degrees);

	/**
	 *    Rolls the camera.
	 * @param degrees The amount of degree to roll.
	 */
	void rollCamera(Ogre::Degree degrees);

	/**
	 * @brief Updates the texture by rendering one frame manually.
	 */
	virtual void updateRender();

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

	/**
	 * @brief Accessor for the entity texture.
	 * @return The entity texture managed by this object.
	 */
	EntityCEGUITexture& getEntityTexture();

protected:

	/**
	 * @brief Rescale the axis marker so that it's of a suitable size compared to the moveable object being shown.
	 *
	 * Call this whenever the movable object has changed.
	 */
	void rescaleAxisMarker();

	virtual float getMovableBoundingRadius() = 0;

	/**
	 * @brief Change the aspect ratio of the render texture when the image is resized.
	 * @param e
	 * @return
	 */
	bool image_Sized(const CEGUI::EventArgs& e);

	EntityCEGUITexture* mTexture;


	/**
	 * @brief used to decide if the camera should be repositioned so that the full scene is shown each time the content of the scene node updates
	 */
	bool mAutoShowFull;

	CEGUI::Window* mImage;

	bool mActive;

	Ogre::Entity* mAxisEntity;
	Ogre::SceneNode* mAxesNode;

	/**
	 * @brief Handles the invalidation of the Window whenever the render texture is updated.
	 *
	 * @note This class shouldn't be needed as there's a way with CEGUI 0.7 now to let CEGUI directly use a Ogre render texture.
	 */
	CEGUIWindowUpdater* mWindowUpdater;

};
}
}

}

#endif
