/*
 Copyright (C) 2014 Erik Ogenvik

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

#include "OnePixelMaterialGenerator.h"

#include <OgreTexture.h>
#include <OgreImage.h>

namespace Ember
{
namespace OgreView
{
namespace Terrain
{
namespace Techniques
{
void OnePixelMaterialGenerator::loadResource(Ogre::Resource* resource)
{

	Ogre::Texture* texture = static_cast<Ogre::Texture*>(resource);
	static char data[3];
	data[0] = 0xFF;
	data[1] = 0x7F;
	data[2] = 0x7F;

	Ogre::DataStreamPtr stream(new Ogre::MemoryDataStream(&data, 3, false, true));
	Ogre::Image image;
	image.loadRawData(stream, 1,1,1, Ogre::PF_R8G8B8);
	Ogre::ConstImagePtrList images({&image});

	texture->_loadImages(images);
}

}
}
}
}
