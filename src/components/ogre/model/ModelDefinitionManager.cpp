//
// C++ Implementation: ModelDefinitionManager
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2004
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

//perhaps this will make it build in debug mode without getting memory manager compile errors...
#include <OgreNoMemoryMacros.h>

#include "ModelDefinitionManager.h"
#include "ModelDefinition.h"

namespace EmberOgre
{
    //-----------------------------------------------------------------------
    template<> ModelDefinitionManager* Ember::Singleton<ModelDefinitionManager>::ms_Singleton = 0;
/*    ModelDefinitionManager* ModelDefinitionManager::getSingletonPtr(void)
    {
        return ms_Singleton;
    }
    ModelDefinitionManager& ModelDefinitionManager::getSingleton(void)
    {  
        assert( ms_Singleton );  return ( *ms_Singleton );  
    }*/
    //-----------------------------------------------------------------------
    ModelDefinitionManager::ModelDefinitionManager()
    {


    }
    //-----------------------------------------------------------------------
    Ogre::Resource* ModelDefinitionManager::create( const Ogre::String& name)
    {
        return new ModelDefinition(name);
    }
	
    //-----------------------------------------------------------------------
    ModelDefinition* ModelDefinitionManager::load( const Ogre::String& filename, int priority)
    {
        ModelDefinition* pModelDefinition = (ModelDefinition*)(getByName(filename));
        if (!pModelDefinition)
        {
            pModelDefinition = (ModelDefinition*)create(filename);
            ResourceManager::load(pModelDefinition, priority);
        }
        return pModelDefinition;

    }



}
