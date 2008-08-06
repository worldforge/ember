/*
-----------------------------------------------------------------------------
This source file is based on source files that are part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 Torus Knot Software Ltd
Copyright (c) 2008 Erik Hjortsberg <erik.hjortsberg@iteam.se>
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

You may alternatively use this source under the terms of a specific version of
the OGRE Unrestricted License provided you have obtained such a license from
Torus Knot Software Ltd.
-----------------------------------------------------------------------------
*/
#ifndef __EmberOgreFileSystem_H__
#define __EmberOgreFileSystem_H__

#include <OgrePrerequisites.h>

#include <OgreArchive.h>
#include <OgreArchiveFactory.h>

namespace EmberOgre {

    /** Specialisation of the Archive class to allow reading of files from
        filesystem folders / directories.
        This has been modified from the original Ogre class to:
        1) not visit hidden directories (such as .svn)
        2) not recurse into directories if there's a file named "norecurse" in them
    */
    class FileSystemArchive : public Ogre::Archive
    {
    protected:
        /** Utility method to retrieve all files in a directory matching pattern.
        @param pattern File pattern
        @param recursive Whether to cascade down directories
        @param dirs Set to true if you want the directories to be listed
            instead of files
        @param simpleList Populated if retrieving a simple list
        @param detailList Populated if retrieving a detailed list
        @param currentDir The current directory relative to the base of the
            archive, for file naming
        */
        void findFiles(const Ogre::String& pattern, bool recursive, bool dirs,
            Ogre::StringVector* simpleList, Ogre::FileInfoList* detailList);

    public:
        FileSystemArchive(const Ogre::String& name, const Ogre::String& archType );
        ~FileSystemArchive();

        /// @copydoc Archive::isCaseSensitive
        bool isCaseSensitive(void) const;

        /// @copydoc Archive::load
        void load();
        /// @copydoc Archive::unload
        void unload();

        /// @copydoc Archive::open
        Ogre::DataStreamPtr open(const Ogre::String& filename) const;

        /// @copydoc Archive::list
        Ogre::StringVectorPtr list(bool recursive = true, bool dirs = false);

        /// @copydoc Archive::listFileInfo
        Ogre::FileInfoListPtr listFileInfo(bool recursive = true, bool dirs = false);

        /// @copydoc Archive::find
        Ogre::StringVectorPtr find(const Ogre::String& pattern, bool recursive = true,
            bool dirs = false);

        /// @copydoc Archive::findFileInfo
        Ogre::FileInfoListPtr findFileInfo(const Ogre::String& pattern, bool recursive = true,
            bool dirs = false);

        /// @copydoc Archive::exists
        bool exists(const Ogre::String& filename);

    };

    /** Specialisation of ArchiveFactory for FileSystem files. */
    class FileSystemArchiveFactory : public Ogre::ArchiveFactory
    {
    public:
        virtual ~FileSystemArchiveFactory() {}
        /// @copydoc FactoryObj::getType
        const Ogre::String& getType(void) const;
        /// @copydoc FactoryObj::createInstance
        Ogre::Archive *createInstance( const Ogre::String& name )
        {
            return new EmberOgre::FileSystemArchive(name, "EmberFileSystem");
        }
        /// @copydoc FactoryObj::destroyInstance
        void destroyInstance( Ogre::Archive* arch) { delete arch; }
    };


}

#endif
