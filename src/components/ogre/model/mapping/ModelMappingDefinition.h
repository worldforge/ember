//
// C++ Interface: ModelMappingDefinition
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2007
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
#ifndef EMBEROGRE_MODEL_MAPPINGMODELMAPPINGDEFINITION_H
#define EMBEROGRE_MODEL_MAPPINGMODELMAPPINGDEFINITION_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include <vector>
#include <map>
#include <string>

namespace EmberOgre {

namespace Model {

namespace Mapping {


class CaseDefinition;

class DefinitionBase
{
public:
	typedef std::map<std::string, std::string> PropertiesMap;
	PropertiesMap& getProperties() { return mProperties; }

    const std::string& getType() const { return mType; }
    void setType(std::string type) { mType = type; }
	
protected:
	std::map<std::string, std::string> mProperties;
	std::string mType;
};

class ActionDefinition : public DefinitionBase
{
public:
protected:
};

class MatchDefinition  : public DefinitionBase
{
public:
	typedef std::vector<CaseDefinition> CaseStore;
	
	CaseStore& getCases() { return mCases;}
	
protected:
	CaseStore mCases;
};

class CaseDefinition : public DefinitionBase
{
public:
	typedef std::vector<MatchDefinition> MatchStore;
	typedef std::vector<ActionDefinition> ActionStore;

	MatchStore& getMatches() { return mMatches;}
	ActionStore& getActions() { return mActions;}

protected:
	MatchStore mMatches;
	ActionStore mActions;
};



/**
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class ModelMappingDefinition{
public:
    ModelMappingDefinition();

    ~ModelMappingDefinition();
    
    const std::string& getName() const { return mName; }
    void setName(std::string name) { mName = name; }
    
    MatchDefinition& getRoot() { return mRootMatch;}
protected:
	MatchDefinition mRootMatch;
	std::string mName;
};

}

}

}

#endif
