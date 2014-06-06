#include "vhcl.h"
#include <iostream>
#include <sstream>
#include <string>

#include <xercesc/util/XMLStringTokenizer.hpp>

#include "bml_noise.hpp"


#include "controllers/me_ct_noise_controller.h"

#include "bml_xml_consts.hpp"
#include "bml_event.hpp"
#include "sbm/xercesc_utils.hpp"
#include "sbm/BMLDefs.h"

using namespace std;
using namespace BML;
using namespace xml_utils;
BehaviorRequestPtr BML::parse_bml_noise( DOMElement* elem, const std::string& unique_id, BML::BehaviorSyncPoints& behav_syncs, bool required, BML::BmlRequestPtr request, SmartBody::SBScene* scene )
{
	MeCtNoiseController* noise_ct = request->actor->noise_ct;
	if (!noise_ct)
	{
		LOG("Character %s does not have a noise controller, so cannot generate Perlin Noise.", request->actor->getName().c_str());
		return BehaviorRequestPtr();
	}

	const XMLCh* id = elem->getAttribute(BMLDefs::ATTR_ID);
	std::string localId;
	if (id)
		xml_utils::xml_translate(&localId, id);
	
	std::string joints = xml_utils::xml_parse_string(BMLDefs::ATTR_JOINT_RANGE, elem);
	float scale = (float)xml_utils::xml_parse_double(BMLDefs::ATTR_SCALE, elem, 0.02f);
	float freq = (float)xml_utils::xml_parse_double(BMLDefs::ATTR_FREQUENCY, elem, 0.03f);
	std::vector<std::string> jointVec;
	vhcl::Tokenize(joints, jointVec);
	std::string enableFlag;
	noise_ct->setJointNoise(jointVec, scale, freq);
	xml_utils::xml_translate(&enableFlag, elem->getAttribute(BMLDefs::ATTR_ENABLE));
	if (enableFlag != "")
	{
		if (stringICompare(enableFlag,"true"))
			noise_ct->setValid(true);
		else if (stringICompare(enableFlag,"false"))
		{
			noise_ct->setValid(false);			
		}
	}

	return BehaviorRequestPtr( new EventRequest(unique_id, localId, "", "", behav_syncs, ""));
}
