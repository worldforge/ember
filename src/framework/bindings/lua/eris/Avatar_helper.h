/*
 Copyright (C) 2020 Erik Ogenvik

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

#ifndef EMBER_AVATAR_HELPER_H
#define EMBER_AVATAR_HELPER_H

#include <Eris/Avatar.h>

namespace {

void _Avatar_place(Eris::Avatar* avatar,
				   const Eris::Entity* entity,
				   const Eris::Entity* container,
				   const WFMath::Point<3>& pos = WFMath::Point<3>(),
				   const WFMath::Quaternion& orientation = WFMath::Quaternion(),
				   float offset = 0,
				   int amount = 1) {
	boost::optional<float> offsetOptional;
	if (offset != 0) {
		offsetOptional = offset;
	}
	avatar->place(entity, container, pos, orientation, offsetOptional, amount);
}

void _Avatar_take(Eris::Avatar* avatar,
				  const Eris::Entity* entity,
				  int amount = 1) {
	avatar->place(entity, avatar->getEntity(), {}, {}, {}, amount);
}


}


#endif //EMBER_AVATAR_HELPER_H
