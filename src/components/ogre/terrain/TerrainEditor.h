
//
// C++ Interface: TerrainEditor
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006
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
#ifndef EMBEROGRETERRAINEDITOR_H
#define EMBEROGRETERRAINEDITOR_H

#include <sigc++/signal.h>
#include <map>
#include <sigc++/trackable.h>
#include <memory>

namespace Mercator {

class BasePoint;

}

namespace Ember {
class EmberEntity;
namespace OgreView {
namespace Camera {
class MainCamera;
}

namespace Terrain {
class TerrainManager;

class TerrainEditorOverlay;

/**
	@brief Provides terrain editing capabilities.

	Through this class base points in the terrain can be changed, and the changes sent to the server. The editor supports undo and redo functionality.
	If no radius is set, any action will only affect the base point being directly manipulated. If a radius however is set, other base points within the radius will also be affected.

	@note Inherits from sigc::trackable because we use an async callback when fetching base points.
	@author Erik Ogenvik <erik@ogenvik.org>
*/
class TerrainEditor : public virtual sigc::trackable {
public:

	/**
	 * @brief Ctor.
	 *
	 * @param manager The main terrain manager instance.
	 * @param camera The main camera used by Ember.
	 */
	TerrainEditor(TerrainManager& manager, Camera::MainCamera& camera);

	/**
	 * @brief Dtor.
	 */
	~TerrainEditor();

	/**
	 * @brief Shows the overlay.
	 */
	void showOverlay();

	/**
	 * @brief Hides the overlay.
	 */
	void hideOverlay();

	/**
	 * @brief Returns true if the overlay is currently shown.
	 * @return
	 */
	bool isOverlayShown() const;


	/**
	 * @brief Emitted when the overlay instance has been created.
	 */
	sigc::signal<void, TerrainEditorOverlay&> EventOverlayCreated;

	/**
	 * @brief Emitted when the overlay instance has been destroyed.
	 */
	sigc::signal<void> EventOverlayDestroyed;


	/**
	 * @brief Gets the radius of the circle within which base points are affected.
	 * @returns The radius, in world units, of the circle within which base points are affected.
	 */
	float getRadius() const;

	/**
	 * @brief Sets the radius of the circle within which base points are affected.
	 * @param radiusInMeters The radius, in world units, of the circle within which base points are affected.
	 */
	void setRadius(float radiusInMeters);


	float getFalloff() const;

	void setFalloff(float falloff);

private:


	/**
	 * @brief The main terrain manager instance of the scene.
	 */
	TerrainManager& mManager;

	/**
	 * @brief The main camera used by Ember for this scene.
	 */
	Camera::MainCamera& mCamera;

	/**
	 * @brief Handles the overlay, which is the graphical representation which shows the position of the base points and allows for manipulation.
	 *
	 * This will be lazily created when the overlay is first asked to be shown.
	 */
	std::unique_ptr<TerrainEditorOverlay> mOverlay;

	/**
	 * @brief The radius, in world units, within which base points will be affected.
	 */
	float mMovementRadiusInMeters;

	float mFalloff;

	/**
	 * @brief Callback for when the base points has been received from the terrain manager.
	 *
	 * @param basePoints A store of all base points.
	 */
	void basepointsRecieved(std::map<int, std::map<int, Mercator::BasePoint>>& basePoints);
};

}
}
}
#endif
