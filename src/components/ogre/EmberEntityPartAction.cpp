//
// C++ Implementation: EmberEntityPartAction
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

#include "EmberEntityPartAction.h"

#include <utility>

#include "domain/EmberEntity.h"
#include "components/ogre/model/ModelRepresentation.h"


namespace Ember {
namespace OgreView {


EmberEntityPartAction::EmberEntityPartAction(EmberEntity& entity, std::string partName)
: mEntity(entity), mPartName(std::move(partName))
{
}


EmberEntityPartAction::~EmberEntityPartAction() = default;

void EmberEntityPartAction::activate(EntityMapping::ChangeContext& context)
{
	S_LOG_VERBOSE("Showing part " << mPartName);
	Model::ModelRepresentation* representation = Model::ModelRepresentation::getRepresentationForEntity(mEntity);
	if (representation) {
		representation->setModelPartShown(mPartName, true);
	}
}

void EmberEntityPartAction::deactivate(EntityMapping::ChangeContext& context)
{
	S_LOG_VERBOSE("Hiding part " << mPartName);
	Model::ModelRepresentation* representation = Model::ModelRepresentation::getRepresentationForEntity(mEntity);
	if (representation) {
		representation->setModelPartShown(mPartName, false);
	}
}

}
}
