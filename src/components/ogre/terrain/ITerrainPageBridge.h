//
// C++ Interface: ITerrainPageBridge
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2008
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
#ifndef EMBEROGREITERRAINPAGEBRIDGE_H
#define EMBEROGREITERRAINPAGEBRIDGE_H

namespace EmberOgre {
namespace Terrain {

class TerrainPage;

/**

@brief Implementation of this interface acts as bridges between an Ember TerrainPage and an Ogre terrain page.

Because we don't want to make Ember and the scene manager used for terrain rendering too tightly coupled we provide this interface instead of letting TerrainPage know of any scene manager classes.
It's up to the scene manager speficic adapter code to provide concrete instances of this interface which knows how to react mainly to calls to updateTerrain(), and update the Ogre representation accordinly.

@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class ITerrainPageBridge
{
friend class TerrainPage;
public:
	/**
	 *    @brief Ctor.
	 */
	ITerrainPageBridge() : mTerrainPage(0) {}
	/**
	 *    @brief Ctor.
	 */
	virtual ~ITerrainPageBridge() {}
	
	
	/**
	 *    @brief Updates the Ogre mesh after changes to the underlying heightdata.
	 *
	 * Call this when the heightdata has changed and you want the Ogre representation to be updated to reflect this.
	 */
	virtual void updateTerrain() = 0;
	
	/**
	 *    @brief Notifies class in the ogre side about the page being ready (after being created or so).
	 */
	virtual void terrainPageReady() = 0;
	
	/**
	 * @brief Accessor to the terrain page this bridge is bound to.
	 * @return A pointer to the terrain page, or null if no page yet has been bound.
	 */
	TerrainPage* getTerrainPage() const;
	
protected:
	
	/**
	 *    @brief Binds the bridge to a TerrainPage.
	 * @param terrainPage 
	 */
	void bindToTerrainPage(TerrainPage* terrainPage);
	/**
	 *    @brief Unbinds from a TerrainPage.
	 * Call this when the TerrainPage to which this bridge is connected to is destroyed. If not, you run the risk of segfaults since the bridge will still keep a pointer to the now deleted TerrainPage.
	 */
	void unbindFromTerrainPage();
	
	/**
	@brief The TerrainPage instance to which this bridge is connected to.
	*/
	TerrainPage* mTerrainPage;
};


inline void ITerrainPageBridge::bindToTerrainPage(TerrainPage* terrainPage)
{
	mTerrainPage = terrainPage;
}

inline void ITerrainPageBridge::unbindFromTerrainPage()
{
	mTerrainPage = 0;
}
	
inline TerrainPage* ITerrainPageBridge::getTerrainPage() const
{
	return mTerrainPage;
}


}
}
#endif
