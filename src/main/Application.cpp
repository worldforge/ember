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
#include "services/config/ConfigListenerContainer.h"
#include "services/metaserver/MetaserverService.h"
#include "services/sound/SoundService.h"
#include "services/scripting/ScriptingService.h"
#include "services/wfut/WfutService.h"
#include "services/time/TimeService.h"
#include "services/input/InputService.h"
#include "services/input/Input.h"

#include "framework/ConsoleBackend.h"
#include "framework/StreamLogObserver.h"
#include "framework/Time.h"

#include "components/lua/LuaScriptingProvider.h"

#include "components/ogre/EmberOgre.h"

#include "ConfigBoundLogObserver.h"

#include <tolua++.h>

TOLUA_API int tolua_Ogre_open(lua_State* tolua_S);
TOLUA_API int tolua_Eris_open(lua_State* tolua_S);
TOLUA_API int tolua_EmberServices_open(lua_State* tolua_S);
TOLUA_API int tolua_EmberOgre_open(lua_State* tolua_S);
TOLUA_API int tolua_Helpers_open(lua_State* tolua_S);
TOLUA_API int tolua_Framework_open(lua_State* tolua_S);
TOLUA_API int tolua_Application_open(lua_State* tolua_S);
TOLUA_API int tolua_AtlasAdapters_open(lua_State* tolua_S);
TOLUA_API int tolua_Atlas_open(lua_State* tolua_S);
TOLUA_API int tolua_Varconf_open(lua_State* tolua_S);

#include <iostream>
#include <sstream>
#ifndef WIN32
#include <stdio.h>
#endif

#include "framework/osdir.h"

namespace Ember
{

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief A simple listener class for the general:desiredfps config setting, which configures the capped fps.
 */
class DesiredFpsListener: public ConfigListenerContainer
{
protected:

	/**
	 * @brief The desired frames per second.
	 * If set to 0 no capping will occur.
	 */
	long mDesiredFps;

	/**
	 * @brief How long each frame should be in milliseconds.
	 * If set to 0 no capping will occur.
	 */
	long mMillisecondsPerFrame;

	void Config_DesiredFps(const std::string& section, const std::string& key, varconf::Variable& variable)
	{
		if (variable.is_int()) {
			mDesiredFps = static_cast<int> (variable);
			if (mDesiredFps != 0) {
				mMillisecondsPerFrame = 1000L / mDesiredFps;
			} else {
				mMillisecondsPerFrame = 0;
			}
		}
	}

public:

	/**
	 * @brief Ctor.
	 * A listener will be set up listening for the general:desiredfps config setting.
	 */
	DesiredFpsListener() :
		mDesiredFps(0), mMillisecondsPerFrame(0)
	{
		registerConfigListener("general", "desiredfps", sigc::mem_fun(*this, &DesiredFpsListener::Config_DesiredFps));
	}

	/**
	 * @brief Accessor for the desired framed per second.
	 * If 0 no capping should occur.
	 */
	long getDesiredFps() const
	{
		return mDesiredFps;
	}

	/**
	 * @brief Accessor for the minimum lenght (in milliseconds) that each frame should take in order for the desired fps to be kept.
	 * If 0 no capping will occur.
	 */
	long getMillisecondsPerFrame() const
	{
		return mMillisecondsPerFrame;
	}
};

template<> Ember::Application *Ember::Singleton<Ember::Application>::ms_Singleton = 0;

Application::Application(const std::string prefix, const std::string homeDir, const ConfigMap& configSettings) :
	mOgreView(0), mShouldQuit(false), mPrefix(prefix), mHomeDir(homeDir), mLogObserver(0), mServices(0), mWorldView(0), mPollEris(true), mLastTimeErisPollStart(0), mLastTimeErisPollEnd(0), mLastTimeInputProcessingStart(0), mLastTimeInputProcessingEnd(0), mConfigSettings(configSettings), mConsoleBackend(new ConsoleBackend()), Quit("quit", this, "Quit Ember."), ToggleErisPolling("toggle_erispolling", this, "Switch server polling on and off.")

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
	Log::removeObserver(mLogObserver);
	delete mLogObserver;
}

void Application::registerComponents()
{
	mOgreView = new EmberOgre::EmberOgre();
}

void Application::mainLoopStep(long minMillisecondsPerFrame)
{
	Ember::Input& input(Ember::Input::getSingleton());
	long currentTimeMillis(0);
	try {
		//If we should cap the fps so that each frame should take a minimum amount of time,
		//we need to see if we should sleep a little.
		if (minMillisecondsPerFrame > 0) {
			currentTimeMillis = Ember::Time::currentTimeMillis();
			long millisecondSinceLastFrame = currentTimeMillis - mLastTimeInputProcessingEnd;
			if (millisecondSinceLastFrame < minMillisecondsPerFrame) {
				input.sleep(minMillisecondsPerFrame - millisecondSinceLastFrame);
			}
		}
		if (mPollEris) {
			currentTimeMillis = Ember::Time::currentTimeMillis();
			EventStartErisPoll.emit((currentTimeMillis - mLastTimeErisPollStart) / 1000.0f);
			mLastTimeErisPollStart = currentTimeMillis;
			Eris::PollDefault::poll(1);
			if (mWorldView)
				mWorldView->update();
			currentTimeMillis = Ember::Time::currentTimeMillis();
			EventEndErisPoll.emit((currentTimeMillis - mLastTimeErisPollEnd) / 1000.0f);
			mLastTimeErisPollEnd = currentTimeMillis;
		}
		currentTimeMillis = Ember::Time::currentTimeMillis();
		EventBeforeInputProcessing.emit((currentTimeMillis - mLastTimeInputProcessingStart) / 1000.0f);
		mLastTimeInputProcessingStart = currentTimeMillis;
		input.processInput();

		currentTimeMillis = Ember::Time::currentTimeMillis();
		EventAfterInputProcessing.emit((currentTimeMillis - mLastTimeInputProcessingEnd) / 1000.0f);
		mLastTimeInputProcessingEnd = currentTimeMillis;
		mOgreView->renderOneFrame();
		EmberServices::getSingleton().getSoundService()->cycle();
	} catch (const std::exception& ex) {
		S_LOG_CRITICAL("Got exception, shutting down." << ex);
		throw ;
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
	long currentTimeMillis = Ember::Time::currentTimeMillis();
	mLastTimeErisPollStart = currentTimeMillis;
	mLastTimeErisPollEnd = currentTimeMillis;
	mLastTimeInputProcessingStart = currentTimeMillis;
	mLastTimeInputProcessingEnd = currentTimeMillis;
	DesiredFpsListener desiredFpsListener;
	while(mShouldQuit == false)
	{
		mainLoopStep(desiredFpsListener.getMillisecondsPerFrame());
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

	mLogObserver = new ConfigBoundLogObserver(*mLogOutStream);
	Ember::Log::addObserver(mLogObserver);

	///default to INFO, though this can be changed by the config file
	mLogObserver->setFilter(Ember::Log::INFO);

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

	///register the lua scripting provider. The provider will be owned by the scripting service, so we don't need to keep the pointer reference.
	Lua::LuaScriptingProvider* luaProvider = new Lua::LuaScriptingProvider();

	tolua_Framework_open(luaProvider->getLuaState());
	tolua_EmberOgre_open(luaProvider->getLuaState());
	tolua_Eris_open(luaProvider->getLuaState());
	tolua_EmberServices_open(luaProvider->getLuaState());
	tolua_Helpers_open (luaProvider->getLuaState());
	tolua_Ogre_open(luaProvider->getLuaState());
	tolua_Application_open(luaProvider->getLuaState());
	tolua_AtlasAdapters_open(luaProvider->getLuaState());
	tolua_Atlas_open(luaProvider->getLuaState());
	tolua_Varconf_open(luaProvider->getLuaState());
	Ember::EmberServices::getSingleton().getScriptingService()->registerScriptingProvider(luaProvider);

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
	try {
		if (!mOgreView->setup()) {
			///The setup was cancelled, return.
			return;
		}
	} catch (const std::exception& ex) {
		std::cout << "Error when setting up Ogre: " << ex.what() << std::endl;
		S_LOG_CRITICAL("Error when setting up Ogre." << ex);
		return;
	} catch (...) {
		std::cout << "Unknown error when setting up Ogre." << std::endl;
		S_LOG_CRITICAL("Unknown error when setting up Ogre.");
		return;
	}

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
	} else if (ToggleErisPolling == command) {
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
