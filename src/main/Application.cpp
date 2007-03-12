/*
 *  File:       Application.cpp
 *  Summary:    The class which initializes the GUI.
 *  Written by: nikal
 *
 *  Copyright (C) 2001, 2002 nikal. 
 *  This code is distributed under the GPL.
 *  See file COPYING for details. 
 *

 */

#include "Application.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
#include "services/server/ServerService.h"
#include "services/config/ConfigService.h"
#include "services/metaserver/MetaserverService.h"
#include "services/sound/SoundService.h"
#include "services/scripting/ScriptingService.h"
#include "services/wfut/WfutService.h"

#include "framework/ConsoleBackend.h"
#include "framework/StreamLogObserver.h"

#include "components/ogre/EmberOgre.h"

#include "LogObserver.h"

#include <iostream>
#include <sstream>
#ifndef WIN32
#include <stdio.h>
#endif

#include "framework/osdir.h"


namespace Ember
{

template<> Ember::Application *Ember::Singleton<Ember::Application>::ms_Singleton = 0;


Application::Application(const std::string& prefix, const std::string& homeDir, bool useBinReloc)
: mShouldQuit(false), mPrefix(prefix), mHomeDir(homeDir), mUseBinreloc(useBinReloc), mOgreView(0), mLogObserver(0), mServices(0)
{

}


Application::~Application()
{
	delete mOgreView;
	delete mLogObserver;
	delete mServices;
}

void Application::registerComponents()
{
	mOgreView = new EmberOgre::EmberOgre();
}


void Application::mainLoopStep() 
{
}

void Application::mainLoop() 
{
	while(mShouldQuit == false) 
	{
		mainLoopStep();
	}
}

void Application::prepareComponents()
{
}
void Application::initializeServices()
{
	/// Initialize Ember services
	std::cout << "Initializing Ember services" << std::endl;

	mServices = new Ember::EmberServices();
	Ember::LoggingService *logging = Ember::EmberServices::getSingleton().getLoggingService();
	
	/// Initialize the Configuration Service
	Ember::EmberServices::getSingleton().getConfigService()->start();
	Ember::EmberServices::getSingleton().getConfigService()->setPrefix(mPrefix);
	if (mHomeDir != "") {
		Ember::EmberServices::getSingleton().getConfigService()->setHomeDirectory(mHomeDir);
		std::cout << "Setting home directory to " << mHomeDir << std::endl;
	}
	
	///output all logging to ember.log
	std::string filename(Ember::EmberServices::getSingleton().getConfigService()->getHomeDirectory() + "/ember.log");
	static std::ofstream outstream(filename.c_str());
	
	///write to the log the version number
	outstream << "Ember version " << VERSION << ", using media version " << MEDIA_VERSION << std::endl;
	
	mLogObserver = new LogObserver(outstream);
	logging->addObserver(mLogObserver);
	
	///default to INFO, though this can be changed by the config file
 	mLogObserver->setFilter(Ember::LoggingService::INFO);
 	
 	


	/// Change working directory
	const std::string& dirName = Ember::EmberServices::getSingleton().getConfigService()->getHomeDirectory();
	oslink::directory osdir(dirName);

	if (!osdir) {
#ifdef WIN32
		mkdir(dirName.c_str());
#else
		mkdir(dirName.c_str(), S_IRWXU);
#endif
	}
	
	
	chdir(Ember::EmberServices::getSingleton().getConfigService()->getHomeDirectory().c_str());

	const std::string& sharePath(Ember::EmberServices::getSingleton().getConfigService()->getSharedConfigDirectory());

	///make sure that there are files 
	///assureConfigFile("ember.conf", sharePath);

	Ember::EmberServices::getSingleton().getConfigService()->loadSavedConfig("ember.conf");

// #ifndef WIN32
// 	/// Initialize the SoundService
// 	if (Ember::EmberServices::getSingleton().getSoundService()->start() == Ember::Service::OK) {
// 		Ember::EmberServices::getSingleton().getSoundService()->registerSoundProvider(new OgreSoundProvider());
// 	}
// #endif

	/// Initialize and start the Metaserver Service.
	S_LOG_INFO("Initializing MetaServer Service");

 	Ember::EmberServices::getSingleton().getMetaserverService()->start();
	///hoho, we get linking errors if we don't do some calls to the service
	Ember::EmberServices::getSingleton().getMetaserverService()->getMetaServer();
	
	/// Initialize the Server Service
	S_LOG_INFO("Initializing Server Service");

/*	Ember::EmberServices::getSingleton().getServerService()->GotConnection.connect(sigc::mem_fun(*this, &Application::connectedToServer));
	Ember::EmberServices::getSingleton().getServerService()->GotView.connect(sigc::mem_fun(*this, &Application::Server_GotView));*/
	
	Ember::EmberServices::getSingleton().getServerService()->start();

 	Ember::EmberServices::getSingleton().getScriptingService()->start();

 	Ember::EmberServices::getSingleton().getWfutService()->start();
 	
 	EventServicesInitialized.emit();
}

void Application::start()
{
	mOgreView->go(mUseBinreloc);
}

bool Application::shouldQuit() 
{
	return mShouldQuit;
}

void Application::quit()
{
	mShouldQuit = true;
}

void Application::runCommand(const std::string& cmd, const std::string& args)
{
}




}
