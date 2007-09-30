//
// C++ Implementation: OpcodeCollisionDetectorVisualizer
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2007
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#include "OpcodeCollisionDetectorVisualizer.h"
#include "ogreopcode/include/OgreOpcodeDebugObject.h"
#include "ogreopcode/include/OgreCollisionManager.h"
#include "ogreopcode/include/OgreEntityCollisionShape.h"
#include "ogreopcode/include/OgreCollisionObject.h"
#include "OpcodeCollisionDetector.h"

namespace EmberOgre {

template<> EmberOgre::OpcodeCollisionDetectorVisualizer* Ember::Singleton<EmberOgre::OpcodeCollisionDetectorVisualizer>::ms_Singleton = 0;

OpcodeCollisionDetectorVisualizer::OpcodeCollisionDetectorVisualizer()
{
	mOpcodeDebugger = new OgreOpcode::Details::OgreOpcodeDebugger("OpcodeCollisionDetectorVisualizer", OgreOpcode::CollisionManager::getSingletonPtr()->getSceneManager());

}

OpcodeCollisionDetectorVisualizer::~OpcodeCollisionDetectorVisualizer()
{
	Ogre::Root::getSingleton().removeFrameListener(this);
	mOpcodeDebugger->clearAll();
	delete mOpcodeDebugger;
}

bool OpcodeCollisionDetectorVisualizer::frameStarted(const Ogre::FrameEvent& event)
{
	mOpcodeDebugger->clearAll();
	for (VisualizerInstanceStore::iterator I = mInstances.begin(); I != mInstances.end(); ++I) {
		(*I)->visualize(mOpcodeDebugger);
	}
	return true;
}

void OpcodeCollisionDetectorVisualizer::addInstance(OpcodeCollisionDetectorVisualizerInstance* instance)
{
	if (!mInstances.size()) {
		Ogre::Root::getSingleton().addFrameListener(this);
	}
	mInstances.push_back(instance);
}

void OpcodeCollisionDetectorVisualizer::removeInstance(OpcodeCollisionDetectorVisualizerInstance* instance)
{
	VisualizerInstanceStore::iterator I = std::find(mInstances.begin(), mInstances.end(), instance);
	if (I != mInstances.end()) {
		mInstances.erase(I);
	}
	if (!mInstances.size()) {
		mOpcodeDebugger->clearAll();
		Ogre::Root::getSingleton().removeFrameListener(this);
	}
}



OpcodeCollisionDetectorVisualizerInstance::OpcodeCollisionDetectorVisualizerInstance(OpcodeCollisionDetector& detector) : mDetector(detector)
{
	OpcodeCollisionDetectorVisualizer::getSingleton().addInstance(this);
}

OpcodeCollisionDetectorVisualizerInstance::~OpcodeCollisionDetectorVisualizerInstance()
{
	for (OpcodeCollisionDetector::CollisionObjectStore::iterator I = mDetector.mCollisionObjects.begin(); I != mDetector.mCollisionObjects.end(); ++I)
	{
		(*I)->getShape()->clearViz();
	}
	OpcodeCollisionDetectorVisualizer::getSingleton().removeInstance(this);
}

void OpcodeCollisionDetectorVisualizerInstance::visualize(OgreOpcode::Details::OgreOpcodeDebugger* debugger)
{
	for (OpcodeCollisionDetector::CollisionObjectStore::iterator I = mDetector.mCollisionObjects.begin(); I != mDetector.mCollisionObjects.end(); ++I)
	{
		(*I)->getShape()->clearViz();
		(*I)->getShape()->visualize(debugger);
	}
}


}
