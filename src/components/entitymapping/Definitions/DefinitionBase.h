//
// C++ Interface: DefinitionBase
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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
#ifndef EMBEROGRE_MODEL_MAPPINGDEFINITIONBASE_H
#define EMBEROGRE_MODEL_MAPPINGDEFINITIONBASE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include <string>
#include <map>


namespace Ember {



namespace EntityMapping {

namespace Definitions {


/**
	Base class for all Definitions.
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class DefinitionBase
{
public:
	typedef std::map<std::string, std::string> PropertiesMap;
	/**
	Returns a map of arbitrary properties.
	*/
	PropertiesMap& getProperties();

	/**
	Returns a map of arbitrary properties.
	*/
	const PropertiesMap& getProperties() const;

	/**
	Gets the type of this definition.
	*/
    const std::string& getType() const;
    void setType(std::string type);

protected:
	std::map<std::string, std::string> mProperties;
	std::string mType;
};

}

}

}


#endif
