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

namespace EmberOgre
{

class EmberEntity;

namespace Terrain {

class TerrainMod;
class InnerTerrainMod_impl;

/**
@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
@brief Base class for all terrain mod specific classes.This is not meant to be used directly by anything else than the TerrainMod class.
The TerrainMod class in itself doesn't hold the actual reference to the terrain mod, and doesn't handle the final parsing of Atlas data. This is instead handled by the different subclasses of this class. Since the different kinds of terrain mods found in Mercator behave differently depending on their type and the kind of shape used, we need to separate the code for handling them into different classes.
*/
class InnerTerrainMod
{
public:
	/**
	* @brief Dtor.
	*/
	virtual ~InnerTerrainMod();
	
	/**
	 * @brief Gets the type of terrain mod handled by this.
	 * This corresponds to the "type" attribute of the "terrainmod" atlas attribute, for example "cratermod" or "slopemod".
	 * Internally, it's stored in the mTypeName field, as set through the constructor.
	 * @return The type of mod handled by any instance of this.
	 */
	const std::string& getTypename() const;
	
	/**
	 * @brief Tries to parse the Atlas data.
	 * It's up to the specific subclasses to provide proper parsing of the data depending on their needs.
	 * If the data is successfully parsed, a new Mercator::TerrainMod instance will be created.
	 * @param modElement The Atlas element describing the terrainmod. This should in most instances correspond directly to the "terrainmod" element found in the root atlas attribute map.
	 * @return If the parsing was successful, true will be returned, and a new Mercator::TerrainMod will have been created, else false.
	 */
	virtual bool parseAtlasData(const Atlas::Message::MapType& modElement) = 0;
	
	/**
	 * @brief Accessor for the Mercator::TerrainMod created and held by this instance.
	 * If no terrain mod could be created, such as with faulty Atlas data, or if parseAtlasData() hasn't been called yet, this will return a null pointer.
	 * @return A pointer to the TerrainMod held by this instance, or null if none created.
	 */
	virtual Mercator::TerrainMod* getModifier() = 0;

protected:

	/**
	 * @brief Ctor.
	 * This is protected to prevent any other class than subclasses of this to call it.
	 * @param terrainMod The TerrainMod instance to which this instance belongs to.
	 * @param typemod The type of terrainmod this handles, such as "cratermod" or "slopemod. This will be stored in mTypeName.
	 */
	InnerTerrainMod(TerrainMod& terrainMod, const std::string& typemod);
	
	/**
	 * @brief The type of mod this instance handles.
	 * @see getTypename()
	 */
	std::string mTypeName;
	
	/**
	 * @brief The TerrainMod instance to which this instance belongs.
	 */
	TerrainMod& mTerrainMod;
	
	/**
	 * @brief Parses the atlas data of the modifiers, finding the base atlas element for the shape definition, and returning the kind of shape specified.
	 * This is an utility method to help with those many cases where we need to parse the shape data in order to determine the kind of shape. The actual parsing and creation of the shape happens in InnerTerrainMod_impl however, since that depends on templated calls. However, in order to know what kind of template to use we must first look at the type of shape, thus the need for this method.
	 * @param modElement The atlas element containing the modifier.
	 * @param shapeMap A shape data is found, and it's in the map form, it will be put here.
	 * @return The name of the shape, or an empty string if no valid data could be found.
	 */
	const std::string& parseShape(const Atlas::Message::MapType& modElement, const Atlas::Message::MapType** shapeMap);
	
	
// 	template <typename InnerTerrainMod_implType>
// 	InnerTerrainMod_implType* createInnerTerrainMod_impInstance(const Atlas::Message::MapType& modElement);

};


/**
@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
@brief Inner terrain mod class for handling slope mods.
This will parse and create an instance of Mercator::SlopeTerrainMod, which is a mod which produces a sloped area in the landscape.
The main parsing of the atlas data and creation of the terrain mod occurs in InnerTerrainMod_impl however, as this is a mod that uses templated shapes.
*/
class InnerTerrainModSlope : public InnerTerrainMod
{
public:
	/**
	 * @brief Ctor.
	 * @param terrainMod The TerrainMod instance to which this instance belongs to.
	 */
	InnerTerrainModSlope(TerrainMod& terrainMod);
	
	/**
	 * @brief Dtor.
	 */
	virtual ~InnerTerrainModSlope();
	
	/**
	 * @copydoc InnerTerrainMod::parseAtlasData()
	 */
	virtual bool parseAtlasData(const Atlas::Message::MapType& modElement);
	
	/**
	 * @copydoc InnerTerrainMod::getModifier()
	 */
	virtual Mercator::TerrainMod* getModifier();
	
protected:
	/**
	 * @brief A reference to inner mod implementation.
	 * This is separate from this class because of the heavy use of templated shapes.
	 * The ownership is ours, so it will be destroyed when this instance is destroyed.
	 */
	InnerTerrainMod_impl* mModifier_impl;
};

/**
@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
@author Tamas Bates
@brief Handles a crater terrain mod.
This will parse and create an instance of Mercator::CraterTerrainMod, which is a mod which produces a crater in the landscape.
Note that this will not make use of InnerTerrainMod_impl since there's no templated shapes in use here.
TODO: Should perhaps this also use the same pattern of InnerTerrainMod_impl as the other mods, just to not break the pattern? /ehj
*/
class InnerTerrainModCrater : public InnerTerrainMod
{
public:
	/**
	 * @brief Ctor.
	 * @param terrainMod The TerrainMod instance to which this instance belongs to.
	 */
	InnerTerrainModCrater(TerrainMod& terrainMod);
	
	/**
	 * @brief Dtor.
	 */
	virtual ~InnerTerrainModCrater();
	
	/**
	 * @copydoc InnerTerrainMod::parseAtlasData()
	 */
	virtual bool parseAtlasData(const Atlas::Message::MapType& modElement);
	
	/**
	 * @copydoc InnerTerrainMod::getModifier()
	 */
	virtual Mercator::TerrainMod* getModifier();

protected:
	/**
	 * @brief A reference to the crater terrain modifier held by this instance.
	 * The ownership is ours, so it will be destroyed when this instance is destroyed.
	 */
	Mercator::CraterTerrainMod* mModifier;
};

/**
@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
@author Tamas Bates
@brief Handles a level terrain mod.
This will parse and create an instance of Mercator::LevelTerrainMod, which is a mod which produces a level area in the landscape.
The main parsing of the atlas data and creation of the terrain mod occurs in InnerTerrainMod_impl however, as this is a mod that uses templated shapes.
*/
class InnerTerrainModLevel : public InnerTerrainMod
{
public:
	/**
	 * @brief Ctor.
	 * @param terrainMod The TerrainMod instance to which this instance belongs to.
	 */
	InnerTerrainModLevel(TerrainMod& terrainMod);
	
	/**
	 * @brief Dtor.
	 */
	virtual ~InnerTerrainModLevel();
	
	/**
	 * @copydoc InnerTerrainMod::parseAtlasData()
	 */
	virtual bool parseAtlasData(const Atlas::Message::MapType& modElement);
	
	/**
	 * @copydoc InnerTerrainMod::getModifier()
	 */
	virtual Mercator::TerrainMod* getModifier();

protected:

	/**
	 * @brief A reference to inner mod implementation.
	 * This is separate from this class because of the heavy use of templated shapes.
	 * The ownership is ours, so it will be destroyed when this instance is destroyed.
	 */
	InnerTerrainMod_impl* mModifier_impl;
};

/**
@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
@author Tamas Bates
@brief Handles a level terrain mod.
This will parse and create an instance of Mercator::AdjustTerrainMod, which is a mod which adjusts the terrain within an area in the landscape.
The main parsing of the atlas data and creation of the terrain mod occurs in InnerTerrainMod_impl however, as this is a mod that uses templated shapes.
*/
class InnerTerrainModAdjust : public InnerTerrainMod
{
public:
	/**
	 * @brief Ctor.
	 * @param terrainMod The TerrainMod instance to which this instance belongs to.
	 */
	InnerTerrainModAdjust(TerrainMod& terrainMod);
	
	/**
	 * @brief Dtor.
	 */
	virtual ~InnerTerrainModAdjust();
	
	/**
	 * @copydoc InnerTerrainMod::parseAtlasData()
	 */
	virtual bool parseAtlasData(const Atlas::Message::MapType& modElement);
	
	/**
	 * @copydoc InnerTerrainMod::getModifier()
	 */
	virtual Mercator::TerrainMod* getModifier();

protected:

	/**
	 * @brief A reference to inner mod implementation.
	 * This is separate from this class because of the heavy use of templated shapes.
	 * The ownership is ours, so it will be destroyed when this instance is destroyed.
	 */
	InnerTerrainMod_impl* mModifier_impl;
};


/**
@author Tamas Bates
@author Erik Hjortsberg
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
	 @brief The owner of this modifier.
	 */
	EmberEntity* mEntity;
	
	/**
	 * @brief Slot used to listen for changes to attributes in the Entity to which this mod belongs to.
	 */
	Eris::Entity::AttrChangedSlot mAttrChangedSlot;

	/**
	 * @brief Called whenever a modifier is changed and handles the update
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
	 * @brief Sets up the previous three handler functions to be called when a change
	 * is made to the entity holding the modifier. 
	 */
	void observeEntity();

	/**
	 * @brief Parses the Atlas data for a modifier
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

	/**@brief Creates a SlopeTerrainMod based on a shape and position
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

	/**
	 * @brief The inner terrain mod instance which holds the actual Mercator::TerrainMod instance and handles the parsing of it.
	 * In order to be able to better support different types of mods the actual instance will be any of the subclasses of InnerTerrainMod, depending on the type of the mod.
	 */
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
