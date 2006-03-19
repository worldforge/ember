#ifndef __PagingLandScapeCallBackEvent_H__
#define __PagingLandScapeCallBackEvent_H__

#include "Ogre.h"
#include "OgrePagingLandScapeCallback.h"

namespace Ogre
{
	//---------------------------------------------------------------------
	class PagingLandscapeEvent
	{
	public:  
		PagingLandscapeEvent(const size_t pagex, const size_t pagez, 
			const size_t tilex, const size_t tilez, 
			const Real* heightData, const AxisAlignedBox &Bbox) :
			mPagex(pagex),
			mPagez(pagez),
			mTilex(tilex),
			mTilez(tilez),
			mHeightData(heightData),
			mBbox(Bbox)
		{ 

		}; 
		~PagingLandscapeEvent(){};

		const size_t mPagex;
		const size_t mPagez;
		const size_t mTilex;
		const size_t mTilez;
		const Real* mHeightData;
		const AxisAlignedBox &mBbox;

	};
	typedef fastdelegate::FastDelegate1<PagingLandscapeEvent *> PagingLandscapeDelegate;
}
#endif //__PagingLandScapeCallBackEvent_H__
