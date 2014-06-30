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

#include "World.h"

#include "Avatar.h"
#include "MovementController.h"
#include "EmberEntityFactory.h"
#include "MotionManager.h"
#include "authoring/EntityMoveManager.h"
#include "Scene.h"
#include "EntityWorldPickListener.h"
#include "domain/EmberEntity.h"
#include "EmberOgreSignals.h"
#include "ForestRenderingTechnique.h"
#include "RenderDistanceManager.h"
#include "AvatarCameraWarper.h"
#include "components/ogre/AvatarCameraMotionHandler.h"
#include "components/ogre/authoring/AuthoringManager.h"
#include "components/ogre/authoring/AuthoringMoverConnector.h"

#include "components/ogre/lod/LodLevelManager.h"

#include "TerrainEntityManager.h"
#include "TerrainPageDataProvider.h"
#include "terrain/TerrainManager.h"
#include "terrain/TerrainHandler.h"
#include "terrain/ITerrainAdapter.h"

#include "camera/MainCamera.h"
#include "camera/ThirdPersonCameraMount.h"

#include "environment/Foliage.h"
#include "environment/FoliageDetailManager.h"
#include "environment/Environment.h"
#include "environment/CaelumEnvironment.h"
#include "environment/SimpleEnvironment.h"

#include "services/config/ConfigListenerContainer.h"
#include "services/EmberServices.h"
#include "services/config/ConfigService.h"

#include "framework/MainLoopController.h"

#include <Eris/Avatar.h>
#include <Eris/View.h>
#include <Eris/Calendar.h>
#include <Eris/EventService.h>
#include <Eris/Connection.h>

#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreRoot.h>

#include <sigc++/bind.h>

namespace Ember
{
namespace OgreView
{

World::World(Eris::View& view, Ogre::RenderWindow& renderWindow, Ember::OgreView::EmberOgreSignals& signals,
		Ember::Input& input, Ember::OgreView::ShaderManager& shaderManager, GraphicalChangeAdapter& graphicalChangeAdapter) :
		mView(view), mRenderWindow(renderWindow), mSignals(signals), mScene(new Scene()), mSbManager(EmberOgre::getSingleton().getSmartBodyManager())
		mViewport(renderWindow.addViewport(&mScene->getMainCamera())), mAvatar(0), mMovementController(0),
		mTerrainManager(new Terrain::TerrainManager(mScene->createTerrainAdapter(), *mScene, shaderManager, view.getAvatar()->getConnection()->getEventService())),
		mMainCamera(new Camera::MainCamera(mScene->getSceneManager(), mRenderWindow, input, mScene->getMainCamera(), *mTerrainManager->getTerrainAdapter())),
		mMoveManager(new Authoring::EntityMoveManager(*this)), mEmberEntityFactory(new EmberEntityFactory(view, *mScene)),
		mMotionManager(new MotionManager(mSbManager)), mAvatarCameraMotionHandler(0), mAvatarCameraWarper(nullptr),
		mEntityWorldPickListener(0), mAuthoringManager(new Authoring::AuthoringManager(*this)),
		mAuthoringMoverConnector(new Authoring::AuthoringMoverConnector(*mAuthoringManager, *mMoveManager)),
		mTerrainEntityManager(0), mLodLevelManager(new Lod::LodLevelManager(graphicalChangeAdapter, mScene->getMainCamera())),
		mFoliage(0), mFoliageDetailManager(0), mFoliageInitializer(0), mEnvironment(0), mConfigListenerContainer(new ConfigListenerContainer()), mCalendar(new Eris::Calendar(view.getAvatar()))
{
	mAfterTerrainUpdateConnection = mTerrainManager->getHandler().EventAfterTerrainUpdate.connect(sigc::mem_fun(*this, &World::terrainManager_AfterTerrainUpdate));

	mTerrainEntityManager = new TerrainEntityManager(view, mTerrainManager->getHandler(), mScene->getSceneManager());

	mPageDataProvider = new TerrainPageDataProvider(mTerrainManager->getHandler());
	mTerrainManager->getTerrainAdapter()->setPageDataProvider(mPageDataProvider);

	signals.EventTerrainManagerCreated.emit(*mTerrainManager);

	mEnvironment = new Environment::Environment(mScene->getSceneManager(), *mTerrainManager, new Environment::CaelumEnvironment(&mScene->getSceneManager(), &renderWindow, mScene->getMainCamera(), *mCalendar), new Environment::SimpleEnvironment(&mScene->getSceneManager(), &renderWindow, mScene->getMainCamera()));

	mScene->addRenderingTechnique("forest", new ForestRenderingTechnique(*mEnvironment->getForest()));
	mTerrainManager->getHandler().setLightning(mEnvironment);

	//set the background colour to black
	mViewport->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	mScene->getMainCamera().setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));

	signals.EventMotionManagerCreated.emit(*mMotionManager);
	Ogre::Root::getSingleton().addFrameListener(mMotionManager);

	//When calling Eris::View::registerFactory ownership is transferred
	view.registerFactory(mEmberEntityFactory);
	signals.EventCreatedEmberEntityFactory.emit(*mEmberEntityFactory);

	view.getAvatar()->GotCharacterEntity.connect(sigc::mem_fun(*this, &World::View_gotAvatarCharacter));

	mEntityWorldPickListener = new EntityWorldPickListener(mView, *mScene);
	mMainCamera->pushWorldPickListener(mEntityWorldPickListener);

	mConfigListenerContainer->registerConfigListener("graphics", "foliage", sigc::bind<-1>(sigc::mem_fun(*this, &World::Config_Foliage), sigc::ref(graphicalChangeAdapter)));

	mRenderDistanceManager = new RenderDistanceManager(graphicalChangeAdapter, *(mEnvironment->getFog()), mScene->getMainCamera());
}

World::~World()
{
	mAfterTerrainUpdateConnection.disconnect();

	delete mFoliageInitializer;
	delete mConfigListenerContainer;
	delete mTerrainEntityManager;
	mSignals.EventTerrainManagerBeingDestroyed();
	delete mTerrainManager;
	mSignals.EventTerrainManagerDestroyed();
	delete mAuthoringMoverConnector;
	delete mAuthoringManager;

	delete mMainCamera;
	delete mEntityWorldPickListener;
	delete mMoveManager;

	//The factory will be deleted by the mWorldView when that is deleted later on, so we shall not delete it here
	// 	delete mEmberEntityFactory;
	delete mAvatarCameraWarper;
	delete mMovementController;
	delete mAvatar;
	delete mAvatarCameraMotionHandler;

	Ogre::Root::getSingleton().removeFrameListener(mMotionManager);
	delete mMotionManager;
	mSignals.EventMotionManagerDestroyed();

	delete mRenderDistanceManager;

	ISceneRenderingTechnique* technique = mScene->removeRenderingTechnique("forest");
	delete technique;
	delete mFoliageDetailManager;
	delete mFoliage;
	delete mEnvironment;

	delete mLodLevelManager;

	delete mScene;
	delete mPageDataProvider;
	delete mCalendar;
}

Eris::View& World::getView() const
{
	return mView;
}

Eris::EventService& World::getEventService() const
{
	return mView.getAvatar()->getConnection()->getEventService();
}


Scene& World::getScene() const
{
	assert(mScene);
	return *mScene;
}

Ogre::SceneManager& World::getSceneManager() const
{
	return mScene->getSceneManager();
}

Avatar* World::getAvatar() const
{
	return mAvatar;
}

MotionManager& World::getMotionManager() const
{
	assert(mMotionManager);
	return *mMotionManager;
}

Camera::MainCamera& World::getMainCamera() const
{
	assert(mMainCamera);
	return *mMainCamera;
}

EmberEntityFactory& World::getEntityFactory() const
{
	assert(mEmberEntityFactory);
	return *mEmberEntityFactory;
}

MovementController* World::getMovementController() const
{
	return mMovementController;
}

EmberEntity* World::getEmberEntity(const std::string & eid) const
{
	return static_cast<EmberEntity*>(mView.getEntity(eid));
}

EntityWorldPickListener& World::getEntityPickListener() const
{
	assert(mEntityWorldPickListener);
	return *mEntityWorldPickListener;
}

Authoring::AuthoringManager& World::getAuthoringManager() const
{
	assert(mAuthoringManager);
	//This can never be null.
	return *mAuthoringManager;
}

Terrain::TerrainManager& World::getTerrainManager() const
{
	assert(mTerrainManager);
	return *mTerrainManager;
}

Lod::LodLevelManager& World::getLodLevelManager() const
{
	assert(mLodLevelManager);
	return *mLodLevelManager;
}

RenderDistanceManager& World::getRenderDistanceManager() const
{
	assert(mRenderDistanceManager);
	return *mRenderDistanceManager;
}

Environment::Environment* World::getEnvironment() const
{
	return mEnvironment;
}

Environment::Foliage* World::getFoliage() const
{
	return mFoliage;
}

Eris::Calendar& World::getCalendar() const
{
	assert(mCalendar);
	return *mCalendar;
}

void World::terrainManager_AfterTerrainUpdate(const std::vector<WFMath::AxisBox<2>>& areas, const std::set<Terrain::TerrainPage*>& pages)
{
	EmberEntity* emberEntity = static_cast<EmberEntity*>(mView.getTopLevel());
	if (emberEntity) {
		updateEntityPosition(emberEntity, areas);
	}
}

void World::updateEntityPosition(EmberEntity* entity, const std::vector<WFMath::AxisBox<2>>& areas)
{
	entity->adjustPosition();
	for (unsigned int i = 0; i < entity->numContained(); ++i) {
		EmberEntity* containedEntity = static_cast<EmberEntity*>(entity->getContained(i));
		updateEntityPosition(containedEntity, areas);
	}
}

void World::View_gotAvatarCharacter(Eris::Entity* entity)
{
	if (entity) {
		EmberEntity& emberEntity = static_cast<EmberEntity&>(*entity);
		//Set up the third person avatar camera and switch to it.
		mAvatar = new Avatar(emberEntity, *mScene, mMainCamera->getCameraSettings(), *(mTerrainManager->getTerrainAdapter()));
		mAvatarCameraMotionHandler = new AvatarCameraMotionHandler(*mAvatar);
		mAvatar->getCameraMount().setMotionHandler(mAvatarCameraMotionHandler);
		mMovementController = new MovementController(*mAvatar, *mMainCamera, *mTerrainManager);
		mMainCamera->setMovementProvider(mMovementController);
		mMainCamera->attachToMount(&mAvatar->getCameraMount());

		if (mAvatar->isAdmin()) {
			mAvatarCameraWarper = new AvatarCameraWarper(*mMovementController, *mMainCamera, mView);
		}

		emberEntity.BeingDeleted.connect(sigc::mem_fun(*this, &World::avatarEntity_BeingDeleted));

		mSignals.EventMovementControllerCreated.emit();
		mSignals.EventCreatedAvatarEntity.emit(emberEntity);
		mTerrainManager->startPaging();
		EventGotAvatar();
	} else {
		S_LOG_CRITICAL("Somehow got a null avatar entity.");
	}
}

void World::avatarEntity_BeingDeleted()
{
	delete mAvatarCameraWarper;
	mAvatarCameraWarper = nullptr;
	mMainCamera->attachToMount(0);
	mMainCamera->setMovementProvider(0);
	delete mMovementController;
	mMovementController = 0;
	mSignals.EventMovementControllerDestroyed.emit();
	delete mAvatarCameraMotionHandler;
	mAvatarCameraMotionHandler = 0;
	delete mAvatar;
	mAvatar = 0;
}

void World::Config_Foliage(const std::string& section, const std::string& key, varconf::Variable& variable, GraphicalChangeAdapter& graphicalChangeAdapter)
{
	if (variable.is_bool() && static_cast<bool>(variable)) {
		if (!mFoliage) {
			//create the foliage
			mFoliage = new Environment::Foliage(*mTerrainManager);
			EventFoliageCreated.emit();
			mFoliageInitializer = new DelayedFoliageInitializer(sigc::bind(sigc::mem_fun(*this, &World::initializeFoliage), sigc::ref(graphicalChangeAdapter)), mView, 1000, 15000);
		}
	} else {
		delete mFoliageDetailManager;
		mFoliageDetailManager = 0;
		delete mFoliageInitializer;
		mFoliageInitializer = 0;
		delete mFoliage;
		mFoliage = 0;
	}
}

void World::initializeFoliage(GraphicalChangeAdapter& graphicalChangeAdapter)
{
	if (mFoliage) {
		mFoliage->initialize();
		mFoliageDetailManager = new Environment::FoliageDetailManager(*mFoliage, graphicalChangeAdapter);
		mFoliageDetailManager->initialize();
	}
}

DelayedFoliageInitializer::DelayedFoliageInitializer(sigc::slot<void> callback, Eris::View& view, unsigned int intervalMs, unsigned int maxTimeMs) :
		mCallback(callback), mView(view), mIntervalMs(intervalMs), mMaxTimeMs(maxTimeMs), mTimeout(new Eris::TimedEvent(view.getAvatar()->getConnection()->getEventService(), boost::posix_time::milliseconds(intervalMs), [&](){this->timout_Expired();})), mTotalElapsedTime(0)
{
	//don't load the foliage directly, instead wait some seconds for all terrain areas to load
	//the main reason is that new terrain areas will invalidate the foliage causing a reload
	//by delaying the foliage we can thus in most cases avoid those reloads
	//wait three seconds
}

DelayedFoliageInitializer::~DelayedFoliageInitializer()
{
	delete mTimeout;
}

void DelayedFoliageInitializer::timout_Expired()
{
	//load the foliage if either all queues entities have been loaded, or 15 seconds has elapsed
	if (mView.lookQueueSize() == 0 || mTotalElapsedTime > mMaxTimeMs) {
		mCallback();
	} else {
		mTotalElapsedTime += mIntervalMs;
		mTimeout = new Eris::TimedEvent(mView.getAvatar()->getConnection()->getEventService(), boost::posix_time::milliseconds(mIntervalMs), [&](){this->timout_Expired();});
	}
}

}
}
