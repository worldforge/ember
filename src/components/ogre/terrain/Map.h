//
// C++ Interface: Map
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2008
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

#include <Ogre.h>

namespace EmberOgre {

namespace Terrain {

class Map;

/**
	Responsible for handling the camera used to render the terrain overhead map.
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>

*/
class MapCamera
{
public:
	MapCamera(Map& map, Ogre::SceneManager* manager);
	virtual ~MapCamera();
	
	void render();
	
	void setDistance(float distance);
	float getDistance() const;
	
    void reposition(Ogre::Vector2 pos);

	
	void setRenderTarget(Ogre::RenderTarget* renderTarget);
	
protected:
	Map& mMap;
	
	Ogre::Camera* mCamera;
	Ogre::Viewport* mViewport;
	float mDistance;
};


/**
	Represents a sub view of the map.
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>

*/
class MapView
{
public:
	MapView(Map& map, MapCamera& mapCamera);
	
	bool reposition(Ogre::Vector2 pos);
	
	const Ogre::TRect<float>& getRelativeViewBounds() const;
	const Ogre::Vector2& getRelativeViewPosition() const;

protected:

	
	Ogre::TRect<int> mFullBounds;
	Ogre::TRect<float> mVisibleRelativeBounds;
	Ogre::Vector2 mRelativeViewPosition;
	
	Map& mMap;
	MapCamera& mMapCamera;
	
	float mViewSize;
	int mViewSizeMeters;
	

};

/**
	An overhead map of the terrain, rendered into a texture.
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
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
    void reposition(Ogre::Vector2 pos);
    void reposition(float x, float y);
    
	void setDistance(float distance);
	float getDistance() const;
	
	float getResolution() const;
	float getResolutionMeters() const;
	
	MapView& getView();
    
    
protected:

	void setupCamera();
	void createTexture();

	
	Ogre::TexturePtr mTexture;
	Ogre::RenderTexture* mRenderTexture;
	
	unsigned int mResolutionMeters;
	unsigned int mTexturePixelSize;
	
	MapCamera mCamera;
	MapView mView;

};


/**
	By using an instance of this every time you want to alter the scene manager for a certain render operation, you can make sure that the scene manager is returned to its initial state after the render operation is complete, even if something goes wrong.
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
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
