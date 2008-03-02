/*
This file is part of Caelum.
See http://www.ogre3d.org/wiki/index.php/Caelum 

Copyright (c) 2008 Caelum team. See Contributors.txt for details.

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

#ifndef ASTRONOMY_H
#define ASTRONOMY_H

#include "CaelumPrerequisites.h"

namespace caelum
{
    /** Static class with astronomy routines.
     *  This class contains various astronomical routines useful in Caelum.
     *
     *  Most of the formulas are from http://stjarnhimlen.se/comp/ppcomp.html
     *  That site contains much more than was implemented here; it has code
     *  for determining the positions of all the planets. Only the sun and
     *  moon are actually useful for caelum.
     *
     *  The formulas are isolated here in pure procedural code for easier
     *  testing (Tests are done as assert in the demo).
     *
     *  Precision is vital here, and this class should be converted to use
     *  caelum::LongReal instead of Ogre::Real and Ogre::Degree.
     */
    class DllExport Astronomy
    {
    private:
        Astronomy() {}

    public:
        /** Normalize an angle to the 0, 360 range.
         *  @param x The angle to normalize
         */
        static const Ogre::Degree normalizeAngle(Ogre::Degree x);

        static void convertRectangularToSpherical (
                LongReal x, LongReal y, LongReal z,
                Ogre::Degree &rasc, Ogre::Degree &decl, LongReal &dist);

        static void convertSphericalToRectangular (
                Ogre::Degree rasc, Ogre::Degree decl, LongReal dist,
                LongReal &x, LongReal &y, LongReal &z);

        /** Convert from equatorial to horizontal coordinates.
         *  This function converts from angles relative to the earth's equator
         *  to angle relative to the horizon at a given point.
         *  @param jday Astronomical time as julian day.
         *  @param longitude Observer's longitude in degrees east.
         *  @param latitude Observer's latitude in degrees north.
         *  @param rasc Object's right ascension.
         *  @param decl Object's declination.
         *  @param azimuth Object's azimuth (clockwise degrees from true north).
         *  @param altitude Object's altitude (degrees above the horizon).
         */
        static void convertEquatorialToHorizontal (
                LongReal jday,
                Ogre::Degree longitude, Ogre::Degree latitude,
                Ogre::Degree rasc,      Ogre::Degree decl,
                Ogre::Degree &azimuth,  Ogre::Degree &altitude);

        /** Get the sun's position in the sky in, relative to the horizon.
         *  @param jday Astronomical time as julian day.
         *  @param longitude Observer longitude
         *  @param latitude Observer latitude
         *  @param azimuth Astronomical azimuth, measured clockwise from North = 0.
         *  @param altitude Astronomical altitude, elevation above the horizon.
         */
        static void getHorizontalSunPosition (
                LongReal jday,
                Ogre::Degree longitude, Ogre::Degree latitude,
                Ogre::Degree &azimuth, Ogre::Degree &altitude);

        /** Get astronomical julian day from normal gregorian calendar.
         *  From wikipedia: the integer number of days that have elapsed
         *  since the initial epoch defined as
         *  noon Universal Time (UT) Monday, January 1, 4713 BC
         *  @note this is the time at noon, not midnight.
         */
        static int getJulianDayFromGregorianDate(
                int year, int month, int day); 

        /** Get astronomical julian day from normal gregorian calendar.
         *  Calculate julian day from a day in the normal gregorian calendar.
         *  Time should be given as UTC.
         *  @see http://en.wikipedia.org/wiki/Julian_day
         */
        static LongReal getJulianDayFromGregorianDateTime(
                int year, int month, int day,
                int hour, int minute, LongReal second); 

        /** Get astronomical julian day from normal gregorian calendar.
         *  @see above (I don't know the proper doxygen syntax).
         */
        static LongReal getJulianDayFromGregorianDateTime(
                int year, int month, int day,
                LongReal secondsFromMidnight); 

        /// Get gregorian date from integer julian day.
        static void getGregorianDateFromJulianDay(
                int julianDay, int &year, int &month, int &day);

        /// Get gregorian date time from floating point julian day.
        static void getGregorianDateTimeFromJulianDay(
                LongReal julianDay, int &year, int &month, int &day,
                int &hour, int &minute, LongReal &second);

        /// Get gregorian date from floating point julian day.
        static void getGregorianDateFromJulianDay(
                LongReal julianDay, int &year, int &month, int &day);

        /** Enter high-precission floating-point mode.
         *
         *  By default Direct3D decreases the precission of ALL floating
         *  point calculations, enough to stop Caelum's astronomy routines
         *  from working correctly.
         *  
         *  To trigger this behaviour in a standard ogre demo select the
         *  Direct3D render system and set "Floating-point mode" to
         *  "Fastest". Otherwise it's not a problem.
         *          
         *  It can be fixed by changing the precission only inside caelum's
         *  astronomy routines using the _controlfp function. This only works
         *  for MSVC on WIN32; This is a no-op on other compilers.
         *
         *  @note: Must be paired with restoreFloatingPointMode.
         *  @return Value to pass to restoreFloatingModeMode.
         */
        static int enterHighPrecissionFloatingPointMode ();

        /** Restore old floating point precission.
         *  @see enterHighPrecissionFloatingPointMode.
         */
        static void restoreFloatingPointMode (int oldMode);
    };
}

#endif // SOLARSYSTEMMODEL_H
