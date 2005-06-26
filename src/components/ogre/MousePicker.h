//
// C++ Interface: MousePicker
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2004
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
#ifndef DIMEOGREMOUSEPICKER_H
#define DIMEOGREMOUSEPICKER_H

#if SIGC_MAJOR_VERSION == 1 && SIGC_MINOR_VERSION == 0
#include <sigc++/signal_system.h>
#else
#include <sigc++/object.h>
#include <sigc++/signal.h>
#include <sigc++/slot.h>
#include <sigc++/bind.h>
#include <sigc++/object_slot.h>
#endif


#include "EmberOgrePrerequisites.h"

namespace EmberOgre {

class EmberEntity;
/**
 * Struct used for returning the result of a mouse pick.
 */
struct EntityPickResult
{
	EmberEntity* entity;
	Ogre::Vector3 position;
	Ogre::Real distance;
};

struct MousePickerArgs
{
	float windowX, windowY;
};

/**
@author Erik Hjortsberg
*/
class MousePicker : virtual public SigC::Object{
public:
    MousePicker();

    ~MousePicker();

	virtual void doMousePicking(const Ogre::Real x, const Ogre::Real y, const MousePickerArgs& args);



	SigC::Signal2<void, const EntityPickResult&, const MousePickerArgs&> EventPickedEntity;
	SigC::Signal1<void, const MousePickerArgs&> EventPickedNothing;
	
	inline EmberEntity* getLastPickedEntity() { return mLastPickedEntityResult.entity; }

	
protected:

	virtual void onEventPickedEntity(const EntityPickResult & result, const MousePickerArgs& args);
	virtual void onEventPickedNothing(const MousePickerArgs& args);

	


	//the currently selected entity
    EmberEntity* mEntityUnderCursor;
	
	//the last clicked entity
	EntityPickResult mLastPickedEntityResult;
};

}

#endif
