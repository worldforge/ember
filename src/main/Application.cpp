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

#include "services/server/ServerService.h"
#include "services/config/ConfigService.h"
#include "services/config/ConfigListenerContainer.h"
#include "services/metaserver/MetaserverService.h"
#include "services/sound/SoundService.h"
#include "services/scripting/ScriptingService.h"
#include "services/input/Input.h"
#include "services/serversettings/ServerSettings.h"

#include "framework/ShutdownException.h"
#include "framework/TimeFrame.h"
#include "framework/FileResourceProvider.h"
#include "framework/StackChecker.h"

#include "components/lua/LuaScriptingProvider.h"

#include "components/ogre/EmberOgre.h"

#include "ConfigBoundLogObserver.h"

#include "services/config/ConfigConsoleCommands.h"
#include "ConsoleInputBinder.h"

#include <Eris/View.h>

#include <memory>
#include <boost/thread.hpp>
#include <boost/asio/steady_timer.hpp>
#include <squall/core/Repository.h>
#include <boost/url/src.hpp>

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

namespace Ember {

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief A simple listener class for the general:desiredfps config setting, which configures the capped fps.
 */
class DesiredFpsListener : public ConfigListenerContainer {
private:

	/**
	 * @brief The desired frames per second.
	 * If set to 0 no capping will occur.
	 */
	long mDesiredFps{0};

	/**
	 * @brief How long each frame should be in microseconds.
	 * If set to 0 no capping will occur.
	 */
	std::chrono::steady_clock::duration mTimePerFrame;

	bool mEnableStackCheck{false};

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
			mTimePerFrame(0) {
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

Application::Application(Input& input,
						 ConfigMap configSettings,
						 ConfigService& configService) :
		mInput(input),
		mConfigService(configService),
		mSession(std::make_unique<Session>()),
		mAssetsUpdater(Squall::Repository(std::filesystem::path{(configService.getHomeDirectory(BaseDirType_DATA) / "squall").string()})),
		mFileSystemObserver(std::make_unique<FileSystemObserver>(mSession->m_io_service)),
		mShouldQuit(false),
		mPollEris(true),
		mMainLoopController(mShouldQuit, mPollEris, *mSession),
		mWorldView(nullptr),
		mConfigSettings(std::move(configSettings)),
		mConsoleBackend(std::make_unique<ConsoleBackend>()),
		mConfigConsoleCommands(std::make_unique<ConfigConsoleCommands>(mConfigService)),
		mConsoleInputBinder(std::make_unique<ConsoleInputBinder>(mInput, *mConsoleBackend)),
		Quit("quit", this, "Quit Ember."),
		ToggleErisPolling("toggle_erispolling", this, "Switch server polling on and off.") {

	// Change working directory
	auto dirName = mConfigService.getHomeDirectory(BaseDirType_CONFIG);

	if (!boost::filesystem::is_directory(dirName)) {
		S_LOG_INFO("Creating home directory at " << dirName.string());
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

	initializeServices();

	auto squallRepoPath = std::filesystem::path(mConfigService.getHomeDirectory(BaseDirType_DATA).string()) / "squall";


	mOgreView = std::make_unique<OgreView::EmberOgre>(mMainLoopController,
													  mSession->m_event_service,
													  mInput,
													  mServices->getServerService(),
													  mServices->getSoundService(),
													  mAssetsUpdater.getRepository());

}

Application::~Application() {

	if (mOgreView) {
		mOgreView->saveConfig();
	}

	// before shutting down, we write out the user config to user's ember home directory
	mConfigService.saveConfig(mConfigService.getHomeDirectory(BaseDirType_CONFIG) / "ember.conf", varconf::USER);

	if (mServices) {
		mSession->m_event_service.processAllHandlers();
		mServices->getServerService().disconnect();
		mServices->getScriptingService().EventShutdown();
		//First process all handlers so we clean up stuff.
		mSession->m_event_service.processAllHandlers();
		mServices->getScriptingService().stop();
	}

	//Process all handlers again before shutting down.
	mSession->m_event_service.processAllHandlers();
	mSession->m_io_service.stop();
	mSession->m_io_service.reset();

	mOgreView.reset();

	mServices.reset();
	mScriptingResourceProvider.reset();
	mFileSystemObserver.reset();
	mSession.reset();

	Log::removeObserver(mLogObserver.get());
}

/**
 * Detach the input system, else the mouse will be locked.
 */
extern "C" void shutdownHandler(int sig);
extern "C" void shutdownHandler(int sig) {
	std::cerr << "Crashed with signal " << sig << ", will try to detach the input system gracefully. Please report bugs at https://bugs.launchpad.net/ember" << std::endl;
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

void Application::mainLoop() {
	DesiredFpsListener desiredFpsListener;
	Eris::EventService& eventService = mSession->m_event_service;

	while (!mShouldQuit) {
		try {
			StreamLogObserver::sCurrentFrameStartMilliseconds = std::chrono::steady_clock::now();

			StackChecker::resetCounter();

			unsigned int frameActionMask = 0;
			auto timePerFrame = desiredFpsListener.getTimePerFrame();
			TimeFrame timeFrame(timePerFrame);

			mInput.processInput();
			frameActionMask |= MainLoopController::FA_INPUT;

			//mShouldQuit is sometimes set by IO, so we might exit here already
			if (mShouldQuit) {
				return;
			}

			bool updatedRendering = mOgreView->renderOneFrame(timeFrame);
			if (updatedRendering) {
				frameActionMask |= MainLoopController::FA_GRAPHICS;
			}

			mServices->getSoundService().cycle();
			frameActionMask |= MainLoopController::FA_SOUND;

			//Execute IO handlers for two milliseconds, if there are any.
			auto end = std::chrono::steady_clock::now() + std::chrono::milliseconds(2);
			while (std::chrono::steady_clock::now() < end) {
				auto executedHandlers = mSession->m_io_service.poll_one();
				if (executedHandlers == 0) {
					break;
				}
			}

			//Then process Eris handlers. These are things that mainly deal with assets being loaded, so it's ok if they are spread out over multiple frames.
			eventService.processOneHandler();

			if (mWorldView) {
				mWorldView->update();
			}

			//If there's time left this frame, poll any outstanding io handlers.
			if (timeFrame.isTimeLeft()) {
				auto handlersRun = mSession->m_io_service.poll_one();
				while (handlersRun != 0 && timeFrame.isTimeLeft()) {
					handlersRun = mSession->m_io_service.poll_one();
				}
			}

			//If there's still time left this frame, process any outstanding main thread handlers.
			if (timeFrame.isTimeLeft()) {
				auto handlersRun = eventService.processOneHandler();
				while (handlersRun != 0 && timeFrame.isTimeLeft()) {
					handlersRun = eventService.processOneHandler();
				}
			}

			//And if there's yet still time left this frame, wait until time is up, and do io in the meantime.
			if (timeFrame.isTimeLeft()) {
				boost::asio::steady_timer deadlineTimer(mSession->m_io_service);
				deadlineTimer.expires_at(std::chrono::steady_clock::now() + timeFrame.getRemainingTime());

				deadlineTimer.async_wait([&](boost::system::error_code ec) {
				});

				while (timeFrame.isTimeLeft()) {
					mSession->m_io_service.run_one();
				}
			}

			mMainLoopController.EventFrameProcessed(timeFrame, frameActionMask);

			//Check if it took more than 140% of the allotted time (but avoid floats when doing the comparisons).
			if (updatedRendering && (timeFrame.getElapsedTime().count() * 1000000) > (timePerFrame.count() * 1400000)) {
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
	}
}

void Application::initializeServices() {
	// Initialize Ember services
	S_LOG_INFO("Initializing Ember services");

	mServices = std::make_unique<EmberServices>(*mSession, mConfigService);

	mInput.setMainLoopController(&mMainLoopController);


	mServices->getServerService().GotView.connect(sigc::mem_fun(*this, &Application::Server_GotView));
	mServices->getServerService().DestroyedView.connect(sigc::mem_fun(*this, &Application::Server_DestroyedView));
	mServices->getServerService().AssetsSyncRequest.connect([this](AssetsSync assetsSync) {
		auto parseResult = boost::urls::parse_uri(assetsSync.assetsPath);
		if (parseResult.has_value()) {
			auto url = parseResult.value();
			if (url.scheme() == "squall") {
				std::string portSection = url.has_port() ? std::string(":") + std::string(url.port()) : "";

				std::string baseUrl = "http://" + url.host() + portSection + url.path();
				Squall::Signature signature(url.fragment());
				auto future = mAssetsUpdater.syncSquall(baseUrl, signature);

				mAssetUpdates.emplace_back(AssetsUpdateBridge{.stage= AssetsUpdateBridge::SyncStage{.pollFuture = std::move(future)},
						.squallSignature = signature.str(),
						.CompleteSignal = assetsSync.Complete});
				if (mAssetUpdates.size() == 1) {
					scheduleAssetsPoll();
				}
			} else {
				assetsSync.Complete(AssetsSync::UpdateResult::Failure);
			}
		} else {
			assetsSync.Complete(AssetsSync::UpdateResult::Success);
		}
	});


	mServices->getServerService().setupLocalServerObservation(mConfigService);

	//register the lua scripting provider. The provider will be owned by the scripting service, so we don't need to keep the pointer reference.
	auto luaProvider = std::make_unique<Lua::LuaScriptingProvider>();


	mServices->getScriptingService().registerScriptingProvider(std::move(luaProvider));

	mScriptingResourceProvider = std::make_unique<FileResourceProvider>(mServices->getConfigService().getSharedDataDirectory() / "scripting");
	mServices->getScriptingService().setResourceProvider(mScriptingResourceProvider.get());

	oldSignals[SIGSEGV] = signal(SIGSEGV, shutdownHandler);
	oldSignals[SIGABRT] = signal(SIGABRT, shutdownHandler);
	oldSignals[SIGILL] = signal(SIGILL, shutdownHandler);
#ifndef _WIN32
	oldSignals[SIGBUS] = signal(SIGBUS, shutdownHandler);
#endif

}

void Application::Server_GotView(Eris::View* view) {
	mWorldView = view;
}

void Application::Server_DestroyedView() {
	mWorldView = nullptr;
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

		for (auto& dir_entry: boost::filesystem::recursive_directory_iterator(userScriptDirectoryPath)) {
			auto fileName = dir_entry.path().string();
			std::string lowerCaseFileName = fileName;
			std::transform(lowerCaseFileName.begin(), lowerCaseFileName.end(), lowerCaseFileName.begin(), ::tolower);

			if (lowerCaseFileName.compare(lowerCaseFileName.length() - luaSuffix.length(), luaSuffix.length(), luaSuffix) == 0) {
				luaFiles.push_back(fileName);
			}

		}


		//Sorting, because we want to load the scripts in a deterministic order.
		luaFiles.sort();
		for (auto& fileName: luaFiles) {
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
			readme
					<< "Any script files placed here will be executed as long as they have a supported file suffix.\nScripts are executed in alphabetical order.\nEmber currently supports lua scripts (ending with '.lua').";
			readme.close();
			S_LOG_INFO("Created user user scripting directory (" + userScriptDirectoryPath.string() + ").");
		} catch (const std::exception&) {
			S_LOG_INFO("Could not create user scripting directory.");
		}
	}
}

void Application::start() {

	try {
		if (!mOgreView->setup(mMainLoopController, mSession->m_event_service)) {
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
	mInput.startInteraction();

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

void Application::scheduleAssetsPoll() {
	//Perhaps a better way to cancel asset pollings?
	if (mShouldQuit) {
		for (auto I = mAssetUpdates.begin(); I != mAssetUpdates.end();) {
			I->CompleteSignal(AssetsSync::UpdateResult::Cancelled);
		}
		mAssetUpdates.clear();
	}
	if (!mAssetUpdates.empty()) {
		mSession->m_event_service.runOnMainThread([this]() {
			mAssetsUpdater.poll();
			for (auto I = mAssetUpdates.begin(); I != mAssetUpdates.end();) {
				auto& assetsUpdate = *I;
				if (std::holds_alternative<AssetsUpdateBridge::SyncStage>(assetsUpdate.stage)) {
					auto& syncStage = std::get<AssetsUpdateBridge::SyncStage>(assetsUpdate.stage);
					if (syncStage.pollFuture.valid() && syncStage.pollFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
						auto pollResult = syncStage.pollFuture.get();
						if (pollResult == UpdateResult::Success) {
							auto loadFuture = mOgreView->loadAssets(assetsUpdate.squallSignature);
							assetsUpdate.stage = AssetsUpdateBridge::LoadingStage{.pollFuture = std::move(loadFuture)};
							I++;
						} else {
							assetsUpdate.CompleteSignal(pollResult == UpdateResult::Failure ? AssetsSync::UpdateResult::Failure : AssetsSync::UpdateResult::Cancelled);
							I = mAssetUpdates.erase(I);
						}
					} else {
						I++;
					}
				} else {
					auto& loadStage = std::get<AssetsUpdateBridge::LoadingStage>(assetsUpdate.stage);

					if (loadStage.pollFuture.valid() && loadStage.pollFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
//						auto loadResult = loadStage.pollFuture.get();
						assetsUpdate.CompleteSignal(AssetsSync::UpdateResult::Success);
						I = mAssetUpdates.erase(I);
					} else {
						I++;
					}
				}
			}

			if (!mAssetUpdates.empty()) {
				scheduleAssetsPoll();
			}
		});
	}
}

}


