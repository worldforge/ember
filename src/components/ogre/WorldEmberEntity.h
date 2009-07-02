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

#ifndef WORLDEMBER_ENTITY_H
#define WORLDEMBER_ENTITY_H

namespace Eris
{
class Timeout;
class View;
}

namespace EmberOgre {

namespace Environment {
class Environment;
class Foliage;
}

namespace Terrain {
class TerrainGenerator;
class TerrainArea;
}
class DelayedFoliageInitializer;

/**

@brief Responsible for parsing terrain information from the Atlas data.

@author Erik Hjortsberg <erik@worldforge.org>

*/
class TerrainParser
{
public:
	TerrainParser(Terrain::TerrainGenerator& terrainGenerator);

	/**
	 * @brief Extracts terrain updates from the element and updates the terrain.
	 * @param terrain The element containing the terrain data.
	 */
	void updateTerrain(const Atlas::Message::Element& terrain);

	/**
	 * @brief Parses surface data and creates appropriate Mercator::Shader instances.
	 * @param surfaces The element containing the surface data.
	 */
	void createShaders(const Atlas::Message::Element& surfaces);


	/**
	 * @brief A fall back method which will create default shaders. This is used only if no valid surface information could be found (for example if a very old version of the server is used).
	 */
	void createDefaultShaders();

private:

	/**
	 * @brief The terrain generator instance used in the system.
	 */
	Terrain::TerrainGenerator& mTerrainGenerator;
};

class EmberEntity;

/**
@brief Represents the root entity, the "world" object to which all other entities belong to.

In any world there should only be a single instance of this. Usually the world entity has id 0, but that's not for certain.

This should also be the very first entity created.
An instance of this also owns a couple of objects responsible for the environment. These are:
* The Environment, responsible for providing sun and moon lightning, water and weather effects and other environment related features such as fog.
* The Foliage, responsible for providing foliage to the world.

Since the world entity has access to the terrain data this instance is also responsible for adjusting contained entities so that they are positioned on the terrain.

@author Erik Hjortsberg <erik@worldforge.org>
*/
class WorldEmberEntity : public EmberEntity {
public:

	/**
	 * @brief Expresses a position on a circular sphere (like the earth or any other planet), as defined by degrees of longitude and latitude.
	 * @author Erik Hjortsberg <erik@worldforge.org>
	 */
	struct WorldPosition
	{
		/**
		 * @brief The longitude, in degrees.
		 */
		float LongitudeDegrees;

		/**
		 * @brief The longitude, in degrees.
		 */
		float LatitudeDegrees;
	};


	/**
	 * @brief Ctor.
	 * @param id The id of the entity.
	 * @param ty The entity type.
	 * @param vw The main Eris::View.
	 * @param sceneManager The main Ogre scene manager.
	 * @param terrainGenerator The terrain generator instance.
	 */
	WorldEmberEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw, Ogre::SceneManager* sceneManager);

	/**
	 * @brief Dtor
	 */
	virtual ~WorldEmberEntity();

	/**
	 * @brief This implementation will adjust the contained entity so that it in normal operation snaps to the terrain.
	 * @param entity
	 * @param position
	 */
	virtual void adjustPositionForContainedNode(const EmberEntity& entity, const Ogre::Vector3& position);

	/**
	 * @brief Gets the main Environment object of the world.
	 * @return An environment object, or null if none has been created yet.
	 */
	Environment::Environment* getEnvironment() const;

	/**
	 * @brief Gets the main Foliage object of the world. Note that the initialization of the foliage might be delayed.
	 * @return A foliage object, or null if none created.
	 */
	Environment::Foliage* getFoliage() const;

	/**
	 * @brief Gets the position of the world, as expressed in degrees of longitude and latitude.
	 * @return The position of the world.
	 */
	const WorldPosition& getWorldPosition() const;

	/**
	 * @brief Emitted when the foliage has been created.
	 */
	sigc::signal<void> EventFoliageCreated;

	/**
	 * @brief Emitted when the environment has been created.
	 */
	sigc::signal<void> EventEnvironmentCreated;



protected:
	virtual const Ogre::Vector3& getOffsetForContainedNode(const Ogre::Vector3& position, const EmberEntity& entity);
	Terrain::TerrainGenerator* mTerrainGenerator;

    virtual void init(const Atlas::Objects::Entity::RootEntity &ge, bool fromCreateOp);

	virtual void onMoved();

	virtual void onAttrChanged(const std::string& str, const Atlas::Message::Element& v);

// 	virtual void onTalk(const Atlas::Objects::Operation::RootOperation& talk);
//	virtual void setContainer(Entity *pr);
	virtual void onVisibilityChanged(bool vis);
	virtual void onLocationChanged(Eris::Entity *oldLocation);


	/**
	 * @brief Adds an area to the world. This method will in turn interface with the TerrainGenerator.
	 * @param area
	 */
	void addArea(Terrain::TerrainArea* area);

	/**
	 * @brief Adds a terrain modifier to the world by making a call to mTerrainGenerator
	 * @param mod
	 */
	void addTerrainMod(Terrain::TerrainMod* mod);

	/**
	 * @brief The foliage system which provides different foliage functions.
	 */
	Environment::Foliage* mFoliage;

	/**
	 * @brief The main environment object. There should only be one in the system, and it's kept here.
	 */
	Environment::Environment* mEnvironment;

	/**
	 * @brief Parses terrain information from the Atlas data sent from the server.
	 */
	std::auto_ptr<TerrainParser> mTerrainParser;

	/**
	 * @brief Takes care of delaying the initialization of the foliage.
	 */
	std::auto_ptr<DelayedFoliageInitializer> mFoliageInitializer;

	/**
	 * @brief True if the world has been intialized.
	 * This defaults to false, and is set to true when all dependent objects have been created. It's mainly used for preventing duplicate initializations.
	 */
	bool mHasBeenInitialized;

	/**
	 * @brief The position of the world on a spherical (planetoid) body.
	 */
	WorldPosition mWorldPosition;

};

inline const WorldEmberEntity::WorldPosition& WorldEmberEntity::getWorldPosition() const
{
	return mWorldPosition;
}


/**
@brief Allows for a delayed initialization of the foliage.
The initialization will occrur when either the sight queue is empty, or a certain time has elapsed.
The main reason for doing this is that whenever a new area is added to the world, the foliage is invalidated and reloaded.
As a result when the user first enters the world and is getting sent all the surrounding entities, there's a great chance that some of these entities will be areas. If the foliage then already has been initialized it will lead to the foliage being reloaded a couple of time.
By delaying the loading of the foliage we can avoid this.

@author Erik Hjortsberg <erik@worldforge.org>

*/
class DelayedFoliageInitializer
{
public:
	/**
	 * @brief Ctor.
	 * @param foliage The foliage object.
	 * @param view The Eris::View object of the world. This will be used for querying about the size of the Sight queue.
	 * @param intervalMs In milliseconds how often to check if the queue is empty or time has elapsed. Defaults to 1 second.
	 * @param maxTimeMs In missiseconds the max time to wait until we initialize the foliage anyway.
	 */
	DelayedFoliageInitializer(Environment::Foliage& foliage, Eris::View& view, unsigned int intervalMs = 1000, unsigned int maxTimeMs = 15000);

	/**
	 * @brief Dtor.
	 */
	virtual ~DelayedFoliageInitializer();

protected:
	Environment::Foliage& mFoliage;
	Eris::View& mView;
	unsigned int mIntervalMs;
	unsigned int mMaxTimeMs;

	std::auto_ptr<Eris::Timeout> mTimeout;
	unsigned int mTotalElapsedTime;

	/**
	 * @brief Called when the time out has expired. We'll check for if either the set max time has elapsed, or if there's no more entities in the sight queue, and if so initialize the foliage. If not we'll just extend the waiting time.
	 */
	void timout_Expired();

};

}

#endif // WORLDEMBER_ENTITY_H
