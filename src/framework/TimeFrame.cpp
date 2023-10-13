/*
 Copyright (C) 2011 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <chrono>
#include <memory>
#include "TimeFrame.h"

namespace Ember {
TimeFrame::TimeFrame(std::chrono::steady_clock::duration timeSlice) :
		mStartTime(std::chrono::steady_clock::now()),
		mEndTime(mStartTime + timeSlice) {
}

bool TimeFrame::isTimeLeft() const {
	return std::chrono::steady_clock::now() < mEndTime;
}

std::chrono::nanoseconds TimeFrame::getRemainingTime() const {
	return mEndTime - std::chrono::steady_clock::now();
}

std::chrono::nanoseconds TimeFrame::getElapsedTime() const {
	return std::chrono::steady_clock::now() - mStartTime;
}

}
