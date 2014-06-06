/*
 *  me_default_prune_policy.cpp - part of Motion Engine and SmartBody-lib
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
 *      Andrew n marshall, USC
 */

#include "me_default_prune_policy.hpp"



bool MeDefaultPrunePolicy::shouldPrune( MeController* ct, MeController *parent ) {
	int children = ct->count_children();
	for( int i=0; i<children; ++i ) {
		MeController* child = ct->child( i );
		MePrunePolicy* prune_policy = child->prune_policy();
		if( prune_policy != NULL ) {
			if( !prune_policy->shouldPrune( child, ct ) )
				return false;
		}
	}

	// All child controllers are pruneable
	return true;
}
