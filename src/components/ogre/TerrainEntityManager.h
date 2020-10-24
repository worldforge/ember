/*
 Copyright (C) 2011 Erik Ogenvik

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

#ifndef TERRAINENTITYMANAGER_H_
#define TERRAINENTITYMANAGER_H_
#include <Atlas/Message/Element.h>

#include <sigc++/trackable.h>
#include <sigc++/connection.h>

#include <unordered_map>
#include <functional>
#include <memory>

namespace Ogre
{
class SceneManager;
}

namespace Eris
{
class Entity;
class View;
}

namespace Ember
{
class EmberEntity;
namespace OgreView
{
namespace Terrain
{
class TerrainHandler;
class TerrainMod;
class TerrainArea;
struct TerrainDefPoint;
}

struct TerrainEffectorListener;

class TerrainEntityManager: public virtual sigc::trackable {
public:
	TerrainEntityManager(Eris::View& view, Terrain::TerrainHandler& terrainHandler, Ogre::SceneManager& sceneManager);

	virtual ~TerrainEntityManager();

private:

	typedef std::unordered_map<EmberEntity*, std::pair<std::unique_ptr<Terrain::TerrainMod>, std::unique_ptr<TerrainEffectorListener>>> ModStore;
	typedef std::unordered_map<EmberEntity*, std::pair<std::unique_ptr<Terrain::TerrainArea>, std::unique_ptr<TerrainEffectorListener>>> AreaStore;

	std::function<void(EmberEntity&, const Atlas::Message::Element&)> mTerrainListener;
	std::function<void(EmberEntity&, const Atlas::Message::Element&)> mTerrainPointsListener;
	std::function<void(EmberEntity&, const Atlas::Message::Element&)> mTerrainAreaListener;
	std::function<void(EmberEntity&, const Atlas::Message::Element&)> mTerrainModListener;

	Eris::View& mView;
	Terrain::TerrainHandler& mTerrainHandler;
	Ogre::SceneManager& mSceneManager;

	ModStore mTerrainMods;
	AreaStore mAreas;

	sigc::connection mTerrainEntityDeleteConnection;
	sigc::connection mTerrainEntityVisibilityConnection;

	void entityTerrainAttrChanged(EmberEntity& entity, const Atlas::Message::Element& value);

	void entityTerrainPointsAttrChanged(EmberEntity& entity, const Atlas::Message::Element& value);

	void entityTerrainModAttrChanged(EmberEntity& entity, const Atlas::Message::Element& value);

	void entityAreaAttrChanged(EmberEntity& entity, const Atlas::Message::Element& value);

	void entityBeingDeleted(EmberEntity& entity);

	void entityMoved(EmberEntity& entity, Terrain::TerrainArea& terrainArea);

	void entityModeChanged(EmberEntity& entity, Terrain::TerrainArea& terrainArea);

	void entityModeChanged(EmberEntity& entity, Terrain::TerrainMod& terrainMod);

	void parseTerrainAttribute(EmberEntity& entity, const Atlas::Message::Element& value);

};
}
}
#endif /* TERRAINENTITYMANAGER_H_ */
