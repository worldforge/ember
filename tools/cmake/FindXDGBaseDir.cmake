# - Try to find the XDG-basedir
# Once done this will define
#
#  XDGBaseDir_FOUND - system has XDG-basedir
#  XDGBaseDir_INLUDE_DIR - the XDG-basedir include directory
#  XDGBaseDir_LIBRARIES - the XDG-basedir library

if(XDGBaseDir_INLUDE_DIR AND XDGBaseDir_LIBRARIES)
	set(XDGBaseDir_FOUND true)
else()
	find_path(XDGBaseDir_INLUDE_DIR basedir.h)
	find_library(XDGBaseDir_LIBRARIES NAMES xdg-basedir)


	include(FindPackageHandleStandardArgs)
	find_package_handle_standard_args(XDGBaseDir
			DEFAULT_MSG
			XDGBaseDir_LIBRARIES
			XDGBaseDir_INLUDE_DIR)

	mark_as_advanced(XDGBaseDir_INLUDE_DIR AND XDGBaseDir_LIBRARIES)
endif()
