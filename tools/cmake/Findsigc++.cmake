# Try to find sigc++
# Once done, this will define
#
# sigc++_FOUND        - system has libsigc++
# sigc++_INCLUDE_DIRS - libsigc++ include directories
# sigc++_LIBRARIES    - libraries need to use libsigc++
#
# and the following imported targets
#
# sigc++::sigc++

find_path(sigc++_INCLUDE_DIR
        NAMES sigc++/sigc++.h
        HINTS ${sigc++ROOT}
        PATH_SUFFIXES sigc++-2.0)

find_library(sigc++_LIBRARY
        NAMES sigc-2.0
        HINTS ${sigc++_ROOT}
        PATH_SUFFIXES ${CMAKE_INSTALL_LIBDIR})

#Sigc++ also places a "sigc++config.h" under PREFIX/lib/sigc++-2.0/include, we need to find it and include the path.
get_filename_component(sigc++_LIBDIR "${sigc++_LIBRARY}" PATH)
find_path(sigc++_CONFIG_INCLUDE_DIR sigc++config.h
        PATH_SUFFIXES sigc++-2.0/include
        PATHS ${sigc++_INCLUDE_DIR} "${sigc++_LIBDIR}" ${CMAKE_SYSTEM_LIBRARY_PATH})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(sigc++
        REQUIRED_VARS sigc++_LIBRARY sigc++_INCLUDE_DIR sigc++_CONFIG_INCLUDE_DIR)

mark_as_advanced(sigc++_FOUND sigc++_LIBRARY sigc++_INCLUDE_DIR sigc++_CONFIG_INCLUDE_DIR)

set(sigc++_INCLUDE_DIRS "${sigc++_INCLUDE_DIR}" "${sigc++_CONFIG_INCLUDE_DIR}")
set(sigc++_LIBRARIES ${sigc++_LIBRARY})

if (sigc++_FOUND AND NOT TARGET sigc++::sigc++)
    add_library(sigc++::sigc++ UNKNOWN IMPORTED)
    set_target_properties(sigc++::sigc++ PROPERTIES
            IMPORTED_LOCATION "${sigc++_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${sigc++_INCLUDE_DIRS}")
endif ()
