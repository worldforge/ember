/*
 Copyright (C) 2022 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include "RegisterLua.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/SubModel.h"
#include "components/ogre/model/SubModelPart.h"
#include "components/ogre/model/ModelDefinition.h"
#include "components/ogre/model/ModelDefinitionManager.h"
#include "components/ogre/model/ModelDefinitionAtlasComposer.h"
#include "components/ogre/model/ModelRepresentation.h"

using namespace Ember::OgreView::Model;
using namespace Ember::Lua;

template<>
void registerLua<Model>(sol::table& space) {
	auto animationSet = space.new_usertype<AnimationSet>("AnimationSet", sol::no_constructor);
	animationSet["addTime"] = [](AnimationSet* self, float timeSlice) { self->addTime(timeSlice); };
	animationSet["addAnimation"] = &AnimationSet::addAnimation;
	animationSet["reset"] = &AnimationSet::reset;
	animationSet["setSpeed"] = &AnimationSet::setSpeed;
	animationSet["getSpeed"] = &AnimationSet::getSpeed;

	auto action = space.new_usertype<Action>("Action", sol::no_constructor);
	action["name"] = &Action::name;
	action["animations"] = &Action::animations;

	auto model = space.new_usertype<Model>("Model", sol::no_constructor);
	model["reload"] = &Model::reload;
	model["getAction"] = sol::resolve<Action*(const std::string&)>(&Model::getAction);
	model["showPart"] = &Model::showPart;
	model["hidePart"] = &Model::hidePart;
	model["setVisible"] = &Model::setVisible;
	model["getRotation"] = &Model::getRotation;
	model["getName"] = &Model::getName;
	model["getDefinition"] = &Model::getDefinition;
	model["hasAttachPoint"] = &Model::hasAttachPoint;
	model["hasParticles"] = &Model::hasParticles;
	model["getSubModel"] = &Model::getSubModel;
	model["setDisplaySkeleton"] = &Model::setDisplaySkeleton;
	model["getDisplaySkeleton"] = &Model::getDisplaySkeleton;

	auto viewDefinition = space.new_usertype<ViewDefinition>("ViewDefinition");
	viewDefinition["Name"] = &ViewDefinition::Name;
	viewDefinition["Rotation"] = &ViewDefinition::Rotation;
	viewDefinition["Distance"] = &ViewDefinition::Distance;

	auto poseDefinition = space.new_usertype<PoseDefinition>("PoseDefinition");
	poseDefinition["Rotate"] = &PoseDefinition::Rotate;
	poseDefinition["Translate"] = &PoseDefinition::Translate;

	auto attachPointDefinition = space.new_usertype<AttachPointDefinition>("AttachPointDefinition");
	attachPointDefinition["Name"] = &AttachPointDefinition::Name;
	attachPointDefinition["BoneName"] = &AttachPointDefinition::BoneName;
	attachPointDefinition["Pose"] = &AttachPointDefinition::Pose;
	attachPointDefinition["Rotation"] = &AttachPointDefinition::Rotation;
	attachPointDefinition["Translation"] = &AttachPointDefinition::Translation;

	auto subEntityDefinition = space.new_usertype<SubEntityDefinition>("SubEntityDefinition");
	subEntityDefinition["subEntityName"] = &SubEntityDefinition::subEntityName;
	subEntityDefinition["subEntityIndex"] = &SubEntityDefinition::subEntityIndex;
	subEntityDefinition["materialName"] = &SubEntityDefinition::materialName;

	auto partDefinition = space.new_usertype<PartDefinition>("PartDefinition");
	partDefinition["name"] = &PartDefinition::name;
	partDefinition["show"] = &PartDefinition::show;
	partDefinition["getSubEntityDefinitions"] = &PartDefinition::getSubEntityDefinitions;
	partDefinition["removeSubEntityDefinition"] = &PartDefinition::removeSubEntityDefinition;
	partDefinition["addSubEntityDefinition"] = &PartDefinition::addSubEntityDefinition;

	auto subModelDefinition = space.new_usertype<SubModelDefinition>("SubModelDefinition");
	subModelDefinition["meshName"] = &SubModelDefinition::meshName;
	subModelDefinition["shadowCaster"] = &SubModelDefinition::shadowCaster;
	subModelDefinition["getPartDefinitions"] = &SubModelDefinition::getPartDefinitions;
	subModelDefinition["removePartDefinition"] = &SubModelDefinition::removePartDefinition;
	subModelDefinition["addPartDefinition"] = &SubModelDefinition::addPartDefinition;

	auto animationPartDefinition = space.new_usertype<AnimationPartDefinition>("AnimationPartDefinition");
	animationPartDefinition["Name"] = &AnimationPartDefinition::Name;
	animationPartDefinition["BoneGroupRefs"] = &AnimationPartDefinition::BoneGroupRefs;

	auto boneGroupDefinition = space.new_usertype<BoneGroupDefinition>("BoneGroupDefinition");
	boneGroupDefinition["Name"] = &BoneGroupDefinition::Name;
	boneGroupDefinition["Bones"] = &BoneGroupDefinition::Bones;

	auto soundDefinition = space.new_usertype<SoundDefinition>("SoundDefinition");
	soundDefinition["groupName"] = &SoundDefinition::groupName;
	soundDefinition["playOrder"] = &SoundDefinition::playOrder;

	auto boneGroupRefDefinition = space.new_usertype<BoneGroupRefDefinition>("BoneGroupRefDefinition");
	boneGroupRefDefinition["Name"] = &BoneGroupRefDefinition::Name;
	boneGroupRefDefinition["Weight"] = &BoneGroupRefDefinition::Weight;

	auto animationDefinition = space.new_usertype<AnimationDefinition>("AnimationDefinition");
	animationDefinition["getAnimationPartDefinitions"] = sol::resolve<const std::vector<AnimationPartDefinition>&() const>(&AnimationDefinition::getAnimationPartDefinitions);
	animationDefinition["removeAnimationPartDefinition"] = &AnimationDefinition::removeAnimationPartDefinition;
	animationDefinition["addAnimationPartDefinition"] = &AnimationDefinition::addAnimationPartDefinition;
	animationDefinition["iterations"] = &AnimationDefinition::iterations;

	auto actionDefinition = space.new_usertype<ActionDefinition>("ActionDefinition");
	actionDefinition["getAnimationDefinitions"] = sol::resolve<const std::vector<AnimationDefinition>&() const>(&ActionDefinition::getAnimationDefinitions);
	actionDefinition["removeAnimationDefinition"] = &ActionDefinition::removeAnimationDefinition;
	actionDefinition["addAnimationDefinition"] = &ActionDefinition::addAnimationDefinition;
	actionDefinition["getSoundDefinitions"] = sol::resolve<const std::vector<SoundDefinition>&() const>(&ActionDefinition::getSoundDefinitions);
	actionDefinition["removeSoundDefinition"] = &ActionDefinition::removeSoundDefinition;
	actionDefinition["addSoundDefinition"] = &ActionDefinition::addSoundDefinition;
	actionDefinition["getActivationDefinitions"] = sol::resolve<const std::vector<ActivationDefinition>&() const>(&ActionDefinition::getActivationDefinitions);
	actionDefinition["name"] = &ActionDefinition::name;
	actionDefinition["animationSpeed"] = &ActionDefinition::animationSpeed;

	auto modelDefinition = space.new_usertype<ModelDefinition>("ModelDefinition");
	modelDefinition["getOrigin"] = &ModelDefinition::getOrigin;
	modelDefinition["setOrigin"] = &ModelDefinition::setOrigin;
	modelDefinition["isValid"] = &ModelDefinition::isValid;
	modelDefinition["setValid"] = &ModelDefinition::setValid;
	modelDefinition["getScale"] = &ModelDefinition::getScale;
	modelDefinition["setScale"] = &ModelDefinition::setScale;
	modelDefinition["getUseScaleOf"] = &ModelDefinition::getUseScaleOf;
	modelDefinition["setUseScaleOf"] = &ModelDefinition::setUseScaleOf;
	modelDefinition["getTranslate"] = &ModelDefinition::getTranslate;
	modelDefinition["setTranslate"] = &ModelDefinition::setTranslate;
	modelDefinition["getContentOffset"] = &ModelDefinition::getContentOffset;
	modelDefinition["setContentOffset"] = &ModelDefinition::setContentOffset;
	modelDefinition["getRotation"] = &ModelDefinition::getRotation;
	modelDefinition["setRotation"] = &ModelDefinition::setRotation;
	modelDefinition["getSubModelDefinitions"] = &ModelDefinition::getSubModelDefinitions;
	modelDefinition["removeSubModelDefinition"] = &ModelDefinition::removeSubModelDefinition;
	modelDefinition["addSubModelDefinition"] = &ModelDefinition::addSubModelDefinition;
	modelDefinition["getActionDefinitions"] = &ModelDefinition::getActionDefinitions;
	modelDefinition["removeActionDefinition"] = &ModelDefinition::removeActionDefinition;
	modelDefinition["addActionDefinition"] = &ModelDefinition::addActionDefinition;
	modelDefinition["getAttachPointsDefinitions"] = &ModelDefinition::getAttachPointsDefinitions;
	modelDefinition["addAttachPointDefinition"] = &ModelDefinition::addAttachPointDefinition;
	modelDefinition["getViewDefinitions"] = &ModelDefinition::getViewDefinitions;
	modelDefinition["removeViewDefinition"] = &ModelDefinition::removeViewDefinition;
	modelDefinition["addViewDefinition"] = &ModelDefinition::addViewDefinition;
	modelDefinition["getPoseDefinitions"] = &ModelDefinition::getPoseDefinitions;
	modelDefinition["addPoseDefinition"] = &ModelDefinition::addPoseDefinition;
	modelDefinition["removePoseDefinition"] = &ModelDefinition::removePoseDefinition;
	modelDefinition["reloadAllInstances"] = &ModelDefinition::reloadAllInstances;


	auto modelDefinitionAtlasComposer = space.new_usertype<ModelDefinitionAtlasComposer>("ModelDefinitionAtlasComposer");
	modelDefinitionAtlasComposer["compose"] = &ModelDefinitionAtlasComposer::compose;
	modelDefinitionAtlasComposer["composeToStream"] = &ModelDefinitionAtlasComposer::composeToStream;
	modelDefinitionAtlasComposer["composeToFile"] = &ModelDefinitionAtlasComposer::composeToFile;

	auto modelDefinitionManager = space.new_usertype<ModelDefinitionManager>("ModelDefinitionManager", sol::no_constructor);
	modelDefinitionManager["getSingleton"] = &ModelDefinitionManager::getSingleton;
	modelDefinitionManager["exportScript"] = &ModelDefinitionManager::exportScript;
	modelDefinitionManager["addDefinition"] = &ModelDefinitionManager::addDefinition;
	modelDefinitionManager["getByName"] = &ModelDefinitionManager::getByName;
	modelDefinitionManager["getAllMeshes"] = &ModelDefinitionManager::getAllMeshes;
	modelDefinitionManager["getShowModels"] = &ModelDefinitionManager::getShowModels;
	modelDefinitionManager["setShowModels"] = &ModelDefinitionManager::setShowModels;

	auto subModel = space.new_usertype<SubModel>("SubModel");
	subModel["getEntity"] = &SubModel::getEntity;
}