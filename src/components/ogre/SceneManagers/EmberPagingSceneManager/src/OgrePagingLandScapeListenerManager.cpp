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

#include "OgrePagingLandScapeListenerManager.h"    

namespace Ogre 
{
    //-----------------------------------------------------------------------
    template<> PagingLandscapeListenerManager* Singleton<PagingLandscapeListenerManager>::ms_Singleton = 0;
    PagingLandscapeListenerManager* PagingLandscapeListenerManager::getSingletonPtr(void)
    {
        return ms_Singleton;
    }
    PagingLandscapeListenerManager& PagingLandscapeListenerManager::getSingleton(void)
    {  
        assert( ms_Singleton );  return ( *ms_Singleton );  
    }
    //-------------------------------------------------------------------------
    void PagingLandscapeListenerManager::addListener(PagingLandscapeListener* pl)
    {
        mPageSourceListeners.push_back(pl);
    }
    //-------------------------------------------------------------------------
    void PagingLandscapeListenerManager::removeListener(PagingLandscapeListener* pl)
    {
        if (mPageSourceListeners.empty())
            return;
        PagingLandscapeListenerList::iterator iend = mPageSourceListeners.end();
        for(PagingLandscapeListenerList::iterator i = mPageSourceListeners.begin(); i != iend;++i)
        {
            if (*i == pl)
            {
                mPageSourceListeners.remove (*i);
                break;
            }
        }
    }
    //-------------------------------------------------------------------------
    void  PagingLandscapeListenerManager::firePagePreloaded(const size_t pagex, const size_t pagez, 
        const Real* heightData, const AxisAlignedBox &Bbox )
    {
        if (mPageSourceListeners.empty())
            return;
        PagingLandscapeListenerList::iterator iend = mPageSourceListeners.end();
        for(PagingLandscapeListenerList::iterator i = mPageSourceListeners.begin(); 
            i != iend; ++i)
        {
            (*i)->pagePreloaded(pagex, pagez, heightData, Bbox);
        }
    }
    //-------------------------------------------------------------------------
    void  PagingLandscapeListenerManager::firePageLoaded(const size_t pagex, const size_t pagez, 
        const Real* heightData, const AxisAlignedBox &Bbox )
    {
        if (mPageSourceListeners.empty())
            return;
        PagingLandscapeListenerList::iterator iend = mPageSourceListeners.end();
        for(PagingLandscapeListenerList::iterator i = mPageSourceListeners.begin(); 
            i != iend; ++i)
        {
            (*i)->pageLoaded(pagex, pagez, heightData, Bbox);
        }
    }
    //-------------------------------------------------------------------------
    void  PagingLandscapeListenerManager::firePageUnloaded(const size_t pagex, const size_t pagez, 
        const Real* heightData, const AxisAlignedBox &Bbox )
    {
        if (mPageSourceListeners.empty())
            return;
        PagingLandscapeListenerList::iterator iend = mPageSourceListeners.end();
        for(PagingLandscapeListenerList::iterator i = mPageSourceListeners.begin(); 
            i != iend; ++i)
        {
            (*i)->pageUnloaded (pagex, pagez, heightData, Bbox);
        }
    }

    //-------------------------------------------------------------------------
    void  PagingLandscapeListenerManager::firePagePostunloaded(const size_t pagex, const size_t pagez)
    {
        if (mPageSourceListeners.empty())
            return;
        PagingLandscapeListenerList::iterator iend = mPageSourceListeners.end();
        for(PagingLandscapeListenerList::iterator i = mPageSourceListeners.begin(); 
            i != iend; ++i)
        {
            (*i)->pagePostunloaded (pagex, pagez);
        }
    }
    //-------------------------------------------------------------------------
    void  PagingLandscapeListenerManager::firePageShow(const size_t pagex, const size_t pagez, 
        const Real* heightData, const AxisAlignedBox &Bbox )
    {
        if (mPageSourceListeners.empty())
            return;
        PagingLandscapeListenerList::iterator iend = mPageSourceListeners.end();
        for(PagingLandscapeListenerList::iterator i = mPageSourceListeners.begin(); 
            i != iend; ++i)
        {
            (*i)->pageShow (pagex, pagez, heightData, Bbox);
        }
    }
    //-------------------------------------------------------------------------
    void  PagingLandscapeListenerManager::firePageHide(const size_t pagex, const size_t pagez, 
        const Real* heightData, const AxisAlignedBox &Bbox )
    {       
        if (mPageSourceListeners.empty())
            return;
        PagingLandscapeListenerList::iterator iend = mPageSourceListeners.end();
        for(PagingLandscapeListenerList::iterator i = mPageSourceListeners.begin(); 
            i != iend; ++i)
        {
            (*i)->pageHide (pagex, pagez, heightData, Bbox);
        }
    }
    //-------------------------------------------------------------------------
    void  PagingLandscapeListenerManager::fireTileLoaded (const size_t pagex, const size_t pagez, 
                            const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox )
    {
        if (mPageSourceListeners.empty())
            return;
        PagingLandscapeListenerList::iterator iend = mPageSourceListeners.end();
        for(PagingLandscapeListenerList::iterator i = mPageSourceListeners.begin(); 
            i != iend; ++i)
        {
            (*i)->tileLoaded (pagex, pagez, tilex, tilez, Bbox);
        }
    }
    //-------------------------------------------------------------------------
    void  PagingLandscapeListenerManager::fireTileUnloaded(const size_t pagex, const size_t pagez, 
                                const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox )
    {
        if (mPageSourceListeners.empty())
            return;
        PagingLandscapeListenerList::iterator iend = mPageSourceListeners.end();
        for (PagingLandscapeListenerList::iterator i = mPageSourceListeners.begin(); 
            i != iend; ++i)
        {
            (*i)->tileUnloaded (pagex, pagez, tilex, tilez, Bbox);
        }
    }
    //-------------------------------------------------------------------------
    void  PagingLandscapeListenerManager::fireTileDeformed(const size_t pagex, const size_t pagez, 
                        const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox )
    {
        if (mPageSourceListeners.empty())
            return;
        PagingLandscapeListenerList::iterator iend = mPageSourceListeners.end();
        for (PagingLandscapeListenerList::iterator i = mPageSourceListeners.begin(); 
            i != iend; ++i)
        {
            (*i)->tileDeformed (pagex, pagez, tilex, tilez, Bbox);
        }
    }
    //-------------------------------------------------------------------------
    void  PagingLandscapeListenerManager::fireTileShow(const size_t pagex, const size_t pagez, 
                        const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox )
    {
        if (mPageSourceListeners.empty())
            return;
        PagingLandscapeListenerList::iterator iend = mPageSourceListeners.end();
        for (PagingLandscapeListenerList::iterator i = mPageSourceListeners.begin(); 
            i != iend; ++i)
        {
            (*i)->tileShow (pagex, pagez, tilex, tilez, Bbox);
        }
    }
    //-------------------------------------------------------------------------
    void  PagingLandscapeListenerManager::fireTileHide(const size_t pagex, const size_t pagez, 
                        const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox )
    {
        if (mPageSourceListeners.empty())
            return;
        PagingLandscapeListenerList::iterator iend = mPageSourceListeners.end();
        for (PagingLandscapeListenerList::iterator i = mPageSourceListeners.begin(); 
            i != iend; ++i)
        {
            (*i)->tileHide (pagex, pagez, tilex, tilez, Bbox);
        }
    }
    //-------------------------------------------------------------------------
    void PagingLandscapeListenerManager::fireTerrainReady()
    {
        if (mPageSourceListeners.empty())
            return;

        std::for_each( mPageSourceListeners.begin(), 
            mPageSourceListeners.end(),  
            std::mem_fun( &PagingLandscapeListener::terrainReady ));

    }
}//namespace

