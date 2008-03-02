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
#include "CameraBoundElement.h"

namespace caelum {
    const Ogre::Real CameraBoundElement::CAMERA_FAR_DISTANCE_MULTIPLIER = 0.99;
    
    const Ogre::Real CameraBoundElement::CAMERA_NEAR_DISTANCE_MULTIPLIER = 50;

    CameraBoundElement::CameraBoundElement(): mAutoRadius(true) {
    }

    void CameraBoundElement::notifyCameraChanged (Ogre::Camera *cam) {
	    if (mAutoRadius) {
            if (cam->getFarClipDistance () > 0) {
                setFarRadius(cam->getFarClipDistance () * CAMERA_FAR_DISTANCE_MULTIPLIER);
            } else {
                setFarRadius(cam->getNearClipDistance () * CAMERA_NEAR_DISTANCE_MULTIPLIER);
            }
	    }
    }

    void CameraBoundElement::forceFarRadius (Ogre::Real radius) {
        if (radius > 0) {
            mAutoRadius = false;
            setFarRadius(radius);
        } else {
            mAutoRadius = true;
        }
    }

    bool CameraBoundElement::getAutoRadius () const {
        return mAutoRadius;
    }

    void CameraBoundElement::setFarRadius(Ogre::Real radius) {
        // Pass
    }

} // namespace caelum
