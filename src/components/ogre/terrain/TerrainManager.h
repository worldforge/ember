/*
    Copyright (C) 2004  Erik Ogenvik

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software Foundation,
    Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#ifndef EMBER_OGREVIEW_TERRAINMANAGER_H
#define EMBER_OGREVIEW_TERRAINMANAGER_H

#include "domain/IHeightProvider.h"
#include "framework/ConsoleObject.h"
#include "services/config/ConfigListenerContainer.h"
#include "TerrainShader.h"

#include <wfmath/point.h> //needed for the terrain position

#include <sigc++/trackable.h>
#include <sigc++/signal.h>
#include <sigc++/slot.h>

#include <OgrePrerequisites.h>
#include <OgreCommon.h>

#include <memory>

namespace Ogre {
class TerrainOptions;
}

namespace Mercator {
class Area;

class Terrain;

class Shader;

class TerrainMod;

class BasePoint;
}

namespace Eris {
class View;

class TimedEvent;

class EventService;
}

namespace Ember {

class TimeFrame;

namespace OgreView {
class ShaderManager;

struct ILightning;

class Scene;

class GraphicalChangeAdapter;

namespace Environment {
class Environment;

class Foliage;

class FoliageDetailManager;
}

/**
 * @brief Namespace for all terrain related classes and activities.
 */
namespace Terrain {

struct TerrainShader;

class TerrainInfo;

class TerrainPage;

struct TerrainLayerDefinition;

class TerrainPageSurfaceLayer;

struct ITerrainAdapter;

class ITerrainPageBridge;

class HeightMap;

class HeightMapBufferProvider;

struct TerrainDefPoint;

struct PlantAreaQuery;

struct PlantAreaQueryResult;

class SegmentManager;

class TerrainHandler;

namespace Techniques {
class CompilerTechniqueProvider;
}

namespace Foliage {
class Vegetation;
}

class DelayedFoliageInitializer;

/**
 * @brief Handles generation and updates of the terrain.
 *
 * Responsible for the setup of the actual terrain rendering done by Ogre.
 *
 * This involves getting terrain from Mercator, converting this to ogre format and creating materials to make it look good.
 *
 * It works closely with EmberTerrainPageSource.
 */
class TerrainManager : public virtual sigc::trackable,
					   public ConfigListenerContainer,
					   public Ember::IHeightProvider {
public:

	/*
	 * @brief Default ctor.
	 *
	 * @param adapter An adapter which binds the terrain to a scene manager. The terrain manager will take ownership of the adapter and will destroy it upon it's destruction.
	 * @param scene The world scene.
	 * @param shaderManager The shader manager.
	 */
	TerrainManager(std::unique_ptr<ITerrainAdapter> adapter,
				   Scene& scene,
				   Eris::View& view,
				   ShaderManager& shaderManager,
				   Eris::EventService& eventService,
				   GraphicalChangeAdapter& graphicalChangeAdapter);

	/**
	 * @brief Dtor.
	 */
	~TerrainManager() override;

	/**
	 * @brief Gets the adapter used to bind this manager to a scene manager.
	 * @return The adapter in use, or null if there is no one registered yet.
	 */
	ITerrainAdapter* getTerrainAdapter() const;

	/**
	 * @brief Accessor for the Scene instance to which this terrain manager is attached to.
	 *
	 * @returns The Scene instance used by the manager.
	 */
	Scene& getScene() const;

	/**
	 * @brief Gets the size of each foliage batch. This is used by the foliage system for setting up batch system for performance.
	 *
	 * @return The size of on foliage batch, in world units.
	 */
	unsigned int getFoliageBatchSize() const;

	/**
	 * @brief Whether the foliage should be shown or not.
	 *
	 * @note If the GPU doesn't support the required shaders, this will return false even though it's set in the config.
	 * @return
	 */
	bool isFoliageShown() const;

	/**
	 * @brief Returns the height at the specified position in the world.
	 *
	 * This will be done using the underlying Mercator data, which depending on LOD techniques used can differ some from the actual graphical representation.
	 * @note The method used for lookup does interpolation, so it's a little bit more expensive than doing a instant data lookup. Calling this is therefore not recommended if you're building height data, but suitable if you're placing entities on the terrain and need a perfect height.
	 * @param atPosition The position, in world space, to get the height for.
	 * @param height The height, in world space, at the specified position.
	 * @returns True if there was a valid, populated segment at the position (and therefore also a valid height).
	 */
	bool getHeight(const TerrainPosition& atPosition, float& height) const override;

	/**
	 * @brief Performs a fast copy of the raw height data for the supplied area.
	 * @param xMin Minimum x coord of the area.
	 * @param xMax Maximum x coord of the area.
	 * @param yMin Minimum y coord of the area.
	 * @param yMax Maximum y coord of the area.
	 * @param heights A vector into which heigh data will be placed. This should preferably already have a capacity reserved.
	 */
	void blitHeights(int xMin, int xMax, int yMin, int yMax, std::vector<float>& heights) const override;

	/**
	 * @brief Accessor for the main terrain info instance.
	 *
	 * @return An instance of TerrainInfo containing all relevant terrain info.
	 */
	const TerrainInfo& getTerrainInfo() const;

	/**
	 * @brief Gets the terrain handler.
	 *
	 * The terrain handler keeps track of the underlying Mercator terrain.
	 * @return The terrain handler.
	 */
	TerrainHandler& getHandler();

	/**
	 * @brief Place the plants for the supplied area in the supplied store.
	 *
	 * This method will perform the lookup in a background thread and return the results through an async callback.
	 * @param query The plant query.
	 * @param asyncCallback A callback to be called when the query has been executed in a background thread.
	 */
	void getPlantsForArea(PlantAreaQuery& query, sigc::slot<void, const PlantAreaQueryResult&> asyncCallback);

	/**
	 * @brief Gets all currently defined basepoints asynchronously.
	 *
	 * The call to the callback will happen in the main thread.
	 * @param asyncCallback The callback which will be called when all base points have been fetched.
	 */
	void getBasePoints(sigc::slot<void, std::map<int, std::map<int, Mercator::BasePoint>>&>& asyncCallback);

	/**
	 * @brief Starts the terrain paging.
	 *
	 * Calling this will cause the terrain system to start generating pages around the camera.
	 */
	void startPaging();

	/**
	 * @brief Emitted when a terrain page has had its geometry updated.
	 *
	 * This often signals that a page is to be loaded and rendered.
	 * @warning You must not do any rendering directly in the methods listening for this event, since that can lead to strange behaviour in the scene manager. The reason is that this event is emitted while already in the rendering loop, and entering the render loop again at that point will lead to some very inconsistent states.
	 */
	sigc::signal<void, TerrainPage&> EventTerrainPageGeometryUpdated;

	/**
	 * @brief Emitted when a terrain page is shown for the first time.
	 *
	 * This is emitted when a terrain page has finished loading and is shown to the user.
	 * The argument is a collection of areas, in world Ogre units, that makes up the pages and is now shown.
	 */
	sigc::signal<void, std::vector<Ogre::TRect<Ogre::Real>>> EventTerrainShown;

	/**
	 * @brief Emitted when the foliage has been created.
	 */
	sigc::signal<void> EventFoliageCreated;

protected:

	/**
	 * @brief Compiler technique provider.
	 */
	std::unique_ptr<Techniques::CompilerTechniqueProvider> mCompilerTechniqueProvider;

	/**
	 * @brief The terrain handler, which handles the underlying Mercator terrain.
	 */
	std::unique_ptr<TerrainHandler> mHandler;

	/**
	 * @brief True if foliage should be shown.
	 */
	bool mIsFoliageShown;

	/**
	 * @brief The adapter acts as a bridge between the manager and the terrain rendering component,
	 * 		  allowing a certain degree of decoupling.
	 */
	std::unique_ptr<ITerrainAdapter> mTerrainAdapter;

	unsigned int mFoliageBatchSize;

	std::unique_ptr<Foliage::Vegetation> mVegetation;

	/**
	 * @brief The scene in which the terrain manager operates.
	 */
	Scene& mScene;

	GraphicalChangeAdapter& mGraphicalChangeAdapter;
	Eris::View& mView;

	bool mIsInitialized;

	/**
	 * @brief The foliage system which provides different foliage functions.
	 */
	std::unique_ptr<Environment::Foliage> mFoliage;

	/**
	 * Utility object that can be used to manage detail level of foliage.
	 */
	std::unique_ptr<Environment::FoliageDetailManager> mFoliageDetailManager;

	/**
	 * @brief A foliage initializer which acts a bit delayed.
	 */
	std::unique_ptr<DelayedFoliageInitializer> mFoliageInitializer;

	void initializeTerrain();

	/**
	 * @brief Iterates through all TerrainPages and shows or hides the foliage.
	 */
	void updateFoliageVisibility();

	void config_Foliage(const std::string& section, const std::string& key, varconf::Variable& variable);

	void config_TerrainTechnique(const std::string& section, const std::string& key, varconf::Variable& variable);

	void config_TerrainPageSize(const std::string& section, const std::string& key, varconf::Variable& variable);

	void config_TerrainLoadRadius(const std::string& section, const std::string& key, varconf::Variable& variable);

	void terrainHandler_AfterTerrainUpdate(const std::vector<WFMath::AxisBox<2>>& areas, const std::set<TerrainPage*>& pages);

	void terrainHandler_ShaderCreated(const Terrain::TerrainLayer& layer);

	void terrainHandler_WorldSizeChanged();

	void terrainHandler_TerrainPageMaterialRecompiled(TerrainPage& page);

	/**
	 * @brief Listen to graphic level updates and ask the pages to regenerate their materials (since they will use different materials depending on the level chosen).
	 *
	 * @param shaderManager The shader manager, which contains information on the graphics level set.
	 */
	void shaderManager_LevelChanged(ShaderManager* shaderManager);

	/**
	 * @brief Bound to the adapter and called when a page is first shown.
	 *
	 * The signal will be passed on to EventTerrainShown
	 * @param rect
	 */
	void adapter_terrainShown(const Ogre::TRect<Ogre::Real>& rect);

	/**
	 * @brief Initializes foliage.
	 * @param graphicalChangeAdapter An adapter for graphical change events.
	 */
	void initializeFoliage();

};

inline unsigned int TerrainManager::getFoliageBatchSize() const {
	return mFoliageBatchSize;
}


/**
 @brief Allows for a delayed initialization of the foliage.

 The initialization will occur when either the sight queue is empty, or a certain time has elapsed.
 The main reason for doing this is that whenever a new area is added to the world, the foliage is invalidated and reloaded.
 As a result when the user first enters the world and is getting sent all the surrounding entities, there's a great chance that some of these entities will be areas. If the foliage then already has been initialized it will lead to the foliage being reloaded a couple of time.
 By delaying the loading of the foliage we can avoid this.

 @author Erik Ogenvik <erik@worldforge.org>

 */
class DelayedFoliageInitializer {
public:
	/**
	 * @brief Ctor.
	 * @param callback The callback to call when the foliage should be initialized.
	 * @param view The Eris::View object of the world. This will be used for querying about the size of the Sight queue.
	 * @param intervalMs In milliseconds how often to check if the queue is empty or time has elapsed. Defaults to 1 second.
	 * @param maxTimeMs In milliseconds the max time to wait until we initialize the foliage anyway.
	 */
	DelayedFoliageInitializer(std::function<void()> callback, Eris::View& view, unsigned int intervalMs = 1000, unsigned int maxTimeMs = 15000);

	/**
	 * @brief Dtor.
	 */
	virtual ~DelayedFoliageInitializer();

protected:
	std::function<void()> mCallback;
	Eris::View& mView;
	unsigned int mIntervalMs;
	unsigned int mMaxTimeMs;

	std::unique_ptr<Eris::TimedEvent> mTimeout;
	unsigned int mTotalElapsedTime;

	/**
	 * @brief Called when the time out has expired. We'll check for if either the set max time has elapsed, or if there's no more entities in the sight queue, and if so initialize the foliage. If not we'll just extend the waiting time.
	 */
	void timout_Expired();

};

}
}
}


#endif // EMBER_OGREVIEW_TERRAINMANAGER_H
