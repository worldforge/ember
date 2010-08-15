/*
 Copyright (C) 2010 erik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef TEXTUREPAIR_H_
#define TEXTUREPAIR_H_

#include <OgreTexture.h>

namespace CEGUI
{
class OgreCEGUITexture;
class Imageset;
class Image;
class Texture;
}

namespace EmberOgre
{

namespace Gui
{

class TexturePair
{
public:
	TexturePair(const Ogre::TexturePtr& mOgreTexture, const CEGUI::Image* mTextureImage, CEGUI::Imageset* mTextureImageset);
	TexturePair();

	Ogre::TexturePtr getOgreTexture() const;
	const CEGUI::Image* getTextureImage() const;
	CEGUI::Imageset* getTextureImageset() const;

	bool hasData();

protected:
	Ogre::TexturePtr mOgreTexture;
	const CEGUI::Image* mTextureImage;
	CEGUI::Imageset* mTextureImageset;
};

}

}

#endif /* TEXTUREPAIR_H_ */
