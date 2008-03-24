//
// C++ Implementation: Compass
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Compass.h"

#include <Ogre.h>

#include <CEGUIImageset.h>
#include <CEGUIImage.h>
#include <CEGUIRect.h>

#include "../terrain/Map.h"

#include "services/logging/LoggingService.h"

using namespace EmberOgre::Terrain;

namespace EmberOgre {

namespace Gui {

Compass::Compass()
: mMap(new Map()), mViewImage(0)
{
	mMap->initialize();
	AssetsManager assetsManager;
	mTexturePair = assetsManager.createTextureImage(mMap->getTexture(), "CompassMap");
	int halfOffset = mMap->getTexture()->getWidth() * 0.25;
	mTexturePair.getTextureImageset()->defineImage("view",  CEGUI::Rect(halfOffset, halfOffset, mMap->getTexture()->getWidth() - halfOffset, mMap->getTexture()->getWidth() - halfOffset), CEGUI::Point(0,0));
	mViewImage = &mTexturePair.getTextureImageset()->getImage("view");
}


Compass::~Compass()
{
}

Terrain::Map& Compass::getMap()
{
	return *mMap;
}

void Compass::reposition(float x, float y)
{
// 	S_LOG_VERBOSE("pos x: " << x << " y: " << y);
	mMap->getView().reposition(Ogre::Vector2(x, y));
	const Ogre::TRect<float>& viewBounds(mMap->getView().getRelativeViewBounds());
	CEGUI::Rect& rect = const_cast<CEGUI::Rect&>(mViewImage->getSourceTextureArea());
	int textureWidth = mTexturePair.getOgreTexture()->getWidth();
	rect.setSize(CEGUI::Size(textureWidth * 0.5, textureWidth * 0.5));
	const Ogre::TRect<float> viewRect(mMap->getView().getRelativeViewBounds());
// 	const Ogre::Vector2& viewPos(mMap->getView().getRelativeViewPosition());
	rect.setPosition(CEGUI::Point(textureWidth * viewRect.left, textureWidth * viewRect.top));
}

const CEGUI::Image* Compass::getViewImage() 
{
	return mViewImage;
}


}

}
