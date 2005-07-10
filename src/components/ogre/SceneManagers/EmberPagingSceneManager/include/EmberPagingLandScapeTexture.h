//
// C++ Interface: EmberPagingLandScapeTexture
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

#ifndef EMBERPAGINGLANDSCAPETEXTURE_H
#define EMBERPAGINGLANDSCAPETEXTURE_H

#include "EmberOgrePrerequisites.h"
#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeTexture.h"

namespace EmberOgre
{
	
class EmberPagingLandScapeTexture : public Ogre::PagingLandScapeTexture
{

public:
	EmberPagingLandScapeTexture( void );
	virtual ~EmberPagingLandScapeTexture( void );
	
	virtual Ogre::String getName(){return Ogre::String("EmberTexture");}
		
	virtual void setPagesize( void ){_setPagesize();};
	virtual void clearData( void ){_clearData();};
		
	static void _setPagesize( void );
	static void _clearData( void );


	virtual Ogre::PagingLandScapeTexture* newTexture( );
	virtual bool TextureRenderCapabilitesFullfilled(); 

protected:
	virtual void _loadMaterial( void );
		
	virtual void _unloadMaterial( void );
	
};
	
}
#endif
