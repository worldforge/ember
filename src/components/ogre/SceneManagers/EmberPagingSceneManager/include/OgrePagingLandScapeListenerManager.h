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
#ifndef __PagingLandScapePageSourceListenerManager_H__
#define __PagingLandScapePageSourceListenerManager_H__

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeListener.h"
#include "OgreSingleton.h"

#include "OgrePagingLandScapeCallBackEvent.h"

namespace Ogre 
{
	/** Simple manager class to hold onto a list of page source listeners 
	across all sources.
	*/
	class _OgrePagingLandScapeExport PagingLandScapeListenerManager : public Singleton< PagingLandScapeListenerManager >
	{
	public:
		PagingLandScapeListenerManager(void);
		~PagingLandScapeListenerManager(void);

		/** Register a class which will be called back whenever a new page is
		available.
		@remarks
		Since this method is static, it applies to any page source which 
		is in active use; there is no need to register one per source.
		*/
		void addListener(PagingLandScapeListener* pl);

		/** Unregister a class which will be called back whenever a new page is
		available.
		*/
		void removeListener(PagingLandScapeListener* pl);

		/// Fire event
		void firePagePreloaded(const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox);

		/// Fire event
		void firePageLoaded(const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox);

		/// Fire event
		void firePageUnloaded(const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox);

		/// Fire event
		void firePagePostunloaded(const size_t pagex, const size_t pagez);

		/// Fire event
		void firePageShow(const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox);

		/// Fire event
		void firePageHide(const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox);

		/// Fire event
		void fireTileLoaded(const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox);

		/// Fire event
		void fireTileUnloaded(const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox);

		/// Fire event
		void fireTileDeformed(const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox);

		/// Fire event
		void fireTileShow(const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox);

		/// Fire event
		void fireTileHide(const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox);

		/// Fire event
		void fireTerrainReady(void);

		/** Override standard Singleton retrieval.
		*/
		static PagingLandScapeListenerManager& getSingleton(void);
		/** Override standard Singleton retrieval.
		*/
		static PagingLandScapeListenerManager* getSingletonPtr(void);	

        bool setOption(const String& strKey, const void* pValue);

		/** Register a delegate method which will be called back whenever terrain is ready (no more queued objects.)
		*/
		void addTerrainListener(PagingLandscapeDelegate* pl);
		void removeTerrainListener(PagingLandscapeDelegate* pl);


		/** Register a delegate method which will be called back whenever terrain is ready (no more queued objects.)
		*/
		void addPreloadPageListener(PagingLandscapeDelegate* pl);
		void removePreloadPageListener(PagingLandscapeDelegate* pl);
		/** Register a delegate method which will be called back whenever terrain is ready (no more queued objects.)
		*/
		void addShowPageListener(PagingLandscapeDelegate* pl);
		void removeShowPageListener(PagingLandscapeDelegate* pl);
		/** Register a delegate method which will be called back whenever terrain is ready (no more queued objects.)
		*/
		void addHidePageListener(PagingLandscapeDelegate* pl);
		void removeHidePageListener(PagingLandscapeDelegate* pl);
		/** Register a delegate method which will be called back whenever terrain is ready (no more queued objects.)
		*/
		void addLoadPageListener(PagingLandscapeDelegate* pl);
		void removeLoadPageListener(PagingLandscapeDelegate* pl);	
		/** Register a delegate method which will be called back whenever terrain is ready (no more queued objects.)
		*/
		void addModifyPageListener(PagingLandscapeDelegate* pl);
		void removeModifyPageListener(PagingLandscapeDelegate* pl);
		/** Register a delegate method which will be called back whenever terrain is ready (no more queued objects.)
		*/
		void addPostunloadPageListener(PagingLandscapeDelegate* pl);
		void removePostunloadPageListener(PagingLandscapeDelegate* pl);
		/** Register a delegate method which will be called back whenever terrain is ready (no more queued objects.)
		*/
		void addUnloadPageListener(PagingLandscapeDelegate* pl);
		void removeUnloadPageListener(PagingLandscapeDelegate* pl);
		

		/** Register a delegate method which will be called back whenever terrain is ready (no more queued objects.)
		*/
		void addShowTileListener(PagingLandscapeDelegate* pl);
		void removeShowTileListener(PagingLandscapeDelegate* pl);
		/** Register a delegate method which will be called back whenever terrain is ready (no more queued objects.)
		*/
		void addHideTileListener(PagingLandscapeDelegate* pl);
		void removeHideTileListener(PagingLandscapeDelegate* pl);
		/** Register a delegate method which will be called back whenever terrain is ready (no more queued objects.)
		*/
		void addLoadTileListener(PagingLandscapeDelegate* pl);
		void removeLoadTileListener(PagingLandscapeDelegate* pl);		
		/** Register a delegate method which will be called back whenever terrain is ready (no more queued objects.)
		*/
		void addModifyTileListener(PagingLandscapeDelegate* pl);
		void removeModifyTileListener(PagingLandscapeDelegate* pl);
		/** Register a delegate method which will be called back whenever terrain is ready (no more queued objects.)
		*/
		void addUnloadTileListener(PagingLandscapeDelegate* pl);
		void removeUnloadTileListener(PagingLandscapeDelegate* pl);




	protected:

		std::list<PagingLandscapeDelegate *> mTerrainReadyListeners;

		std::list<PagingLandscapeDelegate *> mShowPageListeners;
		std::list<PagingLandscapeDelegate *> mHidePageListeners;
		std::list<PagingLandscapeDelegate *> mPreloadPageListeners;
		std::list<PagingLandscapeDelegate *> mLoadPageListeners;
		std::list<PagingLandscapeDelegate *> mUnloadPageListeners;
		std::list<PagingLandscapeDelegate *> mPostunloadPageListeners;
		std::list<PagingLandscapeDelegate *> mModifyPageListeners;


		std::list<PagingLandscapeDelegate *> mShowTileListeners;
		std::list<PagingLandscapeDelegate *> mHideTileListeners;
		std::list<PagingLandscapeDelegate *> mLoadTileListeners;
		std::list<PagingLandscapeDelegate *> mUnloadTileListeners;
		std::list<PagingLandscapeDelegate *> mModifyTileListeners;


	};

}

#endif //__PagingLandScapePageSourceListener_H__
