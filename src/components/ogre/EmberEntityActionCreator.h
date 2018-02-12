//
// C++ Interface: EmberEntityActionCreator
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
#ifndef EMBEROGREEMBERENTITYACTIONCREATOR_H
#define EMBEROGREEMBERENTITYACTIONCREATOR_H
#include "EmberOgrePrerequisites.h"

#include "components/entitymapping/EntityMapping.h"
#include "components/entitymapping/Definitions/EntityMappingDefinition.h"
#include "components/entitymapping/EntityMappingManager.h"
#include "components/entitymapping/IActionCreator.h"

namespace Ember {
class EmberEntity;
namespace OgreView {

class Scene;

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Creates actions specific to the EmberOgre environment.
 */
class EmberEntityActionCreator : public EntityMapping::IActionCreator
{
public:
    EmberEntityActionCreator(EmberEntity& entity, Scene& scene);

    ~EmberEntityActionCreator() override = default;

	void createActions(EntityMapping::EntityMapping& modelMapping,
					   EntityMapping::Cases::CaseBase* aCase,
					   EntityMapping::Definitions::CaseDefinition& caseDefinition) override;
protected:
	EmberEntity& mEntity;
	Scene& mScene;


};

}

}

#endif
