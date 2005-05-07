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
#include "components/ogre/EmberOgrePrerequisites.h"

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
/*    Ogre::Resource* ModelDefinitionManager::create( const Ogre::String& name, const Ogre::String &group)
    {
		std::string path;
		Ogre::ResourceManager::ResourceMap::const_iterator it;
		if( ( it = mResources.find( name ) ) != mResources.end() )
		{
			path = std::string(it->second->getName() + "/" + name);
		} 
		
		return new ModelDefinition(name, path);
    }*/
	
    //-----------------------------------------------------------------------
    
	
	ModelDefinitionPtr ModelDefinitionManager::load( const Ogre::String& filename, const Ogre::String &group)
    {
        ModelDefinitionPtr pModelDefinition = getByName(filename);
        if (pModelDefinition.isNull())
        {
            pModelDefinition = create(filename, group);
			pModelDefinition->load();
            //ResourceManager::load(pModelDefinition, group, priority);
        }
        return pModelDefinition;

    }
	
	     Ogre::Resource* ModelDefinitionManager::createImpl(const Ogre::String& name, Ogre::ResourceHandle handle, 
         const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader, 
         const Ogre::NameValuePairList* createParams)
     {
         // no use for createParams here
		std::string path;
		Ogre::FileInfoListPtr pList = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo (group, name);
		
		
		Ogre::FileInfoList::const_iterator it;
		if( (it = pList->begin() ) != pList->end() )
		{
			path = std::string(it->archive->getName() + "/" + name);
		} 
		 //TODO: implement this in the right way
         return new ModelDefinition(path, this, name, handle, group, isManual, loader);
     }



}
