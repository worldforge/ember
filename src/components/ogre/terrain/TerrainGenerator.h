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
#include <Mercator/Terrain.h>

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
class TerrainLayerDefinition;
class TerrainPageSurfaceLayer;
class ISceneManagerAdapter;



struct TerrainDefPoint
{
	public:
		TerrainDefPoint(float x, float y, float terrainHeight) : position(x,y), height(terrainHeight) {}
	TerrainPosition position;
	float height;
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
	virtual ~TerrainGenerator();

	
	/**
	 *    At each frame, we check for updates shaders and updates the terrain. This is because we want to batch together changes.
	 * @param evt 
	 * @return 
	 */
	virtual bool frameStarted(const Ogre::FrameEvent & evt);
	virtual bool frameEnded(const Ogre::FrameEvent & evt);

	
    void setBasePoint(int x, int y, float z) {mTerrain->setBasePoint(x,y,z);}
	void prepareSegments(long segmentXStart, long segmentZStart, long numberOfSegments, bool alsoPushOntoTerrain);

	/**
	 * Prepares all segments aquired from Mercator. Note that this can be very,
	 * very expensive if there's a lot of terrain defined.
	 */
	void prepareAllSegments();
	
	/**
	 *    Returns the height at the specified position in the world.
	 * @param atPosition 
	 * @return 
	 */
	virtual float getHeight(const TerrainPosition& atPosition) const;
	
	/**
	 *    @brief Updates the terrain with new terrain points.
	 * @param terrainPoints A list of terrain points.
	 * @return True if successful.
	 */
	bool updateTerrain(const TerrainDefPointStore& terrainPoints);

	/**
	 * @brief Return true if there is a valid piece of terrain at the supplied segment indices.
	 * By valid means a populated terrain-
	 * @param pos A position in the world.
	 * @return true if the terrain at the position is valid.
	 */
	bool isValidTerrainAt(const TerrainPosition& pos);
	
	/**
	 *    @brief Provides access to the underlying Mercator::Terrain object.
	 * @return The main terrain object.
	 */
	Mercator::Terrain& getTerrain();
	
	/**
	 *    @brief Gets the max boundaries of the terrain.
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
	 * Gets the size of one page as indices.
	 * @return 
	 */
	int getPageIndexSize() ;
		
	/**
	 *    @brief Adds a new Mercator::Area to the terrain.
	 * @param area 
	 */
	void addArea(TerrainArea* terrainArea);

	
	/**
	 *    Returns a TerrainPage. 
	 *    If createIfMissing is true and there is no yet existing, a new one is created.
	 * @param ogrePosition 
	 * @return 
	 */
	TerrainPage* getTerrainPage(const Ogre::Vector2& ogrePosition, bool createIfMissing = true);
	
	/**
	 *    Gets the page at the specified position in the worl. If no page can be found, a null pointer is returned.
	 * @param worldPosition 
	 * @return 
	 */
	TerrainPage* getTerrainPage(const TerrainPosition& worldPosition);


	
	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command 
	 * @param args 
	 */
	virtual	void runCommand(const std::string &command, const std::string &args);
	
	/**
	 *    Rebuilds the height map, effectively regenerating the terrain
	 */
	void buildHeightmap();
	
	/**
	 *    @brief Accessor for the main terrain info instance.
	 * @return An instance of TerrainInfo containing all relevant terrain info.
	 */
	const TerrainInfo& getTerrainInfo() const;
	
	/**
	Emitted when the size of the world has changed.
	*/
	sigc::signal<void> EventWorldSizeChanged;
	
	
	void registerSceneManagerAdapter(ISceneManagerAdapter* adapter);
	ISceneManagerAdapter* getAdapter() const;
	
	inline const TerrainPagestore& getTerrainPages() const;
	
	/**
	 * Create and registers a new texture shader.
	 * @param textureName 
	 * @param mercatorShader 
	 * @return 
	 */
	TerrainShader* createShader(const TerrainLayerDefinition* layerDef, Mercator::Shader* mercatorShader);
		
	void updateShadows();
	
	const Ember::ConsoleCommandWrapper UpdateShadows;
	
	
	/**
	*    Gets the shadow colour at the supplied position.
	* @param position The position in the world.
	*/
	void getShadowColourAt(const Ogre::Vector2& position, Ogre::uint32& colour);
	void getShadowColourAt(const Ogre::Vector2& position, Ogre::ColourValue& colour);

	/**
	Emitted when a layer is updated.
	The vector parameter is either null if the update can't be constrained to any areas, or an vector of areas if it can.
	*/
	sigc::signal<void, TerrainShader*, std::vector<TerrainArea*>* > EventLayerUpdated;
	
	/**
	Emitted when a new shader is created.
	*/
	sigc::signal<void, TerrainShader*> EventShaderCreated;
	
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

	void markShaderForUpdate(TerrainShader* shader, TerrainArea* terrainArea = 0);
	
	typedef std::set<TerrainShader*> ShaderSet;
	ShaderSet mShadersToUpdate;
	
	typedef std::map<std::string, TerrainPage*> PageStore;
	PageStore mPages;
	
	typedef std::map<TerrainShader*, std::vector<TerrainArea*> > TerrainAreaMap;
	TerrainAreaMap mChangedTerrainAreas;
	
	TerrainPagestore mTerrainPages;
	
	Mercator::Terrain* mTerrain;
	
	Ogre::Real mHeightMax, mHeightMin;
	
	WFMath::Point<3> mCurrentPosition;

	const Mercator::Terrain::Segmentstore* mSegments;
	
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
	a list of the shaders, which will all be used on all Pages
	*/
	std::list<TerrainShader*> mBaseShaders;
	 
	
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
	@brief An adapter class which allows us to access the Ogre scene manager.
	Note that even though this is passed as a parameter in the constructor, this class is then responsible for its destruction.
	*/
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

}
}


#endif // TERRAINGENERATOR_H
