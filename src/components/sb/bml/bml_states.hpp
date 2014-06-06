#ifndef BML_STATES_HPP
#define BML_STATES_HPP

#include "bml.hpp"
#include "sbm/BMLDefs.h"

// Forward Declaration


namespace BML
{
	BML::BehaviorRequestPtr parse_bml_states( DOMElement* elem, const std::string& unique_id, BML::BehaviorSyncPoints& behav_syncs, bool required, BML::BmlRequestPtr request, SmartBody::SBScene* scene );
};

#endif