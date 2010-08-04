//
// C++ Implementation: TerrainEditor
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainEditor.h"
#include "TerrainEditorOverlay.h"
#include "TerrainManager.h"
#include "components/ogre/Scene.h"
#include <OgreSceneManager.h>

using namespace Ember;
namespace EmberOgre
{
namespace Terrain
{

TerrainEditor::TerrainEditor(TerrainManager& manager, Camera::MainCamera& camera) :
	mManager(manager), mCamera(camera), mOverlay(0), mVisible(false), mMovementRadiusInMeters(0), mFalloff(0)
{
}

TerrainEditor::~TerrainEditor()
{

}

void TerrainEditor::showOverlay()
{
	if (!mOverlay) {
		sigc::slot<void, std::map<int, std::map<int, Mercator::BasePoint> >&> slot = sigc::mem_fun(*this, &TerrainEditor::basepointsRecieved);
		mManager.getBasePoints(slot);
	}
}

void TerrainEditor::hideOverlay()
{
	delete mOverlay;
	mOverlay = 0;
}

bool TerrainEditor::isOverlayShown() const
{
	return mOverlay != 0;
}

float TerrainEditor::getRadius() const
{
	return mMovementRadiusInMeters;
}

void TerrainEditor::setRadius(float radiusInMeters)
{
	mMovementRadiusInMeters = radiusInMeters;
}

float TerrainEditor::getFalloff() const
{
	return mFalloff;
}

void TerrainEditor::setFalloff(float falloff)
{
	mFalloff = falloff;
}

void TerrainEditor::basepointsRecieved(std::map<int, std::map<int, Mercator::BasePoint> >& basePoints)
{
	delete mOverlay;
	mOverlay = new TerrainEditorOverlay(*this, mManager.getScene().getSceneManager(), *mManager.getScene().getSceneManager().getRootSceneNode(), mManager, mCamera, basePoints);
	EventOverlayCreated(*mOverlay);
}

}
}
