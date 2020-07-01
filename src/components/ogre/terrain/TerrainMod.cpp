//
// C++ Implementation: TerrainMod
//
// Description:
//
//
// Author: Tamas Bates <rhymre@gmail.com>, (C) 2008
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainMod.h"

#include "TerrainModTranslator.h"

namespace Ember {
namespace OgreView {
namespace Terrain {

TerrainMod::TerrainMod(Eris::Entity& entity, const Atlas::Message::MapType& data) :
		mEntity(entity),
		mTranslator(new Ember::Terrain::TerrainModTranslator(data)) {
}

TerrainMod::~TerrainMod() = default;

const std::string& TerrainMod::getEntityId() const {
	return mEntity.getId();
}

void TerrainMod::parse(const Atlas::Message::Element& attributeValue) {
	if (attributeValue.isMap()) {
		mTranslator = std::make_unique<Ember::Terrain::TerrainModTranslator>(attributeValue.Map());
	}
}

Eris::Entity& TerrainMod::getEntity() const {
	return mEntity;
}

const Ember::Terrain::TerrainModTranslator* TerrainMod::getTranslator() const {
	return mTranslator.get();
}

void TerrainMod::reset() {
	mTranslator->reset();
}


}
}
}
