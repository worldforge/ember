/*
 Copyright (C) 2011 Peter Szucs

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef BUILD_WEBEMBER
#error This module is meant for webember only. If you want to build webember, you should set BUILD_WEBEMBER macro.
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "WebEmberManager.h"

#include "main/Application.h"

#include "framework/LoggingInstance.h"

#include <boost/thread/thread.hpp>
#include <boost/thread/locks.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>


#include <cassert>
#include <iostream>

template<> Ember::WebEmberManager *Ember::Singleton<Ember::WebEmberManager>::ms_Singleton = 0;

namespace Ember
{

int WebEmberManager::start(const std::string& windowHandle, const std::string& prefix)
{
	assert((windowHandle != "") && (windowHandle != "0") && (mWindowHandle == ""));
	mWindowHandle = windowHandle;

	int exitStatus(0);
	bool exit_program = false;
	std::string homeDir("");
	Ember::Application::ConfigMap configMap;
	std::map<std::string, std::string> ogremap;
	ogremap.insert(std::make_pair("suppressconfigdialog", "true"));
	ogremap.insert(std::make_pair("windowhandle", mWindowHandle));
	configMap.insert(std::make_pair("ogre", ogremap));


#if defined (ENABLE_BINRELOC)
	if (prefix == "")
	{
		BrInitError error;

		if (br_init (&error) == 0 && error != BR_INIT_ERROR_DISABLED)
		{
			printf ("Warning: BinReloc failed to initialize (error code %d)\n", error);
			printf ("Will fallback to hardcoded default path.\n");
		}

		char* br_prefixdir = br_find_prefix(PREFIX);
		const std::string prefixDir(br_prefixdir);
		free(br_prefixdir);
		prefix = prefixDir;
	}

#endif
	//put the application object in its own scope so it gets destroyed before we signal all clear
	{
		try
		{
			// Create application object
			Ember::Application app(prefix, "", configMap);
			//Ember::OgreView::EmberOgre app;

			std::cout << "Starting Ember version " << VERSION << std::endl;

			app.registerComponents();

			// Initialize all Ember services needed for this application
			app.prepareComponents();
			app.initializeServices();

			app.start();
		} catch (const std::exception& ex)
		{
			std::cerr << "Unexpected error, aborting.\n\r" << ex.what();
			exitStatus = 1;
		} catch (...){
			std::cerr << "Unexpected error, aborting.\n\r";
			exitStatus = 1;
		}
	}

	std::cout << "Ember shut down successfully." << std::endl;
	mWindowHandle = "";
	return exitStatus;
}

void WebEmberManager::quit()
{
	Application* app;
	// Timeout after 1 second.
	int timeout = 1000;
	while (mWindowHandle != "" && !(app = Application::getSingletonPtr())) {
		// This is a rare situation, when start() is called and after that immediately quit() is called.
		try {
			boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		} catch (...) {
			// This can happen when the thread is interrupted or will be terminated.
			return;
		}
		if (!(--timeout)) {
			// This would leak ember thread to call quit, but saves the browser from freeze.
			// It should never happen.
			return;
		}
	}
	app->quit();
}

void WebEmberManager::sendMessage(const std::string& msg)
{
	try {
		boost::interprocess::message_queue mq(boost::interprocess::open_only, "WEBEMBER_PLUGIN");
		mq.send(msg.c_str(), msg.size() + 1, 0);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Failed to send a message to the plugin. " << ex);
	} catch (...) {
		S_LOG_WARNING("Failed to send a message to the plugin.");
	}
}
}

