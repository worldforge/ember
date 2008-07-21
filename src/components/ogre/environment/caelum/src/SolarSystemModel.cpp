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

    const Ogre::Vector3 SolarSystemModel::makeDirection (
            Ogre::Degree azimuth, Ogre::Degree altitude)
    {
        Ogre::Vector3 res;
        res.z = -Math::Cos (azimuth) * Math::Cos (altitude);  // North 
        res.x =  Math::Sin (azimuth) * Math::Cos (altitude);  // East
        res.y = -Math::Sin (altitude); // Zenith
        return res;
    }

    const Ogre::Vector3 SolarSystemModel::getSunDirection (LongReal jday)
    {
		int fpmode = Astronomy::enterHighPrecissionFloatingPointMode ();
                  
        Degree azimuth;
        Degree altitude; 
		Astronomy::getHorizontalSunPosition(jday,
                getObserverLongitude(), getObserverLatitude(),
                azimuth, altitude);		
        Ogre::Vector3 res = makeDirection(azimuth, altitude);

        Astronomy::restoreFloatingPointMode(fpmode);
        return res;
    }

	const Ogre::Vector3 SolarSystemModel::getMoonDirection (LongReal jday) {
		int fpmode = Astronomy::enterHighPrecissionFloatingPointMode ();

        Ogre::Degree azimuth, altitude;
        Astronomy::getHorizontalMoonPosition(jday,
                getObserverLongitude (), getObserverLatitude (),
                azimuth, altitude);
	
        Ogre::Vector3 res = makeDirection(azimuth, altitude);

		Astronomy::restoreFloatingPointMode(fpmode);
		return res;
	}

    const Ogre::Real SolarSystemModel::getMoonPhase (LongReal jday)
    {
        // Calculates julian days since January 22, 2008 13:36 (full moon)
        // and divides by the time between lunations (synodic month)
        LongReal T = (jday - 2454488.0665L) / 29.531026L;

        T = fabs(fmod(T, 1));
        return -fabs(-4 * T + 2) + 2;
    }
}
