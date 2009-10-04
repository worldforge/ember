/*
 Copyright (C) 2009 Erik Hjortsberg

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

#include "AuthoringVisualizationMover.h"
#include "AuthoringVisualization.h"
#include "EntityMover.h"

namespace EmberOgre
{

namespace Authoring
{
AuthoringVisualizationMover::AuthoringVisualizationMover(AuthoringVisualization& visualization, EntityMover& mover) :
	mVisualization(visualization), mMover(mover)
{
	mVisualization.setControlDelegate(&mMover.getControlDelegate());
	mMover.Moved.connect(sigc::mem_fun(*this, &AuthoringVisualizationMover::mover_Moved));
}

AuthoringVisualizationMover::~AuthoringVisualizationMover()
{
	//At deletion time we'll detach ourselves as a control delegate and tell the visualization to update its position.
	mVisualization.setControlDelegate(0);
	mVisualization.updatePositionAndOrientation();
}

void AuthoringVisualizationMover::mover_Moved()
{
	mVisualization.updatePositionAndOrientation();
}
}
}
