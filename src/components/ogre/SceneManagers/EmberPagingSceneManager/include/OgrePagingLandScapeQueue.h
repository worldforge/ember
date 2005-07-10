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

namespace Ogre 
{

	/** This class holds classes T given to it by the plugin in a FIFO queue. */
    template<class T>
    class PagingLandScapeQueue
    {
	
           //typedef std::queue<T *, std::list<T *> > MsgQueType;
           typedef std::list<T *>  MsgQueType;

        public:
	
            //-----------------------------------------------------------------------
            PagingLandScapeQueue( )
            {
            };
            //-----------------------------------------------------------------------
            virtual ~PagingLandScapeQueue( )
            {
//	            while ( !mQueue.empty ()  )
//	            { 
//                   // Queue is not empty so get a pointer to the
//                   // first message in the queue
//		            T* tmp = mQueue.front( );
//                    // Now remove the pointer from the message queue
//		            mQueue.pop( );                   
//	            }
                mQueue.clear ();
            };
            void clear ()
            {
                mQueue.clear ();
            };
            //-----------------------------------------------------------------------
            void push( T* e )
            {
                // Insert the element at the end of the queue
		mQueue.push_back ( e );
            };
            //-----------------------------------------------------------------------
            void remove(T* e)
            {
	            mQueue.remove (e);
            };
            //-----------------------------------------------------------------------
            T *find_nearest(const uint x, const uint z)
            {
                T *p = 0;
                uint mindist = -1;
		typename MsgQueType::iterator q, qend = mQueue.end ();
                for (q = mQueue.begin (); 
                        q != qend; 
                        ++q)
                {
                    uint lx, lz;
                    (*q)->getCoordinates(lx, lz);

                    uint res = abs (static_cast <int> (lx - x)) + abs (static_cast <int> (lz - z));
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
            T *find_farest(const uint x, const uint z)
            {
                T *p = 0;
                uint maxdist = -1;
                typename MsgQueType::iterator q, qend = mQueue.end ();
                 for (q = mQueue.begin (); 
                        q != qend; 
                        ++q)
                    {
                        uint lx, lz;
                        (*q)->getCoordinates(lx, lz);

                        uint res = abs (lx - x) + abs (lz - z);
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
//            //-----------------------------------------------------------------------
//            void update(PagingLandScapePageManager *pm)
//            {
//                MsgQueType::iterator lend = mQueue.end ();
//                for (MsgQueType::iterator l = mQueue.begin (); 
//                    l != lend; 
//                    ++l)
//                {
//                    if ((*l)->touched ())
//                        pm->removeFromQueues (*l);         
//                }    
//            };
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
            int getSize() const
            {
	            return (int)mQueue.size ();
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
