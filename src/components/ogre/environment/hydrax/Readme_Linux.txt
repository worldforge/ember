#
#	Hydrax 0.4
#	Version for Linux
#
#	adapted by Vincenzo Greco
#	05/09/2008
#	

Dependencies
============
Before building Hydrax, these libraries/apps must be available on your system:

CMake 2.6 -> http://www.cmake.org/

Ogre -> http://www.ogre3d.org  (for lib)
OIS  -> http://sourceforge.net/projects/wgois  (for demo)


Build
=====
To compile Hydrax and Demo just move to the root directory and run:

    cmake -G "Unix Makefiles"

and then,

    make


Changes
=======
* Deleted reference (&) from 'Ogre::TexturePtr& mPerlinTexture0' and 'Ogre::TexturePtr& mPerlinTexture1' in Perlin.cpp
  to avoid compilation error.

* Fixed Cg reflection bug under OpenGL in MaterialManager.cpp (http://www.ogre3d.org/phpBB2/viewtopic.php?t=38148&postdays=0&postorder=asc&start=302).

* Added CMakeLists.txt and 'build' directory needed for compilation.

* Fixed Demo to get it working on linux.

* Deleted some file (MSVC and C::B files)

* Included the GNU Lesser General Public License version 2.1 (lgpl-2.1.txt).

