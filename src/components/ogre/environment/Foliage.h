//
// C++ Interface: Foliage
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2004
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
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

namespace Ember {
namespace OgreView {

namespace Terrain
{
struct TerrainFoliageDefinition;
struct TerrainLayerDefinition;
class TerrainManager;
}

namespace Environment {

class FoliageBase;

/**
@author Erik Ogenvik
*/
class Foliage : public Singleton<Foliage>, public Ogre::FrameListener, public ConsoleObject
{

public:

	explicit Foliage(Terrain::TerrainManager& terrainManager);

	~Foliage() override;
	
	void initialize();

	bool frameStarted(const Ogre::FrameEvent& evt) override;

	void reloadAtPosition(const WFMath::Point<2>& worldPosition);

	/**
	 * @copydoc ConsoleObject::runCommand
	 */
	void runCommand(const std::string &command, const std::string &args) override;

	const ConsoleCommandWrapper ReloadFoliage;
	
	/**
	 * @brief Sets the density of all foliage in a factor of default density.
	 * eg. passing 0.1 will make all foliage density = (the default defined layer density) * 0.1;
	 * @param newDensity the new density percentage in float, where 1 implies normal or full density and 0 implies no density or no foliage. 
	 */
	void setDensity(float newDensity);
	
	/**
	 * @brief Sets the max far distance of all foliage in a factor of default distance.
	 * eg. passing 0.1 will make the furthest distance foliage appears at = default far distance * 0.1;
	 * @param newFarDistance the new far distance percentage, where 1 implies normal or max far distance and 0 implies minimum far distance;
	 */
	void setFarDistance(float newFarDistance);


protected:

	Terrain::TerrainManager& mTerrainManager;

	std::vector<std::unique_ptr<FoliageBase>> mFoliages;

};

}

}

}

#endif
