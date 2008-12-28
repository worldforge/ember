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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Application.h"

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
#include "services/time/TimeService.h"
#include "services/input/InputService.h"
#include "services/input/Input.h"

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


Application::Application(const std::string prefix, const std::string homeDir, const ConfigMap& configSettings)
: mLoggingService(new Ember::LoggingService())
, mOgreView(0)
, mShouldQuit(false)
, mPrefix(prefix)
, mHomeDir(homeDir)
, mLogObserver(0)
, mServices(0)
, mWorldView(0)
, mPollEris(true)
, mConfigSettings(configSettings)
, mConsoleBackend(new ConsoleBackend())
, Quit("quit", this, "Quit Ember.")
, ToggleErisPolling("toggle_erispolling", this, "Switch server polling on and off.")

{

}


Application::~Application()
{
	EmberServices::getSingleton().getServerService()->stop(0);
	///this will not destroy the scripting environments, since there are other components, such as windows in the gui, that depend on the scripting environment being available at destruction time
	EmberServices::getSingleton().getScriptingService()->stop(0);
// 	mOgreView->shutdownGui();
	delete mOgreView;
	delete mServices;
	S_LOG_INFO("Ember shut down normally.");
	LoggingService::getSingleton().removeObserver(mLogObserver);
	delete mLogObserver;
	mLoggingService->stop(0);
}

void Application::registerComponents()
{
	mOgreView = new EmberOgre::EmberOgre();
}


void Application::mainLoopStep() 
{
	try {
		if (mPollEris) {
			EventStartErisPoll.emit();
			Eris::PollDefault::poll(1);
			if (mWorldView)
				mWorldView->update();
			EventEndErisPoll.emit();
		}
		EventBeforeInputProcessing.emit();
		Ember::Input& input(Ember::Input::getSingleton());
		input.processInput();
		EventAfterInputProcessing.emit();
		mOgreView->renderOneFrame();
		#ifndef THREAD_SAFE
		EmberServices::getSingleton().getSoundService()->cycle();
		#endif
	} catch (const std::exception& ex)
	{
		S_LOG_CRITICAL("Got exception, shutting down. " << ex.what());
		throw;
	} catch (const std::string& ex)
	{
		S_LOG_CRITICAL("Got exception, shutting down. " << ex);
		throw;
	} catch (...)
	{
		S_LOG_CRITICAL("Got unknown exception.");
		throw;
	}
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
	/// Initialize the Configuration Service
	EmberServices::getSingleton().getConfigService()->start();
	if (mPrefix != "") {
		EmberServices::getSingleton().getConfigService()->setPrefix(mPrefix);
	}
	if (mHomeDir != "") {
		EmberServices::getSingleton().getConfigService()->setHomeDirectory(mHomeDir);
		std::cout << "Setting home directory to " << mHomeDir << std::endl;
	}
	
	///output all logging to ember.log
	std::string filename(EmberServices::getSingleton().getConfigService()->getHomeDirectory() + "/ember.log");
	mLogOutStream = std::auto_ptr<std::ofstream>(new std::ofstream(filename.c_str()));
	
	///write to the log the version number
	*mLogOutStream << "Ember version " << VERSION << std::endl;
	
	mLogObserver = new LogObserver(*mLogOutStream);
	Ember::LoggingService *logging = EmberServices::getSingleton().getLoggingService();
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
	
	
	int result = chdir(EmberServices::getSingleton().getConfigService()->getHomeDirectory().c_str());
	if (result) {
		S_LOG_WARNING("Could not change directory to '"<< EmberServices::getSingleton().getConfigService()->getHomeDirectory().c_str() <<"'.");
	}

// 	const std::string& sharePath(EmberServices::getSingleton().getConfigService()->getSharedConfigDirectory());

	///make sure that there are files 
	///assureConfigFile("ember.conf", sharePath);

	///load the config file. Note that this will load the shared config file, and then the user config file if available (~/.ember/ember.conf)
	EmberServices::getSingleton().getConfigService()->loadSavedConfig("ember.conf");
	///after loading the config from file, override with command time settings
	for (ConfigMap::iterator I = mConfigSettings.begin(); I != mConfigSettings.end(); ++I) {
		for (std::map<std::string, std::string>::iterator J = I->second.begin(); J != I->second.end(); ++J) {
			EmberServices::getSingleton().getConfigService()->setValue(I->first, J->first, J->second);
		}
	}

	S_LOG_INFO("Using media from " << EmberServices::getSingleton().getConfigService()->getEmberMediaDirectory());

	/// Initialize the Sound Service
	S_LOG_INFO("Initializing sound service");
	EmberServices::getSingleton().getSoundService()->start();

	/// Initialize and start the Metaserver Service.
	S_LOG_INFO("Initializing metaserver service");

 	EmberServices::getSingleton().getMetaserverService()->start();
	///hoho, we get linking errors if we don't do some calls to the service
	EmberServices::getSingleton().getMetaserverService()->getMetaServer();
	
	/// Initialize the Server Service
	S_LOG_INFO("Initializing server service");
	EmberServices::getSingleton().getServerService()->start();

	/// The time service depends on the server service, so we have to intialize it in this order
	S_LOG_INFO("Initializing time service");
	EmberServices::getSingleton().getTimeService()->start();

	S_LOG_INFO("Initializing input service");
	EmberServices::getSingleton().getInputService()->start();


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

void* startSoundServiceCycle(void* arg)
{
	while (1)
	{
		EmberServices::getSingleton().getSoundService()->cycle();
	}
}

void Application::start()
{
	try {
		if (!mOgreView->setup()) {
			///The setup was cancelled, return.
			return;
		}
	} catch (const std::exception& ex) {
		std::cout << "Error when setting up Ogre: " << ex.what() << std::endl;
		S_LOG_CRITICAL("Error when setting up Ogre: " << ex.what());
		return;
	} catch (...) {
		std::cout << "Unknown error when setting up Ogre." << std::endl;
		S_LOG_CRITICAL("Unknown error when setting up Ogre.");
		return;
	}

	// Start the SoundService thread
	#ifdef THREAD_SAFE
	pthread_t soundThread;
	pthread_create(&soundThread, NULL, startSoundServiceCycle, NULL);
	#endif

	mainLoop();
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

void Application::runCommand(const std::string& command, const std::string& args)
{
	if(command == Quit.getCommand()) {
		quit();
	} else if (ToggleErisPolling == command){
		setErisPolling(!getErisPolling());
	}
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
