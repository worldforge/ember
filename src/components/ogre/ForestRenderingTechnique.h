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

#ifndef FORESTRENDERINGTECHNIQUE_H_
#define FORESTRENDERINGTECHNIQUE_H_

#include "components/ogre/ISceneRenderingTechnique.h"

namespace EmberOgre
{

namespace Environment {
class Forest;
}

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief A rendering technique suited for trees in a forest.
 *
 * Models rendered with this technique will be handled as forest trees, which means that they will be using imposters when viewed far away.
 * The PagedGeometry will handle the actual rendering through the Forest class.
 */
class ForestRenderingTechnique : public ISceneRenderingTechnique
{
public:
	/**
	 * @brief Ctor.
	 * @param forest The forest instance which will handle the actual rendering.
	 */
	ForestRenderingTechnique(Environment::Forest& forest);

	/**
	 * @brief Dtor.
	 */
	virtual ~ForestRenderingTechnique();

	virtual void registerEntity(EmberEntity& entity);

protected:

	/**
	 * @brief The forest instance which will handle the rendering.
	 */
	Environment::Forest& mForest;

};


}

#endif /* FORESTRENDERINGTECHNIQUE_H_ */
