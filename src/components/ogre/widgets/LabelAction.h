/*
 Copyright (C) 2014 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef LABELACTION_H_
#define LABELACTION_H_

#include "components/entitymapping/Actions/Action.h"

namespace Ember
{
class EmberEntity;
namespace OgreView
{
namespace Gui
{

class LabelAction: public EntityMapping::Actions::Action
{
public:
	LabelAction(EmberEntity& entity);
	virtual ~LabelAction();

	/**
	 * @copydoc EntityMapping::Actions::Action::activate()
	 */
	virtual void activate(EntityMapping::ChangeContext& context);

	/**
	 * @copydoc EntityMapping::Actions::Action::deactivate()
	 */
	virtual void deactivate(EntityMapping::ChangeContext& context);

private:

	EmberEntity& mEntity;
};

}
}
}
#endif /* LABELACTION_H_ */
