/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#ifndef TERRAINMODCHANGETASK_H_
#define TERRAINMODCHANGETASK_H_

#include "TerrainModTaskBase.h"
#include <Mercator/TerrainMod.h>
#include <Atlas/Message/Element.h>

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Task for changing an existing terrain mod.
 */
class TerrainModChangeTask: public TerrainModTaskBase
{
public:
	TerrainModChangeTask(Mercator::Terrain& terrain, const TerrainMod& terrainMod, TerrainHandler& handler, TerrainModMap& terrainMods);

	virtual void executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context);

	virtual void executeTaskInMainThread();

private:

	const Atlas::Message::Element mModData;
	const WFMath::Point<3> mPosition;
	const WFMath::Quaternion& mOrientation;

};

}

}

}

#endif /* TERRAINMODCHANGETASK_H_ */
