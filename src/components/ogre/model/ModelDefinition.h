//
// C++ Interface: ModelDefinition
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
#ifndef EMBEROGREMODELDEFINITION_H
#define EMBEROGREMODELDEFINITION_H

//we must include xerces stuff before ogre stuff, because else we'll get errors when compiling in debug mode
//this seems to be because both uses their own internal memory handlers
#include <xercesc/util/XMemory.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
// indicate using Xerces-C++ namespace in general
XERCES_CPP_NAMESPACE_USE

// // need to properly scope any forward declarations
// XERCES_CPP_NAMESPACE_BEGIN
//   class AttributeList;
// XERCES_CPP_NAMESPACE_END

#include "components/ogre/EmberOgrePrerequisites.h"

namespace EmberOgre {

class ModelDefinition;
class Model;

     class ModelDefinitionPtr : public Ogre::SharedPtr<ModelDefinition> 
     {
     public:
         ModelDefinitionPtr() : Ogre::SharedPtr<ModelDefinition>() {}
         explicit ModelDefinitionPtr(ModelDefinition* rep) : Ogre::SharedPtr<ModelDefinition>(rep) {}
         ModelDefinitionPtr(const ModelDefinitionPtr& r) : Ogre::SharedPtr<ModelDefinition>(r) {} 
         ModelDefinitionPtr(const Ogre::ResourcePtr& r);
         ModelDefinitionPtr& operator=(const Ogre::ResourcePtr& r);
     protected:
         void destroy(void);
     };

/**
@author Erik Hjortsberg
*/
class ModelDefinition : public Ogre::Resource {
	
public:

 
	ModelDefinition (const Ogre::String& path, Ogre::ResourceManager *creator, const Ogre::String &name, Ogre::ResourceHandle handle, const Ogre::String &group, bool isManual=false, Ogre::ManualResourceLoader *loader=0);

    ~ModelDefinition();
	
        /** Generic load - called by ModelDefinitionManager.
        */
        virtual void loadImpl(void);

        /** Generic unload - called by ModelDefinitionManager.
        */
        virtual void unloadImpl(void);
		
		virtual size_t calculateSize (void) const; 

		
	Model* createModel(Ogre::String name, Ogre::SceneManager* sceneManager);
    bool isValid(void);
		
		
protected:
	struct AnimationDefinition
	{
		std::string Name;
		Ogre::Real Weight;
	};
	
	struct ActionDefinition
	{
		std::string Name;
		std::vector<AnimationDefinition> Animations;
	};
	
	
	struct SubEntityDefinition
	{
		std::string SubEntity;
		std::string Material;
		
	};	
	struct PartDefinition
	{
		std::string Name;
		bool Show;
		std::vector<SubEntityDefinition> SubEntities;
	};
	struct SubModelDefinition
	{
		std::string Mesh;
		std::vector<PartDefinition> Parts;
	};
	
	void readAnimations(xercesc::DOMElement* animationsNode);
	bool createFromXML(std::string path);

	
	
	std::vector<SubModelDefinition> mSubModels;
	std::vector<ActionDefinition> mActions;
	
	unsigned short mUseScaleOf;
	Ogre::Real mScale;
	Ogre::Real mRotation;
	const Ogre::String mPath;
	
	bool mIsValid;
	
};

//typedef Ogre::SharedPtr<ModelDefinition> ModelDefinitionPtr;


};

#endif
