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

xmlNodePtr StateManager::findState(const std::string& state)
{
  xmlNodePtr cur = xmlDocGetRootElement(myStateDoc)->xmlChildrenNode;

  while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar *)"stateinfo"))){
      // Second phase parse this time looking for name
      cur = cur->xmlChildrenNode;

      while (cur != NULL) {
	if ((!xmlStrcmp(cur->name, (const xmlChar *)"name"))){
	  // Check value
	  if ((!xmlStrcmp(xmlNodeListGetString(myStateDoc, cur->xmlChildrenNode, 1),
			  (const xmlChar *)state.c_str()))){
	    return cur;
	  }

	  // Even if we are not the correct name bail as
	  // there should only be one name per stateinfo
	  break;
	}
	
	cur = cur->next;
      }

    }

    cur = cur->next;
  }

  // If we've to here then we probably don't have that state
  THROW("State not found");
}

bool StateManager::setState( const std::string& newState )
{
  // Find the new state in statefile
  xmlNodePtr nstate = findState(newState);

  // If state not found bail
  if (!nstate)
    return false;

  // Unload myCurrentState

  // Load new state

  return true;
}
