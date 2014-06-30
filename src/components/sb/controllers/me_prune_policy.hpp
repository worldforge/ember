/*
 *  me_prune_policy.hpp - part of Motion Engine and SmartBody-lib
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

# ifndef ME_PRUNE_POLICY_HPP
# define ME_PRUNE_POLICY_HPP


class MePrunePolicy;

#include <controllers/me_controller.h>



/**
 *  Policy interface for automatically pruning controlllers from a controller tree.
 *
 *  Allows an automated pruning algorithm to react to consider custom logic when
 *  selecting controllers to prune.
 */
//  While there is no generalized pruning algorithm currently available with 
//  Motion Engine, this interface facilitates custom pruning algorithms, such
//  as the one included with SmartBody-lib (as part of SbmCharacter).
class MePrunePolicy
    : public SrSharedClass
{
public:
	virtual bool shouldPrune( MeController* ct, MeController* parent ) = 0;
};

#endif // ME_PRUNE_POLICY_HPP
