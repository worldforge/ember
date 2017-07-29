//
// C++ Implementation: TerrainPageShadow
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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

#include "TerrainPageShadow.h"

#include "TerrainPage.h"
#include "TerrainPageGeometry.h"
#include "OgreImage.h"
#include "../Convert.h"

#include <OgreColourValue.h>
#include <OgreImage.h>

namespace Ember
{
namespace OgreView
{
namespace Terrain
{

TerrainPageShadow::TerrainPageShadow(const TerrainPage& terrainPage) :
		mTerrainPage(terrainPage), mLightDirection(WFMath::Vector<3>::ZERO()), mImage(nullptr)
{
}

TerrainPageShadow::~TerrainPageShadow()
{
	delete mImage;
}

void TerrainPageShadow::setLightDirection(const WFMath::Vector<3>& lightDirection)
{
	mLightDirection = lightDirection;
}

void TerrainPageShadow::updateShadow(const TerrainPageGeometry& geometry)
{
	if (!mImage) {
		mImage = new OgreImage(new Image::ImageBuffer(mTerrainPage.getBlendMapSize(), 1));
	}
	mImage->reset();


	int pageSizeInVertices = mTerrainPage.getPageSize();
	int pageSizeInMeters = pageSizeInVertices - 1;

	//since Ogre uses a different coord system than WF, we have to do some conversions here
	TerrainPosition origPosition(0, pageSizeInMeters - 1);

	WFMath::Vector<3> wfLightDirection = mLightDirection;
	wfLightDirection = wfLightDirection.normalize(1);

	TerrainPosition position(origPosition);

	auto data = mImage->getData();

	for (int i = 0; i < pageSizeInMeters; ++i) {
		position = origPosition;
		position[1] = position[1] - i;
		for (int j = 0; j < pageSizeInMeters; ++j) {
			WFMath::Vector<3> normal;
			if (geometry.getNormal(position, normal)) {
				float dotProduct = WFMath::Dot(normal.normalize(1), wfLightDirection);

				// if the dotProduct is > 0, the face is looking away from the sun
				*data = static_cast<unsigned char>((1.0f - ((dotProduct + 1.0f) * 0.5f)) * 255);

			} else {
				*data = 0;
			}
			data++;
			position[0] = position[0] + 1;
		}
	}

}

void TerrainPageShadow::loadIntoImage(Ogre::Image& ogreImage) const
{
	ogreImage.loadDynamicImage(mImage->getData(), mImage->getResolution(), mImage->getResolution(), 1, Ogre::PF_L8);

}


void TerrainPageShadow::setShadowTextureName(const std::string& shadowTextureName)
{
	mShadowTextureName = shadowTextureName;
}

const std::string& TerrainPageShadow::getShadowTextureName() const
{
	return mShadowTextureName;
}


}
}
}
