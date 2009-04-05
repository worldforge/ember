//
// C++ Implementation: Exception
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2008
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdarg.h>
#include <stdio.h>

#include "Exception.h"

namespace Ember
{

Exception::Exception()
	: mErrorDescription("Unknown Exception")
{
}


Exception::Exception(const std::string& error)
	: mErrorDescription(error)
{
	mLine = -1;
	mFile = "";
}


Exception::Exception(const std::string& error, const std::string & file, int line, ...)
	: mFile(file), mLine(line)
{
	char buffer[EXCEPTION_TEXT_SIZE];
	va_list va;
	va_start (va, line);
	vsnprintf(buffer, EXCEPTION_TEXT_SIZE, error.c_str(), va);
	mErrorDescription = buffer;
	va_end (va);
}

Exception::~Exception() throw()
{
}

const std::string& Exception::getError() const
{
	return mErrorDescription;
}
	
void Exception::setError(const std::string& error)
{
	mErrorDescription = error;
}

const char* Exception::what() const throw()
{
	return mErrorDescription.c_str();
}

}
