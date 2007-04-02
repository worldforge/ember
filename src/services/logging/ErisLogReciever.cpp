//
// C++ Implementation: ErisLogReciever
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2007
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
#include "ErisLogReciever.h"

namespace Ember {

ErisLogReciever::ErisLogReciever(LoggingService& logService)
: mLogService(logService)
{
    //Hook up to Eris's logging Service
    Eris::Logged.
        connect (sigc::mem_fun (*this, &ErisLogReciever::Eris_Logged));

}


ErisLogReciever::~ErisLogReciever()
{
}

void ErisLogReciever::Eris_Logged (Eris::LogLevel level, const std::string & msg)
{
    LoggingService::MessageImportance importance;


    // Translate Eris importance's to ours

    switch (level)
    {
    case Eris::LOG_ERROR:
        importance = LoggingService::CRITICAL;
        break;
    case Eris::LOG_WARNING:
        importance = LoggingService::WARNING;
        break;
    case Eris::LOG_NOTICE:
      importance=LoggingService::INFO;
      break;
    case Eris::LOG_VERBOSE:
    default:
        importance = LoggingService::VERBOSE;
    }

    mLogService.sendMessage (msg, "ERIS", 0, importance);
}

}
