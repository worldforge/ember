/*
 * Copyright (c) 2013 Peter Szucs <peter.szucs.dev@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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
};

}
}
}
#endif // ifndef EMBEROGREROOT_H
