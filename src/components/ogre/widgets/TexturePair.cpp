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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TexturePair.h"

namespace EmberOgre
{

namespace Gui
{

TexturePair::TexturePair(Ogre::TexturePtr ogreTexture, const CEGUI::Image* textureImage, CEGUI::Imageset* textureImageset) :
	mOgreTexture(ogreTexture), mTextureImage(textureImage), mTextureImageset(textureImageset)
{
}

TexturePair::TexturePair() :
	mOgreTexture(0), mTextureImage(0), mTextureImageset(0)
{
}

Ogre::TexturePtr TexturePair::getOgreTexture() const
{
	return mOgreTexture;
}

const CEGUI::Image* TexturePair::getTextureImage() const
{
	return mTextureImage;
}

CEGUI::Imageset* TexturePair::getTextureImageset() const
{
	return mTextureImageset;
}

bool TexturePair::hasData()
{
	return mTextureImageset != 0;
}

}

}
