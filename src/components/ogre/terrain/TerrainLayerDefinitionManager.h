//
// C++ Interface: TerrainLayerDefinitionManager
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2007
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
#ifndef EMBEROGRE_TERRAINTERRAINLAYERDEFINITIONMANAGER_H
#define EMBEROGRE_TERRAINTERRAINLAYERDEFINITIONMANAGER_H

#include "components/ogre/EmberOgrePrerequisites.h"

#include <OgreResourceManager.h>
#include "framework/Singleton.h"

namespace EmberOgre {

namespace Terrain {

class TerrainLayerDefinition;
/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class TerrainLayerDefinitionManager : public Ogre::ResourceManager, public Ember::Singleton<TerrainLayerDefinitionManager>
{
public:
	typedef std::vector<TerrainLayerDefinition*> DefinitionStore;
    TerrainLayerDefinitionManager();

    virtual ~TerrainLayerDefinitionManager();

	virtual void parseScript(Ogre::DataStreamPtr &stream, const Ogre::String &groupName);

    /**
    Adds a definition to the manager. This definition will be deleted by the manager upon destruction.
    @param definition A valid definition.
    */
    void addDefinition(TerrainLayerDefinition* definition);
    
    const DefinitionStore& getDefinitions() const;
    
    TerrainLayerDefinition* getDefinitionForArea(int areaIndex);
    TerrainLayerDefinition* getDefinitionForShader(const std::string& shaderType);
    
protected:
	DefinitionStore mDefinitions;
	
	Ogre::Resource* createImpl(const Ogre::String& name, Ogre::ResourceHandle handle, 
    const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader, 
    const Ogre::NameValuePairList* createParams);
};

}

}

#endif
