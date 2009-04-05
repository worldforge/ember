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

namespace Eris
{
	class View;
}

namespace EmberOgre
{
	class EmberOgre;
}
namespace Ember
{
	class EmberServices;
	class LogObserver;

	/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
	
	The main application class. There will only be one instance of this in the system, and this holds all other objects in the system. Ember is created and destroyed with this instance.
	
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
	*/
	void mainLoopStep();

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
	 *    @brief Registers all components with the system.
	 * Make sure to call this before calling prepareComponents(). This will allow all components to register themselves with the system, but won't do anything more.
	 */
	void registerComponents();
	
	/**
	 *    @brief Prepares all components.
	 * Make sure to call this after you've called registerComponents(). This will tell all components to prepare themselves before the application and services are started. The reason this is separate from registerComponents() is that some components needs to know about the existence of others, which they might not properly do at the registerComponents() step.
	 */
	void prepareComponents();
	
	/**
	 *    @brief Initializes all services.
	 * Make sure to call this before calling start() and after calling registerComponents() and prepareComponents().
	 */
	void initializeServices();
	
	/**
	 *    @brief Starts the application.
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
	 * Normally Eris is polled each frame. A "poll" means that Eris is asked to send and recieve any data from the server and act on it.
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
	 */
	sigc::signal<void> EventStartErisPoll;
	
	/**
	 * @brief Emitted after the eris polling has finished.
	 */
	sigc::signal<void> EventEndErisPoll;
	
	/**
	 * @brief Emitted before processing input. This event is emitted continously.
	 */
	sigc::signal<void> EventBeforeInputProcessing;
	
	/**
	 * @brief Emitted after processing input. This event is emitted continously.
	 */
	sigc::signal<void> EventAfterInputProcessing;
	
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
	
	/**
	 * @brief We listen to the GotView event to be able to store a reference to the View instance.
	 * @see mWorldView
	 * @param view The world view.
	 */
	void Server_GotView(Eris::View* view);

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
