//
// C++ Interface: ModelManager
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
#ifndef EMBEROGREMODELMANAGER_H
#define EMBEROGREMODELMANAGER_H

#include <OgreResourceManager.h>
#include "framework/Singleton.h"

namespace EmberOgre {

class ModelDefinition;
class ModelDefinitionPtr;
/**
@author Erik Hjortsberg
*/
class ModelDefinitionManager: public Ogre::ResourceManager, public Ember::Singleton<ModelDefinitionManager>

{
public:
    ModelDefinitionManager();

    //~ModelDefinitionManager();
	
        ModelDefinitionPtr load( const Ogre::String& filename, const Ogre::String &group);

        /** Creates a Skeleton resource.
            @note
                Mainly used internally.
        */
//        Ogre::Resource* create( const Ogre::String& name);

        /** Override standard Singleton retrieval.
        @remarks
        Why do we do this? Well, it's because the Singleton
        implementation is in a .h file, which means it gets compiled
        into anybody who includes it. This is needed for the
        Singleton template to work, but we actually only want it
        compiled into the implementation of the class based on the
        Singleton, not all of them. If we don't change this, we get
        link errors when trying to use the Singleton-based class from
        an outside dll.
        @par
        This method just delegates to the template version anyway,
        but the implementation stays in this single compilation unit,
        preventing link errors.
        */
/*        static ModelDefinitionManager& getSingleton(void);*/
        /** Override standard Singleton retrieval.
        @remarks
        Why do we do this? Well, it's because the Singleton
        implementation is in a .h file, which means it gets compiled
        into anybody who includes it. This is needed for the
        Singleton template to work, but we actually only want it
        compiled into the implementation of the class based on the
        Singleton, not all of them. If we don't change this, we get
        link errors when trying to use the Singleton-based class from
        an outside dll.
        @par
        This method just delegates to the template version anyway,
        but the implementation stays in this single compilation unit,
        preventing link errors.
        */
//         static ModelDefinitionManager* getSingletonPtr(void);

		Ogre::Resource* createImpl(const Ogre::String& name, Ogre::ResourceHandle handle, 
         const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader, 
         const Ogre::NameValuePairList* createParams);

};

};

#endif
