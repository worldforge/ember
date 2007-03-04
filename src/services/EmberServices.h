/*
    Copyright (C) 2002  Hans Häggström

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

#ifndef DIME_SERVICES_H
#define DIME_SERVICES_H

#include "framework/Singleton.h"
#include <memory>


namespace Ember {

  // some forward declarations before we start
  class LoggingService;
  class ConfigService;
  class InputService;
  //class GuiService;
  class MetaserverService;
  class ServerService;
  class SoundService;
  class TestService;
  class ScriptingService;
  class WfutService;
  



template <typename T>
  class UninitializedInnerServiceContainer;
  
template <typename T>
class IInnerServiceContainer
{
public:
	virtual T* getService() = 0;

};

/**
A simple container for a service. Upon the first call to getService(), a new instance will be created.
The main reason for doing it this way is to remove an extra null check every time a service is accessed.
*/
template <typename T>
class ServiceContainer
{
public:
	ServiceContainer()
	: mInnerContainer(new UninitializedInnerServiceContainer<T>(this))
	{
	}

	T* getService()
	{
		return mInnerContainer->getService();
	}
	
	void setInnerContainer(IInnerServiceContainer<T>* newContainer)
	{
		mInnerContainer = std::auto_ptr<IInnerServiceContainer<T> >(newContainer);
	}
private:
	std::auto_ptr<IInnerServiceContainer<T> > mInnerContainer;
};

  
template <typename T>
class InitializedInnerServiceContainer : public IInnerServiceContainer<T>
{
public:
	InitializedInnerServiceContainer()
	: mService(new T)
	{
	}
	
	virtual T* getService()
	{
		return mService.get();
	}
private:
	std::auto_ptr<T> mService;
};  
  
template <typename T>
class UninitializedInnerServiceContainer : public IInnerServiceContainer<T>
{
public:
	UninitializedInnerServiceContainer(ServiceContainer<T>* container): mContainer(container)
	{
	}

	virtual T* getService()
	{
		mContainer->setInnerContainer(new InitializedInnerServiceContainer<T>());
		return mContainer->getService();
	}
private:
	ServiceContainer<T>* mContainer;
};



/**
 * This is a singleton class that is used to access instances of all the
 * different Ember services.
 *
 * There's a getServiceName() method for each service. <p>
 *
 * TODO: Should this class also create the instances of the services,
 * or should it have set methods for them?  <p>
 *
 * Example: <p>
 * <code>
 *
 *   EmberServices.getInstance()->getLoggingService()->log( ... );  <br/>
 *   ... = EmberServices.getInstance()->getMetaServerService()->getMetaServerList();
 *
 * </code>
 *
 * @author Hans Häggström
 */
class EmberServices : public Ember::Singleton<EmberServices>
{


    public:

    EmberServices();

    /**
     * Deletes a EmberServices instance.
     */
    virtual ~EmberServices();
    
    /**
     * Returns an instance of the TestService.
     */
    Ember::TestService *getTestService();
    

    /**
     * Returns an instance of the LoggingService
     */
    Ember::LoggingService *getLoggingService();

    /**
     * Returns an instance of the ConfigService
     */
    Ember::ConfigService *getConfigService();

    /**
     * Returns an instance of the InputService
     */
    Ember::InputService *getInputService();
	
    /**
     * Returns an instance of the GuiService
     */
    //Ember::GuiService *getGuiService();

    /**
     * Returns an instance of the MetaserverService
     */
     Ember::MetaserverService *getMetaserverService();

    /**
     * Returns an instance of the ServerService
     */
    Ember::ServerService *getServerService();

    /**
     * Returns an instance of the SoundService
     */
    Ember::SoundService *getSoundService();

    /**
     * Returns an instance of the ScriptingService
     */
    Ember::ScriptingService *getScriptingService();

    /**
     * Returns an instance of the ScriptingService
     */
    Ember::WfutService *getWfutService();

    //----------------------------------------------------------------------
    // Setters

    //======================================================================
    // Disabled constructors and operators
    //======================================================================
    private:



    /**
     * Copy constructor not provided.
     */
    EmberServices( const EmberServices &source )
    {
    }


	/**
	* Assignment operator not provided.
	*/
	EmberServices &operator= ( const EmberServices &source )
	{
		return *this;
	}

private:
	
	std::auto_ptr<ServiceContainer<ScriptingService> > mScriptingService;
	std::auto_ptr<ServiceContainer<SoundService> > mSoundService;
	std::auto_ptr<ServiceContainer<ServerService> > mServerService;
	std::auto_ptr<ServiceContainer<MetaserverService> > mMetaserverService;
//     std::auto_ptr<ServiceContainer<InputService> > mInputService;
	std::auto_ptr<ServiceContainer<WfutService> > mWfutService;
	std::auto_ptr<ServiceContainer<ConfigService> > mConfigService;


};
}

#endif
