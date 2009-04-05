//
// C++ Interface: EntityCEGUITexture
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005
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
#ifndef EMBEROGREENTITYCEGUITEXTURE_H
#define EMBEROGREENTITYCEGUITEXTURE_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include <memory>

namespace CEGUI
{
	class Imageset;
	class Image;
	class Texture;
}

namespace EmberOgre {

class SimpleRenderContext;

namespace Model {
	class Model;
}

namespace Gui {

/**
@author Erik Hjortsberg

Useful class for rendering a single scene node to a CEGUI texture.

*/
class EntityCEGUITexture
//  : public RenderTargetListener
{
public:
    
    /**
     * Constructor.
     * @param imageSetName The name of the imageset.
     * @param width The width of the image created.
     * @param height The height of the image created.
     * @return 
     */
    EntityCEGUITexture(const std::string& imageSetName, int width, int height);

    virtual ~EntityCEGUITexture();
    
    /**
     * Gets the rendered image.
     * @return 
     */
    const CEGUI::Image* getImage() const;
    
    
    SimpleRenderContext* getRenderContext();
    
private:

	/**
	The rendered image.
	*/
	const CEGUI::Image* mImage;
	
	/**
	The main imageset, though only one image will be defined for it.
	*/
	CEGUI::Imageset* mImageSet;
	
	/**
	 *    Creates the imageset and image and sets up the rendering.
	 * @param imageSetName 
	 */
	void createImage(const std::string& imageSetName);
	
	/**
	Width and height of the image.
	*/
	int mWidth, mHeight;
		
	std::auto_ptr<SimpleRenderContext> mRenderContext;
	CEGUI::Texture* mCeguiTexture;
};


}
}

#endif
