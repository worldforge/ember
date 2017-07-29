//
// C++ Implementation: TerrainEditor
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainEditor.h"
#include "TerrainEditorOverlay.h"
#include "TerrainManager.h"
#include "components/ogre/Scene.h"
#include <OgreSceneManager.h>

using namespace Ember;
namespace Ember
{
namespace OgreView
{
namespace Terrain
{

TerrainEditor::TerrainEditor(TerrainManager& manager, Camera::MainCamera& camera) :
	mManager(manager), mCamera(camera), mOverlay(0), mMovementRadiusInMeters(0), mFalloff(0)
{
}

TerrainEditor::~TerrainEditor()
{
	delete mOverlay;
}

void TerrainEditor::showOverlay()
{
	if (!mOverlay) {
		sigc::slot<void, std::map<int, std::map<int, Mercator::BasePoint>>&> slot = sigc::mem_fun(*this, &TerrainEditor::basepointsRecieved);
		mManager.getBasePoints(slot);
	} else {
		mOverlay->setVisible(true);
	}
}

void TerrainEditor::hideOverlay()
{
	if (mOverlay) {
		mOverlay->setVisible(false);
	}
}

bool TerrainEditor::isOverlayShown() const
{
	return mOverlay != 0 && mOverlay->getVisible();
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

void TerrainEditor::basepointsRecieved(std::map<int, std::map<int, Mercator::BasePoint>>& basePoints)
{
	if (mOverlay) {
		delete mOverlay;
		EventOverlayDestroyed.emit();
	}
	mOverlay = new TerrainEditorOverlay(*this, mManager.getScene().getSceneManager(), *mManager.getScene().getSceneManager().getRootSceneNode(), mManager, mCamera, basePoints);
	EventOverlayCreated(*mOverlay);
}

}
}
}
