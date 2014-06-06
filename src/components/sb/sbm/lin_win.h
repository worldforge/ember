#ifndef LIN_WIN_PORTABILITY_H
#define LIN_WIN_PORTABILITY_H

#ifdef WIN32
#else

#include <stdio.h>
#include <string.h>

#define _snprintf snprintf

#define _stricmp strcasecmp
#define _strncmp strncasecmp
#define _strdup strdup

#define _getcwd getcwd

//#ifndef abs
//#define abs fabs
//#endif

#endif

int LinWin_strcmp( const char *s1, const char *s2 );
int LinWin_strncmp( const char *s1, const char *s2, int n );

#endif
