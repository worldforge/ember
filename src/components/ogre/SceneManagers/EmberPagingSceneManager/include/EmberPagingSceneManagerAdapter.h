//
// C++ Interface: EmberPagingSceneManagerAdapter
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

#include "../../../EmberOgrePrerequisites.h"

#include "../../../terrain/ISceneManagerAdapter.h"

namespace Ogre
{
class PagingLandScapeOptions;
class SceneManager;
}

namespace EmberOgre {

class EmberPagingSceneManager;

class EmberPagingSceneManagerAdapter : public ISceneManagerAdapter
{

public:

	EmberPagingSceneManagerAdapter(EmberPagingSceneManager* scenemanager);
	
	
	virtual int getPageSize();
	
	virtual void setWorldPagesDimensions(int numberOfPagesHeight, int numberOfPagesWidth);
	
	virtual void setCamera(Ogre::Camera* camera);
	virtual void setResourceGroupName(const std::string& groupName);
	virtual void loadOptions(const std::string& filePath);
	virtual void resize(Ogre::AxisAlignedBox newSize, int levels);
	virtual void loadScene();

	virtual void setOption(const std::string& strKey, const void* pValue);
	virtual void getOption(const std::string& strKey, void* pDestValue);

	virtual Ogre::SceneManager* getSceneManager() const;
	virtual void reloadAllPages();
	virtual void reloadPage(unsigned int x, unsigned int z);

private:
	EmberPagingSceneManager* mSceneManager;
	Ogre::PagingLandScapeOptions* getOptions();

};


}
