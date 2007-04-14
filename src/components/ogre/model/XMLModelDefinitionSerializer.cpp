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
#include "XMLModelDefinitionSerializer.h"
#include "ModelDefinition.h"
#include "Model.h"
#include "components/ogre/XMLHelper.h"

#include "ModelDefinitionManager.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"

#ifdef WIN32
	#include <tchar.h>
	#define snprintf _snprintf
    #include <io.h> // for _access, Win32 version of stat()
    #include <direct.h> // for _mkdir
//	#include <sys/stat.h>


	//#include <iostream>
	//#include <fstream>
	//#include <ostream>
#endif

#include "framework/osdir.h"

// Namespaces
namespace EmberOgre {
namespace Model {

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
	Ember::TiXmlDocument xmlDoc;
	XMLHelper xmlHelper;
	if (!xmlHelper.Load(xmlDoc, stream)) {
		return;
	}

	Ember::TiXmlElement* rootElem = xmlDoc.RootElement();

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
			if (!modelDef.isNull()) {
				readModel(modelDef, smElem);
				modelDef->setValid(true);
			}
		} catch (const Ogre::Exception& ex) {
			S_LOG_FAILURE(ex.getFullDescription());
			//std::cerr << ex.getFullDescription();
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
	
/*	//rotation
	///TODO: change this into a better system, perhaps using quaternions, instead of like now just rotating around the y-axis
	tmp =  modelNode->Attribute("rotation");
	if (tmp)
		modelDef->mRotation = Ogre::StringConverter::parseReal(tmp);*/
	
	//showcontained
	tmp =  modelNode->Attribute("showcontained");
	if (tmp)
		modelDef->mShowContained = Ogre::StringConverter::parseBool(tmp);

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
	
	tmp =  modelNode->Attribute("renderingdistance");
	if (tmp)
	{
		modelDef->setRenderingDistance(Ogre::StringConverter::parseReal(tmp));
	}
	
	
	//submodels
	elem = modelNode->FirstChildElement("submodels");
	if (elem)
		readSubModels(modelDef, elem);

	//actions
	elem = modelNode->FirstChildElement("actions");
	if (elem)
		readActions(modelDef, elem);

	//attachpoints
	elem = modelNode->FirstChildElement("attachpoints");
	if (elem)
		readAttachPoints(modelDef, elem);

	//attachpoints
	elem = modelNode->FirstChildElement("particlesystems");
	if (elem)
		readParticleSystems(modelDef, elem);
		
	elem = modelNode->FirstChildElement("translate");
	if (elem)
	{
		modelDef->mTranslate = fillVector3FromElement(elem);
	}
	
	elem = modelNode->FirstChildElement("rotation");
	if (elem)
	{
		modelDef->setRotation(fillQuaternionFromElement(elem));
	}

	elem = modelNode->FirstChildElement("contentoffset");
	if (elem)
	{
		Ogre::Vector3 offset = fillVector3FromElement(elem);
		modelDef->setContentOffset(offset);
	}
	
}



void XMLModelDefinitionSerializer::readSubModels(ModelDefinitionPtr modelDef, Ember::TiXmlElement* mSubModelNode)
{
	S_LOG_VERBOSE( "Read Submodels" );
	const char* tmp = 0;
	Ember::TiXmlElement* elem;
	bool notfound = true;

	for (Ember::TiXmlElement* smElem = mSubModelNode->FirstChildElement();
            smElem != 0; smElem = smElem->NextSiblingElement())
    {
		notfound = false;

		tmp =  smElem->Attribute("mesh");
		if (tmp)
		{
			SubModelDefinition* subModelDef = modelDef->createSubModelDefinition(tmp);
			
			S_LOG_VERBOSE( " Add submodel  : "+ subModelDef->getMeshName() );
			try 
			{
				//preload
				//FIX Ogre::MeshManager::getSingleton().load(subModelDef.Mesh);
/*					Ogre::MeshManager::getSingleton().load( subModelDef.Mesh, 
							Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );*/
			
				//parts

				elem = smElem->FirstChildElement("parts");
				if (elem)
					readParts(elem, subModelDef);

//				modelDef->mSubModels.push_back(subModelDef);

			} 
			catch (const Ogre::Exception& e) 
			{
				S_LOG_FAILURE("Load error : " << tmp);
			}
		}
	}

	if(notfound)
	{
		S_LOG_VERBOSE( "No submodel found.");
	}
}

void XMLModelDefinitionSerializer::readParts(Ember::TiXmlElement* mPartNode, SubModelDefinition* def)
{
	Ember::TiXmlElement* elem;
	const char* tmp = 0;
	bool notfound = true;

	for (Ember::TiXmlElement* partElem = mPartNode->FirstChildElement();
            partElem != 0; partElem = partElem->NextSiblingElement())
	{

		// name
		tmp =  partElem->Attribute("name");
		if (tmp) {
			notfound = false;
		
			PartDefinition* partDef = def->createPartDefinition(tmp);
			
			S_LOG_VERBOSE( "  Add part  : "+ partDef->getName() );
	
			// show
			tmp =  partElem->Attribute("show");
			if (tmp)
				partDef->setShow(Ogre::StringConverter::parseBool(tmp));
	
			tmp =  partElem->Attribute("group");
			if (tmp)
				partDef->setGroup(tmp);
	
			elem = partElem->FirstChildElement("subentities");
			if (elem)
				readSubEntities(elem, partDef);
	
		} else {
			S_LOG_FAILURE( "A name must be specified for each part." );
		}
	}

	if(notfound)
	{
		S_LOG_VERBOSE( "No part found." );
	}
}

void XMLModelDefinitionSerializer::readSubEntities(Ember::TiXmlElement* mSubEntNode, PartDefinition* def)
{

	const char* tmp = 0;
	bool notfound = true;

	for (Ember::TiXmlElement* seElem = mSubEntNode->FirstChildElement();
            seElem != 0; seElem = seElem->NextSiblingElement())
	{
		SubEntityDefinition* subEntityDef = 0;
		// name
		tmp =  seElem->Attribute("index");
		if (tmp) {
				notfound = false;
				subEntityDef = def->createSubEntityDefinition(Ogre::StringConverter::parseInt(tmp));
				S_LOG_VERBOSE( "   Add sub entity with index: " << subEntityDef->getSubEntityIndex());
		} else {
			tmp =  seElem->Attribute("name");
			if (tmp) {
				notfound = false;
				subEntityDef = def->createSubEntityDefinition(tmp);
				S_LOG_VERBOSE( "   Add sub entity: " << subEntityDef->getSubEntityName());
			}
		}
		if (!notfound) {
			//material
			tmp =  seElem->Attribute("material");
			if (tmp)
			{
				subEntityDef->setMaterialName(tmp);
				//preload subEntityDef.Material
				//Ogre::MaterialManager::getSingleton().load( subEntityDef.Material, 
								//Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
	
			}

		} else {
			S_LOG_FAILURE( "A subentity name or index must be specified for each subentity." );
		}
	}

	if(notfound)
	{
		S_LOG_VERBOSE( "No sub entity found." );
	}
}

void XMLModelDefinitionSerializer::readActions(ModelDefinitionPtr modelDef, Ember::TiXmlElement* mAnimNode)
{
	Ember::TiXmlElement* elem;
	const char* tmp = 0;
	bool notfound = true;
	S_LOG_VERBOSE( "Read Actions" );

	for (Ember::TiXmlElement* animElem = mAnimNode->FirstChildElement();
            animElem != 0; animElem = animElem->NextSiblingElement())
	{
		notfound=false;

		tmp =  animElem->Attribute("name");
		if (tmp) {
			ActionDefinition* actionDef = modelDef->createActionDefinition(tmp);
			S_LOG_VERBOSE( " Add action  : "<< tmp  );
			
			tmp =  animElem->Attribute("speed");
			if (tmp) { 
				actionDef->setAnimationSpeed(Ogre::StringConverter::parseReal(tmp));
			}


			elem = animElem->FirstChildElement("animations");
			if (elem)
				readAnimations(elem, actionDef);
			
			elem = animElem->FirstChildElement("sound");
			if (elem) {
				//only attach one sound to each action
				std::string name;
				bool repeat = false;
				Ogre::Real volume = 1.0f;
				tmp =  animElem->Attribute("name");
				if (tmp) { 
					name = tmp;
					tmp =  animElem->Attribute("repeat");
					if (tmp) { 
						repeat = Ogre::StringConverter::parseBool(tmp);
					}
					tmp =  animElem->Attribute("volume");
					if (tmp) { 
						volume = Ogre::StringConverter::parseReal(tmp);
					}
					actionDef->createSoundDefinition(name, repeat, volume);
				}
			}
		}

	}

	if(notfound)
	{
		S_LOG_VERBOSE( "No actions found." );
	}

}
	
void XMLModelDefinitionSerializer::readAnimations(Ember::TiXmlElement* mAnimationsNode, ActionDefinition* action)
{
	const char* tmp = 0;
	bool nopartfound = true;
	

	for (Ember::TiXmlElement* animElem = mAnimationsNode->FirstChildElement();
            animElem != 0; animElem = animElem->NextSiblingElement())
	{
		int iterations(1);
		nopartfound = false;

		// name
		tmp =  animElem->Attribute("iterations");
		if (tmp) {
			iterations = Ogre::StringConverter::parseInt(tmp);
		}

		AnimationDefinition* animDef = action->createAnimationDefinition(iterations);
		readAnimationParts(animElem, animDef);
	}

	if(nopartfound)
	{
		S_LOG_VERBOSE( "  No animations found!!" );
	}

}

void XMLModelDefinitionSerializer::readAnimationParts(Ember::TiXmlElement* mAnimPartNode, AnimationDefinition* animDef)
{
	const char* tmp = 0;
	bool nopartfound = true;
	
	for (Ember::TiXmlElement* apElem = mAnimPartNode->FirstChildElement();
            apElem != 0; apElem = apElem->NextSiblingElement())
	{
		std::string name;
		Ogre::Real weight = 1.0f;
		nopartfound = false;

		// name
		tmp =  apElem->Attribute("name");
		if (tmp) {
			name = tmp;
			S_LOG_VERBOSE( "  Add animation  : "+ name );
		}
	

		// weight
		tmp =  apElem->Attribute("weight");
		if (tmp)
			weight = Ogre::StringConverter::parseReal(tmp);

		animDef->createAnimationPartDefinition(name, weight);
	}

	if(nopartfound)
	{
		S_LOG_VERBOSE( "   No anim parts found." );
	}
}


void XMLModelDefinitionSerializer::readAttachPoints(ModelDefinitionPtr modelDef, Ember::TiXmlElement* mAnimPartNode)
{
	AttachPointDefinitionStore & attachPoints = modelDef->mAttachPoints;
	
	const char* tmp = 0;
	bool nopartfound = true;

	for (Ember::TiXmlElement* apElem = mAnimPartNode->FirstChildElement();
            apElem != 0; apElem = apElem->NextSiblingElement())
	{
		AttachPointDefinition attachPointDef;
		nopartfound = false;

		// name
		tmp =  apElem->Attribute("name");
		if (tmp)
			attachPointDef.Name = tmp;
		S_LOG_VERBOSE( "  Add attachpoint  : "+ attachPointDef.Name );

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
		S_LOG_VERBOSE( "  Add particlescript  : "+ def.Script );

		elem = apElem->FirstChildElement("bindings");
		if (elem)
			readParticleSystemsBindings(def, elem);


		particleSystems.push_back(def);
	}
}

void XMLModelDefinitionSerializer::readParticleSystemsBindings(ModelDefinition::ParticleSystemDefinition& def, Ember::TiXmlElement* mParticleSystemsNode)
{
	const char* tmp = 0;
// 	bool nopartfound = true;

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
		
		S_LOG_VERBOSE( "  Add binding between "<< binding.EmitterVar << " and " << binding.AtlasAttribute << "." );


		def.Bindings.push_back(binding);
	}

}

Ogre::Vector3 XMLModelDefinitionSerializer::fillVector3FromElement(Ember::TiXmlElement* elem)
{
	Ogre::Real x=0.0f, y=0.0f, z=0.0f;
	if (elem->Attribute("x")) {
		x = atof(elem->Attribute("x"));
	}
	if (elem->Attribute("y")) {
		y = atof(elem->Attribute("y"));
	}
	if (elem->Attribute("z")) {
		z = atof(elem->Attribute("z"));
	}
	
	return Ogre::Vector3(x,y,z);
}


void XMLModelDefinitionSerializer::fillElementFromVector3(Ember::TiXmlElement& elem, Ogre::Vector3 vector)
{
	elem.SetDoubleAttribute("x", vector.x);
	elem.SetDoubleAttribute("y", vector.y);
	elem.SetDoubleAttribute("z", vector.z);
}

Ogre::Quaternion XMLModelDefinitionSerializer::fillQuaternionFromElement(Ember::TiXmlElement* elem)
{
	Ogre::Vector3 vector = fillVector3FromElement(elem);
	Ogre::Degree degrees;
	///first check if degrees is specified, but also allow for radians to be specified
	if (elem->Attribute("degrees")) {
		degrees = atof(elem->Attribute("degrees"));
	} else if (elem->Attribute("radians")) {
		degrees = Ogre::Radian(atof(elem->Attribute("radians")));
	}
	Ogre::Quaternion q;
	q.FromAngleAxis(degrees, vector);
	return q;
	
}

void XMLModelDefinitionSerializer::fillElementFromQuaternion(Ember::TiXmlElement& elem, Ogre::Quaternion quaternion)
{
	///split the quaternion into an axis and a degree (our format allows us to store the angle element as a radian too, but I prefer degrees)
	Ogre::Degree degrees;
	Ogre::Vector3 axis;
	quaternion.ToAngleAxis( degrees, axis);
	fillElementFromVector3(elem, axis);
	elem.SetDoubleAttribute("degrees", degrees.valueDegrees());
}


void XMLModelDefinitionSerializer::exportScript(ModelDefinitionPtr modelDef, const std::string& filename)
{
	if (filename == "") {
		return;
	}

	Ember::TiXmlDocument xmlDoc;
	
	try
	{
		//make sure the directory exists
		std::string dir = Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory() + "/user-media";
		
		if (!oslink::directory(dir).isExisting()) {
			S_LOG_INFO("Creating directory " << dir);
#ifdef __WIN32__
			mkdir(dir.c_str());
#else 
			mkdir(dir.c_str(), S_IRWXU);
#endif
		}




		dir = Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory() + "/user-media/modeldefinitions/";
		
		if (!oslink::directory(dir).isExisting()) {
			S_LOG_INFO("Creating directory " << dir);
#ifdef __WIN32__
			mkdir(dir.c_str());
#else 
			mkdir(dir.c_str(), S_IRWXU);
#endif
		}
		
		Ember::TiXmlElement elem("models");
		Ember::TiXmlElement modelElem("model");
		modelElem.SetAttribute("name", modelDef->getName().c_str());
		
		std::string useScaleOf;
		switch (modelDef->getUseScaleOf()) {
			case ModelDefinition::MODEL_ALL:
				useScaleOf = "all";
				break;
			case ModelDefinition::MODEL_DEPTH:
				useScaleOf = "depth";
				break;
			case ModelDefinition::MODEL_HEIGHT:
				useScaleOf = "height";
				break;
			case ModelDefinition::MODEL_NONE:
				useScaleOf = "none";
				break;
			case ModelDefinition::MODEL_WIDTH:
				useScaleOf = "width";
				break;
		}
		modelElem.SetAttribute("usescaleof", useScaleOf.c_str());
		
		if (modelDef->getRenderingDistance() != 0.0f) {
			modelElem.SetDoubleAttribute("renderingdistance", modelDef->getRenderingDistance());
		}
		
		if (modelDef->getScale() != 0) {
			modelElem.SetDoubleAttribute("scale", modelDef->getScale());
		}
		
		modelElem.SetAttribute("showcontained", modelDef->getShowContained() ? "true" : "false");
		
		if (modelDef->getContentOffset() != Ogre::Vector3::ZERO) {
			Ember::TiXmlElement contentOffset("contentoffset");
			fillElementFromVector3(contentOffset, modelDef->getContentOffset());
			modelElem.InsertEndChild(contentOffset);
		}
		
		Ember::TiXmlElement translate("translate");
		fillElementFromVector3(translate, modelDef->getTranslate());
		modelElem.InsertEndChild(translate);
		
		Ember::TiXmlElement rotation("rotation");
		fillElementFromQuaternion(rotation, modelDef->getRotation());
		modelElem.InsertEndChild(rotation);
		
		
		
		//start with submodels
		exportSubModels(modelDef, modelElem);
		
		//now do actions
		exportActions(modelDef, modelElem);
		
		
		
		elem.InsertEndChild(modelElem);
		
		xmlDoc.InsertEndChild(elem);
		xmlDoc.SaveFile((dir + filename).c_str());
		S_LOG_INFO("Saved file " << (dir + filename));
	}
	catch (...)
	{
		S_LOG_FAILURE("An error occurred saving the modeldefinition for "<< modelDef->getName() << "." );
	}
		

}

void XMLModelDefinitionSerializer::exportActions(ModelDefinitionPtr modelDef, Ember::TiXmlElement& modelElem)
{
	Ember::TiXmlElement actionsElem("actions");
	
	for (ActionDefinitionsStore::const_iterator I = modelDef->getActionDefinitions().begin(); I != modelDef->getActionDefinitions().end(); ++I) {
		Ember::TiXmlElement actionElem("action");
		actionElem.SetAttribute("name", (*I)->getName().c_str());
		actionElem.SetDoubleAttribute("speed", (*I)->getAnimationSpeed());
		
		if ((*I)->getAnimationDefinitions().size() > 0) {
			Ember::TiXmlElement animationsElem("animations");
			for (AnimationDefinitionsStore::const_iterator J = (*I)->getAnimationDefinitions().begin(); J != (*I)->getAnimationDefinitions().end(); ++J) {
				Ember::TiXmlElement animationElem("animation");
				animationElem.SetAttribute("iterations", (*J)->getIterations());
				
				for (AnimationPartDefinitionsStore::const_iterator K = (*J)->getAnimationPartDefinitions().begin(); K != (*J)->getAnimationPartDefinitions().end(); ++K) {
					Ember::TiXmlElement animationPartElem("animationpart");
					animationPartElem.SetAttribute("name", (*K)->Name.c_str());
					animationPartElem.SetDoubleAttribute("weight", (*K)->Weight);
					animationElem.InsertEndChild(animationPartElem);
				}
				
				animationsElem.InsertEndChild(animationElem);
			}
			actionElem.InsertEndChild(animationsElem);
		}
		
		//for now, only allow one sound
		if ((*I)->getSoundDefinitions().size() > 0) {
			SoundDefinition* def = *(*I)->getSoundDefinitions().begin();
			Ember::TiXmlElement soundElem("sound");
			soundElem.SetAttribute("name", def->Name.c_str());
			soundElem.SetAttribute("repeat", def->Repeat);
			soundElem.SetDoubleAttribute("volume", def->Volume);
			actionElem.InsertEndChild(soundElem);
		}
		actionsElem.InsertEndChild(actionElem);
	}
	modelElem.InsertEndChild(actionsElem);
}

void XMLModelDefinitionSerializer::exportSubModels(ModelDefinitionPtr modelDef, Ember::TiXmlElement& modelElem)
{
	Ember::TiXmlElement submodelsElem("submodels");
	
	for (SubModelDefinitionsStore::const_iterator I = modelDef->getSubModelDefinitions().begin(); I != modelDef->getSubModelDefinitions().end(); ++I) {
		Ember::TiXmlElement submodelElem("submodel");
		submodelElem.SetAttribute("mesh", (*I)->getMeshName().c_str());
		Ember::TiXmlElement partsElem("parts");
		
		for (PartDefinitionsStore::const_iterator J = (*I)->getPartDefinitions().begin(); J != (*I)->getPartDefinitions().end(); ++J) {
			Ember::TiXmlElement partElem("part");
			partElem.SetAttribute("name", (*J)->getName().c_str());
			if ((*J)->getGroup() != "") {
				partElem.SetAttribute("group", (*J)->getGroup().c_str());
			}
			partElem.SetAttribute("show", (*J)->getShow() ? "true" : "false");
			
			if ((*J)->getSubEntityDefinitions().size() > 0) {
				Ember::TiXmlElement subentitiesElem("subentities");
				for (SubEntityDefinitionsStore::const_iterator K = (*J)->getSubEntityDefinitions().begin(); K != (*J)->getSubEntityDefinitions().end(); ++K) {
					Ember::TiXmlElement subentityElem("subentity");
					if ((*K)->getSubEntityName() != "") {
						subentityElem.SetAttribute("name", (*K)->getSubEntityName().c_str());
					} else {
						subentityElem.SetAttribute("index", (*K)->getSubEntityIndex());
					}
					if ((*K)->getMaterialName() != "") {
						subentityElem.SetAttribute("material", (*K)->getMaterialName().c_str());
					}
					subentitiesElem.InsertEndChild(subentityElem);
				}
				partElem.InsertEndChild(subentitiesElem);
			}
			partsElem.InsertEndChild(partElem);
		}
		submodelElem.InsertEndChild(partsElem);
		submodelsElem.InsertEndChild(submodelElem);
	}
	modelElem.InsertEndChild(submodelsElem);

}

} //end namespace
}
