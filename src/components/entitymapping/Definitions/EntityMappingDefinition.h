//
// C++ Interface: EntityMappingDefinition
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBEROGRE_MODEL_MAPPINGMODELMAPPINGDEFINITION_H
#define EMBEROGRE_MODEL_MAPPINGMODELMAPPINGDEFINITION_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include <vector>
#include <map>
#include <string>

#include "MatchDefinition.h"

namespace Ember {



namespace EntityMapping {

namespace Definitions {



/**
	A definition for a EntityMapping.
	@author Erik Ogenvik <erik@ogenvik.org>
*/
class EntityMappingDefinition
{
public:
    EntityMappingDefinition();

    ~EntityMappingDefinition() = default;

    const std::string& getName() const;
    void setName(std::string name);

    CaseDefinition& getRoot();
    const CaseDefinition& getRoot() const;

	/**
	 * If true, the rules in the mapping will override any "present-mesh" or "present-model" property of the entity.
	 * @return
	 */
	bool isOverride() const;
	void setIsOverride(bool isOverride);
protected:
	CaseDefinition mRootCase;
	std::string mName;

	/**
	 * If set to true, the rules in the mapping will override any "present-mesh" or "present-model" property of the entity.
	 * Defaults to false.
	 */
	bool mOverride;
};



}

}

}

#endif
