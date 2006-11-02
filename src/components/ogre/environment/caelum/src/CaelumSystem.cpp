#include "CaelumSystem.h"

namespace caelum {

const Ogre::String CaelumSystem::SKY_DOME_MATERIAL_NAME = "CaelumSkyDomeMaterial";
const Ogre::String CaelumSystem::STARFIELD_MATERIAL_NAME = "CaelumStarfieldMaterial";

CaelumSystem::CaelumSystem (Ogre::Root *root, Ogre::SceneManager *sceneMgr) {
	LOG ("Initialising Caelum system...");
	mOgreRoot = root;
	mSceneMgr = sceneMgr;

	mOgreRoot->addFrameListener (this);

	createSkyDomeMaterial ();
	createStarfieldMaterial ();

	mUpdateRate = 0;
	mTimeSinceLastUpdate = 0;

	mTotalDayTime = 24 * 59 * 56;
	mTimeScale = 1.0f;
	mLocalTime = 0.0f;

	mManageFog = false;

	mSun = new Sun (sceneMgr);

	mStarfield = 0;

	LOG ("DONE");
}

CaelumSystem::~CaelumSystem () {
	LOG ("Shutting down Caelum system...");
	// Unregister all the listeners
	mListeners.clear ();

	// Remove all the sky domes
	std::set<SkyDome *>::iterator sdIt, sdIend = mSkyDomes.end ();
	for (sdIt = mSkyDomes.begin (); sdIt != sdIend; ++sdIt) {
		delete *sdIt;
	}
	mSkyDomes.clear ();

	destroyStarfield ();

	delete mSun;
	mSun = 0;

	mOgreRoot->removeFrameListener (this);
	LOG ("DONE");
}

void CaelumSystem::addListener (CaelumListener *listener) {
	mListeners.insert (listener);
}

void CaelumSystem::removeListener (CaelumListener *listener) {
	mListeners.erase (listener);
}

void CaelumSystem::registerAllToTarget (Ogre::RenderTarget *target) {
	std::set<Ogre::RenderTargetListener *> *list = collectAllRenderTargetListeners ();

	std::set<Ogre::RenderTargetListener *>::iterator it, iend = list->end ();
	for (it = list->begin (); it != iend; ++it) {
		target->addListener (*it);
	}

	delete list;
}

void CaelumSystem::unregisterAllFromTarget (Ogre::RenderTarget *target) {
	std::set<Ogre::RenderTargetListener *> *list = collectAllRenderTargetListeners ();

	std::set<Ogre::RenderTargetListener *>::iterator it, iend = list->end ();
	for (it = list->begin (); it != iend; ++it) {
		target->removeListener (*it);
	}

	delete list;
}

void CaelumSystem::setUpdateRate (float rate) {
	if (rate < 0)
		rate = 0;

	mUpdateRate = rate;

	// Force an update when changing this
	mTimeSinceLastUpdate = mUpdateRate + 1;
}

float CaelumSystem::getUpdateRate () {
	return mUpdateRate;
}

void CaelumSystem::setTimeScale (const float scale) {
	mTimeScale = scale;
}

float CaelumSystem::getTimeScale () const {
	return mTimeScale;
}

void CaelumSystem::setLocalTime (const float time) {
	mLocalTime = time;

	// Force an update when changing this
	mTimeSinceLastUpdate = mUpdateRate + 1;
}

float CaelumSystem::getLocalTime () const {
	return mLocalTime;
}

void CaelumSystem::setTotalDayTime (const float time) {
	mTotalDayTime = time;

	if (mTotalDayTime <= 0)
		mTotalDayTime = 24 * 60 * 60 - 4;

	// Force an update when changing this
	mTimeSinceLastUpdate = mUpdateRate + 1;
}

float CaelumSystem::getTotalDayTime () const {
	return mTotalDayTime;
}

void CaelumSystem::setSunDirection (Ogre::Vector3 dir) {
	if (!mSkyDomeMaterial.isNull ()) {
		if (mOgreRoot->getRenderSystem ()->getCapabilities ()->hasCapability (Ogre::RSC_VERTEX_PROGRAM)) {
			mSkyDomeMaterial->getTechnique (0)->getPass (0)->getVertexProgramParameters ()->setNamedConstant ("sunDirection", dir);
		}
	}
}

bool CaelumSystem::frameStarted (const Ogre::FrameEvent &e) {
	// Call every listener before doing anything
	if (!fireStartedEvent (e))
		return false;

	mLocalTime += e.timeSinceLastFrame * mTimeScale;
	if (mLocalTime > mTotalDayTime) {
		clampLocalTime ();
	}

	// Update only if required
	mTimeSinceLastUpdate += (e.timeSinceLastFrame * Ogre::Math::Abs (mTimeScale)) / mTotalDayTime;
	if (mTimeSinceLastUpdate > mUpdateRate) {
		// Do it this way so we don't lose any seconds and the updates doesn't queue
		if (mUpdateRate != 0)
			mTimeSinceLastUpdate -= mUpdateRate * Ogre::Math::Floor (mTimeSinceLastUpdate / mUpdateRate);
		else
			mTimeSinceLastUpdate = 0;

		if (mSun) {
			mSun->update (mLocalTime / mTotalDayTime);
		}

		if (mStarfield) {
			mStarfield->update (mLocalTime / mTotalDayTime);
		}

		updateSkyDomeMaterialTime ();

		if (!mSkyDomeMaterial.isNull ()) {
			if (mOgreRoot->getRenderSystem ()->getCapabilities ()->hasCapability (Ogre::RSC_VERTEX_PROGRAM)) {
				mSkyDomeMaterial->getTechnique (0)->getPass (0)->getVertexProgramParameters ()->setNamedConstant ("sunDirection", mSun->getSunDirection ());
			}
		}

		if (mManageFog) {
			// TODO: Fog stuff here!!!
			if (mSkyColourModel) {
				mSceneMgr->setFog (Ogre::FOG_EXP2,
													mSkyColourModel->getFogColour (mLocalTime / mTotalDayTime) * 0.7,
													mSkyColourModel->getFogDensity (mLocalTime / mTotalDayTime));
			}
		}
	}

	mSun->setSunColour (mSceneMgr->getFogColour ());

	return fireFinishedEvent (e);
}

SkyDome *CaelumSystem::createSkyDome () {
	SkyDome *temp = new SkyDome (mSceneMgr);

	mSkyDomes.insert (temp);

	return temp;
}

void CaelumSystem::destroySkyDome (SkyDome *skyDome) {
	mSkyDomes.erase (skyDome);

	delete skyDome;
}

void CaelumSystem::setSkyColourModel (SkyColourModel *model) {
	mSkyColourModel = model;
	mSkyColourModel->setSkyGradientsTextureUnitState (mSkyDomeMaterial->getTechnique (0)->getPass (0)->getTextureUnitState (0));
}

void CaelumSystem::setManageFog (bool manage) {
	mManageFog = manage;
}

bool CaelumSystem::isFogManaged () const {
	return mManageFog;
}

Sun *CaelumSystem::getSun () const {
	return mSun;
}

Starfield *CaelumSystem::createStarfield (const Ogre::String &mapName) {
	if (mStarfield)
		destroyStarfield ();

	mStarfield = new Starfield (mSceneMgr);

	// Update the starfield material
	mStarfieldMaterial->getBestTechnique ()->getPass (0)->getTextureUnitState (0)->setTextureName (mapName);

	return mStarfield;
}

Starfield *CaelumSystem::getStarfield () const {
	return mStarfield;
}

void CaelumSystem::destroyStarfield () {
	if (mStarfield) {
		delete mStarfield;
		mStarfield = 0;
	}
}

bool CaelumSystem::fireStartedEvent (const Ogre::FrameEvent &e) {
	std::set<CaelumListener *>::iterator it, iend = mListeners.end ();
	bool flag = true;

	it = mListeners.begin ();

	while (it != iend && flag) {
		flag &= (*it)->caelumStarted (e, this);

		++it;
	}

	return flag;
}

bool CaelumSystem::fireFinishedEvent (const Ogre::FrameEvent &e) {
	std::set<CaelumListener *>::iterator it, iend = mListeners.end ();
	bool flag = true;

	it = mListeners.begin ();

	while (it != iend && flag) {
		flag &= (*it)->caelumFinished (e, this);

		++it;
	}

	return flag;
}

std::set<Ogre::RenderTargetListener *> *CaelumSystem::collectAllRenderTargetListeners () {
	std::set<Ogre::RenderTargetListener *> *list = new std::set<Ogre::RenderTargetListener *>();

	// Insert all the domes
	std::set<SkyDome *>::iterator sdIt, sdIend = mSkyDomes.end ();
	for (sdIt = mSkyDomes.begin (); sdIt != sdIend; ++sdIt) {
		list->insert (*sdIt);
	}

	// Insert the sun
	if (mSun) {
		list->insert (static_cast<Ogre::RenderTargetListener *>(mSun));
	}

	// Insert the starfield
	if (mStarfield) {
		list->insert (mStarfield);
	}

	return list;
}

void CaelumSystem::clampLocalTime () {
	if (mLocalTime < 0) {
		int ratio = (int )(mTotalDayTime / -mLocalTime);
		mLocalTime += mTotalDayTime * ratio;
	}
	if (mLocalTime > mTotalDayTime) {
		int ratio = (int )(mTotalDayTime / mLocalTime);
		mLocalTime -= mTotalDayTime * ratio;
	}
}

void CaelumSystem::createSkyDomeMaterial () {
	Ogre::MaterialPtr mat;

	LOG ("Generating sky dome material...");
	if (!Ogre::MaterialManager::getSingleton ().resourceExists (SKY_DOME_MATERIAL_NAME)) {
		LOG ("\tMaterial not found; creating...");
		mat = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton ().create (SKY_DOME_MATERIAL_NAME, "Caelum"));
		mat->setReceiveShadows (false);
		LOG ("\t\tMaterial [OK]");
		Ogre::Pass *pass = mat->getTechnique (0)->getPass (0);
		pass->setSceneBlending (Ogre::SBT_TRANSPARENT_ALPHA);
		pass->setDepthCheckEnabled (false);
		pass->setDepthWriteEnabled (false);
		pass->setLightingEnabled (false);
		pass->setFog (true);
		// Bind the sky light absorption shader if capable to
		// TODO
		if (mOgreRoot->getRenderSystem ()->getCapabilities ()->hasCapability (Ogre::RSC_FRAGMENT_PROGRAM)) {
			Ogre::HighLevelGpuProgramPtr fp = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram("SkyLightAbsorptionFP","Caelum","cg",Ogre::GPT_FRAGMENT_PROGRAM);
			fp->setSourceFile ("SkyLightAbsorption.cg");
			fp->setParameter ("entry_point", "main_fp");
			fp->setParameter ("profiles", "ps_2_0 arbfp1");
			pass->setFragmentProgram ("SkyLightAbsorptionFP");
			Ogre::GpuProgramParametersSharedPtr parameters = pass->getFragmentProgramParameters();
			parameters->setNamedConstant("offset", 0);
		}
		if (mOgreRoot->getRenderSystem ()->getCapabilities ()->hasCapability (Ogre::RSC_VERTEX_PROGRAM)) {
			Ogre::HighLevelGpuProgramPtr vp = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram("SkyLightAbsorptionVP","Caelum","cg",Ogre::GPT_VERTEX_PROGRAM);
			vp->setSourceFile ("SkyLightAbsorption.cg");
			vp->setParameter ("entry_point", "main_vp");
			vp->setParameter ("profiles", "vs_2_0 arbvp1");
			pass->setVertexProgram ("SkyLightAbsorptionVP");
			Ogre::GpuProgramParametersSharedPtr parameters = pass->getVertexProgramParameters();
			parameters->setNamedAutoConstant("worldViewProj", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
			parameters->setNamedConstant("sunDirection", Ogre::Vector3 (1, 0, 0));
		}
		LOG ("\t\tPass [OK]");
		Ogre::TextureUnitState *tus = pass->createTextureUnitState ();
		tus->setTextureAddressingMode (Ogre::TextureUnitState::TAM_WRAP, Ogre::TextureUnitState::TAM_CLAMP, Ogre::TextureUnitState::TAM_CLAMP);
		LOG ("\t\tTextureUnit [OK]");
		mat->load ();
		LOG ("\tDONE");
	}
	else {
		mat = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton ().getByName (SKY_DOME_MATERIAL_NAME));
	}
	LOG ("DONE");

	mSkyDomeMaterial = mat;
}

void CaelumSystem::createStarfieldMaterial () {
	Ogre::MaterialPtr mat;

	LOG ("Generating starfield material...");
	if (!Ogre::MaterialManager::getSingleton ().resourceExists (STARFIELD_MATERIAL_NAME)) {
		LOG ("\tMaterial not found; creating...");
		mat = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton ().create (STARFIELD_MATERIAL_NAME, "Caelum"));
		mat->setReceiveShadows (false);
		LOG ("\t\tMaterial [OK]");
		Ogre::Pass *pass = mat->getTechnique (0)->getPass (0);
		pass->setDepthCheckEnabled (false);
		pass->setDepthWriteEnabled (false);
		pass->setLightingEnabled (false);
		pass->setFog (true);
		LOG ("\t\tPass [OK]");
		Ogre::TextureUnitState *tus = pass->createTextureUnitState ();
		tus->setTextureAddressingMode (Ogre::TextureUnitState::TAM_WRAP, Ogre::TextureUnitState::TAM_WRAP, Ogre::TextureUnitState::TAM_WRAP);
		LOG ("\t\tTextureUnit [OK]");
		mat->load ();
		LOG ("\tDONE");
	}
	else {
		mat = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton ().getByName (STARFIELD_MATERIAL_NAME));
	}
	LOG ("DONE");

	mStarfieldMaterial = mat;
}

void CaelumSystem::updateSkyDomeMaterialTime () {
	Ogre::TextureUnitState *tus = mSkyDomeMaterial->getTechnique (0)->getPass (0)->getTextureUnitState (0);
	tus->setTextureUScroll (mLocalTime / mTotalDayTime);
	if (mOgreRoot->getRenderSystem ()->getCapabilities ()->hasCapability (Ogre::RSC_FRAGMENT_PROGRAM)) {
		mSkyDomeMaterial->getTechnique (0)->getPass (0)->getFragmentProgramParameters ()->setNamedConstant ("offset", mLocalTime / mTotalDayTime);
	}
}

} // namespace caelum
