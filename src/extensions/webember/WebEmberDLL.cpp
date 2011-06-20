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

/**
 * @file This file contains the DllMain and the interface for the WebEmber DLL, which will be exported.
 * @author Peter Szucs
 */

#ifndef _WIN32
#error This module can only be compiled on windows.
#endif

#ifndef BUILD_WEBEMBER
#error This module is meant for webember only. If you want to build webember, you should set BUILD_WEBEMBER macro.
#endif

#include "WebEmberManager.h"

#include "framework/LoggingInstance.h"

#include "platform/platform_windows.h"

#include <sstream>

using namespace Ember;

/**
 * @brief Start WebEmber as a child window of a given HWND.
 * @param hwnd The window handle of the owner.
 * @returns Returns the exit value of ember.
 * This function will run ember and will only return, when ember quits.
 */
int __declspec(dllexport) __stdcall StartWebEmber(HWND hwnd)
{
	std::stringstream sstr;
	sstr << reinterpret_cast<long>(hwnd);
	try {
		//WebEmberManager is platform independent, so hwnd needs to be a string.
		return WebEmberManager::getSingleton().start(sstr.str());
	} catch (const std::exception& ex) {
		// do not let exception pass the DLL interface.
		// The exception can have different implementation.
		S_LOG_WARNING("Error: Ember shut down with exception. " << ex);
	} catch (...) {
		S_LOG_WARNING("Error: Ember shut down with exception.");
	}
	return 1;
}

/**
 * @brief Signals quit for WebEmber.
 * This function will return immediatelly and ember will shut down before drawing the next frame.
 */
void __declspec(dllexport) __stdcall QuitWebEmber()
{
	WebEmberManager::getSingleton().quit();
}

/**
 * @brief DllMain is the entry point for DLLs.
 * Here you can do some initialization and uninitialization.
 * For more info, read: http://msdn.microsoft.com/en-us/library/ms682583%28v=vs.85%29.aspx
 */
#ifdef __cplusplus
extern "C"
#endif
BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch(ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		// DLL is loaded into the memory
		// if you return here FALSE the DLL will note be loaded and linking fails.
		new Ember::WebEmberManager(hModule);
		break;
	case DLL_PROCESS_DETACH:
		// DLL is unloaded from the memory
		delete Ember::WebEmberManager::getSingletonPtr();
		break;
	}
	return TRUE;
}