/*
-------------------------------------------------------------------------------
	This source file is part of Cataclysmos
	For the latest info, see http://www.cataclysmos.org/

	Copyright (c) 2005 The Cataclysmos Team
    Copyright (C) 2005  Erik Ogenvik

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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "XMLModelDefinitionSerializer.h"
#include "Model.h"
#include "components/ogre/XMLHelper.h"

#include "ModelDefinitionManager.h"

#include <OgreStringConverter.h>

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

namespace Ember {
namespace OgreView {
namespace Model {

XMLModelDefinitionSerializer::XMLModelDefinitionSerializer()
{}

XMLModelDefinitionSerializer::~XMLModelDefinitionSerializer()
{}
void XMLModelDefinitionSerializer::importModelDefinition(Ogre::DataStreamPtr&, ModelDefinition*)
{
}

void XMLModelDefinitionSerializer::parseScript(ModelDefinitionManager& modelDefManager, Ogre::DataStreamPtr& stream, const Ogre::String& groupName)
{
	TiXmlDocument xmlDoc;
	XMLHelper xmlHelper;
	if (!xmlHelper.Load(xmlDoc, stream)) {
		return;
	}

	TiXmlElement* rootElem = xmlDoc.RootElement();
	if (rootElem) {

		for (TiXmlElement* smElem = rootElem->FirstChildElement();
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
				ModelDefinitionPtr modelDef = modelDefManager.create(name, groupName);
				if (!modelDef.isNull()) {
					readModel(modelDef, smElem);
					modelDef->setValid(true);
				}
			} catch (const Ogre::Exception& ex) {
				S_LOG_FAILURE("Error when parsing model '" << name << "'." << ex);
			}

			//modelDef->_notifyOrigin(context.filename);
		}
	}

}

void XMLModelDefinitionSerializer::readModel(ModelDefinitionPtr modelDef, TiXmlElement* modelNode)
{
	TiXmlElement* elem;
	//root elements
	//scale
	const char* tmp =  modelNode->Attribute("scale");
	if (tmp)
		modelDef->mScale=Ogre::StringConverter::parseReal(tmp);

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
			modelDef->mUseScaleOf = ModelDefinition::UseScaleOf::MODEL_HEIGHT;
		else if (useScaleOf == "width")
			modelDef->mUseScaleOf = ModelDefinition::UseScaleOf::MODEL_WIDTH;
		else if (useScaleOf == "depth")
			modelDef->mUseScaleOf = ModelDefinition::UseScaleOf::MODEL_DEPTH;
		else if (useScaleOf == "none")
			modelDef->mUseScaleOf = ModelDefinition::UseScaleOf::MODEL_NONE;
	}

	tmp =  modelNode->Attribute("renderingdistance");
	if (tmp)
	{
		modelDef->setRenderingDistance(Ogre::StringConverter::parseReal(tmp));
	}

	tmp = modelNode->Attribute("icon");
	if (tmp) {
		modelDef->mIconPath = tmp;
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

	//views
	elem = modelNode->FirstChildElement("views");
	if (elem)
		readViews(modelDef, elem);

	elem = modelNode->FirstChildElement("translate");
	if (elem)
	{
		modelDef->mTranslate = XMLHelper::fillVector3FromElement(elem);
	}

	elem = modelNode->FirstChildElement("rotation");
	if (elem)
	{
		modelDef->setRotation(XMLHelper::fillQuaternionFromElement(elem));
	}

	elem = modelNode->FirstChildElement("contentoffset");
	if (elem)
	{
		Ogre::Vector3 offset = XMLHelper::fillVector3FromElement(elem);
		modelDef->setContentOffset(offset);
	}

	elem = modelNode->FirstChildElement("rendering");
	if (elem)
	{
		modelDef->mRenderingDef = new RenderingDefinition();
		tmp = elem->Attribute("scheme");
		if (tmp) {
			modelDef->mRenderingDef->setScheme(tmp);
		}
		for (TiXmlElement* paramElem = elem->FirstChildElement("param");
				paramElem != 0; paramElem = paramElem->NextSiblingElement())
		{
			tmp = paramElem->Attribute("key");
			if (tmp) {
				modelDef->mRenderingDef->mParams.insert(StringParamStore::value_type(tmp, paramElem->GetText()));
			}
		}
	}

	elem = modelNode->FirstChildElement("lights");
	if (elem)
		readLights(modelDef, elem);

	elem = modelNode->FirstChildElement("bonegroups");
	if (elem)
		readBoneGroups(modelDef, elem);
	elem = modelNode->FirstChildElement("poses");
	if (elem)
		readPoses(modelDef, elem);

}



void XMLModelDefinitionSerializer::readSubModels(ModelDefinitionPtr modelDef, TiXmlElement* mSubModelNode)
{
	S_LOG_VERBOSE( "Read Submodels" );
	const char* tmp = 0;
	TiXmlElement* elem;
	bool notfound = true;

	for (TiXmlElement* smElem = mSubModelNode->FirstChildElement();
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

void XMLModelDefinitionSerializer::readParts(TiXmlElement* mPartNode, SubModelDefinition* def)
{
	TiXmlElement* elem;
	const char* tmp = 0;
	bool notfound = true;

	for (TiXmlElement* partElem = mPartNode->FirstChildElement();
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

void XMLModelDefinitionSerializer::readSubEntities(TiXmlElement* mSubEntNode, PartDefinition* def)
{

	const char* tmp = 0;
	bool notfound = true;

	for (TiXmlElement* seElem = mSubEntNode->FirstChildElement();
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

void XMLModelDefinitionSerializer::readActions(ModelDefinitionPtr modelDef, TiXmlElement* mAnimNode)
{
	TiXmlElement* elem;
	const char* tmp = 0;
	bool notfound = true;
	S_LOG_VERBOSE( "Read Actions" );

	for (TiXmlElement* animElem = mAnimNode->FirstChildElement();
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

			elem = animElem->FirstChildElement("sounds");
			if (elem)
				readSounds(elem, actionDef);

			elem = animElem->FirstChildElement("activations");
			if (elem)
				readActivations(elem, actionDef);
		}

	}

	if(notfound)
	{
		S_LOG_VERBOSE( "No actions found." );
	}

}

void XMLModelDefinitionSerializer::readSounds(TiXmlElement* mAnimationsNode, ActionDefinition* action)
{
	const char* tmp = 0;

	for (TiXmlElement* soundElem = mAnimationsNode->FirstChildElement();
            soundElem != 0; soundElem = soundElem->NextSiblingElement())
	{
		tmp = soundElem->Attribute("group");
		if (tmp)
		{
			std::string groupName(tmp);

			unsigned int playOrder = 0;
			tmp = soundElem->Attribute("playOrder");
			if (tmp)
			{
				std::string playO(tmp);
				if (playO == "linear")
					playOrder = 0;
				else
				if (playO == "inverse")
					playOrder = 1;
				else
				if (playO == "random")
					playOrder = 2;
			}

			action->createSoundDefinition(groupName, playOrder);
			S_LOG_VERBOSE( "  Add Sound: " << groupName);
		}
	}
}

void XMLModelDefinitionSerializer::readActivations(TiXmlElement* activationsNode, ActionDefinition* action)
{
	const char* tmp = 0;

	for (TiXmlElement* activationElem = activationsNode->FirstChildElement();
            activationElem != 0; activationElem = activationElem->NextSiblingElement())
	{
		tmp = activationElem->Attribute("type");
		if (tmp)
		{
			std::string typeString(tmp);

			ActivationDefinition::Type type;
			if (typeString == "movement") {
				type = ActivationDefinition::MOVEMENT;
			} else if (typeString == "action") {
				type = ActivationDefinition::ACTION;
			} else if (typeString == "task") {
				type = ActivationDefinition::TASK;
			} else {
				S_LOG_WARNING("No recognized activation type: " << typeString);
				continue;
			}
			std::string trigger = activationElem->GetText();
			action->createActivationDefinition(type, trigger);
			S_LOG_VERBOSE( "  Add activation: " << typeString << " : " << trigger);
		}
	}
}

void XMLModelDefinitionSerializer::readAnimations(TiXmlElement* mAnimationsNode, ActionDefinition* action)
{
	const char* tmp = 0;
	bool nopartfound = true;


	for (TiXmlElement* animElem = mAnimationsNode->FirstChildElement();
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

void XMLModelDefinitionSerializer::readAnimationParts(TiXmlElement* mAnimPartNode, AnimationDefinition* animDef)
{
	const char* tmp = 0;
	bool nopartfound = true;

	for (TiXmlElement* apElem = mAnimPartNode->FirstChildElement();
            apElem != 0; apElem = apElem->NextSiblingElement())
	{
		std::string name;
		nopartfound = false;

		// name
		tmp =  apElem->Attribute("name");
		if (tmp) {
			name = tmp;
			S_LOG_VERBOSE( "  Add animation  : "+ name );
		}

		AnimationPartDefinition* animPartDef = animDef->createAnimationPartDefinition(name);

		TiXmlElement* elem = apElem->FirstChildElement("bonegrouprefs");
		if (elem) {
			for (TiXmlElement* boneGroupRefElem = elem->FirstChildElement();
					boneGroupRefElem != 0; boneGroupRefElem = boneGroupRefElem->NextSiblingElement())
			{
				tmp = boneGroupRefElem->Attribute("name");
				if (tmp) {
					BoneGroupRefDefinition boneGroupRef;
					boneGroupRef.Name = tmp;
					tmp = boneGroupRefElem->Attribute("weight");
					if (tmp) {
						boneGroupRef.Weight = Ogre::StringConverter::parseReal(tmp);
					} else {
						boneGroupRef.Weight = 1.0f;
					}
					animPartDef->BoneGroupRefs.push_back(boneGroupRef);
				}
			}
		}


	}

	if(nopartfound)
	{
		S_LOG_VERBOSE( "   No anim parts found." );
	}
}


void XMLModelDefinitionSerializer::readAttachPoints(ModelDefinitionPtr modelDef, TiXmlElement* mAnimPartNode)
{
	AttachPointDefinitionStore & attachPoints = modelDef->mAttachPoints;

	const char* tmp = 0;

	for (TiXmlElement* apElem = mAnimPartNode->FirstChildElement();
            apElem != 0; apElem = apElem->NextSiblingElement())
	{
		AttachPointDefinition attachPointDef;

		// name
		tmp =  apElem->Attribute("name");
		if (tmp)
			attachPointDef.Name = tmp;
		S_LOG_VERBOSE( "  Add attachpoint  : "+ attachPointDef.Name );

		// bone
		tmp =  apElem->Attribute("bone");
		if (tmp)
			attachPointDef.BoneName = tmp;

		// pose
		tmp =  apElem->Attribute("pose");
		if (tmp)
			attachPointDef.Pose = tmp;

		TiXmlElement* elem = apElem->FirstChildElement("rotation");
		if (elem)
		{
			attachPointDef.Rotation = XMLHelper::fillQuaternionFromElement(elem);
		} else {
			attachPointDef.Rotation = Ogre::Quaternion::IDENTITY;
		}

		TiXmlElement* tranelem = apElem->FirstChildElement("translation");
		if (tranelem)
		{
			attachPointDef.Translation = XMLHelper::fillVector3FromElement(tranelem);
		} else {
			attachPointDef.Translation = Ogre::Vector3::ZERO;
		}

		attachPoints.push_back(attachPointDef);
	}

}

void  XMLModelDefinitionSerializer::readParticleSystems(ModelDefinitionPtr modelDef, TiXmlElement* mParticleSystemsNode)
{
	TiXmlElement* elem;
	ModelDefinition::ParticleSystemSet& particleSystems = modelDef->mParticleSystems;

	const char* tmp = 0;

	for (TiXmlElement* apElem = mParticleSystemsNode->FirstChildElement();
            apElem != 0; apElem = apElem->NextSiblingElement())
	{
		ModelDefinition::ParticleSystemDefinition def;

		// name
		tmp =  apElem->Attribute("script");
		if (tmp)
			def.Script = tmp;
		S_LOG_VERBOSE( "  Add particlescript  : "+ def.Script );

		elem = apElem->FirstChildElement("bindings");
		if (elem)
			readParticleSystemsBindings(def, elem);

		elem = apElem->FirstChildElement("direction");
		if (elem) {
			def.Direction = XMLHelper::fillVector3FromElement(elem);
		} else {
			def.Direction = Ogre::Vector3(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN());
		}


		particleSystems.push_back(def);
	}
}

void XMLModelDefinitionSerializer::readParticleSystemsBindings(ModelDefinition::ParticleSystemDefinition& def, TiXmlElement* mParticleSystemsNode)
{
	const char* tmp = 0;
// 	bool nopartfound = true;

	for (TiXmlElement* apElem = mParticleSystemsNode->FirstChildElement();
            apElem != 0; apElem = apElem->NextSiblingElement())
	{
		ModelDefinition::BindingDefinition binding;

		// emittervar
		tmp =  apElem->Attribute("emittervar");
		if (tmp)
			binding.EmitterVar = tmp;
		else
			continue;

		// atlasattribute
		tmp =  apElem->Attribute("atlasattribute");
		if (tmp)
			binding.AtlasAttribute = tmp;
		else
			continue;

		S_LOG_VERBOSE( "  Add binding between "<< binding.EmitterVar << " and " << binding.AtlasAttribute << "." );


		def.Bindings.push_back(binding);
	}

}

void XMLModelDefinitionSerializer::readViews(ModelDefinitionPtr modelDef, TiXmlElement* viewsNode)
{
	TiXmlElement* elem;

	const char* tmp = 0;

	for (TiXmlElement* viewElem = viewsNode->FirstChildElement();
            viewElem != 0; viewElem = viewElem->NextSiblingElement())
	{

		// name
		tmp =  viewElem->Attribute("name");
		if (tmp) {
			std::string name(tmp);

			ViewDefinition* def = modelDef->createViewDefinition(name);

			S_LOG_VERBOSE( " Add View  : "+ def->Name );

			elem = viewElem->FirstChildElement("rotation");
			if (elem) {
				def->Rotation = XMLHelper::fillQuaternionFromElement(elem);
			} else {
				def->Rotation = Ogre::Quaternion::IDENTITY;
			}

			elem = viewElem->FirstChildElement("distance");
			if (elem) {
				def->Distance = Ogre::StringConverter::parseReal(elem->GetText());
			} else {
				def->Distance = 0;
			}

		}
	}
}

void XMLModelDefinitionSerializer::readLights(ModelDefinitionPtr modelDef, TiXmlElement* mLightsNode)
{
	TiXmlElement* elem;
	ModelDefinition::LightSet& lights = modelDef->mLights;

	const char* tmp = 0;

	for (TiXmlElement* lElem = mLightsNode->FirstChildElement();
            lElem != 0; lElem = lElem->NextSiblingElement())
	{
		ModelDefinition::LightDefinition def;

		def.type = Ogre::Light::LT_POINT;

		tmp = lElem->Attribute("type");
		if (tmp)
		{
			std::string type = tmp;
			if (type == "point")
			{
				def.type = Ogre::Light::LT_POINT;
			}
			else if (type == "directional")
			{
				def.type = Ogre::Light::LT_DIRECTIONAL;
			}
			else if (type == "spotlight")
			{
				def.type = Ogre::Light::LT_SPOTLIGHT;
			}
		}

		Ogre::Real r=1.0f, g=1.0f, b=1.0f;

		elem = lElem->FirstChildElement("diffusecolour");
		if (elem)
		{
			if (elem->Attribute("r")) {
				r = atof(elem->Attribute("r"));
			}
			if (elem->Attribute("g")) {
				g = atof(elem->Attribute("g"));
			}
			if (elem->Attribute("b")) {
				b = atof(elem->Attribute("b"));
			}
		}
		def.diffuseColour = Ogre::ColourValue(r, g, b);

		elem = lElem->FirstChildElement("specularcolour");
		if (elem)
		{
			if (elem->Attribute("r")) {
				r = atof(elem->Attribute("r"));
			}
			if (elem->Attribute("g")) {
				g = atof(elem->Attribute("g"));
			}
			if (elem->Attribute("b")) {
				b = atof(elem->Attribute("b"));
			}
			def.specularColour = Ogre::ColourValue(r, g, b);
		}
		else
		{
			def.specularColour = def.diffuseColour;
		}

		elem = lElem->FirstChildElement("attenuation");
		def.range = 100000.0;
		def.constant = 1.0;
		def.linear = 0.0;
		def.quadratic = 0.0;
		if (elem)
		{
			if (elem->Attribute("range")) {
				def.range = atof(elem->Attribute("range"));
			}
			if (elem->Attribute("constant")) {
				def.constant = atof(elem->Attribute("constant"));
			}
			if (elem->Attribute("linear")) {
				def.linear = atof(elem->Attribute("linear"));
			}
			if (elem->Attribute("quadratic")) {
				def.quadratic = atof(elem->Attribute("quadratic"));
			}
		}

		elem = lElem->FirstChildElement("position");
		if (elem)
		{
			def.position = XMLHelper::fillVector3FromElement(elem);
		}
		else
		{
			def.position = Ogre::Vector3::ZERO;
		}

		S_LOG_VERBOSE( "  Add light");

		lights.push_back(def);
	}
}

void XMLModelDefinitionSerializer::readBoneGroups(ModelDefinitionPtr modelDef, TiXmlElement* boneGroupsNode)
{
	TiXmlElement* elem;

	const char* tmp = 0;

	for (TiXmlElement* boneGroupElem = boneGroupsNode->FirstChildElement();
			boneGroupElem != 0; boneGroupElem = boneGroupElem->NextSiblingElement())
	{

		// name
		tmp = boneGroupElem->Attribute("name");
		if (tmp) {
			std::string name(tmp);

			BoneGroupDefinition* def = modelDef->createBoneGroupDefinition(name);

			S_LOG_VERBOSE( " Add Bone Group  : "+ def->Name );

			elem = boneGroupElem->FirstChildElement("bones");
			if (elem) {
				for (TiXmlElement* boneElem = elem->FirstChildElement();
						boneElem != 0; boneElem = boneElem->NextSiblingElement())
				{
					const char* text = boneElem->Attribute("index");
					if (text) {
						std::istringstream stream(text);
						size_t index;
						stream >> index;
						def->Bones.push_back(index);
					}
				}
			}
		}
	}
}

void XMLModelDefinitionSerializer::readPoses(ModelDefinitionPtr modelDef, TiXmlElement* mNode)
{
	PoseDefinitionStore& poses = modelDef->mPoseDefinitions;

	const char* tmp = 0;

	for (TiXmlElement* apElem = mNode->FirstChildElement(); apElem != 0; apElem = apElem->NextSiblingElement()) {
		PoseDefinition definition;

		// name
		tmp = apElem->Attribute("name");
		if (!tmp) {
			S_LOG_WARNING("Read pose definition with no name; skipping it.");
			continue;
		}
		std::string name(tmp);
		S_LOG_VERBOSE( "  Add pose  : " + name);

		tmp = apElem->Attribute("ignoreEntityData");
		definition.IgnoreEntityData = false;
		if (tmp) {
			if (std::string(tmp) == "true") {
				definition.IgnoreEntityData = true;
			}
		}


		TiXmlElement* elem = apElem->FirstChildElement("rotate");
		if (elem) {
			definition.Rotate = XMLHelper::fillQuaternionFromElement(elem);
		} else {
			definition.Rotate = Ogre::Quaternion::IDENTITY;
		}

		elem = apElem->FirstChildElement("translate");
		if (elem) {
			definition.Translate = XMLHelper::fillVector3FromElement(elem);
		} else {
			definition.Translate = Ogre::Vector3::ZERO;
		}

		poses.insert(std::make_pair(name, definition));
	}

}




bool XMLModelDefinitionSerializer::exportScript(ModelDefinitionPtr modelDef, const std::string& directory, const std::string& filename)
{
	if (filename == "") {
		return false;
	}

	TiXmlDocument xmlDoc;

	try
	{

		if (!oslink::directory(directory).isExisting()) {
			S_LOG_INFO("Creating directory " << directory);
			oslink::directory::mkdir(directory.c_str());
		}

		TiXmlElement elem("models");
		TiXmlElement modelElem("model");
		modelElem.SetAttribute("name", modelDef->getName().c_str());

		std::string useScaleOf;
		switch (modelDef->getUseScaleOf()) {
			case ModelDefinition::UseScaleOf::MODEL_ALL:
				useScaleOf = "all";
				break;
			case ModelDefinition::UseScaleOf::MODEL_DEPTH:
				useScaleOf = "depth";
				break;
			case ModelDefinition::UseScaleOf::MODEL_HEIGHT:
				useScaleOf = "height";
				break;
			case ModelDefinition::UseScaleOf::MODEL_NONE:
				useScaleOf = "none";
				break;
			case ModelDefinition::UseScaleOf::MODEL_WIDTH:
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
			TiXmlElement contentOffset("contentoffset");
			XMLHelper::fillElementFromVector3(contentOffset, modelDef->getContentOffset());
			modelElem.InsertEndChild(contentOffset);
		}

		const RenderingDefinition* renderingDef = modelDef->getRenderingDefinition();
		if (renderingDef) {
			TiXmlElement rendering("rendering");
			rendering.SetAttribute("scheme", renderingDef->getScheme().c_str());
			for (StringParamStore::const_iterator I  = renderingDef->getParameters().begin();
					I != renderingDef->getParameters().end(); ++I)
			{
				TiXmlElement param("param");
				param.SetAttribute("key", I->first.c_str());
				param.SetValue(I->second.c_str());
				rendering.InsertEndChild(param);
			}
			modelElem.InsertEndChild(rendering);
		}


		TiXmlElement translate("translate");
		XMLHelper::fillElementFromVector3(translate, modelDef->getTranslate());
		modelElem.InsertEndChild(translate);

		TiXmlElement rotation("rotation");
		XMLHelper::fillElementFromQuaternion(rotation, modelDef->getRotation());
		modelElem.InsertEndChild(rotation);

		modelElem.SetAttribute("icon", modelDef->getIconPath().c_str());

		if (modelDef->getRenderingDefinition()) {
			TiXmlElement rendering("rendering");
			rendering.SetAttribute("scheme", modelDef->getRenderingDefinition()->getScheme().c_str());
			for (StringParamStore::const_iterator I = modelDef->getRenderingDefinition()->getParameters().begin(); I != modelDef->getRenderingDefinition()->getParameters().end(); ++I) {
				TiXmlElement param("param");
				param.SetAttribute("key", I->first.c_str());
				param.SetValue(I->second.c_str());
			}
		}

		//start with submodels
		exportSubModels(modelDef, modelElem);

		//now do actions
		exportActions(modelDef, modelElem);

		exportAttachPoints(modelDef, modelElem);

		exportViews(modelDef, modelElem);

		exportLights(modelDef, modelElem);

		exportParticleSystems(modelDef, modelElem);

		exportBoneGroups(modelDef, modelElem);

		exportPoses(modelDef, modelElem);

		elem.InsertEndChild(modelElem);

		xmlDoc.InsertEndChild(elem);
		xmlDoc.SaveFile((directory + filename).c_str());
		S_LOG_INFO("Saved file " << (directory + filename));
		return true;
	}
	catch (...)
	{
		S_LOG_FAILURE("An error occurred saving the modeldefinition for "<< modelDef->getName() << "." );
		return false;
	}


}

void XMLModelDefinitionSerializer::exportViews(ModelDefinitionPtr modelDef, TiXmlElement& modelElem)
{
	TiXmlElement viewsElem("views");

	for (ViewDefinitionStore::const_iterator I = modelDef->getViewDefinitions().begin(); I != modelDef->getViewDefinitions().end(); ++I) {
		TiXmlElement viewElem("view");
		viewElem.SetAttribute("name", I->second->Name.c_str());

		TiXmlElement distanceElem("distance");
		std::stringstream ss;
		ss << I->second->Distance;
		distanceElem.InsertEndChild(TiXmlText(ss.str().c_str()));
		viewElem.InsertEndChild(distanceElem);

		TiXmlElement rotation("rotation");
		XMLHelper::fillElementFromQuaternion(rotation, I->second->Rotation);
		viewElem.InsertEndChild(rotation);

		viewsElem.InsertEndChild(viewElem);
	}
	modelElem.InsertEndChild(viewsElem);
}

void XMLModelDefinitionSerializer::exportActions(ModelDefinitionPtr modelDef, TiXmlElement& modelElem)
{
	TiXmlElement actionsElem("actions");

	for (ActionDefinitionsStore::const_iterator I = modelDef->getActionDefinitions().begin(); I != modelDef->getActionDefinitions().end(); ++I) {
		TiXmlElement actionElem("action");
		actionElem.SetAttribute("name", (*I)->getName().c_str());
		actionElem.SetDoubleAttribute("speed", (*I)->getAnimationSpeed());


		TiXmlElement activationsElem("activations");
		for (auto& activationDef : (*I)->getActivationDefinitions()) {
			TiXmlElement activationElem("activation");
			std::string type;
			switch (activationDef.type) {
			case ActivationDefinition::MOVEMENT:
				type = "movement";
				break;
			case ActivationDefinition::ACTION:
				type = "action";
				break;
			case ActivationDefinition::TASK:
				type = "task";
				break;
			}
			activationElem.SetAttribute("type", type);
			activationElem.InsertEndChild(TiXmlText(activationDef.trigger));
			activationsElem.InsertEndChild(activationElem);
		}
		actionElem.InsertEndChild(activationsElem);

		if ((*I)->getAnimationDefinitions().size() > 0) {
			TiXmlElement animationsElem("animations");
			for (AnimationDefinitionsStore::const_iterator J = (*I)->getAnimationDefinitions().begin(); J != (*I)->getAnimationDefinitions().end(); ++J) {
				TiXmlElement animationElem("animation");
				animationElem.SetAttribute("iterations", (*J)->getIterations());

				for (AnimationPartDefinitionsStore::const_iterator K = (*J)->getAnimationPartDefinitions().begin(); K != (*J)->getAnimationPartDefinitions().end(); ++K) {
					TiXmlElement animationPartElem("animationpart");
					animationPartElem.SetAttribute("name", (*K)->Name.c_str());
					for (std::vector<BoneGroupRefDefinition>::const_iterator L = (*K)->BoneGroupRefs.begin(); L != (*K)->BoneGroupRefs.end(); ++L) {
						TiXmlElement boneGroupRefElem("bonegroupref");
						boneGroupRefElem.SetAttribute("name", L->Name.c_str());
						if (L->Weight != 1.0f) {
							boneGroupRefElem.SetAttribute("weight", L->Weight);
						}
						animationPartElem.InsertEndChild(boneGroupRefElem);
					}
					animationElem.InsertEndChild(animationPartElem);
				}

				animationsElem.InsertEndChild(animationElem);
			}
			actionElem.InsertEndChild(animationsElem);
		}

		//for now, only allow one sound
		if ((*I)->getSoundDefinitions().size() > 0)
		{
			TiXmlElement soundsElem("sounds");

			for (SoundDefinitionsStore::const_iterator J = (*I)->getSoundDefinitions().begin(); J != (*I)->getSoundDefinitions().end(); ++J) {
				TiXmlElement soundElem("sound");
				soundElem.SetAttribute("groupName", (*J)->groupName);
				soundElem.SetAttribute("playOrder", (*J)->playOrder);
				soundsElem.InsertEndChild(soundElem);
			}
		}
		actionsElem.InsertEndChild(actionElem);
	}
	modelElem.InsertEndChild(actionsElem);
}

void XMLModelDefinitionSerializer::exportSubModels(ModelDefinitionPtr modelDef, TiXmlElement& modelElem)
{
	TiXmlElement submodelsElem("submodels");

	for (SubModelDefinitionsStore::const_iterator I = modelDef->getSubModelDefinitions().begin(); I != modelDef->getSubModelDefinitions().end(); ++I) {
		TiXmlElement submodelElem("submodel");
		submodelElem.SetAttribute("mesh", (*I)->getMeshName().c_str());
		TiXmlElement partsElem("parts");

		for (PartDefinitionsStore::const_iterator J = (*I)->getPartDefinitions().begin(); J != (*I)->getPartDefinitions().end(); ++J) {
			TiXmlElement partElem("part");
			partElem.SetAttribute("name", (*J)->getName().c_str());
			if ((*J)->getGroup() != "") {
				partElem.SetAttribute("group", (*J)->getGroup().c_str());
			}
			partElem.SetAttribute("show", (*J)->getShow() ? "true" : "false");

			if ((*J)->getSubEntityDefinitions().size() > 0) {
				TiXmlElement subentitiesElem("subentities");
				for (SubEntityDefinitionsStore::const_iterator K = (*J)->getSubEntityDefinitions().begin(); K != (*J)->getSubEntityDefinitions().end(); ++K) {
					TiXmlElement subentityElem("subentity");
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

void XMLModelDefinitionSerializer::exportAttachPoints(ModelDefinitionPtr modelDef, TiXmlElement& modelElem)
{
	TiXmlElement attachpointsElem("attachpoints");

	for (AttachPointDefinitionStore::const_iterator I = modelDef->getAttachPointsDefinitions().begin(); I != modelDef->getAttachPointsDefinitions().end(); ++I) {
		TiXmlElement attachpointElem("attachpoint");
		attachpointElem.SetAttribute("name", I->Name.c_str());
		attachpointElem.SetAttribute("bone", I->BoneName.c_str());
		if (I->Pose != "") {
			attachpointElem.SetAttribute("pose", I->Pose.c_str());
		}
		TiXmlElement rotationElem("rotation");
		XMLHelper::fillElementFromQuaternion(rotationElem, I->Rotation);
		attachpointElem.InsertEndChild(rotationElem);
		TiXmlElement translationElem("translation");
		XMLHelper::fillElementFromVector3(translationElem, I->Translation);
		attachpointElem.InsertEndChild(translationElem);

		attachpointsElem.InsertEndChild(attachpointElem);
	}
	modelElem.InsertEndChild(attachpointsElem);
}

void XMLModelDefinitionSerializer::exportLights(ModelDefinitionPtr modelDef, TiXmlElement& modelElem)
{
	TiXmlElement lightsElem("lights");

	for (ModelDefinition::LightSet::const_iterator I = modelDef->mLights.begin(); I != modelDef->mLights.end(); ++I) {
		ModelDefinition::LightDefinition def(*I);
		TiXmlElement lightElem("light");
		std::string type;
		if (def.type == Ogre::Light::LT_POINT) {
			type = "point";
		} else if (def.type == Ogre::Light::LT_DIRECTIONAL) {
			type = "directional";
		} else if (def.type == Ogre::Light::LT_SPOTLIGHT) {
			type = "spotlight";
		}
		lightElem.SetAttribute("type", type);

		TiXmlElement diffuseElem("diffusecolour");
		diffuseElem.SetAttribute("r", def.diffuseColour.r);
		diffuseElem.SetAttribute("g", def.diffuseColour.g);
		diffuseElem.SetAttribute("b", def.diffuseColour.b);
		lightElem.InsertEndChild(diffuseElem);

		TiXmlElement specularElem("specularcolour");
		specularElem.SetAttribute("r", def.specularColour.r);
		specularElem.SetAttribute("g", def.specularColour.g);
		specularElem.SetAttribute("b", def.specularColour.b);
		lightElem.InsertEndChild(specularElem);

		TiXmlElement attenuationElem("attenuation");
		attenuationElem.SetDoubleAttribute("range", def.range);
		attenuationElem.SetDoubleAttribute("constant", def.constant);
		attenuationElem.SetDoubleAttribute("linear", def.linear);
		attenuationElem.SetDoubleAttribute("quadratic", def.quadratic);
		lightElem.InsertEndChild(attenuationElem);

		TiXmlElement posElem("position");
		XMLHelper::fillElementFromVector3(posElem, def.position);
		lightElem.InsertEndChild(posElem);

		lightsElem.InsertEndChild(lightElem);
	}
	modelElem.InsertEndChild(lightsElem);
}

void XMLModelDefinitionSerializer::exportPoses(ModelDefinitionPtr modelDef, TiXmlElement& modelElem)
{
	if (modelDef->mPoseDefinitions.size()) {
		TiXmlElement elem("poses");

		for (PoseDefinitionStore::const_iterator I = modelDef->mPoseDefinitions.begin(); I != modelDef->mPoseDefinitions.end(); ++I) {
			TiXmlElement poseElem("pose");
			poseElem.SetAttribute("name", I->first.c_str());
			if (I->second.IgnoreEntityData) {
				poseElem.SetAttribute("ignoreEntityData", "true");
			}
			if (!I->second.Translate.isNaN()) {
				TiXmlElement translateElem("translate");
				XMLHelper::fillElementFromVector3(translateElem, I->second.Translate);
				poseElem.InsertEndChild(translateElem);
			}
			if (!I->second.Rotate.isNaN()) {
				TiXmlElement rotateElem("rotate");
				XMLHelper::fillElementFromQuaternion(rotateElem, I->second.Rotate);
				poseElem.InsertEndChild(rotateElem);
			}

			elem.InsertEndChild(poseElem);
		}
		modelElem.InsertEndChild(elem);
	}
}

void XMLModelDefinitionSerializer::exportParticleSystems(ModelDefinitionPtr modelDef, TiXmlElement& modelElem)
{
	if (modelDef->mParticleSystems.size()) {
		TiXmlElement particleSystemsElem("particlesystems");

		for (ModelDefinition::ParticleSystemSet::const_iterator I = modelDef->mParticleSystems.begin(); I != modelDef->mParticleSystems.end(); ++I) {
			TiXmlElement particleSystemElem("particlesystem");
			particleSystemElem.SetAttribute("script", I->Script.c_str());
			if (!I->Direction.isNaN()) {
				TiXmlElement directionElem("direction");
				XMLHelper::fillElementFromVector3(directionElem, I->Direction);
				particleSystemElem.InsertEndChild(directionElem);
			}
			if (I->Bindings.size()) {
				TiXmlElement bindingsElem("bindings");

				for (ModelDefinition::BindingSet::const_iterator J = I->Bindings.begin(); J != I->Bindings.end(); ++J) {
					TiXmlElement bindingElem("binding");
					bindingsElem.SetAttribute("emittervar", J->EmitterVar);
					bindingsElem.SetAttribute("atlasattribute", J->AtlasAttribute);
					particleSystemElem.InsertEndChild(bindingsElem);
				}
			}
			particleSystemsElem.InsertEndChild(particleSystemElem);
		}
		modelElem.InsertEndChild(particleSystemsElem);
	}
}

void XMLModelDefinitionSerializer::exportBoneGroups(ModelDefinitionPtr modelDef, TiXmlElement& modelElem)
{
	TiXmlElement boneGroupsElem("bonegroups");

	for (BoneGroupDefinitionStore::const_iterator I = modelDef->getBoneGroupDefinitions().begin(); I != modelDef->getBoneGroupDefinitions().end(); ++I) {
		TiXmlElement boneGroupElem("bonegroup");
		boneGroupElem.SetAttribute("name", I->second->Name.c_str());

		TiXmlElement bonesElem("bones");
		for (std::vector<size_t>::const_iterator J = I->second->Bones.begin(); J != I->second->Bones.end(); ++J) {
			TiXmlElement boneElem("bone");
			std::stringstream ss;
			ss << *J;
			boneElem.SetValue(ss.str().c_str());
			bonesElem.InsertEndChild(boneElem);
		}
		boneGroupElem.InsertEndChild(bonesElem);

		boneGroupsElem.InsertEndChild(boneGroupElem);
	}
	modelElem.InsertEndChild(boneGroupsElem);
}


} //end namespace
}
}
