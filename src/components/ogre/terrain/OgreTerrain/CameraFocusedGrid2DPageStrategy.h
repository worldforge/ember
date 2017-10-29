/*
 Copyright (C) 2013 Erik Ogenvik

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

#ifndef CAMERAFOCUSEDGRID2DPAGESTRATEGY_H_
#define CAMERAFOCUSEDGRID2DPAGESTRATEGY_H_

#include <OgreGrid2DPageStrategy.h>

namespace Ember
{
namespace OgreView
{
namespace Terrain
{

/**
 * @brief Loads pages near the camera first.
 *
 * This is a slight modified version of the base Ogre::Grid2DPageStrategy class
 * with the only difference being that pages that are close to the camera are loaded
 * first (which is what you would want in most cases).
 */
class CameraFocusedGrid2DPageStrategy : public Ogre::Grid2DPageStrategy
{
public:
	explicit CameraFocusedGrid2DPageStrategy(Ogre::PageManager* manager);

	~CameraFocusedGrid2DPageStrategy() override;

    void notifyCamera(Ogre::Camera* cam, Ogre::PagedWorldSection* section) override;

protected:

    /**
     * @brief Loads the pages nearest to the camera.
     *
     * This is done by sampling the pages at the position of the camera as well as in 100 meters out.
     * @param gridpos The grid position of the camera.
     * @param section
     */
    void loadNearestPages(const Ogre::Vector2& gridpos, Ogre::PagedWorldSection* section);


};


}
}
}
#endif /* CAMERAFOCUSEDGRID2DPAGESTRATEGY_H_ */
