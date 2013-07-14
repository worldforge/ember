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

#ifndef PMINJECTORSIGNALER_H
#define PMINJECTORSIGNALER_H


#include "components/ogre/EmberOgrePrerequisites.h"
#include "framework/Singleton.h"

#include <OgreQueuedProgressiveMeshGenerator.h>

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
	public Ogre::PMInjectorListener
{
public:
	sigc::signal<void, Ogre::LodConfig*> LodInjected;

private:
	bool shouldInject(Ogre::PMGenRequest* request);
	void injectionCompleted(Ogre::PMGenRequest* request);
};

}
}
}
#endif // ifndef PMINJECTORSIGNALER_H
