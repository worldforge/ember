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

#include "RendererFactory.h"
#include <iostream>
using namespace std;
namespace dime {

RendererFactory* RendererFactory::theInstance = NULL;

Renderer* RendererFactory::getRenderer( const std::string& name,
					const Rectangle& rect,
					xmlNodePtr stateNode,
					xmlDocPtr doc )
{
  // Paranoid code
  if (xmlStrcmp(stateNode->name, (const xmlChar *)"stateinfo")){
    // Perhaps throw exception here
    return NULL;
  }
  xmlNodePtr cur = stateNode->xmlChildrenNode;

  // Search for renderers within StateInfo
  while (cur != NULL) {
    xmlNodePtr cur2 = cur->xmlChildrenNode;

    // Parse a layout
    if (!xmlStrcmp(cur->name, (const xmlChar *)"renderers")){
      while (cur2 != NULL) {
	// For each widget in a layout create it
	if (!xmlStrcmp(xmlGetProp(cur2,(const xmlChar*)"name"),
		       (const xmlChar*)name.c_str()))
	  {
	    return parseRenderer(rect, cur2, doc);
	  }
	cur2 = cur2->next;
      }

      // Should only be one layout in a state so return
      return NULL;
    }

    cur = cur->next;
  }
  return NULL;
}

Renderer* RendererFactory::parseRenderer( const Rectangle& rect,
					  xmlNodePtr rendererNode,
					  xmlDocPtr doc )
{
  if (!xmlStrcmp(rendererNode->name, (const xmlChar *)"bitmap"))
  {
    BitmapRenderer::BitmapStyle style = BitmapRenderer::CENTER;

    xmlChar* align = xmlGetProp(rendererNode, (const xmlChar *)"align");
    if (!xmlStrcmp(align, (const xmlChar *)"CENTER"))
      style = BitmapRenderer::CENTER;

    // Do we need to delete align?
    // note the const char* is unsafe
    return new BitmapRenderer(rect, (const char*)xmlGetProp(rendererNode, (const xmlChar *)"src"),style);
  }
  return NULL;
}

}
