//
// C++ Interface: QuaternionAdapter
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
#ifndef EMBEROGRE_CEGUIQUATERNIONADAPTER_H
#define EMBEROGRE_CEGUIQUATERNIONADAPTER_H
#include "components/ogre/EmberOgrePrerequisites.h"
#include "Vector3Adapter.h"

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
class QuaternionAdapter{
public:
    QuaternionAdapter(CEGUI::Window *degreeWindow, CEGUI::Window *xWindow, CEGUI::Window *yWindow, CEGUI::Window *zWindow, const Ogre::Quaternion& quaternion = Ogre::Quaternion::ZERO);

    ~QuaternionAdapter();
    
    
    const Ogre::Quaternion& getValue() const;
    const Ogre::Quaternion& getOriginalValue() const;
    
    
    /**
    Sets the value, thus also updating the gui.
    */
    void setValue(const Ogre::Quaternion& quaternion);
    
    /**
    Updates the gui with new values.
    */
    void updateGui(const Ogre::Quaternion& vector);
    
    /**
    	Emitted when the value has been changed from a gui update.
    */
    sigc::signal<void> EventValueChanged;

private:

	mutable Ogre::Quaternion mQuaternion;
	Ogre::Quaternion mOriginalQuaternion;

	/**
	A Vector3Adapter which takes care of the vector part.
	*/
	Vector3Adapter mVectorAdapter;
	//CEGUI::Window mXWindow, m

	/**
	The window which holds the degree value.
	*/
	CEGUI::Window *mDegreeWindow;

	bool window_TextChanged(const CEGUI::EventArgs& e);
	
	bool mSelfUpdate;

	void vectorAdapter_ValueChanged();

};
}

}

#endif
