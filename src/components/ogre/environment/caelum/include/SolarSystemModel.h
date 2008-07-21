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

#ifndef SOLARSYSTEMMODEL_H
#define SOLARSYSTEMMODEL_H

#include "CaelumPrerequisites.h"

namespace caelum
{
    /** Class which calculates sun and moon positions on the sky.
     *  Most of the calculations are done in the astronomy class.
     */
    class CAELUM_EXPORT SolarSystemModel
    {
    private:
        Ogre::Degree mObserverLatitude;
        Ogre::Degree mObserverLongitude;

        static const Ogre::Vector3 makeDirection (
                Ogre::Degree azimuth, Ogre::Degree altitude);

	public:
		/** Constructor, initializes observer's position on earth.
		 */
        SolarSystemModel (
                Ogre::Degree longitude = Ogre::Degree (0),
                Ogre::Degree latitude = Ogre::Degree (45));
		
        /// Get the observer's longitude. East is positive, west is negative.
        inline const Ogre::Degree getObserverLongitude () const { return mObserverLongitude; }

        /// Set the observer's longitude. East is positive, west is negative.
        inline void setObserverLongitude (Ogre::Degree value) { mObserverLongitude = value; }

        /// Get the observer's latitude. North is positive, south is negative.
        inline const Ogre::Degree getObserverLatitude () const { return mObserverLatitude; }

        /// Set the observer's latitude. North is positive, south is negative.
        inline void setObserverLatitude (Ogre::Degree value) { mObserverLatitude = value; }

		/** Get the sun's direction at a certain time.
         *  @param jday astronomical julian day.
         *  @see UniversalClock for julian day calculations.
		 */
		const Ogre::Vector3 getSunDirection (LongReal jday);

		/** Get the moon's direction at a certain time.
         *  @param jday astronomical julian day.
		 */
		const Ogre::Vector3 getMoonDirection (LongReal jday);

        /** Fake function to get the phase of the moon
         *  @param jday Julian day
         *  @return the phase of the moon; ranging from 0(full moon) to 2(new moon).
         *  The calculations performed by this function are completely fake.
         *  It's a triangle wave with a period of 28.5 days.
         */
		const Ogre::Real getMoonPhase (LongReal jday);
    };
}

#endif // SOLARSYSTEMMODEL_H
