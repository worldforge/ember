//
// C++ Interface: EntityMappingManager
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
#ifndef EMBEROGRE_MODEL_MAPPINGMODELMAPPINGMANAGER_H
#define EMBEROGRE_MODEL_MAPPINGMODELMAPPINGMANAGER_H


#include <vector>
#include <unordered_map>

#include <Eris/TypeInfo.h>
#include <Eris/Entity.h>
#include <boost/noncopyable.hpp>

#include "Definitions/EntityMappingDefinition.h"
#include "EntityMapping.h"


namespace Ember {


namespace EntityMapping {

class EntityMapping;

struct IActionCreator;


/**
	Handles all EntityMapping instances, as well as creation and setup.

	Applications are expected to add definitions to the manager through the addDefinition(...) method. Definitions are managed by the manager and will be deleted by this upon destruction.
	New EntityMapping instances are created by calling createMapping(...). It's up to the application to delete all EntityMapping instances created by the manager.

	@author Erik Ogenvik <erik@ogenvik.org>
*/
class EntityMappingManager : boost::noncopyable {
public:
	typedef std::unordered_map<std::string, std::unique_ptr<Definitions::EntityMappingDefinition>> EntityMappingDefinitionStore;

	/**
	Default constructor.
	*/
	EntityMappingManager();

	~EntityMappingManager();

	/**
	Adds a definition to the manager. This definition will be deleted by the manager upon destruction.
	@param definition A valid definition.
	*/
	void addDefinition(std::unique_ptr<Definitions::EntityMappingDefinition> definition);

	/**
	Creates a new EntityMapping instance. This will not be handled by the manager and needs to be deleted by the application when suitable.
	Remember to call EntityMapping::initialize(...).
	@param entity An eris type info instance.
	@param actionCreator An eris type info instance.
	@param view An optional view, if any such is available.
	*/
	std::unique_ptr<EntityMapping> createMapping(Eris::Entity& entity, IActionCreator& actionCreator, Eris::TypeService& typeService, Eris::View* view);

protected:

	EntityMappingDefinitionStore mDefinitions;


};



}

}

#endif
