//
// C++ Interface: TerrainMod
//
// Description: The purpose of this class is to handle the bulk of the work
//		involved with using Mercator::TerrainMods. It handles parsing
//		the Atlas data and storing all the information needed by 
//		TerrainGenerator to add and remove them from the Terrain.
//
//		TerrainGenerator listens for changes in the modifier and
//		updates or removes the modifiers from the terrain as needed.
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
	class CraterTerrainMod;
}

namespace EmberOgre
{

class EmberEntity;

namespace Terrain {

class TerrainMod;

class InnerTerrainMod
{
public:
	virtual ~InnerTerrainMod();
	
	const std::string& getTypename() const;
	
	virtual bool parseAtlasData(const Atlas::Message::MapType& modElement) = 0;
	
	virtual Mercator::TerrainMod* getModifier() = 0;

protected:
	InnerTerrainMod(TerrainMod& terrainMod, const std::string& typemod);
	std::string mTypeName;
	TerrainMod& mTerrainMod;
};


class InnerTerrainModSlope : public InnerTerrainMod
{
public:
	InnerTerrainModSlope(TerrainMod& terrainMod);
	bool parseAtlasData(const Atlas::Message::MapType& modElement);
protected:
};

class InnerTerrainModCrater : public InnerTerrainMod
{
public:
	InnerTerrainModCrater(TerrainMod& terrainMod);
	virtual ~InnerTerrainModCrater();
	virtual bool parseAtlasData(const Atlas::Message::MapType& modElement);
	virtual Mercator::TerrainMod* getModifier();
protected:
	Mercator::CraterTerrainMod* mModifier;
};

class InnerTerrainModLevel : public InnerTerrainMod
{
public:
	InnerTerrainModLevel(TerrainMod& terrainMod);
	bool parseAtlasData(const Atlas::Message::MapType& modElement);
protected:
};

class InnerTerrainModAdjust : public InnerTerrainMod
{
public:
	InnerTerrainModAdjust(TerrainMod& terrainMod);
	bool parseAtlasData(const Atlas::Message::MapType& modElement);
protected:
};



/**
@author Tamas Bates
@brief Wrapper class that envelopes a Mercator::TerrainMod.
This class is mainly responsible for parsing atlas data and create or update an instance of Mercator::TerrainMod with the correct data.
The actual application of the Mercator::TerrainMod to the terrain and the subsequent update of the rendering display (i.e. the Ogre terrain) is handled mainly by TerrainGenerator, which reacts to the events emitted by this class whenever a terrain mod changes or is moved.
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
	 * @brief Accessor for the entity to which this terrain mod belongs.
	 * @return A pointer to the entity to which this terrain mod belongs.
	 */
	EmberEntity* getEntity() const;

protected:
	
	/**
	The owner of this modifier
	 */
	EmberEntity* mEntity;
	
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


	InnerTerrainMod* mInnerMod;
};

Mercator::TerrainMod* TerrainMod::getMod() const
{
	if (mInnerMod) {
		return mInnerMod->getModifier();
	}
	return 0;
}

}
};

#endif
