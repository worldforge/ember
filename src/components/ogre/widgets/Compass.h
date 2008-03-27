//
// C++ Interface: Compass
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
#ifndef EMBEROGRE_GUICOMPASS_H
#define EMBEROGRE_GUICOMPASS_H

#include <memory>
#include "AssetsManager.h"
#include <OgreMaterial.h>
#include <OgreFrameListener.h>

namespace EmberOgre {

namespace Terrain
{
class Map;
class MapView;
}

namespace Gui {


class ICompassImpl;
/**
Helper class for the compass widget.

	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class Compass
{
public:
    Compass(ICompassImpl* compassImpl);

    virtual ~Compass();
    
    Terrain::Map& getMap();
    
    void reposition(float x, float y);
    void rotate(const Ogre::Degree& degree);
    void rotate(const Ogre::Radian& radian);
    
    
protected:

	std::auto_ptr<Terrain::Map> mMap;
	ICompassImpl* mCompassImpl;
};

class ICompassImpl
{
friend class Compass;
public:
	ICompassImpl();
    virtual void reposition(float x, float y) = 0;
    virtual void rotate(const Ogre::Degree& degree) = 0;

protected:

	Terrain::Map* mMap;
	
	void setCompass(Compass* compass);
	
	virtual void _setCompass(Compass* compass) = 0;

	Compass* mCompass;
};

class CEGUICompassImpl : ICompassImpl
{
public:
	CEGUICompassImpl();
	virtual ~CEGUICompassImpl();
    const CEGUI::Image* getViewImage();
    
    virtual void reposition(float x, float y) ;
    virtual void rotate(const Ogre::Degree& degree);
    
protected:
	const CEGUI::Image* mViewImage;
	TexturePair mTexturePair;
	virtual void _setCompass(Compass* compass);
};

class OverlayCompassImpl : ICompassImpl
{
public:
	OverlayCompassImpl();
	virtual ~OverlayCompassImpl();
	
    virtual void reposition(float x, float y);
    virtual void rotate(const Ogre::Degree& degree);
    
protected:

	Ogre::Overlay* mCompassOverlay;
	Ogre::MaterialPtr mCompassMaterial;
	virtual void _setCompass(Compass* compass);
	
};


class CompassAnchor
: public Ogre::FrameListener
{
friend class CompassCameraAnchor;
friend class CompassSceneNodeAnchor;
public:

	
	virtual ~CompassAnchor();

	/**
	 * Methods from Ogre::FrameListener
	 */
	bool frameStarted(const Ogre::FrameEvent& event);

protected:
	CompassAnchor(Compass& compass, const Ogre::Vector3& position, const Ogre::Quaternion& orientation);
	
	Compass& mCompass;
	float mPreviousX, mPreviousZ;
	const Ogre::Vector3& mPosition;
	const Ogre::Quaternion& mOrientation;
};

class CompassCameraAnchor
{
public:

	CompassCameraAnchor(Compass& compass, Ogre::Camera* camera);
	
	virtual ~CompassCameraAnchor();

protected:

	CompassAnchor mAnchor;
	Ogre::Camera* mCamera;
};


class CompassSceneNodeAnchor
{
public:

	CompassSceneNodeAnchor(Compass& compass, Ogre::SceneNode* sceneNode);
	
	virtual ~CompassSceneNodeAnchor();

protected:
	CompassAnchor mAnchor;
	Ogre::SceneNode* mSceneNode;
};


}

}

#endif
