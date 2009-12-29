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

#ifndef EMBER_TOKENISER_H
#define EMBER_TOKENISER_H

#include <string>

namespace Ember {

/**
 * @author Martin Pollard (Xmp) aka Xmp
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 *
 * @brief Accepts an input string and splits the string up into separate "tokens" which can be handled individually.
 * The main use for this is in a console environment where you need to act on the arguments submitted along with a command.
 * For example, a typical scenario would be that the command "set_value foo bar" was entered. The command object would then be "set_value", and the following string would need to be split into "tokens" (i.e. two tokens, "foo" and "bar").
 */
class Tokeniser
{
	protected:
	std::string::size_type mPos, mLastPos;
	std::string mTokenString;
	const std::string mDelimeters;
	
	public:


	/**
	* Creates a new Tokeniser using default values.
	*/
	Tokeniser();
	
	/**
	 * @brief Creates a new Tokeniser using the supplied string, using a space character as delimiter.
	 * @param tokens The string which we want split into tokens.
	 */
	Tokeniser(const std::string &tokens);
	
	/**
	 * @brief Creates a new Tokeniser using the supplied string, using the supplied delimiter.
	 * @param tokens The string which we want split into tokens.
	 * @param delimiters The delimiter to use for splitting up the tokens.
	 */
	Tokeniser(const std::string &tokens, const std::string &delimiters);

	virtual ~Tokeniser ()
	{
	}

	
	/**
	 * @brief Initializes the tokeniser with the supplied string.
	 * This allows you to initialize an already existing tokeniser with a new string.
	 * @param tokens 
	 */
	void initTokens(const std::string &tokens);
	
	/**
	 * @brief Advances to the next token and returns it.
	 * @return The next token.
	 */
	std::string nextToken();

	/**
	 * @brief Returns the string of remaining tokens.
	 * @return The remaining tokens.
	 */
	std::string remainingTokens() const;

	/**
	 * @brief Returns whether there are any remaining tokens.
	 * @return True if there are any remaining tokens.
	 */
	bool hasRemainingTokens() const;
}; 

}

#endif
