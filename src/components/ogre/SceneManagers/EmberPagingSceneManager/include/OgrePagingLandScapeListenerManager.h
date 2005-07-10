/*-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright © 2000-2004 The OGRE Team
Also see acknowledgements in Readme.html
 
This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------*/
#ifndef __PagingLandscapePageSourceListenerManager_H__
#define __PagingLandscapePageSourceListenerManager_H__

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeListener.h"
#include "OgreSingleton.h"

namespace Ogre 
{
	/** Simple manager class to hold onto a list of page source listeners 
	across all sources.
	*/
	class _OgrePagingLandScapeExport PagingLandscapeListenerManager : public Singleton< PagingLandscapeListenerManager >
	{
	public:
		PagingLandscapeListenerManager( void )
		{
		};

		~PagingLandscapeListenerManager( void )
		{
		};

		/** Register a class which will be called back whenever a new page is
		available.
		@remarks
		Since this method is static, it applies to any page source which 
		is in active use; there is no need to register one per source.
		*/
		void addListener( PagingLandscapeListener* pl );

		/** Unregister a class which will be called back whenever a new page is
		available.
		*/
		void removeListener( PagingLandscapeListener* pl );

		/// Fire event
		void firePagePreloaded( const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox  );

		/// Fire event
		void firePageLoaded( const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox  );

		/// Fire event
		void firePageUnloaded( const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox  );

		/// Fire event
		void firePagePostunloaded( const size_t pagex, const size_t pagez);

		/// Fire event
		void firePageShow( const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox  );

		/// Fire event
		void firePageHide( const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox  );

		/// Fire event
		void fireTileLoaded( const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox  );

		/// Fire event
		void fireTileUnloaded( const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox  );

		/// Fire event
		void fireTileDeformed( const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox  );

		/// Fire event
		void fireTileShow( const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox  );

		/// Fire event
		void fireTileHide( const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox  );

		/// Fire event
		void fireTerrainReady( void );

		/** Override standard Singleton retrieval.
		*/
		static PagingLandscapeListenerManager& getSingleton( void );
		/** Override standard Singleton retrieval.
		*/
		static PagingLandscapeListenerManager* getSingletonPtr( void );	

	protected:
		typedef std::list< PagingLandscapeListener* > PagingLandscapeListenerList;
		PagingLandscapeListenerList mPageSourceListeners;

	};

}

#endif //__PagingLandscapePageSourceListener_H__
