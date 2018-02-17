//
// C++ Interface: EntityCreatorHideModelAction
//
// Description:
//
//
// Author: Alexey Torkhov <atorkhov@gmail.com>, (C) 2008
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2009
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
#ifndef EMBEROGRE_GUIENTITYCREATORHIDEMODELACTION_H
#define EMBEROGRE_GUIENTITYCREATORHIDEMODELACTION_H

#include "components/entitymapping/Actions/Action.h"

namespace Ember {
namespace OgreView {

namespace Gui {

class EntityCreatorCreationInstance;

/**
 * Shows or hides specific model in entity creator preview.
 */
class EntityCreatorHideModelAction : public EntityMapping::Actions::Action
{
public:
	explicit EntityCreatorHideModelAction(EntityCreatorCreationInstance& creationInstance);
	~EntityCreatorHideModelAction() override = default;
	/**
	 * Hides model. Called by model mapping framework.
	 */
	void activate(EntityMapping::ChangeContext& context) override;
	/**
	 * Does nothing. Called by model mapping framework.
	 */
	void deactivate(EntityMapping::ChangeContext& context) override;
protected:
	EntityCreatorCreationInstance& mCreationInstance;
};

}

}

}

#endif
