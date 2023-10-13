/*
 Copyright (C) 2010 Erik Ogenvik <erik@ogenvik.org>

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

#include "EnteredWorldState.h"
#include "ServerServiceSignals.h"

#include "framework/Tokeniser.h"
#include "framework/ConsoleBackend.h"
#include "framework/LoggingInstance.h"

#include <Eris/Entity.h>
#include <Eris/Avatar.h>
#include <Eris/Account.h>
#include <Eris/Connection.h>
#include <Eris/View.h>
#include <Eris/TypeInfo.h>

namespace Ember {

EnteredWorldState::EnteredWorldState(IState& parentState, Eris::Avatar& avatar, Eris::Account& account) :
		StateBase<void>::StateBase(parentState),
		mAvatar(avatar),
		mAccount(account) {
	getSignals().GotAvatar.emit(&mAvatar);
	getSignals().GotView.emit(&getView());

}

EnteredWorldState::~EnteredWorldState() {
	getSignals().DestroyedView.emit();
	getSignals().DestroyedAvatar.emit();
}

bool EnteredWorldState::logout() {
	mAvatar.deactivate();
	return true;
}

void EnteredWorldState::runCommand(const std::string& command, const std::string& args) {

}

Eris::Connection& EnteredWorldState::getConnection() const {
	return mAvatar.getConnection();
}

Eris::View& EnteredWorldState::getView() const {
	return mAvatar.getView();
}

}
