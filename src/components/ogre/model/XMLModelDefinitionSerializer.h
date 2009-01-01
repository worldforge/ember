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

#ifndef __XMLModelDefinitionSerializer__
#define __XMLModelDefinitionSerializer__

// Includes

#include "components/ogre/EmberOgrePrerequisites.h"

#include "ModelDefinition.h"
#include "framework/tinyxml/tinyxml.h"

// Namespaces
namespace EmberOgre { 
namespace Model {

// Classes
class XMLModelDefinitionSerializer
{
public :
	XMLModelDefinitionSerializer();
	virtual ~XMLModelDefinitionSerializer();

	void importModelDefinition(Ogre::DataStreamPtr& stream, ModelDefinition* pmModelDef);
	void parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName);
	
	void exportScript(ModelDefinitionPtr modelDef, const std::string& filename);

private:
	//TiXmlDocument* _XMLDoc;
	//ModelDefinition* _pModelDef;

	void readModel(ModelDefinitionPtr modelDef, TiXmlElement* modelNode);
	void readSubModels(ModelDefinitionPtr modelDef, TiXmlElement* mSubModelNode);
	void readParts(TiXmlElement* mPartNode, SubModelDefinition* def);
	void readSubEntities(TiXmlElement* mSubEntNode, PartDefinition* def);
	void readActions(ModelDefinitionPtr modelDef, TiXmlElement* mAnimNode);
	void readAnimations(TiXmlElement* mAnimationsNode, ActionDefinition* action);
	void readAnimationParts(TiXmlElement* mAnimPartNode, AnimationDefinition* animDef);
	void readAttachPoints(ModelDefinitionPtr modelDef, TiXmlElement* mAnimPartNode);
	void readParticleSystems(ModelDefinitionPtr modelDef, TiXmlElement* mParticleSystemsNode);
	void readParticleSystemsBindings(ModelDefinition::ParticleSystemDefinition& def, TiXmlElement* mParticleSystemsNode);
	void readViews(ModelDefinitionPtr modelDef, TiXmlElement* viewsNode);
	void readSounds(TiXmlElement* mAnimationsNode, ActionDefinition* action);
	void readLights(ModelDefinitionPtr modelDef, TiXmlElement* mLightsNode);

	/**
	 * @brief Utility method for filling an Ogre Vector3 with data from an xml element.
	 * @param elem The xml element from which data will be taken. The xml element must have three numerical properties, "x", "y" and "z".
	 * @return An Ogre::Vector3 instance.
	 */
	Ogre::Vector3 fillVector3FromElement(TiXmlElement* elem);
	
	/**
	 * @brief Utility method for filling an xml element with data from an Ogre::Vector3 instance.
	 * @param elem The element which should be filled. The data will be represented through three numerical properties, "x", "y" and "z".
	 * @param vector The Ogre Vector3 instance from which the data will be taken.
	 */
	void fillElementFromVector3(TiXmlElement& elem, Ogre::Vector3 vector);

	/**
	 * @brief Utility method for filling an Ogre Quaternion with data from an xml element.
	 * @param elem The xml element from which data will be taken. The xml element must have four numerical properties, "w", "x", "y" and "z".
	 * @return An Ogre::Quaternion instance.
	 */
	Ogre::Quaternion fillQuaternionFromElement(TiXmlElement* elem);
	
	/**
	 * @brief Utility method for filling an xml element with data from an Ogre::Quaternion instance.
	 * @param elem The element which should be filled. The data will be represented through four numerical properties, "w", "x", "y" and "z".
	 * @param vector The Ogre Quaternion instance from which the data will be taken.
	 */
	void fillElementFromQuaternion(TiXmlElement& elem, Ogre::Quaternion quaternion);

	void exportSubModels(ModelDefinitionPtr modelDef, TiXmlElement& modelElem);
	void exportActions(ModelDefinitionPtr modelDef, TiXmlElement& modelElem);
	void exportAttachPoints(ModelDefinitionPtr modelDef, TiXmlElement& modelElem);
	void exportViews(ModelDefinitionPtr modelDef, TiXmlElement& modelElem);
	
	/**
	 * @brief Exports the light data.
	 * @param modelDef The model definition.
	 * @param modelElem The model xml element.
	 */
	void exportLights(ModelDefinitionPtr modelDef, TiXmlElement& modelElem);

};

}
}
#endif
