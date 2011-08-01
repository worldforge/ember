/**
 * Copyright (C) 2002 Bart Vanhauwaert
 *
 * Permission to use, copy, modify, distribute and sell this software
 * for any purpose is hereby granted without fee. This license
 * includes (but is not limited to) standalone compilation or as part
 * of a larger project.
 *
 * This software is provided "as is" without express or implied warranty.
 *
 * For a full statement on warranty and terms and conditions for
 * copying, distribution and modification, please see the comment block
 * at the end of this file.
 *
 * Version 1
 *
 */

#ifndef OSLINK_OSDIR_HEADER_H_
#define OSLINK_OSDIR_HEADER_H_

#ifdef _WIN32
#define OSLINK_OSDIR_WINDOWS
#include "platform/platform_windows.h"
#else
#define OSLINK_OSDIR_POSIX
#endif

#include <string>
#include <string.h>

#if defined(OSLINK_OSDIR_POSIX)

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

namespace oslink
{
	class directory
	{
		public:
			directory(const std::string& aName)
				: handle(opendir(aName.c_str())), willfail(false)
				{
					if (!handle)
						willfail = true;
					else
					{
						dirent* entry = readdir(handle);
						if (entry)
							current = entry->d_name;
						else
							willfail = true;
					}
				}
			~directory()
				{
					if (handle)
						closedir(handle);
				}
			operator void*() const
				{
					return willfail ? (void*)0:(void*)(-1);
				}
			bool isExisting()
			{
				return !willfail;
			}
			std::string next()
				{
					std::string prev(current);
					dirent* entry = readdir(handle);
					if (entry)
						current = entry->d_name;
					else
						willfail = true;
					return prev;
				}

			/**
			 * @brief Creates a new directory recursively.
			 * @param path The full directory path.
			 */
			static void mkdir(const char *path)
			{
			        char opath[256];
			        char *p;
			        size_t len;

			        strncpy(opath, path, sizeof(opath));
			        len = strlen(opath);
			        if(opath[len - 1] == '/')
			                opath[len - 1] = '\0';
			        for(p = opath; *p; p++)
			                if(*p == '/') {
			                        *p = '\0';
			                        if(access(opath, F_OK))
			                                ::mkdir(opath, S_IRWXU);
			                        *p = '/';
			                }
			        if(access(opath, F_OK))         /* if path is not terminated with / */
			                ::mkdir(opath, S_IRWXU);
			}
		private:
			DIR* handle;
			bool willfail;
			std::string current;
	};
}

#elif defined(OSLINK_OSDIR_WINDOWS)

namespace oslink
{
	class directory
	{
		public:
			directory(const std::string& aName)
				: handle(INVALID_HANDLE_VALUE), willfail(false)
				{
					// First check the attributes trying to access a non-directory with 
					// FindFirstFile takes ages
					DWORD attrs = GetFileAttributes(aName.c_str());
					if ( (attrs == 0xFFFFFFFF) || ((attrs && FILE_ATTRIBUTE_DIRECTORY) == 0) )
					{
						willfail = true;
						return;
					}
					std::string Full(aName);
					// Circumvent a problem in FindFirstFile with c:\\* as parameter 
					if ( (Full.length() > 0) && (Full[Full.length()-1] != '\\') )
						Full += "\\";
					WIN32_FIND_DATA entry;
					handle = FindFirstFile( (Full+"*").c_str(), &entry);
					if (handle == INVALID_HANDLE_VALUE)
						willfail = true;
					else
						current = entry.cFileName;
				}
			~directory()
				{
					if (handle != INVALID_HANDLE_VALUE)
						FindClose(handle);
				}

			operator void*() const				
				{
					return willfail ? (void*)0:(void*)(-1);
				}
			bool isExisting()
			{
				return !willfail;
			}
			std::string next()
				{
					std::string prev = current;
					WIN32_FIND_DATA entry;
					int ok = FindNextFile(handle, &entry);
					if (!ok)
						willfail = true;
					else
						current = entry.cFileName;
					return current; 
				}
			static void mkdir(const char *path)
			{
			        char opath[256];
			        char *p;
			        size_t len;

			        strncpy(opath, path, sizeof(opath));
			        len = strlen(opath);
			        if(opath[len - 1] == '/')
			                opath[len - 1] = '\0';
			        for(p = opath; *p; p++)
			                if(*p == '/') {
			                        *p = '\0';
			                        if(access(opath, F_OK))
			                                _mkdir(opath);
			                        *p = '/';
			                }
			        if(access(opath, F_OK))         /* if path is not terminated with / */
			                _mkdir(opath);
			}
			private:
			HANDLE	handle;
			bool	willfail;
			std::string current;
	};
}


#endif

#endif

/**
 *
 * The "library", below, refers to the collection of software functions
 * and/or data contained in this file, prepared so as to be conveniently
 * compiled and linked with application programs (which use some of those
 * functions and data) to form executables.
 *
 *                             NO WARRANTY
 *                              
 * 1. BECAUSE THE LIBRARY IS LICENSED FREE OF CHARGE, THERE IS NO
 * WARRANTY FOR THE LIBRARY, TO THE EXTENT PERMITTED BY APPLICABLE LAW.
 * EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR
 * OTHER PARTIES PROVIDE THE LIBRARY "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE
 * LIBRARY IS WITH YOU.  SHOULD THE LIBRARY PROVE DEFECTIVE, YOU ASSUME
 * THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.
 * 
 * 2. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN
 * WRITING WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY
 * AND/OR REDISTRIBUTE THE LIBRARY AS PERMITTED ABOVE, BE LIABLE TO YOU
 * FOR DAMAGES, INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OR INABILITY TO USE THE
 * LIBRARY (INCLUDING BUT NOT LIMITED TO LOSS OF DATA OR DATA BEING
 * RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD PARTIES OR A
 * FAILURE OF THE LIBRARY TO OPERATE WITH ANY OTHER SOFTWARE), EVEN IF
 * SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGES.
 * 
 * END OF TERMS AND CONDITIONS
 *
 */

