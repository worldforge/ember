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
    class DllExport SolarSystemModel
    {
    private:
        Ogre::Degree mObserverLatitude;
        Ogre::Degree mObserverLongitude;

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
    };
}

#endif // SOLARSYSTEMMODEL_H
