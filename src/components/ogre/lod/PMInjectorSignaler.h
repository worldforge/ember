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

#ifndef PMINJECTORSIGNALER_H
#define PMINJECTORSIGNALER_H

#include "QueuedProgressiveMeshGenerator.h"
#include "components/ogre/EmberOgrePrerequisites.h"
#include "framework/Singleton.h"
#include <sigc++/signal.h>

namespace Ember
{
namespace OgreView
{
namespace Lod
{

/**
 * @brief Extends PMInjector with sigc++ signaling, when Lod is injected into a mesh.
 */
class PMInjectorSignaler :
	public Ember::Singleton<PMInjectorSignaler>,
	public PMInjector
{
public:
	sigc::signal<void, LodConfig*> LodInjected;

	bool frameStarted(const Ogre::FrameEvent& evt);
};

}
}
}
#endif // ifndef PMINJECTORSIGNALER_H
