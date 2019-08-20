//
// C++ Interface: EmberEntityPartAction
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
#ifndef EMBEROGREEMBERENTITYPARTACTION_H
#define EMBEROGREEMBERENTITYPARTACTION_H

#include "EmberOgrePrerequisites.h"
#include "components/entitymapping/Actions/Action.h"

namespace Ember
{
class EmberEntity;
namespace OgreView
{


/**
 * @brief Show a certain part of a Model.
 * @author Erik Ogenvik <erik@ogenvik.org>
 */
class EmberEntityPartAction: public EntityMapping::Actions::Action
{
public:
	EmberEntityPartAction(EmberEntity& entity, std::string partName);

	~EmberEntityPartAction() override;

	void activate(EntityMapping::ChangeContext& context) override;

	void deactivate(EntityMapping::ChangeContext& context) override;

protected:

	/**
	 * @brief The entity to which this action is attached.
	 */
	EmberEntity& mEntity;

	/**
	 * @brief The name of the part to show.
	 */
	const std::string mPartName;
};

}

}

#endif
