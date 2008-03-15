/*
    Copyright (C) 2002  Hans Häggström
    Copyright (C) 2005	Erik Hjortsberg

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

#include "EmberServices.h"

//#include "test/TestService.h"
#include "logging/LoggingService.h"
#include "config/ConfigService.h"
//#include "input/InputService.h"
//#include "gui/GuiService.h"
#include "sound/SoundService.h"
#include "metaserver/MetaserverService.h"
#include "server/ServerService.h"
#include "scripting/ScriptingService.h"
#include "wfut/WfutService.h"
#include "time/TimeService.h"


namespace Ember{


template<> Ember::EmberServices* Ember::Singleton<Ember::EmberServices>::ms_Singleton = 0;


  EmberServices::~EmberServices()
  {
  }


//   TestService* EmberServices::getTestService()
//   {
//     // TODO
//     return NULL;
//   }

  LoggingService* EmberServices::getLoggingService()
  {
    return Ember::LoggingService::getInstance();
  }

	ConfigService* EmberServices::getConfigService()
	{
		return mConfigService->getService();
	}

/*  InputService* EmberServices::getInputService()
  {
    return Ember::InputService::getInstance();
  }
  */

/*
  GuiService* EmberServices::getGuiService()
  {
    if (myGuiService == NULL)
      myGuiService = new Ember::GuiService();
    
    return myGuiService;
  }
*/
	MetaserverService* EmberServices::getMetaserverService()
	{
		return mMetaserverService->getService();
	}
  
	ServerService* EmberServices::getServerService()
	{
		return mServerService->getService();
	}

	SoundService* EmberServices::getSoundService()
	{
		return mSoundService->getService();
	}

	ScriptingService* EmberServices::getScriptingService()
	{
		return mScriptingService->getService();
	}
  
	Ember::WfutService* EmberServices::getWfutService()
	{
		return mWfutService->getService();
	}
  
	Ember::Services::Time* EmberServices::getTimeService()
	{
		return mTimeService->getService();
	}
	
	EmberServices::EmberServices()
	: mScriptingService(std::auto_ptr<ServiceContainer<ScriptingService> >(new ServiceContainer<ScriptingService>()) )
	, mSoundService(std::auto_ptr<ServiceContainer<SoundService> >(new ServiceContainer<SoundService>()) )
	, mServerService(std::auto_ptr<ServiceContainer<ServerService> >(new ServiceContainer<ServerService>()) )
	, mMetaserverService(std::auto_ptr<ServiceContainer<MetaserverService> >(new ServiceContainer<MetaserverService>()) )
//   , mInputService(std::auto_ptr<ServiceContainer<InputService> >(new ServiceContainer<InputService>()) )
	, mWfutService(std::auto_ptr<ServiceContainer<WfutService> >(new ServiceContainer<WfutService>()) )
	, mConfigService(std::auto_ptr<ServiceContainer<ConfigService> >(new ServiceContainer<ConfigService>()) )
	, mTimeService(std::auto_ptr<ServiceContainer<Services::Time> >(new ServiceContainer<Services::Time>()) )
	{
	}

}
