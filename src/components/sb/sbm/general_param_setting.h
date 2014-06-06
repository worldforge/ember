/*
*  general_param_map.h - part of SmartBody-lib
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
*      Yuyu Xu, USC
*/

#ifndef SBM_GENERAL_PARAM_SETTING_HPP
#define SBM_GENERAL_PARAM_SETTING_HPP

#include <map>
#include <string>
#include <vector>


///////////////////////////////////////////////////////////////
struct GeneralParam{
	int size;
	std::vector<std::string> char_names;
};

///////////////////////////////////////////////////////////////
//			<ExtraDataName, Shader(type, size and value)>
typedef std::map< std::string, GeneralParam * > GeneralParamMap;


#endif // SBM_SHADER_SETTING_HPP