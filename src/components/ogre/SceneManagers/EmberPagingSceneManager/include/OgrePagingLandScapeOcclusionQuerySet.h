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

#ifndef __OgrePagingLandScapeOcclusionQuerySet_H__
#define __OgrePagingLandScapeOcclusionQuerySet_H__

#include "OgrePrerequisites.h"


namespace Ogre 
{

   
    /** A collection of pre-allocated queries
    */
    class QuerySet 
    {
        public:

            /** Usual constructor 
                @param
                    poolSize The initial size of the query pool. Estimate of the number of queries
                    which will be required, and pass it using this parameter. The set will
                    preallocate this number to avoid memory fragmentation. The default behaviour
                    once this pool has run out is to double it.            
                @see
                    QuerySet::setAutoextend
            */
            QuerySet(unsigned int poolSize = 0, 
                        bool autoExtendPool = false);
            virtual ~QuerySet();

            /** Creates a new query and adds it to this set.
                @remarks
                    Behavior once the query pool has been exhausted depends on the
                    QuerySet::setAutoextendPool option.         
                @returns
                    On success, a pointer to a newly created Query is
                    returned.
                @par
                    On failure (i.e. no more space and can't autoextend),
                    <b>NULL</b> is returned.
                @see
                    QuerySet::setAutoextend
            */
            HardwareOcclusionQuery* getQuery();

            /** Removes a query from the set.
                @note
                    This version is more efficient than removing by index.
            */
            virtual void removeQuery(HardwareOcclusionQuery* pQuery);

            /** Returns the number of active queries which currently make up this set.
            */
            virtual int getNumQueries(void) const;

            /** Tells the set whether to allow automatic extension of the pool of queries.
                @remarks
                    A QuerySet stores a pool of pre-constructed queries which are used as needed when
                    a new query is requested. This allows applications to create / remove queries efficiently
                    without incurring construction / destruction costs (a must for sets with lots of queries like
                    particle effects). This method allows you to configure the behavior when a new query is requested
                    but the query pool has been exhausted.
                @par
                    The default behavior is to allow the pool to extend (typically this allocates double the current
                    pool of queries when the pool is expended), equivalent to calling this method with
                    autoExtend = true. If you set the parameter to false however, any attempt to create a new query
                    when the pool has expired will simply fail silently, returning a null pointer.
                @param autoextend true to double the pool every time it runs out, false to fail silently.
            */
            virtual void setAutoextend(bool autoextend);

            /** Returns true if the query pool automatically extends.
                @see
                    QuerySet::setAutoextend
            */
            virtual bool getAutoextend(void) const;


            /** Adjusts the size of the pool of queries available in this set.
                @remarks
                    See the QuerySet::setAutoextend method for full details of the query pool. This method adjusts
                    the preallocated size of the pool. If you try to reduce the size of the pool, the set has the option
                    of ignoring you if too many queries are already in use. Bear in mind that calling this method will
                    incur significant construction / destruction calls so should be avoided in time-critical code. The same
                    goes for auto-extension, try to avoid it by estimating the pool size correctly up-front.
                @param
                    size The new size for the pool.
            */
            virtual void setPoolSize(unsigned int size);
            /** Returns the current size of the query pool.
                @returns
                    The current size of the query pool.
                @see
                    QuerySet::setAutoextend
            */
            virtual unsigned int getPoolSize(void) const;

            /** Empties this set of all queries.
            */
            virtual void clear();

        private:



            typedef std::list<HardwareOcclusionQuery*> ActiveQueryList;
            typedef std::list<HardwareOcclusionQuery*> FreeQueryList;
            typedef std::vector<HardwareOcclusionQuery*> QueryPool;

            /** Active query list.
                @remarks
                    This is a linked list of pointers to queries in the query pool.
                @par
                    This allows very fast insertions and deletions from anywhere in the list to activate / deactivate queries
                    (required for particle systems etc)    as well as reuse of Query instances in the pool
                    without construction & destruction which avoids memory thrashing.
            */
            ActiveQueryList mActiveQuerys;

            /** Free query queue.
                @remarks
                    This contains a list of the queries free for use as new instances
                    as required by the set. Query instances are pre-constructed up to the estimated size in the
                    mQueryPool vector and are referenced on this deque at startup. As they get used this deque
                    reduces, as they get released back to to the set they get added back to the deque.
            */
            FreeQueryList mFreeQuerys;

            /** Pool of query instances for use and reuse in the active query list.
                @remarks
                    This vector will be preallocated with the estimated size of the set,and will extend as required.
            */
            QueryPool mQueryPool;


            /// Internal method for increasing pool size
            virtual void increasePool(unsigned int size);


            /// The number of query in the pool.
            unsigned int mPoolSize;
            /// Flag indicating whether to autoextend pool
            bool mAutoExtendPool;
    };

}


#endif//__OgrePagingLandScapeOcclusionQuerySet_H__
