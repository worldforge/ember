	/*
 *  sbm_speech.hpp - part of SmartBody-lib
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

#include "vhcl.h"

#include "sbm/sbm_speech.hpp"


using namespace std;
using namespace SmartBody;


VisemeData::VisemeData( const std::string& id, int numKeys, const std::string& curveInfo )
	: _id( id ), _weight( 1.0 ), _time( 0.0 ), _duration( 0.0 ), _numKeys( numKeys ), _curveInfo( curveInfo ), _curveMode(true), _trapezoidMode(false), _floatCurveMode(false), _motionMode(false)
{
	// NOTE: We are parsing twice - once here in order to extract the proper duration of the curve,
	//       and later again when processing the command. These two parses should be unified here for
	//       efficiency reasons. The parse below is a 'light' parse - although we need to tokenize the
	//       entire curve data string, we are only interested in the last control point.

	std::vector<std::string> tokens;
	vhcl::Tokenize(curveInfo, tokens, " ");
	if (tokens.size() > 0 && numKeys > 0)
	{
		// make sure the curve info is properly created
		int index = 4 * (numKeys - 1);
		if (tokens.size() <= (unsigned int) index)
		{
			LOG("Curve %s is not properly created - insufficient number of keys (%d) - expected %d.", id.c_str(), tokens.size() / 4, numKeys * 4);
		}
		else
		{
			_duration = float(atof(tokens[index].c_str()));
		}
	}
}


ostream& operator << ( ostream& strm, const VisemeData& v ) {
	return strm << "VisemeData: " << (v.id()) << " (" << (v.weight()) <<") @ " << (v.time());
}
