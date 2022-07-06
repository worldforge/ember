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
#include "RegisterLua.h"

using namespace Ogre;

template<>
void registerLua<Vector3>(sol::table& space) {
	auto vector3 = space.new_usertype<Vector3>("Vector3",
											   sol::constructors<Vector3(Vector3), Vector3(Real, Real, Real)>()
	);
	vector3["ZERO"] = sol::var(&Vector3::ZERO);
	vector3["x"] = &Vector3::x;
	vector3["y"] = &Vector3::y;
	vector3["z"] = &Vector3::z;

	auto axisAlignedBox = space.new_usertype<AxisAlignedBox>("AxisAlignedBox");
	axisAlignedBox["getMinimum"] = sol::resolve<const Vector3&() const>(&AxisAlignedBox::getMinimum);
	axisAlignedBox["getMaximum"] = sol::resolve<const Vector3&() const>(&AxisAlignedBox::getMaximum);

	space.new_usertype<Radian>("Radian",
							   sol::constructors<Radian(float), Radian(const Degree&)>()
	);
	space.new_usertype<Degree>("Degree",
							   sol::constructors<Degree(float), Degree(const Radian&)>()
	);

	space.new_usertype<ColourValue>("ColourValue");

	space.new_usertype<Quaternion>("Quaternion",
								   "IDENTITY", sol::var(&Quaternion::IDENTITY),
								   "ZERO", sol::var(&Quaternion::ZERO)
	);
}