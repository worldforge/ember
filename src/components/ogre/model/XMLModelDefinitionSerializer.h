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

#ifndef __XMLModelDefinitionSerializer__
#define __XMLModelDefinitionSerializer__


#include <boost/filesystem/path.hpp>
#include "components/ogre/EmberOgrePrerequisites.h"

#include "ModelDefinition.h"
#include "framework/tinyxml/tinyxml.h"

namespace Ember {
namespace OgreView {
namespace Model {

class ModelDefinitionManager;

class XMLModelDefinitionSerializer {
public :
	XMLModelDefinitionSerializer() = default;

	virtual ~XMLModelDefinitionSerializer() = default;

	ModelDefinitionPtr parseScript(Ogre::DataStreamPtr& stream);

	ModelDefinitionPtr parseScript(std::istream& stream, const boost::filesystem::path& path);

	/**
	 * @brief Exports the model definition to a file.
	 * @param modelDef The model definition to export.
	 * @param directory The script directory.
	 * @param filename The name of the file to which the script will be written.
	 * @return True if the script was successfully written.
	 */
	bool exportScript(const ModelDefinitionPtr& modelDef, const boost::filesystem::path& path);

private:

	ModelDefinitionPtr parseDocument(TiXmlDocument& xmlDoc, const std::string& origin);

	void readModel(const ModelDefinitionPtr& modelDef, TiXmlElement* modelNode);

	void readSubModels(const ModelDefinitionPtr& modelDef, TiXmlElement* mSubModelNode);

	void readParts(TiXmlElement* mPartNode, SubModelDefinition* def);

	void readSubEntities(TiXmlElement* mSubEntNode, PartDefinition* def);

	void readActions(const ModelDefinitionPtr& modelDef, TiXmlElement* mAnimNode);

	void readAnimations(TiXmlElement* mAnimationsNode, ActionDefinition* action);

	void readAnimationParts(TiXmlElement* mAnimPartNode, AnimationDefinition* animDef);

	void readAttachPoints(const ModelDefinitionPtr& modelDef, TiXmlElement* mAnimPartNode);

	void readParticleSystems(const ModelDefinitionPtr& modelDef, TiXmlElement* mParticleSystemsNode);

	void readParticleSystemsBindings(ModelDefinition::ParticleSystemDefinition& def, TiXmlElement* mParticleSystemsNode);

	void readViews(const ModelDefinitionPtr& modelDef, TiXmlElement* viewsNode);

	void readSounds(TiXmlElement* mAnimationsNode, ActionDefinition* action);

	void readActivations(TiXmlElement* activationsNode, ActionDefinition* action);

	void readLights(const ModelDefinitionPtr& modelDef, TiXmlElement* mLightsNode);

	void readPoses(const ModelDefinitionPtr& modelDef, TiXmlElement* mPoseNode);

	/**
	 * @brief Reads bone groups data.
	 * @param modelDef The modeldefinition node.
	 * @param boneGroupsNode The bone groups node.
	 */
	void readBoneGroups(const ModelDefinitionPtr& modelDef, TiXmlElement* boneGroupsNode);


	void exportSubModels(const ModelDefinitionPtr& modelDef, TiXmlElement& modelElem);

	void exportActions(const ModelDefinitionPtr& modelDef, TiXmlElement& modelElem);

	void exportAttachPoints(const ModelDefinitionPtr& modelDef, TiXmlElement& modelElem);

	void exportViews(const ModelDefinitionPtr& modelDef, TiXmlElement& modelElem);

	void exportParticleSystems(const ModelDefinitionPtr& modelDef, TiXmlElement& modelElem);

	void exportPoses(const ModelDefinitionPtr& modelDef, TiXmlElement& modelElem);

	/**
	 * @brief Exports the light data.
	 * @param modelDef The model definition.
	 * @param modelElem The model xml element.
	 */
	void exportLights(const ModelDefinitionPtr& modelDef, TiXmlElement& modelElem);

	/**
	 * @brief Exports bone groups data.
	 * @param modelDef The model definition.
	 * @param modelElem The model xml element.
	 */
	void exportBoneGroups(const ModelDefinitionPtr& modelDef, TiXmlElement& modelElem);

};

}
}
}
#endif
