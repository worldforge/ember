/*
 Copyright (C) 2001,2002  Martin Pollard (Xmp), Lakin Weckerd (nikal)

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// Redefines main() on Win32 and MacOS
#include <SDL_main.h>

#include "Application.h"
#include "framework/Tokeniser.h"

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

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <iostream>

#include "framework/binreloc.h" //this is needed for binreloc functionality

#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char **argv)
{
	int exitStatus(0);
	bool exit_program = false;
	std::string prefix("");
	std::string homeDir("");
	Ember::Application::ConfigMap configMap;
#ifndef __WIN32__
	if (argc > 1)
	{
		std::string invoked = std::string((char *) argv[0]);
		(argv)++;
		argc--;
		while (argc > 0)
		{
			std::string arg = std::string((char *) argv[0]);
			argv++;
			argc--;
			if (arg == "-v" || arg == "--version")
			{
				std::cout << "Ember version: " << VERSION << std::endl;
				exit_program = true;
			}
			else if (arg == "-h" || arg == "--help")
			{
				std::cout << invoked << " {options}" << std::endl;
				std::cout << "-h, --help    - display this message" << std::endl;
				std::cout << "-v, --version - display version info" << std::endl;
				std::cout << "--home <path>- sets the home directory to something different than the default (~/.ember on *NIX systems, $APPDATA\\Ember on win32 systems)" << std::endl;
				std::cout << "-p <path>, --prefix <path> - sets the prefix to something else than the one set at compilation (only valid on *NIX systems)" << std::endl;
				std::cout << "--config <section>:<key> <value> - allows you to override config file settings. See the ember.conf file for examples. (~/.ember/ember.conf on *NIX systems)" << std::endl;
				exit_program = true;
			}
			else if (arg == "-p" || arg == "--prefix")
			{
				if (!argc)
				{
					std::cout << "You didn't supply a prefix.";
					exit_program = true;
				}
				else
				{
					prefix = std::string((char *) argv[0]);
					argv++;
					argc--;
				}

			}
			else if (arg == "--home")
			{
				if (!argc)
				{
					std::cout << "You didn't supply a home directory.";
					exit_program = true;
				}
				else
				{
					homeDir = std::string((char *) argv[0]);
					argv++;
					argc--;
				}

			}
			else if (arg == "--config")
			{
				if (argc < 2)
				{
					std::cout << "You didn't supply any config arguments.";
				}
				else
				{
					std::string fullkey(argv[0]);
					std::string value(argv[1]);
					Ember::Tokeniser tokeniser(fullkey, ":");
					if (tokeniser.remainingTokens() != "")
					{
						std::string category(tokeniser.nextToken());
						if (tokeniser.remainingTokens() != "")
						{
							std::string key(tokeniser.nextToken());
							configMap[category][key] = value;
						}
					}
				}
			}
		}
	}

#if !defined(__WIN32__) && !defined(__APPLE__)
	if (exit_program)
	{
		if (homeDir != "")
		{
			if (chdir(homeDir.c_str()))
			{
				std::cerr << "Could not set homedir to '" << homeDir << "'." << std::endl;
			}
		}
		else
		{
			if (chdir("~/.ember"))
			{
				std::cerr << "Could not set homedir to '~/.ember'." << std::endl;
			}
		}
		return 0;
	}
#endif

#ifdef ENABLE_BINRELOC
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
	// 	if (prefix == "") {
	// 		prefix = PREFIX;
	// 	}

#else
	//  char tmp[64];

	//  unsigned int floatSetting = _controlfp( 0, 0 );
	//sprintf(tmp, "Original: 0x%.4x\n", floatSetting );
	//   MessageBox( 0, tmp, "floating point control", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	//_fpreset();
	//_controlfp(_PC_64, _MCW_PC);
	//_controlfp(_RC_NEAR , _MCW_RC);
	//floatSetting = _controlfp( 0, 0 );
	//sprintf(tmp, "New: 0x%.4x\n", floatSetting );
	//   MessageBox( 0, tmp, "floating point control", MB_OK | MB_ICONERROR | MB_TASKMODAL);

#endif

	///put the application object in its own scope so it gets destroyed before we signal all clear
	{
		try
		{
			/// Create application object
			Ember::Application app(prefix, homeDir, configMap);
			//EmberOgre::EmberOgre app;

			std::cout << "Starting Ember version " << VERSION << std::endl;

			app.registerComponents();

			/// Initialize all Ember services needed for this application
			app.prepareComponents();
			app.initializeServices();

			app.start();
		} catch (const std::exception& ex)
		{
			std::cerr << "Unexpected error, aborting.\n\r" << ex.what();
			exitStatus = 1;
		}
	}

#if !defined(__WIN32__) && !defined(__APPLE__)
	if (homeDir != "")
	{
		if (chdir(homeDir.c_str()))
		{
			std::cerr << "Could not set homedir to '" << homeDir << "'." << std::endl;
		}
	}
	else
	{
		if (chdir("~/.ember"))
		{
			std::cerr << "Could not set homedir to '~/.ember'." << std::endl;
		}
	}
#endif
	std::cout << "Ember shut down successfully." << std::endl;
	return exitStatus;
}
