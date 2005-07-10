/***************************************************************************
	OgrePagingLandScapeTileManager.h  -  description
	-------------------
  begin                : Mon Jun 16 2003
  copyright            : (C) 2003-2005 by Jose A. Milan and Tuan Kuranes
  email                : spoke2@supercable.es && tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as        *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
*                                                                         *
***************************************************************************/

#ifndef PAGINGLANDSCAPETILEMANAGER_H
#define PAGINGLANDSCAPETILEMANAGER_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgreSingleton.h"

#include "OgrePagingLandScapeQueue.h"

namespace Ogre
{


    class PagingLandScapeTileManager : public Singleton< PagingLandScapeTileManager >
    {
        public:

	        PagingLandScapeTileManager( void );

	        virtual ~PagingLandScapeTileManager( void );

	        /** Retrieve a free tile.
	        */
	        PagingLandScapeTile* getTile( void );

	        /** Make a tile free.
	        */
	        void freeTile( PagingLandScapeTile* tile );

            void reset( void );

	        uint numTiles( void ) const;

	        int numFree( void ) const;

	        static PagingLandScapeTileManager& getSingleton( void );

	        static PagingLandScapeTileManager* getSingletonPtr( void );

            void load( void );
            void clear( void );

        protected:
	        void _addBatch( const uint num );

	        PagingLandScapeTileRow mTiles;

	        PagingLandScapeQueue< PagingLandScapeTile > mQueue;

	        uint mNumTiles;

    };

} //namespace

#endif
