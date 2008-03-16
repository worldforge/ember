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
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class Map{
public:
    Map();

    virtual ~Map();
    
    void initialize();
    
    Ogre::TexturePtr getTexture() const;
    
    void render();
    void reposition(Ogre::Vector2 pos);
    void reposition(float x, float y);
    
	void setDistance(float distance);
	float getDistance() const;
    
    
protected:

	void setupCamera();
	void createTexture();

	MapCamera mCamera;
	
	Ogre::TexturePtr mTexture;
	Ogre::RenderTexture* mRenderTexture;
};


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
