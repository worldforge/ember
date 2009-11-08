//
// C++ Interface: EmberEntityPartAction
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
#ifndef EMBEROGREEMBERENTITYPARTACTION_H
#define EMBEROGREEMBERENTITYPARTACTION_H

#include "EmberOgrePrerequisites.h"
#include "components/entitymapping/Actions/Action.h"

namespace EmberOgre
{

class EmberEntity;

/**
 * @brief Show a certain part of a Model.
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 */
class EmberEntityPartAction: public Ember::EntityMapping::Actions::Action
{
public:
	EmberEntityPartAction(EmberEntity& entity, const std::string& partName);
	~EmberEntityPartAction();

	virtual void activate(Ember::EntityMapping::ChangeContext& context);
	virtual void deactivate(Ember::EntityMapping::ChangeContext& context);

	/**
	 * @brief Returns whether this part is currently active or not.
	 * @returns True if the part is active.
	 */
	bool isActive() const;

protected:

	/**
	 * @brief The entity to which this action is attached.
	 */
	EmberEntity& mEntity;

	/**
	 * @brief The name of the part to show.
	 */
	std::string mPartName;

	/**
	 * @brief Whether the part is active or not.
	 */
	bool mIsActive;
};

}

#endif
