//
// C++ Interface: EmberEntityHideModelAction
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2008
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
#ifndef EMBEROGREEMBERENTITYHIDEMODELACTION_H
#define EMBEROGREEMBERENTITYHIDEMODELACTION_H

#include "EmberOgrePrerequisites.h"
#include "domain/EmberEntity.h"

#include "components/entitymapping/Actions/Action.h"

namespace Ember {
namespace OgreView {

/**
	@brief Hides the current graphical representation when activated.
	@author Erik Ogenvik <erik@ogenvik.org>
*/
class EmberEntityHideModelAction : public EntityMapping::Actions::Action
{
public:
    EmberEntityHideModelAction(EmberEntity& entity);

    virtual ~EmberEntityHideModelAction();

	virtual void activate(EntityMapping::ChangeContext& context);
	virtual void deactivate(EntityMapping::ChangeContext& context);

protected:
	EmberEntity& mEntity;

};

}

}

#endif
