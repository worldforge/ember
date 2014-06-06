/*
 *  bml_event.hpp - part of SmartBody-lib
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

#ifndef BML_EVENT_HPP
#define BML_EVENT_HPP

#include <sb/SBTypes.h>
#include "bml.hpp"


namespace BML {



	BML::BehaviorRequestPtr parse_bml_event( DOMElement* elem, const std::string& unique_id, BML::BehaviorSyncPoints& behav_syncs, bool required, BML::BmlRequestPtr request, SmartBody::SBScene* scene );

	class EventRequest : public SequenceRequest {
		protected:
			const std::string oldMessage;
			const std::string pythonMessage;
			std::string syncPointName;
		
		public:
			EventRequest( const std::string& unique_id, const std::string& localId, const std::string& oldStyleMessage, const std::string& pythonMessage,
						  const BehaviorSyncPoints& syncs_in, std::string spName );
		
			void realize_impl( BmlRequestPtr request, SmartBody::SBScene* scene );

			SBAPI const std::string getMessage();
			SBAPI std::string getSyncPointName();
	};
};


#endif // BML_EVENT_HPP