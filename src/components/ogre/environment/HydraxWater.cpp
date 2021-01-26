//
// C++ Implementation: HydraxWater
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2008
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

#include "HydraxWater.h"

#include "hydrax/src/Hydrax.h"
#include "hydrax/src/Noise/Perlin/Perlin.h"
#include "hydrax/src/Modules/ProjectedGrid/ProjectedGrid.h"

#include "services/config/ConfigService.h"

namespace Ember {
namespace OgreView {

namespace Environment {

HydraxWater::HydraxWater(Ogre::Camera& camera, Ogre::SceneManager& sceneMgr)
: mCamera(camera), mSceneMgr(sceneMgr), mHydrax(0)
{
}


HydraxWater::~HydraxWater()
{
	Ogre::Root::getSingleton().removeFrameListener(this);

	delete mHydrax;
}

bool HydraxWater::isSupported() const
{
	//TODO: check that it's supported
	return true;
}

bool HydraxWater::initialize()
{
	//TODO: reactivate if we get hydrax to perform better
//	try {
//	// Init Hydrax
//
//		// Create Hydrax object
//		mHydrax = new Hydrax::Hydrax(&mSceneMgr, &mCamera, mCamera.getViewport());
//
//		// Create our projected grid module
//		Hydrax::Module::ProjectedGrid *mModule = new Hydrax::Module::ProjectedGrid(// Hydrax parent pointer
//												mHydrax,
//												// Noise module
//												new Hydrax::Noise::Perlin(/*Generic one*/),
//												// Base plane
//												Ogre::Plane(Ogre::Vector3(0,1,0), Ogre::Vector3(0,0,0)),
//												// Normal mode
//												Hydrax::MaterialManager::NM_VERTEX,
//												// Projected grid options
//												Hydrax::Module::ProjectedGrid::Options(16));
//
//		// Set our module
//		mHydrax->setModule(static_cast<Hydrax::Module::Module*>(mModule));
//
//		// Load all parameters from config file
//		// Remarks: The config file must be in Hydrax resource group.
//		// All parameters can be set/updated directly by code(Like previous versions),
//		// but due to the high number of customizable parameters, Hydrax 0.4 allows save/load config files.
//
//		std::string configFilename(ConfigService::getSingleton().getSharedConfigDirectory() + "hydrax.cfg");
//
//		struct stat theStat;
//		int ret = stat(configFilename.c_str(), &theStat);
//		if (ret) {
//			S_LOG_FAILURE("Could not find file "<< configFilename);
//			return false;
//		}
//		std::ifstream *filestream = OGRE_NEW_T(std::ifstream, Ogre::MEMCATEGORY_GENERAL)();
//		filestream->open(configFilename.c_str(), std::ios::in);
//
//		if (filestream->fail())
//		{
//			S_LOG_FAILURE("Could not open file "<< configFilename);
//			delete filestream;
//			return false;
//		}
//
//		//this will envelope the file stream pointer and delete it when it's destroyed itself
//		Ogre::FileStreamDataStream* stream = OGRE_NEW Ogre::FileStreamDataStream(configFilename, filestream, theStat.st_size, true);
//		Ogre::DataStreamPtr dataPtr(stream);
//
//		if (stream->size() == 0) {
//			S_LOG_FAILURE("Zero size file found at "<< configFilename);
//			return false;
//		}
//
//		Ogre::ConfigFile configFile;
//
//		configFile.load(dataPtr);
//
//		mHydrax->loadCfg(configFile);
//
//		// Create water
//		mHydrax->create();
//
//		Ogre::Root::getSingleton().addFrameListener(this);
//
//		return true;
//	} catch (const std::exception& ex)
//	{
//		S_LOG_FAILURE("Error when creating hydrax water: " << ex.what());
//		return false;
//	}
	return true;
}

bool HydraxWater::frameStarted(const Ogre::FrameEvent& event)
{
//	mHydrax->update(event.timeSinceLastFrame);
	return true;
}

}

}
}
