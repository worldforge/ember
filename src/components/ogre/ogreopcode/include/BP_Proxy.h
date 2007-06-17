/*
* SOLID - Software Library for Interference Detection
* Copyright (c) 2001 Dtecta <gino@dtecta.com>
*
* All rights reserved.
*/

#ifndef BP_PROXY_H
#define BP_PROXY_H

#include "BP_Endpoint.h"
namespace OgreOpcode
{
	namespace Details
	{

		class BP_Scene;

		class BP_Proxy {
		public:
			BP_Proxy(void *object, 
				BP_Scene& scene, 
				const Ogre::Vector3& min,
				const Ogre::Vector3& max);

			void setBBox(const Ogre::Vector3& min, const Ogre::Vector3& max);

			void *getObject() { return m_object; }

			Ogre::Real getMin(int i) const { return m_min[i].getPos(); }
			Ogre::Real getMax(int i) const { return m_max[i].getPos(); }

		private:
			void        *m_object;
			BP_Scene&    m_scene;
			BP_Endpoint  m_min[3];
			BP_Endpoint  m_max[3];
		};

		inline bool BP_overlap(const BP_Proxy& a, const BP_Proxy& b)
		{
			return a.getMin(0) <= b.getMax(0) && b.getMin(0) <= a.getMax(0) && 
				a.getMin(1) <= b.getMax(1) && b.getMin(1) <= a.getMax(1) &&
				a.getMin(2) <= b.getMax(2) && b.getMin(2) <= a.getMax(2);
		}

	}
}

#endif
