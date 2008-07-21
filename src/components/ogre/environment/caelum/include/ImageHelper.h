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

#ifndef CAELUM_HEADER__IMAGE_HELPER_H
#define CAELUM_HEADER__IMAGE_HELPER_H

#include "CaelumPrerequisites.h"

namespace caelum
{
	/** Gets the interpolated colour between two pixels from an image.
        Interpolate a texture pixel by hand. (fx, fy) are in texture coordinates,
        ranging [0-1] across the entire texture.
        Smooth blending is only done on the x coordinate.
        Wrapping is only supported on X as well.

		@param fx Horizontal coordinate.
		@param fy Vertical coordiate.
		@param img The lookup image.
		@param wrapX To wrap the x coordinate.
		@return The interpolated colour.
	 */
	CAELUM_EXPORT Ogre::ColourValue getInterpolatedColour (float fx, float fy, Ogre::Image *img,
            bool wrapX = true);
}

#endif // CAELUM_HEADER__IMAGE_HELPER_H
