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


#include <Ogre.h>
#include <CEGUISystem.h>
#include <CEGUIInputEvent.h> 

namespace DimeOgre {

class DimeEntity;

/**
@author Erik Hjortsberg
*/
class MousePicker : virtual public SigC::Object{
public:
    MousePicker();

    ~MousePicker();

	void doMousePicking(Ogre::MouseEvent* ogreMouseEvent, const CEGUI::MouseEventArgs& args);



	SigC::Signal2<void, DimeEntity*, const CEGUI::MouseEventArgs&> EventPickedEntity;
	SigC::Signal1<void, const CEGUI::MouseEventArgs&> EventPickedNothing;
	
	inline DimeEntity* getLastPickedEntity() { return mLastPickedEntity; }

	
protected:

	virtual void onEventPickedEntity(DimeEntity* entity, const CEGUI::MouseEventArgs& args);
	virtual void onEventPickedNothing(const CEGUI::MouseEventArgs& args);

	


	//the currently selected entity
    DimeEntity* mEntityUnderCursor;
	
	//the last clicked entity
    DimeEntity* mLastPickedEntity;

};

}

#endif
