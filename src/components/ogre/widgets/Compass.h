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
@brief Helper class for the compass widget.

@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
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
    
    /**
     * @brief Refreshes the compass rendering.
     */
    void refresh();
    
    
protected:
	
	/**
	 * @brief The terrain map instance which is responsible for creating and updating the underlying map.
	 * This is owned by this class.
	 */
	std::auto_ptr<Terrain::Map> mMap;
	
	/**
	 * @brief The compass implementation, responsible for the actual transformation of the map texture info something which can be presented in the gui.
	 */
	ICompassImpl* mCompassImpl;
};

class ICompassImpl
{
friend class Compass;
public:
	ICompassImpl();
    virtual void reposition(float x, float y) = 0;
    virtual void rotate(const Ogre::Degree& degree) = 0;
    
    /**
     * @brief Refreshes the compass rendering.
     */
    virtual void refresh() = 0;

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
    /**
     * @brief Refreshes the compass rendering.
     */
    virtual void refresh();
    
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
    /**
     * @brief Refreshes the compass rendering.
     */
    virtual void refresh();
    
protected:

	Ogre::Overlay* mCompassOverlay;
	Ogre::MaterialPtr mCompassMaterial;
	virtual void _setCompass(Compass* compass);
	
};

/**
@brief A compass implementation which uses a compositor to create the rounded map image.
The main problem with CEGUI is that there's no easy way to apply transparence to a dynamic render texture. By using a compositor we can however render the /ui/compass material, which will use an alpha mask to remove the border from the map texture, providing a rounded shape.
This implementation will only provide the rounded map texture. It's up to other components to then provide further functionality. This can perhaps be done through CEGUI.
Note that we use a separate scene manager, owned by this class, for this.
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
     * @brief Refreshes the compass rendering.
     */
    virtual void refresh();
    
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
	
	/**
	 * @brief The camera used for rendering.
	 */
	Ogre::Camera* mCamera;
	
	/**
	 * @brief The scene manager used for rendering. We use a completely separate scene manager to avoid interference with other scenes.
	 */
	Ogre::SceneManager* mSceneManager;
	
	/**
	 * @brief The main viewport used by our camera.
	 */
	Ogre::Viewport* mViewport;
	
	/**
	 * @brief The texture unit state onto which the map is projected. This will be scrolled to simulate movement.
	 */
	Ogre::TextureUnitState* mCompassMaterialMapTUS;
};


/**
@brief An anchor to which the compass is attached.

When the anchor moves the compass should move too. Any instance of this class is self contained and will make sure to listen for updates and frame events and then tell the compass to update itself accordingly.
An instance of this class can't be created directly, instead use on of the friend classes such as CompassCameraAnchor or CompassSceneNodeAnchor.
@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class CompassAnchor
: public Ogre::FrameListener
{
friend class CompassCameraAnchor;
friend class CompassSceneNodeAnchor;
friend class CompassThirdPersonCameraAnchor;
public:

	
	/**
	 * @brief Dtor.
	 */
	virtual ~CompassAnchor();

	/**
	 * Methods from Ogre::FrameListener
	 */
	bool frameStarted(const Ogre::FrameEvent& event);

protected:
	/**
	 * @brief Ctor.
	 * @param compass The compass instance to which this anchor belongs.
	 * @param position A reference to the position of the anchor. It's required that this instance survives the anchor, since it's through this that the anchor every frame checks for updates.
	 * @param orientation A reference to the orientation of the anchor. It's required that this instance survives the anchor, since it's through this that the anchor every frame checks for updates.
	 */
	CompassAnchor(Compass& compass, const Ogre::Vector3& position, const Ogre::Quaternion& orientation);
	
	/**
	 * @brief The compass instance to which this anchor belongs to.
	 * The anchor will make sure to tell the compass to update itself when needed.
	 */
	Compass& mCompass;
	
	/**
	 * @brief The previous X position of the anchor in Ogre space. This is used for making sure that we only tell the compass to update itself when the anchor has moved.
	 */
	float mPreviousX;
	
	/**
	 * @brief The previous Z position of the anchor in Ogre space. This is used for making sure that we only tell the compass to update itself when the anchor has moved.
	 */
	float mPreviousZ;
	
	/**
	 * @brief A reference to the position of the anchor.
	 */
	const Ogre::Vector3& mPosition;
	
	/**
	 * @brief A reference to the orienatation of the anchor.
	 */
	const Ogre::Quaternion& mOrientation;
};

/**
@brief An anchor implementation which will attach the anchor to a certain Ogre::Camera instance.
@see CompassAnchor

@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class CompassCameraAnchor
{
public:

	/**
	 * @brief Ctor.
	 * @param compass The compass to which we want to attach the anchor.
	 * @param camera The Ogre::Camera instance which we want the anchor to follow. When the camera moves the compass will move.
	 */
	CompassCameraAnchor(Compass& compass, Ogre::Camera* camera);
	
	/**
	 * @brief Dtor.
	 */
	virtual ~CompassCameraAnchor();

protected:
	
	/**
	 * @brief The anchor instance which does the heavy lifting.
	 */
	CompassAnchor mAnchor;
	
	/**
	 * @brief The camera to which an instance of this is attached.
	 */
	Ogre::Camera* mCamera;
};


/**
@brief An anchor implementation which will attach the anchor to a certain Ogre::SceneNode instance.
@see CompassAnchor

@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class CompassSceneNodeAnchor
{
public:

	/**
	 * @brief Ctor.
	 * @param compass The compass to which we want to attach the anchor.
	 * @param sceneNode The Ogre::SceneNode instance which we want the anchor to follow. When the scene node moves the compass will move.
	 */
	CompassSceneNodeAnchor(Compass& compass, Ogre::SceneNode* sceneNode);
	
	/**
	 * @brief Dtor.
	 */
	virtual ~CompassSceneNodeAnchor();

protected:

	/**
	 * @brief The anchor instance which does the heavy lifting.
	 */
	CompassAnchor mAnchor;
	
	/**
	 * @brief The scene node to which an instance of this is attached.
	 */
	Ogre::SceneNode* mSceneNode;
};

/**
@brief An anchor implementation which will attach the anchor to both a certain Ogre::Camera instance and a scene node.
The camera will be used for determining the direction of the compass and the scene node will be used to determine the position. This anchor is mainly useful for thir person cameras.
@see CompassAnchor

@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class CompassThirdPersonCameraAnchor
{
public:

	/**
	 * @brief Ctor.
	 * @param compass The compass to which we want to attach the anchor.
	 * @param camera The Ogre::Camera instance which we want the anchor to follow. When the camera moves the compass will move.
	 */
	CompassThirdPersonCameraAnchor(Compass& compass, Ogre::Camera* camera, Ogre::SceneNode* node);
	
	/**
	 * @brief Dtor.
	 */
	virtual ~CompassThirdPersonCameraAnchor();

protected:
	
	/**
	 * @brief The anchor instance which does the heavy lifting.
	 */
	CompassAnchor mAnchor;
	
	/**
	 * @brief The camera to which an instance of this is attached.
	 */
	Ogre::Camera* mCamera;
	
	/**
	 * @brief The scene node to which an instance of this is attached.
	 */
	Ogre::SceneNode* mSceneNode;
};



}

}

#endif
