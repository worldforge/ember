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

#include <Ogre.h>
#include <ModelDefinition.h>
#include "framework/tinyxml/tinyxml.h"

// Namespaces
namespace EmberOgre { 

// Classes
class XMLModelDefinitionSerializer
{
public :
	XMLModelDefinitionSerializer();
	~XMLModelDefinitionSerializer();

	void importModelDefinition(Ogre::DataStreamPtr& stream, ModelDefinition* pmModelDef);
	void parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName);

private:
	Ember::TiXmlDocument* _XMLDoc;
	//ModelDefinition* _pModelDef;

	void readModel(ModelDefinitionPtr modelDef, Ember::TiXmlElement* modelNode);
	void readSubModels(ModelDefinitionPtr modelDef, Ember::TiXmlElement* mSubModelNode);
	void readParts(Ember::TiXmlElement* mPartNode,ModelDefinition::SubModelDefinition& subModel);
	void readSubEntities(Ember::TiXmlElement* mSubEntNode,ModelDefinition::PartDefinition& part);
	void readActions(ModelDefinitionPtr modelDef, Ember::TiXmlElement* mAnimNode);
	void readAnimationParts(Ember::TiXmlElement* mAnimPartNode,ModelDefinition::ActionDefinition& action);
	void readAttachPoints(ModelDefinitionPtr modelDef, Ember::TiXmlElement* mAnimPartNode);
	void readParticleSystems(ModelDefinitionPtr modelDef, Ember::TiXmlElement* mParticleSystemsNode);
	void readParticleSystemsBindings(ModelDefinition::ParticleSystemDefinition& def, Ember::TiXmlElement* mParticleSystemsNode);

};

}
#endif
