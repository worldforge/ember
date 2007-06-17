/*
* SOLID - Software Library for Interference Detection
* Copyright (c) 2001 Dtecta <gino@dtecta.com>
*
* All rights reserved.
*/

#include "BP_Scene.h"
#include "BP_Proxy.h"

#include <algorithm>

namespace OgreOpcode
{
	namespace Details
	{

		BP_Scene::~BP_Scene()
		{
			T_ProxyList::iterator i;
			for (i = m_proxyList.begin(); !(i == m_proxyList.end()); ++i) {
				delete *i;
			}
		}


		BP_Proxy *BP_Scene::createProxy(void *object,
			const Ogre::Vector3& min,
			const Ogre::Vector3& max)
		{
			BP_Proxy *proxy = new BP_Proxy(object, *this, min, max);

			T_ProxyList::iterator i;
			for (i = m_proxyList.begin(); !(i == m_proxyList.end()); ++i) {
				if (BP_overlap(*proxy, *(*i))) {
					callBeginOverlap(proxy->getObject(), (*i)->getObject());
				}
			}

			m_proxyList.push_back(proxy);

			return proxy;
		}

		void BP_Scene::deleteProxy(BP_Proxy *proxy)
		{
			T_ProxyList::iterator i =
				std::find(m_proxyList.begin(), m_proxyList.end(), proxy);

			if (i != m_proxyList.end()) {
				m_proxyList.erase(i);

				T_ProxyList::iterator j;
				for (j = m_proxyList.begin(); !(j == m_proxyList.end()); ++j) {
					if (BP_overlap(*proxy, *(*j))) {
						callEndOverlap(proxy->getObject(), (*j)->getObject());
					}
				}

				delete proxy;
			}
		}
	}
}
