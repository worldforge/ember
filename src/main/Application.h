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

#include <list>
#include <string>
#include <fstream>
#include <map>

#include "services/EmberServices.h"
#include "framework/ConsoleObject.h"
#include "framework/ConsoleBackend.h"


#include <sigc++/signal.h>


/**
 * @mainpage
 *
 * Ember is a client for the Worldforge virtual world system. It uses the 3d library <a href="http://www.ogre3d.org">OGRE</a> to show the world to the user, and the <a href="http://www.cegui.org.uk">CEGUI</a> library to present a user interface.
 * Two of the design tenants of Ember is to both be as modular as possible, and to reuse existing FOSS components as much as possible. The list of dependencies for the client is therefore quite hefty, but the advantage is the wealth of capabilities.
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
 * 	Commonly used services are to be placed here. It might not always be clear what constitutes a service, but the most common characteristic of a service is an adapted API over an external component. A service should be light weight and not keep to much state.
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
 * These bindings are not kept in one central location, but instead are spread out throughout the codebase. They can be found under the "bindings/lua" directories. When creating a new binding, or altering an existing one a developer needs to edit the *.pkg files found in these directories. Once that is done the bindings needs to be recreated using the tolua++ tool. For this we provide scripts in the "scripts" directory. If you are unsure of how to use them, execute the "update_all_lua_bindings.sh" script.
 *
 * @section CodingGuidelines Coding guidelines
 *
 * A coding style guideline can be found in the "doc/template_header.h" file. If you're using Eclipse as your IDE there's a code formatting style available in EclipseCodeStyle.xml.
 */

namespace Eris
{
	class View;
}

namespace EmberOgre
{
	class EmberOgre;
}

/**
 * @brief The main namespace for Ember.
 *
 * @note The Ogre component is placed under the EmberOgre namespace to prevent clashes with Ogre (rather than Ember::Ogre).
 */
namespace Ember
{
class EmberServices;
class LogObserver;

/**
@author Erik Hjortsberg <erik.hjortsberg@gmail.com>

@brief The main application class. There will only be one instance of this in the system, and this holds all other objects in the system. Ember is created and destroyed with this instance.

After creating it, be sure to call these methods in order:

registerComponents();
prepareComponents();
initializeServices();

start();

*/
class Application : public ConsoleObject, public Ember::Singleton<Application>
{
public:
	typedef std::map<std::string, std::map<std::string, std::string> > ConfigMap;
	Application(const std::string prefix, const std::string homeDir, const ConfigMap& configSettings);

	/**
	* @brief At destruction pretty much all game objects will be destroyed.
	*/
	~Application();

	/**
	* @brief Performs one step of the main loop.
	* You only need to call this each "frame" if you're not using mainLoop().
	* @param minMillisecondsPerFrame If the fps is capped, this is the minimum milliseconds needed to spend on each frame.
	*/
	void mainLoopStep(long minMillisecondsPerFrame);

	/**
	* @brief Enters the main loop.
	* Will loop through the application until it exits. In most cases you want to call this for the main loop. However, if you want to handle all looping yourself you can call mainLoopStep() manually.
	*/
	void mainLoop();

	/**
	* @brief Return true if application has received an "exit" command else false.
	*
	* @return true if "shouldquit" else false
	*/
	bool shouldQuit();

	/**
	 * @brief Registers all components with the system.
	 * Make sure to call this before calling prepareComponents(). This will allow all components to register themselves with the system, but won't do anything more.
	 */
	void registerComponents();

	/**
	 * @brief Prepares all components.
	 * Make sure to call this after you've called registerComponents(). This will tell all components to prepare themselves before the application and services are started. The reason this is separate from registerComponents() is that some components needs to know about the existence of others, which they might not properly do at the registerComponents() step.
	 */
	void prepareComponents();

	/**
	 * @brief Initializes all services.
	 * Make sure to call this before calling start() and after calling registerComponents() and prepareComponents().
	 */
	void initializeServices();

	/**
	 * @brief Starts the application.
	 * Calling this will make it first setup the graphical components and then enter the main loop.
	 * @see mainLoop()
	 */
	void start();

	/**
	@brief Emitted when all services have been initialized.
	*/
	sigc::signal<void> EventServicesInitialized;

	/**
	 * @brief Causes the application to quit.
	 * This will instantly shut down the application, in contrast to requestQuit which will try to show a confirmation dialog to the user.
	 */
	void quit();


	/**
	 * @brief Callback for running Console Commands
	 */
	void runCommand(const std::string& command,const std::string& args);

	/**
	 * @brief Sets whether eris should be polled each frame. Defaults to true.
	 * Normally Eris is polled each frame. A "poll" means that Eris is asked to send and receive any data from the server and act on it.
	 * @param doPoll True if polling should occur each frame.
	 */
	void setErisPolling(bool doPoll);

	/**
	 *  @brief Gets whether eris should be polled each frame.
	 * @return True if polling occurs each frame.
	 */
	bool getErisPolling() const;

	/**
	 * @brief Emitted when the use wants to quit the game. Preferrebly the GUI should show some kind of confirmation window.
	 */
	sigc::signal<void, bool&> EventRequestQuit;

	/**
	 * @brief Emitted before the eris polling is started.
	 * The parameter sent is the time slice since this event last was emitted.
	 */
	sigc::signal<void, float> EventStartErisPoll;

	/**
	 * @brief Emitted after the eris polling has finished.
	 * The parameter sent is the time slice since this event last was emitted.
	 */
	sigc::signal<void, float> EventEndErisPoll;

	/**
	 * @brief Emitted before processing input. This event is emitted continously.
	 * The parameter sent is the time slice since this event last was emitted.
	 */
	sigc::signal<void, float> EventBeforeInputProcessing;

	/**
	 * @brief Emitted after processing input. This event is emitted continously.
	 * The parameter sent is the time slice since this event last was emitted.
	 */
	sigc::signal<void, float> EventAfterInputProcessing;

	/**
	 * @brief Call this to "soft quit" the app. This means that an signal will be emitted, which hopefully will be taken care of by some widget, which will show a confirmation window, asking the user if he/she wants to quit.
	 * However, if there is no widget etc. handling the request, the application will instantly quit.
	 */
	void requestQuit();

	/**
	 * @brief Accessor for the main eris world view, if any.
	 */
	Eris::View* getMainView();

private:

//	IGameView mGraphicalComponent;
	/**
	 * @brief The main Ogre graphical view.
	 */
	EmberOgre::EmberOgre* mOgreView;

	/**
	 * @brief If set to true, Ember should quit before next loop step.
	 * @see mainLoop()
	 */
	bool mShouldQuit;

	/**
	 * @brief The file system prefix to where Ember has been installed.
	 */
	const std::string mPrefix;

	/**
	 * @brief The path to the Ember home directory, where all settings will be stored.
	 * On Linux this is ~/.ember by default. On an English Windows it's c:\Document and Settings\USERNAME\Application Data\Ember.
	 */
	const std::string mHomeDir;

	/**
	 * @brief The main log observer used for all logging.
	 * The default implementation is to write all log messages to a file out stream.
	 */
	LogObserver* mLogObserver;

	/**
	 * @brief The main services object.
	 */
	EmberServices* mServices;

	/**
	 * @brief Once connected to a world, this will hold the main world view.
	 */
	Eris::View* mWorldView;

	/**
	 * @brief Controls whether eris should be polled at each frame update.
	 */
	bool mPollEris;

	long mLastTimeErisPollStart;
	long mLastTimeErisPollEnd;
	long mLastTimeInputProcessingStart;
	long mLastTimeInputProcessingEnd;

	/**
	 * @brief We listen to the GotView event to be able to store a reference to the View instance.
	 * @see mWorldView
	 * @param view The world view.
	 */
	void Server_GotView(Eris::View* view);

	void Server_DestroyedView();

	/**
	 * @brief We hold a pointer to the stream to which all logging messages are written.
	 */
	std::auto_ptr<std::ofstream> mLogOutStream;

	/**
	 * @brief A transient copy of command line set config settings. The settings here will be injected into the ConfigService when the services are started.
	 * @see initializeServices()
	 */
	ConfigMap mConfigSettings;

	/**
	 * @brief The main console backend instance.
	 */
	std::auto_ptr<ConsoleBackend> mConsoleBackend;

	/**
	 * @brief The "quit" command will quit the application, bypassing any confirmation dialog.
	 */
	const Ember::ConsoleCommandWrapper Quit;

	/**
	 * @brief Toggles the polling of data from eris. Normally Eris is polled each frame, but this can be turned off (mainly for debug reasons).
	 */
	const Ember::ConsoleCommandWrapper ToggleErisPolling;
};
}

#endif
