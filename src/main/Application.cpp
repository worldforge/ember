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


#include <Eris/View.h>

#include "services/logging/LoggingService.h"
#include "services/server/ServerService.h"
#include "services/config/ConfigService.h"
#include "services/config/ConfigListenerContainer.h"
#include "services/metaserver/MetaserverService.h"
#include "services/sound/SoundService.h"
#include "services/scripting/ScriptingService.h"
#include "services/wfut/WfutService.h"
#include "services/input/InputService.h"
#include "services/input/Input.h"
#include "services/serversettings/ServerSettings.h"

#include "framework/ShutdownException.h"
#include "framework/TimeFrame.h"
#include "framework/FileResourceProvider.h"
#include "framework/StackChecker.h"

#include "components/lua/LuaScriptingProvider.h"
#include "components/lua/Connectors.h"

#include "components/ogre/EmberOgre.h"

#include "ConfigBoundLogObserver.h"

#include "components/lua/embertolua++.h"
#include "services/config/ConfigConsoleCommands.h"
#include <Atlas/Objects/Factories.h>

TOLUA_API int tolua_Ogre_open(lua_State* tolua_S);

TOLUA_API int tolua_Eris_open(lua_State* tolua_S);

TOLUA_API int tolua_EmberServices_open(lua_State* tolua_S);

TOLUA_API int tolua_EmberOgre_open(lua_State* tolua_S);

TOLUA_API int tolua_Helpers_open(lua_State* tolua_S);

TOLUA_API int tolua_Framework_open(lua_State* tolua_S);

TOLUA_API int tolua_AtlasAdapters_open(lua_State* tolua_S);

TOLUA_API int tolua_Adapters_open(lua_State* tolua_S);

TOLUA_API int tolua_Representations_open(lua_State* tolua_S);

TOLUA_API int tolua_Atlas_open(lua_State* tolua_S);

TOLUA_API int tolua_Varconf_open(lua_State* tolua_S);

TOLUA_API int tolua_Lua_open(lua_State* tolua_S);

TOLUA_API int tolua_ConnectorDefinitions_open(lua_State* tolua_S);

TOLUA_API int tolua_Domain_open(lua_State* tolua_S);

TOLUA_API int tolua_Cegui_open(lua_State* tolua_S);

#include <memory>
#include <boost/thread.hpp>

#ifdef _WIN32
#include "platform/platform_windows.h"
#endif

#ifndef HAVE_SIGHANDLER_T

typedef void (* sighandler_t)(int);

#endif

extern "C"
{

sighandler_t oldSignals[NSIG];
}

using boost::posix_time::microsec_clock;
using boost::posix_time::ptime;

namespace Ember {

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief A simple listener class for the general:desiredfps config setting, which configures the capped fps.
 */
class DesiredFpsListener : public ConfigListenerContainer {
protected:

	/**
	 * @brief The desired frames per second.
	 * If set to 0 no capping will occur.
	 */
	long mDesiredFps;

	/**
	 * @brief How long each frame should be in microseconds.
	 * If set to 0 no capping will occur.
	 */
	std::chrono::steady_clock::duration mTimePerFrame;

	bool mEnableStackCheck;

	void Config_DesiredFps(const std::string& section, const std::string& key, varconf::Variable& variable) {
		//Check for double, but cast to int. That way we'll catch all numbers.
		if (variable.is_double()) {
			mDesiredFps = static_cast<int>(variable);
			if (mDesiredFps != 0) {
				mTimePerFrame = std::chrono::microseconds(1000000L / mDesiredFps);
			} else {
				mTimePerFrame = std::chrono::steady_clock::duration::zero();
			}

			if (mEnableStackCheck && mDesiredFps > 0) {
				//Pad the frame duration by 1.5 to only catch those frames that are noticeable off
				StackChecker::start(std::chrono::milliseconds((int64_t) ((1000L / mDesiredFps) * 1.5)));
			} else {
				StackChecker::stop();
			}
		}
	}

	void Config_FrameStackCheck(const std::string& section, const std::string& key, varconf::Variable& variable) {
		if (variable.is_bool()) {
			mEnableStackCheck = static_cast<bool>(variable);
			if (mEnableStackCheck && mDesiredFps > 0) {
				//Pad the frame duration by 1.5 to only catch those frames that are noticeable off
				StackChecker::start(std::chrono::milliseconds((int64_t) ((1000L / mDesiredFps) * 1.5)));
			} else {
				StackChecker::stop();
			}
		}
	}

public:

	/**
	 * @brief Ctor.
	 * A listener will be set up listening for the general:desiredfps config setting.
	 */
	DesiredFpsListener() :
			mDesiredFps(0),
			mTimePerFrame(0),
			mEnableStackCheck(false) {
		registerConfigListener("general", "desiredfps", sigc::mem_fun(*this, &DesiredFpsListener::Config_DesiredFps));
		registerConfigListener("general", "slowframecheck", sigc::mem_fun(*this, &DesiredFpsListener::Config_FrameStackCheck));

	}

	/**
	 * @brief Accessor for the desired framed per second.
	 * If 0 no capping should occur.
	 */
	long getDesiredFps() const {
		return mDesiredFps;
	}

	/**
	 * @brief Accessor for the minimum length (in microseconds) that each frame should take in order for the desired fps to be kept.
	 * If 0 no capping will occur.
	 */
	std::chrono::steady_clock::duration getTimePerFrame() const {
		return mTimePerFrame;
	}
};

Application::Application(std::string prefix, std::string homeDir, ConfigMap configSettings, ConfigService& configService) :
		mConfigService(configService),
		mAtlasFactories(new Atlas::Objects::Factories()),
		mSession(new Eris::Session()),
		mFileSystemObserver(new FileSystemObserver(mSession->getIoService())),
		mShouldQuit(false),
		mPollEris(true),
		mMainLoopController(mShouldQuit, mPollEris, *mSession),
		mPrefix(std::move(prefix)),
		mHomeDir(std::move(homeDir)),
		mWorldView(nullptr),
		mConfigSettings(std::move(configSettings)),
		mConsoleBackend(new ConsoleBackend()),
		mConfigConsoleCommands(new ConfigConsoleCommands(mConfigService)),
		Quit("quit", this, "Quit Ember."),
		ToggleErisPolling("toggle_erispolling", this, "Switch server polling on and off."),
		mScriptingResourceProvider(nullptr) {

	// Change working directory
	auto dirName = mConfigService.getHomeDirectory(BaseDirType_CONFIG);

	if (!boost::filesystem::is_directory(dirName)) {
		boost::filesystem::create_directories(dirName);
	}

	int result = chdir(mConfigService.getHomeDirectory(BaseDirType_CONFIG).generic_string().c_str());

	if (result) {
		S_LOG_WARNING("Could not change directory to '" << mConfigService.getHomeDirectory(BaseDirType_CONFIG).c_str() << "'.");
	}

	//load the config file. Note that this will load the shared config file, and then the user config file if available.
	mConfigService.loadSavedConfig("ember.conf", mConfigSettings);

	//Check if there's a user specific ember.conf file. If not, create an empty template one.
	auto userConfigFilePath = mConfigService.getHomeDirectory(BaseDirType_CONFIG) / "ember.conf";
	if (!boost::filesystem::exists(userConfigFilePath)) {
		//Create empty template file.
		std::ofstream outstream(userConfigFilePath.c_str());
		outstream << "#This is a user specific settings file. Settings here override those found in the application installed ember.conf file." << std::endl << std::flush;
		S_LOG_INFO("Created empty user specific settings file at '" << userConfigFilePath.string() << "'.");
	}

	S_LOG_INFO("Using media from " << mConfigService.getEmberMediaDirectory().string());
}

Application::~Application() {

	if (mOgreView) {
		mOgreView->saveConfig();
	}

	// before shutting down, we write out the user config to user's ember home directory
	ConfigService& configService = mServices->getConfigService();
	configService.saveConfig(configService.getHomeDirectory(BaseDirType_CONFIG) / "ember.conf", varconf::USER);

	mServices->getServerService().stop();
	mServices->getMetaserverService().stop();

	mSession->getEventService().processAllHandlers();
	mSession->getIoService().stop();
	mSession->getIoService().reset();

	mOgreView.reset();

	mServices.reset();
	mScriptingResourceProvider.reset();
	mFileSystemObserver.reset();
	mSession.reset();
	S_LOG_INFO("Ember shut down normally.");
	Log::removeObserver(mLogObserver.get());
}

/**
 * Detach the input system, else the mouse will be locked.
 */
extern "C" void shutdownHandler(int sig);
extern "C" void shutdownHandler(int sig) {
	std::cerr << "Crashed with signal " << sig << ", will try to detach the input system gracefully. Please report bugs at https://bugs.launchpad.net/ember" << std::endl << std::flush;
	if (Input::hasInstance()) {
		Input::getSingleton().shutdownInteraction();
	}
	if (oldSignals[sig] != SIG_DFL && oldSignals[sig] != SIG_IGN) {
		/* Call saved signal handler. */
		oldSignals[sig](sig);
	} else {
		/* Reraise the signal. */
		signal(sig, SIG_DFL);
		raise(sig);
	}
}

void Application::registerComponents() {
	mOgreView = std::make_unique<OgreView::EmberOgre>();
}

void Application::mainLoop() {
	DesiredFpsListener desiredFpsListener;
	Eris::EventService& eventService = mSession->getEventService();
	Input& input(Input::getSingleton());

	do {
		try {
			Log::sCurrentFrameStartMilliseconds = microsec_clock::local_time();

			StackChecker::resetCounter();

			unsigned int frameActionMask = 0;
			auto timePerFrame = desiredFpsListener.getTimePerFrame();
			TimeFrame timeFrame(timePerFrame);

			auto end = std::chrono::steady_clock::now() + std::chrono::milliseconds(2);
			//Execute IO handlers for two milliseconds, if there are any.
			while (std::chrono::steady_clock::now() < end) {
				auto executedHandlers = mSession->getIoService().poll_one();
				if (executedHandlers == 0) {
					break;
				}
			}

			//Then process Eris handlers. These are things that mainly deal with assets being loaded, so it's ok if they are spread out over multiple frames.
			eventService.processOneHandler();

			if (mWorldView) {
				mWorldView->update();
			}


			bool updatedRendering = mOgreView->renderOneFrame(timeFrame);
			if (updatedRendering) {
				frameActionMask |= MainLoopController::FA_GRAPHICS;
				frameActionMask |= MainLoopController::FA_INPUT;
			} else {
				input.processInput();
				frameActionMask |= MainLoopController::FA_INPUT;
			}

			mServices->getSoundService().cycle();
			frameActionMask |= MainLoopController::FA_SOUND;

			//If there's time left this frame, poll any outstanding io handlers.
			if (timeFrame.isTimeLeft()) {
				size_t handersRun = 0;
				do {
					handersRun = mSession->getIoService().poll_one();
				} while (handersRun != 0 && timeFrame.isTimeLeft());
			}

			//If there's still time left this frame, process any outstanding main thread handlers.
			if (timeFrame.isTimeLeft()) {
				size_t handlersRun = 0;
				do {
					handlersRun = eventService.processOneHandler();
				} while (handlersRun != 0 && timeFrame.isTimeLeft());
			}

			//And if there's yet still time left this frame, wait until time is up, and do io in the meantime.
			if (timeFrame.isTimeLeft()) {
				boost::asio::steady_timer deadlineTimer(mSession->getIoService());
				deadlineTimer.expires_at(std::chrono::steady_clock::now() + timeFrame.getRemainingTime());

				deadlineTimer.async_wait([&](boost::system::error_code ec) {
				});

				while (timeFrame.isTimeLeft()) {
					mSession->getIoService().run_one();
				}
			}

			mMainLoopController.EventFrameProcessed(timeFrame, frameActionMask);

			if (updatedRendering && timeFrame.getElapsedTime().count() > (timePerFrame.count() * 1.4f)) {
				S_LOG_VERBOSE("Frame took too long.");
			}

			StackChecker::printBacktraces();

		} catch (const boost::exception& ex) {
			S_LOG_CRITICAL("Got exception, shutting down." << boost::diagnostic_information(ex));
			throw;
		} catch (const std::exception& ex) {
			S_LOG_CRITICAL("Got exception, shutting down." << ex);
			throw;
		} catch (...) {
			S_LOG_CRITICAL("Got unknown exception, shutting down.");
			throw;
		}

	} while (!mShouldQuit);
}

void Application::initializeServices() {
	// Initialize Ember services
	S_LOG_INFO("Initializing Ember services");

	mServices = std::make_unique<EmberServices>(*mSession, mConfigService);

	// Initialize the Sound Service
	S_LOG_INFO("Initializing sound service");
	mServices->getSoundService().start();

	// Initialize and start the Metaserver Service.
	S_LOG_INFO("Initializing metaserver service");

	mServices->getMetaserverService().start();
	//hoho, we get linking errors if we don't do some calls to the service
	mServices->getMetaserverService().getMetaServer();

	// Initialize the Server Service
	S_LOG_INFO("Initializing server service");
	mServices->getServerService().start();

	S_LOG_INFO("Initializing input service");
	mServices->getInputService().start();
	mServices->getInputService().getInput().setMainLoopController(&mMainLoopController);

	S_LOG_INFO("Initializing scripting service");
	mServices->getScriptingService().start();

	S_LOG_INFO("Initializing wfut service");
	mServices->getWfutService().start();

	S_LOG_INFO("Initializing server settings service");
	mServices->getServerSettingsService().start();

	mServices->getServerService().GotView.connect(sigc::mem_fun(*this, &Application::Server_GotView));
	mServices->getServerService().DestroyedView.connect(sigc::mem_fun(*this, &Application::Server_DestroyedView));

	mServices->getServerService().setupLocalServerObservation(mConfigService);

	//register the lua scripting provider. The provider will be owned by the scripting service, so we don't need to keep the pointer reference.
	auto luaProvider = std::make_unique<Lua::LuaScriptingProvider>();

	tolua_Lua_open(luaProvider->getLuaState());
	tolua_Framework_open(luaProvider->getLuaState());
	tolua_EmberOgre_open(luaProvider->getLuaState());
	tolua_Eris_open(luaProvider->getLuaState());
	tolua_EmberServices_open(luaProvider->getLuaState());
	tolua_Helpers_open(luaProvider->getLuaState());
	tolua_Ogre_open(luaProvider->getLuaState());
	tolua_AtlasAdapters_open(luaProvider->getLuaState());
	tolua_Adapters_open(luaProvider->getLuaState());
	tolua_Representations_open(luaProvider->getLuaState());
	tolua_Atlas_open(luaProvider->getLuaState());
	tolua_Varconf_open(luaProvider->getLuaState());
	tolua_ConnectorDefinitions_open(luaProvider->getLuaState());
	tolua_Domain_open(luaProvider->getLuaState());
	tolua_Cegui_open(luaProvider->getLuaState());

	Lua::ConnectorBase::setState(luaProvider->getLuaState());
	mServices->getScriptingService().registerScriptingProvider(std::move(luaProvider));

	mScriptingResourceProvider = std::make_unique<FileResourceProvider>(mServices->getConfigService().getSharedDataDirectory() / "scripting");
	mServices->getScriptingService().setResourceProvider(mScriptingResourceProvider.get());

	oldSignals[SIGSEGV] = signal(SIGSEGV, shutdownHandler);
	oldSignals[SIGABRT] = signal(SIGABRT, shutdownHandler);
	oldSignals[SIGILL] = signal(SIGILL, shutdownHandler);
#ifndef _WIN32
	oldSignals[SIGBUS] = signal(SIGBUS, shutdownHandler);
#endif

	EventServicesInitialized.emit();
}

void Application::Server_GotView(Eris::View* view) {
	mWorldView = view;
}

void Application::Server_DestroyedView() {
	mWorldView = nullptr;
}

Eris::View* Application::getMainView() {
	return mWorldView;
}

void Application::startScripting() {
	//this should be defined in some kind of text file, which should be different depending on what game you're playing (like deeds)
	try {
		//load the bootstrap script which will load all other scripts
		mServices->getScriptingService().loadScript("lua/Bootstrap.lua");
	} catch (const std::exception& e) {
		S_LOG_FAILURE("Error when loading bootstrap script." << e);
	}
	static const std::string luaSuffix = ".lua";
	std::list<std::string> luaFiles;

	//load any user defined scripts
	auto userScriptDirectoryPath = boost::filesystem::path(mServices->getConfigService().getHomeDirectory(BaseDirType_CONFIG)) / "scripts";

	if (boost::filesystem::is_directory(userScriptDirectoryPath)) {

		for (auto& dir_entry : boost::filesystem::recursive_directory_iterator(userScriptDirectoryPath)) {
			auto fileName = dir_entry.path().string();
			std::string lowerCaseFileName = fileName;
			std::transform(lowerCaseFileName.begin(), lowerCaseFileName.end(), lowerCaseFileName.begin(), ::tolower);

			if (lowerCaseFileName.compare(lowerCaseFileName.length() - luaSuffix.length(), luaSuffix.length(), luaSuffix) == 0) {
				luaFiles.push_back(fileName);
			}

		}


		//Sorting, because we want to load the scripts in a deterministic order.
		luaFiles.sort();
		for (auto& fileName : luaFiles) {
			std::ifstream stream((userScriptDirectoryPath / fileName).string(), std::ios::in);
			if (stream) {
				std::stringstream ss;
				ss << stream.rdbuf();
				stream.close();
				//It's important that we inform the user that we're loading a script (in case it provides any confusing behaviour).
				ConsoleBackend::getSingleton().pushMessage("Loading user Lua script from '" + fileName + "'.", "info");
				mServices->getScriptingService().executeCode(ss.str(), "LuaScriptingProvider");
			}
		}
	} else {
		try {
			//Create the script user script directory
			boost::filesystem::create_directories(userScriptDirectoryPath);
			std::ofstream readme((userScriptDirectoryPath / "/README").string(), std::ios::out);
			readme << "Any script files placed here will be executed as long as they have a supported file suffix.\nScripts are executed in alphabetical order.\nEmber currently supports lua scripts (ending with '.lua').";
			readme.close();
			S_LOG_INFO("Created user user scripting directory (" + userScriptDirectoryPath.string() + ").");
		} catch (const std::exception&) {
			S_LOG_INFO("Could not create user scripting directory.");
		}
	}
}

void Application::start() {

	try {
		if (!mOgreView->setup(Input::getSingleton(), mMainLoopController, mSession->getEventService())) {
			//The setup was cancelled, return.
			return;
		}
	} catch (const std::exception& ex) {
		std::cerr << "==== Error during startup ====\n\r\t" << ex.what() << "\n" << std::endl;
		S_LOG_CRITICAL("Error during startup." << ex);
		return;
	} catch (ShutdownException& ex2) {
		//Note that a ShutdownException is not an error. It just means that the user closed the application during startup. We should therefore just exit, as intended.
		S_LOG_INFO("ShutdownException caught: " << ex2.getReason());
		return;
	} catch (...) {
		std::cerr << "==== Error during startup ====\n\r\tUnknown fatal error during startup. Something went wrong which caused a shutdown. Check the log file for more information." << std::endl;
		S_LOG_CRITICAL("Unknown fatal error during startup.");
		return;
	}
	Input::getSingleton().startInteraction();

	startScripting();

	mainLoop();

}

void Application::runCommand(const std::string& command, const std::string& args) {
	if (command == Quit.getCommand()) {
		mShouldQuit = true;
	} else if (ToggleErisPolling == command) {
		mPollEris = !mPollEris;
	}
}

}
