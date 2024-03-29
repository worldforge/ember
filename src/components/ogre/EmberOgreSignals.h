/*
 Copyright (C) 2010 Erik Ogenvik <erik@ogenvik.org>

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

#ifndef EMBEROGRE_EMBEROGRESIGNALS_H_
#define EMBEROGRE_EMBEROGRESIGNALS_H_

#include <sigc++/signal.h>

namespace Ember
{
class EmberEntity;
namespace OgreView
{

namespace Terrain
{
class TerrainManager;
}

class GUIManager;
class MotionManager;
class EmberEntityFactory;
class World;

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief The signals which are emitted by the EmberOgre class.
 *
 * These signals are emitted by the EmberOgre class. Since an instance of EmberOgre forms the base of the Ogre component of Ember, many of these signals are of use by subcomponents, both in their lifecycle as well as on frame render basis.
 */
struct EmberOgreSignals
{
	/**
	 * @brief Emitted when the avatar entity has been created.
	 */
	sigc::signal<void(EmberEntity&)> EventCreatedAvatarEntity;

	/**
	 * @brief Emitted after the GUIManager has been created, but not yet initialized.
	 */
	sigc::signal<void(GUIManager&)> EventGUIManagerCreated;

	/**
	 * @brief Emitted before the GUIManager is destroyed.
	 *
	 * Right after this has been emitted a garbage collection sweep of all scripting provider will occur.
	 * This makes this signal a suitable one to listen to if you want to shut down your widgets.
	 */
	sigc::signal<void()> EventGUIManagerBeingDestroyed;

	/**
	 * @brief Emitted after the Motion has been created.
	 */
	sigc::signal<void(MotionManager&)> EventMotionManagerCreated;

	/**
	 * @brief Emitted after the TerrainManager has been created.
	 */
	sigc::signal<void(Terrain::TerrainManager&)> EventTerrainManagerCreated;

	/**
	 * @brief Emitted right before the TerrainManager is destroyed.
	 */
	sigc::signal<void()> EventTerrainManagerBeingDestroyed;

	/**
	 * @brief Emitted after the TerrainManager has been destroyed.
	 */
	sigc::signal<void()> EventTerrainManagerDestroyed;

	/**
	 * @brief Emitted after the MotionManager has been destroyed.
	 */
	sigc::signal<void()> EventMotionManagerDestroyed;

	/**
	 * @brief Emitted after the MovementController has been created.
	 */
	sigc::signal<void()> EventMovementControllerCreated;

	/**
	 * @brief Emitted after the MovementController has been destroyed.
	 */
	sigc::signal<void()> EventMovementControllerDestroyed;

	/**
	 * @brief Emitted before the main Ogre render window is rendered.
	 */
	sigc::signal<void()> EventBeforeRender;

	/**
	 * @brief Emitted after the main Ogre render window is rendered.
	 */
	sigc::signal<void()> EventAfterRender;

	/**
	 * @brief Emitted after a World instance has been created.
	 */
	sigc::signal<void(World&)> EventWorldCreated;

	/**
	 * @brief Emitted before a World instance has been destroyed.
	 */
	sigc::signal<void()> EventWorldBeingDestroyed;

	/**
	 * @brief Emitted after a World instance has been destroyed.
	 */
	sigc::signal<void()> EventWorldDestroyed;

	sigc::signal<void(const std::string&)> EventResourceLoaded;

};
}

}

#endif /* EMBEROGRE_EMBEROGRESIGNALS_H_ */
