//
// C++ Interface: EntityCreatorActionCreator
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009
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
#ifndef EMBEROGRE_GUIENTITYCREATORACTIONCREATOR_H
#define EMBEROGRE_GUIENTITYCREATORACTIONCREATOR_H

#include "components/entitymapping/IActionCreator.h"

namespace Ember {
namespace OgreView {

namespace Gui {

class EntityCreatorCreationInstance;

/**
 * @brief Creating entity creator specific actions with model mapping framework.
 *
 * Implementation of IActionCreator interface to use with model mapping framework.
 * Creates entity creator specific actions. Used for showing entity preview.
 */
class EntityCreatorActionCreator : public EntityMapping::IActionCreator
{
public:
	EntityCreatorActionCreator(EntityCreatorCreationInstance& creationInstance);
	virtual ~EntityCreatorActionCreator();
	virtual void createActions(EntityMapping::EntityMapping& modelMapping, EntityMapping::Cases::CaseBase* aCase, EntityMapping::Definitions::CaseDefinition& caseDefinition);
protected:
	EntityCreatorCreationInstance& mCreationInstance;
};

}

}

}

#endif
