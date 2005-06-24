/*
-------------------------------------------------------------------------------
	This source file is part of Cataclysmos
	For the latest info, see http://www.cataclysmos.org/

	Copyright (c) 2005 The Cataclysmos Team
    Copyright (C) 2005  Erik Hjortsberg

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
-------------------------------------------------------------------------------
*/
// Includes
#include <XMLModelDefinitionSerializer.h>
#include <ModelDefinition.h>
#include <Model.h>

#include "ModelDefinitionManager.h"

// Namespaces
namespace EmberOgre {
// Variables

// Functions
XMLModelDefinitionSerializer::XMLModelDefinitionSerializer()
{}

XMLModelDefinitionSerializer::~XMLModelDefinitionSerializer()
{}
void XMLModelDefinitionSerializer::importModelDefinition(Ogre::DataStreamPtr& stream, ModelDefinition* pModelDef)
{
}

void XMLModelDefinitionSerializer::parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName)
{
	_XMLDoc = new Ember::TiXmlDocument();
	bool success = _XMLDoc->LoadFile(stream); //load from data stream
	
	if (!success) {
		S_LOG_FAILURE("Failed to load modeldefinition file!");
		return;
	}
	
	Ember::TiXmlElement* elem;


	Ember::TiXmlElement* rootElem = _XMLDoc->RootElement();

	for (Ember::TiXmlElement* smElem = rootElem->FirstChildElement();
            smElem != 0; smElem = smElem->NextSiblingElement())
    {
		const char* tmp =  smElem->Attribute("name");
		std::string name;
		if (!tmp) {
			continue;
		} else {
			name = tmp;
		}
				
		try {
			ModelDefinitionPtr modelDef = ModelDefinitionManager::getSingleton().create(name, groupName);
			readModel(modelDef, smElem);
			modelDef->setValid(true);
		} catch (Ogre::Exception& ex) {
			S_LOG_FAILURE(ex.getFullDescription());
			std::cerr << ex.getFullDescription();
		}
		
		//modelDef->_notifyOrigin(context.filename);
	}	
	
}

void XMLModelDefinitionSerializer::readModel(ModelDefinitionPtr modelDef, Ember::TiXmlElement* modelNode)
{
	Ember::TiXmlElement* elem;
	//root elements
	//scale
	const char* tmp =  modelNode->Attribute("scale");
	if (tmp)
		modelDef->mScale=Ogre::StringConverter::parseReal(tmp);
	
	//rotation
	tmp =  modelNode->Attribute("rotation");
	if (tmp)
		modelDef->mRotation = Ogre::StringConverter::parseReal(tmp);

	//usescaleof
	tmp =  modelNode->Attribute("usescaleof");
	if (tmp)
	{
		std::string useScaleOf(tmp);
		if (useScaleOf == "height")
			modelDef->mUseScaleOf = ModelDefinition::MODEL_HEIGHT;
		else if (useScaleOf == "width") 
			modelDef->mUseScaleOf = ModelDefinition::MODEL_WIDTH;
		else if (useScaleOf == "depth") 
			modelDef->mUseScaleOf = ModelDefinition::MODEL_DEPTH;
		else if (useScaleOf == "none") 
			modelDef->mUseScaleOf = ModelDefinition::MODEL_NONE;
	}
	
	//submodels
	elem = modelNode->FirstChildElement("submodels");
	if (elem)
		readSubModels(modelDef, elem);

	//actions
	elem = modelNode->FirstChildElement("Actions");
	if (elem)
		readActions(modelDef, elem);

	//attachpoints
	elem = modelNode->FirstChildElement("attachpoints");
	if (elem)
		readAttachPoints(modelDef, elem);

	//attachpoints
	elem = modelNode->FirstChildElement("ParticleSystems");
	if (elem)
		readParticleSystems(modelDef, elem);
		
}



void XMLModelDefinitionSerializer::readSubModels(ModelDefinitionPtr modelDef, Ember::TiXmlElement* mSubModelNode)
{
	S_LOG_INFO( "Read Submodels" );
	const char* tmp = 0;
	Ember::TiXmlElement* elem;
	bool notfound = true;

	for (Ember::TiXmlElement* smElem = mSubModelNode->FirstChildElement();
            smElem != 0; smElem = smElem->NextSiblingElement())
    {
		ModelDefinition::SubModelDefinition subModelDef;
		notfound = false;

		tmp =  smElem->Attribute("mesh");
		if (tmp)
		{
			subModelDef.Mesh = tmp;
			S_LOG_INFO( " Add submodel  : "+ subModelDef.Mesh );
			try 
			{
				//preload
				//FIX Ogre::MeshManager::getSingleton().load(subModelDef.Mesh);
					Ogre::MeshManager::getSingleton().load( subModelDef.Mesh, 
							Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
			
				//parts

				elem = smElem->FirstChildElement("parts");
				if (elem)
					readParts(elem,subModelDef);

				modelDef->mSubModels.push_back(subModelDef);

			} 
			catch (Ogre::Exception e) 
			{
				S_LOG_FAILURE("Load error : "+ subModelDef.Mesh);
			}
		}
	}

	if(notfound)
	{
		S_LOG_FAILURE( "No submodel found !!");
	}
}

void XMLModelDefinitionSerializer::readParts(Ember::TiXmlElement* mPartNode,ModelDefinition::SubModelDefinition& subModel)
{
	Ember::TiXmlElement* elem;
	const char* tmp = 0;
	bool notfound = true;

	for (Ember::TiXmlElement* partElem = mPartNode->FirstChildElement();
            partElem != 0; partElem = partElem->NextSiblingElement())
	{
		ModelDefinition::PartDefinition partDef;
		notfound = false;

		// name
		tmp =  partElem->Attribute("name");
		if (tmp)
			partDef.Name = tmp;
		S_LOG_INFO( "  Add part  : "+ partDef.Name );

		// show
		tmp =  partElem->Attribute("show");
		if (tmp)
			partDef.Show = Ogre::StringConverter::parseBool(tmp);


		elem = partElem->FirstChildElement("subentities");
		if (elem)
			readSubEntities(elem,partDef);

		subModel.Parts.push_back(partDef);
	}

	if(notfound)
	{
		S_LOG_FAILURE( "No part found !!" );
	}
}

void XMLModelDefinitionSerializer::readSubEntities(Ember::TiXmlElement* mSubEntNode,ModelDefinition::PartDefinition& part)
{

	const char* tmp = 0;
	bool notfound = true;

	for (Ember::TiXmlElement* seElem = mSubEntNode->FirstChildElement();
            seElem != 0; seElem = seElem->NextSiblingElement())
	{
		ModelDefinition::SubEntityDefinition subEntityDef;
		notfound = false;

		// name
		tmp =  seElem->Attribute("name");
		if (tmp)
			subEntityDef.SubEntity = tmp;
		S_LOG_INFO( "   Add sub entity  : "+ subEntityDef.SubEntity  );

		//material
		tmp =  seElem->Attribute("material");
		if (tmp)
		{
			subEntityDef.Material = tmp;
			//preload subEntityDef.Material
			Ogre::MaterialManager::getSingleton().load( subEntityDef.Material, 
							Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

		}

		part.SubEntities.push_back(subEntityDef);
	}

	if(notfound)
	{
		S_LOG_FAILURE( "No sub entity found !!" );
	}
}

void XMLModelDefinitionSerializer::readActions(ModelDefinitionPtr modelDef, Ember::TiXmlElement* mAnimNode)
{
	Ember::TiXmlElement* elem;
	const char* tmp = 0;
	bool notfound = true;
	S_LOG_INFO( "Read Actions" );

	for (Ember::TiXmlElement* animElem = mAnimNode->FirstChildElement();
            animElem != 0; animElem = animElem->NextSiblingElement())
	{
		ModelDefinition::ActionDefinition actionDef;
		notfound=false;

		tmp =  animElem->Attribute("name");
		if (tmp)
			actionDef.Name = tmp;
		S_LOG_INFO( " Add action  : "+ actionDef.Name  );

		elem = animElem->FirstChildElement("animationparts");
		if (elem)
			readAnimationParts(elem,actionDef);
		
		elem = animElem->FirstChildElement("sound");
		if (elem) {
			//only attach one sound to each action
			ModelDefinition::SoundDefinition soundDef;
			tmp =  animElem->Attribute("name");
			if (tmp) { 
				soundDef.Name = tmp;
				tmp =  animElem->Attribute("repeat");
				if (tmp) { 
					soundDef.Repeat = Ogre::StringConverter::parseBool(tmp);
				}
				actionDef.Sounds.push_back(soundDef);
			}
		}

		modelDef->mActions.push_back(actionDef);
	}

	if(notfound)
	{
		S_LOG_FAILURE( "No actions found !!" );
	}

}

void XMLModelDefinitionSerializer::readAnimationParts(Ember::TiXmlElement* mAnimPartNode,ModelDefinition::ActionDefinition& action)
{
	const char* tmp = 0;
	bool nopartfound = true;

	for (Ember::TiXmlElement* apElem = mAnimPartNode->FirstChildElement();
            apElem != 0; apElem = apElem->NextSiblingElement())
	{
		ModelDefinition::AnimationDefinition animDef;
		nopartfound = false;

		// name
		tmp =  apElem->Attribute("name");
		if (tmp)
			animDef.Name = tmp;
		S_LOG_INFO( "  Add animation  : "+ animDef.Name );

		// weight
		tmp =  apElem->Attribute("weight");
		if (tmp)
			animDef.Weight = Ogre::StringConverter::parseReal(tmp);


		action.Animations.push_back(animDef);
	}

	if(nopartfound)
	{
		S_LOG_FAILURE( "  No anim parts found !!" );
	}
}


void XMLModelDefinitionSerializer::readAttachPoints(ModelDefinitionPtr modelDef, Ember::TiXmlElement* mAnimPartNode)
{
	ModelDefinition::AttachPointDefinitionStore & attachPoints = modelDef->mAttachPoints;
	
	const char* tmp = 0;
	bool nopartfound = true;

	for (Ember::TiXmlElement* apElem = mAnimPartNode->FirstChildElement();
            apElem != 0; apElem = apElem->NextSiblingElement())
	{
		ModelDefinition::AttachPointDefinition attachPointDef;
		nopartfound = false;

		// name
		tmp =  apElem->Attribute("name");
		if (tmp)
			attachPointDef.Name = tmp;
		S_LOG_INFO( "  Add attachpoint  : "+ attachPointDef.Name );

		// weight
		tmp =  apElem->Attribute("bone");
		if (tmp)
			attachPointDef.BoneName = tmp;


		attachPoints.push_back(attachPointDef);
	}

}

void  XMLModelDefinitionSerializer::readParticleSystems(ModelDefinitionPtr modelDef, Ember::TiXmlElement* mParticleSystemsNode)
{
	Ember::TiXmlElement* elem;
	ModelDefinition::ParticleSystemSet& particleSystems = modelDef->mParticleSystems;
	
	const char* tmp = 0;
	bool nopartfound = true;

	for (Ember::TiXmlElement* apElem = mParticleSystemsNode->FirstChildElement();
            apElem != 0; apElem = apElem->NextSiblingElement())
	{
		ModelDefinition::ParticleSystemDefinition def;
		nopartfound = false;

		// name
		tmp =  apElem->Attribute("script");
		if (tmp)
			def.Script = tmp;
		S_LOG_INFO( "  Add particlescript  : "+ def.Script );

		elem = apElem->FirstChildElement("Bindings");
		if (elem)
			readParticleSystemsBindings(def, elem);


		particleSystems.push_back(def);
	}
}

void XMLModelDefinitionSerializer::readParticleSystemsBindings(ModelDefinition::ParticleSystemDefinition& def, Ember::TiXmlElement* mParticleSystemsNode)
{
	const char* tmp = 0;
	bool nopartfound = true;

	for (Ember::TiXmlElement* apElem = mParticleSystemsNode->FirstChildElement();
            apElem != 0; apElem = apElem->NextSiblingElement())
	{
		ModelDefinition::BindingDefinition binding;
		
		// name
		tmp =  apElem->Attribute("emittervar");
		if (tmp)
			binding.EmitterVar = tmp;
		else
			continue;

		// weight
		tmp =  apElem->Attribute("atlasattribute");
		if (tmp)
			binding.AtlasAttribute = tmp;
		else
			continue;
		
		S_LOG_INFO( "  Add binding between "+ binding.EmitterVar + " and " + binding.AtlasAttribute + "." );


		def.Bindings.push_back(binding);
	}

}


} //end namespace
