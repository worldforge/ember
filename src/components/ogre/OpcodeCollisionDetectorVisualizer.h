//
// C++ Interface: OpcodeCollisionDetectorVisualizer
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
#ifndef EMBEROGREOPCODECOLLISIONDETECTORVISUALIZER_H
#define EMBEROGREOPCODECOLLISIONDETECTORVISUALIZER_H

#include "framework/Singleton.h"
#include <Ogre.h>

namespace OgreOpcode
{
	class CollisionObject;
	namespace Details 
	{
		class OgreOpcodeDebugger;
	}
};

namespace EmberOgre {

class OpcodeCollisionDetector;
class OpcodeCollisionDetectorVisualizerInstance;

/**
Helps with visualizing the collision objects. Create and register instances of OpcodeCollisionDetectorVisualizerInstance to visualize entities.
@see OpcodeCollisionDetectorVisualizerInstance
*/
class OpcodeCollisionDetectorVisualizer : public Ember::Singleton<OpcodeCollisionDetectorVisualizer>, public Ogre::FrameListener
{
public:
	friend class OpcodeCollisionDetectorVisualizerInstance;
	/**
	 *    Default ctor.
	 */
	OpcodeCollisionDetectorVisualizer();
	virtual ~OpcodeCollisionDetectorVisualizer();
	/**
	 * Methods from Ogre::FrameListener
	 */
	virtual bool frameStarted(const Ogre::FrameEvent& event);
	
	/**
	 *    Registers an instance of OpcodeCollisionDetectorVisualizerInstance to be rendered each frame.
	 * @param instance An instance of OpcodeCollisionDetectorVisualizerInstance which in turn points to an instance of EmberPhysicalEntity.
	 */
	void addInstance(OpcodeCollisionDetectorVisualizerInstance* instance);
	/**
	 *    Removes an instance of OpcodeCollisionDetectorVisualizerInstance which will no longer be rendered each frame.
	 * @param instance An instance of OpcodeCollisionDetectorVisualizerInstance which in turn points to an instance of EmberPhysicalEntity.
	 */
	void removeInstance(OpcodeCollisionDetectorVisualizerInstance* instance);
protected:
	typedef std::vector<OpcodeCollisionDetectorVisualizerInstance*> VisualizerInstanceStore;
	
	/**
	* The debugger object responsible for rendering.
	*/
	OgreOpcode::Details::OgreOpcodeDebugger* mOpcodeDebugger;
	
	/**
	* All the registered instances which will be rendered each frame.
	*/
	VisualizerInstanceStore mInstances;
};

class OpcodeCollisionDetectorVisualizerInstance 
{
public:
	OpcodeCollisionDetectorVisualizerInstance(OpcodeCollisionDetector& detector);
	virtual ~OpcodeCollisionDetectorVisualizerInstance();
	
	/**
	 * Called each frame by OpcodeCollisionDetectorVisualizer to let the object tell the debugger how to render this instance.
	 * @param debugger 
	 */
	void visualize(OgreOpcode::Details::OgreOpcodeDebugger* debugger);

protected:
	OpcodeCollisionDetector& mDetector;
};

}

#endif
