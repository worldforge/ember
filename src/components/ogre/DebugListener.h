/*
    Copyright (C) 2004  Erik Hjortsberg
    Some parts Copyright © 2000-2003 The OGRE Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/



#ifndef DEBUGLISTENER_H
#define DEBUGLISTENER_H

#include <Ogre.h>
#include <OgreFrameListener.h>
#include <OgreEventListeners.h>
#include <OgreStringConverter.h>

namespace DimeOgre {
	
class DebugListener : public Ogre::FrameListener {
public:
	static DebugListener & getSingleton(void);

	DebugListener();
	virtual ~DebugListener();
	
    void initialise(Ogre::InputReader* inputReader);
	
	bool frameStarted(const Ogre::FrameEvent& event);
	bool frameEnded(const Ogre::FrameEvent& event);
	
	void setString(const Ogre::String string);
private:
    static DebugListener* _instance;
	void updateStats(void);
	Ogre::String mString;
	Ogre::GuiElement* mDebugElement;
};

}

#endif // DEBUGLISTENER_H
