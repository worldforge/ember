/*
-----------------------------------------------------------------------------
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
-----------------------------------------------------------------------------
*/
#include "OgreTerrainPageSource.h"
#include "OgreTerrainPage.h"
#include "OgreTerrainRenderable.h"
#include "OgreSceneNode.h"
#include "OgreTerrainSceneManager.h"
#include <OgreNoMemoryMacros.h>

namespace Ogre {

    //-----------------------------------------------------------------------
    template<> TerrainPageSourceListenerManager* Singleton<TerrainPageSourceListenerManager>::ms_Singleton = 0;
    TerrainPageSourceListenerManager* TerrainPageSourceListenerManager::getSingletonPtr(void)
    {
        return ms_Singleton;
    }
    TerrainPageSourceListenerManager& TerrainPageSourceListenerManager::getSingleton(void)
    {  
        assert( ms_Singleton );  return ( *ms_Singleton );  
    }
    //-------------------------------------------------------------------------
	void TerrainPageSourceListenerManager::addListener(TerrainPageSourceListener* pl)
	{
        mPageSourceListeners.push_back(pl);
	}
    //-------------------------------------------------------------------------
	void TerrainPageSourceListenerManager::removeListener(TerrainPageSourceListener* pl)
	{
        PageSourceListenerList::iterator i, iend;
        iend = mPageSourceListeners.end();
        for(i = mPageSourceListeners.begin(); i != iend; ++i)
        {
            if (*i == pl)
            {
                mPageSourceListeners.erase(i);
                break;
            }
        }
	}
    //-------------------------------------------------------------------------
	void TerrainPageSourceListenerManager::firePageConstructed(size_t pagex, size_t pagez, Real* heightData)
	{
        PageSourceListenerList::iterator i, iend;
        iend = mPageSourceListeners.end();
        for(i = mPageSourceListeners.begin(); i != iend; ++i)
        {
            (*i)->pageConstructed(pagex, pagez, heightData);
        }
	}
	//-------------------------------------------------------------------------
	TerrainPageSource::TerrainPageSource() : mSceneManager(0), mAsyncLoading(false) {
	}
	//-------------------------------------------------------------------------
	TerrainPage* TerrainPageSource::buildPage(Real* heightData, Material* pMaterial)
    {
        String name;

        // Create a Terrain Page
        TerrainPage* page = new TerrainPage((mPageSize-1) / (mTileSize-1));
        // Create a node for all tiles to be attached to
        // Note we sequentially name since page can be attached at different points
        // so page x/z is not appropriate
        static size_t pageIndex = 0;
		StringUtil::StrStreamType page_str;
		page_str << pageIndex;
        name = "page[";
        name += page_str.str() + "]";
        page->pageSceneNode = mSceneManager->createSceneNode(name);
        
        size_t q = 0;
        for ( size_t j = 0; j < mPageSize - 1; j += ( mTileSize - 1 ) )
        {
            size_t p = 0;

            for ( size_t i = 0; i < mPageSize - 1; i += ( mTileSize - 1 ) )
            {
				StringUtil::StrStreamType new_name_str;
				
                // Create scene node for the tile and the TerrainRenderable
                new_name_str << "tile[" << pageIndex << "][" << (int)p << "," << (int)q << "]";
				name = new_name_str.str();

                SceneNode *c = page->pageSceneNode->createChildSceneNode( name );
                TerrainRenderable *tile = new TerrainRenderable(name);
                // Initialise the tile
/*				if (p == 5 && q == 0) {
					tile->setMaterial(static_cast<Ogre::Material*>(Ogre::MaterialManager::getSingleton().getByName("Malebuilder/Body")));
				} else {
                	tile->setMaterial(pMaterial);
				}*/
               tile->setMaterial(pMaterial);
                tile->initialise(i, j, heightData);
                // Attach it to the page
                page->tiles[ p ][ q ] = tile;
                // Attach it to the node
                c ->attachObject( tile );
 				//tile->setForcedRenderLevel(4);
                p++;
            }

            q++;

        }

        pageIndex++;

        // calculate neighbours for page
        page->linkNeighbours();

        return page;
    }
    //-------------------------------------------------------------------------
    void TerrainPageSource::firePageConstructed(size_t pagex, size_t pagez, Real* heightData)
    {
		TerrainPageSourceListenerManager::getSingleton().firePageConstructed(pagex, pagez, heightData);
    }
    //-------------------------------------------------------------------------
    void TerrainPageSource::addListener(TerrainPageSourceListener* pl)
    {
		TerrainPageSourceListenerManager::getSingleton().addListener(pl);
    }
    //-------------------------------------------------------------------------
    void TerrainPageSource::removeListener(TerrainPageSourceListener* pl)
    {
		TerrainPageSourceListenerManager::getSingleton().removeListener(pl);

    }
    //-------------------------------------------------------------------------

}
