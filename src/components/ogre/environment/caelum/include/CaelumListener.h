/*
This file is part of Caelum.
See http://www.ogre3d.org/wiki/index.php/Caelum 

Copyright (c) 2006-2007 Caelum team. See Contributors.txt for details.

Caelum is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Caelum is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Caelum. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CAELUMLISTENER_H
#define CAELUMLISTENER_H

#include "CaelumPrerequisites.h"

namespace caelum {

// Predefinition of the system for the crossed reference.
class CaelumSystem;

/** The Caelum listener interface.
	Allows to register its subclasses to Caelum so that it receives notifications each frame 
	before it's started or finished. Useful, for instance, to prototype certain functionality 
	like passing the sun direction to a shader/material that isn't defined inside Caelum 
	itself.
	@author Jesús Alonso Abad
	@version 1.0
 */
class DllExport CaelumListener {
// Methods --------------------------------------------------------------------
	public: 
		/** Destructor.
		 */
		virtual ~CaelumListener () {};
		/** Trigger fired just before Caelum is about to do its work.
			@param e The Ogre FrameEvent object, in case it's desired to use this as a 
			FrameListener as well.
			@param caelumSystem A reference to the invoker (the Caelum system) itself in case some 
			operations on it are wanted to be done.
			@return True if it's desired to continue running Caelum.
		 */
		virtual bool caelumStarted (const Ogre::FrameEvent &e, CaelumSystem *caelumSystem) {
			return true;
		}

		/** Trigger fired just after Caelum is about to do its work.
			@param e The Ogre FrameEvent object, in case it's desired to use this as a 
			FrameListener as well.
			@param caelumSystem A reference to the invoker (the Caelum system) itself in case some 
			operations on it are wanted to be done.
			@return True if it's desired to continue running after Caelum finished.
		 */
		virtual bool caelumFinished (const Ogre::FrameEvent &e, CaelumSystem *caelumSystem) {
			return true;
		}
};

} // namespace caelum

#endif //CAELUMLISTENER_H
