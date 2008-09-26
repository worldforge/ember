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

#include "OgrePagingLandScapePrecompiledHeaders.h"


#include "OgrePagingLandScapeListenerManager.h"    
#include "OgrePagingLandScapeSceneManager.h"    

namespace Ogre 
{
	//-------------------------------------------------------------------------
    PagingLandScapeListenerManager::PagingLandScapeListenerManager(PagingLandScapeSceneManager * scnMgr)
	{
	};
	//-------------------------------------------------------------------------
	PagingLandScapeListenerManager::~PagingLandScapeListenerManager(void)
	{
		std::for_each(mTerrainReadyListeners.begin(), mTerrainReadyListeners.end(), delete_object());

		std::for_each(mShowPageListeners.begin(), mShowPageListeners.end(), delete_object());
		std::for_each(mHidePageListeners.begin(), mHidePageListeners.end(), delete_object());
		std::for_each(mPreloadPageListeners.begin(), mPreloadPageListeners.end(), delete_object());
		std::for_each(mLoadPageListeners.begin(), mLoadPageListeners.end(), delete_object());
		std::for_each(mUnloadPageListeners.begin(), mUnloadPageListeners.end(), delete_object());
		std::for_each(mPostunloadPageListeners.begin(), mPostunloadPageListeners.end(), delete_object());
		std::for_each(mModifyPageListeners.begin(), mModifyPageListeners.end(), delete_object());


		std::for_each(mShowTileListeners.begin(), mShowTileListeners.end(), delete_object());
		std::for_each(mHideTileListeners.begin(), mHideTileListeners.end(), delete_object());
		std::for_each(mLoadTileListeners.begin(), mLoadTileListeners.end(), delete_object());
		std::for_each(mUnloadTileListeners.begin(), mUnloadTileListeners.end(), delete_object());
		std::for_each(mModifyTileListeners.begin(), mModifyTileListeners.end(), delete_object());

	};
    //-------------------------------------------------------------------------
    void  PagingLandScapeListenerManager::firePagePreloaded(const size_t pagex, 
															const size_t pagez, 
															const Real* heightData, 
															const AxisAlignedBox &Bbox)
	{
		if (mPreloadPageListeners.empty())
			return;

		PagingLandscapeEvent e (pagex, pagez, 0, 0, heightData, Bbox);
		for (std::list<PagingLandscapeDelegate *>::iterator it = mPreloadPageListeners.begin(); 
			it != mPreloadPageListeners.end(); ++it)
			(*(*it))(&e);
    }
    //-------------------------------------------------------------------------
    void  PagingLandScapeListenerManager::firePageLoaded(const size_t pagex, const size_t pagez, 
        const Real* heightData, const AxisAlignedBox &Bbox)
	{
		if (mLoadPageListeners.empty())
			return;

		PagingLandscapeEvent e (pagex, pagez, 0, 0, heightData, Bbox);
		for (std::list<PagingLandscapeDelegate *>::iterator it = mLoadPageListeners.begin(); 
			it != mLoadPageListeners.end(); ++it)
			(*(*it))(&e);
    }
    //-------------------------------------------------------------------------
    void  PagingLandScapeListenerManager::firePageUnloaded(const size_t pagex, const size_t pagez, 
        const Real* heightData, const AxisAlignedBox &Bbox)
	{
		if (mUnloadPageListeners.empty())
			return;

		PagingLandscapeEvent e (pagex, pagez, 0, 0, heightData, Bbox);
		for (std::list<PagingLandscapeDelegate *>::iterator it = mUnloadPageListeners.begin(); 
			it != mUnloadPageListeners.end(); ++it)
			(*(*it))(&e);
    }

    //-------------------------------------------------------------------------
    void  PagingLandScapeListenerManager::firePagePostunloaded(const size_t pagex, const size_t pagez)
	{
		if (mPostunloadPageListeners.empty())
			return;

		PagingLandscapeEvent e (pagex, pagez, 0, 0, 0, AxisAlignedBox());
		for (std::list<PagingLandscapeDelegate *>::iterator it = mPostunloadPageListeners.begin(); 
			it != mPostunloadPageListeners.end(); ++it)
			(*(*it))(&e);
    }
    //-------------------------------------------------------------------------
    void  PagingLandScapeListenerManager::firePageShow(const size_t pagex, const size_t pagez, 
        const Real* heightData, const AxisAlignedBox &Bbox)
	{
		if (mShowPageListeners.empty())
			return;

		PagingLandscapeEvent e (pagex, pagez, 0, 0, heightData, Bbox);
		for (std::list<PagingLandscapeDelegate *>::iterator it = mShowPageListeners.begin(); 
			it != mShowPageListeners.end(); ++it)
			(*(*it))(&e);
    }
    //-------------------------------------------------------------------------
    void  PagingLandScapeListenerManager::firePageHide(const size_t pagex, const size_t pagez, 
        const Real* heightData, const AxisAlignedBox &Bbox)
	{   
		if (mHidePageListeners.empty())
			return;

		PagingLandscapeEvent e (pagex, pagez, 0, 0, heightData, Bbox);
		for (std::list<PagingLandscapeDelegate *>::iterator it = mHidePageListeners.begin(); 
			it != mHidePageListeners.end(); ++it)
			(*(*it))(&e);
    }
    //-------------------------------------------------------------------------
    void  PagingLandScapeListenerManager::fireTileLoaded (const size_t pagex, const size_t pagez, 
                            const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox)
	{
		if (mLoadTileListeners.empty())
			return;

		PagingLandscapeEvent e (pagex, pagez, tilex, tilez, 0, Bbox);
		for (std::list<PagingLandscapeDelegate *>::iterator it = mLoadTileListeners.begin(); 
			it != mLoadTileListeners.end(); ++it)
			(*(*it))(&e);
    }
    //-------------------------------------------------------------------------
    void  PagingLandScapeListenerManager::fireTileUnloaded(const size_t pagex, const size_t pagez, 
                                const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox)
	{
		if (mUnloadTileListeners.empty())
			return;

		PagingLandscapeEvent e (pagex, pagez, tilex, tilez, 0, Bbox);
		for (std::list<PagingLandscapeDelegate *>::iterator it = mUnloadTileListeners.begin(); 
			it != mUnloadTileListeners.end(); ++it)
			(*(*it))(&e);
    }
    //-------------------------------------------------------------------------
    void  PagingLandScapeListenerManager::fireTileDeformed(const size_t pagex, const size_t pagez, 
                        const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox)
	{
		if (mModifyTileListeners.empty())
			return;

		PagingLandscapeEvent e (pagex, pagez, tilex, tilez, 0, Bbox);
		for (std::list<PagingLandscapeDelegate *>::iterator it = mModifyTileListeners.begin(); 
			it != mModifyTileListeners.end(); ++it)
			(*(*it))(&e);
    }
    //-------------------------------------------------------------------------
    void  PagingLandScapeListenerManager::fireTileShow(const size_t pagex, const size_t pagez, 
                        const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox)
	{
		if (mShowTileListeners.empty())
			return;

		PagingLandscapeEvent e (pagex, pagez, tilex, tilez, 0, Bbox);
		for (std::list<PagingLandscapeDelegate *>::iterator it = mShowTileListeners.begin(); 
			it != mShowTileListeners.end(); ++it)
			(*(*it))(&e);
    }
    //-------------------------------------------------------------------------
    void  PagingLandScapeListenerManager::fireTileHide(const size_t pagex, const size_t pagez, 
                        const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox)
	{
		if (mHideTileListeners.empty())
			return;

		PagingLandscapeEvent e (pagex, pagez, tilex, tilez, 0, Bbox);
		for (std::list<PagingLandscapeDelegate *>::iterator it = mHideTileListeners.begin(); 
			it != mHideTileListeners.end(); ++it)
			(*(*it))(&e);
    }
    //-------------------------------------------------------------------------
    void PagingLandScapeListenerManager::fireTerrainReady()
    {
        if (mTerrainReadyListeners.empty())
            return;

		PagingLandscapeEvent e (0, 0, 0, 0, 0, AxisAlignedBox ());
		for (std::list<PagingLandscapeDelegate *>::iterator it = mTerrainReadyListeners.begin(); 
										it != mTerrainReadyListeners.end(); ++it)
				(*(*it))(&e);
    }
    //-------------------------------------------------------------------------
    bool  PagingLandScapeListenerManager::setOption(const String& strKey, const void* pValue)
    {

		PagingLandscapeDelegate *d = const_cast < PagingLandscapeDelegate * > (static_cast < const PagingLandscapeDelegate * >(pValue));
		if (StringUtil::startsWith(strKey, "add", true))
		{
			if (strKey == "addTerrainListener")
			{
				addTerrainListener (d);
				return true;
			}

			if (strKey == "addPreloadPageListener")
			{
				addPreloadPageListener (d);
				return true;
			}
			if (strKey == "addShowPageListener")
			{
				addShowPageListener (d);
				return true;
			}
			if (strKey == "addHidePageListener")
			{
				addHidePageListener (d);
				return true;
			}
			if (strKey == "addLoadPageListener")
			{
				addLoadPageListener (d);
				return true;
			}
			if (strKey == "addModifyPageListener")
			{
				addModifyPageListener (d);
				return true;
			}
			if (strKey == "addPostunloadPageListener")
			{
				addPostunloadPageListener (d);
				return true;
			}
			if (strKey == "addUnloadPageListener")
			{
				addUnloadPageListener (d);
				return true;
			}

			if (strKey == "addShowTileListener")
			{
				addShowTileListener (d);
				return true;
			}
			if (strKey == "addHideTileListener")
			{
				addHideTileListener (d);
				return true;
			}
			if (strKey == "addLoadTileListener")
			{
				addLoadTileListener (d);
				return true;
			}
			if (strKey == "addModifyTileListener")
			{
				addModifyTileListener (d);
				return true;
			}
			if (strKey == "addUnloadTileListener")
			{
				addUnloadTileListener (d);
				return true;
			}
		} 

		if (StringUtil::startsWith(strKey, "remove", true))
		{

			if (strKey == "removeTerrainListener")
			{
				removeTerrainListener (d);
				return true;
			}

			if (strKey == "removePreloadPageListener")
			{
				removePreloadPageListener (d);
				return true;
			}
			if (strKey == "removeShowPageListener")
			{
				removeShowPageListener (d);
				return true;
			}
			if (strKey == "removeHidePageListener")
			{
				removeHidePageListener (d);
				return true;
			}
			if (strKey == "removeLoadPageListener")
			{
				removeLoadPageListener (d);
				return true;
			}
			if (strKey == "removeModifyPageListener")
			{
				removeModifyPageListener (d);
				return true;
			}
			if (strKey == "removePostunloadPageListener")
			{
				removePostunloadPageListener (d);
				return true;
			}
			if (strKey == "removeUnloadPageListener")
			{
				removeUnloadPageListener (d);
				return true;
			}

			if (strKey == "removeShowTileListener")
			{
				removeShowTileListener (d);
				return true;
			}
			if (strKey == "removeHideTileListener")
			{
				removeHideTileListener (d);
				return true;
			}
			if (strKey == "removeLoadTileListener")
			{
				removeLoadTileListener (d);
				return true;
			}
			if (strKey == "removeModifyTileListener")
			{
				removeModifyTileListener (d);
				return true;
			}
			if (strKey == "removeUnloadTileListener")
			{
				removeUnloadTileListener (d);
				return true;
			}
		} 
        return false;
	}
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::addTerrainListener(PagingLandscapeDelegate* pl) 
	{
		mTerrainReadyListeners.push_back(pl);
	}
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::removeTerrainListener(PagingLandscapeDelegate* pl) 
	{
		mTerrainReadyListeners.remove (pl);
	}
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::addPreloadPageListener(PagingLandscapeDelegate* pl) 
	{
		mPreloadPageListeners.push_back(pl);
	};
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::removePreloadPageListener(PagingLandscapeDelegate* pl) 
	{
		mPreloadPageListeners.remove (pl);
	};
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::addShowPageListener(PagingLandscapeDelegate* pl) 
	{
		mShowPageListeners.push_back(pl);
	};
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::removeShowPageListener(PagingLandscapeDelegate* pl) 
	{
		mShowPageListeners.remove (pl);
	};
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::addHidePageListener(PagingLandscapeDelegate* pl) 
	{
	mHidePageListeners.push_back(pl);
	};
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::removeHidePageListener(PagingLandscapeDelegate* pl) 
	{
	mHidePageListeners.remove (pl);
	};
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::addLoadPageListener(PagingLandscapeDelegate* pl) 
	{
	mLoadPageListeners.push_back(pl);
	};
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::removeLoadPageListener(PagingLandscapeDelegate* pl) 
	{
		mLoadPageListeners.remove (pl);
	};		
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::addModifyPageListener(PagingLandscapeDelegate* pl) 
	{
	mModifyPageListeners.push_back(pl);
	};
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::removeModifyPageListener(PagingLandscapeDelegate* pl) 
	{
		mModifyPageListeners.remove (pl);
	};
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::addPostunloadPageListener(PagingLandscapeDelegate* pl) 
	{
		mPostunloadPageListeners.push_back(pl);
	};
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::removePostunloadPageListener(PagingLandscapeDelegate* pl) 
	{
		mPostunloadPageListeners.remove (pl);
	};	
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::addUnloadPageListener(PagingLandscapeDelegate* pl) 
	{
		mUnloadPageListeners.push_back(pl);
	};
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::removeUnloadPageListener(PagingLandscapeDelegate* pl) 
	{
		mUnloadPageListeners.remove (pl);
	};
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::addShowTileListener(PagingLandscapeDelegate* pl) 
	{
		mShowTileListeners.push_back(pl);
	};
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::removeShowTileListener(PagingLandscapeDelegate* pl) 
	{
		mShowTileListeners.remove (pl);
	};
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::addHideTileListener(PagingLandscapeDelegate* pl) 
	{
		mHideTileListeners.push_back(pl);
	};
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::removeHideTileListener(PagingLandscapeDelegate* pl) 
	{
		mHideTileListeners.remove (pl);
	};
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::addLoadTileListener(PagingLandscapeDelegate* pl) 
	{
		mLoadTileListeners.push_back(pl);
	};
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::removeLoadTileListener(PagingLandscapeDelegate* pl) 
	{
		mLoadTileListeners.remove (pl);
	};	
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::addModifyTileListener(PagingLandscapeDelegate* pl) 
	{
		mModifyTileListeners.push_back(pl);
	};
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::removeModifyTileListener(PagingLandscapeDelegate* pl) 
	{
		mModifyTileListeners.remove (pl);
	};
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::addUnloadTileListener(PagingLandscapeDelegate* pl) 
	{
		mUnloadTileListeners.push_back(pl);
	};
	//-------------------------------------------------------------------------
	void PagingLandScapeListenerManager::removeUnloadTileListener(PagingLandscapeDelegate* pl) 
	{	
		mUnloadTileListeners.remove (pl);
	};
}//namespace

