//
// C++ Implementation: AvatarLogger
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2008
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "AvatarLogger.h"
#include "Avatar.h"
#include "EmberEntity.h"
#include "EmberPhysicalEntity.h"
#include "AvatarEmberEntity.h"
#include "services/config/ConfigService.h"
#include "services/time/TimeService.h"



#include <sigc++/signal.h>

namespace EmberOgre {

AvatarLogger::AvatarLogger(AvatarEmberEntity& avatarEntity)
: mChatLogger(0),
  ToggleChatLogging("toggle_chatlogging", this, "Toggle character chat logging.")
{
	assert(&avatarEntity);
	///perform setup of the stream
	std::stringstream logFileSS;
	logFileSS << Ember::EmberServices::getSingleton().getConfigService()->getHomeDirectory() << "/" << avatarEntity.getName() << "_chatlog.log";
	mChatLogger = std::auto_ptr<std::ofstream>(new std::ofstream(logFileSS.str().c_str(), std::ios::app));
	S_LOG_VERBOSE("Chat Logging set to write in [ " << logFileSS.str() << " ]");
	
	*mChatLogger << "-------------------------------------------------------" << std::endl;
	*mChatLogger << "Chat Logging Initialized at " <<  Ember::EmberServices::getSingleton().getTimeService()->getLocalTimeStr() << std::endl;
	*mChatLogger << "-------------------------------------------------------" << std::endl;
	
	GUIManager::getSingleton().AppendIGChatLine.connect(sigc::mem_fun(*this, &AvatarLogger::GUIManager_AppendIGChatLine));
}


AvatarLogger::~AvatarLogger()
{
	*mChatLogger << "-------------------------------------------------------" << std::endl;
	*mChatLogger << "Chat Logging Ended at " <<  Ember::EmberServices::getSingleton().getTimeService()->getLocalTimeStr() << std::endl;
	*mChatLogger << "-------------------------------------------------------" << std::endl;
}

void AvatarLogger::GUIManager_AppendIGChatLine(const std::string& message, EmberEntity* entity)
{
	*mChatLogger << "[" << Ember::EmberServices::getSingleton().getTimeService()->getLocalTimeStr() << "] <" <<  entity->getName() << "> says: " << message << std::endl;
} 

void AvatarLogger::runCommand(const std::string &command, const std::string &args)
{
	if( ToggleChatLogging == command) {
		S_LOG_VERBOSE("Toggle Chat Logging");
		toggleChatLogging();
	} 
}
void AvatarLogger::toggleChatLogging()
{
	if(Ember::EmberServices::getSingletonPtr()->getConfigService()->itemExists("general", "logchatmessages")) {
		S_LOG_VERBOSE("general:logchatmessages exists");
		if(static_cast<bool>(Ember::EmberServices::getSingletonPtr()->getConfigService()->getValue("general","logchatmessages")))
		{
			S_LOG_VERBOSE("general:logchatmessages was true, setting to false");
			// chat config is set to true, turn it off
			Ember::EmberServices::getSingletonPtr()->getConfigService()->setValue("general","logchatmessages",false);
		} 
		else
	    {
			S_LOG_VERBOSE("general:logchatmessages was false, setting to true");
			// chat was set to false, turn it on
			Ember::EmberServices::getSingletonPtr()->getConfigService()->setValue("general","logchatmessages",true);
	    }
	} else {
		S_LOG_VERBOSE("general:logchatmessages does not exist, adding");
		// if the config key doesn't exist, we assume off and turn it on.
		Ember::EmberServices::getSingletonPtr()->getConfigService()->setValue("general","logchatmessages",true);
	}
}

AvatarLoggerParent::AvatarLoggerParent(Avatar& avatar)
{
	///we either already have an entity, or we need to wait until it's creeated
	if (avatar.getAvatarEmberEntity()) {
		mLogger = std::auto_ptr<AvatarLogger>(new AvatarLogger(*avatar.getAvatarEmberEntity()));
	} else
	{
		avatar.EventCreatedAvatarEntity.connect(sigc::mem_fun(*this, &AvatarLoggerParent::Avatar_CreatedAvatarEntity));
	}
}

void AvatarLoggerParent::Avatar_CreatedAvatarEntity(AvatarEmberEntity* entity)
{
	mLogger = std::auto_ptr<AvatarLogger>(new AvatarLogger(*entity));
}


}
