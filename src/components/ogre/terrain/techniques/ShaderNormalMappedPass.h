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


#ifndef EMBEROGRETERRAINTECHNIQUESSHADERNORMALMAPPEDPASS_H_
#define EMBEROGRETERRAINTECHNIQUESSHADERNORMALMAPPEDPASS_H_

#include "ShaderPass.h"

namespace EmberOgre
{

namespace Terrain
{

class TerrainPageSurfaceLayer;

namespace Techniques
{

class ShaderPassCoverageBatch;

class ShaderNormalMappedPass : public ShaderPass
{
public:
	ShaderNormalMappedPass(Ogre::Pass* pass, const TerrainPage& page);
	virtual ~ShaderNormalMappedPass() {}

// 	virtual void addLayer(TerrainPageSurfaceLayer* layer);
// 	virtual void setBaseLayer(TerrainPageSurfaceLayer* layer);

	virtual bool hasRoomForLayer(const TerrainPageSurfaceLayer* layer);


	/**
	 *    Creates the combined final coverage textures and sets the shader params. Be sure to call this before you load the material.
	 */
	virtual bool finalize();


protected:
	virtual ShaderPassCoverageBatch* createNewBatch();

};

}

}

}

#endif /* EMBEROGRETERRAINTECHNIQUESSHADERNORMALMAPPEDPASS_H_ */
