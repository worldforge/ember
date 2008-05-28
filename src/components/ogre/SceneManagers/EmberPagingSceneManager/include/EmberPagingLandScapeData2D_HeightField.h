//
// C++ Interface: EmberPagingLandScapeData2D_HeightField
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

#ifndef EMBERPAGINGLANDSCAPEDATA2D_HEIGHTFIELD_H
#define EMBERPAGINGLANDSCAPEDATA2D_HEIGHTFIELD_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeData2D.h"


namespace EmberOgre
{
namespace Terrain {
	class TerrainPage;
}
	class EmberTerrainPageBridge;
/**
* @brief A specialized class for loading 2D Data from Mercator, through an EmberOgre::TerrainPage class.

Ember has its own system for generating the terrain. This class takes care of interfacing with Ember to fetch and update the heightfield.
The PagingLandscape SceneManager comes with a whole set of different heightfield loading classes by default. However, we'll never use any of them and instead always use this in all instances.
@author Erik Hjortsberg
*/
	class EmberPagingLandScapeData2D_HeightField: public Ogre::PagingLandScapeData2D
	{
		public:
			EmberPagingLandScapeData2D_HeightField(Ogre::PagingLandScapeData2DManager *pageMgr);
			virtual Ogre::String getName() const {return Ogre::String("EmberHeightField");}
			virtual ~EmberPagingLandScapeData2D_HeightField( void );

			virtual const Ogre::Vector3 getNormal( const Ogre::Real mX, const Ogre::Real mZ );
			virtual const Ogre::ColourValue getBase( const Ogre::Real mX, const Ogre::Real mZ );
			virtual const Ogre::ColourValue getCoverage( const Ogre::Real mX, const Ogre::Real mZ );
			virtual const Ogre::Real getShadow( const Ogre::Real mX, const Ogre::Real mZ, const bool& positive );

			virtual Ogre::PagingLandScapeData2D* newPage();
		
			virtual const Ogre::Real getMaxAbsoluteHeight(void) const;
			
			void setMaxHeight(float maxHeight);
		protected:

			virtual void _save( void );
			virtual bool _load( const uint x, const uint z );
			virtual void _load( void );
			virtual void _unload( void );

		private:
			Terrain::TerrainPage* mTerrainPage;
			EmberTerrainPageBridge* mBridge;
	};

}


#endif

