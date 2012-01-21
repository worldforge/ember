//
// C++ Interface: EmberPagingTerrainObserver
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2011
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

#include "../../../EmberOgrePrerequisites.h"

#include "../../../terrain/ITerrainObserver.h"
#include <OgreCommon.h>

namespace Ogre {
class PagingLandscapeEvent;
}

namespace Ember {
namespace OgreView {

/**
 * @brief A terrain observer which listens to new tiles being shown.
 */
class EmberPagingTerrainObserver : public Terrain::ITerrainObserver
{

public:

	/**
	 * @brief Ctor,
	 */
	EmberPagingTerrainObserver();

	/**
	 * @brief Dtor.
	 */
	virtual ~EmberPagingTerrainObserver();
	
	virtual void observeArea(const WFMath::AxisBox<2>& area);
	virtual void observeArea(const Ogre::TRect<int>& area);

	/**
	 * @brief Callback method for when a new tile is shown.
	 */
	void tileShow(Ogre::PagingLandscapeEvent* event);

private:

	/**
	 * @brief The area under observation.
	 */
	Ogre::TRect<Ogre::Real> mObservedArea;

	/**
	 * @brief Tests if the supplied rect intersects with the observed area.
	 * @param rect The rect to test.
	 * @return True if the rects intersect.
	 */
	bool testIntersects(const Ogre::TRect<Ogre::Real>& rect) const;


};

}

}
