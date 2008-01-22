//
// C++ Interface: ISceneManagerAdapter
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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

class ISceneManagerAdapter
{
public:

	virtual ~ISceneManagerAdapter() {}

	virtual int getPageSize() = 0;
	virtual Ogre::Real getHeightAt(const Ogre::Real x, const Ogre::Real z) = 0;
	
	virtual void setWorldPagesDimensions(int numberOfPagesHeight, int numberOfPagesWidth, int heightOffsetInPages, int widthOffsetInPages) = 0;

	virtual void setCamera(Ogre::Camera* camera) = 0;
	virtual void setResourceGroupName(const std::string& groupName) = 0;
	virtual void loadOptions(const std::string& filePath) = 0;
	virtual void resize(Ogre::AxisAlignedBox newSize, int levels) = 0;
	virtual void loadScene() = 0;
	
	virtual void setOption(const std::string& strKey, const void* pValue) = 0;
	virtual void getOption(const std::string& strKey, void* pDestValue) = 0;
	
	virtual Ogre::SceneManager* getSceneManager() const = 0;
	
	virtual void reloadAllPages() = 0;
	virtual void reloadPage(unsigned int x, unsigned int z) = 0;
};
}
}
