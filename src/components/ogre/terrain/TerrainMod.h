//
// C++ Interface: TerrainMod
//
// Description: The purpose of this class is to wrap an Eris::TerrainMod instance
//
//		TerrainManager listens for changes in the modifier and
//		updates or removes the modifiers from the terrain as needed.
//
//
// Author: Tamas Bates <rhymer@gmail.com>, (C) 2008
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2008
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBEROGRETERRAINMOD_H
#define EMBEROGRETERRAINMOD_H

#include <Eris/Entity.h>
#include <sigc++/signal.h>
#include <string>

namespace Atlas {
namespace Message {
class Element;
}
}

namespace Mercator {
class TerrainMod;
}

namespace Ember {
class EmberEntity;
namespace Terrain {
class TerrainModTranslator;
}
namespace OgreView {
namespace Terrain {

/**
@author Tamas Bates
@author Erik Ogenvik
@brief Connects an Eris::Entity to a Mercator terrain mod.

Note that however the parsing and application of the terrain mod doesn't occur here.
Instead it's handled by the TerrainMod*Task classes, which allows it to occur in a background thread.
*/
class TerrainMod {
public:
	/**
	 * @brief Ctor.
	 * @param entity The entity to which this mod belongs.
	 */
	TerrainMod(Eris::Entity& entity, const Atlas::Message::MapType& data);

	/**
	 * @brief Dtor.
	 */
	virtual ~TerrainMod();


	/**
	 * Parses a terrain mod value. This will create a translator.
	 * @param value
	 */
	void parse(const Atlas::Message::Element& value);

	/**
	 * Resets the terrain mod, removing any translator.
	 */
	void reset();

	/**
	 * @brief Gets the id of the entity which is observed.
	 * @returns The id of the entity to which this mod belongs.
	 */
	const std::string& getEntityId() const;

	/**
	* @brief Accessor for the entity which is observed.
	* @return The entity observed by this instance.
	*/
	Eris::Entity& getEntity() const;

	/**
	 * @brief Gets the atlas data which defines the terrain mod.
	 */
	const Ember::Terrain::TerrainModTranslator& getTranslator() const;


protected:

	/**
	@brief The observed entity, containing terrain mod data.
	*/
	Eris::Entity& mEntity;

	std::unique_ptr<Ember::Terrain::TerrainModTranslator> mTranslator;

};

}
}
}

#endif
