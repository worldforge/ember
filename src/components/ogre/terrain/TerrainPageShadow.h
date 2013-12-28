//
// C++ Interface: TerrainPageShadow
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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
#ifndef EMBEROGRETERRAINPAGESHADOW_H
#define EMBEROGRETERRAINPAGESHADOW_H
#include "../EmberOgrePrerequisites.h"

#include <memory>
#include <wfmath/vector.h>
#include <OgreMath.h>

namespace Ogre {
	class ColourValue;
	class Image;
	class Vector2;
}

namespace Ember {
namespace OgreView {
namespace Terrain {

class TerrainPage;
class TerrainPageGeometry;
class OgreImage;

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class TerrainPageShadow
{
public:
	TerrainPageShadow(const TerrainPage& terrainPage);

	virtual ~TerrainPageShadow();

	void setLightDirection(const WFMath::Vector<3>& lightDirection);

	void updateShadow(const TerrainPageGeometry& geometry);

	void loadIntoImage(Ogre::Image& ogreImage) const;

	/**
	 * @brief Sets an optional shadow texture name.
	 *
	 * This refers to a precomputed shadow texture in Ogre. Note that this only is of use when using
	 * the fixed function pipeline which requires precomputed shadows.
	 *
	 * @param shadowTextureName The name of the shadow texture.
	 */
	void setShadowTextureName(const std::string& shadowTextureName);

	/**
	 * @brief Gets an optional shadow texture name.
	 *
	 * This refers to a precomputed shadow texture in Ogre. Note that this only is of use when using
	 * the fixed function pipeline which requires precomputed shadows.
	 *
	 * @return A name of a texture in Ogre, or an empty string if there's no shadow texture.
	 */
	const std::string& getShadowTextureName() const;


protected:
	const TerrainPage& mTerrainPage;
	WFMath::Vector<3> mLightDirection;

	OgreImage* mImage;

	/**
	 * @brief An optional shadow texture name.
	 *
	 * This refers to a precomputed shadow texture in Ogre. Note that this only is of use when using
	 * the fixed function pipeline which requires precomputed shadows.
	 */
	std::string mShadowTextureName;

};

}
}

}

#endif
