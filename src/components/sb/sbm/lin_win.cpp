#include "sbm/lin_win.h"

#ifdef WIN32

#else

#endif

#include <string.h>

/////////////////////////////////////////////////////////////////

#ifdef WIN32

int LinWin_strcmp( const char *s1, const char *s2 ) {
	return( _stricmp( s1, s2 ) );
}
int LinWin_strncmp( const char *s1, const char *s2, int n )	{
	return( _strnicmp( s1, s2, n ) );
}

#else

int LinWin_strcmp( const char *s1, const char *s2 ) {
	return( strcasecmp( s1, s2 ) );
}
int LinWin_strncmp( const char *s1, const char *s2, int n )	{
	return( strncasecmp( s1, s2, n ) );
}

#endif

/////////////////////////////////////////////////////////////////
