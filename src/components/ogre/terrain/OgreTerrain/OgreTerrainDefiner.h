/*
 Copyright (C) 2013 Samuel Kogler

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef OGRETERRAINDEFINER_H_
#define OGRETERRAINDEFINER_H_

#include <OgreTerrainPagedWorldSection.h>

namespace Ember
{
namespace OgreView
{
struct IPageDataProvider;

namespace Terrain
{

/**
 * @brief Upon request of the paging system, loads and defines terrain pages for subsequent rendering.
 */
class OgreTerrainDefiner : public Ogre::TerrainPagedWorldSection::TerrainDefiner
{
public:
	explicit OgreTerrainDefiner(IPageDataProvider& provider);

	~OgreTerrainDefiner() override = default;

	void define(Ogre::TerrainGroup *terrainGroup, long x, long y) override;

private:
	IPageDataProvider& mProvider;
};

} /* namespace Terrain */
} /* namespace OgreView */
} /* namespace Ember */
#endif /* OGRETERRAINDEFINER_H_ */
