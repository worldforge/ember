/*
 *  action_unit.hpp - part of SmartBody-lib
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
 */

#ifndef SBM_ActionUnit_HPP
#define SBM_ActionUnit_HPP

#include <map>

#include <sr/sr_shared_class.h>
#include <sr/sr_shared_ptr.hpp>
#include <sk/sk_motion.h>


/**
 *  Maps the a Facial Action Coding (FAC) Action Unit
 *  to some asset type.
 *
 *  Currently we use the first frame of SkMotion because
 *  of limitations in our exports (can't export direct to .skp).
 *
 *  The unit number is not stored internally.  It can be found
 *  in the ActionUnitMap as a std::pair< int, ActionUnit >.
 */
class ActionUnit
{
public:
	ActionUnit(int id, SkMotion* unified );
	ActionUnit(int id, SkMotion* left, SkMotion* right );
	ActionUnit(ActionUnit* source);
	~ActionUnit();

	SBAPI int getId();
	SBAPI void setId(int id);

	SBAPI bool is_bilateral() const;
	SBAPI void reset_type();
	SBAPI void set_left();
	SBAPI void set_bilateral();
	SBAPI void set_right();
	SBAPI bool is_left() const;
	SBAPI bool is_right() const;
	SBAPI void set( SkMotion* motion );
	SBAPI void set( SkMotion* left, SkMotion* right );
	SBAPI const std::string& getLeftName();
	SBAPI const std::string& getRightName();
	SBAPI const std::string& getBilateralName();

	SkMotion* left;
	SkMotion* right;

	protected:
		bool m_isLeft;
		bool m_isRight;
		bool m_isBilateral;
		std::string _leftName;
		std::string _rightName;
		std::string _bilateralName;
		bool _dirtyName;
		int _id;
};

#endif // SBM_ActionUnit_HPP
