/*
 *  bml_general_param.cpp - part of SmartBody-lib
 *  Copyright (C) 2009  University of Southern California
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
 *      Yuyu Xu, USC
 */

#include "vhcl.h"

#include <iostream>
#include <sstream>
#include <string>

#include <sb/SBScene.h>

#include "sbm/xercesc_utils.hpp"
#include "bml_general_param.hpp"
#include "sbm/general_param_setting.h"
#include "bml_xml_consts.hpp"
#include "sbm/BMLDefs.h"
#include "controllers/me_ct_channel_writer.hpp"




using namespace std;
using namespace BML;
using namespace xml_utils;

SrBuffer<float> Data_Array;

BehaviorRequestPtr BML::parse_bml_param( DOMElement* elem, const std::string& unique_id, BehaviorSyncPoints& behav_syncs, bool required, BmlRequestPtr request, SmartBody::SBScene* scene ) {
    const XMLCh* tag		= elem->getTagName();
	const XMLCh* param_name	= elem->getAttribute( BMLDefs::ATTR_NAME );
	const XMLCh* value		= elem->getAttribute( BMLDefs::ATTR_VALUE );

	const XMLCh* id = elem->getAttribute(BMLDefs::ATTR_ID);
	std::string localId;
	xml_utils::xml_translate(&localId, id);

	std::string valueStr;
	xml_utils::xml_translate(&valueStr, value);
	std::string paramNameStr;
	xml_utils::xml_translate(&paramNameStr, param_name);

	int channel_size = 0;
	int Index = 0;
	
	GeneralParamMap::const_iterator pos	=	SmartBody::SBScene::getScene()->getGeneralParameters().begin();
	GeneralParamMap::const_iterator end	=	SmartBody::SBScene::getScene()->getGeneralParameters().end();

	string char_name;
	string unique_id_copy = unique_id;
	int name_start_pos = unique_id_copy.find_first_of("_");
	int name_end_pos = unique_id_copy.find_first_of("_", name_start_pos + 1);
	char_name = unique_id_copy.substr(name_start_pos+1, name_end_pos-name_start_pos-1);


	int flag = 0;		//Error Checking Signal
	for(; pos!= end; pos++)
	{
		if( flag == 1 ) break;
		for(int i = 0 ; i < (int)pos->second->char_names.size(); i++)
		{
			if( pos->second->char_names[i] == char_name)
			{
				Index ++;
				if (pos->first == paramNameStr)
				{ 
					channel_size = pos->second->size; 
					flag = 1; 
					break; 
				}
			}
		}
	}
	if(flag == 0)
	{	
		std::wstringstream wstrstr;
		wstrstr<<"WARNING: Cannot find the parameter name! Please check your initialization in Sequence File\n";
		LOG(convertWStringToString(wstrstr.str()).c_str());
		return BehaviorRequestPtr();
	}

	MeCtChannelWriter * channelWriter = new MeCtChannelWriter();
	// Name controller with behavior unique_id
	ostringstream name;
	name << unique_id;
	channelWriter->setName( name.str() );

	SkChannelArray Param_Channel;
	Data_Array.size(channel_size);
	char* pch =strtok((char *)valueStr.c_str(), " ");
	for(int i = 0; i < channel_size ; i++)
	{
		if(pch == NULL) 
		{
			flag = 0; 
			LOG("WARNING: The input value size is invalid! Size is smaller than required");
			return BehaviorRequestPtr();
		}
		Data_Array[i] = (float)atof(pch);
		pch =(strtok(NULL," "));
		std::stringstream joint_name;
		joint_name << char_name << "_" << Index << "_" << ( i + 1 );
		std::string channel_name = joint_name.str();
		Param_Channel.add(channel_name,SkChannel::XPos);
	}
	
	pch = (strtok(NULL, " "));
	if(pch != NULL) 
	{
		flag = 0; 
		LOG("WARNING: The input value size is invalid! Size is larger than required");
		return BehaviorRequestPtr();
	}

	channelWriter->init(const_cast<SbmCharacter*>(request->actor),Param_Channel,true);
	channelWriter->set_data(Data_Array);
	// assign some (arbitrary) transition duration
	channelWriter->inoutdt(0.5,0.5);
	return BehaviorRequestPtr( new MeControllerRequest ( unique_id, localId, channelWriter, request->actor->param_sched_p, behav_syncs ) );
}
