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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

//#include "test/TestService.h"
#include "logging/LoggingService.h"
#include "config/ConfigService.h"
//#include "input/InputService.h"
//#include "gui/GuiService.h"
#include "sound/SoundService.h"
#include "metaserver/MetaserverService.h"
#include "server/ServerService.h"


#include "EmberServices.h"

namespace Ember{

EmberServices* EmberServices::theInstance = NULL;

  EmberServices::~EmberServices()
  {
    // TODO: Free any allocated resources here.
    delete myConfigService;
    //delete myGuiService;
    delete myMetaserverService;
    delete myServerService;
  }

  EmberServices* EmberServices::getInstance()
  {
    if( !theInstance )
      {
	theInstance = new EmberServices();
      }
    
    return theInstance;
  }

  TestService* EmberServices::getTestService()
  {
    // TODO
    return NULL;
  }

  LoggingService* EmberServices::getLoggingService()
  {
    return Ember::LoggingService::getInstance();
  }
  
  ConfigService* EmberServices::getConfigService()
  {
    if (myConfigService == NULL)
      {
	myConfigService = new Ember::ConfigService();
      }
    
    return myConfigService;
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
    if (myMetaserverService == NULL)
      myMetaserverService = new Ember::MetaserverService();
    return myMetaserverService;
  }
  
  ServerService* EmberServices::getServerService()
  {
    if (myServerService == NULL)
      myServerService = new Ember::ServerService();
    return myServerService;
  }

  SoundService* EmberServices::getSoundService()
  {
    if (mySoundService == NULL)
      mySoundService = new Ember::SoundService();
    return mySoundService;
  }

  EmberServices::EmberServices()
  {
    myConfigService = NULL;
    //myGuiService = NULL;
    myMetaserverService = NULL;
    myServerService = NULL;
    mySoundService = NULL;
  }

}
