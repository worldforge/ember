/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 The OGRE Team
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
		poolSize The initial size of the object pool. Estimate of the number of objects
		which will be required, and pass it using this parameter. The set will
		preallocate this number to avoid memory fragmentation. The default behaviour
		once this pool has run out is to double it.            
		@see
		PoolSet::setAutoextend
		*/
		PoolSet(unsigned int poolSize = 0,  bool autoExtendPool = true):
				mAutoExtendPool(autoExtendPool),
				mAutoExtendFactor(2.0f),
				mPoolSize(poolSize)
		  {
			  assert (mFreePoolables.empty());
			  assert (mActivePoolables.empty());
			  assert (mPoolablePool.empty());

			  setPoolSize(poolSize);
		  }
		  /** 
			dtor
		  */
		  virtual ~PoolSet() 
		  {
			  assert (mPoolSize == 0 && mPoolablePool.empty());
		  }
		  /** 
			empty totally the pool.
		  */
		  void deletePool() 
		  {
			  // Free pool items
			  for (typename PoolablePool::iterator i = mPoolablePool.begin(); 
				  i != mPoolablePool.end(); ++i)
			  {   
				  deallocate (*i);
			  }
			  mPoolablePool.clear();
			  mFreePoolables.clear();
			  mActivePoolables.clear();
			  mPoolSize = 0;
		  }
		  /** Creates a new pooled object and adds it to this set.
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
					  autoExtend ();   
				  }
				  else
				  {
					  return 0;
				  }
			  }
			  // Get a new Object
			  T* newPoolable = mFreePoolables.front();
			  mFreePoolables.pop_front();
			  mActivePoolables.push_back(newPoolable); 

			  return newPoolable;
		  }
		  /*
		  Extend the pool Size by a mAutoExtendFactor factor (default 2.0f)
		  */
		  void autoExtend()
		  {
			  unsigned int newSize = (unsigned int)(mPoolSize * mAutoExtendFactor);
			  if (newSize <= mPoolSize)
				  newSize = mPoolSize + 1;
			  setPoolSize (newSize);
		  }
		  /*
		  Extend the pool Size by this factor (default is 2.0f)
		  */
		  void setExtendFactor(const float factor)
		  {
			  assert(factor >= 1.0f);// otherwise you'll make it smaller
			  mAutoExtendFactor = factor;
		  }
		  /** Removes a pooled object from the set.
		  @note
		  This version is more efficient than removing by index.
		  */
		  void removePoolable(T * const pPoolable)
		  {
			  mActivePoolables.remove (pPoolable);
			  mFreePoolables.push_back (pPoolable);
		  }
		  /** Returns the number of active object which currently make up this set.
		  */
		  size_t getActivePoolablesSize(void) const
		  {
			  return static_cast< size_t >(mActivePoolables.size());
		  }
		  ActivePoolableList &getActivePoolables(void) 
		  {
			  return mActivePoolables;
		  }
		  /** Tells the set whether to allow automatic extension of the pool of objects.
		  @remarks
		  A PoolSet stores a pool of pre-constructed queries which are used as needed when
		  a new object is requested. This allows applications to create / remove objects efficiently
		  without incurring construction / destruction costs. This method allows you to configure
		  the behavior when a new object is requested  but the object pool has been exhausted.
		  @par
		  The default behavior is to allow the pool to extend (typically this allocates double the current
		  pool of queries when the pool is expended), equivalent to calling this method with
		  autoExtend = true. If you set the parameter to false however, any attempt to create a new pooled object
		  when the pool has expired will simply fail silently, returning a null pointer.
		  @param autoextend true to double the pool every time it runs out, false to fail silently.
		  */
		  void setAutoextend(bool autoextend)
		  {
			  mAutoExtendPool = autoextend;
		  }
		  /** Returns true if the object pool automatically extends.
		  @see
		  PoolSet::setAutoextend
		  */
		  bool getAutoextend(void) const
		  {
			  return mAutoExtendPool;
		  }
		  /** Adjusts the size of the pool of object available in this set.
		  @remarks
		  See the PoolSet::setAutoextend method for full details of the query pool. This method adjusts
		  the preallocated size of the pool. If you try to reduce the size of the pool, the set has the option
		  of ignoring you if too many objects are already in use. Bear in mind that calling this method will
		  incur significant construction / destruction calls so should be avoided in time-critical code. The same
		  goes for auto-extension, try to avoid it by estimating the pool size correctly up-front.
		  @param
		  size The new size for the pool.
		  */
		  void setPoolSize(const unsigned int size)
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
		  /** Returns the current size of the object pool.
		  @returns
		  The current size of the object pool.
		  @see
		  PoolSet::setAutoextend
		  */
		  unsigned int getPoolSize(void) const
		  {
			  return static_cast< unsigned int >(mPoolablePool.size());
		  }
		  /** Empties this set of all Actives object (but do not delete them).
		  */
		  void clear()
		  {
			  // Insert actives into free list
			  mFreePoolables.insert(mFreePoolables.end(), mActivePoolables.begin(), mActivePoolables.end());

			  // Remove all active instances
			  mActivePoolables.clear(); 
		  }

	protected:
		/** Active object list.
		@remarks
		This is a linked list of pointers to objects in the object pool.
		@par
		This allows very fast insertions and deletions from anywhere in the list to activate / deactivate objects
		(required for particle systems etc) as well as reuse of Poolable instances in the pool
		without construction & destruction which avoids memory thrashing.
		*/
		ActivePoolableList mActivePoolables;
		/** Free object queue.
		@remarks
		This contains a list of the objects free for use as new instances
		as required by the set. Poolable instances are pre-constructed up to the estimated size in the
		mPoolablePool vector and are referenced on this deque at startup. As they get used this deque
		reduces, as they get released back to to the set they get added back to the deque.
		*/
		FreePoolableList mFreePoolables;
		/** Pool of objects instances for use and reuse in the active query list.
		@remarks
		This vector will be preallocated with the estimated size of the set,and will extend as required.
		*/
		PoolablePool mPoolablePool;
		/// The number of query in the pool.
		unsigned int mPoolSize;
		/// Flag indicating whether to auto extend pool
		bool mAutoExtendPool;
		// by how much we will auto extend current pool
		Real mAutoExtendFactor;

		/// method for increasing pool size
		void increasePool(unsigned int size)
		{
			assert (size > 0);
			const size_t oldSize = mPoolablePool.size();

			// Increase size
			mPoolablePool.reserve(size);
			mPoolablePool.resize(size);

			// Create new queries
			for (size_t i = oldSize; i < size; ++i)
				mPoolablePool[i] = allocate();
		}
	protected :
		virtual T* allocate() = 0;
		virtual void deallocate(T* p) = 0;

	};

}

#endif//__OgrePagingLandScapePoolSet_H__
