/*
* SOLID - Software Library for Interference Detection
* Copyright (c) 2001 Dtecta <gino@dtecta.com>
*
* All rights reserved.
*/

#include <new>

#include "BP_Proxy.h"
#include "BP_Scene.h"

namespace OgreOpcode
{
	namespace Details
	{

		BP_Proxy::BP_Proxy(void *object, BP_Scene& scene, 
			const Ogre::Vector3& min,
			const Ogre::Vector3& max) : 
		m_object(object),
			m_scene(scene)
		{
			int i;
			for (i = 0; i < 3; ++i) {
				new (&m_min[i]) BP_Endpoint(min[i], BP_Endpoint::MINIMUM, 
					this, scene.getLists()[i]);
				new (&m_max[i]) BP_Endpoint(max[i], BP_Endpoint::MAXIMUM,
					this, scene.getLists()[i]);
			}
		}

		bool overlapXY(const BP_Proxy& a, const BP_Proxy& b)
		{
			return a.getMin(0) <= b.getMax(0) && b.getMin(0) <= a.getMax(0) && 
				a.getMin(1) <= b.getMax(1) && b.getMin(1) <= a.getMax(1);
		}

		bool overlapXZ(const BP_Proxy& a, const BP_Proxy& b)
		{
			return a.getMin(0) <= b.getMax(0) && b.getMin(0) <= a.getMax(0) && 
				a.getMin(2) <= b.getMax(2) && b.getMin(2) <= a.getMax(2); 
		}

		bool overlapYZ(const BP_Proxy& a, const BP_Proxy& b)
		{
			return a.getMin(1) <= b.getMax(1) && b.getMin(1) <= a.getMax(1) && 
				a.getMin(2) <= b.getMax(2) && b.getMin(2) <= a.getMax(2); 
		}

		void BP_Proxy::setBBox(const Ogre::Vector3& min, const Ogre::Vector3& max)
		{	
			static T_Overlap overlap[3] = { overlapYZ, overlapXZ, overlapXY };

			int i;
			for (i = 0; i < 3; ++i) {
				if (min[i] > m_max[i].getPos()) {
					m_max[i].move(max[i], m_scene, overlap[i]);
					m_min[i].move(min[i], m_scene, overlap[i]);
				}
				else {
					m_min[i].move(min[i], m_scene, overlap[i]);
					m_max[i].move(max[i], m_scene, overlap[i]);
				}
			}
		}

	}
}



