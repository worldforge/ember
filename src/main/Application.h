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

#include "services/EmberServices.h"
#include "framework/ConsoleObject.h"

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

    class Application : public ConsoleObject, public Ember::Singleton<Application>
    {
        //======================================================================
        // Inner Classes, Typedefs, and Enums
        //======================================================================
    public:
        //======================================================================
        // Public Methods
        //======================================================================
         Application(const std::string prefix, const std::string homeDir, bool useBinReloc);

        /**
         * Dtor for Ember::Application.  Free the current surface.
         *
         */
        ~Application();
       
        /**
         * Main loop step.   Does one iteration of the mainloop. 
         *
         *
         */
        void mainLoopStep();

        /**
         * the Main loop.  Returns when application has received an "exit" command
         *
         */
        void mainLoop();
       
        /**
         * return true if application has received an "exit" command else false.
         *
         * @return true if "shouldquit" else false
         */
        bool shouldQuit();
        
        void registerComponents();
        void prepareComponents();
        void initializeServices();
        void start();
        
        sigc::signal<void> EventServicesInitialized;
        
        

	/**
	 * Causes the application to quit.
	 */
	void quit();
	   
		
	/**
	 * Callback for running Console Commands
	 */
	void runCommand(const std::string&,const std::string&);

	/**
	 *    Sets whether eris should be polled each frame. Defaults to true.
	 * @param doPoll 
	 */
	void setErisPolling(bool doPoll);

	/**
	 *    Gets whether eris should be polled each frame.
	 * @return 
	 */
	bool getErisPolling() const;
	
	/**
	 * Emitted when the use wants to quit the game. Preferrebly the GUI should show some kind of confirmation window.
	 */ 
	sigc::signal<void, bool&> EventRequestQuit;
	
	/**
	Emitted before the eris polling is started
	*/
	sigc::signal<void> EventStartErisPoll;
	
	/**
	Emitted after the eris polling has finished
	*/
	sigc::signal<void> EventEndErisPoll;
	
	/**
	 *    Call this to "soft quit" the app. This means that an signal will be emitted, which hopefully will be taken care of by some widget, which will show a confirmation window, asking the user if he/she wants to quit.
	 However, if there is no widget etc. handling the request, the application will instantly quit.
	 */
	void requestQuit();

	/**
	 * Accessor for the main eris world view, if any.
	 */
	Eris::View* getMainView();
    
    private:
    
     
//	IGameView mGraphicalComponent;
	EmberOgre::EmberOgre* mOgreView;
	bool mShouldQuit;
	const std::string mPrefix;
	const std::string mHomeDir;
	bool mUseBinreloc;
	
	/**
	The main log observer used for all logging.
	*/
	LogObserver* mLogObserver;
	
	/**
	The main services object.
	*/
	EmberServices* mServices;
	
	/**
	Once connected to a world, this will hold the main world view.
	*/
	Eris::View* mWorldView;
	
	/**
	Controls whether eris should be polled at each frame update.
	*/
	bool mPollEris;
	
	void Server_GotView(Eris::View* view);

	std::auto_ptr<std::ofstream> mLogOutStream;

};//class Application
}//namespace Ember

#endif
