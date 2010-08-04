/*
 Copyright (C) 2010 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#ifndef EMBEROGRE_EMBEROGRESIGNALS_H_
#define EMBEROGRE_EMBEROGRESIGNALS_H_

#include <sigc++/signal.h>

namespace EmberOgre
{

namespace Terrain {
class TerrainManager;
}

class GUIManager;
class MotionManager;
class EmberEntity;
class EmberEntityFactory;
class World;

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief The signals which are emitted by the EmberOgre class.
 *
 * These signals are emitted by the EmberOgre class. Since an instance of EmberOgre forms the base of the Ogre component of Ember, many of these signals are of use by subcomponents, both in their lifecycle as well as on frame render basis.
 */
class EmberOgreSignals {
public:
	virtual ~EmberOgreSignals(){}

	/**
	 * @brief Emitted when the Ember entity factory has been created.
	 */
	sigc::signal<void, EmberEntityFactory&> EventCreatedEmberEntityFactory;

	/**
	 * @brief Emitted when the avatar entity has been created.
	 */
	sigc::signal<void, EmberEntity&> EventCreatedAvatarEntity;

	/**
	 * @brief Emitted after the GUIManager has been created, but not yet initialized.
	 */
	sigc::signal<void, GUIManager&> EventGUIManagerCreated;

	/**
	 * @brief Emitted after the GUIManager has been initialized.
	 */
	sigc::signal<void, GUIManager&> EventGUIManagerInitialized;

	/**
	 * @brief Emitted after the Motion has been created.
	 */
	sigc::signal<void, MotionManager&> EventMotionManagerCreated;

	/**
	 * @brief Emitted after the TerrainManager has been created.
	 */
	sigc::signal<void, Terrain::TerrainManager&> EventTerrainManagerCreated;

	/**
	 * @brief Emitted after the TerrainManager has been destroyed.
	 */
	sigc::signal<void> EventTerrainManagerDestroyed;

	/**
	 * @brief Emitted after the MovementController has been created.
	 */
	sigc::signal<void> EventMovementControllerCreated;

	/**
	 * @brief Emitted after the MovementController has been destroyed.
	 */
	sigc::signal<void> EventMovementControllerDestroyed;

	/**
	 * @brief Emitted before the main Ogre render window is rendered.
	 */
	sigc::signal<void> EventBeforeRender;

	/**
	 * @brief Emitted after the main Ogre render window is rendered.
	 */
	sigc::signal<void> EventAfterRender;

	/**
	 * @brief Emitted after a World instance has been created.
	 */
	sigc::signal<void, World&> EventWorldCreated;

	/**
	 * @brief Emitted after a World instance has been destroyed.
	 */
	sigc::signal<void> EventWorldDestroyed;

};
}

#endif /* EMBEROGRE_EMBEROGRESIGNALS_H_ */
