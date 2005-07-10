//
// C++ Implementation: ModelDefinition
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2004
// Copyright (c) 2005 The Cataclysmos Team
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


#include "ModelDefinition.h"
#include "Model.h"
#include "SubModel.h"
#include "SubModelPart.h"

namespace EmberOgre {

ModelDefinition::ModelDefinition(Ogre::ResourceManager* creator, const Ogre::String& name, Ogre::ResourceHandle handle,
    const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader) 
    : Resource(creator, name, handle, group, isManual, loader), mScale(0), mUseScaleOf(MODEL_ALL), mRotation(0), mIsValid(false)
{
    if (createParamDictionary("ModelDefinition"))
    {
        // no custom params
    }
}

ModelDefinition::~ModelDefinition()
{
    // have to call this here reather than in Resource destructor
    // since calling virtual methods in base destructors causes crash
    unload(); 
}

void ModelDefinition::loadImpl(void)
{
//     XMLModelDefinitionSerializer serializer;
// 
// 	Ogre::String filename = mName + ".modeldef.xml";
// 
// 	S_LOG_INFO( "ModelDefinition: Loading " + filename );
// 
// 	Ogre::DataStreamPtr stream = 
// 		Ogre::ResourceGroupManager::getSingleton().openResource(filename, mGroup);
// 
//     serializer.importModelDefinition(stream, this);
// 
// 	mIsValid = true;
// 	mIsLoaded = true;

}

void ModelDefinition::unloadImpl(void)
{
}

bool ModelDefinition::isValid(void)
{
	return mIsValid;
}


};
