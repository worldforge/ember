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


#ifndef EMBER_OGREVIEW_TERRAINMANAGER_H
#define EMBER_OGREVIEW_TERRAINMANAGER_H

#include "Types.h"
#include "framework/ConsoleObject.h"
#include "services/config/ConfigListenerContainer.h"

#include <wfmath/point.h> //needed for the terrain position

#include <sigc++/trackable.h>
#include <sigc++/signal.h>
#include <sigc++/slot.h>

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
	class BasePoint;
}

namespace Ember {

namespace OgreView {
class ShaderManager;
class ILightning;
class Scene;

/**
 * @brief Namespace for all terrain related classes and activities.
 */
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
class TerrainDefPoint;
class PlantAreaQuery;
class PlantAreaQueryResult;
class SegmentManager;
class TerrainHandler;

namespace Techniques {
class CompilerTechniqueProvider;
}

namespace Foliage {
class Vegetation;
}

/**
 * @brief Handles generation and updates of the terrain.
 *
 * This class takes care of generating terrain for Ogre's scenemanager.
 *
 * This involves getting terrain from Mercator, converting this to ogre format and creating materials to make it look good.
 *
 * It works closely with EmberTerrainPageSource.
 */
class TerrainManager : public Ogre::FrameListener, public virtual sigc::trackable, public ConsoleObject, public ConfigListenerContainer
{
public:

	/**
	 * @brief Default ctor.
	 *
	 * @param adapter An adapter which binds the terrain to a scene manager. The terrain manager will take ownership of the adapter and will destroy it upon it's destruction.
	 * @param scene The world scene.
	 * @param shaderManager The shader manager.
	 * @param erisEndPollSignal A signal emitted when one Eris polling cycle has ended.
	 */
	TerrainManager(ISceneManagerAdapter* adapter, Scene& scene, ShaderManager& shaderManager, sigc::signal<void, float>& erisEndPollSignal);

	/**
	 * @brief Dtor.
	 */
	virtual ~TerrainManager();

	/**
	 * @brief At each frame, we check for updates shaders and updates the terrain. This is because we want to batch together changes.
	 *
	 * @param evt
	 * @return
	 */
	virtual bool frameEnded(const Ogre::FrameEvent & evt);

	/**
	 * @copydoc ConsoleObject::runCommand
	 */
	virtual	void runCommand(const std::string& command, const std::string& args);


	/**
	 * @brief Gets the adapter used to bind this manager to a scene manager.
	 * @return The adapter in use, or null if there is no one registered yet.
	 */
	ISceneManagerAdapter* getAdapter() const;

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
	 * @brief Console command for updating all terrain shadow maps.
	 */
	const ConsoleCommandWrapper UpdateShadows;

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
	bool getHeight(const TerrainPosition& atPosition, float& height) const;

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
	void getBasePoints(sigc::slot<void, std::map<int, std::map<int, Mercator::BasePoint> >& >& asyncCallback);

	/**
	 * @brief Emitted when a terrain page has had its geometry updated.
	 *
	 * This often signals that a page is to be loaded and rendered.
	 * @warning You must not do any rendering directly in the methods listening for this event, since that can lead to strange behaviour in the scene manager. The reason is that this event is emitted while already in the rendering loop, and entering the render loop again at that point will lead to some very inconsistent states.
	 */
	sigc::signal<void, TerrainPage&> EventTerrainPageGeometryUpdated;

protected:

	/**
	 * @brief Compiler technique provider.
	 */
	Techniques::CompilerTechniqueProvider* mCompilerTechniqueProvider;

	/**
	 * @brief The terrain handler, which handles the underlying Mercator terrain.
	 */
	TerrainHandler* mHandler;

	/**
	 * @brief True if foliage should be shown.
	 */
	bool mIsFoliageShown;

	/**
	 * @brief The adapter acts as a bridge between the manager and the actual scene manager, allowing a certain degree of decoupling.
	 */
	ISceneManagerAdapter* mSceneManagerAdapter;

	unsigned int mFoliageBatchSize;

	Foliage::Vegetation* mVegetation;

	/**
	 * @brief The scene in which the terrain manager operates.
	 */
	Scene& mScene;

	bool mIsInitialized;

	void initializeTerrain();

	void loadTerrainOptions();

	/**
	 * @brief Iterates through all TerrainPages and shows or hides the foliage.
	 */
	void updateFoliageVisibility();

	void config_Foliage(const std::string& section, const std::string& key, varconf::Variable& variable);

	void terrainHandler_AfterTerrainUpdate(const std::vector<WFMath::AxisBox<2> >& areas, const std::set<TerrainPage*>& pages);

	void terrainHandler_ShaderCreated(const TerrainShader& shader);

	void terrainHandler_WorldSizeChanged();

	/**
	 * @brief Listen to graphic level updates and ask the pages to regenerate their materials (since they will use different materials depending on the level chosen).
	 *
	 * @param shaderManager The shader manager, which contains information on the graphics level set.
	 */
	void shaderManager_LevelChanged(ShaderManager* shaderManager);

	/**
	 * @brief Called at the end of an Eris poll cycle.
	 */
	void application_EndErisPoll(float);

};

inline unsigned int TerrainManager::getFoliageBatchSize() const
{
	return mFoliageBatchSize;
}

}
}
}


#endif // EMBER_OGREVIEW_TERRAINMANAGER_H
