/*
    Copyright (C) 2004  Erik Hjortsberg

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



#ifndef DIMEEVENTPROCESSOR_H
#define DIMEEVENTPROCESSOR_H

#include <Ogre.h>
#include <OgreEventProcessor.h>
#include <OgreEventQueue.h>

/*
 *  @author Erik Hjortsberg
 * 
 * This class is basically a subclass of Ogre::EventProcessor with the addition 
 * of allowing the application to grab or release the mouse, something which
 * is not possible with the current EventProcessor class (as of Ogre 0.14.
 * By default the mouse is not grabbed until toggleMouse() is called.
 * 
 */

class DimeEventProcessor : public Ogre::EventProcessor {
public:

	DimeEventProcessor();
	void initialise(Ogre::RenderWindow* ren);

	/*
	 * call this method when you want to toggle the grabbing or release of the mouse
	 */
	void toggleMouse();
	
	/*
	 * takes a screenshot and writes it to a file
	 * returns the name of the screenshot
	 */
	std::string takeScreenshot(); 
	
	/*
	 * Returns true if the application has grabbed the mouse.
	 */
	bool isMouseUsed();
protected:

	void createInputReader(bool useMouse);
private:
	bool mUseMouse;
	Ogre::RenderWindow* mWin;
//	virtual ~DimeEventProcessor();
};

#endif // DIMEEVENTPROCESSOR_H
