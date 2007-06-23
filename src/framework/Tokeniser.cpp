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

// Rewritten for Ember by Martin Pollard (Xmp)

// Originally written for Sear by Simon Goodall, University of Southampton
// Original Copyright (C) 2001 - 2002 

#include "Tokeniser.h"

namespace Ember
{


Tokeniser::Tokeniser()
: mDelimeters(" ")
{
}

Tokeniser::Tokeniser(const std::string &tokens)
: mDelimeters(" ")
{
	initTokens(tokens);
}

Tokeniser::Tokeniser(const std::string &tokens, const std::string &delimiters)
: mDelimeters(delimiters)
{
	initTokens(tokens);
}


void Tokeniser::initTokens(const std::string &tokens) {
  mTokenString = tokens;
//  try {
    mLastPos = mTokenString.find_first_not_of(mDelimeters, 0);
    mPos = mTokenString.find_first_of(mDelimeters, mLastPos);
//  } catch (...) {
//
//  }
 
}

std::string Tokeniser::nextToken() {
  if (mLastPos == std::string::npos) return "";
//  try {
    std::string token = mTokenString.substr(mLastPos, mPos - mLastPos);
    mLastPos = mTokenString.find_first_not_of(mDelimeters, mPos);
    mPos = mTokenString.find_first_of(mDelimeters, mLastPos);
    return token;
//  } catch (...) {
//    return "";
//  }
}

std::string Tokeniser::remainingTokens() {
  if (mLastPos == std::string::npos) return "";
//try {  
    return mTokenString.substr(mLastPos, mTokenString.size() - mLastPos);
//  } catch (...) {
//    return "";
//  }
}                                

}// end of namespace Ember
