/*
    Copyright (C) 2002 Hans Häggström, Martin Pollard (Xmp)

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

#include "GuiService.h"
#include "widget/Label.h"
#include <services/logging/LoggingService.h>

void dime::GuiService::refresh()
{
  if (myDrawTarget != NULL && myRootWidget != NULL)
    {
      myRootWidget->draw(myDrawTarget);
      myDrawTarget->update();
    }
  else
    {
      dime::LoggingService::getInstance()->log(__FILE__, __LINE__, dime::LoggingService::FAILURE, "Unable to refresh Gui as no draw target has been specified or Root Widget does not exist");
    }    
}

/*
  void dime::GuiService::MouseMotion(InputDevice *mouse, const SDLKey &key, dime::InputMapping::InputSignalType signaltype)
  {
  std::vector<int> coords(2);
  coords[0] = mouse->getAxisPosition(0);
  coords[1] = mouse->getAxisPosition(1);
  myRootWidget.checkMouseEvent(coords);
  }
  
  void dime::GuiService::MouseClick(InputDevice *mouse, const SDLKey &key, dime::InputMapping::InputSignalType signaltype)
  {
  std::vector<int> coords(2);
  coords[0] = mouse->getAxisPosition(0);
  coords[1] = mouse->getAxisPosition(1);
  dime::LoggingService::getInstance()->slog(__FILE__, __LINE__, dime::LoggingService::INFO) << "Mouse click of button " << key << " at (" << coords[0] << ", " << coords[1] << ") received" << ENDM;
  myRootWidget.checkMouseEvent(coords);
  }
*/

dime::GuiService::GuiService()
{
    setName( "GuiService" );
    setDescription( "Handles the Widgets that make up the GUI" );
    
    myRootWidget = new Widget();
    myEventGenerator = new dime::EventGenerator(myRootWidget);
    myRootWidget->setEventGenerator(myEventGenerator);
    
    myDrawTarget = NULL;
    
    dime::LoggingService::getInstance()->log(__FILE__, __LINE__, dime::LoggingService::INFO, "GuiService initialized.");
    myInputService = dime::InputService::getInstance();  
}

long dime::GuiService::createWidget(xmlNodePtr widgetNode, xmlDocPtr doc)
{
  // Parses the node with the widget info in it

  // Be paranoid if we've not been given a widget node bail
  if (xmlStrcmp(widgetNode->name, (const xmlChar *)"widget"))
    return 0;

  // Loop this time looking for type
  xmlAttrPtr attr = widgetNode->properties;

  while (attr != NULL) {
    if ((!xmlStrcmp(attr->name, (const xmlChar *)"type"))){
      // Check value
      if (!xmlStrcmp(xmlNodeListGetString(doc, attr->xmlChildrenNode, 1),
		     (const xmlChar *)"label"))
      {
	return parseLabel(widgetNode, doc);
      }

      // Even if we are not the correct name bail as
      // there should only be one type per widget
      // Shouldn't get here unless widget is unrecognised
      break;
    }
	
    attr = attr->next;
  }

  return 0;
}

long dime::GuiService::parsePanel(xmlNodePtr widgetNode, xmlDocPtr doc)
{
  return 0; // Alter this to return proper value
}

long dime::GuiService::parseLabel(xmlNodePtr widgetNode, xmlDocPtr doc)
{
  // Get name
  xmlGetProp(widgetNode, (const xmlChar *)"name");

  // Get data
  string data = (const char*)xmlGetProp(widgetNode, (const xmlChar *)"data");

  // Get rectangle
  // FIXME: there has to be a better way to do this
  dime::Rectangle rect( atoi((const char*)xmlGetProp(widgetNode, (const xmlChar *)"x")), atoi((const char*)xmlGetProp(widgetNode, (const xmlChar *)"y")),
		   atoi((const char*)xmlGetProp(widgetNode, (const xmlChar *)"w")), atoi((const char*)xmlGetProp(widgetNode, (const xmlChar *)"h")));

  dime::Label* newLabel = new dime::Label(data, rect);

  myRootWidget->addWidget(newLabel);

  return 0; // Change this to proper return
}

void dime::GuiService::nukeWidget( long id )
{
}

void dime::GuiService::nukeAllWidgets()
{
  myRootWidget->removeAllWidgets();
}
