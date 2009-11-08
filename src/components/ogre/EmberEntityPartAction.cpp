//
// C++ Implementation: EmberEntityPartAction
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "EmberEntityPartAction.h"

#include "EmberEntity.h"
#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/ModelRepresentationManager.h"


namespace EmberOgre {


EmberEntityPartAction::EmberEntityPartAction(EmberEntity& entity, const std::string& partName)
: mEntity(entity), mPartName(partName), mIsActive(false)
{
}


EmberEntityPartAction::~EmberEntityPartAction()
{
}

void EmberEntityPartAction::activate(Ember::EntityMapping::ChangeContext& context)
{
	S_LOG_VERBOSE("Showing part " << mPartName);
	Model::ModelRepresentation* representation = Model::ModelRepresentationManager::getSingleton().getRepresentationForEntity(mEntity);
	if (representation) {
		representation->setModelPartShown(mPartName, true);
	}
	mIsActive = true;
}

void EmberEntityPartAction::deactivate(Ember::EntityMapping::ChangeContext& context)
{
	S_LOG_VERBOSE("Hiding part " << mPartName);
	Model::ModelRepresentation* representation = Model::ModelRepresentationManager::getSingleton().getRepresentationForEntity(mEntity);
	if (representation) {
		representation->setModelPartShown(mPartName, false);
	}
	mIsActive = false;
}

bool EmberEntityPartAction::isActive() const
{
	return mIsActive;
}

}
