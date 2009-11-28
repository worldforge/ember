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


#ifndef EMBEROGRETERRAINTECHNIQUESSHADERNORMALMAPPED_H_
#define EMBEROGRETERRAINTECHNIQUESSHADERNORMALMAPPED_H_

#include "Shader.h"

namespace EmberOgre
{

namespace Terrain
{

namespace Techniques
{

class ShaderNormalMapped : public Shader
{
public:
	/**
	 * @brief Ctor.
	 * @param includeShadows If true, shadows will be used.
	 */
	ShaderNormalMapped(bool includeShadows, const TerrainPageGeometry& geometry, const SurfaceLayerStore& terrainPageSurfaces, const TerrainPageShadow* terrainPageShadow, const TerrainPage& page);
protected:
	virtual ShaderPass* addPass(Ogre::Technique* technique);

};

}

}

}

#endif /* EMBEROGRETERRAINTECHNIQUESSHADERNORMALMAPPED_H_ */
