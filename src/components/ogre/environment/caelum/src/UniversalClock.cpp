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
#include "UniversalClock.h"
#include "Astronomy.h"

namespace caelum {

const caelum::LongReal UniversalClock::SECONDS_PER_DAY = 86400.0;

UniversalClock::UniversalClock () {
    setJulianDay (Astronomy::J2000);        
	setTimeScale (1.0);
	setUpdateRate (0);
    // The above call does forceUpdate but let's be explicit for clarity.
    forceUpdate ();
}

void UniversalClock::setJulianDay (caelum::LongReal value) {
    mJulianDayBase = value;
    mCurrentTime = 0;
    mLastUpdateTime = 0;
}

void UniversalClock::setGregorianDateTime(
        int year, int month, int day,
        int hour, int minute, double second)
{
    int fpmode = Astronomy::enterHighPrecissionFloatingPointMode ();
    setJulianDay(Astronomy::getJulianDayFromGregorianDateTime(year, month, day, hour, minute, second));
    Astronomy::restoreFloatingPointMode(fpmode);
}

caelum::LongReal UniversalClock::getJulianDay () const
{
    int fpmode = Astronomy::enterHighPrecissionFloatingPointMode ();
    caelum::LongReal res = mJulianDayBase + mCurrentTime / SECONDS_PER_DAY;
    Astronomy::restoreFloatingPointMode(fpmode);
    return res;
}

caelum::LongReal UniversalClock::getJulianDayDifference () const {
    return (mCurrentTime - mLastUpdateTime) / SECONDS_PER_DAY;
}

caelum::LongReal UniversalClock::getJulianSecond () const {
    int fpmode = Astronomy::enterHighPrecissionFloatingPointMode ();
    caelum::LongReal res = mJulianDayBase * SECONDS_PER_DAY + mCurrentTime;
    Astronomy::restoreFloatingPointMode(fpmode);
    return res;
}

caelum::LongReal UniversalClock::getJulianSecondDifference () const {
    return mCurrentTime - mLastUpdateTime;
}

void UniversalClock::setTimeScale (const Ogre::Real scale) {
	mTimeScale = scale;
}

Ogre::Real UniversalClock::getTimeScale () const {
	return mTimeScale;
}

void UniversalClock::setUpdateRate (const Ogre::Real rate) {
	mUpdateRate = rate;

    if (mUpdateRate < 0) {
		mUpdateRate = 0;
    }

    forceUpdate();
}

Ogre::Real UniversalClock::getUpdateRate () const {
	return mUpdateRate;
}

bool UniversalClock::update (const Ogre::Real time) {
	mTimeSinceLastUpdate += time;

	if (mTimeSinceLastUpdate > mUpdateRate) {
        mLastUpdateTime = mCurrentTime;
        mCurrentTime += mTimeSinceLastUpdate * mTimeScale;
        mTimeSinceLastUpdate = 0;
		return true;
	} else {
		return false;
	}
}

void UniversalClock::forceUpdate () {
    mTimeSinceLastUpdate = mUpdateRate;
}

} // namespace caelum
