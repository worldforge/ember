/*
 Copyright (C) 2002  Simon Goodall, Martin Pollard (Xmp)
	
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details. 

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

// Rewritten for Dime by Martin Pollard (Xmp)

// Originally written for Sear by Simon Goodall, University of Southampton
// Original Copyright (C) 2001 - 2002 

#include "Tokeniser.h"

namespace dime
{
const std::string Tokeniser::delimeters = " ";
void Tokeniser::initTokens(const std::string &tokens) {
  token_string = tokens;
//  try {
    last_pos = token_string.find_first_not_of(delimeters, 0);
    pos = token_string.find_first_of(delimeters, last_pos);
//  } catch (...) {
//
//  }
 
}

std::string Tokeniser::nextToken() {
  if (last_pos == std::string::npos) return "";
//  try {
    std::string token = token_string.substr(last_pos, pos - last_pos);
    last_pos = token_string.find_first_not_of(delimeters, pos);
    pos = token_string.find_first_of(delimeters, last_pos);
    return token;
//  } catch (...) {
//    return "";
//  }
}

std::string Tokeniser::remainingTokens() {
  if (last_pos == std::string::npos) return "";
//try {  
    return token_string.substr(last_pos, token_string.size() - last_pos);
//  } catch (...) {
//    return "";
//  }
}                                

}// end of namespace dime
