/*
 * Copyright (C) 2012 Peter Szucs <peter.szucs.dev@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "PMInjectorSignaler.h"
#include <OgreFrameListener.h>

template<>
Ember::OgreView::Lod::PMInjectorSignaler * Ember::Singleton<Ember::OgreView::Lod::PMInjectorSignaler>::ms_Singleton = 0;

namespace Ember
{
namespace OgreView
{
namespace Lod
{

bool PMInjectorSignaler::frameStarted(const Ogre::FrameEvent& evt)
{
	OGRE_LOCK_MUTEX(this->OGRE_AUTO_MUTEX_NAME);
	while (!readyLods.empty()) {
		PMGenRequest* request = readyLods.top();
		inject(request);
		LodInjected.emit(&request->config);
		delete request;
		readyLods.pop();
	}
	return true;
}

}
}
}
