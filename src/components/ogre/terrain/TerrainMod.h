//
// C++ Interface: TerrainMod
//
// Description: The purpose of this class is to wrap an Eris::TerrainMod instance
//
//		TerrainGenerator listens for changes in the modifier and
//		updates or removes the modifiers from the terrain as needed.
//
//
// Author: Tamas Bates <rhymer@gmail.com>, (C) 2008
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2008
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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifndef EMBEROGRETERRAINMOD_H
#define EMBEROGRETERRAINMOD_H

#include <sigc++/signal.h>
#include <Eris/Entity.h>

namespace Mercator
{
	class TerrainMod;
	class CraterTerrainMod;
}

namespace Eris 
{
	class TerrainMod;
}

namespace EmberOgre
{

class EmberEntity;

namespace Terrain {

/**
@author Tamas Bates
@author Erik Hjortsberg
@brief Wrapper class that envelopes an Eris::TerrainMod.
The heavy lifting occurs in Eris::TerrainMod, but this class allows some additional features to be added if so needed.
*/
class TerrainMod
{
public:
	/**
	 * @brief Ctor.
	 * @param entity The entity to which this mod belongs.
	 */
	TerrainMod(EmberEntity* entity);

	/**
	 * @brief Dtor.
	 */
	~TerrainMod();
	
	/**
	 * @brief Sets up the observation of the entity, and parses the mod info, creating the initial mod instance.
	 * @return True if the atlas data was conformant and successfully parsed.
	 */
	bool init();

	/**
	 *    @brief Used to retrieve a pointer to this modifier
	 * @returns a pointer to this modifier
	 */
	Eris::TerrainMod* getErisMod() const;

	/**
	 * Emitted whenever the modifier is changed or moved.
	 */
	sigc::signal<void> EventModChanged;

	/**
	 *Emitted just before the entity owning this mod is deleted.
	 *Should be caught by TerrainGenerator to remove this mod from the terrain.
	 */
	sigc::signal<void> EventModDeleted;
	
protected:

	/**
	 * @brief Listen for the mod changed event and pass it on.
	 */
	void terrainMod_ModChanged();
	
	/**
	 * @brief Listen for the mod deleted event and pass it on.
	 */
	void terrainMod_ModDeleted();
	
	/**
	 * @brief The actual Eris::TerrainMod implementation responsible for the parsing and updating of the mod.
	 */
	Eris::TerrainMod* mInnerMod;
};

}
};

#endif
