//
// C++ Implementation: CaseBase
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

#include "CaseBase.h"

#include "../EntityMapping.h"
#include "../Actions/Action.h"

namespace Ember {


namespace EntityMapping {

namespace Cases {

CaseBase::CaseBase()
		: mParentCase(nullptr), mIsTrue(false), mIsActive(false) {
}

CaseBase::~CaseBase() = default;

void CaseBase::addAction(std::unique_ptr<Actions::Action> action) {
	action->setCase(this);
	mActions.emplace_back(std::move(action));
}

void CaseBase::evaluateChanges(ChangeContext& changeContext) {
	//if we're true to the root, but not yet active, add ourself to the activation queue. If the opposite, i.e. we're no longer true to the root but active, we need to be deactivated.
	if (getIsTrueToRoot()) {
		if (!getIsActive()) {
			changeContext.addCaseToActivate(this);
		}
	} else {
		if (getIsActive()) {
			changeContext.addCaseToDeactivate(this);
		}
	}
	//recursively iterate over the child matches
	for (auto& match : mMatches) {
		match->evaluateChanges(changeContext);
	}

}

void CaseBase::accept(IVisitor& visitor) {
	visitor.visit(*this);
	for (auto& action : mActions) {
		action->accept(visitor);
	}
	for (auto& match : mMatches) {
		match->accept(visitor);
	}
}

void CaseBase::activateActions(ChangeContext& context) {
	for (auto& action : mActions) {
		action->activate(context);
	}
	mIsActive = true;
}

void CaseBase::deactivateActions(ChangeContext& context) {
	for (auto& action : mActions) {
		action->deactivate(context);
	}
	mIsActive = false;
}

void CaseBase::addMatch(std::unique_ptr<Matches::MatchBase> match) {
	mMatches.push_back(std::move(match));
}

void CaseBase::setEntity(Eris::Entity* entity) {
	for (auto& match : mMatches) {
		match->setEntity(entity);
	}
}


}

}

}
