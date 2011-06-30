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

#include "WebEmberManager.h"

#include "main/Application.h"

#include "framework/LoggingInstance.h"

#include <SDL_main.h>

#include <boost/thread/thread.hpp>
#include <boost/thread/locks.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>

#include <cassert>

template<> Ember::WebEmberManager *Ember::Singleton<Ember::WebEmberManager>::ms_Singleton = 0;

// References to Ember.cpp, its actually SDL_main, but SDL_main.h will add a macro for renaming this.
int main(int argc, char **argv);

namespace Ember
{

	int WebEmberManager::start(const std::string& windowHandle)
	{
		assert((windowHandle != "") && (windowHandle != "0") && (mWindowHandle == ""));
		mWindowHandle = windowHandle;
		// calls the global main function without parameters
		return main(0, 0);
	}

	void WebEmberManager::quit()
	{
		Application* app;
		// Timeout after 1 second.
		int timeout = 1000;
		while (mWindowHandle != "" && !(app = Application::getSingletonPtr())) {
			// This is a rare situation, when start() is called and after that immediatelly quit() is called.
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

	std::string WebEmberManager::getPrefix()
	{
#ifdef _WIN32
		// To find the resources, we need to set the prefix.
		// we need to assume that WebEmber.dll is in the PREFIX/bin/WebEmber.dll
		char buffer[MAX_PATH];
		GetModuleFileNameA( (HMODULE)mModuleHandle, buffer, 512 );
		int pos = 0;
		int pos2 = 0;
		//get prefix by removing "/bin/WebEmber.dll" from the end of the path.
		for(int i=0; i < MAX_PATH && buffer[i]; i++) {
			if(buffer[i] == '\\' || buffer[i] == '/') {
				pos2 = pos;
				pos = i;
			}
		}
		buffer[pos2] = 0;
		return buffer;
#else
		return "";
#endif
	}
	
	void WebEmberManager::sendMessage(std::string msg)
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
