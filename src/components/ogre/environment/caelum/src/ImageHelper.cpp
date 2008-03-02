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

#include "CaelumPrecompiled.h"
#include "ImageHelper.h"

namespace caelum
{
    Ogre::ColourValue getInterpolatedColour (
            float fx, float fy, Ogre::Image *img, bool wrapX)
    {
	    // Don't -> all the time, and avoid unsigned warnings
        int imgWidth = static_cast<int>(img->getWidth ());
        int imgHeight = static_cast<int>(img->getHeight ());

	    // Calculate pixel y coord.
        int py = Ogre::Math::IFloor(Ogre::Math::Abs (fy) * (imgHeight - 1));
        // Snap to py image bounds.
        py = std::max(0, std::min(py, imgHeight - 1));

	    // Get the two closest pixels on x.
        // px1 and px2 are the closest integer pixels to px.
        float px = fx * (img->getWidth () - 1);
	    int px1, px2;
        px1 = Ogre::Math::IFloor(px);
        px2 = Ogre::Math::ICeil(px);

        if (wrapX) {
            // Wrap x coords. The funny addition ensures that it does
            // "the right thing" for negative values.
            px1 = (px1 % imgWidth + imgWidth) % imgWidth;
            px2 = (px2 % imgWidth + imgWidth) % imgWidth;
        } else {
            px1 = std::max(0, std::min(px1, imgWidth - 1));
            px2 = std::max(0, std::min(px2, imgWidth - 1));
        }

	    // Calculate the interpolated pixel
	    Ogre::ColourValue c1, c2, cf;
        c1 = img->getColourAt (px1, py, 0);
        c2 = img->getColourAt (px2, py, 0);

        // Blend the two pixels together.
        // diff is the weight between pixel 1 and pixel 2.
        float diff = px - px1;
	    cf = c1 * (1 - diff) + c2 * diff;

	    return cf;
    }

}
