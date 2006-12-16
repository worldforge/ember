#ifndef CAMERABOUNDELEMENT_H
#define CAMERABOUNDELEMENT_H

#include "CaelumPrerequisites.h"

namespace caelum {

/** A camera-bound element.
		This interface serves as the base class for all the elements that must follow cameras, adjust its size, and such operations.
		@author Jesús Alonso Abad
 */
class DllExport CameraBoundElement {
// Attributes -----------------------------------------------------------------
	protected:
		/** Defines if the element has an automatic "far" radius or not.
		 */
		bool mAutoRadius;
		
// Methods --------------------------------------------------------------------
	public:
		/** Notify new camera conditions.
			This method notifies that a new camera is about to be used, so this element can follow it or perform other operations.
			@param cam The new camera.
		 */
		virtual void notifyCameraChanged (Ogre::Camera *cam) = 0;

		/** Forces the "far" size of the element to a specific radius.
			In other words: The distance considered the furthest locally to this element.
			If the parameter is negative or zero, the radius is set automatically.
			@param radius The positive radius of the element, or a negative/zero value to let Caelum to resize it.
		 */
		virtual void setFarRadius (float radius) = 0;
};
				
} // namespace caelum

#endif // CAMERABOUNDELEMENT_H
