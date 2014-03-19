//
// C++ Implementation: CEGUILogger
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

#include "CEGUILogger.h"
#include "framework/LoggingInstance.h"
#include <CEGUI/Exceptions.h>

namespace Ember {
namespace Cegui {

CEGUILogger::CEGUILogger()
{
	registerConfigListener("cegui", "minimumlogginglevel", sigc::mem_fun(*this, &CEGUILogger::Config_MinimumLogLevel));

	//By default CEGUI will print all exceptions to std::cerr as well as the log; disable this.
	CEGUI::Exception::setStdErrEnabled(false);
}


CEGUILogger::~CEGUILogger()
{
}

void CEGUILogger::logEvent(const CEGUI::String& message, CEGUI::LoggingLevel level)
{
	//just reroute to the Ember logging service
	static std::string cegui("(CEGUI) ");
	if (d_level >= level) {
		switch (level) {
			case CEGUI::Insane:
				Log::slog("CEGUI", Log::VERBOSE) << cegui << message.c_str() << Log::END_MESSAGE;
				break;
			case CEGUI::Informative:
				Log::slog("CEGUI", Log::VERBOSE) << cegui << message.c_str() << Log::END_MESSAGE;
				break;
			case CEGUI::Standard:
				Log::slog("CEGUI", Log::INFO) << cegui << message.c_str() << Log::END_MESSAGE;
				break;
			case CEGUI::Warnings:
				Log::slog("CEGUI", Log::WARNING) << cegui << message.c_str() << Log::END_MESSAGE;
				break;
			case CEGUI::Errors:
				Log::slog("CEGUI", Log::FAILURE) << cegui << message.c_str() << Log::END_MESSAGE;
				break;
		}
	}
}


void CEGUILogger::Config_MinimumLogLevel(const std::string& section, const std::string& /*key */, varconf::Variable& variable)
{
	std::string newLogLevel(variable);
	
	S_LOG_INFO("Setting CEGUI logging level to '" << newLogLevel << "'.");

	if (newLogLevel == "insane") {
		d_level = CEGUI::Insane;
	} else if (newLogLevel == "informative") {
		d_level = CEGUI::Informative;
	} else if (newLogLevel == "standard") {
		d_level = CEGUI::Standard;
	} else if (newLogLevel == "warnings") {
		d_level = CEGUI::Warnings;
	} else if (newLogLevel == "errors") {
		d_level = CEGUI::Errors;
	} 
}

}
}
