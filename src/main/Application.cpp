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

#ifdef WIN32
	#include <tchar.h>
	#define snprintf _snprintf
    #include <io.h> // for _access, Win32 version of stat()
    #include <direct.h> // for _mkdir
//	#include <sys/stat.h>
	
	#include <iostream>
	#include <fstream>
	#include <ostream>
#else
	#include <dirent.h>
#endif

#include <Eris/View.h>
#include <Eris/PollDefault.h>

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


Application::Application(const std::string prefix, const std::string homeDir, bool useBinReloc)
: mShouldQuit(false)
, mPrefix(prefix)
, mHomeDir(homeDir)
, mUseBinreloc(useBinReloc)
, mOgreView(0)
, mLogObserver(0)
, mServices(0)
, mWorldView(0)
, mPollEris(true)
{

}


Application::~Application()
{
	delete mWorldView;
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
	if (mPollEris) {
		EventStartErisPoll.emit();
		try {
			Eris::PollDefault::poll(1);
		} catch (const Ember::Exception& ex) {
			S_LOG_CRITICAL(ex.getError());
			throw ex;
		} catch (const std::exception& ex)
		{
			S_LOG_CRITICAL("Got exception, shutting down. " << ex.what());
			throw ex;
		} catch (const std::string& ex)
		{
			S_LOG_CRITICAL("Got exception, shutting down. " << ex);
			throw ex;
		} catch (...)
		{
			S_LOG_CRITICAL("Got unknown exception.");
		}
		if (mWorldView)
			mWorldView->update();
		EventEndErisPoll.emit();
	}
	mOgreView->renderOneFrame();
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

	mServices = new EmberServices();
	Ember::LoggingService *logging = EmberServices::getSingleton().getLoggingService();
	
	/// Initialize the Configuration Service
	EmberServices::getSingleton().getConfigService()->start();
	EmberServices::getSingleton().getConfigService()->setPrefix(mPrefix);
	if (mHomeDir != "") {
		EmberServices::getSingleton().getConfigService()->setHomeDirectory(mHomeDir);
		std::cout << "Setting home directory to " << mHomeDir << std::endl;
	}
	
	///output all logging to ember.log
	std::string filename(EmberServices::getSingleton().getConfigService()->getHomeDirectory() + "/ember.log");
	mLogOutStream = std::auto_ptr<std::ofstream>(new std::ofstream(filename.c_str()));
	
	///write to the log the version number
	*mLogOutStream << "Ember version " << VERSION << ", using media version " << MEDIA_VERSION << std::endl;
	
	mLogObserver = new LogObserver(*mLogOutStream);
	logging->addObserver(mLogObserver);
	
	///default to INFO, though this can be changed by the config file
 	mLogObserver->setFilter(Ember::LoggingService::INFO);
 	
 	


	/// Change working directory
	const std::string& dirName = EmberServices::getSingleton().getConfigService()->getHomeDirectory();
	oslink::directory osdir(dirName);

	if (!osdir) {
#ifdef WIN32
		mkdir(dirName.c_str());
#else
		mkdir(dirName.c_str(), S_IRWXU);
#endif
	}
	
	
	chdir(EmberServices::getSingleton().getConfigService()->getHomeDirectory().c_str());

	const std::string& sharePath(EmberServices::getSingleton().getConfigService()->getSharedConfigDirectory());

	///make sure that there are files 
	///assureConfigFile("ember.conf", sharePath);

	EmberServices::getSingleton().getConfigService()->loadSavedConfig("ember.conf");

// #ifndef WIN32
// 	/// Initialize the SoundService
// 	if (EmberServices::getSingleton().getSoundService()->start() == Ember::Service::OK) {
// 		EmberServices::getSingleton().getSoundService()->registerSoundProvider(new OgreSoundProvider());
// 	}
// #endif

	/// Initialize and start the Metaserver Service.
	S_LOG_INFO("Initializing metaserver service");

 	EmberServices::getSingleton().getMetaserverService()->start();
	///hoho, we get linking errors if we don't do some calls to the service
	EmberServices::getSingleton().getMetaserverService()->getMetaServer();
	
	/// Initialize the Server Service
	S_LOG_INFO("Initializing server service");
	EmberServices::getSingleton().getServerService()->start();

	S_LOG_INFO("Initializing scripting service");
 	EmberServices::getSingleton().getScriptingService()->start();

	S_LOG_INFO("Initializing wfut service");
 	EmberServices::getSingleton().getWfutService()->start();
 	
 	
	EmberServices::getSingleton().getServerService()->GotView.connect(sigc::mem_fun(*this, &Application::Server_GotView));
 	
 	EventServicesInitialized.emit();
}

void Application::Server_GotView(Eris::View* view)
{
	mWorldView = view;
}

Eris::View* Application::getMainView()
{
	return mWorldView;
}

void Application::start()
{
	mOgreView->setup(mUseBinreloc);
	mainLoop();
	//mOgreView->go(mUseBinreloc);
}

bool Application::shouldQuit() 
{
	return mShouldQuit;
}

void Application::requestQuit()
{
	bool handled = false;
	EventRequestQuit.emit(handled);
	///check it was handled (for example if the gui wants to show a confirmation window)
	if (!handled) {
		///it's not handled, quit now
		quit();
	}

}
void Application::quit()
{
	mShouldQuit = true;
}

void Application::runCommand(const std::string& cmd, const std::string& args)
{
}

void Application::setErisPolling(bool doPoll)
{
	mPollEris = doPoll;
}

bool Application::getErisPolling() const
{
	return mPollEris;
}




}
