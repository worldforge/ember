/*
   This file is part of VHMsg written by Edward Fast at 
   University of Southern California's Institute for Creative Technologies.
   http://www.ict.usc.edu
   Copyright 2008 Edward Fast, University of Southern California

   VHMsg is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   VHMsg is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with VHMsg.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <vhcl/vhcl.h>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <wchar.h>

#if defined(WIN_BUILD)
#include <windows.h>
#include <conio.h>
#endif

#include <string>
#include <vector>
#include <algorithm>


using std::string;
using std::wstring;
using std::vector;


namespace vhcl
{


string Format( const char * fmt, ... )
{
   va_list argList;
   va_start( argList, fmt );
   string result = vFormat( fmt, argList );
   va_end( argList );

   return result;
}


wstring Format( const wchar_t * fmt, ... )
{
   va_list argList;
   va_start( argList, fmt );
   wstring result = vFormat( fmt, argList );
   va_end( argList );

   return result;
}


string vFormat( const char * fmt, va_list argPtr )
{
   // If the string is less than a certain amount of characters,
   // allocate it on the stack because this saves the malloc/free time.
   const int32_t bufSize = 512;
   char stackBuffer[ bufSize ];

   int32_t attemptedSize = bufSize - 1;

   int32_t numChars = vsnprintf( stackBuffer, attemptedSize, fmt, argPtr );
   if ( numChars >= 0 )
   {
      // if we had exactly the number of characters, then the null doesn't automatically get appended
      stackBuffer[ attemptedSize ] = '\0';

      // Got it on the first try.
      return string( stackBuffer );
   }

   // Define a given max size for the longest a string can be
   const int32_t maxSize = 32768;

   // Now use the heap.
   char * heapBuffer = NULL;

   while ( ( numChars == -1 ) && ( attemptedSize < maxSize ) )
   {
      // Try a bigger size
      attemptedSize *= 2;
      delete [] heapBuffer;
      heapBuffer = new char [ attemptedSize + 1 ];
      numChars = vsnprintf( heapBuffer, attemptedSize, fmt, argPtr );
      heapBuffer[ attemptedSize ] = '\0';  // make sure there's a null at the end, doesn't always get appended
   }

   string result = string( heapBuffer );

   delete [] heapBuffer;
   heapBuffer = NULL;

   return result;
}


wstring vFormat( const wchar_t * fmt, va_list argPtr )
{
// TODO: linux
#if defined(WIN_BUILD)
   // If the string is less than a certain amount of characters,
   // allocate it on the stack because this saves the malloc/free time.
   const int32_t bufSize = 512;
   wchar_t stackBuffer[ bufSize ];

   int32_t attemptedSize = bufSize - 1;

   int32_t numChars = _vsnwprintf( stackBuffer, attemptedSize, fmt, argPtr );
   if ( numChars >= 0 )
   {
      // if we had exactly the number of characters, then the null doesn't automatically get appended
      stackBuffer[ attemptedSize ] = '\0';

      // Got it on the first try.
      return wstring( stackBuffer );
   }

   // Define a given max size for the longest a string can be
   const int32_t maxSize = 32768;

   // Now use the heap.
   wchar_t * heapBuffer = NULL;

   while ( ( numChars == -1 ) && ( attemptedSize < maxSize ) )
   {
      // Try a bigger size
      attemptedSize *= 2;
      delete [] heapBuffer;
      heapBuffer = new wchar_t [ attemptedSize + 1 ];
      numChars = _vsnwprintf( heapBuffer, attemptedSize, fmt, argPtr );
      heapBuffer[ attemptedSize ] = '\0';  // make sure there's a null at the end, doesn't always get appended
   }

   wstring result = wstring( heapBuffer );

   delete [] heapBuffer;
   heapBuffer = NULL;

   return result;
#else
   return L"";
#endif
}


bool StartsWith( const string & str, const string & start )
{
   size_t pos = str.find( start );
   return pos == 0;
}


bool EndsWith( const string & str, const string & end )
{
   size_t pos = str.rfind( end );
   return ( pos != string::npos ) && ( pos == ( str.length() - end.length() ) );
}



string Replace( const string & orig, const string & find, const string & replace )
{
   string str = orig;

   size_t pos = 0;

   while ( ( pos = str.find( find, pos ) ) != string::npos && pos < str.length() )
   {
      str.replace( pos, find.length(), replace );
      pos += replace.length();
   }

   return str;
}


wstring Replace( const wstring & orig, const wstring & find, const wstring & replace )
{
   wstring str = orig;

   size_t pos = 0;

   while ( ( pos = str.find( find, pos ) ) != wstring::npos && pos < str.length() )
   {
      str.replace( pos, find.length(), replace );
      pos += replace.length();
   }

   return str;
}


string ToLower( const string & str )
{
   string s = str;
   std::transform( s.begin(), s.end(), s.begin(), tolower );
   return s;
}


string ToUpper( const string & str )
{
   string s = str;
   std::transform( s.begin(), s.end(), s.begin(), toupper );
   return s;
}


wstring AnsiToWString( const string & str )
{
// TODO: linux
#if defined(WIN_BUILD)
   size_t len = str.length() + 1;
   wchar_t * unicodestr = new wchar_t[ len ];
   MultiByteToWideChar( CP_ACP, 0, str.c_str(), -1, unicodestr, (int)len );
   wstring wstr = unicodestr;
   delete [] unicodestr;
   unicodestr = NULL;
   return wstr;
#else
   size_t len = str.length() + 1;
   wchar_t * unicodestr = new wchar_t[ len ];
   mbstowcs( unicodestr, str.c_str(), len );
   wstring wstr = unicodestr;
   delete [] unicodestr;
   unicodestr = NULL;
   return wstr;
#endif
}


string WStringToAnsi( const wstring & wstr )
{
// TODO: see http://stackoverflow.com/questions/215963/how-do-you-properly-use-widechartomultibyte
// TODO: see http://www.dinkumware.com/manuals/?manual=compleat&page=multibyte.html
// TODO: see http://www.codeguru.com/forum/archive/index.php/t-231165.html

// TODO: linux
#if defined(WIN_BUILD)
   size_t len = wstr.length() + 1;
   char * ansistr = new char[ len ];
   WideCharToMultiByte( CP_ACP, 0, wstr.c_str(), -1, ansistr, (int)len, NULL, NULL );
   string str = ansistr;
   delete [] ansistr;
   ansistr = NULL;
   return str;
#else
   size_t len = wstr.length() + 1;
   char * ansistr = new char[ len ];
   wcstombs( ansistr, wstr.c_str(), len );
   string str = ansistr;
   delete [] ansistr;
   ansistr = NULL;
   return str;
#endif
}


wstring Utf8ToWString( const std::string & u8str )
{
// TODO: linux
#if defined(WIN_BUILD)
   size_t wcharcount = u8str.length() + 1;
   wchar_t * tempWstr = new wchar_t[ wcharcount ];
   MultiByteToWideChar( CP_UTF8, 0, u8str.c_str(), -1, tempWstr, (int)wcharcount );
   wstring w = tempWstr;
   delete [] tempWstr;
   return w;
#else
   size_t wcharcount = u8str.length() + 1;
   wchar_t * tempWstr = new wchar_t[ wcharcount ];
   mbstowcs( tempWstr, u8str.c_str(), wcharcount );
   wstring w = tempWstr;
   delete [] tempWstr;
   return w;
#endif
}


// modified from: http://oopweb.com/CPP/Documents/CPPHOWTO/Volume/C++Programming-HOWTO-7.html
// license:       http://oopweb.com/CPP/Documents/CPPHOWTO/Volume/C++Programming-HOWTO-22.html
void Tokenize( const string & str, vector<string> & tokens, const string & delimiters )
{
   // Skip delimiters at beginning.
   string::size_type lastPos = str.find_first_not_of( delimiters, 0 );

   // Find first "non-delimiter".
   string::size_type pos = str.find_first_of( delimiters, lastPos );

   while ( string::npos != pos || string::npos != lastPos )
   {
      // Found a token, add it to the vector.
      tokens.push_back( str.substr( lastPos, pos - lastPos ) );

      // Skip delimiters.  Note the "not_of"
      lastPos = str.find_first_not_of( delimiters, pos );

      // Find next "non-delimiter"
      pos = str.find_first_of( delimiters, lastPos );
   }
}


// modified from: http://oopweb.com/CPP/Documents/CPPHOWTO/Volume/C++Programming-HOWTO-7.html
// license:       http://oopweb.com/CPP/Documents/CPPHOWTO/Volume/C++Programming-HOWTO-22.html
void Tokenize( const wstring & str, vector<wstring> & tokens, const wstring & delimiters )
{
   // Skip delimiters at beginning.
   wstring::size_type lastPos = str.find_first_not_of( delimiters, 0 );

   // Find first "non-delimiter".
   wstring::size_type pos = str.find_first_of( delimiters, lastPos );

   while ( wstring::npos != pos || wstring::npos != lastPos )
   {
      // Found a token, add it to the vector.
      tokens.push_back( str.substr( lastPos, pos - lastPos ) );

      // Skip delimiters.  Note the "not_of"
      lastPos = str.find_first_not_of( delimiters, pos );

      // Find next "non-delimiter"
      pos = str.find_first_of( delimiters, lastPos );
   }
}


float ToFloat( const string & str )
{
   return (float)atof( str.c_str() );
}


float ToFloat( const wstring & str )
{
#if defined(WIN_BUILD)
   return (float)_wtof( str.c_str() );
#else
   return (float)wcstod( str.c_str(), NULL );
#endif
}


double ToDouble( const string & str )
{
   return atof( str.c_str() );
}


double ToDouble( const wstring & str )
{
#if defined(WIN_BUILD)
   return _wtof( str.c_str() );
#else
   return (float)wcstod( str.c_str(), NULL );
#endif
}


int32_t ToInt( const string & str )
{
   return atoi( str.c_str() );
}


int32_t ToInt( const wstring & str )
{
#if defined(WIN_BUILD)
   return _wtoi( str.c_str() );
#else
   return (int32_t)wcstol( str.c_str(), NULL, 10 );
#endif
}


int64_t ToInt64( const string & str )
{
#if defined(WIN_BUILD)
   return _atoi64( str.c_str() );
#else
   return atoll( str.c_str() );
#endif
}


int64_t ToInt64( const wstring & str )
{
// TODO: linux
#if defined(WIN_BUILD)
   return _wtoi64( str.c_str() );
#else
   return 0;
#endif
}


string ToString( const int i )
{
   return vhcl::Format( "%d", i );
}


void StripPath( const char * source, char * path, char * file )
{
   //ASSERT( source );

   if ( path ) strcpy( path, "" );
   if ( file ) strcpy( file, "" );

   size_t len = strlen( source );

   if ( len == 0 )
   {
      return;
   }

   // Set the pointer to the end of the string.
   const char * ptr = source + len - 1;

   // Find the last '/' or '\'
   size_t count = len;
   while ( ( *ptr != '\\' && *ptr != '/' ) &&
           count-- )
   {
      ptr--;
   }

   if ( path )
   {
      if ( ptr > source )
      {
         strncpy( path, source, ptr - source );
         path[ ptr - source ] = '\0';
      }
   }

   if ( file )
   {
      strcpy( file, ptr + 1 );
   }
}


void StripPath( const string & source, string & path, string & file )
{
   path = "";
   file = "";

   size_t pos = source.rfind( "\\" );
   if ( pos == string::npos )
   {
      pos = source.rfind( "/" );
   }
   else
   {
      size_t pos2 = source.rfind( "/" );
      if ( pos2 != string::npos )
      {
         if ( pos2 > pos )
         {
            pos = pos2;
         }
      }
   }

   if ( pos != string::npos )
   {
      path.assign( source, 0, pos );
      file.assign( source, pos+1, source.size()-pos );
   }
   else
   {
      file = source;
   }
}


void StripPath( const wchar_t * source, wchar_t * path, wchar_t * file )
{
   //ASSERT( source );

   if ( path ) wcscpy( path, L"" );
   if ( file ) wcscpy( file, L"" );

   size_t len = wcslen( source );

   if ( len == 0 )
   {
      return;
   }

   // Set the pointer to the end of the string.
   const wchar_t * ptr = source + len - 1;

   // Find the last '/' or '\'
   size_t count = len;
   while ( ( *ptr != L'\\' && *ptr != L'/' ) &&
           count-- )
   {
      ptr--;
   }

   if ( path )
   {
      if ( ptr > source )
      {
         wcsncpy( path, source, ptr - source );
         path[ ptr - source ] = L'\0';
      }
   }

   if ( file )
   {
      wcscpy( file, ptr + 1 );
   }
}


void StripPath( const wstring & source, wstring & path, wstring & file )
{
   path = L"";
   file = L"";

   size_t pos = source.rfind( L"\\" );
   if ( pos == string::npos )
   {
      pos = source.rfind( L"/" );
   }
   else
   {
      size_t pos2 = source.rfind( L"/" );
      if ( pos2 != string::npos )
      {
         if ( pos2 > pos )
         {
            pos = pos2;
         }
      }
   }

   if ( pos != string::npos )
   {
      path.assign( source, 0, pos );
      file.assign( source, pos+1, source.size()-pos );
   }
   else
   {
      file = source;
   }
}


void StripExt( const char * source, char * file, char * ext )
{
   //ASSERT( source );

   if ( file ) strcpy( file, "" );
   if ( ext )  strcpy( ext, "" );

   size_t len = strlen( source );

   if ( len == 0 )
   {
      return;
   }

   // Set the pointer to the end of the string.
   const char * ptr = source + len - 1;

   // Find the last '.'
   size_t count = len;
   while ( *ptr != '.' &&
           count-- )
   {
      ptr--;
   }

   if ( file )
   {
      if ( ptr > source )
      {
         strncpy( file, source, ptr - source );
         file[ ptr - source ] = '\0';
      }
   }

   if ( ext )
   {
      strcpy( ext, ptr + 1 );
   }
}


void StripExt( const string & source, string & file, string & ext )
{
   file = "";
   ext  = "";

   size_t pos = source.rfind( "." );
   if ( pos == string::npos )
   {
      pos = source.rfind( "/" );
   }

   if ( pos != string::npos )
   {
      file.assign( source, 0, pos );
      ext.assign( source, pos+1, source.size()-pos );
   }
   else
   {
      file = source;
   }
}


void StripExt( const wchar_t * source, wchar_t * file, wchar_t * ext )
{
   //ASSERT( source );

   if ( file ) wcscpy( file, L"" );
   if ( ext )  wcscpy( ext, L"" );

   size_t len = wcslen( source );

   if ( len == 0 )
   {
      return;
   }

   // Set the pointer to the end of the string.
   const wchar_t * ptr = source + len - 1;

   // Find the last '.'
   size_t count = len;
   while ( *ptr != L'.' &&
           count-- )
   {
      ptr--;
   }

   if ( file )
   {
      if ( ptr > source )
      {
         wcsncpy( file, source, ptr - source );
         file[ ptr - source ] = L'\0';
      }
   }

   if ( ext )
   {
      wcscpy( ext, ptr + 1 );
   }
}


void StripExt( const wstring & source, wstring & file, wstring & ext )
{
   file = L"";
   ext  = L"";

   size_t pos = source.rfind( L"." );
   if ( pos == string::npos )
   {
      pos = source.rfind( L"/" );
   }

   if ( pos != string::npos )
   {
      file.assign( source, 0, pos );
      ext.assign( source, pos+1, source.size()-pos );
   }
   else
   {
      file = source;
   }
}


int kbhit()
{
#if defined(WIN_BUILD)
   return _kbhit();
#else
   struct timeval tv;
   fd_set read_fd;

   tv.tv_sec = 0;
   tv.tv_usec = 0;
   FD_ZERO( &read_fd );
   FD_SET( 0, &read_fd );

   if ( select( 1, &read_fd, NULL, NULL, &tv ) == -1 )
      return 0;

   if ( FD_ISSET( 0, &read_fd ) )
      return 1;

   return 0;
#endif
}


int getch()
{
#if defined(WIN_BUILD)
   return( _getch() );
#else
   return( getchar() );
#endif
}


// might add these at a later date if we attempt to eliminate C runtime dependencies
#if 0

size_t Length( const char * str )
{
   return strlen( str );
}


size_t Length( const wchar_t * str )
{
   return wcslen( str );
}


char * Copy( char * dest, const char * source )
{
   return strcpy( dest, source );
}


wchar_t * Copy( wchar_t * dest, const wchar_t * source )
{
   return wcscpy( dest, source );
}


char * Cat( char * dest, const char * source )
{
   return strcat( dest, source );
}


wchar_t * Cat( wchar_t * dest, const wchar_t * source )
{
   return wcscat( dest, source );
}


int32 Compare( const char * string1, const char * string2, const bool caseSensitive )
{
   if ( caseSensitive )
   {
      return strcmp( string1, string2 );
   }
   else
   {
      return _stricmp( string1, string2 );
   }
}


int32 Compare( const wchar_t * string1, const wchar_t * string2, const bool caseSensitive )
{
   if ( caseSensitive )
   {
      return wcscmp( string1, string2 );
   }
   else
   {
      return _wcsicmp( string1, string2 );
   }
}

#endif


};
