#ifndef _PagingLandScapeOcclusionSorterH
#define _PagingLandScapeOcclusionSorterH

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeOcclusionElement.h"

namespace Ogre
{
   
    //comparator for priority queue
    class FrontToBackNodeSorterOperator 
    {

	    public:
			FrontToBackNodeSorterOperator(const Vector3& position) :   pos(position) {};
			//-----------------------------------------------------------------------
		    inline bool operator()(OcclusionElement*& a, OcclusionElement*& b) const 
            {
                // could check if both distance == 0 then order visible first// ?
		        return vectorToBoxDistance (*a, pos) > vectorToBoxDistance (*b, pos);
			}       
			//-----------------------------------------------------------------------
			inline bool operator()(const OcclusionElement*& a, const OcclusionElement*& b) const 
			{
				// could check if both distance == 0 then order visible first ?
				return vectorToBoxDistance (*a, pos) > vectorToBoxDistance (*b, pos);
			}       
			//-----------------------------------------------------------------------
            Real vectorToBoxDistance(const OcclusionElement &a, const Vector3& point)  const
	        {
                const Vector3 &boxMin = a.getCullBoundingBox ().getMinimum ();
                const Vector3 halfSize ((a.getCullBoundingBox ().getMaximum () - boxMin) * 0.5);
                // work in the box's coordinate system
                const Vector3 kDiff (point - (halfSize + boxMin));
                // compute squared distance and closest point on box
                Real fSqrDistance (0.0);
                for (uint i = 0; i < 3; i++)
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

        private:
		    const Vector3 pos;
    };
}//namespace Ogre

#endif //_PagingLandScapeOcclusionSorterH
