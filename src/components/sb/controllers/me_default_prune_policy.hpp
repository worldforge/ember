/*
 *  me_default_prune_policy.hpp - part of Motion Engine and SmartBody-lib
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
#ifndef ME_DEFAULT_PRUNE_POLICY_HPP
#define ME_DEFAULT_PRUNE_POLICY_HPP


class MeDefaultPrunePolicy;

#include "me_prune_policy.hpp"


class MeDefaultPrunePolicy : public MePrunePolicy {
public:
	bool shouldPrune( MeController* ct, MeController *parent );
};


#endif // ME_DEFAULT_PRUNE_POLICY_HPP