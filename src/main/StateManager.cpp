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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "StateManager.h"
#include "services/DimeServices.h"
#include "services/gui/GuiService.h"
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

  // Build new states layout
  parseStateLayout(nstate);

  // Load new state

  return true;
}

void StateManager::parseStateLayout(xmlNodePtr stateNode)
{
  dime::GuiService* gs = dime::DimeServices::getInstance()->getGuiService();
  // Paranoid code
  if ((xmlStrcmp(stateNode->name, (const xmlChar *)"stateinfo"))){
    // Perhaps throw exception here
    return;
  }
  xmlNodePtr cur = stateNode->xmlChildrenNode;

  // Search for Layout within StateInfo
  while (cur != NULL) {
    xmlNodePtr cur2 = cur->xmlChildrenNode;

    // Parse a layout
    if (!xmlStrcmp(cur->name, (const xmlChar *)"layout")){
      while (cur2 != NULL) {
	// For each widget in a layout create it
	gs->createWidget(stateNode, cur2, myStateDoc);
	cur2 = cur2->next;
      }

      // Should only be one layout in a state so return
      return;
    }

    cur = cur->next;
  }

  return;
}
