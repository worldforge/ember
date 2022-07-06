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

#ifndef EMBER_REGISTERLUA_H
#define EMBER_REGISTERLUA_H

#include "components/lua/Connector.h"
#include <Ogre.h>

namespace sol {
template<typename T>
struct unique_usertype_traits<Ogre::SharedPtr<T>> {
	typedef T type;
	typedef Ogre::SharedPtr<T> actual_type;
	static const bool value = true;

	static bool is_null(const actual_type& ptr) {
		return ptr == nullptr;
	}

	static type* get(const actual_type& ptr) {
		return ptr.get();
	}
};
}

template<typename T>
void registerLua(sol::table& space);

#endif //EMBER_REGISTERLUA_H
