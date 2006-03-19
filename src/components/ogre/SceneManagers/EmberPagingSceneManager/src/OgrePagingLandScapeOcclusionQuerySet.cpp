/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
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
#include "Ogre.h"
#include "OgrePagingLandScapeOcclusionQuerySet.h"
#include "OgreHardwareOcclusionQuery.h"


namespace Ogre 
 {

    //-----------------------------------------------------------------------
    QuerySet::QuerySet(
        unsigned int poolSize, 
        bool autoExtendPool) :
        mAutoExtendPool(autoExtendPool),
        mPoolSize(poolSize)
    {
        setPoolSize(poolSize);
    }
    //-----------------------------------------------------------------------
    QuerySet::~QuerySet()
    {
        RenderSystem * const renderSys = Ogre::Root::getSingleton().getRenderSystem();
        // Free pool items
        for (QueryPool::iterator i = mQueryPool.begin(); i != mQueryPool.end(); ++i)
        {   
            renderSys->destroyHardwareOcclusionQuery(*i);
            //delete (*i);
        }
        mQueryPool.clear();
    }
    //-----------------------------------------------------------------------
    HardwareOcclusionQuery* QuerySet::getQuery()
    {
        if (mFreeQuerys.empty())
        {
            if (mAutoExtendPool)
            {
                setPoolSize (getPoolSize() * 2);
            }
            else
            {
                return 0;
            }
        }

        // Get a new query
        HardwareOcclusionQuery* newQuery = mFreeQuerys.front();
        mFreeQuerys.pop_front();
		mActiveQuerys.push_back(newQuery); 
        
        return newQuery;
    }
    //-----------------------------------------------------------------------
    int QuerySet::getNumQueries(void) const
    {
        return static_cast< int >(mActiveQuerys.size());
    }

    //-----------------------------------------------------------------------
    void QuerySet::clear()
    {
		// Insert actives into free list
		mFreeQuerys.insert(mFreeQuerys.end(), mActiveQuerys.begin(), mActiveQuerys.end());
      
		// Remove all active instances
      	mActiveQuerys.clear(); 
    }

    //-----------------------------------------------------------------------
    void QuerySet::removeQuery(HardwareOcclusionQuery* pQuery)
    {
        mActiveQuerys.remove(pQuery);
        mFreeQuerys.push_back(pQuery);
    }

    //-----------------------------------------------------------------------
    void QuerySet::setAutoextend(bool autoextend)
    {
        mAutoExtendPool = autoextend;
    }

    //-----------------------------------------------------------------------
    bool QuerySet::getAutoextend(void) const
    {
        return mAutoExtendPool;
    }
    //-----------------------------------------------------------------------
    void QuerySet::setPoolSize(unsigned int size)
    {
        // Never shrink below size()
        size_t currSize = mQueryPool.size();

        if(currSize < size)
        {
            increasePool(size);

            for (size_t i = currSize; i < size; ++i)
            {
                // Add new items to the queue
                mFreeQuerys.push_back (mQueryPool[i]);
            }
            
            mPoolSize = size;
        }
    }


    //-----------------------------------------------------------------------
    unsigned int QuerySet::getPoolSize(void) const
    {
        return static_cast< unsigned int >(mQueryPool.size());
    }

    //-----------------------------------------------------------------------
    void QuerySet::increasePool(unsigned int size)
    {
        size_t oldSize = mQueryPool.size();

        // Increase size
        mQueryPool.reserve(size);
        mQueryPool.resize(size);

        // Create new queries
        RenderSystem * const renderSys = Ogre::Root::getSingleton().getRenderSystem();
        for (size_t i = oldSize; i < size; ++i)
            mQueryPool[i] = renderSys->createHardwareOcclusionQuery();;

    }
}
