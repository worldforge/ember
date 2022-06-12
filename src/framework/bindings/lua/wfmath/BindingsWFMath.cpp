/*
 Copyright (C) 2022 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "BindingsWFMath.h"
#include "components/lua/Connector.h"

#include <wfmath/wfmath.h>

using namespace WFMath;
using namespace Ember::Lua;

void registerBindingsWFMath(sol::state_view& lua) {
	auto wfmath = lua["WFMath"].get_or_create<sol::table>();

	auto vector2 = wfmath.new_usertype<Vector<2>>("Vector<2>");
	vector2["x"] = sol::resolve<WFMath::CoordType() const>(&Vector<2>::x);
	vector2["y"] = sol::resolve<WFMath::CoordType() const>(&Vector<2>::y);

	auto vector3 = wfmath.new_usertype<Vector<3>>("Vector<3>");
	vector3["x"] = sol::resolve<WFMath::CoordType() const>(&Vector<3>::x);
	vector3["y"] = sol::resolve<WFMath::CoordType() const>(&Vector<3>::y);
	vector3["z"] = sol::resolve<WFMath::CoordType() const>(&Vector<3>::z);

	auto point2 = wfmath.new_usertype<Point<2>>("Point<2>");
	point2["x"] = sol::resolve<WFMath::CoordType() const>(&Point<2>::x);
	point2["y"] = sol::resolve<WFMath::CoordType() const>(&Point<2>::y);

	auto point3 = wfmath.new_usertype<Point<3>>("Point<3>");
	point3["x"] = sol::resolve<WFMath::CoordType() const>(&Point<3>::x);
	point3["y"] = sol::resolve<WFMath::CoordType() const>(&Point<3>::y);
	point3["z"] = sol::resolve<WFMath::CoordType() const>(&Point<3>::z);
}