//
// C++ Implementation: EntityMapping
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
#include "EntityMapping.h"
#include <Eris/View.h>

namespace Ember {


namespace EntityMapping {


EntityMapping::EntityMapping(Eris::Entity& entity) : mEntity(entity) {
	mBaseCase.setState(true);
}

Cases::CaseBase& EntityMapping::getBaseCase() {
	return mBaseCase;
}


void EntityMapping::initialize() {
	ChangeContext changeContext{};

	mBaseCase.evaluateChanges(changeContext);

	changeContext.performActions();
}


}

}
