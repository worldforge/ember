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

#ifndef TOKENISER_H
#define TOKENISER_H

#include <string>

namespace Ember {

/**
 * @author Martin Pollard (Xmp) aka Xmp
 * @author Erik Hjortsberg <erik.hjortsberg@iteam.se>
 */

class Tokeniser
{
	protected:
	std::string::size_type mPos, mLastPos;
	std::string mTokenString;
	const std::string mDelimeters;
//     static const std::string mQuotes;
//     bool quoted;

	public:


	/**
	* Creates a new Tokeniser using default values.
	*/
	Tokeniser();
	Tokeniser(const std::string &tokens);
	Tokeniser(const std::string &tokens, const std::string &delimiters);

	virtual ~Tokeniser ()
	{
	}


	void initTokens(const std::string &tokens);
	std::string nextToken();
	std::string remainingTokens();
}; // End of Tokeniser

} // End of Ember namespace

#endif
