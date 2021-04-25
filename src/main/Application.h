/*
 *  File:       Application.h
 *  Summary:    The class which initializes the GUI.
 *  Written by: nikal
 *
 *  Copyright (C) 2001, 2002 nikal.
 *  This code is distributed under the GPL.
 *  See file COPYING for details.
 *
 */

#ifndef EMBER_APPLICATION_H
#define EMBER_APPLICATION_H

#include "services/EmberServices.h"
#include "framework/ConsoleCommandWrapper.h"
#include "framework/ConsoleObject.h"
#include "framework/ConsoleBackend.h"
#include "framework/MainLoopController.h"

#include <Atlas/Objects/Factories.h>

#include <list>
#include <string>
#include <fstream>
#include <map>
#include <unordered_map>
#include <memory>
#include <framework/FileSystemObserver.h>

/**
 * @mainpage
 *
 * Ember is a client for the Worldforge virtual world system. It uses the 3d library <a href="http://www.ogre3d.org">OGRE</a> to show the world to the user, and the <a href="http://www.cegui.org.uk">CEGUI</a> library to present a user interface.
 * Two of the design tenets of Ember is to both be as modular as possible, and to reuse existing FOSS components as much as possible. The list of dependencies for the client is therefore quite hefty, but the advantage is the wealth of capabilities.
 *
 * @section SystemDesign System design
 *
 * The system is laid out using a decoupled and layered design, where components in the lower levels are used by components in the higher, but not vice versa.
 *
 * List of system levels, as defined by the directory layout:
 * - framework
 * 	Basic functionality which can be used by pretty much any component in the system is placed here. This is the lowest level and has no dependency on any other level in the system.
 * - extensions
 * 	Sometimes we need to extend the libraries used by Ember, such as Atlas or Eris. Any extension added should then be moved to the real library, but while it's either being developed it can be put in this layer.
 * - services
 * 	Commonly used services are to be placed here. It might not always be clear what constitutes a service, but the most common characteristic of a service is an adapted API over an external component. A service should be light weight and not keep to much state. The services should never depend on anything which is handled by the components (for example Ogre, CEGUI, Lua etc.).
 * - components
 *	The meat of the application is placed here. These are highly complex components which provide the main functionality of what is shown to the user.
 *	- carpenter
 *		A system for combining blocks of geometry into more advanced shapes, a little bit like Lego. Not currently in use.
 *	- cegui
 *		Bindings and setup classes for the CEGUI system. Note that the actual widgets are not placed here, but rather in the "ogre" component.
 *	- entitymapping
 *		A system for mapping entities to different actions. This is used to determine how to present entities to the user.
 *	- lua
 *		Bindings and setup classes for the lua scripting language interaction. Note that the actual bindings class bindings are spread out in the code base.
 *	- ogre
 *		The bulk of the code resides here, as this deals with the representation of the world through the Ogre 3d library.
 *
 * @section ScriptBindings Script bindings
 *
 * For various tasks, such as UI interaction and entity creation, a scripting language is used. While the ScriptingService allows for any scripting language to be used we have settled on the <a href="http://www.lua.org">Lua</a> language. We use the <a href="http://www.codenix.com/~tolua/">tolua++ tool</a> for generating the bindings needed.
 * These bindings are not kept in one central location, but instead are spread out throughout the codebase. They can be found under the "bindings/lua" directories. When creating a new binding, or altering an existing one a developer needs to edit the *.pkg files found in these directories.
 *
 * Throughout Ember we use sigc++ for signal binding and passing. In order to expose these signals to Lua we've designed a mechanism using "connectors". The main bulk of the code for that can be found in src/components/lua. When you extend the bindings with new signals, you need to define these signals so that the connector system properly recognizes them. In order to do this you must alter the code in "src/bindings/lua".
 * When implementing a new sigc++-to-lua binding you sometimes need to define a method which can translate between a C++ type to a Lua class name. Do this by extending the src/bindings/TypeResolving.cpp. Note that all of the types, even for those only defined in subcomponents, are defined in this one place. The main reason is to better fit with how tolua++ handles function overriding.
 *
 * @section CodingGuidelines Coding guidelines
 *
 * A coding style guideline can be found in the "doc/template_header.h" file. If you're using Eclipse as your IDE there's a code formatting style available in EclipseCodeStyle.xml.
 */

namespace Atlas {
namespace Objects {
class Factories;
}
}

namespace Eris {
class View;
}

/**
 * @brief The main namespace for Ember.
 *
 */
namespace Ember {
struct Session;

class Input;
/**
 * @brief All Ogre specific functionality is contained under this namespace. It currenly also houses all CEGUI related functionality too.
 *
 */
namespace OgreView {
class EmberOgre;
}

class FileSystemObserver;

class EmberServices;

class LogObserver;

struct IResourceProvider;

class ConfigConsoleCommands;

class ConsoleInputBinder;

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 *
 * @brief The main application class. There will only be one instance of this in the system, and this holds all other objects in the system. Ember is created and destroyed with this instance.
 *
 *
 * After creating it, be sure to call these methods in order:
 *
 * registerComponents();
 * initializeServices();
 *
 * start();
 */
class Application : public ConsoleObject, public virtual sigc::trackable {
public:
	typedef std::unordered_map<std::string, std::map<std::string, std::string>> ConfigMap;

	/**
	 * @brief Ctor.
	 * @param prefix The prefix, i.e. the path in the filesystem where the main application is installed.
	 * @param homeDir The path to the Ember home directory. On a UNIX system this was historically "~/.ember". By default, this is now set according to the XDG Base Directory Specification.
	 * @param configSettings Command line configuration settings.
	 */
	Application(Input& input,
				ConfigMap configSettings,
				ConfigService& configService);

	/**
	 * @brief At destruction pretty much all game objects will be destroyed.
	 */
	~Application() override;

	/**
	 * @brief Enters the main loop.
	 * Will loop through the application until it exits. In most cases you want to call this for the main loop. However, if you want to handle all looping yourself you can call mainLoopStep() manually.
	 */
	void mainLoop();

	/**
	 * @brief Registers all components with the system.
	 */
	void registerComponents();


	/**
	 * @brief Initializes all services.
	 * Make sure to call this before calling start() and after calling registerComponents().
	 */
	void initializeServices();

	/**
	 * @brief Starts the application.
	 * Calling this will make it first setup the graphical components and then enter the main loop.
	 * @see mainLoop()
	 */
	void start();

	/**
	 * @brief Callback for running Console Commands
	 */
	void runCommand(const std::string& command, const std::string& args) override;

private:

	Input& mInput;

	ConfigService& mConfigService;

	Atlas::Objects::Factories mAtlasFactories;

	std::unique_ptr<Session> mSession;

	std::unique_ptr<FileSystemObserver> mFileSystemObserver;

	/**
	 * @brief The main Ogre graphical view.
	 */
	std::unique_ptr<OgreView::EmberOgre> mOgreView;

	/**
	 * @brief If set to true, Ember should quit before next loop step.
	 * @see mainLoop()
	 */
	bool mShouldQuit;

	/**
	 * @brief Controls whether eris should be polled at each frame update.
	 */
	bool mPollEris;

	/**
	 * @brief The main loop controller instance, which mainly controls whether the application should quit or not.
	 */
	MainLoopController mMainLoopController;

	/**
	 * @brief The main services object.
	 */
	std::unique_ptr<EmberServices> mServices;

	/**
	 * @brief Once connected to a world, this will hold the main world view.
	 */
	Eris::View* mWorldView;

	/**
	 * @brief The main log observer used for all logging.
	 * The default implementation is to write all log messages to a file out stream.
	 */
	std::unique_ptr<LogObserver> mLogObserver;

	/**
	 * @brief A transient copy of command line set config settings. The settings here will be injected into the ConfigService when the services are started.
	 * @see initializeServices()
	 */
	ConfigMap mConfigSettings;

	/**
	 * @brief The main console backend instance.
	 */
	std::unique_ptr<ConsoleBackend> mConsoleBackend;

	std::unique_ptr<ConfigConsoleCommands> mConfigConsoleCommands;

	std::unique_ptr<ConsoleInputBinder> mConsoleInputBinder;

	/**
	 * @brief The "quit" command will quit the application, bypassing any confirmation dialog.
	 */
	const ConsoleCommandWrapper Quit;

	/**
	 * @brief Toggles the polling of data from eris. Normally Eris is polled each frame, but this can be turned off (mainly for debug reasons).
	 */
	const ConsoleCommandWrapper ToggleErisPolling;

	/**
	 * @brief Provides resources to the scripting system.
	 */
	std::unique_ptr<IResourceProvider> mScriptingResourceProvider;

	/**
	 * @brief We listen to the GotView event to be able to store a reference to the View instance.
	 * @see mWorldView
	 * @param view The world view.
	 */
	void Server_GotView(Eris::View* view);

	/**
	 * @brief We listen to the DestroyedView event so that we can remove our View reference.
	 */
	void Server_DestroyedView();

	/**
	 * @brief Starts the scripting system.
	 */
	void startScripting();

};
}

#endif
