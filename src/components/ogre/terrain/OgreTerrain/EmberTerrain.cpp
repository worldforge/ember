/*
 Copyright (C) 2013 Erik Ogenvik

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

#include "EmberTerrain.h"

namespace Ember
{
namespace OgreView
{
namespace Terrain
{

EmberTerrain::EmberTerrain(Ogre::SceneManager* sm)
: Ogre::Terrain(sm)
{
	//This is a hack to prevent the Terrain class from creating blend map textures.
	//Since we provide our own material with its own blend maps we don't want the
	//Ogre::Terrain instance creating new ones.
	//Since most methods in the base class aren't virtual we can't override the
	//createGPUBlendTextures() method. Instead we have to trick it into thinking
	//that all textures already have been created.
	//This of course means that we can't touch _any_ method which in some way interacts
	//with the layers and blend maps.
	mBlendTextureList.push_back(Ogre::TexturePtr());

}

EmberTerrain::~EmberTerrain()
{
	//Remove the fake blend map texture, else the base class will try to delete it.
	mBlendTextureList.clear();
}

}
}
}
