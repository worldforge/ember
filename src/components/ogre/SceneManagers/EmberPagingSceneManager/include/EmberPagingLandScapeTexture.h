//
// C++ Interface: EmberPagingLandScapeTexture
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

#ifndef EMBERPAGINGLANDSCAPETEXTURE_H
#define EMBERPAGINGLANDSCAPETEXTURE_H

#include "../../../EmberOgrePrerequisites.h"
#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeTexture.h"

namespace EmberOgre
{

/**
Ember uses it's own system for generating the textures and materials needed for the terrain. This class takes care of interfacing with the Ember terrain engine and creating the correct material and textures.

The PagingLandscape SceneManager comes with a whole set of different texture classes by default. However, we'll never use any of them and instead always use this in all instances.

@author Erik Hjortsberg
*/
class EmberPagingLandScapeTexture : public Ogre::PagingLandScapeTexture
{

public:
 	EmberPagingLandScapeTexture(Ogre::PagingLandScapeTextureManager* pageMgr);
	virtual ~EmberPagingLandScapeTexture();

	virtual Ogre::String getName() const { return Ogre::String("EmberTexture"); }

	virtual Ogre::PagingLandScapeTexture* newTexture();
	virtual bool isMaterialSupported(bool recursive = true);
	void setOptions();

protected:
	virtual void _loadMaterial();
	virtual void _unloadMaterial();
};
	
}
#endif
