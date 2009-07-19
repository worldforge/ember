/*
    Copyright (C) 2004  Erik Hjortsberg

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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "MainCamera.h"

#include "ICameraMount.h"
#include "Recorder.h"

#include "components/ogre/AvatarTerrainCursor.h"

#include "components/ogre/Convert.h"
#include "components/ogre/ICameraMotionHandler.h"
#include "components/ogre/IMovementProvider.h"
#include "components/ogre/IWorldPickListener.h"
#include "components/ogre/MousePicker.h"



//#include "services/time/TimeService.h"
//#ifndef WIN32
//#include "services/sound/SoundService.h"
//#endif

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/input/Input.h"


#include "framework/ConsoleBackend.h"
#include "framework/osdir.h"


#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreCompositorManager.h>
#include <OgreRenderSystem.h>
#include <OgreRenderSystemCapabilities.h>
#include <OgreSceneQuery.h>

#include <OgreMath.h>
#include <OgreRay.h>
#include <OgreVector3.h>

#ifdef __WIN32__
#include <windows.h>
#include <direct.h>
#endif

using namespace Ember;
namespace EmberOgre {

namespace Camera
{

MainCamera::MainCamera(Ogre::SceneManager& sceneManager, Ogre::RenderWindow& window, Input& input, Ogre::Camera& camera)
:	ToggleRendermode("toggle_rendermode", this, "Toggle between wireframe and solid render modes."),
	ToggleFullscreen("toggle_fullscreen", this, "Switch between windowed and full screen mode."),
	Screenshot("screenshot", this, "Take a screenshot and write to disk."),
	Record("+record", this, "Record to disk."),
	mCamera(camera),
	mCameraMount(0),
	mRecorder(new Recorder()),
	mWindow(window),
	mClosestPickingDistance(10000),
	mCameraRaySceneQuery(0),
	mAvatarTerrainCursor(new AvatarTerrainCursor(camera)),
	mCameraOrientationChangedThisFrame(false),
	mMovementProvider(0)
{
	mCamera.setNearClipDistance(0.5);

	///set the far clip distance high to make sure that the sky is completely shown
	if (Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
	{
/*		//NOTE: this won't currently work with the sky
		mCamera.setFarClipDistance(0);*/

		mCamera.setFarClipDistance(10000);
	} else {
		mCamera.setFarClipDistance(10000);
	}

	createRayQueries(sceneManager);

	/// Register this as a frame listener
	Ogre::Root::getSingleton().addFrameListener(this);

	input.EventMouseMoved.connect(sigc::mem_fun(*this, &MainCamera::Input_MouseMoved));

}

MainCamera::~MainCamera()
{

}

Ogre::Camera& MainCamera::getCamera()
{
	return mCamera;
}

Ogre::Camera& MainCamera::getCamera() const
{
	return mCamera;
}

AvatarTerrainCursor& MainCamera::getTerrainCursor() const
{
	return *mAvatarTerrainCursor.get();
}

const Ogre::Quaternion& MainCamera::getOrientation(bool onlyHorizontal) const {
	if (!onlyHorizontal) {
		return mCamera.getDerivedOrientation();
	} else {
		static Ogre::Quaternion quat;
		quat = mCamera.getDerivedOrientation();
		quat.x = 0;
		quat.z = 0;
		return quat;
	}
}

const Ogre::Vector3& MainCamera::getPosition() const
{
	return mCamera.getDerivedPosition();
}

void MainCamera::markCameraNodeAsDirty()
{
	if (mCamera.getParentNode()) {
		///We need to mark the parent node of the camera as dirty. The update of the derived orientation and position of the node should normally occur when the scene tree is traversed, but in some instances we need to access the derived position or orientataion of the camera before the traversal occurs, and if we don't mark the node as dirty it won't be updated
		mCamera.getParentNode()->needUpdate(true);
	}
}

void MainCamera::pickInWorld(Ogre::Real mouseX, Ogre::Real mouseY, const MousePickerArgs& mousePickerArgs)
{
	S_LOG_INFO("Trying to pick an entity at mouse coords: "  << Ogre::StringConverter::toString(mouseX) << ":" << Ogre::StringConverter::toString(mouseY) << ".");

	// get the terrain vector for mouse coords when a pick event happens
 	//mAvatarTerrainCursor->getTerrainCursorPosition();

	/// Start a new ray query
	Ogre::Ray cameraRay = getCamera().getCameraToViewportRay( mouseX, mouseY );

	mCameraRaySceneQuery->setRay(cameraRay);
	mCameraRaySceneQuery->execute();


	///now check the entity picking
	Ogre::RaySceneQueryResult& queryResult = mCameraRaySceneQuery->getLastResults();
	bool continuePicking = true;

	for (WorldPickListenersStore::iterator I = mPickListeners.begin(); I != mPickListeners.end(); ++I) {
		(*I)->initializePickingContext();
	}


	Ogre::RaySceneQueryResult::iterator rayIterator = queryResult.begin( );
	Ogre::RaySceneQueryResult::iterator rayIterator_end = queryResult.end( );
	if (rayIterator != rayIterator_end) {
		for ( ; rayIterator != rayIterator_end && continuePicking; rayIterator++ ) {
			for (WorldPickListenersStore::iterator I = mPickListeners.begin(); I != mPickListeners.end(); ++I) {
				(*I)->processPickResult(continuePicking, *rayIterator, cameraRay, mousePickerArgs);
				if (!continuePicking) {
					break;
				}
			}
		}
	}

	for (WorldPickListenersStore::iterator I = mPickListeners.begin(); I != mPickListeners.end(); ++I) {
		(*I)->endPickingContext(mousePickerArgs);
	}
}

void MainCamera::setClosestPickingDistance(Ogre::Real distance)
{
	mClosestPickingDistance = distance;
}
Ogre::Real MainCamera::getClosestPickingDistance() const
{
	return mClosestPickingDistance;
}

void MainCamera::setMovementProvider(IMovementProvider* movementProvider)
{
	mMovementProvider = movementProvider;
}


bool MainCamera::worldToScreen(const Ogre::Vector3& worldPos, Ogre::Vector2& screenPos)
{
	Ogre::Vector3 hcsPosition = mCamera.getProjectionMatrix() * (mCamera.getViewMatrix() * worldPos);

	if ((hcsPosition.x < -1.0f) ||
	(hcsPosition.x > 1.0f) ||
	(hcsPosition.y < -1.0f) ||
	(hcsPosition.y > 1.0f))
	return false;


	screenPos.x = (hcsPosition.x + 1) * 0.5;
	screenPos.y = (-hcsPosition.y + 1) * 0.5;

	return true;}


void MainCamera::runCommand(const std::string &command, const std::string &args)
{
	if(Screenshot == command) {
		//just take a screen shot
		takeScreenshot();
	} else if (ToggleFullscreen == command){
		SDL_WM_ToggleFullScreen(SDL_GetVideoSurface());

	} else if (ToggleRendermode == command) {
		toggleRenderMode();
	} else if (Record == command) {
		mRecorder->startRecording();
	} else if (Record.getInverseCommand() == command) {
		mRecorder->stopRecording();
	}
}

bool MainCamera::frameStarted(const Ogre::FrameEvent& event)
{
	if (mCameraMount) {
		if (mMovementProvider) {
			WFMath::Vector<3> movement = mMovementProvider->getMovementForCurrentFrame();
			if (movement != WFMath::Vector<3>::ZERO() || mCameraOrientationChangedThisFrame) {
				mCameraMount->move(movement, Convert::toWF(getOrientation()));
			}
		} else {
			mCameraMount->move(WFMath::Vector<3>::ZERO(), Convert::toWF(getOrientation()));
		}
		mCameraOrientationChangedThisFrame = false;
	}
	return true;
}

void MainCamera::Input_MouseMoved(const Ember::MouseMotion& motion, Ember::Input::InputMode mode)
{
	if (mCameraMount) {
		if (mode == Input::IM_MOVEMENT) {

			bool moved = false;
			if (motion.xRelativeMovement) {
				moved = mCameraMount->yaw(motion.xRelativeMovement).valueDegrees() || moved;
			}
			if (motion.yRelativeMovement) {
				moved = mCameraMount->pitch(motion.yRelativeMovement).valueDegrees() || moved;
			}

			if (moved) {
				markCameraNodeAsDirty();
				mCameraOrientationChangedThisFrame = true;
				MovedCamera.emit(mCamera);
			}
		}
	}
}

void MainCamera::enableCompositor(const std::string& compositorName, bool enable)
{
	if (std::find(mLoadedCompositors.begin(), mLoadedCompositors.end(), compositorName) == mLoadedCompositors.end()) {
		Ogre::CompositorManager::getSingleton().addCompositor(mWindow.getViewport(0), compositorName);
	}
	Ogre::CompositorManager::getSingleton().setCompositorEnabled(mWindow.getViewport(0), compositorName, enable);
}

void MainCamera::pushWorldPickListener(IWorldPickListener* worldPickListener)
{
	mPickListeners.push_front(worldPickListener);

}

void MainCamera::removeWorldPickListener(IWorldPickListener* worldPickListener)
{
	if (worldPickListener) {
		WorldPickListenersStore::iterator I = std::find(mPickListeners.begin(), mPickListeners.end(), worldPickListener);
		if (I != mPickListeners.end()) {
			mPickListeners.erase(I);
		}
	}
}

void MainCamera::toggleRenderMode()
{
	S_LOG_INFO("Switching render mode.");
	if (mCamera.getPolygonMode() == Ogre::PM_SOLID) {
		mCamera.setPolygonMode(Ogre::PM_WIREFRAME);
	} else {
		mCamera.setPolygonMode(Ogre::PM_SOLID);
	}

}


const std::string MainCamera::_takeScreenshot()
{
	// retrieve current time
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	// construct filename string
	// padding with 0 for single-digit values
	std::stringstream filename;
	filename << "screenshot_" << ((*timeinfo).tm_year + 1900); // 1900 is year "0"
	int month = ((*timeinfo).tm_mon + 1); // January is month "0"
	if(month <= 9)
	{
		filename << "0";
	}
	filename << month;
	int day = (*timeinfo).tm_mday;
	if(day <= 9)
	{
		filename << "0";
	}
	filename << day << "_";
	int hour = (*timeinfo).tm_hour;
	if(hour <= 9)
	{
		filename << "0";
	}
	filename << hour;
	int min = (*timeinfo).tm_min;
	if(min <= 9)
	{
		filename << "0";
	}
	filename << min;
	int sec = (*timeinfo).tm_sec;
	if(sec <= 9)
	{
		filename << "0";
	}
	filename << sec << ".jpg";

	const std::string dir = Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory() + "/screenshots/";
	try {
		//make sure the directory exists

		oslink::directory osdir(dir);

		if (!osdir.isExisting()) {
#ifdef __WIN32__
			mkdir(dir.c_str());
#else
			mkdir(dir.c_str(), S_IRWXU);
#endif
		}
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when creating directory for screenshots. Message: " << std::string(ex.what()));
		throw Ember::Exception("Error when saving screenshot. Message: " + std::string(ex.what()));
	}

	try {
		// take screenshot
		mWindow.writeContentsToFile(dir + filename.str());
	} catch (const Ogre::Exception& ex) {
		S_LOG_FAILURE("Could not write screenshot to disc. Message: "<< ex.getFullDescription());
		throw Ember::Exception("Error when saving screenshot. Message: " + ex.getDescription());
	}
	return dir + filename.str();
}

void MainCamera::takeScreenshot()
{
	try {
		const std::string& result = _takeScreenshot();
		S_LOG_INFO(result);
		Ember::ConsoleBackend::getSingletonPtr()->pushMessage("Wrote image: " + result);
	} catch (const std::exception& ex) {
		Ember::ConsoleBackend::getSingletonPtr()->pushMessage(std::string("Error when saving screenshot: ") + ex.what());
	} catch (...) {
		Ember::ConsoleBackend::getSingletonPtr()->pushMessage("Unknown error when saving screenshot.");
	}
}

void MainCamera::createRayQueries(Ogre::SceneManager& sceneManager)
{
	unsigned long queryMask = Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK;
	queryMask |= MousePicker::CM_AVATAR;
	queryMask |= MousePicker::CM_ENTITY;
	queryMask |= MousePicker::CM_NATURE;
	queryMask |= MousePicker::CM_UNDEFINED;
// 	queryMask |= Ogre::RSQ_FirstTerrain;

	mCameraRaySceneQuery = sceneManager.createRayQuery( Ogre::Ray(), queryMask);
	mCameraRaySceneQuery->setWorldFragmentType(Ogre::SceneQuery::WFT_SINGLE_INTERSECTION);
	mCameraRaySceneQuery->setSortByDistance(true);
}

ICameraMount* MainCamera::attachToMount(ICameraMount* newCameraMount)
{
	if (mCameraMount) {
		mCameraMount->detachFromCamera();
	}
	mCameraMount = newCameraMount;
	if (mCameraMount) {
		mCameraMount->attachToCamera(*this);
	}
}

}

}
