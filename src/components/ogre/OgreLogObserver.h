//
// C++ Interface: OgreLogObserver
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
#ifndef EMBEROGREOGRELOGOBSERVER_H
#define EMBEROGREOGRELOGOBSERVER_H

#include "EmberOgrePrerequisites.h"
#include "framework/StreamLogObserver.h"

namespace EmberOgre {

/**
@author Erik Hjortsberg
A log observer which writes to the Ogre log system.
This is a combined Ogre::LogListener and a Ember::StreamLogObserver.
The Ember::StreamLogObserver part does the main work, while the Ogre::LogListener implementation allow us to recieve ogre log events.
*/
class OgreLogObserver: public Ogre::LogListener
{
	public:
		/**
		* Creates a new OgreLogObserver using default values.
		*/
		OgreLogObserver();
		~OgreLogObserver();
		virtual void messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName );
		
	protected:
		
	
		
};

}

#endif
