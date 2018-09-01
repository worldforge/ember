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

#ifndef EMBER_EMBERENTITYREF_H
#define EMBER_EMBERENTITYREF_H

#include "EmberEntity.h"

namespace Ember {
class EmberEntityRef {

public:
	EmberEntityRef();

	EmberEntityRef(EmberEntity* entity);

	EmberEntityRef(const EmberEntityRef& ref) noexcept;

	~EmberEntityRef();


	const EmberEntity& operator*() const {
		return *mEntity;
	}

	EmberEntity& operator*() {
		return *mEntity;
	}

	const EmberEntity* operator->() const {
		return mEntity;
	}

	EmberEntity* operator->() {
		return mEntity;
	}

	EmberEntity* get() const {
		return mEntity;
	}

	explicit operator bool() const {
		return (mEntity != nullptr);
	}

	bool operator!() const {
		return (mEntity == nullptr);
	}

	bool operator==(const EmberEntityRef& e) const {
		return (mEntity == e.mEntity);
	}

	bool operator==(const EmberEntity* e) const {
		return (mEntity == e);
	}

	bool operator<(const EmberEntityRef& e) const {
		return (mEntity < e.mEntity);
	}

	EmberEntityRef& operator=(const EmberEntityRef& ref) noexcept;


private:
	void onEntityDeleted();

	sigc::connection mConnection;
	EmberEntity* mEntity;

};
}

#endif //EMBER_EMBERENTITYREF_H
