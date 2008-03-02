/*
This file is part of Caelum.
See http://www.ogre3d.org/wiki/index.php/Caelum 

Copyright (c) 2006-2008 Caelum team. See Contributors.txt for details.

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
#include "SolarSystemModel.h"
#include "UniversalClock.h"
#include "Astronomy.h"

using Ogre::Degree;
using Ogre::Radian;
using Ogre::Math;
using Ogre::Real;

namespace caelum
{
    SolarSystemModel::SolarSystemModel (
            Ogre::Degree longitude, Ogre::Degree latitude):
            mObserverLatitude(latitude), mObserverLongitude(longitude)
    {
    }

    const Ogre::Vector3 SolarSystemModel::getSunDirection (LongReal jday)
    {
        int fpmode = Astronomy::enterHighPrecissionFloatingPointMode ();
           
        // Horizontal spherical.
        Degree azimuth;
        Degree altitude; 
        Astronomy::getHorizontalSunPosition(jday,
                getObserverLongitude(), getObserverLatitude(),
                azimuth, altitude);

        // Ogre direction.
        Ogre::Vector3 res;
        res.x = Math::Sin (azimuth);  // East
        res.y = -Math::Sin (altitude); // Zenith
        res.z = -Math::Cos (azimuth);  // North

        Astronomy::restoreFloatingPointMode(fpmode);

        return res;
    }
}
