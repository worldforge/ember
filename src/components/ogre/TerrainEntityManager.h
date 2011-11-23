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
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef TERRAINENTITYMANAGER_H_
#define TERRAINENTITYMANAGER_H_
#include <map>
#include <Atlas/Message/Element.h>

#include <sigc++/trackable.h>

namespace Eris
{
class Entity;
class View;
}

namespace Ember
{
namespace OgreView
{
class EmberEntity;
namespace Terrain
{
class TerrainHandler;
class TerrainMod;
class TerrainArea;
}

class TerrainEntityManager: public sigc::trackable
{
public:
	TerrainEntityManager(Eris::View& view, Terrain::TerrainHandler& terrainHandler);
	virtual ~TerrainEntityManager();

	void registerEntity(EmberEntity* entity);

private:

	typedef std::map<EmberEntity*, Terrain::TerrainMod*> ModStore;
	typedef std::map<EmberEntity*, Terrain::TerrainArea*> AreaStore;

	Terrain::TerrainHandler& mTerrainHandler;

	ModStore mTerrainMods;
	AreaStore mAreas;

	void entitySeen(Eris::Entity* entity);
	void entityTerrainAttrChanged(const Atlas::Message::Element& value, EmberEntity* entity);
	void entityTerrainModAttrChanged(const Atlas::Message::Element& value, EmberEntity* entity);
	void entityAreaAttrChanged(const Atlas::Message::Element& value, EmberEntity* entity);

	void entityBeingDeletedTerrainMod(EmberEntity* entity);
	void entityBeingDeletedArea(EmberEntity* entity);
};

}
}
#endif /* TERRAINENTITYMANAGER_H_ */
