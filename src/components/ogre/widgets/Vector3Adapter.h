//
// C++ Interface: Vector3Adapter
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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
#ifndef EMBEROGRE_CEGUIVECTOR3ADAPTER_H
#define EMBEROGRE_CEGUIVECTOR3ADAPTER_H
#include "components/ogre/EmberOgrePrerequisites.h"

namespace CEGUI
{
	class Window;
	class EventArgs;

}

namespace EmberOgre {

namespace Gui {

/**
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class Vector3Adapter {
public:
    Vector3Adapter(CEGUI::Window *xWindow, CEGUI::Window *yWindow, CEGUI::Window *zWindow, const Ogre::Vector3& vector = Ogre::Vector3::ZERO);

    ~Vector3Adapter();
    
    const Ogre::Vector3& getValue() const;
    const Ogre::Vector3& getOriginalValue() const;
    
     /**
    Sets the value, thus also updating the gui.
    */
    void setValue(const Ogre::Vector3& vector);
    
     /**
    Updates the gui with new values.
    */
   void updateGui(const Ogre::Vector3& vector);
    
    /**
    	Emitted when the value has been changed from a gui update.
    */
    sigc::signal<void> EventValueChanged;
protected:

	mutable Ogre::Vector3 mVector;
	Ogre::Vector3 mOriginalVector;

	/**
	The windows which holds the values.
	*/
	CEGUI::Window *mXWindow, *mYWindow, *mZWindow;
	
	bool window_TextChanged(const CEGUI::EventArgs& e);
	
	bool mSelfUpdate;

};

}

}

#endif
