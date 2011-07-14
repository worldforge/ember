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

#include "WebEmberManager.h"

#include "framework/LoggingInstance.h"

#ifdef _WIN32
#include "platform/platform_windows.h"
#define WEBEMBER_EXPORT __declspec(dllexport) __stdcall
#else
#define WEBEMBER_EXPORT __attribute__((visibility("default")))
#endif

#include <sstream>

using namespace Ember;
#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Start WebEmber as a child window of a given HWND.
 *
 * This function will run ember and will only return, when ember quits.
 *
 * @param hwnd The window handle of the owner.
 * @returns Returns the exit value of ember.
 */

int WEBEMBER_EXPORT StartWebEmber(const char* windowHandle, const char* prefix)
{
	return WebEmberManager::getSingleton().start(windowHandle, prefix);
}

/**
 * @brief Signals quit for WebEmber.
 *
 * This function will return immediately and ember will shut down before drawing the next frame.
 */
void WEBEMBER_EXPORT QuitWebEmber()
{
	WebEmberManager::getSingleton().quit();
}


#ifdef _WIN32
/**
 * @brief DllMain is the entry point for DLLs.
 *
 * Here you can do some initialization and uninitialization.
 * For more info, read: http://msdn.microsoft.com/en-us/library/ms682583%28v=vs.85%29.aspx
 */
BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch(ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
		// DLL is loaded into the memory
		// if you return here FALSE the DLL will not be loaded and linking fails.
		new Ember::WebEmberManager(hModule);
		break;
		case DLL_PROCESS_DETACH:
		// DLL is unloaded from the memory
		delete Ember::WebEmberManager::getSingletonPtr();
		break;
	}
	return TRUE;
}
#else

//Set library constructor and destructor functions.
void __attribute__ ((constructor)) initWebEmber(void);
void __attribute__ ((destructor)) deinitWebEmber(void);

// Called when the library is loaded and before dlopen() returns
void initWebEmber(void)
{
	new Ember::WebEmberManager;
}

// Called when the library is unloaded and before dlclose()
void deinitWebEmber(void)
{
	delete Ember::WebEmberManager::getSingletonPtr();
}

#endif

#ifdef __cplusplus
}
#endif
