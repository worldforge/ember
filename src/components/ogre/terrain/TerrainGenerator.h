/*
    Copyright (C) 2004  Erik Hjortsberg

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


#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include "../EmberOgrePrerequisites.h"

#include <wfmath/point.h>

#include <sigc++/trackable.h>
#include <sigc++/signal.h>

#include "framework/ConsoleObject.h"

#include "TerrainInfo.h"
#include "../MathConverter.h"
namespace Ogre
{
	class TerrainOptions;
}

namespace Eris
{
	class Entity;
	class View;
}

namespace Mercator
{
	class Area;
	class Terrain;
	class Shader;
	class Segment;
	class TerrainMod;
}

namespace EmberOgre {
class EmberEntity;
class EmberEntityFactory;
class EmberPagingSceneManager;

namespace Environment
{
	class Environment;
}

namespace Terrain {
	
class TerrainShader;

class TerrainPage;
class TerrainArea;
class TerrainMod;
class TerrainLayerDefinition;
class TerrainPageSurfaceLayer;
class ISceneManagerAdapter;

/**
@brief Defines the height of a special "base point" in the terrain. 
These base points are then user by Mercator::Terrain for generating the actual terrain.
*/
struct TerrainDefPoint
{
	public:
	/**
	*       Ctor.
	* @param x The position of the point, on the x axis, in world units.
	* @param y The position of the point, on the y axis, in world units.
	* @param terrainHeight The height of the point, in world units.
	*/
	TerrainDefPoint(float x, float y, float terrainHeight) : mPosition(x,y), mHeight(terrainHeight) {}
	
	/**
	 * @brief Gets the position of the definition point, in world units.
	 * @return The position of the point.
	 */
	inline const TerrainPosition& getPosition() const;
	
	/**
	 * @brief Gets the height of the definition point, in world units.
	 * @return The height of the point.
	 */
	inline float getHeight() const;
	
	private:
	/**
	The position of the point, in world units.
	*/
	TerrainPosition mPosition;
	
	/**
	The height of the point, in world units.
	*/
	float mHeight;
};



/**
 * @brief Handles generation and updates of the terrain.
 * This class takes care of generating terrain for Ogre's scenemanager.
 * This involves getting terrain from Mercator, converting this to ogre
 * format and creating materials to make it look good.
 * 
 * It works closely with EmberTerrainPageSource.
 * 
 */
class TerrainGenerator :  public Ogre::FrameListener, 
public sigc::trackable, public Ember::ConsoleObject
{
public:

	/**
	A type used for storing changes to aeas. We use instances instead of pointers or references since this type will be used in delayed updating, where the originating instance might not any longer be around.
	*/
	typedef std::vector<Mercator::Area> AreaStore;
	
	/**
	A type used for storing the terrain definition points.
	*/
	typedef std::vector<TerrainDefPoint> TerrainDefPointStore;

	/**
	 * STL map to store sparse array of TerrainPage pointers.
	 * 
	 */ 
	typedef std::map<int, TerrainPage *> TerrainPagecolumn;
	
	/**
	* STL map to store sparse array of TerrainPage pointer columns.
	*/
	typedef std::map<int, TerrainPagecolumn > TerrainPagestore;
	
	/**
	 * @brief Default ctor.
	 * @param adapter An adapter which binds the terrain to a scene manager. The terrain generator will take ownership of the adapter and will destroy it upon it's destruction.
	 */
	TerrainGenerator(ISceneManagerAdapter* adapter);
	
	
	/**
	 *    Dtor.
	 */
	virtual ~TerrainGenerator();

	
	/**
	 * @brief At each frame, we check for updates shaders and updates the terrain. This is because we want to batch together changes.
	 * @param evt 
	 * @return 
	 */
	virtual bool frameEnded(const Ogre::FrameEvent & evt);

// 	void prepareSegments(long segmentXStart, long segmentZStart, long numberOfSegments, bool alsoPushOntoTerrain);

	/**
	 * Prepares all segments aquired from Mercator. Note that this can be very,
	 * very expensive if there's a lot of terrain defined.
	 */
	void prepareAllSegments();
	
	/**
	 * @brief Returns the height at the specified position in the world.
	 * This will be done using the underlying Mercator data, which depending on LOD techniques used can differ some from the actual graphical representation.
	 * @param atPosition The position, in world space, to get the height for.
	 * @return The height, in world space, at the specified position.
	 */
	virtual float getHeight(const TerrainPosition& atPosition) const;
	
	/**
	 *    @brief Updates the terrain with new terrain points.
	 * @param terrainIndexPoints A list of terrain index points, i.e. points positioned using the base point positioning scale. In normal setup that's 64 meters per index point, so an index point of 2:1 would translate to real world coords of 128:64
	 * @return True if the terrain was successfully updated.
	 */
	bool updateTerrain(const TerrainDefPointStore& terrainIndexPoints);


	/**
	 * @brief Return true if there is a valid piece of terrain at the supplied segment indices.
	 * By valid means a populated terrain-
	 * @param pos A position in the world.
	 * @return true if the terrain at the position is valid.
	 */
	bool isValidTerrainAt(const TerrainPosition& pos);
	
	/**
	 * @brief Provides access to the underlying Mercator::Terrain object.
	 * @return The main terrain object.
	 */
	Mercator::Terrain& getTerrain();
	
	/**
	 * @brief Gets the max boundaries of the terrain.
	 * @return 
	 */
	const TerrainPosition getMax() const;
	
	/**
	 *    @brief Gets the min boundaries of the terrain.
	 * @return 
	 */
	const TerrainPosition getMin() const;
	
	/**
	 *   @brief Gets the size of one terrain segment.
	 *	(note that this is different from Mercator segments, which always are of size 64)
	 * @return 
	 */
	int getPageMetersSize();
	

	/**
	 * @brief Gets the size of one page as indices.
	 * @return 
	 */
	int getPageIndexSize() ;
		
	/**
	 * @brief Adds a new Mercator::Area to the terrain.
	 * @param area 
	 */
	void addArea(TerrainArea* terrainArea);

	/**
	 *    @brief Adds a new Mercator::TerrainMod to the terrain.
	 * @param mod
	 */
	void addTerrainMod(TerrainMod* terrainMod);
	
	/**
	 * @brief Returns a TerrainPage, creating one if there's no existing and so requested.
	 * If createIfMissing is true and there is no yet existing, a new one is created.
	 * @param ogreIndexPosition The index, in Ogre space, to create the new page on.
	 * @param createIfMissing If set to true, a new page will be created if there's no pre-existing. Defaults to true.
	 * @return An instance of terrain page, or null if there was no existing one and createIfMissing was set to false.
	 */
	TerrainPage* getTerrainPageAtIndex(const Ogre::Vector2& ogreIndexPosition, bool createIfMissing = true);
	
	/**
	 * @brief Gets the page at the specified position in the world. If no page can be found, a null pointer is returned.
	 * @param worldPosition The position in world space to get the terrain page for.
	 * @return An terrain page, or null of no page can be found at the specified position.
	 */
	TerrainPage* getTerrainPageAtPosition(const TerrainPosition& worldPosition);

	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command 
	 * @param args 
	 */
	virtual	void runCommand(const std::string &command, const std::string &args);
	
	/**
	 * @brief Rebuilds the Mercator height map, effectively regenerating the terrain.
	 * Note that this only regenerates the Mercator height map, and won't update the Ogre representation.
	 */
	void buildHeightmap();
	
	/**
	 * @brief Accessor for the main terrain info instance.
	 * @return An instance of TerrainInfo containing all relevant terrain info.
	 */
	const TerrainInfo& getTerrainInfo() const;
	
	/**
	Emitted when the size of the world has changed.
	*/
	sigc::signal<void> EventWorldSizeChanged;
	
	
	/**
	 *    @brief Registers to the scene manager adapter to be used by the generator.
	 * The adapter acts as a bridge between the generator and the actual scene manager, allowing a certain degree of decoupling.
	 * @param adapter The adapter to use.
	 */
	void registerSceneManagerAdapter(ISceneManagerAdapter* adapter);
	
	/**
	 * @brief Gets the adapter used to bind this generator to a scene manager.
	 * @return The adapter in use, or null if there is no one registered yet.
	 */
	ISceneManagerAdapter* getAdapter() const;
	
	/**
	 * @brief Accessor for all instances of TerrainPage that are registered with the generator.
	 * @return A store of TerrainPage instances.
	 */
	inline const TerrainPagestore& getTerrainPages() const;
	
	/**
	 * @brief Create and registers a new texture shader.
	 * @param layerDef The terrain layer defintiion to use. This will be used to determine what textures and materials to use for rendering the layer defined by the shader.
	 * @param mercatorShader The Mercator::Shader to use.
	 * @return 
	 */
	TerrainShader* createShader(const TerrainLayerDefinition* layerDef, Mercator::Shader* mercatorShader);
	
	
	/**
	 * @brief Regenerates all terrain shadow maps.
	 */
	void updateShadows();
	
	/**
	 * @brief Console command for updating all terrain shadow maps.
	 */
	const Ember::ConsoleCommandWrapper UpdateShadows;
	
	
	/**
	 * @brief Gets the shadow colour at the supplied position.
	 * @param position The position in the world.
	 * @param colour The shadow colour will be put into this value.
	 */
	void getShadowColourAt(const Ogre::Vector2& position, Ogre::uint32& colour);
	
	/**
	 * @brief Gets the shadow colour at the supplied position.
	 * @param position The position in the world.
	 * @param colour The shadow colour will be put into this value.
	 */
	void getShadowColourAt(const Ogre::Vector2& position, Ogre::ColourValue& colour);

	/**
	Emitted when a layer is updated.
	The vector parameter is either null if the update can't be constrained to any areas, or an vector of areas if it can.
	*/
	sigc::signal<void, TerrainShader*, AreaStore* > EventLayerUpdated;
	
	/**
	Emitted when a new shader is created.
	*/
	sigc::signal<void, TerrainShader*> EventShaderCreated;
	
	/**
	@brief Emitted before the terrain geometry is changed.
	
	When the terrain geometry is about to be changed this signal is emitted.
	The first parameter is the TerrainPositions that were updated, i.e. the BasePoints that were changed. The terrain is generated from a series of base points, in the default setting dispersed with 64 meters between them.
	The second parameter is the pages that will be updated.
	*/
	sigc::signal<void, std::vector<TerrainPosition>&, std::set<TerrainPage*>&> EventBeforeTerrainUpdate;
	
	/**
	@brief Emitted after the terrain geometry has changed.
	
	When the terrain geometry is about to be changed this signal is emitted.
	The first parameter is the TerrainPositions that were updated, i.e. the BasePoints that were changed. The terrain is generated from a series of base points, in the default setting dispersed with 64 meters between them.
	The second parameter is the pages that were updated.
	*/
	sigc::signal<void, std::vector<TerrainPosition>&, std::set<TerrainPage*>&> EventAfterTerrainUpdate;
	
	/**
	 *    Gets the size of each foliage batch. This is used by the foliage system for setting up batch system for performance.
	 * @return 
	 */
	inline unsigned int getFoliageBatchSize() const;

protected:

	/**
	Information about the world, such as size and number of pages.
	*/
	TerrainInfo mTerrainInfo;

	typedef std::map<int,TerrainShader*> AreaShaderstore;
	AreaShaderstore mAreaShaders;

	/**
	 * @brief Marks a shader for update, to be updated on the next batch, normally a frameEnded event.
	 * For performance reasons we want to batch together multiple request for shader updates, so we can do them all at once, normally on frameEnded(). By calling this method the supplied shader will be marked for updating.
	 * @param shader The shader to update.
	 * @param terrainArea If an area is specified here, only pages touched by the area will be updated.
	 */
	void markShaderForUpdate(TerrainShader* shader, TerrainArea* terrainArea = 0);
	
	typedef std::set<TerrainShader*> ShaderSet;
	ShaderSet mShadersToUpdate;
	
	typedef std::map<std::string, TerrainPage*> PageStore;
	PageStore mPages;
	
	/**
	A type for use when keeping track of changes done to areas. We use instances of Mercator::Area instead of pointers or references since we want to batch the updates, and the original area instances might not be around at that time.
	*/
	typedef std::map<TerrainShader*, std::vector<Mercator::Area> > TerrainAreaMap;
	TerrainAreaMap mChangedTerrainAreas;


	typedef std::multimap<const std::string, Mercator::TerrainMod*> TerrainModMap;
	TerrainModMap mTerrainMods;
	
	TerrainPagestore mTerrainPages;
	
	Mercator::Terrain* mTerrain;
	
	Ogre::Real mHeightMax, mHeightMin;
	
	WFMath::Point<3> mCurrentPosition;

	const std::map<int, std::map<int, Mercator::Segment*> >* mSegments;
	
	/**
	 * the min and max indices for segments
	 */
// 	int mXmin, mXmax, mYmin, mYmax;
	
	void loadTerrainOptions();
	
	/**
	true if we have some kind of terrain info, i.e. if mX* and mY* are valid
	*/
	bool mHasTerrainInfo;
	
	

	/**
	 *   Creates a new TerrainPage and puts it in mTerrainPages
	 * @param pos 
	 */
	TerrainPage* createPage(const TerrainPosition& pos);

	
	typedef std::map<const Mercator::Shader*, TerrainShader*> ShaderStore;
	
	/**
	 * This holds a map of the TerrainShaders
	 */
	ShaderStore mShaderMap;
	
	/**
	@brief A list of the shaders, which will all be used on all Pages.
	*/
	std::list<TerrainShader*> mBaseShaders;
	 
	
	/**
	 * @brief Reloads the terrain found at the specified positions.
	 * Calling this method will update both the internal Mercator heightfield data as well as the Ogre graphical representation.
	 * @param positions A vector of terrain positions, in world space. The terrain found at these positions will be reloaded. Note that if there's any pages between these positions, these will not be updated.
	 * TODO: Add a similiar method which also takes a WFMath::AxisBox<2> or another area.
	 */
	void reloadTerrain(std::vector<TerrainPosition>& positions);
	void updateHeightMapAndShaders(const std::set<TerrainPage*>& pagesToUpdate);
	void updateEntityPositions(const std::set<TerrainPage*>& pagesToUpdate);
	void updateEntityPosition(EmberEntity* entity, const std::set<TerrainPage*>& pagesToUpdate);

	
	/**
	 *    @brief Whether the foliage should be shown or not.
	 *    @note If the GPU doesn't support the required shaders, this will return false even though it's set in the config.
	 * @return 
	 */
	bool isFoliageShown() const;
	
	/**
	 *    @brief Iterates through all TerrainPages and shows or hides the foliage.
	 */
	void updateFoliageVisibilty();

	/**
	 *    @brief Catch changes to the configuration.
	 * @param section 
	 * @param key 
	 */
	void ConfigService_EventChangedConfigItem(const std::string& section, const std::string& key);
	
	/**
	Listen to changes in areas.
	*/
	void TerrainArea_Changed(TerrainArea* terrainArea);
	
	/**
	 * @brief Listen to removal of terrain areas and trigger an update of the terrain.
	 * @param terrainArea The area being removed.
	 */
	void TerrainArea_Removed(TerrainArea* terrainArea);
	
	/**
	Listen to changes in terrain mods.
	*/
	void TerrainMod_Changed(TerrainMod* terrainMod);
	
	/**
	@brief An adapter class which allows us to access the Ogre scene manager.
	Note that even though this is passed as a parameter in the constructor, this class is then responsible for its destruction.
	*/
	void TerrainMod_Deleted(TerrainMod* terrainMod);
	ISceneManagerAdapter* mSceneManagerAdapter;
	
	unsigned int mFoliageBatchSize;

};

const TerrainGenerator::TerrainPagestore& TerrainGenerator::getTerrainPages() const
{
	return mTerrainPages;
}

unsigned int TerrainGenerator::getFoliageBatchSize() const
{
	return mFoliageBatchSize;
}

const TerrainPosition& TerrainDefPoint::getPosition() const
{
	return mPosition;
}
	
float TerrainDefPoint::getHeight() const
{
	return mHeight;
}


}
}


#endif // TERRAINGENERATOR_H
