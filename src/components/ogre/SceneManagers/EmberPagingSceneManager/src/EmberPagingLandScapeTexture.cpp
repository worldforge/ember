//
// C++ Implementation: EmberPagingLandScapeTexture
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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

#include "EmberPagingLandScapeTexture.h"
#include "EmberOgre.h"
#include "TerrainPage.h"
#include "TerrainGenerator.h"

namespace EmberOgre
{
	

	EmberPagingLandScapeTexture::EmberPagingLandScapeTexture( Ogre::PagingLandScapeTextureManager *textureMgr ): Ogre::PagingLandScapeTexture(textureMgr)
	{
	}
	
	EmberPagingLandScapeTexture::~EmberPagingLandScapeTexture( void )
	{
	}


	void EmberPagingLandScapeTexture::_setPagesize( void )
	{
	}
	
	void EmberPagingLandScapeTexture::_clearData( void )
	{
	}


	Ogre::PagingLandScapeTexture* EmberPagingLandScapeTexture::newTexture( )
	{
		return new EmberPagingLandScapeTexture(mParent);
	}
	
	bool EmberPagingLandScapeTexture::TextureRenderCapabilitesFullfilled()
	{
		//TODO: check for stuff here
		return true;
	}

	void EmberPagingLandScapeTexture::_loadMaterial( )
	{
		TerrainGenerator* terrainGenerator = EmberOgre::getSingleton().getTerrainGenerator();
		TerrainPage* page = terrainGenerator->getTerrainPage(Ogre::Vector2(mDataX, mDataZ));
		assert(page);
		if (page) {
			mMaterial = page->getMaterial();
		}
		
	}

	void EmberPagingLandScapeTexture::_unloadMaterial()
	{
		S_LOG_VERBOSE("Unloading terrain material.");
	}
	
	
}
