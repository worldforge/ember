/*
    Copyright (C) 2002  Martin Pollard (Xmp)

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

#include "StateManager.h"
#include "framework/Exception.h"

// Get in LibXML2
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

using namespace dime;

StateManager::StateManager(const std::string& stateFile)
  : myStateFile(stateFile), myStateDoc(NULL)
{
  // Load the XML Document containing the States
  xmlNodePtr cur;

  myStateDoc = xmlParseFile(myStateFile.c_str());
  if (!myStateDoc)
    {
      xmlFreeDoc(myStateDoc);
      throw "Missing State Doc";
    }

  cur = xmlDocGetRootElement(myStateDoc);

  if (!cur)
    {
      xmlFreeDoc(myStateDoc);
      throw "Empty State Doc";
    }

  if (xmlStrcmp(cur->name, (const xmlChar *) "states"))
    {
      xmlFreeDoc(myStateDoc);
      throw "document of the wrong type, root node != states";
    }
  // OK if we're here then we've loaded the doc
}


xmlNodePtr StateManager::findState(const std::string& state)
{
  xmlNodePtr cur = xmlDocGetRootElement(myStateDoc)->xmlChildrenNode;

  while (cur != NULL) {
    if (!xmlStrcmp(cur->name, (const xmlChar *)"stateinfo")){
      // Second phase parse this time looking for name
      xmlAttrPtr attr = cur->properties;

      while (attr != NULL) {
	if ((!xmlStrcmp(attr->name, (const xmlChar *)"name"))){
	  // Check value
	  if ((!xmlStrcmp(xmlNodeListGetString(myStateDoc, attr->xmlChildrenNode, 1),
			  (const xmlChar *)state.c_str()))){
	    return cur;
	  }

	  // Even if we are not the correct name bail as
	  // there should only be one name per stateinfo
	  break;
	}
	
	attr = attr->next;
      }

    }

    cur = cur->next;
  }

  // If we've to here then we probably don't have that state
  return NULL;
}

#if 1
#include "Application.h"
#include <services/gui/widget/Button.h>
#include <services/gui/widget/Panel.h>
#include <services/gui/widget/TextBox.h>
#include <services/gui/widget/Console.h>
void quitButton(dime::Button* button);
#endif

bool StateManager::setState( const std::string& newState )
{
  // Find the new state in statefile
  xmlNodePtr nstate = findState(newState);

  // If state not found bail
  if (!nstate)
    return false;

  // Unload myCurrentState
  xmlNodePtr cstate = findState(myCurrentState);

  // Erase Widgets
  dime::DimeServices::getInstance()->getGuiService()->nukeAllWidgets();

  // Unload current services not mentioned in nstate

  // Load missing services mentioned in nstate

  // Create new state's widgets
  parseStateInfoWidgets(nstate);

  // Load new state

#if 0
  return true;
#endif
  if (newState == "initial state")
    {
      // Create the Widgets
      dime::Console* myTestConsole = new dime::Console(dime::Rectangle(10,300,620,120));
      dime::TextBox* myTestTextBox = new dime::TextBox("TextBox!", dime::Rectangle(13,43,97,32));
      dime::Button* myTestButton = new dime::Button(dime::Rectangle(535,450,100,25));
      dime::Panel* myTestPanel = new dime::Panel(dime::Rectangle(550,0,90,90));
            
      // Set Additional options for myTestButton
      myTestButton->setBackground(new dime::BitmapRenderer(myTestButton->getRectangle(),"quitbutton2.png", dime::BitmapRenderer::TILE));
      myTestButton->setHighlightBackground(new dime::BitmapRenderer(myTestButton->getRectangle(),"quitbutton.png", dime::BitmapRenderer::TILE));
      myTestButton->setPressedBackground(new dime::BitmapRenderer(myTestButton->getRectangle(),"quitbutton3.png", dime::BitmapRenderer::TILE));
      // Set Additional options for my TestPanel
      myTestPanel->setBackground(new dime::BitmapRenderer(myTestPanel->getRectangle(),"dimelogo_small2.png", dime::BitmapRenderer::CENTER));
      // Bind myButton onClicked method
      myTestButton->onClicked.connect(SigC::slot(quitButton));

      dime::DimeServices::getInstance()->getGuiService()->getRootWidget().addWidget(myTestPanel);
      dime::DimeServices::getInstance()->getGuiService()->getRootWidget().addWidget(myTestTextBox);
      dime::DimeServices::getInstance()->getGuiService()->getRootWidget().addWidget(myTestConsole);
      dime::DimeServices::getInstance()->getGuiService()->getRootWidget().addWidget(myTestButton);
      return true;
    } else if ( newState == "OOG View" ) {
      return true;
    }else if ( newState == "OGRE View" ) {
      return true;
    }
  return false;
}

void StateManager::parseStateInfoWidgets(xmlNodePtr widgetNode)
{
  dime::GuiService* gs = dime::DimeServices::getInstance()->getGuiService();
  // Paranoid code
  if ((xmlStrcmp(widgetNode->name, (const xmlChar *)"stateinfo"))){
    // Perhaps throw exception here
    return;
  }
  xmlNodePtr cur = widgetNode->xmlChildrenNode;

  while (cur != NULL) {
    if (!xmlStrcmp(cur->name, (const xmlChar *)"widget")){
      gs->createWidget(cur,myStateDoc);
   }
    cur = cur->next;
  }

  return;
}
