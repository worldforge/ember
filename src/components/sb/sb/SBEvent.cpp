//#include "vhcl_log.h"
#include "SBEvent.h"
#include <sb/SBScene.h>
#include <sb/SBCommandManager.h>
#include <sb/SBScene.h>
#include <sb/SBAttribute.h>

#include <boost/algorithm/string/replace.hpp>

namespace SmartBody {

SBEventHandler::SBEventHandler() : SBObject()
{
	m_type = "";
	m_action = "";
	_enabled = true;
	this->createBoolAttribute("enable", true, true, "Event", 10, false, false, false, "Whether this event is enabled.");
}

void SBEventHandler::setEnable(bool val)
{
	_enabled = val;
	BoolAttribute* boolAttribute = dynamic_cast<BoolAttribute*>(this->getAttribute("enable"));
	boolAttribute->setValueFast(val);
}

bool SBEventHandler::isEnable()
{
	return _enabled;
}


void SBEventHandler::notify(SBSubject* subject)
{
	SBObject::notify(subject);

	SBAttribute* attribute = dynamic_cast<SBAttribute*>(subject);
	if (attribute)
	{
		if (attribute->getName() == "enable")
		{
			BoolAttribute* boolAttr = dynamic_cast<BoolAttribute*>(attribute);
			if (boolAttr)
			{
				bool val = boolAttr->getValue();
				this->setEnable(val);
			}
		}
	}
}


SBBasicHandler::SBBasicHandler() : SBEventHandler() 
{
	this->createStringAttribute("eventName", "", true, "Event", 20, false, false, false, "Name of the event that this handler will respond to.");
	this->createStringAttribute("eventAction", "", true, "Event", 30, false, false, false, "Behavior of event when triggered (command run using Python).");
}

void SBBasicHandler::setAction(const std::string& action)
{ 
	m_action = action;
	StringAttribute* stringAttr = dynamic_cast<StringAttribute*>(this->getAttribute("eventAction"));
	stringAttr->setValueFast(action);
}

const std::string& SBBasicHandler::getAction()
{ 
	return m_action;
}

#ifdef EMBER_SB_PYTHON
void SBBasicHandler::executeAction(SBEvent* event) 
{
	/*
	std::string action = getAction();
	boost::replace_all(action, "$1", event->getParameters());

	
	SmartBody::SBScene::getScene()->getCommandManager()->execute((char*) action.c_str());
	*/
	StringAttribute* stringAttr = dynamic_cast<StringAttribute*>(this->getAttribute("eventAction"));
	SmartBody::SBScene::getScene()->run(stringAttr->getValue());
}
#endif

void SBBasicHandler::notify(SBSubject* subject)
{
	SBEventHandler::notify(subject);

	SBAttribute* attribute = dynamic_cast<SBAttribute*>(subject);
	if (attribute)
	{
		if (attribute->getName() == "eventAction")
		{
			StringAttribute* stringAttr = dynamic_cast<StringAttribute*>(attribute);
			if (stringAttr)
			{
				setAction(stringAttr->getValue());
			}
		}
	}
}



SBEventManager::SBEventManager()
{
}

SBEventManager::~SBEventManager()
{
	for (std::map<std::string, SBEventHandler*>::iterator iter = eventHandlers.begin();
		 iter != eventHandlers.end();
		 iter++)
	{
		SBEventHandler* handler = (*iter).second;
//		delete handler;
	}
}

void SBEventManager::handleEvent(SBEvent* e, double time)
{
	// find the appropriate event handler
	std::map<std::string, SBEventHandler*>::iterator iter = eventHandlers.find(e->getType());
	if (iter == eventHandlers.end())
		return;

	SBEventHandler* handler = (*iter).second;
	if (handler->isEnable())
		handler->executeAction(e);	
}

SBEvent* SBEventManager::createEvent(const std::string& type, const std::string parameters)
{
	SBEvent* event = new SBEvent();
	event->setType(type);
	event->setParameters(parameters);

	return event;
}

void SBEventManager::addEventHandler(const std::string& type, SBEventHandler* handler)
{
	removeEventHandler(type);

	handler->setName(type);

	eventHandlers.insert(std::pair<std::string, SBEventHandler*>(type, handler));
}

void SBEventManager::removeEventHandler(const std::string& type)
{
	std::map<std::string, SBEventHandler*>::iterator iter = eventHandlers.find(type);
	if (iter != eventHandlers.end())
	{
		SBEventHandler* oldHandler = (*iter).second;
		eventHandlers.erase(iter);
		//delete oldHandler; // deleting old handler causes crash when handler is created with Python - need to fix this
	}
}

int SBEventManager::getNumEventHandlers()
{
	return eventHandlers.size();
}

SBEventHandler* SBEventManager::getEventHandlerByIndex(int num)
{
	int counter = 0;
	for (std::map<std::string, SBEventHandler*>::iterator iter = eventHandlers.begin();
		 iter != eventHandlers.end();
		 iter++)
	{
		if (counter == num)
		{
			SBEventHandler* handler = (*iter).second;
			return handler;
		}
	}

	return NULL;
}

SBEventHandler* SBEventManager::getEventHandler(const std::string& type)
{
	std::map<std::string, SBEventHandler*>::iterator iter = eventHandlers.find(type);
	if (iter != eventHandlers.end())
	{
		SBEventHandler* handler = (*iter).second;
		return handler;
	}

	return NULL;
}



}
