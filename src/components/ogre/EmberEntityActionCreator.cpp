//
// C++ Implementation: EmberEntityActionCreator
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "EmberEntityActionCreator.h"
#include "EmberEntityModelAction.h"
#include "EmberEntityHideModelAction.h"
#include "EmberEntityPartAction.h"
#include "CompositionAction.h"
#include "components/ogre/environment/OceanAction.h"
#include "components/ogre/widgets/LabelAction.h"
#include "PresentAction.h"

using namespace Ember::EntityMapping;

namespace Ember {
namespace OgreView {

EmberEntityActionCreator::EmberEntityActionCreator(EmberEntity& entity, Scene& scene)
: mEntity(entity), mScene(scene)
{
}

void EmberEntityActionCreator::createActions(EntityMapping::EntityMapping& modelMapping, Cases::CaseBase* aCase, Definitions::CaseDefinition& caseDefinition)
{
	for (auto& actionDef : caseDefinition.getActions()) {
		if (actionDef.getType() == "display-part") {
			aCase->addAction(new EmberEntityPartAction(mEntity, actionDef.getValue()));
		} else if (actionDef.getType() == "display-model") {
			aCase->addAction(new EmberEntityModelAction(mEntity, actionDef.getValue(), mScene, modelMapping));
		} else if (actionDef.getType() == "hide-model") {
			aCase->addAction(new EmberEntityHideModelAction(mEntity));
		} else if (actionDef.getType() == "display-label") {
			aCase->addAction(new Gui::LabelAction(mEntity));
		} else if (actionDef.getType() == "display-ocean") {
			aCase->addAction(new Environment::OceanAction(mEntity));
		} else if (actionDef.getType() == "enable-composition") {
			aCase->addAction(new CompositionAction(mEntity, actionDef.getValue()));
		} else if (actionDef.getType() == "present") {
			aCase->addAction(new PresentAction(mEntity, mScene, modelMapping));
		} else {
			S_LOG_WARNING("Could not recognize entity action '" << actionDef.getType() << "'.");
		}
	}

}

}
}
