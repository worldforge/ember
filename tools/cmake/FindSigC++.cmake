# - Try to find SigC++-2.0
# Once done this will define
#
#  SIGC++_FOUND - system has SigC++
#  SIGC++_INCLUDE_DIR - the SigC++ include directory
#  SIGC++_LIBRARY - SigC++ library

if(SIGC++_INCLUDE_DIR AND SIGC++_LIBRARIES)
    # Already in cache, be silent
    set(SIGC++_FIND_QUIETLY TRUE)
endif(SIGC++_INCLUDE_DIR AND SIGC++_LIBRARIES)

if (NOT WIN32)
    include(UsePkgConfig)
    pkgconfig(sigc++-2.0 _LibSIGC++IncDir _LibSIGC++LinkDir _LibSIGC++LinkFlags _LibSIGC++Cflags)
endif(NOT WIN32)

find_path(SIGC++_INCLUDE_DIR sigc++/sigc++.h
        PATH_SUFFIXES sigc++-2.0
        PATHS ${_LibSIGC++IncDir} )

find_library(SIGC++_LIBRARY
        NAMES sigc-2.0
        PATHS ${_LibSIGC++LinkDir} )

get_filename_component(sigc++LibDir "${SIGC++_LIBRARY}" PATH)

# search the sigc++config.h include dir under the same root where the library is found

find_path(SIGC++_CONFIG_INCLUDE_DIR sigc++config.h
        PATH_SUFFIXES sigc++-2.0/include
        PATHS ${_LibSIGC++IncDir} "${sigc++LibDir}" ${CMAKE_SYSTEM_LIBRARY_PATH})

# not sure if this include dir is optional or required
# for now it is optional
if(SIGC++_CONFIG_INCLUDE_DIR)
    set(SIGC++_INCLUDE_DIR ${SIGC++_INCLUDE_DIR} "${SIGC++_CONFIG_INCLUDE_DIR}")
endif(SIGC++_CONFIG_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SIGC++  DEFAULT_MSG  SIGC++_LIBRARY SIGC++_INCLUDE_DIR)

mark_as_advanced(SIGC++_INCLUDE_DIR SIGC++_LIBRARY)