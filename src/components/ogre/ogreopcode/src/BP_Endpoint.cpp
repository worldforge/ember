/*
* SOLID - Software Library for Interference Detection
* Copyright (c) 2001 Dtecta <gino@dtecta.com>
*
* All rights reserved.
*/

#include <assert.h>
#include <new>
#include "BP_Endpoint.h"
#include "BP_Proxy.h"
#include "BP_Scene.h"

namespace OgreOpcode
{
	namespace Details
	{

		BP_Endpoint::BP_Endpoint(Ogre::Real pos, Type type, BP_Proxy *proxy, 
			GEN_List& endpointList) :
		m_pos(pos), m_type(type), m_proxy(proxy)
		{
			GEN_Link *next = endpointList.getHead();

			while (!next->isTail() && (*(BP_Endpoint *)next < *this)) {
				next = next->getNext();
			}
			insertBefore(next);       	
		}

		BP_Endpoint::~BP_Endpoint()
		{ 
			if (m_proxy != 0) {
				remove();
			}
		}

		void encounters(const BP_Endpoint& a, const BP_Endpoint& b,
			BP_Scene& scene,	T_Overlap overlap)
		{
			assert(a.m_proxy != b.m_proxy);

			if ((a.m_type != b.m_type) && overlap(*a.m_proxy, *b.m_proxy)) {
				if (a.m_type == BP_Endpoint::MAXIMUM) {
					scene.callBeginOverlap(a.m_proxy->getObject(), 
						b.m_proxy->getObject());
				}
				else {
					scene.callEndOverlap(a.m_proxy->getObject(), 
						b.m_proxy->getObject());
				}
			}
		}

		void BP_Endpoint::move(Ogre::Real x, BP_Scene& scene, T_Overlap overlap)
		{
			int sign = MT_sign(x - m_pos);

			m_pos = x;

			switch (sign) {
	case -1: {
		GEN_Link *prev = getPrev();
		if (!prev->isHead() && (*this < *(BP_Endpoint *)prev)) {
			remove();
			do {
				encounters(*(BP_Endpoint *)prev, *this, scene, overlap);
				prev = prev->getPrev();
			}
			while (!prev->isHead() && (*this < *(BP_Endpoint *)prev));
			insertAfter(prev);
		}
		break;
			 }
	case 1: {
		GEN_Link *next = getNext();
		if (!next->isTail() && (*(BP_Endpoint *)next < *this)) {
			remove();
			do {
				encounters(*this, *(BP_Endpoint *)next, scene, overlap);
				next = next->getNext();
			}
			while (!next->isTail() && (*(BP_Endpoint *)next < *this));
			insertBefore(next);   
		}
		break;
			}
	case 0:
		// nothing to do 
		break;
	default:
		assert(false);
			}
		}

	}
}





