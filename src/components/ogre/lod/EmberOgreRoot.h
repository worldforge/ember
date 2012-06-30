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

#ifndef EMBEROGREROOT_H
#define EMBEROGREROOT_H

#include <OgreRoot.h>

namespace Ember
{
namespace OgreView
{
namespace Lod
{

class EmberOgreMeshManager;

/**
 * @brief This wraps Ogre::Root to modify Ogre::MeshManager.
 *
 * This is a temporary solution, until Ogre 1.9 is released.
 */
class EmberOgreRoot :
	public Ogre::Root
{
public:

	/**
	 * @brief Ctor.
	 *
	 * @param pluginFileName The file that contains plugins information.
	 *                       Defaults to "plugins.cfg", may be left blank to ignore.
	 * @param configFileName The file that contains the configuration to be loaded.
	 *                       Defaults to "ogre.cfg", may be left blank to load nothing.
	 * @param logFileName    The logfile to create, defaults to Ogre.log, may be
	 *                       left blank if you've already set up LogManager & Log yourself
	 */
	EmberOgreRoot(const Ogre::String& pluginFileName = "plugins.cfg",
	              const Ogre::String& configFileName = "ogre.cfg",
	              const Ogre::String& logFileName = "Ogre.log");

	virtual ~EmberOgreRoot();


private:

	/**
	 * @brief Holds the derived MeshManager.
	 *
	 * It is not allowed to delete a class with the base class pointer,
	 * so we need a separated pointer from mMeshManager.
	 * It would work in this case, but if this class would have multiple
	 * base classes it would crash.
	 */
	EmberOgreMeshManager* mEmberOgreMeshManager;
};

}
}
}
#endif // ifndef EMBEROGREROOT_H
