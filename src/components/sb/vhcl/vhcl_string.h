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

#ifndef VHCL_STRING_H
#define VHCL_STRING_H


#include <string>
#include <vector>

#include <stdarg.h>


// android does not support wstring. needs to hack it
#ifdef ANDROID_BUILD
namespace std
{
   typedef basic_string<wchar_t> wstring;
}
#endif


namespace vhcl
{
   std::string Format( const char * fmt, ... );
   std::wstring Format( const wchar_t * fmt, ... );
   std::string vFormat( const char * fmt, va_list argPtr );
   std::wstring vFormat( const wchar_t * fmt, va_list argPtr );

   bool StartsWith( const std::string & str, const std::string & start );
   bool EndsWith( const std::string & str, const std::string & end );

   std::string Replace( const std::string & orig, const std::string & find, const std::string & replace );
   std::wstring Replace( const std::wstring & orig, const std::wstring & find, const std::wstring & replace );

   std::string ToLower( const std::string & str );
   std::string ToUpper( const std::string & str );

   std::wstring AnsiToWString( const std::string & str );
   std::string WStringToAnsi( const std::wstring & wstr );
   std::wstring Utf8ToWString( const std::string & u8str );

   void Tokenize( const std::string & str, std::vector<std::string> & tokens, const std::string & delimiters = " " );
   void Tokenize( const std::wstring & str, std::vector<std::wstring> & tokens, const std::wstring & delimiters = L" " );

   float ToFloat( const std::string & str );
   float ToFloat( const std::wstring & str );
   double ToDouble( const std::string & str );
   double ToDouble( const std::wstring & str );
   int32_t ToInt( const std::string & str );
   int32_t ToInt( const std::wstring & str );
   int64_t ToInt64( const std::string & str );
   int64_t ToInt64( const std::wstring & str );

   std::string ToString( const int i );

   // filesystem directory helper functions
   void StripPath( const char * source, char * path, char * file );
   void StripPath( const std::string & source, std::string & path, std::string & file );
   void StripPath( const wchar_t * source, wchar_t * path, wchar_t * file );
   void StripPath( const std::wstring & source, std::wstring & path, std::wstring & file );

   void StripExt( const char * source, char * file, char * ext );
   void StripExt( const std::string & source, std::string & file, std::string & ext );
   void StripExt( const wchar_t * source, wchar_t * file, wchar_t * ext );
   void StripExt( const std::wstring & source, std::wstring & file, std::wstring & ext );

   int kbhit();
   int getch();

   // might add these at a later date if we attempt to eliminate C runtime dependencies

   //size_t Length( const char * str );
   //size_t Length( const wchar_t * str );
   //char * Copy( char * dest, const char * source );
   //wchar_t * Copy( wchar_t * dest, const wchar_t * source );
   //char * Cat( char * dest, const char * source );
   //wchar_t * Cat( wchar_t * dest, const wchar_t * source );

   //int32 Compare( const char * string1, const char * string2, const bool caseSensitive = true );
   //int32 Compare( const wchar_t * string1, const wchar_t * string2, const bool caseSensitive = true );
};


#endif  // VHCL_STRING_H
