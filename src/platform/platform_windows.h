//This file contains windows specific code
#ifndef PLATFORM_WINDOWS_H
#define PLATFORM_WINDOWS_H

//These will speed up windows.h, by excluding rarely used functions.
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define _WIN32_IE 0x0500
#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <direct.h>
#include <stdio.h> //_snprintf
#include <string.h> //_stricmp

#define putenv _putenv
#define unlink _unlink
#define chdir _chdir
#define snprintf _snprintf
#define stricmp _stricmp
#define access _access

//NOTE: localtime_s has different return value!
#define localtime_r(a, b) localtime_s(b, a)

//F_OK is not defined in MSVC, but used in osdir.h
#define F_OK 0

#endif //PLATFORM_WINDOWS_H

