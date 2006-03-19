#ifndef __PagingLandScapeOcclusionTraversal_H
#define __PagingLandScapeOcclusionTraversal_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeOctreeCamera.h"

namespace Ogre
{
    //-----------------------------------------------------------------------
    class ConstTraversalConst 
    {

        public:
		    virtual void onTree(const PagingLandScapeOctree& n) const = 0;
	        virtual void onLeaf(const PagingLandScapeOctreeNode&) const = 0;

        protected:
            virtual void traverseChildren(const PagingLandScapeOctree& n) const;
    };
    //-----------------------------------------------------------------------
    class TraversalConst 
    {

        public:
		    virtual void onTree(PagingLandScapeOctree& n) const = 0;
	        virtual void onLeaf(PagingLandScapeOctreeNode&) const = 0;

        protected:
            virtual void traverseChildren(PagingLandScapeOctree& n) const ;
    };
    //-----------------------------------------------------------------------
    class Traversal 
    {

        public:
		    virtual void onTree(PagingLandScapeOctree& n) = 0;
	        virtual void onLeaf(PagingLandScapeOctreeNode&) = 0;

        protected:
            virtual void traverseChildren(PagingLandScapeOctree& n);
    };
}
#endif //PagingLandScapeOcclusionTraversal
