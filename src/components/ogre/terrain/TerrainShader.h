/*
    Copyright (C) 2004  Erik Hjortsberg

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

#ifndef TERRAINSHADER_H
#define TERRAINSHADER_H

#include <Ogre.h>
#include <Mercator/Shader.h>


namespace EmberOgre {
	
class TerrainShader{
public:

	TerrainShader(const Ogre::String textureName, Mercator::Shader* shader);
	virtual ~TerrainShader();
	
	Mercator::Shader* getShader() const;
	const Ogre::String getTextureName() const;

	/*
	 * Adds a texture unit with a splatting alpha texture to the supplied pass.
	 * Use this when you're using many texture units in the same pass
	 */
	void addTextureUnitsToPass(Ogre::Pass* pass, Ogre::String splatTextureName);

	/*
	 * Adds a pass with a splatting alpha texture to the supplied technique.
	 * Use this when you're using many passes. This is more expensive than 
	 * addTextureUnitsToPass(...) but works on card with a low number of
	 * TextureUnits.
	 */
	Ogre::Pass* addPassToTechnique(Ogre::Technique* technique, Ogre::String splatTextureName);
	
protected:
	const Ogre::String mTextureName;
	Mercator::Shader* mShader;
};

}

#endif // TERRAINSHADER_H
