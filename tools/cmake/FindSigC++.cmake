# - Try to find SigC++-2.0
# Once done this will define
#
#  SigC++_FOUND - system has SigC++
#  SigC++_INCLUDE_DIR - the SigC++ include directory
#  SigC++_LIBRARY - SigC++ library

if(SigC++_INCLUDE_DIR AND SigC++_LIBRARIES)
    # Already in cache, be silent
    set(SigC++_FIND_QUIETLY TRUE)
endif(SigC++_INCLUDE_DIR AND SigC++_LIBRARIES)

if (PKG_CONFIG_FOUND)
    pkg_check_modules(SigC++ sigc++-2.0)
endif(PKG_CONFIG_FOUND)

find_path(SigC++_INCLUDE_DIR sigc++/sigc++.h
        PATH_SUFFIXES sigc++-2.0
        PATHS ${_LibSigC++IncDir} )

find_library(SigC++_LIBRARY
        NAMES sigc-2.0
        PATHS ${_LibSigC++LinkDir} )

get_filename_component(sigc++LibDir "${SigC++_LIBRARY}" PATH)

# search the sigc++config.h include dir under the same root where the library is found

find_path(SigC++_CONFIG_INCLUDE_DIR sigc++config.h
        PATH_SUFFIXES sigc++-2.0/include
        PATHS ${_LibSigC++IncDir} "${sigc++LibDir}" ${CMAKE_SYSTEM_LIBRARY_PATH})

# not sure if this include dir is optional or required
# for now it is optional
if(SigC++_CONFIG_INCLUDE_DIR)
    set(SigC++_INCLUDE_DIR ${SigC++_INCLUDE_DIR} "${SigC++_CONFIG_INCLUDE_DIR}")
endif(SigC++_CONFIG_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SigC++  DEFAULT_MSG  SigC++_LIBRARY SigC++_INCLUDE_DIR)

mark_as_advanced(SigC++_INCLUDE_DIR SigC++_LIBRARY)