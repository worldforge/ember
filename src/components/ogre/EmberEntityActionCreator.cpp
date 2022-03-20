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
#include "components/ogre/environment/OceanAction.h"
#include "components/ogre/widgets/LabelAction.h"
#include "PresentAction.h"

using namespace Ember::EntityMapping;

namespace Ember {
namespace OgreView {

EmberEntityActionCreator::EmberEntityActionCreator(EmberEntity& entity,
												   Scene& scene,
												   AttachmentFunction attachmentFunction,
												   std::function<void(bool)> oceanAttachmentFunction)
		: mEntity(entity),
		  mScene(scene),
		  mAttachmentFunction(std::move(attachmentFunction)),
		  mOceanAttachmentFunction(std::move(oceanAttachmentFunction)) {
}

void EmberEntityActionCreator::createActions(EntityMapping::EntityMapping& modelMapping, Cases::CaseBase& aCase, Definitions::CaseDefinition& caseDefinition) {
	for (auto& actionDef : caseDefinition.Actions) {
		if (actionDef.Type == "display-part") {
			aCase.addAction(std::make_unique<EmberEntityPartAction>(mEntity, actionDef.Value));
		} else if (actionDef.Type == "display-model") {
			aCase.addAction(std::make_unique<EmberEntityModelAction>(mEntity, actionDef.Value, mScene, modelMapping, mAttachmentFunction));
		} else if (actionDef.Type == "hide-model") {
			aCase.addAction(std::make_unique<EmberEntityHideModelAction>(mEntity));
		} else if (actionDef.Type == "display-label") {
			aCase.addAction(std::make_unique<Gui::LabelAction>(mEntity));
		} else if (actionDef.Type == "display-ocean") {
			aCase.addAction(std::make_unique<Environment::OceanAction>(mOceanAttachmentFunction));
		} else if (actionDef.Type == "present") {
			aCase.addAction(std::make_unique<PresentAction>(mEntity, mScene, modelMapping, mAttachmentFunction));
//		} else if (actionDef.Type == "show-effect") {
//			aCase.addAction(new PresentAction(mEntity, mScene, modelMapping));
		}
	}

}

}
}
