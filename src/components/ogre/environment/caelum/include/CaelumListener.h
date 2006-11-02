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
