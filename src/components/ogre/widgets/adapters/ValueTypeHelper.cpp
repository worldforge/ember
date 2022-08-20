/*
 Copyright (C) 2011 Erik Ogenvik <erik@ogenvik.org>

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

#include "ValueTypeHelper.h"

namespace Ember::OgreView::Gui::Adapters {

bool CompareHelper<varconf::Variable>::areEqual(const varconf::Variable& one, const varconf::Variable& two) {
	if (one.is_int() && two.is_int()) {
		return static_cast<int>(one) == static_cast<int>(two);
	} else if (one.is_double() && two.is_double()) {
		//To skip the whole issue of precision when comparing floating values we'll just convert them to strings with 4 character precision.
		std::stringstream ss_one, ss_two;
		ss_one << std::setprecision(4) << static_cast<double>(one);
		ss_two << std::setprecision(4) << static_cast<double>(two);
		return ss_one.str() == ss_two.str();
	} else if (one.is_bool() && two.is_bool()) {
		return static_cast<bool>(one) == static_cast<bool>(two);
	}
	return one == two;
}

}

