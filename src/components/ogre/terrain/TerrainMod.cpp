//
// C++ Implementation: TerrainMod
//
// Description:
//
//
// Author: Tamas Bates <rhymre@gmail.com>, (C) 2008
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2008
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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainMod.h"
#include <Eris/TerrainMod.h>
#include "../EmberEntity.h"

namespace EmberOgre
{
namespace Terrain
{

TerrainMod::TerrainMod(EmberEntity* entity) :
	mInnerMod(new Eris::TerrainMod(entity))
{
	mInnerMod->ModChanged.connect(sigc::mem_fun(*this, &TerrainMod::terrainMod_ModChanged));
	mInnerMod->ModDeleted.connect(sigc::mem_fun(*this, &TerrainMod::terrainMod_ModDeleted));
}

TerrainMod::~TerrainMod()
{
	delete mInnerMod;
}

bool TerrainMod::init()
{
	return mInnerMod->init();
}

Eris::TerrainMod* TerrainMod::getErisMod() const
{
	return mInnerMod;
}

const std::string& TerrainMod::getEntityId() const
{
	return mInnerMod->getEntity()->getId();
}

void TerrainMod::terrainMod_ModChanged()
{
	EventModChanged.emit();
}

void TerrainMod::terrainMod_ModDeleted()
{
	EventModDeleted.emit();
}

Mercator::TerrainMod* TerrainMod::getMercatorMod() const
{
	return mInnerMod->getMod();
}

} // close Namespace Terrain
} // close Namespace EmberOgre
