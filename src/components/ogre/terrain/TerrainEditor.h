
//
// C++ Interface: TerrainEditor
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
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
#ifndef EMBEROGRETERRAINEDITOR_H
#define EMBEROGRETERRAINEDITOR_H

#include <sigc++/signal.h>
#include <map>


namespace Mercator {

class BasePoint;

}

namespace EmberOgre {

class EmberEntity;
namespace Camera
{
class MainCamera;
}

namespace Terrain {
class TerrainManager;
class TerrainEditorOverlay;

/**
	Provides terrain editing capabilities. Through this class base points in the terrain can be changed, and the changes sent to the server. The editor supports undo and redo functionality.
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class TerrainEditor
{
public:
    TerrainEditor(TerrainManager& manager, Camera::MainCamera& camera);

    ~TerrainEditor();

    /**
     * Shows the overlay.
     */
    void showOverlay();
    /**
     * Hides the overlay.
     */
    void hideOverlay();

	/**
	 *    Returns true if the overlay is currently shown.
	 * @return
	 */
	bool isOverlayShown() const;


	sigc::signal<void, TerrainEditorOverlay&> EventOverlayCreated;



	float getRadius() const;
	void setRadius(float radiusInMeters);


	float getFalloff() const;
	void setFalloff(float falloff);



private:


	TerrainManager& mManager;

	Camera::MainCamera& mCamera;

	TerrainEditorOverlay* mOverlay;

	bool mVisible;

	float mMovementRadiusInMeters;
	float mFalloff;


	void basepointsRecieved(std::map<int, std::map<int, Mercator::BasePoint> >& basePoints);
};

}
}
#endif
