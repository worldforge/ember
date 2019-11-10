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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "OnePixelMaterialGenerator.h"

#include <OgreTexture.h>
#include <OgreDataStream.h>

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

	auto* texture = static_cast<Ogre::Texture*>(resource);
	static Ogre::uchar data[3] = {0xFF, 0x7F, 0x7F};

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
