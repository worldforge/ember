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
	along with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
-------------------------------------------------------------------------------
*/
// Includes
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "XMLModelDefinitionSerializer.h"
#include "Model.h"
#include "components/ogre/XMLHelper.h"

#ifdef WIN32
#include <tchar.h>
#include <io.h> // for _access, Win32 version of stat()
#include <direct.h> // for _mkdir
//	#include <sys/stat.h>


	//#include <iostream>
	//#include <fstream>
	//#include <ostream>
#endif

#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>

namespace {
bool shouldExport(const Ogre::Vector3& vector) {
	return !vector.isNaN() && vector != Ogre::Vector3::ZERO;
}

bool shouldExport(const Ogre::Quaternion& quaternion) {
	return !quaternion.isNaN() && quaternion != Ogre::Quaternion::IDENTITY;
}

bool shouldExport(const std::string& string) {
	return !string.empty();
}
}

namespace Ember {
namespace OgreView {
namespace Model {


ModelDefinitionPtr XMLModelDefinitionSerializer::parseDocument(TiXmlDocument& xmlDoc, const std::string& origin) {
	TiXmlElement* rootElem = xmlDoc.RootElement();
	if (rootElem) {

		if (rootElem->ValueStr() == "model") {
			auto& name = origin;
			try {
				auto modelDef = std::make_shared<ModelDefinition>();
				if (modelDef) {
					readModel(modelDef, rootElem);
					modelDef->setValid(true);
					modelDef->setOrigin(origin);
					return modelDef;
				}
			} catch (const Ogre::Exception& ex) {
				S_LOG_FAILURE("Error when parsing model '" << name << "'." << ex);
			}
		} else {
			S_LOG_FAILURE("Invalid initial element in model definition '" << origin << "': " << rootElem->ValueStr());
		}
	}
	return ModelDefinitionPtr();
}

ModelDefinitionPtr XMLModelDefinitionSerializer::parseScript(std::istream& stream, const boost::filesystem::path& path) {
	TiXmlDocument xmlDoc;
	XMLHelper xmlHelper;
	if (xmlHelper.Load(xmlDoc, stream, path)) {
		return parseDocument(xmlDoc, path.string());
	}
	return ModelDefinitionPtr();
}

ModelDefinitionPtr XMLModelDefinitionSerializer::parseScript(Ogre::DataStreamPtr& stream) {
	TiXmlDocument xmlDoc;
	XMLHelper xmlHelper;
	if (xmlHelper.Load(xmlDoc, stream)) {
		return parseDocument(xmlDoc, stream->getName());
	}
	return ModelDefinitionPtr();
}

void XMLModelDefinitionSerializer::readModel(const ModelDefinitionPtr& modelDef, TiXmlElement* modelNode) {
	TiXmlElement* elem;
	//root elements
	//scale
	const char* tmp = modelNode->Attribute("scale");
	if (tmp)
		modelDef->mScale = std::stof(tmp);

	//usescaleof
	tmp = modelNode->Attribute("usescaleof");
	if (tmp) {
		std::string useScaleOf(tmp);
		if (useScaleOf == "height") {
			modelDef->mUseScaleOf = ModelDefinition::UseScaleOf::MODEL_HEIGHT;
		} else if (useScaleOf == "width") {
			modelDef->mUseScaleOf = ModelDefinition::UseScaleOf::MODEL_WIDTH;
		} else if (useScaleOf == "depth") {
			modelDef->mUseScaleOf = ModelDefinition::UseScaleOf::MODEL_DEPTH;
		} else if (useScaleOf == "none") {
			modelDef->mUseScaleOf = ModelDefinition::UseScaleOf::MODEL_NONE;
		} else if (useScaleOf == "fit") {
			modelDef->mUseScaleOf = ModelDefinition::UseScaleOf::MODEL_FIT;
		} else if (useScaleOf == "all") {
			//This is also the default
			modelDef->mUseScaleOf = ModelDefinition::UseScaleOf::MODEL_ALL;
		} else {
			S_LOG_WARNING("Unrecognized model scaling directive: " << useScaleOf);
		}
	}

	tmp = modelNode->Attribute("renderingdistance");
	if (tmp) {
		modelDef->setRenderingDistance(std::stof(tmp));
	}

	tmp = modelNode->Attribute("icon");
	if (tmp) {
		modelDef->mIconPath = tmp;
	}

	tmp = modelNode->Attribute("useinstancing");
	if (tmp) {
		modelDef->mUseInstancing = boost::algorithm::to_lower_copy(std::string(tmp)) == "true";
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
	if (elem) {
		modelDef->mTranslate = XMLHelper::fillVector3FromElement(elem);
	}

	elem = modelNode->FirstChildElement("rotation");
	if (elem) {
		modelDef->setRotation(XMLHelper::fillQuaternionFromElement(elem));
	}

	elem = modelNode->FirstChildElement("contentoffset");
	if (elem) {
		Ogre::Vector3 offset = XMLHelper::fillVector3FromElement(elem);
		modelDef->setContentOffset(offset);
	}

	elem = modelNode->FirstChildElement("rendering");
	if (elem) {
		modelDef->mRenderingDef = std::make_unique<RenderingDefinition>();
		tmp = elem->Attribute("scheme");
		if (tmp) {
			modelDef->mRenderingDef->scheme = tmp;
		}
		for (TiXmlElement* paramElem = elem->FirstChildElement("param");
			 paramElem != nullptr; paramElem = paramElem->NextSiblingElement()) {
			tmp = paramElem->Attribute("key");
			if (tmp) {
				modelDef->mRenderingDef->params.insert(StringParamStore::value_type(tmp, paramElem->GetText()));
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


void XMLModelDefinitionSerializer::readSubModels(const ModelDefinitionPtr& modelDef, TiXmlElement* mSubModelNode) {
	S_LOG_VERBOSE("Read Submodels");
	const char* tmp = nullptr;
	TiXmlElement* elem;
	bool notfound = true;

	for (TiXmlElement* smElem = mSubModelNode->FirstChildElement();
		 smElem != nullptr; smElem = smElem->NextSiblingElement()) {
		notfound = false;

		tmp = smElem->Attribute("mesh");
		if (tmp) {
			SubModelDefinition subModelDef{tmp};

			tmp = smElem->Attribute("shadowcaster");
			if (tmp) {
				subModelDef.shadowCaster = boost::algorithm::to_lower_copy(std::string(tmp)) == "true";
			}

			S_LOG_VERBOSE(" Add submodel  : " + subModelDef.meshName);
			try {
				elem = smElem->FirstChildElement("parts");
				if (elem)
					readParts(elem, &subModelDef);

				modelDef->addSubModelDefinition(subModelDef);
			}
			catch (const Ogre::Exception& e) {
				S_LOG_FAILURE("Load error : " << tmp);
			}
		}
	}

	if (notfound) {
		S_LOG_VERBOSE("No submodel found.");
	}
}

void XMLModelDefinitionSerializer::readParts(TiXmlElement* mPartNode, SubModelDefinition* def) {
	TiXmlElement* elem;
	const char* tmp = nullptr;
	bool notfound = true;

	for (TiXmlElement* partElem = mPartNode->FirstChildElement();
		 partElem != nullptr; partElem = partElem->NextSiblingElement()) {

		// name
		tmp = partElem->Attribute("name");
		notfound = false;

		PartDefinition partDef{tmp ? std::string(tmp) : ""};

		S_LOG_VERBOSE("  Add part  : " + partDef.name);

		// show
		tmp = partElem->Attribute("show");
		if (tmp)
			partDef.show = boost::algorithm::to_lower_copy(std::string(tmp)) == "true";

		tmp = partElem->Attribute("group");
		if (tmp)
			partDef.group = tmp;

		elem = partElem->FirstChildElement("subentities");
		if (elem)
			readSubEntities(elem, &partDef);

		def->addPartDefinition(partDef);

	}

	if (notfound) {
		S_LOG_VERBOSE("No part found.");
	}
}

void XMLModelDefinitionSerializer::readSubEntities(TiXmlElement* mSubEntNode, PartDefinition* def) {

	const char* tmp = nullptr;
	bool notfound = true;

	for (TiXmlElement* seElem = mSubEntNode->FirstChildElement();
		 seElem != nullptr; seElem = seElem->NextSiblingElement()) {
		SubEntityDefinition subEntityDef{};
		// name
		tmp = seElem->Attribute("index");
		if (tmp) {
			notfound = false;
			subEntityDef.subEntityIndex = static_cast<unsigned int>(std::strtoul(tmp, nullptr, 10));
			S_LOG_VERBOSE("   Add sub entity with index: " << subEntityDef.subEntityIndex);
		} else {
			tmp = seElem->Attribute("name");
			if (tmp) {
				notfound = false;
				subEntityDef.subEntityName = tmp;
				S_LOG_VERBOSE("   Add sub entity: " << subEntityDef.subEntityName);
			}
		}
		if (!notfound) {
			//material
			tmp = seElem->Attribute("material");
			if (tmp) {
				subEntityDef.materialName = tmp;
			}
			def->addSubEntityDefinition(std::move(subEntityDef));
		} else {
			S_LOG_FAILURE("A subentity name or index must be specified for each subentity.");
		}
	}

	if (notfound) {
		S_LOG_VERBOSE("No sub entity found.");
	}
}

void XMLModelDefinitionSerializer::readActions(const ModelDefinitionPtr& modelDef, TiXmlElement* mAnimNode) {
	TiXmlElement* elem;
	const char* tmp = nullptr;
	bool notfound = true;
	S_LOG_VERBOSE("Read Actions");

	for (TiXmlElement* animElem = mAnimNode->FirstChildElement();
		 animElem != nullptr; animElem = animElem->NextSiblingElement()) {
		notfound = false;

		tmp = animElem->Attribute("name");
		if (tmp) {
			ActionDefinition actionDef{tmp};
			S_LOG_VERBOSE(" Add action  : " << tmp);

			tmp = animElem->Attribute("speed");
			if (tmp) {
				actionDef.animationSpeed = std::stof(tmp);
			}


			elem = animElem->FirstChildElement("animations");
			if (elem)
				readAnimations(elem, &actionDef);

			elem = animElem->FirstChildElement("sounds");
			if (elem)
				readSounds(elem, &actionDef);

			elem = animElem->FirstChildElement("activations");
			if (elem)
				readActivations(elem, &actionDef);

			modelDef->addActionDefinition(actionDef);
		}

	}

	if (notfound) {
		S_LOG_VERBOSE("No actions found.");
	}

}

void XMLModelDefinitionSerializer::readSounds(TiXmlElement* mAnimationsNode, ActionDefinition* action) {
	const char* tmp = nullptr;

	for (TiXmlElement* soundElem = mAnimationsNode->FirstChildElement();
		 soundElem != nullptr; soundElem = soundElem->NextSiblingElement()) {
		tmp = soundElem->Attribute("group");
		if (tmp) {
			std::string groupName(tmp);

			SoundDefinition def{groupName};
			tmp = soundElem->Attribute("playOrder");
			if (tmp) {
				std::string playO(tmp);
				if (playO == "linear")
					def.playOrder = 0;
				else if (playO == "inverse")
					def.playOrder = 1;
				else if (playO == "random")
					def.playOrder = 2;
			}

			action->addSoundDefinition(std::move(def));
			S_LOG_VERBOSE("  Add Sound: " << groupName);
		}
	}
}

void XMLModelDefinitionSerializer::readActivations(TiXmlElement* activationsNode, ActionDefinition* action) {
	const char* tmp = nullptr;

	for (TiXmlElement* activationElem = activationsNode->FirstChildElement();
		 activationElem != nullptr; activationElem = activationElem->NextSiblingElement()) {
		tmp = activationElem->Attribute("type");
		if (tmp) {
			std::string typeString(tmp);

			ActivationDefinition def{};
			if (typeString == "movement") {
				def.type = ActivationDefinition::MOVEMENT;
			} else if (typeString == "action") {
				def.type = ActivationDefinition::ACTION;
			} else if (typeString == "task") {
				def.type = ActivationDefinition::TASK;
			} else {
				S_LOG_WARNING("No recognized activation type: " << typeString);
				continue;
			}
			def.trigger = activationElem->GetText();
			S_LOG_VERBOSE("  Add activation: " << typeString << " : " << def.trigger);
			action->getActivationDefinitions().emplace_back(std::move(def));
		}
	}
}

void XMLModelDefinitionSerializer::readAnimations(TiXmlElement* mAnimationsNode, ActionDefinition* action) {
	const char* tmp = nullptr;
	for (TiXmlElement* animElem = mAnimationsNode->FirstChildElement();
		 animElem != nullptr; animElem = animElem->NextSiblingElement()) {
		int iterations = 1;

		// name
		tmp = animElem->Attribute("iterations");
		if (tmp) {
			iterations = std::stoi(tmp);
		}
		AnimationDefinition def{iterations};

		readAnimationParts(animElem, &def);
		action->getAnimationDefinitions().emplace_back(std::move(def));
	}


}

void XMLModelDefinitionSerializer::readAnimationParts(TiXmlElement* mAnimPartNode, AnimationDefinition* animDef) {
	const char* tmp = nullptr;

	for (TiXmlElement* apElem = mAnimPartNode->FirstChildElement();
		 apElem != nullptr; apElem = apElem->NextSiblingElement()) {
		std::string name;

		// name
		tmp = apElem->Attribute("name");
		if (tmp) {
			name = tmp;
			S_LOG_VERBOSE("  Add animation  : " + name);
		}

		AnimationPartDefinition animPartDef{name};

		TiXmlElement* elem = apElem->FirstChildElement("bonegrouprefs");
		if (elem) {
			for (TiXmlElement* boneGroupRefElem = elem->FirstChildElement();
				 boneGroupRefElem != nullptr; boneGroupRefElem = boneGroupRefElem->NextSiblingElement()) {
				tmp = boneGroupRefElem->Attribute("name");
				if (tmp) {
					BoneGroupRefDefinition boneGroupRef;
					boneGroupRef.Name = tmp;
					tmp = boneGroupRefElem->Attribute("weight");
					if (tmp) {
						boneGroupRef.Weight = std::stof(tmp);
					} else {
						boneGroupRef.Weight = 1.0f;
					}
					animPartDef.BoneGroupRefs.emplace_back(std::move(boneGroupRef));
				}
			}
		}
		animDef->getAnimationPartDefinitions().emplace_back(std::move(animPartDef));
	}

}


void XMLModelDefinitionSerializer::readAttachPoints(const ModelDefinitionPtr& modelDef, TiXmlElement* mAnimPartNode) {
	AttachPointDefinitionStore& attachPoints = modelDef->mAttachPoints;

	const char* tmp = nullptr;

	for (TiXmlElement* apElem = mAnimPartNode->FirstChildElement();
		 apElem != nullptr; apElem = apElem->NextSiblingElement()) {
		AttachPointDefinition attachPointDef;

		// name
		tmp = apElem->Attribute("name");
		if (tmp)
			attachPointDef.Name = tmp;
		S_LOG_VERBOSE("  Add attachpoint  : " + attachPointDef.Name);

		// bone
		tmp = apElem->Attribute("bone");
		if (tmp)
			attachPointDef.BoneName = tmp;

		// pose
		tmp = apElem->Attribute("pose");
		if (tmp)
			attachPointDef.Pose = tmp;

		TiXmlElement* elem = apElem->FirstChildElement("rotation");
		if (elem) {
			attachPointDef.Rotation = XMLHelper::fillQuaternionFromElement(elem);
		} else {
			attachPointDef.Rotation = Ogre::Quaternion::IDENTITY;
		}

		TiXmlElement* tranelem = apElem->FirstChildElement("translation");
		if (tranelem) {
			attachPointDef.Translation = XMLHelper::fillVector3FromElement(tranelem);
		} else {
			attachPointDef.Translation = Ogre::Vector3::ZERO;
		}

		attachPoints.push_back(std::move(attachPointDef));
	}

}

void XMLModelDefinitionSerializer::readParticleSystems(const ModelDefinitionPtr& modelDef, TiXmlElement* mParticleSystemsNode) {
	TiXmlElement* elem;
	ModelDefinition::ParticleSystemSet& particleSystems = modelDef->mParticleSystems;

	const char* tmp = nullptr;

	for (auto apElem = mParticleSystemsNode->FirstChildElement();
		 apElem != nullptr; apElem = apElem->NextSiblingElement()) {
		ModelDefinition::ParticleSystemDefinition def;

		// name
		tmp = apElem->Attribute("script");
		if (tmp) {
			def.Script = tmp;
		}
		S_LOG_VERBOSE("  Add particlescript  : " + def.Script);

		elem = apElem->FirstChildElement("bindings");
		if (elem) {
			readParticleSystemsBindings(def, elem);
		}

		elem = apElem->FirstChildElement("params");
		if (elem) {
			readParticleSystemsParams(def, elem);
		}

		elem = apElem->FirstChildElement("direction");
		if (elem) {
			def.Direction = XMLHelper::fillVector3FromElement(elem);
		} else {
			def.Direction = Ogre::Vector3(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN());
		}


		particleSystems.push_back(std::move(def));
	}
}

void XMLModelDefinitionSerializer::readParticleSystemsBindings(ModelDefinition::ParticleSystemDefinition& def, TiXmlElement* mParticleSystemsNode) {
	const char* tmp = nullptr;
// 	bool nopartfound = true;

	for (TiXmlElement* apElem = mParticleSystemsNode->FirstChildElement();
		 apElem != nullptr; apElem = apElem->NextSiblingElement()) {
		ModelDefinition::BindingDefinition binding;

		// emittervar
		auto emitterVar = apElem->Attribute("emittervar");
		if (emitterVar) {
			auto setting = parseParticleSystemSetting(emitterVar);
			if (!setting) {
				continue;
			}
			binding.EmitterVar = *setting;
		} else {
			continue;
		}

		// atlasattribute
		tmp = apElem->Attribute("atlasattribute");
		if (tmp) {
			binding.AtlasAttribute = tmp;
		} else {
			continue;
		}

		S_LOG_VERBOSE("  Add binding between " << emitterVar << " and " << binding.AtlasAttribute << ".");


		def.Bindings.emplace_back(std::move(binding));
	}

}

void XMLModelDefinitionSerializer::readParticleSystemsParams(ModelDefinition::ParticleSystemDefinition& def, TiXmlElement* mParticleSystemsNode) {

	for (auto apElem = mParticleSystemsNode->FirstChildElement();
		 apElem != nullptr; apElem = apElem->NextSiblingElement()) {

		auto name = apElem->Attribute("emittervar");
		if (!name) {
			continue;
		}

		auto setting = parseParticleSystemSetting(name);
		if (!setting) {
			continue;
		}

		auto text = apElem->GetText();
		if (!text) {
			continue;
		}
		def.Params.emplace_back(*setting, std::stod(text));
	}

}

void XMLModelDefinitionSerializer::readViews(const ModelDefinitionPtr& modelDef, TiXmlElement* viewsNode) {
	TiXmlElement* elem;

	const char* tmp = nullptr;

	for (TiXmlElement* viewElem = viewsNode->FirstChildElement();
		 viewElem != nullptr; viewElem = viewElem->NextSiblingElement()) {

		// name
		tmp = viewElem->Attribute("name");
		if (tmp) {
			std::string name(tmp);

			ViewDefinition def{name};

			S_LOG_VERBOSE(" Add View  : " + def.Name);

			elem = viewElem->FirstChildElement("rotation");
			if (elem) {
				def.Rotation = XMLHelper::fillQuaternionFromElement(elem);
			} else {
				def.Rotation = Ogre::Quaternion::IDENTITY;
			}

			elem = viewElem->FirstChildElement("distance");
			if (elem) {
				def.Distance = std::stof(elem->GetText());
			} else {
				def.Distance = 0;
			}
			modelDef->addViewDefinition(name, std::move(def));
		}
	}
}

void XMLModelDefinitionSerializer::readLights(const ModelDefinitionPtr& modelDef, TiXmlElement* mLightsNode) {
	TiXmlElement* elem;
	ModelDefinition::LightSet& lights = modelDef->mLights;

	const char* tmp = nullptr;

	for (TiXmlElement* lElem = mLightsNode->FirstChildElement();
		 lElem != nullptr; lElem = lElem->NextSiblingElement()) {
		ModelDefinition::LightDefinition def;

		def.type = Ogre::Light::LT_POINT;

		tmp = lElem->Attribute("type");
		if (tmp) {
			std::string type = tmp;
			if (type == "point") {
				def.type = Ogre::Light::LT_POINT;
			} else if (type == "directional") {
				def.type = Ogre::Light::LT_DIRECTIONAL;
			} else if (type == "spotlight") {
				def.type = Ogre::Light::LT_SPOTLIGHT;
			}
		}

		Ogre::Real r = 1.0f, g = 1.0f, b = 1.0f;

		elem = lElem->FirstChildElement("diffusecolour");
		if (elem) {
			if (elem->Attribute("r")) {
				r = std::stof(elem->Attribute("r"));
			}
			if (elem->Attribute("g")) {
				g = std::stof(elem->Attribute("g"));
			}
			if (elem->Attribute("b")) {
				b = std::stof(elem->Attribute("b"));
			}
		}
		def.diffuseColour = Ogre::ColourValue(r, g, b);

		elem = lElem->FirstChildElement("specularcolour");
		if (elem) {
			if (elem->Attribute("r")) {
				r = std::stof(elem->Attribute("r"));
			}
			if (elem->Attribute("g")) {
				g = std::stof(elem->Attribute("g"));
			}
			if (elem->Attribute("b")) {
				b = std::stof(elem->Attribute("b"));
			}
			def.specularColour = Ogre::ColourValue(r, g, b);
		} else {
			def.specularColour = def.diffuseColour;
		}

		elem = lElem->FirstChildElement("attenuation");
		def.range = 100000.0;
		def.constant = 1.0;
		def.linear = 0.0;
		def.quadratic = 0.0;
		if (elem) {
			if (elem->Attribute("range")) {
				def.range = std::stof(elem->Attribute("range"));
			}
			if (elem->Attribute("constant")) {
				def.constant = std::stof(elem->Attribute("constant"));
			}
			if (elem->Attribute("linear")) {
				def.linear = std::stof(elem->Attribute("linear"));
			}
			if (elem->Attribute("quadratic")) {
				def.quadratic = std::stof(elem->Attribute("quadratic"));
			}
		}

		elem = lElem->FirstChildElement("position");
		if (elem) {
			def.position = XMLHelper::fillVector3FromElement(elem);
		} else {
			def.position = Ogre::Vector3::ZERO;
		}

		S_LOG_VERBOSE("  Add light");

		lights.emplace_back(def);
	}
}

void XMLModelDefinitionSerializer::readBoneGroups(const ModelDefinitionPtr& modelDef, TiXmlElement* boneGroupsNode) {
	TiXmlElement* elem;

	const char* tmp = nullptr;

	for (TiXmlElement* boneGroupElem = boneGroupsNode->FirstChildElement();
		 boneGroupElem != nullptr; boneGroupElem = boneGroupElem->NextSiblingElement()) {

		// name
		tmp = boneGroupElem->Attribute("name");
		if (tmp) {
			std::string name(tmp);

			BoneGroupDefinition def{name};

			S_LOG_VERBOSE(" Add Bone Group  : " + def.Name);

			elem = boneGroupElem->FirstChildElement("bones");
			if (elem) {
				for (TiXmlElement* boneElem = elem->FirstChildElement();
					 boneElem != nullptr; boneElem = boneElem->NextSiblingElement()) {
					const char* text = boneElem->Attribute("index");
					if (text) {
						std::istringstream stream(text);
						size_t index;
						stream >> index;
						def.Bones.push_back(index);
					}
				}
			}
			modelDef->addBoneGroupDefinition(name, std::move(def));
		}
	}
}

void XMLModelDefinitionSerializer::readPoses(const ModelDefinitionPtr& modelDef, TiXmlElement* mNode) {
	PoseDefinitionStore& poses = modelDef->mPoseDefinitions;

	const char* tmp = nullptr;

	for (TiXmlElement* apElem = mNode->FirstChildElement(); apElem != nullptr; apElem = apElem->NextSiblingElement()) {
		PoseDefinition definition;

		// name
		tmp = apElem->Attribute("name");
		if (!tmp) {
			S_LOG_WARNING("Read pose definition with no name; skipping it.");
			continue;
		}
		std::string name(tmp);
		S_LOG_VERBOSE("  Add pose  : " + name);


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

		poses.emplace(name, std::move(definition));
	}

}


bool XMLModelDefinitionSerializer::exportScript(const ModelDefinitionPtr& modelDef, const boost::filesystem::path& path) {
	if (path.empty()) {
		return false;
	}

	TiXmlDocument xmlDoc;
	TiXmlDeclaration xmlDeclaration("1.0", "UTF-8", "");
	xmlDoc.InsertEndChild(xmlDeclaration);

	try {

		if (!boost::filesystem::exists(path.parent_path())) {
			S_LOG_INFO("Creating directory " << path.parent_path().string());
			boost::filesystem::create_directories(path.parent_path());
		}

		TiXmlElement modelElem("model");

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
			case ModelDefinition::UseScaleOf::MODEL_FIT:
				useScaleOf = "fit";
				break;
		}
		modelElem.SetAttribute("usescaleof", useScaleOf);

		if (!modelDef->mUseInstancing) {
			modelElem.SetAttribute("useinstancing", "false");
		}

		if (modelDef->getRenderingDistance() != 0.0f) {
			modelElem.SetDoubleAttribute("renderingdistance", modelDef->getRenderingDistance());
		}

		if (modelDef->getScale() != 0) {
			modelElem.SetDoubleAttribute("scale", modelDef->getScale());
		}

		if (modelDef->getContentOffset() != Ogre::Vector3::ZERO) {
			TiXmlElement contentOffset("contentoffset");
			XMLHelper::fillElementFromVector3(contentOffset, modelDef->getContentOffset());
			modelElem.InsertEndChild(contentOffset);
		}

		const RenderingDefinition* renderingDef = modelDef->getRenderingDefinition();
		if (renderingDef) {
			TiXmlElement rendering("rendering");
			rendering.SetAttribute("scheme", renderingDef->scheme);
			for (const auto& aParam : renderingDef->params) {
				TiXmlElement param("param");
				param.SetAttribute("key", aParam.first);
				param.SetValue(aParam.second);
				rendering.InsertEndChild(param);
			}
			modelElem.InsertEndChild(rendering);
		}


		if (shouldExport(modelDef->getTranslate())) {
			TiXmlElement translate("translate");
			XMLHelper::fillElementFromVector3(translate, modelDef->getTranslate());
			modelElem.InsertEndChild(translate);
		}

		if (shouldExport(modelDef->getRotation())) {
			TiXmlElement rotation("rotation");
			XMLHelper::fillElementFromQuaternion(rotation, modelDef->getRotation());
			modelElem.InsertEndChild(rotation);
		}

		if (shouldExport(modelDef->getIconPath())) {
			modelElem.SetAttribute("icon", modelDef->getIconPath());
		}

		if (modelDef->getRenderingDefinition()) {
			TiXmlElement rendering("rendering");
			rendering.SetAttribute("scheme", modelDef->getRenderingDefinition()->scheme);
			for (const auto& aParam : modelDef->getRenderingDefinition()->params) {
				TiXmlElement param("param");
				param.SetAttribute("key", aParam.first);
				param.SetValue(aParam.second);
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

		xmlDoc.InsertEndChild(modelElem);

		xmlDoc.SaveFile(path.string());
		S_LOG_INFO("Saved file " << path.string());
		return true;
	}
	catch (...) {
		S_LOG_FAILURE("An error occurred saving the modeldefinition for " << path.filename().string() << ".");
		return false;
	}


}

void XMLModelDefinitionSerializer::exportViews(const ModelDefinitionPtr& modelDef, TiXmlElement& modelElem) {
	if (modelDef->getViewDefinitions().empty()) {
		return;
	}
	TiXmlElement viewsElem("views");

	for (const auto& viewDefinition : modelDef->getViewDefinitions()) {
		TiXmlElement viewElem("view");
		viewElem.SetAttribute("name", viewDefinition.second.Name);

		TiXmlElement distanceElem("distance");
		std::stringstream ss;
		ss << viewDefinition.second.Distance;
		distanceElem.InsertEndChild(TiXmlText(ss.str()));
		viewElem.InsertEndChild(distanceElem);

		if (shouldExport(viewDefinition.second.Rotation)) {
			TiXmlElement rotation("rotation");
			XMLHelper::fillElementFromQuaternion(rotation, viewDefinition.second.Rotation);
			viewElem.InsertEndChild(rotation);
		}

		viewsElem.InsertEndChild(viewElem);
	}
	modelElem.InsertEndChild(viewsElem);
}

void XMLModelDefinitionSerializer::exportActions(const ModelDefinitionPtr& modelDef, TiXmlElement& modelElem) {
	if (modelDef->getActionDefinitions().empty()) {
		return;
	}
	TiXmlElement actionsElem("actions");

	for (const auto& actionDefinition : modelDef->getActionDefinitions()) {
		TiXmlElement actionElem("action");
		actionElem.SetAttribute("name", actionDefinition.name);
		actionElem.SetDoubleAttribute("speed", actionDefinition.animationSpeed);


		TiXmlElement activationsElem("activations");
		for (auto& activationDef : actionDefinition.getActivationDefinitions()) {
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

		if (!actionDefinition.getAnimationDefinitions().empty()) {
			TiXmlElement animationsElem("animations");
			for (const auto& animDef : actionDefinition.getAnimationDefinitions()) {
				TiXmlElement animationElem("animation");
				animationElem.SetAttribute("iterations", animDef.iterations);

				for (const auto& animationPartDefinition : animDef.getAnimationPartDefinitions()) {
					TiXmlElement animationPartElem("animationpart");
					animationPartElem.SetAttribute("name", animationPartDefinition.Name);
					for (auto& boneGroupRef : animationPartDefinition.BoneGroupRefs) {
						TiXmlElement boneGroupRefElem("bonegroupref");
						boneGroupRefElem.SetAttribute("name", boneGroupRef.Name);
						if (boneGroupRef.Weight != 1.0f) {
							boneGroupRefElem.SetAttribute("weight", static_cast<int>(boneGroupRef.Weight));
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
		if (!actionDefinition.getSoundDefinitions().empty()) {
			TiXmlElement soundsElem("sounds");

			for (auto& soundDefinition : actionDefinition.getSoundDefinitions()) {
				TiXmlElement soundElem("sound");
				soundElem.SetAttribute("groupName", soundDefinition.groupName);
				soundElem.SetAttribute("playOrder", (int) soundDefinition.playOrder);
				soundsElem.InsertEndChild(soundElem);
			}
		}
		actionsElem.InsertEndChild(actionElem);
	}
	modelElem.InsertEndChild(actionsElem);
}

void XMLModelDefinitionSerializer::exportSubModels(const ModelDefinitionPtr& modelDef, TiXmlElement& modelElem) {
	if (modelDef->getSubModelDefinitions().empty()) {
		return;
	}

	TiXmlElement submodelsElem("submodels");

	for (const auto& subModelDefinition : modelDef->getSubModelDefinitions()) {
		TiXmlElement submodelElem("submodel");
		submodelElem.SetAttribute("mesh", subModelDefinition.meshName);
		if (!subModelDefinition.shadowCaster) {
			submodelElem.SetAttribute("shadowcaster", "false");
		}

		if (!subModelDefinition.getPartDefinitions().empty()) {
			TiXmlElement partsElem("parts");

			for (const auto& partDefinition : subModelDefinition.getPartDefinitions()) {
				TiXmlElement partElem("part");
				partElem.SetAttribute("name", partDefinition.name);
				if (!partDefinition.group.empty()) {
					partElem.SetAttribute("group", partDefinition.group);
				}
				partElem.SetAttribute("show", partDefinition.show ? "true" : "false");

				if (!partDefinition.getSubEntityDefinitions().empty()) {
					TiXmlElement subentitiesElem("subentities");
					for (const auto& subEntityDefinition : partDefinition.getSubEntityDefinitions()) {
						TiXmlElement subentityElem("subentity");
						if (!subEntityDefinition.subEntityName.empty()) {
							subentityElem.SetAttribute("name", subEntityDefinition.subEntityName);
						} else {
							subentityElem.SetAttribute("index", (int) subEntityDefinition.subEntityIndex);
						}
						if (shouldExport(subEntityDefinition.materialName)) {
							subentityElem.SetAttribute("material", subEntityDefinition.materialName);
						}
						subentitiesElem.InsertEndChild(subentityElem);
					}
					partElem.InsertEndChild(subentitiesElem);
				}
				partsElem.InsertEndChild(partElem);
			}
			submodelElem.InsertEndChild(partsElem);
		}
		submodelsElem.InsertEndChild(submodelElem);
	}
	modelElem.InsertEndChild(submodelsElem);

}

void XMLModelDefinitionSerializer::exportAttachPoints(const ModelDefinitionPtr& modelDef, TiXmlElement& modelElem) {
	if (modelDef->getAttachPointsDefinitions().empty()) {
		return;
	}
	TiXmlElement attachpointsElem("attachpoints");

	for (const auto& attachPointDef : modelDef->getAttachPointsDefinitions()) {
		TiXmlElement attachpointElem("attachpoint");
		attachpointElem.SetAttribute("name", attachPointDef.Name);
		attachpointElem.SetAttribute("bone", attachPointDef.BoneName);
		if (shouldExport(attachPointDef.Pose)) {
			attachpointElem.SetAttribute("pose", attachPointDef.Pose);
		}
		if (shouldExport(attachPointDef.Rotation)) {
			TiXmlElement rotationElem("rotation");
			XMLHelper::fillElementFromQuaternion(rotationElem, attachPointDef.Rotation);
			attachpointElem.InsertEndChild(rotationElem);
		}
		if (shouldExport(attachPointDef.Translation)) {
			TiXmlElement translationElem("translation");
			XMLHelper::fillElementFromVector3(translationElem, attachPointDef.Translation);
			attachpointElem.InsertEndChild(translationElem);
		}
		attachpointsElem.InsertEndChild(attachpointElem);
	}
	modelElem.InsertEndChild(attachpointsElem);
}

void XMLModelDefinitionSerializer::exportLights(const ModelDefinitionPtr& modelDef, TiXmlElement& modelElem) {
	if (modelDef->mLights.empty()) {
		return;
	}
	TiXmlElement lightsElem("lights");

	for (auto lightDefinition : modelDef->mLights) {
		TiXmlElement lightElem("light");
		std::string type;
		if (lightDefinition.type == Ogre::Light::LT_POINT) {
			type = "point";
		} else if (lightDefinition.type == Ogre::Light::LT_DIRECTIONAL) {
			type = "directional";
		} else if (lightDefinition.type == Ogre::Light::LT_SPOTLIGHT) {
			type = "spotlight";
		}
		lightElem.SetAttribute("type", type);

		TiXmlElement diffuseElem("diffusecolour");
		diffuseElem.SetDoubleAttribute("r", lightDefinition.diffuseColour.r);
		diffuseElem.SetDoubleAttribute("g", lightDefinition.diffuseColour.g);
		diffuseElem.SetDoubleAttribute("b", lightDefinition.diffuseColour.b);
		lightElem.InsertEndChild(diffuseElem);

		TiXmlElement specularElem("specularcolour");
		specularElem.SetDoubleAttribute("r", lightDefinition.specularColour.r);
		specularElem.SetDoubleAttribute("g", lightDefinition.specularColour.g);
		specularElem.SetDoubleAttribute("b", lightDefinition.specularColour.b);
		lightElem.InsertEndChild(specularElem);

		TiXmlElement attenuationElem("attenuation");
		attenuationElem.SetDoubleAttribute("range", lightDefinition.range);
		attenuationElem.SetDoubleAttribute("constant", lightDefinition.constant);
		attenuationElem.SetDoubleAttribute("linear", lightDefinition.linear);
		attenuationElem.SetDoubleAttribute("quadratic", lightDefinition.quadratic);
		lightElem.InsertEndChild(attenuationElem);

		TiXmlElement posElem("position");
		XMLHelper::fillElementFromVector3(posElem, lightDefinition.position);
		lightElem.InsertEndChild(posElem);

		lightsElem.InsertEndChild(lightElem);
	}
	modelElem.InsertEndChild(lightsElem);
}

void XMLModelDefinitionSerializer::exportPoses(const ModelDefinitionPtr& modelDef, TiXmlElement& modelElem) {
	if (modelDef->mPoseDefinitions.empty()) {
		return;
	}

	if (!modelDef->mPoseDefinitions.empty()) {
		TiXmlElement elem("poses");

		for (auto& entry :  modelDef->mPoseDefinitions) {
			TiXmlElement poseElem("pose");
			poseElem.SetAttribute("name", entry.first);

			if (shouldExport(entry.second.Translate)) {
				TiXmlElement translateElem("translate");
				XMLHelper::fillElementFromVector3(translateElem, entry.second.Translate);
				poseElem.InsertEndChild(translateElem);
			}
			if (shouldExport(entry.second.Rotate)) {
				TiXmlElement rotateElem("rotate");
				XMLHelper::fillElementFromQuaternion(rotateElem, entry.second.Rotate);
				poseElem.InsertEndChild(rotateElem);
			}

			elem.InsertEndChild(poseElem);
		}
		modelElem.InsertEndChild(elem);
	}
}

void XMLModelDefinitionSerializer::exportParticleSystems(const ModelDefinitionPtr& modelDef, TiXmlElement& modelElem) {
	if (modelDef->mParticleSystems.empty()) {
		return;
	}

	if (!modelDef->mParticleSystems.empty()) {
		TiXmlElement particleSystemsElem("particlesystems");

		for (const auto& particleDef :modelDef->mParticleSystems) {
			TiXmlElement particleSystemElem("particlesystem");
			particleSystemElem.SetAttribute("script", particleDef.Script);
			if (shouldExport(particleDef.Direction)) {
				TiXmlElement directionElem("direction");
				XMLHelper::fillElementFromVector3(directionElem, particleDef.Direction);
				particleSystemElem.InsertEndChild(directionElem);
			}
			if (!particleDef.Bindings.empty()) {
				TiXmlElement bindingsElem("bindings");

				for (const auto& binding : particleDef.Bindings) {
					TiXmlElement bindingElem("binding");
					auto emitterVarName = particleSystemSettingToString(binding.EmitterVar);
					if (!emitterVarName) {
						continue;
					}
					bindingsElem.SetAttribute("emittervar", *emitterVarName);
					bindingsElem.SetAttribute("atlasattribute", binding.AtlasAttribute);
					particleSystemElem.InsertEndChild(bindingsElem);
				}
			}
			if (!particleDef.Params.empty()) {
				TiXmlElement bindingsElem("params");
				for (const auto& param : particleDef.Params) {
					TiXmlElement bindingElem("param");
					auto emitterVarName = particleSystemSettingToString(param.first);
					if (!emitterVarName) {
						continue;
					}
					bindingElem.SetAttribute("emittervar", *emitterVarName);
					bindingElem.SetValue(std::to_string(param.second));
					bindingsElem.InsertEndChild(bindingElem);
				}
				particleSystemElem.InsertEndChild(bindingsElem);

			}
			particleSystemsElem.InsertEndChild(particleSystemElem);
		}
		modelElem.InsertEndChild(particleSystemsElem);
	}
}

void XMLModelDefinitionSerializer::exportBoneGroups(const ModelDefinitionPtr& modelDef, TiXmlElement& modelElem) {
	if (modelDef->getBoneGroupDefinitions().empty()) {
		return;
	}

	TiXmlElement boneGroupsElem("bonegroups");

	for (const auto& entry : modelDef->getBoneGroupDefinitions()) {
		TiXmlElement boneGroupElem("bonegroup");
		boneGroupElem.SetAttribute("name", entry.second.Name);

		TiXmlElement bonesElem("bones");
		for (auto boneIndex : entry.second.Bones) {
			TiXmlElement boneElem("bone");
			std::stringstream ss;
			ss << boneIndex;
			boneElem.SetValue(ss.str());
			bonesElem.InsertEndChild(boneElem);
		}
		boneGroupElem.InsertEndChild(bonesElem);

		boneGroupsElem.InsertEndChild(boneGroupElem);
	}
	modelElem.InsertEndChild(boneGroupsElem);
}

boost::optional<ModelDefinition::ParticleSystemSetting> XMLModelDefinitionSerializer::parseParticleSystemSetting(const std::string& setting) {
	if (setting == "emission_rate") {
		return ModelDefinition::ParticleSystemSetting::EMISSION_RATE;
	} else if (setting == "time_to_live") {
		return ModelDefinition::ParticleSystemSetting::TIME_TO_LIVE;
	}
	return boost::none;
}

boost::optional<std::string> XMLModelDefinitionSerializer::particleSystemSettingToString(ModelDefinition::ParticleSystemSetting setting) {
	if (setting == ModelDefinition::ParticleSystemSetting::TIME_TO_LIVE) {
		return std::string("time_to_live");
	} else if (setting == ModelDefinition::ParticleSystemSetting::EMISSION_RATE) {
		return std::string("emission_rate");
	}
	return boost::none;
}

} //end namespace
}
}
