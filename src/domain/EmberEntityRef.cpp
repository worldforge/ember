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

#include "EmberEntityRef.h"

namespace Ember {

EmberEntityRef::EmberEntityRef() :
		mEntity(nullptr) {
}

EmberEntityRef::EmberEntityRef(EmberEntity* e) :
		mEntity(e) {
	if (e) {
		mConnection = mEntity->BeingDeleted.connect(sigc::mem_fun(*this, &EmberEntityRef::onEntityDeleted));
	}
}

EmberEntityRef::EmberEntityRef(const EmberEntityRef& ref) noexcept:
		mEntity(ref.mEntity) {
	if (mEntity) {
		mConnection = mEntity->BeingDeleted.connect(sigc::mem_fun(*this, &EmberEntityRef::onEntityDeleted));
	}
}

EmberEntityRef::~EmberEntityRef() {
	mConnection.disconnect();
}

EmberEntityRef& EmberEntityRef::operator=(const EmberEntityRef& ref) noexcept {
	mConnection.disconnect();
	mEntity = ref.mEntity;

	if (mEntity) {
		mConnection = mEntity->BeingDeleted.connect(sigc::mem_fun(*this, &EmberEntityRef::onEntityDeleted));
	}

	return *this;
}

void EmberEntityRef::onEntityDeleted() {
	mEntity = nullptr;
}


}