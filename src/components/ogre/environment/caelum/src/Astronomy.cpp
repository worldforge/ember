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

#include "CaelumPrecompiled.h"
#include "Astronomy.h"

using Ogre::Degree;
using Ogre::Radian;
using Ogre::Math;

namespace caelum
{
    const Ogre::Degree Astronomy::normalizeAngle (Ogre::Degree value)
    {
        value = fmod (value.valueDegrees (), 360);
        if (value < Degree (0)) {
            value += Degree (360);
        }
        return value;
    }

    void Astronomy::convertRectangularToSpherical (
            LongReal x, LongReal y, LongReal z,
            Degree &rasc, Degree &decl, LongReal &dist)
    {
        dist = Math::Sqrt (x * x + y * y + z * z);
        rasc = Math::ATan2 (y, x);
        decl = Math::ATan2 (z, Math::Sqrt (x * x + y * y));
    }

    void Astronomy::convertSphericalToRectangular (
            Degree rasc, Degree decl, LongReal dist,
            LongReal &x, LongReal &y, LongReal &z)
    {
        x = dist * Math::Cos (rasc) * Math::Cos (decl);
        y = dist * Math::Sin (rasc) * Math::Cos (decl);
        z = dist * Math::Sin (decl);
    }

    void Astronomy::convertEquatorialToHorizontal (
            LongReal jday,
            Degree longitude,   Degree latitude,
            Degree rasc,        Degree decl,
            Degree &azimuth,    Degree &altitude)
    {
        LongReal d = jday - 2451543.5;
        Degree w = Degree (282.9404 + 4.70935E-5 * d);
        Degree M = Degree (356.0470 + 0.9856002585 * d);
        // Sun's mean longitude
        Degree L = w + M;
        // Universal time of day in degrees.
        Degree UT = Degree(fmod(d, 1) * 360);
        Degree hourAngle = longitude + L + Degree (180) + UT - rasc;

        LongReal x = Math::Cos (hourAngle) * Math::Cos (decl);
        LongReal y = Math::Sin (hourAngle) * Math::Cos (decl);
        LongReal z = Math::Sin (decl);

        LongReal xhor = x * Math::Sin (latitude) - z * Math::Cos (latitude);
        LongReal yhor = y;
        LongReal zhor = x * Math::Cos (latitude) + z * Math::Sin (latitude);

        azimuth = Math::ATan2 (yhor, xhor) + Degree (180);
        altitude = Math::ATan2 (zhor, Math::Sqrt (xhor * xhor + yhor * yhor));
    }

    void Astronomy::getHorizontalSunPosition (
            LongReal jday,
            Ogre::Degree longitude, Ogre::Degree latitude,
            Ogre::Degree &azimuth, Ogre::Degree &altitude)
    {
        // Midnight at the start of 31 december 2000 
        // 2451543.5 == Astronomy::getJulianDayFromGregorianDateTime(1999, 12, 31, 0, 0, 0));
        LongReal d = jday - 2451543.5;

        // Sun's Orbital elements:
        // argument of perihelion
        Degree w = Degree (282.9404 + 4.70935E-5 * d);
        // eccentricity (0=circle, 0-1=ellipse, 1=parabola)
        LongReal e = 0.016709 - 1.151E-9 * d;
        // mean anomaly (0 at perihelion; increases uniformly with time)
        Degree M = Degree(356.0470 + 0.9856002585 * d);
        // Obliquity of the ecliptic.
        Degree oblecl = Degree (23.4393 - 3.563E-7 * d);

        // Eccentric anomaly
        Degree E = M + Radian(e * Math::Sin (M) * (1 + e * Math::Cos (M)));

        // Sun's Distance(R) and true longitude(L)
        LongReal xv = Math::Cos (E) - e;
        LongReal yv = Math::Sin (E) * Math::Sqrt (1 - e * e);
        LongReal r = Math::Sqrt (xv * xv + yv * yv);
        Degree lon = Math::ATan2 (yv, xv) + w;

        // Ecliptic rectangular.
        LongReal xecl = r * Math::Cos(lon);
        LongReal yecl = r * Math::Sin(lon);
        LongReal zecl = 0;

        // Equatorial rectangular.
        LongReal xequ = xecl;
        LongReal yequ = yecl * Math::Cos (oblecl) - zecl * Math::Sin (oblecl);
        LongReal zequ = yecl * Math::Sin (oblecl) + zecl * Math::Cos (oblecl);

        // Equatorial spherical.
        Degree rasc, decl;
        Astronomy::convertRectangularToSpherical (xequ, yequ, zequ, rasc, decl, r);

        // Horizontal spherical.
        Astronomy::convertEquatorialToHorizontal (
                jday, longitude, latitude, rasc, decl, azimuth, altitude);
    }

    int Astronomy::getJulianDayFromGregorianDate(
            int year, int month, int day)
    {
        // Formulas from http://en.wikipedia.org/wiki/Julian_day
        // These are all integer divisions, but I'm not sure it works
        // correctly for negative values.
        int a = (14 - month) / 12;
        int y = year + 4800 - a;
        int m = month + 12 * a - 3;
        return day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
    }

    LongReal Astronomy::getJulianDayFromGregorianDateTime(
            int year, int month, int day,
            int hour, int minute, LongReal second)
    {
        int fpmode = Astronomy::enterHighPrecissionFloatingPointMode ();

        int jdn = getJulianDayFromGregorianDate (year, month, day);
        // These are NOT integer divisions.
        LongReal jd = jdn + (hour - 12) / 24.0 + minute / 1440.0 + second / 86400.0;

        Astronomy::restoreFloatingPointMode(fpmode);
        return jd;
    }

    LongReal Astronomy::getJulianDayFromGregorianDateTime(
            int year, int month, int day,
            LongReal secondsFromMidnight)
    {
        int jdn = getJulianDayFromGregorianDate(year, month, day);
        LongReal jd = jdn + secondsFromMidnight / 86400.0 - 0.5;
        return jd;
    }

    void Astronomy::getGregorianDateFromJulianDay(
            int julianDay, int &year, int &month, int &day)
    {
        // From http://en.wikipedia.org/wiki/Julian_day
        int J = julianDay;
        int j = J + 32044;
        int g = j / 146097;
        int dg = j % 146097;
        int c = (dg / 36524 + 1) * 3 / 4;
        int dc = dg - c * 36524;
        int b = dc / 1461;
        int db = dc % 1461;
        int a = (db / 365 + 1) * 3 / 4;
        int da = db - a * 365;
        int y = g * 400 + c * 100 + b * 4 + a;
        int m = (da * 5 + 308) / 153 - 2;
        int d = da - (m + 4) * 153 / 5 + 122;
        year = y - 4800 + (m + 2) / 12;
        month = (m + 2) % 12 + 1;
        day = d + 1;
    }

    void Astronomy::getGregorianDateTimeFromJulianDay(
            LongReal julianDay, int &year, int &month, int &day,
            int &hour, int &minute, LongReal &second)
    {
        // Integer julian days are at noon.
        // static_cast<int)(floor( is more precise than Ogre::Math::IFloor.
        // Yes, it does matter.
        int ijd = static_cast<int>(floor(julianDay + 0.5));
        getGregorianDateFromJulianDay(ijd, year, month, day);

        LongReal s = (julianDay + 0.5 - ijd) * 86400.0;
        hour = static_cast<int>(floor(s / 3600));
        s -= hour * 3600;
        minute = static_cast<int>(floor(s / 60));
        s -= minute * 60;
        second = s;
    }

    void Astronomy::getGregorianDateFromJulianDay(
            LongReal julianDay, int &year, int &month, int &day)
    {
        int hour;
        int minute;
        LongReal second;
        getGregorianDateTimeFromJulianDay(julianDay, year, month, day, hour, minute, second);
    }

#if (OGRE_PLATFORM == OGRE_PLATFORM_WIN32) && (OGRE_COMPILER == OGRE_COMPILER_MSVC)
    int Astronomy::enterHighPrecissionFloatingPointMode ()
    {
        int oldMode = ::_controlfp (0, 0);
        ::_controlfp (_PC_64, _MCW_PC);
        return oldMode;
    }

    void Astronomy::restoreFloatingPointMode (int oldMode)
    {
        ::_controlfp (oldMode, _MCW_PC);
    }
#else
    int Astronomy::enterHighPrecissionFloatingPointMode ()
    {
        // Meaningless
        return 0xC0FFEE;
    }

    void Astronomy::restoreFloatingPointMode (int oldMode)
    {
        // Useless check.
        assert(oldMode == 0xC0FFEE);
    }
#endif
}
