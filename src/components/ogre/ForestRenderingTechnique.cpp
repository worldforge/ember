/*
 Copyright (C) 2010 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ForestRenderingTechnique.h"
#include "environment/Forest.h"
#include "model/ModelRepresentationManager.h"
#include "model/ModelRepresentation.h"

namespace EmberOgre
{

ForestRenderingTechnique::ForestRenderingTechnique(Environment::Forest& forest) :
	mForest(forest)
{

}

ForestRenderingTechnique::~ForestRenderingTechnique()
{
}

void ForestRenderingTechnique::registerEntity(EmberEntity& entity)
{
	Model::ModelRepresentation* modelRepresentation = Model::ModelRepresentationManager::getSingleton().getRepresentationForEntity(entity);
	if (modelRepresentation) {
		mForest.addEmberEntity(modelRepresentation);
	}
}

}
