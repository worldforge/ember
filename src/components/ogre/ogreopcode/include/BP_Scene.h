/*
* SOLID - Software Library for Interference Detection
* Copyright (c) 2001 Dtecta <gino@dtecta.com>
*
* All rights reserved.
*/

#ifndef BP_SCENE_H
#define BP_SCENE_H

#include <vector>

#include "OgreBroadPhase.h"
#include "GEN_List.h"
namespace OgreOpcode
{
	namespace Details
	{

		class BP_Proxy;
		class Ogre::Vector3;

		class BP_Scene {
		public:
			BP_Scene(void *client_data,
				BP_Callback beginOverlap,
				BP_Callback endOverlap) :
			m_client_data(client_data),
				m_beginOverlap(beginOverlap),
				m_endOverlap(endOverlap) {}

			~BP_Scene();

			BP_Proxy *createProxy(void *object, 
				const Ogre::Vector3& min,
				const Ogre::Vector3& max);

			void         deleteProxy(BP_Proxy *proxy);

			void callBeginOverlap(void *object1, void *object2) {
				m_beginOverlap(m_client_data, object1, object2);
			}

			void callEndOverlap(void *object1, void *object2) {
				m_endOverlap(m_client_data, object1, object2);
			}

			GEN_List *getLists() { return m_endpointList; }

		private:
			typedef std::vector<BP_Proxy *> T_ProxyList;

			void        *m_client_data;
			BP_Callback  m_beginOverlap; 
			BP_Callback  m_endOverlap; 
			T_ProxyList  m_proxyList;
			GEN_List     m_endpointList[3];
		};
	}
}

#endif







