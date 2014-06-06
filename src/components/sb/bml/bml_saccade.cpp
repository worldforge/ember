/*
 *  bml_saccade.cpp - part of Motion Engine and SmartBody-lib
 *  Copyright (C) 2011  University of Southern California
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

#include <xercesc/util/XMLStringTokenizer.hpp>

#include "bml_saccade.hpp"


#include "controllers/me_ct_saccade.h"

#include "bml_xml_consts.hpp"
#include "bml_event.hpp"
#include "sbm/xercesc_utils.hpp"
#include "sbm/BMLDefs.h"

using namespace std;
using namespace BML;
using namespace xml_utils;
BehaviorRequestPtr BML::parse_bml_saccade( DOMElement* elem, const std::string& unique_id, BML::BehaviorSyncPoints& behav_syncs, bool required, BML::BmlRequestPtr request, SmartBody::SBScene* scene )
{
	MeCtSaccade* saccade_ct = request->actor->saccade_ct;
	if (!saccade_ct)
	{
		LOG("Character %s does not have a saccade controller, so cannot saccade.", request->actor->getName().c_str());
		return BehaviorRequestPtr();
	}

	saccade_ct->setValid(true);
	saccade_ct->setUseModel(true);	
	const XMLCh* id = elem->getAttribute(BMLDefs::ATTR_ID);
	std::string localId;
	if (id)
		xml_utils::xml_translate(&localId, id);
	
	float duration = 0.03f;
	float magnitude = 3.0f;
	float direction = 45.0f;
	std::string dur = xml_utils::xml_translate_string(elem->getAttribute(BMLDefs::ATTR_DURATION));
	if (dur != "")
	{
		saccade_ct->setUseModel(false);
		duration = (float)atof(dur.c_str());
	}
	std::string mag = xml_utils::xml_translate_string(elem->getAttribute(BMLDefs::ATTR_MAGNITUDE));
	if (mag != "")
	{
		saccade_ct->setUseModel(false);
		magnitude = (float)atof(mag.c_str());
	}
	std::string dir = xml_utils::xml_translate_string(elem->getAttribute(BMLDefs::ATTR_DIRECTION));
	if (dir != "")
	{
		saccade_ct->setUseModel(false);
		direction = (float)atof(dir.c_str());
	}
	if (!saccade_ct->getUseModel())
	{
		saccade_ct->spawnOnce(direction, magnitude, duration);
		return BehaviorRequestPtr( new EventRequest(unique_id, localId, "", "", behav_syncs, ""));
	}

	//-----
	std::string bMode;
	xml_utils::xml_translate(&bMode, elem->getAttribute(BMLDefs::ATTR_MODE));
	if (bMode != "")
	{
		if (bMode == "talk")
			saccade_ct->setBehaviorMode(MeCtSaccade::Talking);
		else if (bMode == "listen")
			saccade_ct->setBehaviorMode(MeCtSaccade::Listening);
		else if (bMode == "think")
			saccade_ct->setBehaviorMode(MeCtSaccade::Thinking);
		else
		{
			LOG("BML::parse_bml_saccade ERR: this mode not recognized");
			return BehaviorRequestPtr(); 
		}
	}

	/*
	// deprecated by attributes
	float bin0 = xml_utils::xml_translate_float(elem->getAttribute(BMLDefs::ATTR_PERCENTAGE_BIN0));
	float bin45 = xml_utils::xml_translate_float(elem->getAttribute(BMLDefs::ATTR_PERCENTAGE_BIN45));
	float bin90 = xml_utils::xml_translate_float(elem->getAttribute(BMLDefs::ATTR_PERCENTAGE_BIN90));
	float bin135 = xml_utils::xml_translate_float(elem->getAttribute(BMLDefs::ATTR_PERCENTAGE_BIN135));
	float bin180 = xml_utils::xml_translate_float(elem->getAttribute(BMLDefs::ATTR_PERCENTAGE_BIN180));
	float bin225 = xml_utils::xml_translate_float(elem->getAttribute(BMLDefs::ATTR_PERCENTAGE_BIN225));
	float bin270 = xml_utils::xml_translate_float(elem->getAttribute(BMLDefs::ATTR_PERCENTAGE_BIN270));
	float bin315 = xml_utils::xml_translate_float(elem->getAttribute(BMLDefs::ATTR_PERCENTAGE_BIN315));
	if (bin0 != 0.0f && bin45 != 0.0f && bin90 != 0.0f && bin135 != 0.0f &&
		bin180 != 0.0f && bin225 != 0.0f && bin270 != 0.0f && bin315 != 0.0f)
	{
		saccade_ct->setPercentageBins(bin0, bin45, bin90, bin135, bin180, bin225, bin270, bin315);
	}

	float mean = xml_utils::xml_translate_float(elem->getAttribute(BMLDefs::ATTR_SACCADE_INTERVAL_MEAN));
	float variant = xml_utils::xml_translate_float(elem->getAttribute(BMLDefs::ATTR_SACCADE_INTERVAL_VARIANT));
	if (mean != 0.0f && variant != 0.0f)
	{
		saccade_ct->setGaussianParameter(mean, variant);
	}

	float limitAngle = xml_utils::xml_translate_float(elem->getAttribute(BMLDefs::ATTR_ANGLELIMIT));
	if (limitAngle != 0)
	{
		float angle = limitAngle;
		saccade_ct->setAngleLimit(angle);
	}
	*/

	std::string finishFlag;
	xml_utils::xml_translate(&finishFlag, elem->getAttribute(BMLDefs::ATTR_FINISH));
	if (finishFlag != "")
	{
		if (finishFlag == "true" || finishFlag == "TRUE")
			saccade_ct->setValid(false);
		else
		{
			LOG("BML::parse_bml_saccade ERR: this tag not recognized");
			return BehaviorRequestPtr();
		}
	}

	return BehaviorRequestPtr( new EventRequest(unique_id, localId, "", "", behav_syncs, ""));
}
