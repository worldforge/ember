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

/**
@brief A compass implementation which uses a compositor to create the rounded map image.
The main problem with CEGUI is that there's no easy way to apply transparence to a dynamic render texture. By using a compositor we can however render the /ui/compass material, which will use an alpha mask to remove the border from the map texture, providing a rounded shape.
This implementation will only provide the rounded map texture. It's up to other components to then provide further functionality. This can perhaps be done through CEGUI.
@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class CompositorCompassImpl : ICompassImpl
{
public:

	/**
	 * @brief Ctor.
	 */
	CompositorCompassImpl();
	
	/**
	 * @brief Dtor.
	 * The render texture which this instance owns will be destroyed along with this instance.
	 */
	virtual ~CompositorCompassImpl();
	
    /**
     * @copydoc ICompassImpl::reposition
     */
    virtual void reposition(float x, float y);
    
    /**
     * @copydoc ICompassImpl::rotate
     */
    virtual void rotate(const Ogre::Degree& degree);
    
	/**
	* @brief Gets the texture onto which the compass is rendered.
	* @return The texture pointer.
	*/
	Ogre::TexturePtr getTexture();

protected:
	/**
	@brief The texture into which the final compass texture will be rendered.
	*/
	Ogre::TexturePtr mTexture;
	
	/**
	@brief The render texture representation of mTexture.
	*/
	Ogre::RenderTexture* mRenderTexture;
	
	/**
	@brief The compositor used for rendering our full screen quad.
	*/
	Ogre::CompositorInstance* mCompositor;
	
	/**
	@brief The compass material used in the rendering. By changing the offset of the first texture we can simulate the terrain being scrolled.
	*/
	Ogre::MaterialPtr mCompassMaterial;

	/**
	 * @brief When the owner compass instance is set, the render texture will be created and the compositior registered.
	 * @param compass 
	 */
	virtual void _setCompass(Compass* compass);
	
	Ogre::Camera* mCamera;
	Ogre::SceneManager* mSceneManager;
	Ogre::Viewport* mViewport;
	Ogre::TextureUnitState* mCompassMaterialMapTUS;
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
