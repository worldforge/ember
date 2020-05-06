/*
 Copyright (C) 2018 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef EMBER_MODELACTIONCREATOR_H
#define EMBER_MODELACTIONCREATOR_H

#include <functional>
#include <Eris/Entity.h>
#include "components/entitymapping/IActionCreator.h"
#include "components/entitymapping/Actions/Action.h"

namespace Ember {
namespace OgreView {
namespace Mapping {

/**
 * A basic implementation of IActionCreator used for displaying Models.
 */
class ModelActionCreator : public Ember::EntityMapping::IActionCreator {
public:
	ModelActionCreator(Eris::Entity& entity,
					   std::function<void(const std::string&)> showModelFn,
					   std::function<void(const std::string&)> showPartFn);

	void createActions(EntityMapping::EntityMapping& modelMapping,
					   EntityMapping::Cases::CaseBase& aCase,
					   EntityMapping::Definitions::CaseDefinition& caseDefinition) override;

protected:
	Eris::Entity& mEntity;
	std::function<void(std::string)> mShowModelFn;
	std::function<void(std::string)> mShowPartFn;

	class DisplayModelAction : public EntityMapping::Actions::Action {
	public:
		ModelActionCreator* mCreator;
		std::string mModelName;

		explicit DisplayModelAction(ModelActionCreator* creator, std::string modelName);

		void activate(EntityMapping::ChangeContext& context) override;

		void deactivate(EntityMapping::ChangeContext& context) override {
		}

	};

	class DisplayPartAction : public EntityMapping::Actions::Action {
	public:
		ModelActionCreator* mCreator;
		std::string mPartName;

		explicit DisplayPartAction(ModelActionCreator* creator, std::string partName);

		void activate(EntityMapping::ChangeContext& context) override;

		void deactivate(EntityMapping::ChangeContext& context) override {
		}

	};

	class PresentAction : public EntityMapping::Actions::Action {
	public:
		ModelActionCreator* mCreator;

		explicit PresentAction(ModelActionCreator* creator);

		void activate(EntityMapping::ChangeContext& context) override;

		void deactivate(EntityMapping::ChangeContext& context) override {
		}

	};


};

}
}
}


#endif //EMBER_MODELACTIONCREATOR_H
