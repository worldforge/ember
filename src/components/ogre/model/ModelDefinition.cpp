//
// C++ Implementation: ModelDefinition
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

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"

#include "ModelDefinition.h"
#include "Model.h"
#include "SubModel.h"
#include "SubModelPart.h"
#include "components/ogre/EmberOgre.h"
#include "components/ogre/EmberSceneManager/include/EmberTerrainSceneManager.h"

namespace EmberOgre {

ModelDefinition::ModelDefinition(const Ogre::String& name, const Ogre::String& path)
: mScale(0)
, mRotation(0)
, mUseScaleOf(0)
, mPath(path)
{
	mName = name;
	mIsValid = false;
}


ModelDefinition::~ModelDefinition()
{
        unload();

		
}

    bool ModelDefinition::isValid(void)
	{
		return mIsValid;
	}

	
    void ModelDefinition::load(void)
	{
		mIsValid = createFromXML(mPath);
		mIsLoaded = true;
	}	
    
	
	void ModelDefinition::unload(void)
    {
	}	
	
	Model* ModelDefinition::createModel(Ogre::String name, Ogre::SceneManager* sceneManager)
	{
		Model* aModel = new Model(sceneManager, name);
		aModel->mScale = mScale;
		aModel->mUseScaleOf = mUseScaleOf;
		aModel->mRotation = mRotation;

		std::vector<std::string> showPartVector;
		
			
		std::vector<SubModelDefinition>::iterator I_subModels = mSubModels.begin();
		std::vector<SubModelDefinition>::iterator I_subModels_end = mSubModels.end();
		std::cout << "Number of submodels: " << mSubModels.size() << "\n";
		
		for (; I_subModels != I_subModels_end; ++I_subModels) {
			std::string entityName = name + "/" + (*I_subModels).Mesh;
			Ogre::Entity* entity = sceneManager->createEntity(entityName, (*I_subModels).Mesh);
			SubModel* submodel = new SubModel(entity);

			Model::SubModelPartMapping* submodelPartMapping = new Model::SubModelPartMapping();
						
			
			std::vector<PartDefinition>::iterator I_parts = (*I_subModels).Parts.begin();
			std::vector<PartDefinition>::iterator I_parts_end = (*I_subModels).Parts.end();
			for (; I_parts != I_parts_end; ++I_parts) {
				Model::StringSet parts;
				if ((*I_parts).SubEntities.size() > 0)
				{
					std::vector<SubEntityDefinition>::iterator I_subEntities = (*I_parts).SubEntities.begin();
					std::vector<SubEntityDefinition>::iterator I_subEntities_end = (*I_parts).SubEntities.end();
					for (; I_subEntities != I_subEntities_end; ++I_subEntities) {
						parts.insert((*I_subEntities).SubEntity);
						if ((*I_subEntities).Material != "") {
							entity->getSubEntity((*I_subEntities).SubEntity)->setMaterialName((*I_subEntities).Material);
						}
					}		
				} 
				Model::SubModelPartMapping::value_type part((*I_parts).Name, parts);
				submodelPartMapping->insert(part);
				if ((*I_parts).Show) {
					showPartVector.push_back((*I_parts).Name);
				}
			}
			submodel->createSubModelParts(submodelPartMapping);
			aModel->addSubmodel(submodel);
			
		}
		
		
		std::vector<ActionDefinition>::const_iterator I_actions = mActions.begin();
		std::vector<ActionDefinition>::const_iterator I_actions_end = mActions.end();
		for (;I_actions != I_actions_end; ++I_actions) {
			std::multiset< Model::AnimationPart* >* animationPartSet = new std::multiset< Model::AnimationPart* >();
			
			
			std::vector<AnimationDefinition>::const_iterator I_anims = (*I_actions).Animations.begin();
			std::vector<AnimationDefinition>::const_iterator I_anims_end = (*I_actions).Animations.end();
			for (;I_anims != I_anims_end; ++I_anims) {
				Model::AnimationPart* animPart = new Model::AnimationPart();
				animPart->name = (*I_anims).Name;
				animPart->weight = (*I_anims).Weight;
				animationPartSet->insert(animPart);
			}
			aModel->mAnimationPartMap.insert(Model::AnimationPartMap::value_type((*I_actions).Name, animationPartSet));
		}
		
		
		
		
		std::vector<std::string>::const_iterator I = showPartVector.begin();
		std::vector<std::string>::const_iterator I_end = showPartVector.end();
		for (;I != I_end; I++) {
			aModel->showPart(*I);	
		}
		
		return aModel;
	
	}
	



	
bool ModelDefinition::createFromXML(std::string path)
{
/*	Ogre::Datachunk chunk;
	if (!_findResourceData(path, chunk)) {
		return false;
	}*/
	
	//std::string realPath(Ember::EmberServices::getInstance()->getConfigService()->getEmberDataDirectory() + "/media/modeldefinitions/"+ path);
	if (path == "") {
		return false;
	}
	
	try {
	        xercesc::XMLPlatformUtils::Initialize();
	}
	catch (const xercesc::XMLException& toCatch) {
	        char* message = xercesc::XMLString::transcode(toCatch.getMessage());
	        std::cout << "Error during initialization! :\n"
	         << message << "\n";
	    xercesc::XMLString::release(&message);
	    return false;
	}
	
	
	XMLCh tempStr[100];
	xercesc::XMLString::transcode("LS", tempStr, 99);
	xercesc::DOMImplementation *impl = xercesc::DOMImplementationRegistry::getDOMImplementation(tempStr);
	xercesc::DOMBuilder* parser = ((xercesc::DOMImplementationLS*)impl)->createDOMBuilder(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, 0);
	
	xercesc::DOMDocument *doc = 0;

	
	try {
	    doc = parser->parseURI(path.c_str());  
	}
	catch (const xercesc::XMLException& toCatch) {
		parser->release();
//		delete impl;
		//xercesc::XMLString::release(**tempStr);
		return false;
	}
	
	if (!doc) {
		parser->release();
		return false;
	}
	
	
	xercesc::DOMElement* root = doc->getDocumentElement();
	
	xercesc::XMLString::transcode("scale", tempStr, 99);
	if (root->hasAttribute(tempStr)) {
		mScale = atof(xercesc::XMLString::transcode(root->getAttribute(tempStr)));
	}
	xercesc::XMLString::transcode("rotation", tempStr, 99);
	if (root->hasAttribute(tempStr)) {
		mRotation = atof(xercesc::XMLString::transcode(root->getAttribute(tempStr)));
	}
	xercesc::XMLString::transcode("usescaleof", tempStr, 99);
	if (root->hasAttribute(tempStr)) {
		std::string useScaleOf = xercesc::XMLString::transcode(root->getAttribute(tempStr));
		if (useScaleOf == "height") {
			mUseScaleOf = Model::MODEL_HEIGHT;
		} else if (useScaleOf == "width") {
			mUseScaleOf = Model::MODEL_WIDTH;
		} else {
			mUseScaleOf = Model::MODEL_DEPTH;
		}
	}
	 

	xercesc::XMLString::transcode("submodel", tempStr, 99);
	xercesc::DOMNodeList* submodelsNodes = doc->getElementsByTagName(tempStr);
	std::cout << "Number of submodel nodes: " << submodelsNodes->getLength() << "\n";
	
	for (unsigned int i = 0; i < submodelsNodes->getLength(); ++i) {
		SubModelDefinition subModelDef;
		xercesc::XMLString::transcode("mesh", tempStr, 99);
		subModelDef.Mesh = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(submodelsNodes->item(i))->getAttribute(tempStr));
		//preload
		Ogre::MeshManager::getSingleton().load(subModelDef.Mesh);
		
		
		
		xercesc::XMLString::transcode("part", tempStr, 99);
		xercesc::DOMNodeList* partsNodes = dynamic_cast<xercesc::DOMElement*>(submodelsNodes->item(i))->getElementsByTagName(tempStr);
		for (unsigned int j = 0; j < partsNodes->getLength(); ++j) {
			PartDefinition partDef;
			
			
// StringSet parts;

			//check if there's a "subentities" element
			xercesc::XMLString::transcode("subentities", tempStr, 99);
			if (dynamic_cast<xercesc::DOMElement*>(partsNodes->item(j))->getElementsByTagName(tempStr)->getLength()) {
				xercesc::XMLString::transcode("subentity", tempStr, 99);
				xercesc::DOMNodeList* subentitiesNodes = dynamic_cast<xercesc::DOMElement*>(partsNodes->item(j))->getElementsByTagName(tempStr);
				for (unsigned int k = 0; k < subentitiesNodes->getLength(); ++k) {
					SubEntityDefinition subEntityDef;
					xercesc::DOMElement* subentity = dynamic_cast<xercesc::DOMElement*>(subentitiesNodes->item(k));
					xercesc::XMLString::transcode("name", tempStr, 99);
					subEntityDef.SubEntity = xercesc::XMLString::transcode(subentity->getAttribute(tempStr));
					xercesc::XMLString::transcode("material", tempStr, 99);
					if (subentity->hasAttribute(tempStr)) {
						subEntityDef.Material = xercesc::XMLString::transcode(subentity->getAttribute(tempStr));
						//preload
						EmberOgre::getSingleton().getSceneManager()->getMaterial(subEntityDef.Material);
						
						
					}
					partDef.SubEntities.push_back(subEntityDef);
					
				}
			}
			xercesc::XMLString::transcode("name", tempStr, 99);
			std::string name = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(partsNodes->item(j))->getAttribute(tempStr));
			partDef.Name = name;
			xercesc::XMLString::transcode("show", tempStr, 99);
			std::string show = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(partsNodes->item(j))->getAttribute(tempStr));
			partDef.Show = (show == "true");
			subModelDef.Parts.push_back(partDef);
		}
		mSubModels.push_back(subModelDef);
	}

	xercesc::XMLString::transcode("Actions", tempStr, 99);
	xercesc::DOMNodeList* animationNodes = doc->getElementsByTagName(tempStr);
	if (animationNodes->getLength()) {
		readAnimations(dynamic_cast<xercesc::DOMElement*>(animationNodes->item(0)));
	}	 



	parser->release();

	return true;
	
}

void ModelDefinition::readAnimations(xercesc::DOMElement* animationsNode)
{
	XMLCh tempStr[100];
	
	xercesc::XMLString::transcode("Action", tempStr, 99);
	xercesc::DOMNodeList* animationNodes = animationsNode->getElementsByTagName(tempStr);
	for (unsigned int i = 0; i < animationNodes->getLength(); ++i) {
		ActionDefinition actionDef;
		
		xercesc::XMLString::transcode("animationpart", tempStr, 99);
		xercesc::DOMNodeList* animationPartNodes = dynamic_cast<xercesc::DOMElement*>(animationNodes->item(i))->getElementsByTagName(tempStr);
		for (unsigned int j = 0; j< animationPartNodes->getLength(); ++j) {
			AnimationDefinition animDef;
			xercesc::XMLString::transcode("name", tempStr, 99);
			std::string name = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(animationPartNodes->item(j))->getAttribute(tempStr));
			xercesc::XMLString::transcode("weight", tempStr, 99);
			Ogre::Real weight = atof(xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(animationPartNodes->item(j))->getAttribute(tempStr)));
			animDef.Name = name;
			animDef.Weight = weight;
			actionDef.Animations.push_back(animDef);
		}
		xercesc::XMLString::transcode("name", tempStr, 99);
		std::string name = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(animationNodes->item(i))->getAttribute(tempStr));
		std::cout << "Added action: " << name << "\n";
		actionDef.Name = name;
		mActions.push_back(actionDef);		

	}
	
}
	


};
