/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as        *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
*                                                                         *
***************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>

#ifdef _WIN32
#   include <windows.h>
#   include <direct.h>
#   include <io.h>

#   define S_ISDIR(mode) (mode&S_IFDIR)
#   define STRUCT_STAT  struct _stat
#   define CHDIR        _chdir
#   define GETCWD       _getcwd
#   define MKDIR(A)     _mkdir(A)
#   define STAT(A,S)    _stat(A,S)
#else //_LINUX _APPLE

#   include <malloc.h>
#   include <unistd.h>
#   include <sys/param.h>

#   define MAX_PATH MAXPATHLEN
#   define STRUCT_STAT  struct stat
#   define CHDIR        chdir
#   define GETCWD       getcwd
#   define MKDIR(A)     mkdir(A, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)// set mode of directory to drwxr-xr-x.
#   define STAT(A,S)    stat(A,S)
#endif  //_LINUX _APPLE

static char* GetCurrDir()
{
    // GETCWD MALLOCS A BUFFER. REMEMBER TO FREE IT.
    return GETCWD(0,0);;
}

char * ChangeToDir (const char *Dirname)
{
    STRUCT_STAT st;

    if (STAT(Dirname, &st))
        {
        // doen't exist; must create it
        MKDIR (Dirname);
        }
    if (S_ISDIR(st.st_mode) == 0) 
        {
        // it's not a dir, must create a dir
        MKDIR (Dirname);
        }
    char *oldDirname = GetCurrDir ();
    CHDIR(Dirname);
    return oldDirname;
}


void RetablishDir(char *oldDirname)
{
    if (oldDirname != NULL) 
     {
        ChangeToDir (oldDirname);
        // FREE MALLOC'ED GETCWD BUFFER.
        free (oldDirname);
    }
}
