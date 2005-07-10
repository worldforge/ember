/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as        *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
*                                                                         *
***************************************************************************/
/**
@file 
fileutils.h
@brief
Utility function to manage directories
*/
#ifndef fileutils_H
#define fileutils_H

/**
 * change the current directory and return old one
 * as a MALLOC'ED by system ONE
 * \param *Dirname 
 * \return 
 */
char* ChangeToDir( const char* Dirname );
/**
 * Reestablish directory to current one before changing
 * and free the char* MALLOC'ED by system when changing.
 * \param *oldDirname 
 */
void RetablishDir( char* oldDirname );

#endif //fileutils_H
