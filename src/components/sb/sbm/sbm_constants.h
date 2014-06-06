/*
 *  sbm_constants.h - part of SmartBody-lib
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
 *      Marcelo Kallmann, USC (currently at UC Merced)
 *      Marcus Thiebaux, USC
 *      Andrew n marshall, USC
 *      Ashok Basawapatna, USC (no longer)
 */

#ifndef SR_FLAG_H
#define SR_FLAG_H

#ifndef NULL
#define NULL 0x0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef CMD_SUCCESS
#define CMD_SUCCESS 0
#endif
#ifndef CMD_FAILURE
#define CMD_FAILURE -1
#endif
#ifndef CMD_NOT_FOUND
#define CMD_NOT_FOUND 1
#endif


#define EMPTY_STRING	""
#define WHITE_SPACE		" \t\n\r"


#define MAX_FILENAME_LEN	(512)
#define MAX_JOINT_LABEL_LEN	(128)

#endif
