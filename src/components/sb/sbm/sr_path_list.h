/*
 *   - part of SmartBody-lib
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
 *      Marcus Thiebaux, USC
 *      Andrew n marshall, USC
 *      Ed Fast, USC
 */

#ifndef SR_PATH_LIST_H
#define SR_PATH_LIST_H

#include <vhcl.h>
#include <cstdio>
#include <string>
#include <vector>

//////////////////////////////////////////////////////////////////////////////////

class srPathList
{
	public:
		srPathList();
		virtual ~srPathList();

		bool insert(std::string path);
		bool remove(std::string path);
		void removeAll();
		void reset();
		std::string next_path(bool addPrefix = true);
		std::string next_filename(char *buffer, const char *name);
		std::vector<std::string>& getPaths()	{ return _paths; }
	private:
		std::string _prefix;
		std::vector<std::string> _paths;
		int _curIndex;
};

//////////////////////////////////////////////////////////////////////////////////
#endif
