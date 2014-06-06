/*
 *  bml_event.cpp - part of SmartBody-lib
 *  Copyright (C) 2008  University of Southern California
 *
 *  SmartBody-lib is free software: you can redistribute it and/or
 *  modify it under the terms of the Lesser GNU General Public License
 *  as published by the Free Software Foundation, version 3 of the
 *  license.
 *
 *  SmartBody-lib is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Lesser GNU General Public License for more details.
 *
 *  You should have received a copy of the Lesser GNU General Public
 *  License along with SmartBody-lib.  If not, see:
 *      http://www.gnu.org/licenses/lgpl-3.0.txt
 *
 *  CONTRIBUTORS:
 *      Andrew n marshall, USC
 */

#include "vhcl.h"
#include <iostream>
#include <sstream>


#include "bml_event.hpp"
#include "bml_xml_consts.hpp"
#include "sbm/BMLDefs.h"
#include "sbm/xercesc_utils.hpp"

using namespace std;
using namespace BML;
using namespace xml_utils;


BML::EventRequest::EventRequest( const std::string& unique_id, const std::string& localId, 
								 const std::string& oldStyleMessage, const std::string& message, 
								 const BehaviorSyncPoints& syncs_in, std::string spName )
						:	SequenceRequest( unique_id, localId, syncs_in,
						    /* Default Timing */ 0, 0, 0, 0, 0 ),
							oldMessage( oldStyleMessage ),
							pythonMessage( message ),
							syncPointName( spName )
{
}
	
void BML::EventRequest::realize_impl( BmlRequestPtr request, SmartBody::SBScene* scene )
{
	if (oldMessage == "" && pythonMessage == "")
		return;
	// for backwards compatibility, there are two possible behaviors:
	// 1) using stroke and old-style commands sent via vhmsg, or
	// 2) using start and python commands
	time_sec eventTime = behav_syncs.sync_stroke()->time();
	if (pythonMessage.size() > 0)
		eventTime = behav_syncs.sync_start()->time();

	VecOfSbmCommand commands;

	ostringstream cmd;
	if (oldMessage.size() > 0)
	{
		cmd << "send sbm " << oldMessage;
		//cmd << oldMessage;
	}
	else
	{
		cmd << "python " << pythonMessage;
	}

	string str = cmd.str();
	commands.push_back( new SbmCommand( str, (float) eventTime ) );

	realize_sequence( commands, scene );
}

const std::string BML::EventRequest::getMessage() 
{ 
	if (pythonMessage.size() > 0)
		return pythonMessage; 
	else
		return oldMessage;
}

std::string BML::EventRequest::getSyncPointName() 
{ 
	return syncPointName; 
}


BehaviorRequestPtr BML::parse_bml_event( DOMElement* elem, const std::string& unique_id, BehaviorSyncPoints& behav_syncs, bool required, BmlRequestPtr request, SmartBody::SBScene* scene ) {

    const XMLCh* tag      = elem->getTagName();

	// message (the event behavior) can be retrieved either via the attribute, or via the text content.
	std::string msg  = xml_parse_string( BMLDefs::ATTR_MESSAGE, elem, "");	

	std::string spNameOld = xml_parse_string( BMLDefs::ATTR_STROKE, elem, ""); // if 'stroke' is used, assume old-style command sent via VHMSG	
	std::string spNameNew = xml_parse_string( BMLDefs::ATTR_START, elem, ""); // if 'start' is used, assume Python style internally only
	std::string localId  = xml_parse_string( BMLDefs::ATTR_ID, elem, "");

	DOMNode* child = elem->getFirstChild();
	while (child)
	{
		DOMNode::NodeType type = child->getNodeType();
		if (type == DOMNode::CDATA_SECTION_NODE)
		{
			child->normalize();
			DOMText* textNode = dynamic_cast<DOMText*>(child);
			const XMLCh* messageData = textNode->getNodeValue();
			msg = xml_utils::xml_translate_string(messageData);

			break;
		}
	
		child = child->getNextSibling();
	}

	if (msg != "" )
	{	
		if (spNameNew.size() > 0)
		{
			// new style event sent via Python
			return BehaviorRequestPtr( new EventRequest( unique_id, localId, "", msg, behav_syncs, spNameNew ) );
		}
		else
		{
			// old style command send via vhmsg
			return BehaviorRequestPtr( new EventRequest( unique_id, localId, msg, "", behav_syncs, spNameOld ) );
		}
	} 
	else
	{
		xml_parse_error( BMLDefs::ATTR_MESSAGE, elem );
		return BehaviorRequestPtr();  // a.k.a., NULL
	}
}
