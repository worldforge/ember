//
// C++ Interface: TerrainMod
//
// Description: 
//
//
// Author: Tamas Bates <rhymer@gmail.com>, (C) 2008
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
}

namespace EmberOgre {

class EmberEntity;

namespace Terrain {

/**
@author Tamas Bates
*/
class TerrainMod
{
public:
    TerrainMod(EmberEntity* entity);

    ~TerrainMod();
	bool init();
	
	/**
	 *    @brief Used to retrieve a pointer to this modifier
	 * @returns a pointer to this modifier
	 */
	inline Mercator::TerrainMod* getMod() const;

	/**
	 *    @brief Used to set the pointer to this modifier
	 * @param modifier The TerrainMod we need stored (usually returned from Terrain::addMod() )
	 */
	inline void setMod(Mercator::TerrainMod* modifier);
	
	/**
	 * Emitted whenever the modifier is changed or moved.
	 */
	sigc::signal<void, TerrainMod*> EventModChanged;
	
	/**
	 *Emitted just before the entity owning this mod is deleted.
	 *Should be caught by TerrainGenerator to remove this mod from the terrain.
	 */
	sigc::signal<void, TerrainMod*> EventModDeleted;

	/**
	 *The owner of this modifier
	 */
	EmberEntity* mEntity;

protected:
	/**
	 *A pointer to the modifier. Used to update and remove it from the terrain.
	 */
	Mercator::TerrainMod* mModifier;
	Eris::Entity::AttrChangedSlot mAttrChangedSlot;

	/**
	 *    @brief Called whenever a modifier is changed and handles the update
	 * @param attributeValue The new Atlas data for the terrain mod
	 */	
	void attributeChanged(const Atlas::Message::Element& attributeValue);

	/**
	 *    @brief Called whenever a modifier is moved and handles the update
	 */
	void entity_Moved();

	/**
	Called whenever the entity holding a modifier is deleted and handles
	removing the mod from the terrain
	*/
	void entity_Deleted();

	/**
	 *    @brief Sets up the previous three handler functions to be called when a change
	 * is made to the entity holding the modifier. 
	 */
	void observeEntity();

	/**
 	 *    @brief Parses the Atlas data for a modifier
	 * @returns True if it was able to successfully create a Mercator::TerrainMod, False otherwise
	 * All work specific to a certain kind of TerrainMod is handled by the functions below.
	 */
	bool parseMod();

	/**
	 *    @brief creates a CraterTerrainMod based on a shape and position
	 * @param shapeMap An Atlas MapType containing all the information about the shape defining the mod
	 * @param pos A 3D Point containing the position the modifier is to be applied at
	 * @returns A pointer to the new TerrainMod on success, or NULL on failure
	 * pos is used to generate the shape for the modifier, and can cause the mod to be
	 * applied at a position other than the owning entity's position.
	 */
	Mercator::TerrainMod * newCraterMod(const Atlas::Message::MapType, WFMath::Point<3>);

	/**    @brief Creates a LevelTerrainMod based on a shape and position
	 * @param shapeMap An Atlas MapType containing all the information about the shape defining the mod
	 * @param pos A 3D Point containing the position the modifier is to be applied at
	 * @returns A pointer to the new TerrainMod on success, or NULL on failure
	 * In this case the LevelMod only uses 2D coordinates for its position, so pos.z() actually
	 * holds the height that the LevelMod will be raised to.
	 */
	Mercator::TerrainMod * newLevelMod(const Atlas::Message::MapType, WFMath::Point<3>);

	/**    @brief Creates a SlopeTerrainMod based on a shape and position
	 * @param shapeMap An Atlas MapType containing all the information about the shape defining the mod
	 * @param pos A 3D Point containing the position the modifier is to be applied at
	 * @param dx The desired slope on the X-Axis
	 * @param dy The desired slope on the Y-Axis
	 * @returns A pointer to the new TerrainMod on success, or NULL on failure
	 * In this case the SlopeMod only uses 2D coordinates for its position, so pos.z() actually
	 * holds the height that the terrain will be raised to.
	 */
	Mercator::TerrainMod * newSlopeMod(const Atlas::Message::MapType, WFMath::Point<3>, float, float);

	/**    @brief Creates a AdjustTerrainMod based on a shape and position
	 * @param shapeMap An Atlas MapType containing all the information about the shape defining the mod
	 * @param pos A 3D Point containing the position the modifier is to be applied at
	 * @returns A pointer to the new TerrainMod on success, or NULL on failure
	 * In this case the LevelMod only uses 2D coordinates for its position, so pos.z() actually
	 * holds the value of the height adjustment made to the terrain.
	 */
	Mercator::TerrainMod * newAdjustMod(const Atlas::Message::MapType, WFMath::Point<3>);

};

Mercator::TerrainMod* TerrainMod::getMod() const { return mModifier; }
void TerrainMod::setMod(Mercator::TerrainMod* modifier) { mModifier = modifier; }

}
};

#endif
