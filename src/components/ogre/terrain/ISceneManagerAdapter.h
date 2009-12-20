//
// C++ Interface: ISceneManagerAdapter
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
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

namespace Ogre
{
class SceneManager;
class Camera;
}

namespace EmberOgre {
namespace Terrain {

class TerrainManager;

/**
Acts as a bridge between the Mercator terrain system and the Ogre terrain rendering engine. We don't want to directly interface with the terrain rendering system since we want to have the ability to replace or alter that, thus we use an instance of this whenever we need to interact directly to it.

Thus, if you have a feature which requires some kind of interfacing with the terrain rendering system that's not currently available, this is where you put it (and then implement it in the concrete class which implements this interface).

@author Erik Hjortsberg
*/
class ISceneManagerAdapter
{
public:

	virtual ~ISceneManagerAdapter() {}

	/**
	 *    Gets the size of the width of one page, in indices (so it's often a power of two + 1)
	 * @return 
	 */
	virtual int getPageSize() = 0;
	
	
	/**
	 *    Returns the height at the given position.
	 * @param x The x position, in world coords.
	 * @param z The y position, in world coords.
	 * @return The height at the specified position.
	 */
	virtual Ogre::Real getHeightAt(const Ogre::Real x, const Ogre::Real z) = 0;
	
	/**
	 *    Sets the new dimensions of the world, in pages. Since the underlying terrain engine often works with pages, it needs to know exactly how many to load.
	 * @param numberOfPagesHeight The total number of pages in Ogre x space.
	 * @param numberOfPagesWidth The total number of pages in Ogre z space.
	 * @param heightOffsetInPages How much the terrain should be offset in x space in order to get the terrain point 0 to align with the world 0.
	 * @param widthOffsetInPages How much the terrain should be offset in z space in order to get the terrain point 0 to align with the world 0.
	 */
	virtual void setWorldPagesDimensions(int numberOfPagesWidth, int numberOfPagesHeight, int widthOffsetInPages, int heightOffsetInPages) = 0;

	/**
	 *    Sets the main camera used.
	 * The terrain system needs to know where the main camera is in order to properly calculate proper LOD levels and similiar.
	 * @param camera The main camera.
	 */
	virtual void setCamera(Ogre::Camera* camera) = 0;
	/**
	 *    Sets the name of the resource group from where the terrain engine should load its resources.
	 * @param groupName The name of a resource group.
	 */
	virtual void setResourceGroupName(const std::string& groupName) = 0;
	/**
	 *    Loads terrain engine specific options from a file. This is specific for each terrain engine.
	 * @param filePath The absolute path to the file with the options.
	 */
	virtual void loadOptions(const std::string& filePath) = 0;
	/**
	 *    Sets the new dimensions of the world. Call this whenever the bounds of the world changes. It allows the terrain engine to update it's internal structures for better space partitioning and similiar things.
	 * @param newSize The new size of the world.
	 * @param levels The number of levels for which to generate partitioning blocks. 16 should be a good number.
	 */
	virtual void resize(Ogre::AxisAlignedBox newSize, int levels) = 0;
	/**
	 *    Loads the whole scene. Call this when the terrain has been set up and you want to show it.
	 */
	virtual void loadScene() = 0;
	
	/**
	 *    Sets an arbitrary option.
	 * @param strKey The key of the option to set.
	 * @param pValue The value to set.
	 */
	virtual void setOption(const std::string& strKey, const void* pValue) = 0;
	/**
	 *    Gets an arbitrary option.
	 * @param strKey The key of the option to get.
	 * @param pDestValue The value of the option.
	 */
	virtual void getOption(const std::string& strKey, void* pDestValue) = 0;
	
	/**
	 *    Returns the scene manager instance that handles the terrain. This is most often a specialized subclass of Ogre::SceneManager.
	 * @return 
	 */
	virtual Ogre::SceneManager* getSceneManager() const = 0;
	
	/**
	 *    Reloads all pages. This can be very expensive since it will do a full reload of all the terrain. In most instances you don't want to do this and instead just want to reload a page.
	 * @see reloadPage
	 */
	virtual void reloadAllPages() = 0;
	/**
	 *    Reloads a single page.
	 * @param x The x index of the page.
	 * @param z The z index of the page.
	 */
	virtual void reloadPage(unsigned int x, unsigned int z) = 0;
	
	
	/**
	 *    Force the loading of the first page. Call this just after the terrain has been set up to load the current page the camera is on. 
	 This is to prevent an empty world as the pages are loaded.
	 */
	virtual void loadFirstPage() = 0;

};
}
}
