//
// C++ Interface: SimpleRenderContext
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
#ifndef EMBEROGRESIMPLERENDERCONTEXT_H
#define EMBEROGRESIMPLERENDERCONTEXT_H

#include "EmberOgrePrerequisites.h"
#include <OgreResource.h>
#include <OgreColourValue.h>
#include <OgreTexture.h>

namespace Ember {
namespace OgreView {

class SimpleRenderContext;

/**
Responsible for making sure that the texture is rerendered when the texture resource needs to be reloaded.
*/
class SimpleRenderContextResourceLoader : public Ogre::ManualResourceLoader
{
public:
	/**
	 *    Ctor.
	 * @param renderContext The SimpleRenderContext to which this instance belongs.
	 */
	SimpleRenderContextResourceLoader(SimpleRenderContext& renderContext);


	/**
	 *    At load time the texture will be rerendered.
	 * @param resource
	 */
	virtual void loadResource (Ogre::Resource *resource);
protected:
	SimpleRenderContext& mRenderContext;
};

/**
	@author Erik Ogenvik <erik@ogenvik.org>

Useful class for rendering a single scene node.

*/
class SimpleRenderContext
{
public:

    /**
    Specifices how the camera will be positioned.
    */
    enum CameraPositioningMode
	{
		/**
		Centers the camera on the object.
		*/
		CPM_OBJECTCENTER = 0,
		/**
		Centers the camera on the center of the world.
		*/
		CPM_WORLDCENTER = 1,
		/**
		Allows free roaming.
		*/
		CPM_FREE = 2,
	};

    /**
     * Constructor.
     * @param prefix A unique prefix for the scene manager.
     * @param width The width of the image created.
     * @param height The height of the image created.
     * @return
     */
    SimpleRenderContext(const std::string& prefix, int width, int height);
    SimpleRenderContext(const std::string& prefix, Ogre::TexturePtr texturePtr);

    ~SimpleRenderContext();

    /**
     * Gets the scene node which is being rendered.
     * @return
     */
    Ogre::SceneNode* getSceneNode() const;


    /**
     * Gets the camera used for rendering.
     * @return
     */
    Ogre::Camera* getCamera() const;

    /**
     * Gets the default distance of the camera from the base, most likely somewhere where the whole scene is shown
     * @return
     */
    Ogre::Real getDefaultCameraDistance() const;

    /**
     * Sets whether the rendering should be active or not.
     * @param active
     */
    void setActive(bool active);

    /**
     * Adjusts the camera distance so that the full scene is shown
     */
    void showFull(float renderableBoundingRadius);


	/**
	 *    After you've changed the target of the camera, i.e. the object attached to the base SceneNode,
	      the camera has to be repositioned. Make sure to always call this method after updating the scene.
	 */
	void repositionCamera();


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
	 * Sets the relative camera distance. Note that this is adjusted after calling repositionCamera(). A value of 1.0 indicates the most optimal distance for showing the complete mesh.
	 * @param distance
	 */
	void setCameraDistance(Ogre::Real distance);


	/**
	 *    Gets the relative distance. 1.0 being the most optimal distance for showing the complete mesh.
	 * @return
	 */
	float getCameraDistance() const;

	/**
	 *    Sets the absolute distance of the camera.
	 * @param distance
	 */
// 	void setCameraAbsoluteDistance(Ogre::Real distance);

	/**
	 *    Gets the absolute distance in world units.
	 * @return
	 */
	float getAbsoluteCameraDistance() const;

	/**
	 * @brief Gets the rotation of the camera.
	 * @return The rotation of the camera.
	 */
	Ogre::Quaternion getCameraOrientation() const;

	/**
	 * @brief Resets the orientation of the camera.
	 */
	void resetCameraOrientation();


    Ogre::SceneManager* getSceneManager() const;

    Ogre::RenderTexture* getRenderTexture();

    Ogre::TexturePtr getTexture();

    Ogre::SceneNode* getCameraRootNode() const;

    Ogre::Viewport* getViewport() const;

    /**
     * Gets the main light.
     * @return
     */
    Ogre::Light* getLight();


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
    Sets the render texture to which the scene will be rendered. By default an instance of this class will create it's own Render Texture instance, but this allows you to use a preexisting one if you want.
    */
    void setTexture(Ogre::TexturePtr texture);

	/**
	 *    Gets the current camera positioning mode. The default is CPM_OBJECTCENTER which centers the camera on the current displayed object.
	 * @return
	 */
	CameraPositioningMode getCameraPositionMode() const;

	/**
	 *    Sets the camera positioning mode. This determines how the camera behaves.
	 * @param mode
	 */
	void setCameraPositionMode(CameraPositioningMode mode);

private:

	/**
	Main light for the scene, places a bit to the right
	*/
	Ogre::Light* mMainLight;

	/**
	Since we don't want this to be shown in the "real" world, we'll use our own scene manager
	*/
	Ogre::SceneManager* mSceneManager;

	/**
	The default distance of the camera from the base, most likely somewhere where the whole scene is shown
	*/
	Ogre::Real mDefaultCameraDistance;

	/**
	 *    Creates the image and sets up the rendering.
	 * @param prefix
	 */
	void createImage(const std::string& prefix);

	/**
	Width and height of the image.
	*/
	int mWidth, mHeight;

	/**
	The rendertexture used.
	*/
	Ogre::RenderTexture* mRenderTexture;

	/**
	 *    Creates the ogre camera.
	 * @param imageSetName
	 */
	void createCamera(const std::string& prefix);

	void setupScene(const std::string& prefix);

	/**
	The node to which the camera is attached.
	*/
	Ogre::SceneNode *mCameraNode, *mCameraPitchNode;

	/**
	The node to which the rendered entities are attched.
	*/
	Ogre::SceneNode* mEntityNode;

	/**
	The root node.
	*/
	Ogre::SceneNode* mRootNode;

	/**
	The camera used.
	*/
	Ogre::Camera* mCamera;

	Ogre::TexturePtr mTexture;

	Ogre::Viewport* mViewPort;

	SimpleRenderContextResourceLoader mResourceLoader;

	/**
	The background colour of the scene.
	*/
	Ogre::ColourValue mBackgroundColour;

	/**
	The current camera positioning mode.
	*/
	CameraPositioningMode mCameraPositionMode;

	bool mTextureOwned;

};

inline Ogre::Real SimpleRenderContext::getDefaultCameraDistance() const
{
	return mDefaultCameraDistance;
}

inline Ogre::SceneManager* SimpleRenderContext::getSceneManager() const
{
	return mSceneManager;
}

}

}

#endif
