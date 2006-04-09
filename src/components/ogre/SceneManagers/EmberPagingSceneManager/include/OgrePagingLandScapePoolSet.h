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

#ifndef __OgrePagingLandScapePoolSet_H__
#define __OgrePagingLandScapePoolSet_H__

#include <list>
#include <vector>


namespace Ogre 
{
    /** A collection of pre-allocated object instance of a same base class
    */
    template <class T> class PoolSet 
    {
    protected:

        typedef std::list<T*> ActivePoolableList;
        typedef std::list<T*> FreePoolableList;
        typedef std::vector<T*> PoolablePool;

    public:

        /** Usual constructor 
            @param
                poolSize The initial size of the query pool. Estimate of the number of queries
                which will be required, and pass it using this parameter. The set will
                preallocate this number to avoid memory fragmentation. The default behaviour
                once this pool has run out is to double it.            
            @see
                PoolSet::setAutoextend
        */
         PoolSet(unsigned int poolSize = 0,  bool autoExtendPool = true):
          mAutoExtendPool(autoExtendPool),
              mPoolSize(poolSize)
          {
              setPoolSize(poolSize);
          }
          /** 
          Creates a new query and adds it to this set.
          */
          virtual ~PoolSet() 
          {
             assert (mPoolSize == 0 && mPoolablePool.empty());
          }
          /** 
          Creates a new query and adds it to this set.
          */
          void deletePool() 
          {
              // Free pool items
              for (typename PoolablePool::iterator i = mPoolablePool.begin(); 
                  i != mPoolablePool.end(); 
                  ++i)
              {   
                  deallocate (*i);
              }
              mPoolablePool.clear();
              mPoolSize = 0;
          }
          /** Creates a new query and adds it to this set.
          @remarks
          Behavior once the query pool has been exhausted depends on the
          PoolSet::setAutoextendPool option.         
          @returns
          On success, a pointer to a newly created Poolable is
          returned.
          @par
          On failure (i.e. no more space and can't autoextend),
          <b>NULL</b> is returned.
          @see
          PoolSet::setAutoextend
          */
          T* getPoolable() 
          {
              if (mFreePoolables.empty())
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
              T* newPoolable = mFreePoolables.front();
              mFreePoolables.pop_front();
              mActivePoolables.push_back(newPoolable); 

              return newPoolable;
          }

          /** Removes a query from the set.
          @note
          This version is more efficient than removing by index.
          */
          virtual void removePoolable(T* pPoolable)
          {
              mActivePoolables.remove(pPoolable);
              mFreePoolables.push_back(pPoolable);
          }

          /** Returns the number of active queries which currently make up this set.
          */
          virtual size_t getNumPooled(void) const
          {
              return static_cast< size_t >(mActivePoolables.size());
          };

          /** Tells the set whether to allow automatic extension of the pool of queries.
          @remarks
          A PoolSet stores a pool of pre-constructed queries which are used as needed when
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
          virtual void setAutoextend(bool autoextend)
          {
              mAutoExtendPool = autoextend;
          }

          /** Returns true if the query pool automatically extends.
          @see
          PoolSet::setAutoextend
          */
          virtual bool getAutoextend(void) const
          {
              return mAutoExtendPool;
          }


          /** Adjusts the size of the pool of queries available in this set.
          @remarks
          See the PoolSet::setAutoextend method for full details of the query pool. This method adjusts
          the preallocated size of the pool. If you try to reduce the size of the pool, the set has the option
          of ignoring you if too many queries are already in use. Bear in mind that calling this method will
          incur significant construction / destruction calls so should be avoided in time-critical code. The same
          goes for auto-extension, try to avoid it by estimating the pool size correctly up-front.
          @param
          size The new size for the pool.
          */
          virtual void setPoolSize(const unsigned int size)
          {
              // Never shrink below size()
              const size_t currSize = mPoolablePool.size();

              if(currSize < size)
              {
                  increasePool(size);

                  for (size_t i = currSize; i < size; ++i)
                  {
                      // Add new items to the queue
                      mFreePoolables.push_back (mPoolablePool[i]);
                  }

                  mPoolSize = size;
              }
          }
          /** Returns the current size of the query pool.
          @returns
          The current size of the query pool.
          @see
          PoolSet::setAutoextend
          */
          virtual unsigned int getPoolSize(void) const
          {
              return static_cast< unsigned int >(mPoolablePool.size());
          }

          /** Empties this set of all queries.
          */
          virtual void clear()
          {
              // Insert actives into free list
              mFreePoolables.insert(mFreePoolables.end(), mActivePoolables.begin(), mActivePoolables.end());

              // Remove all active instances
              mActivePoolables.clear(); 
          }

    protected:
        /** Active query list.
        @remarks
        This is a linked list of pointers to queries in the query pool.
        @par
        This allows very fast insertions and deletions from anywhere in the list to activate / deactivate queries
        (required for particle systems etc)    as well as reuse of Poolable instances in the pool
        without construction & destruction which avoids memory thrashing.
        */
        ActivePoolableList mActivePoolables;

        /** Free query queue.
        @remarks
        This contains a list of the queries free for use as new instances
        as required by the set. Poolable instances are pre-constructed up to the estimated size in the
        mPoolablePool vector and are referenced on this deque at startup. As they get used this deque
        reduces, as they get released back to to the set they get added back to the deque.
        */
        FreePoolableList mFreePoolables;

        /** Pool of query instances for use and reuse in the active query list.
        @remarks
        This vector will be preallocated with the estimated size of the set,and will extend as required.
        */
        PoolablePool mPoolablePool;


        /// Internal method for increasing pool size
        virtual void increasePool(unsigned int size)
        {
            size_t oldSize = mPoolablePool.size();

            // Increase size
            mPoolablePool.reserve(size);
            mPoolablePool.resize(size);

            // Create new queries
            for (size_t i = oldSize; i < size; ++i)
                mPoolablePool[i] = allocate();

        }


        /// The number of query in the pool.
        unsigned int mPoolSize;
        /// Flag indicating whether to autoextend pool
        bool mAutoExtendPool;

    protected :
        virtual T* allocate() = 0;
        virtual void deallocate(T* p) = 0;

    };

}

#endif//__OgrePagingLandScapePoolSet_H__
