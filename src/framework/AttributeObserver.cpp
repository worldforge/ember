//
// C++ Implementation: AttributeObserver
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

#include "AttributeObserver.h"

#include <memory>
#include "DirectAttributeObserver.h"
#include "DeepAttributeObserver.h"
#include "Tokeniser.h"

namespace Ember {

AttributeObserver::AttributeObserver(Eris::Entity& entity, const std::string& attributeName)
		: mDirectAttributeObserver(std::make_unique<DirectAttributeObserver>(entity, EventChanged, attributeName)) {
}

AttributeObserver::AttributeObserver(Eris::Entity& entity, const std::vector<std::string>& attributePath)
		: mDeepAttributeObserver(nullptr) {
	if (attributePath.size() > 1) {
		mDeepAttributeObserver = std::make_unique<DeepAttributeObserver>(entity, EventChanged, attributePath);
	} else if (!attributePath.empty()) {
		mDirectAttributeObserver = std::make_unique<DirectAttributeObserver>(entity, EventChanged, attributePath.front());
	}
}

AttributeObserver::AttributeObserver(Eris::Entity& entity, const std::string& attributePath, const std::string& delimitor)
		: mDeepAttributeObserver(nullptr) {
	std::vector<std::string> path = Tokeniser::split(attributePath, delimitor);
	if (path.size() > 1) {
		mDeepAttributeObserver = std::make_unique<DeepAttributeObserver>(entity, EventChanged, path);
	} else if (!path.empty()) {
		mDirectAttributeObserver = std::make_unique<DirectAttributeObserver>(entity, EventChanged, path.front());
	}
}

AttributeObserver::~AttributeObserver() = default;

void AttributeObserver::forceEvaluation() {
	if (mDeepAttributeObserver) {
		mDeepAttributeObserver->forceEvaluation();
	} else if (mDirectAttributeObserver) {
		mDirectAttributeObserver->forceEvaluation();
	}
}
}
