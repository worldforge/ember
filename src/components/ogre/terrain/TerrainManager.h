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


#ifndef TerrainManager_H
#define TerrainManager_H

#include "Types.h"
#include "framework/ConsoleObject.h"
#include "services/config/ConfigListenerContainer.h"

#include <wfmath/point.h> //needed for the terrain position

#include <sigc++/trackable.h>
#include <sigc++/signal.h>

#include <OgreFrameListener.h>

#include <memory>

namespace Ogre {
	class TerrainOptions;
}

namespace Mercator {
	class Area;
	class Terrain;
	class Shader;
	class TerrainMod;
}

namespace Ember {
namespace Tasks {
	class TaskQueue;
}
}

namespace EmberOgre {
class EmberEntity;
class ShaderManager;

namespace Terrain {

class TerrainShader;
class TerrainInfo;
class TerrainPage;
class TerrainArea;
class TerrainMod;
class TerrainLayerDefinition;
class TerrainPageSurfaceLayer;
class ISceneManagerAdapter;
class ITerrainPageBridge;
class HeightMap;
class HeightMapBufferProvider;

/**
 * @brief Handles generation and updates of the terrain.
 *
 * This class takes care of generating terrain for Ogre's scenemanager.
 *
 * This involves getting terrain from Mercator, converting this to ogre format and creating materials to make it look good.
 *
 * It works closely with EmberTerrainPageSource.
 */
class TerrainManager : public Ogre::FrameListener, public sigc::trackable, public Ember::ConsoleObject, public Ember::ConfigListenerContainer
{
public:

	/**
	 * @brief Default ctor.
	 * @param adapter An adapter which binds the terrain to a scene manager. The terrain manager will take ownership of the adapter and will destroy it upon it's destruction.
	 */
	TerrainManager(ISceneManagerAdapter* adapter);

	/**
	 * @brief Dtor.
	 */
	virtual ~TerrainManager();

	/**
	 * @brief At each frame, we check for updates shaders and updates the terrain. This is because we want to batch together changes.
	 * @param evt
	 * @return
	 */
	virtual bool frameEnded(const Ogre::FrameEvent & evt);

	/**
	 * @copydoc Ember::ConsoleObject::runCommand
	 */
	virtual	void runCommand(const std::string& command, const std::string& args);

	/**
	 * @brief Prepares all segments acquired from Mercator. Note that this can be very, very expensive if there's a lot of terrain defined.
	 */
	void prepareAllSegments();

	/**
	 * @brief Returns the height at the specified position in the world.
	 * This will be done using the underlying Mercator data, which depending on LOD techniques used can differ some from the actual graphical representation.
	 * @note The method used for lookup does interpolation, so it's a little bit more expensive than doing a instant data lookup. Calling this is therefore not recommended if you're building height data, but suitable if you're placing entities on the terrain and need a perfect height.
	 * @param atPosition The position, in world space, to get the height for.
	 * @param height The height, in world space, at the specified position.
	 * @returns True if there was a valid, populated segment at the position (and therefore also a valid height).
	 */
	bool getHeight(const TerrainPosition& atPosition, float& height) const;

	/**
	 * @brief Updates the terrain with new terrain points.
	 * @param terrainIndexPoints A list of terrain index points, i.e. points positioned using the base point positioning scale. In normal setup that's 64 meters per index point, so an index point of 2:1 would translate to real world coords of 128:64
	 * @return True if the terrain was successfully updated.
	 */
	bool updateTerrain(const TerrainDefPointStore& terrainIndexPoints);

	/**
	 * @brief Gets the max boundaries of the terrain.
	 * @return
	 */
	const TerrainPosition getMax() const;

	/**
	 * @brief Gets the min boundaries of the terrain.
	 * @return
	 */
	const TerrainPosition getMin() const;

	/**
	 * @brief Gets the size of one terrain segment.
	 * (note that this is different from Mercator segments, which always are of size 64)
	 * @return
	 */
	int getPageMetersSize() const;


	/**
	 * @brief Adds a new Mercator::Area to the terrain.
	 * @param area Area to be added
	 */
	void addArea(TerrainArea* terrainArea);

	/**
	 * @brief Adds a new Mercator::TerrainMod to the terrain.
	 * @param mod Mod to be added
	 */
	void addTerrainMod(TerrainMod* terrainMod);

	/**
	 * @brief Create and registers a new texture shader.
	 * @param layerDef The terrain layer defintiion to use. This will be used to determine what textures and materials to use for rendering the layer defined by the shader.
	 * @param mercatorShader The Mercator::Shader to use.
	 * @return
	 */
	TerrainShader* createShader(const TerrainLayerDefinition* layerDef, Mercator::Shader* mercatorShader);

	/**
	 * @brief Sets up a TerrainPage.
	 * @param ogreIndexPosition The index, in Ogre space, to create the new page on.
	 * @param bridge The bridge to bind it to Ogre representation
	 */
	void setUpTerrainPageAtIndex(const Ogre::Vector2& ogreIndexPosition, ITerrainPageBridge& bridge);

	/**
	 * @brief Returns a TerrainPage.
	 * @param ogreIndexPosition The index, in Ogre space, to create the new page on.
	 * @return An instance of terrain page, or null if there was no existing one.
	 */
	TerrainPage* getTerrainPageAtIndex(const Ogre::Vector2& ogreIndexPosition);

	/**
	 * @brief Gets the page at the specified position in the world. If no page can be found, a null pointer is returned.
	 * @param worldPosition The position in world space to get the terrain page for.
	 * @return An terrain page, or null of no page can be found at the specified position.
	 */
	TerrainPage* getTerrainPageAtPosition(const TerrainPosition& worldPosition) const;

	/**
	 * @brief Accessor for the main terrain info instance.
	 * @return An instance of TerrainInfo containing all relevant terrain info.
	 */
	const TerrainInfo& getTerrainInfo() const;

	/**
	 * @brief Gets the adapter used to bind this manager to a scene manager.
	 * @return The adapter in use, or null if there is no one registered yet.
	 */
	ISceneManagerAdapter* getAdapter() const;

	/**
	 * @brief Gets the size of each foliage batch. This is used by the foliage system for setting up batch system for performance.
	 * @return The size of on foliage batch, in world units.
	 */
	unsigned int getFoliageBatchSize() const;

	/**
	 * @brief Accessor for base shaders.
	 * @return A store of shaders.
	 */
	const std::list<TerrainShader*>& getBaseShaders() const;


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
	void getShadowColourAt(const Ogre::Vector2& position, Ogre::uint32& colour) const;

	/**
	 * @brief Gets the shadow colour at the supplied position.
	 * @param position The position in the world.
	 * @param colour The shadow colour will be put into this value.
	 */
	void getShadowColourAt(const Ogre::Vector2& position, Ogre::ColourValue& colour) const;

	/**
	 * @brief Gets the normal at the specified world position.
	 * If an invalid world position is specified the method will return false.
	 * @param worldPosition The world position to get the normal for.
	 * @param normal The normal values will be placed in this.
	 * @return True if a valid segment and normal could be found at the specified world position.
	 */
	bool getNormal(const TerrainPosition& worldPosition, WFMath::Vector<3>& normal) const;

	/**
	 * @brief Provides access to the underlying Mercator::Terrain object.
	 * @return The main terrain object.
	 */
	const Mercator::Terrain& getTerrain() const;

	/**
	 * @brief Gets the size of one page as indices.
	 * @return The size of one page as indices.
	 */
	int getPageIndexSize() const;

	/**
	 * @brief Return true if there is a valid piece of terrain at the supplied segment indices.
	 * By valid means a populated terrain-
	 * @param pos A position in the world.
	 * @return true if the terrain at the position is valid.
	 */
	bool isValidTerrainAt(const TerrainPosition& pos) const;

	/**
	 * @brief Emitted when the size of the world has changed.
	 */
	sigc::signal<void> EventWorldSizeChanged;

	/**
	@brief Emitted when a layer is updated.
	The vector parameter is either null if the update can't be constrained to any areas, or an vector of areas if it can.
	*/
	sigc::signal<void, const TerrainShader*, const AreaStore* > EventLayerUpdated;

	/**
	@brief Emitted when a new shader is created.
	The shader paremeter is the newly created shader.
	*/
	sigc::signal<void, const TerrainShader*> EventShaderCreated;

	/**
	@brief Emitted before the terrain geometry is changed.

	When the terrain geometry is about to be changed this signal is emitted.
	The first parameter is the TerrainPositions that were updated, i.e. the BasePoints that were changed. The terrain is generated from a series of base points, in the default setting dispersed with 64 meters between them.
	The second parameter is the pages that will be updated.
	*/
	sigc::signal<void, std::vector<TerrainPosition>&, std::set<TerrainPage*>&> EventBeforeTerrainUpdate;

	/**
	@brief Emitted after the terrain geometry has changed.

	When the terrain geometry has been changed this signal is emitted.
	The first parameter is the TerrainPositions that were updated, i.e. the BasePoints that were changed. The terrain is generated from a series of base points, in the default setting dispersed with 64 meters between them.
	The second parameter is the pages that were updated.
	*/
	sigc::signal<void, std::vector<TerrainPosition>&, std::set<TerrainPage*>&> EventAfterTerrainUpdate;

	/**
	 * @brief Emitted when a terrain page has had its geometry updated.
	 *
	 * This often signals that a page is to be loaded and rendered.
	 * @warning You must not do any rendering directly in the methods listening for this event, since that can lead to strange behaviour in the scene manager. The reason is that this event is emitted while already in the rendering loop, and entering the render loop again at that point will lead to some very inconsistent states.
	 */
	sigc::signal<void, TerrainPage&> EventTerrainPageGeometryUpdated;

	/**
	 *    @brief Whether the foliage should be shown or not.
	 *    @note If the GPU doesn't support the required shaders, this will return false even though it's set in the config.
	 * @return
	 */
	bool isFoliageShown() const;

	void addPage(TerrainPage* page);

protected:

	/**
	@brief Information about the world, such as size and number of pages.
	*/
	std::auto_ptr<TerrainInfo> mTerrainInfo;

	/**
	@brief We use this to keep track on the terrain shaders used for areas, stored with the layer id as the key.
	*/
	AreaShaderstore mAreaShaders;

	/**
	 * @brief Marks a shader for update, to be updated on the next batch, normally a frameEnded event.
	 * For performance reasons we want to batch together multiple request for shader updates, so we can do them all at once, normally on frameEnded(). By calling this method the supplied shader will be marked for updating.
	 * @param shader The shader to update.
	 * @param terrainArea If an area is specified here, only pages touched by the area will be updated.
	 */
	void markShaderForUpdate(const TerrainShader* shader,  Mercator::Area* terrainArea = 0);

	/**
	 * @brief Stores the shaders needing update, to be processed on the next frame.
	 * For performance reasons we try to batch all shaders updates together, rather than doing them one by one. This is done by adding the shaders needing update to this store, and then on frameEnded processing them.
	 * @see markShaderForUpdate
	 * @see frameEnded
	 */
	ShaderUpdateSet mShadersToUpdate;

	PageStore mPages;

	TerrainAreaMap mChangedTerrainAreas;

	TerrainModMap mTerrainMods;

	TerrainPagestore mTerrainPages;

	Mercator::Terrain* mTerrain;

	Ogre::Real mHeightMax, mHeightMin;

	void loadTerrainOptions();

	/**
	true if we have some kind of terrain info, i.e. if mX* and mY* are valid
	*/
	bool mHasTerrainInfo;

	/**
	 * This holds a map of the TerrainShaders
	 */
	ShaderStore mShaderMap;

	/**
	@brief A list of the shaders, which will all be used on all Pages.
	*/
	std::list<TerrainShader*> mBaseShaders;


	/**
	 * @brief True if foliage should be shown.
	 */
	bool mIsFoliageShown;

	HeightMap* mHeightMap;

	HeightMapBufferProvider* mHeightMapBufferProvider;


	/**
	 * @brief Reloads the terrain found at the specified positions.
	 * Calling this method will update both the internal Mercator heightfield data as well as the Ogre graphical representation.
	 * @param positions A vector of terrain positions, in world space. The terrain found at these positions will be reloaded. Note that if there's any pages between these positions, these will not be updated.
	 * TODO: Add a similiar method which also takes a WFMath::AxisBox<2> or another area.
	 */
	void reloadTerrain(std::vector<TerrainPosition>& positions);
	void updateHeightMapAndShaders(const std::set<TerrainPage*>& pagesToUpdate);
	static void updateEntityPositions(const std::set<TerrainPage*>& pagesToUpdate);
	static void updateEntityPosition(EmberEntity* entity, const std::set<TerrainPage*>& pagesToUpdate);


	/**
	 *    @brief Iterates through all TerrainPages and shows or hides the foliage.
	 */
	void updateFoliageVisibility();

	void config_Foliage(const std::string& section, const std::string& key, varconf::Variable& variable);

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
	 * @brief Listen to swapping of terrain areas and trigger an update of the terrain.
	 * @param oldArea The area being removed.
	 * @param terrainArea The terrain area.
	 */
	void TerrainArea_Swapped(Mercator::Area& oldArea, TerrainArea* terrainArea);

	/**
	Listen to changes in terrain mods.
	*/
	void TerrainMod_Changed(TerrainMod* terrainMod);

	/**
	@brief An adapter class which allows us to access the Ogre scene manager.
	Note that even though this is passed as a parameter in the constructor, this class is then responsible for its destruction.
	*/
	void TerrainMod_Deleted(TerrainMod* terrainMod);
	/**
	 * The adapter acts as a bridge between the manager and the actual scene manager, allowing a certain degree of decoupling.
	 */
	ISceneManagerAdapter* mSceneManagerAdapter;

	unsigned int mFoliageBatchSize;

	/**
	 * @brief Listen to graphic level updates and ask the pages to regenerate their materials (since they will use different materials depending on the level chosen).
	 * @param shaderManager The shader manager, which contains information on the graphics level set.
	 */
	void shaderManager_LevelChanged(ShaderManager* shaderManager);

	/**
	 * @brief Rebuilds the Mercator height map, effectively regenerating the terrain.
	 * Note that this only regenerates the Mercator height map, and won't update the Ogre representation.
	 */
	void buildHeightmap();


	void application_EndErisPoll(float);

	/**
	 * @brief The task queue we'll use for all background terrain updates.
	 */
	std::auto_ptr<Ember::Tasks::TaskQueue> mTaskQueue;

};

inline const std::list<TerrainShader*>& TerrainManager::getBaseShaders() const
{
	return mBaseShaders;
}
inline unsigned int TerrainManager::getFoliageBatchSize() const
{
	return mFoliageBatchSize;
}

inline const TerrainPosition& TerrainDefPoint::getPosition() const
{
	return mPosition;
}

inline float TerrainDefPoint::getHeight() const
{
	return mHeight;
}

}
}


#endif // TerrainManager_H
