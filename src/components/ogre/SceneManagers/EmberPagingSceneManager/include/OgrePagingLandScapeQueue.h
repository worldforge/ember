/***************************************************************************
	OgrePagingLandScapeQueue.h  -  description
  -------------------
  begin                : Mon Aug 04 2003
  copyright            : (C) 2003 by Jose A. Milan
  email                : spoke2@supercable.es
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as        *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
*                                                                         *
***************************************************************************/

#ifndef PAGINGLANDSCAPEQUEUE_H
#define PAGINGLANDSCAPEQUEUE_H

#include <queue>
#include <list>

#include "Ogre.h"
#include "OgreSceneNode.h"
#include "OgreVector3.h"


namespace Ogre 
{
	//-----------------------------------------------------------------------
	inline Real vectorToBoxDistance(const AxisAlignedBox &bbox, const Vector3& point)
	{
		const Vector3 &boxMin = bbox.getMinimum ();
		const Vector3 halfSize ((bbox.getMaximum () - boxMin) * 0.5);
		// work in the box's coordinate system
		const Vector3 kDiff (point - (halfSize + boxMin));
		// compute squared distance and closest point on box
		Real fSqrDistance (0.0);
		for (unsigned int i = 0; i < 3; i++)
		{
			const Real kClosest = kDiff[i];
			const Real khalfSize = halfSize[i];
			if (kClosest < -khalfSize)
			{
				const Real fDelta = kClosest + khalfSize;
				fSqrDistance += fDelta * fDelta;
			}
			else if (kClosest > khalfSize)
			{
				const Real fDelta = kClosest - khalfSize;
				fSqrDistance += fDelta * fDelta;
			}
		}
		return fSqrDistance;
	}
	//-----------------------------------------------------------------------
	template <class T>
	class  distanceToBoxSort
	{
	public:
		//-----------------------------------------------------------------------
		distanceToBoxSort(const Vector3 &camPos) : mCamPos(camPos)
		{};
		//-----------------------------------------------------------------------
		bool operator()(T* x, T* y)
		{ 
			
			return (x->getCenter() - mCamPos).squaredLength() <
					(y->getCenter() - mCamPos).squaredLength();

			//return vectorToBoxDistance (x->getWorldBbox(), mCamPos) <
			//		vectorToBoxDistance (y->getWorldBbox(), mCamPos);
		}
	private:
		const Vector3 mCamPos;
	};
	//-----------------------------------------------------------------------
	/** This class holds classes T given to it by the plugin in a FIFO queue. */
    template<class T>
    class PagingLandScapeQueue
    {
	
        public:
	
           //typedef std::queue<T *, std::list<T *> > MsgQueType;
			typedef std::list<T *>  MsgQueType;

            //-----------------------------------------------------------------------
            PagingLandScapeQueue( )
            {
            };
            //-----------------------------------------------------------------------
            virtual ~PagingLandScapeQueue( )
            {
                mQueue.clear ();
			};
			//-----------------------------------------------------------------------
			void clear ()
            {
                mQueue.clear ();
            };
            //-----------------------------------------------------------------------
            void push( T* e )
            {
				assert ( std::find(mQueue.begin(), mQueue.end(), e) == mQueue.end());
                // Insert the element at the end of the queue
				mQueue.push_back ( e );
			};
			//-----------------------------------------------------------------------
			typename MsgQueType::iterator begin()
			{
				return mQueue.begin ();
			};
			//-----------------------------------------------------------------------
			typename MsgQueType::iterator end()
			{
				return mQueue.end ();
			};
			//-----------------------------------------------------------------------
			typename MsgQueType::iterator erase(typename MsgQueType::iterator it)
			{
				return mQueue.erase (it);
			};
            //-----------------------------------------------------------------------
            void remove (T* e)
            {
	            mQueue.remove (e);
			};
			//-----------------------------------------------------------------------
			void sortNearest(const Vector3 &pos)
			{
				mQueue.sort (distanceToBoxSort <T>(pos));
			};
			//-----------------------------------------------------------------------
			T *find_nearest(const Vector3 &pos)
			{
				T *p = 0;
				Real mindist = std::numeric_limits<Real>::max ();
				typename MsgQueType::iterator q, qend = mQueue.end ();
				for (q = mQueue.begin (); 
					q != qend; 
					++q)
				{
					const Real res = (pos - (*q)->getCenter()).squaredLength();
					//const Real res = vectorToBoxDistance ((*q)->getSceneNode()->_getWorldAABB(), pos);					
					if (res < mindist)
					{
						mindist = res;
						p = (*q);
					}
				}
				if (p)
					mQueue.remove (p);
				return p;
			};
            //-----------------------------------------------------------------------
            T *find_nearest(const unsigned int x, const unsigned int z)
            {
                T *p = 0;
                unsigned int mindist = 0;
				typename MsgQueType::iterator q, qend = mQueue.end ();
                for (q = mQueue.begin (); 
                        q != qend; 
                        ++q)
                {
                    unsigned int lx, lz;
                    (*q)->getCoordinates(lx, lz);

                    const unsigned int res = abs (static_cast <int> (lx - x)) + abs (static_cast <int> (lz - z));
                    if (res < mindist)
                    {
                        mindist = res;
                        p = (*q);
                    }
                }
                if (p)
                   mQueue.remove (p);
                return p;
            };
            //-----------------------------------------------------------------------
            T *find_farest(const unsigned int x, const unsigned int z)
            {
                T *p = 0;
                unsigned int maxdist = -1;
                typename MsgQueType::iterator q, qend = mQueue.end ();
                 for (q = mQueue.begin (); 
                        q != qend; 
                        ++q)
                    {
                        unsigned int lx, lz;
                        (*q)->getCoordinates(lx, lz);

                        const unsigned int res = abs ((int)(lx - x)) + abs ((int)(lz - z));
                        if (res > maxdist)
                        {
                            maxdist = res;
                            p = (*q);
                        }
                    }
                
                if (p)
                   mQueue.remove (p);
                return p;
            }; 
            //-----------------------------------------------------------------------
            T* pop( )
            {
	            T* tmp = 0;
	            if (  !mQueue.empty () )
	            {
		            // Queue is not empty so get a pointer to the
                    // first message in the queue
		            tmp = mQueue.front( );

                    // Now remove the pointer from the message queue
		            mQueue.pop_front( );
	            }
	            return tmp;
            };
            //-----------------------------------------------------------------------
            size_t getSize() const
            {
	            return mQueue.size ();
            };
            //-----------------------------------------------------------------------
            bool empty() const
            {
	            return mQueue.empty( );
            };            
            //-----------------------------------------------------------------------
            MsgQueType *getQueue()
            {
                return &mQueue;    
            }
	
        protected:
	       MsgQueType mQueue;

    };

} //namespace

#endif //PAGINGLANDSCAPEQUEUE_H
