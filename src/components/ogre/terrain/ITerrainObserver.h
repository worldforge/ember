//
// C++ Interface: ITerrainObserver
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2011
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

#ifndef ITERRAINOBSERVER_H
#define ITERRAINOBSERVER_H

#include <sigc++/signal.h>

namespace Ogre {
template<typename>
struct TRect;
}

namespace WFMath {
template<int>
class AxisBox;
}

namespace Ember {
namespace OgreView {
namespace Terrain {

/**
 * Listener interface for an terrain observer, which is observes a particular terrain area.
 * @author Erik Ogenvik
 */
struct ITerrainObserver {

	virtual ~ITerrainObserver() = default;

	/**
	 * @brief Instructs the observer to observe a certain area.
	 * @param area The area to observe.
	 */
	virtual void observeArea(const WFMath::AxisBox<2>& area) = 0;

	/**
	 * @brief Instructs the observer to observe a certain area.
	 * @param area The area to observe.
	 */
	virtual void observeArea(const Ogre::TRect<int>& area) = 0;

	/**
	 * @brief Emitted when a piece of the area being observed is shown.
	 */
	sigc::signal<void> EventAreaShown;

};
}
}
}
#endif /* end of include guard: ITERRAINOBSERVER_H */
