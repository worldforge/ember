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

#ifndef DIME_APPLICATION_H
#define DIME_APPLICATION_H

#include <list>
#include <string>


#include "services/EmberServices.h"
#include "framework/ConsoleObject.h"

#include <sigc++/signal.h>


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
         Application(const std::string& prefix, const std::string& homeDir, bool useBinReloc);

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

    private:
    
//	IGameView mGraphicalComponent;
	EmberOgre::EmberOgre* mOgreView;
	bool mShouldQuit;
	std::string mPrefix, mHomeDir;
	bool mUseBinreloc;
	
	/**
	The main log observer used for all logging.
	*/
	LogObserver* mLogObserver;
	
	/**
	The main services object.
	*/
	EmberServices* mServices;
		

};//class Application
}//namespace Ember

#endif
