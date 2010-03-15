//
// C++ Interface: Foliage
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2004
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
#ifndef EMBEROGREFOLIAGE_H
#define EMBEROGREFOLIAGE_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include "framework/Singleton.h"
#include "framework/ConsoleObject.h"

#include <OgreFrameListener.h>

namespace WFMath
{
	template<int> class Point;
}

namespace Forests {
	class PagedGeometry;
}

namespace EmberOgre {

namespace Terrain
{
class TerrainFoliageDefinition;
class TerrainLayerDefinition;
class TerrainManager;
}

namespace Environment {

class FoliageImpl;
class FoliageBase;

/**
@author Erik Hjortsberg
*/
class Foliage : public Ember::Singleton<Foliage>, public Ogre::FrameListener, public Ember::ConsoleObject
{

public:

	typedef std::map<const std::string, Ogre::Entity* > EntityStore;
	typedef std::vector<FoliageBase*> FoliageStore;

	Foliage(Terrain::TerrainManager& terrainManager);

	~Foliage();

	void initialize();

	virtual bool frameStarted(const Ogre::FrameEvent& evt);

	void reloadAtPosition(const WFMath::Point<2>& worldPosition);

	/**
	 * @copydoc ConsoleObject::runCommand
	 */
	virtual void runCommand(const std::string &command, const std::string &args);

	const Ember::ConsoleCommandWrapper ReloadFoliage;


protected:

	Terrain::TerrainManager& mTerrainManager;

	EntityStore mEntities;

	FoliageImpl* mImpl;

	FoliageStore mFoliages;


	void createGrassMesh();
};

}

}

#endif
