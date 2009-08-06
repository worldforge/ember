/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#ifndef ATLASHELPER_H_
#define ATLASHELPER_H_

#include <Atlas/Objects/ObjectsFwd.h>
#include <string>

namespace EmberOgre
{

namespace Gui
{

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Provides simple Atlas help functionality, mainly meant for those things that are cumbersome to do in the script language.
 */
class AtlasHelper
{
public:

	/**
	 * @brief Serializes the supplied object into a string using the supplied codec type.
	 * @param obj The object to serialize.
	 * @param codecType The codec type to use.
	 * @return The object in serialized form.
	 */
	static std::string serialize(const Atlas::Objects::Root& obj, const std::string& codecType = "bach");
};

}
}
#endif /* ATLASHELPER_H_ */
