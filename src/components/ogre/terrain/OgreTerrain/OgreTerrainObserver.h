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

#ifndef OGRETERRAINOBSERVER_H_
#define OGRETERRAINOBSERVER_H_

#include "../../EmberOgrePrerequisites.h"
#include "../ITerrainObserver.h"

#include <OgrePrerequisites.h>
#include <OgreCommon.h>
#include <sigc++/connection.h>
#include <memory>


namespace Ember
{
namespace OgreView
{
namespace Terrain
{

/**
 * @brief An ITerrainObserver implementation which can be used to observe the Ogre Terrain component.
 * Instances are managed by OgreTerrainAdapter.
 */
class OgreTerrainObserver: public Ember::OgreView::Terrain::ITerrainObserver
{
public:
	/**
	 * @brief Usually you should call OgreTerrainAdapter::createObserver to create an instance of this observer.
	 * @param terrainAreaUpdatedSignal A valid signal that can be listened to for terrain updates.
	 */
	OgreTerrainObserver(sigc::signal<void, Ogre::TRect<Ogre::Real>>& terrainAreaUpdatedSignal);
	virtual ~OgreTerrainObserver();

	virtual void observeArea(const WFMath::AxisBox<2>& area);

	virtual void observeArea(const Ogre::TRect<int>& area);


	/**
	 * @brief Callback for terrain areas being updated. This is connected to mUpdateSignal in the constructor.
	 */
	virtual void terrainUpdated(const Ogre::TRect<Ogre::Real>& updatedArea);

private:

	/**
	 * @brief The area under observation.
	 */
	Ogre::TRect<Ogre::Real> mObservedArea;


	/**
	 * The connection to the updateSignal.
	 */
	sigc::connection mTerrainAreaUpdatedConnection;

};

} /* namespace Terrain */
} /* namespace OgreView */
} /* namespace Ember */
#endif /* OGRETERRAINOBSERVER_H_ */
