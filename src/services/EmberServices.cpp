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

#include "test/TestService.h"
#include "logging/LoggingService.h"
#include "config/ConfigService.h"
#include "input/InputService.h"
//#include "gui/GuiService.h"
#include "sound/SoundService.h"
#include "metaserver/MetaserverService.h"
#include "server/ServerService.h"


#include "DimeServices.h"

namespace dime{

DimeServices* DimeServices::theInstance = NULL;

  DimeServices::~DimeServices()
  {
    // TODO: Free any allocated resources here.
    delete myConfigService;
    //delete myGuiService;
    delete myMetaserverService;
    delete myServerService;
  }

  DimeServices* DimeServices::getInstance()
  {
    if( !theInstance )
      {
	theInstance = new DimeServices();
      }
    
    return theInstance;
  }

  TestService* DimeServices::getTestService()
  {
    // TODO
    return NULL;
  }

  LoggingService* DimeServices::getLoggingService()
  {
    return dime::LoggingService::getInstance();
  }
  
  ConfigService* DimeServices::getConfigService()
  {
    if (myConfigService == NULL)
      {
	myConfigService = new dime::ConfigService();
      }
    
    return myConfigService;
  }

  InputService* DimeServices::getInputService()
  {
    return dime::InputService::getInstance();
  }
/*
  GuiService* DimeServices::getGuiService()
  {
    if (myGuiService == NULL)
      myGuiService = new dime::GuiService();
    
    return myGuiService;
  }
*/
  MetaserverService* DimeServices::getMetaserverService()
  {
    if (myMetaserverService == NULL)
      myMetaserverService = new dime::MetaserverService();
    return myMetaserverService;
  }
  
  ServerService* DimeServices::getServerService()
  {
    if (myServerService == NULL)
      myServerService = new dime::ServerService();
    return myServerService;
  }

  SoundService* DimeServices::getSoundService()
  {
    if (mySoundService == NULL)
      mySoundService = new dime::SoundService();
    return mySoundService;
  }

  DimeServices::DimeServices()
  {
    myConfigService = NULL;
    //myGuiService = NULL;
    myMetaserverService = NULL;
    myServerService = NULL;
    mySoundService = NULL;
  }

}
