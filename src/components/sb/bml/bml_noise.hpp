#ifndef BML_NOISE_HPP
#define BML_NOISE_HPP

#include "bml.hpp"


// Forward Declaration


namespace BML {

	BML::BehaviorRequestPtr parse_bml_noise( DOMElement* elem, const std::string& unique_id, BML::BehaviorSyncPoints& behav_syncs, bool required, BML::BmlRequestPtr request, SmartBody::SBScene* scene );
};


#endif // BML_SACCADE_HPP
