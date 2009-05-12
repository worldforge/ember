//
// C++ Interface: Map
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2008
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
#ifndef EMBEROGRE_TERRAINMAP_H
#define EMBEROGRE_TERRAINMAP_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include <vector>
#include <OgreColourValue.h>
#include <OgreVector2.h>
#include <OgreCommon.h>
#include <OgreTexture.h>
#include <OgreSceneManager.h>

namespace EmberOgre {

namespace Terrain {

class Map;

/**
@brief Provides lightning for the map rendering.

We don't want to use the regular scene lightning since then we won't see anything at all during the night. This class takes care of setting up a light which will provide a good sunny day for the map rendering.

You don't use this directly when you want to render however, instead you use an instance of MapCameraLightningInstance.

@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class MapCameraLightning
{
public:
	/**
	 * @brief Ctor.
	 * The light will automatically be created in the constructor.
	 * @param sceneManager The scene manager which is used for rendering.
	 */
	MapCameraLightning(Ogre::SceneManager& sceneManager);

	/**
	 * @brief Dtor.
	 */
	virtual ~MapCameraLightning();

	/**
	 * @brief Gets the light which is used for rendering. This is a yellow, bright light in the middle of the sky.
	 * @return The main light.
	 */
	Ogre::Light* getLight();

	/**
	 * @brief Gets the scene manager.
	 * @return The scene manager.
	 */
	Ogre::SceneManager& getSceneManager();

protected:
	/**
	 * @brief The light, owned by this instance.
	 */
	Ogre::Light* mLight;

	/**
	 * @brief The scene manager to which everything belongs.
	 */
	Ogre::SceneManager& mSceneManager;
};

/**
@brief An instance of map specific lightning, using RAII to make sure that the lightning is correctly reset after rendering is complete.

Use an instance of this during your rendering to make sure that the lightning is correctly set up, and then reset afterwards.

@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/class MapCameraLightningInstance {
public:
	/**
	 * @brief Ctor.
	 * @param lightning The lightning.
	 */
	MapCameraLightningInstance(MapCameraLightning& lightning);

	/**
	 * @brief Dtor.
	 */
	~MapCameraLightningInstance();

protected:
	typedef std::vector<Ogre::MovableObject*> LightStore;

	/**
	 * @brief The lightning.
	 */
	MapCameraLightning& mLightning;

	/**
	 * @brief A store of visible lights which will be disabled when this instance is created, and enabled when it's destroyed.
	 */
	LightStore mVisibleLights;

	/**
	 * @brief The ambient colour of the scene before this instance is created, which will be reset when it's destroyed.
	 */
	Ogre::ColourValue mAmbientColour;
};

/**
	@brief Responsible for handling the camera used to render the terrain overhead map.
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>

*/
class MapCamera
{
public:
	MapCamera(Map& map, Ogre::SceneManager* manager);
	virtual ~MapCamera();

	void render();

	void setDistance(float distance);
	float getDistance() const;

	void reposition(const Ogre::Vector2& pos);
	/**
	 * @brief Gets the current 2d position of the camera, in world space.
	 * @return The current position of the camera, in world space.
	 */
	const Ogre::Vector2 getPosition() const;


	void setRenderTarget(Ogre::RenderTarget* renderTarget);

protected:
	Map& mMap;

	Ogre::Camera* mCamera;
	Ogre::Viewport* mViewport;
	float mDistance;

	MapCameraLightning mLightning;
};


/**
	@brief Represents a sub view of the map.
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>

*/
class MapView
{
public:
	MapView(Map& map, MapCamera& mapCamera);

	/**
	 * @brief Reposition the view.
	 * If the view after the reposition will be outside of the current rendered map, the map will be repositioned and rendered. This happens automatically.
	 * @param pos The world position in ogre space to where we want to reposition the view.
	 * @return True if the map needed to be repositioned and rerendered.
	 */
	bool reposition(const Ogre::Vector2& pos);

	const Ogre::TRect<float>& getRelativeViewBounds() const;
	const Ogre::Vector2& getRelativeViewPosition() const;

	/**
	 * @brief Recalculates the bounds. Call this whenever you've altered the scaling or repositioned the camera.
	 * This will also be called internally whenever the camera needs to be repositioned through a call to MapView::reposition.
	 */
	void recalculateBounds();

protected:


	Ogre::TRect<int> mFullBounds;
	Ogre::TRect<float> mVisibleRelativeBounds;
	Ogre::Vector2 mRelativeViewPosition;

	Map& mMap;
	MapCamera& mMapCamera;

	float mViewSize;

};

/**
	An overhead map of the terrain, rendered into a texture.
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class Map{
public:
    Map();

    virtual ~Map();

    void initialize();

    Ogre::TexturePtr getTexture() const;

    /**
     * @brief Gets the render texture into which the map is being rendered.
     * This is the same texture as the one you will get from getTexture(), but this accesses the more low level rendering structure, allowing you to access the actual ViewPort.
     * If you haven't called initialize() yet this will return a null pointer.
     * @return A pointer to the render texture being used for rendering the map, or null if no such has been created yet.
     */
    Ogre::RenderTexture* getRenderTexture() const;

    void render();
    void reposition(const Ogre::Vector2& pos);
    void reposition(float x, float y);

	void setDistance(float distance);
	float getDistance() const;

	/**
	 * @brief Gets the resolution in meters per pixel.
	 * @return The resolution in meters per pixel.
	 */
	float getResolution() const;

	/**
	 * @brief Sets the resolution of the map.
	 * The map will be rerendered after the resolution has been changed.
	 * @param metersPerPixel The resolution of the map in pixels per meter.
	 */
	void setResolution(float metersPerPixel);

	/**
	 * @brief Gets the resolution in meters of the map.
	 * @return The size of one side of the map in meters.
	 */
	float getResolutionMeters() const;

	MapView& getView();


protected:

	void setupCamera();
	void createTexture();


	Ogre::TexturePtr mTexture;
	Ogre::RenderTexture* mRenderTexture;

	unsigned int mTexturePixelSize;
	float mMetersPerPixel;

	MapCamera mCamera;
	MapView mView;

};



/**
	By using an instance of this every time you want to alter the scene manager for a certain render operation, you can make sure that the scene manager is returned to its initial state after the render operation is complete, even if something goes wrong.
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class RenderingInstance
{
public:
	RenderingInstance(Ogre::SceneManager* manager);
	virtual ~RenderingInstance();

protected:
	Ogre::SceneManager* mManager;

	Ogre::FogMode mFogMode;
	Ogre::ColourValue mFogColour;
	Ogre::Real mFogDensity;
	Ogre::Real mFogStart;
	Ogre::Real mFogEnd;

	Ogre::SceneManager::SpecialCaseRenderQueueMode mSpecialCaseRenderQueueMode;

};


}

}

#endif
