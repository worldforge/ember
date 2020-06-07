cmake_minimum_required(VERSION 3.2)

#Specifies the version of the media. This should normally be the version after the current version.
set(MEDIA_VERSION "0.8.0")
#If no VERSION is set externally, and we can't figure out the Git version, we'll use this version as fallback.
set(VERSION_FALLBACK "0.8.0-dev")

project(Ember)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/tools/cmake)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# Meta data
set(DESCRIPTION "The Worldforge client.")

include(GNUInstallDirs)
include(FindPkgConfig)

# Version setup. Figure out the version from 'git describe' if VERSION isn't set externally.
message(${PROJECT_SOURCE_DIR})
execute_process(COMMAND git -C ${PROJECT_SOURCE_DIR} describe
        OUTPUT_VARIABLE GIT_REPO_VERSION)
string(REGEX REPLACE "\n$" "" GIT_REPO_VERSION "${GIT_REPO_VERSION}")
if (NOT VERSION)
    if (NOT GIT_REPO_VERSION)
        set(VERSION "${VERSION_FALLBACK}")
    else ()
        set(VERSION "${GIT_REPO_VERSION}")
    endif ()
endif ()

if (NOT VERSION_PACKAGE)
    message(STATUS "Setting VERSION_PACKAGE to ${VERSION}. You can override this by setting the 'VERSION_PACKAGE' variable externally.")
    set(VERSION_PACKAGE "${VERSION}")
endif ()

message(STATUS "Building version ${VERSION}")

#Split a version number like "1.2.3" into a list to extract parts.
string(REPLACE "." ";" VERSION_LIST ${VERSION})
list(GET VERSION_LIST 0 EMBER_VERSION_MAJOR)
list(GET VERSION_LIST 1 EMBER_VERSION_MINOR)
list(GET VERSION_LIST 2 EMBER_VERSION_PATCH)


# Set compiler flags, but only if not running in a CI server.
if ($ENV{CI}==1)
    message(STATUS "Build server detected, will not set any compiler warning flags.")
    set(WF_WARNING_FLAGS "")
else ()
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(WF_WARNING_FLAGS /W3)
    else ()
        set(WF_WARNING_FLAGS -Wall -Winit-self -Wwrite-strings -Wundef -Wno-unused-parameter -Wno-missing-field-initializers -Wno-long-long)
    endif ()
endif ()

# This is needed to make Widget Plugins work.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_definitions(-DPREFIX="${CMAKE_INSTALL_PREFIX}")
add_definitions(-DEMBER_DATADIR="${CMAKE_INSTALL_FULL_DATADIR}")
add_definitions(-DEMBER_LIBDIR="${CMAKE_INSTALL_FULL_LIBDIR}")
add_definitions(-DEMBER_SYSCONFDIR="${CMAKE_INSTALL_FULL_SYSCONFDIR}")
if (NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    # EMBER_SOURCEMEDIAREPODIR points to an optional media repo path, i.e. a Subversion checkout of the media repo
    # found at https://svn.worldforge.org:886/svn/media/trunk/.
    add_definitions(-DEMBER_SOURCEMEDIAREPODIR="${PROJECT_SOURCE_DIR}/mediarepo/trunk")
    # EMBER_PROCESSEDMEDIAREPODIR points to an optional "processed" media path, i.e. a directory where the source media
    # found at EMBER_SOURCEMEDIAREPODIR has been processed using the "scripts/make_dist_media.py" script.
    add_definitions(-DEMBER_PROCESSEDMEDIAREPODIR="${CMAKE_BINARY_DIR}/ember-media-${MEDIA_VERSION}/media")
endif (NOT CMAKE_BUILD_TYPE STREQUAL "Release")


# We've been getting issues with memory corruption in boost when building with GCC -O2 flags, and it
# seems to be related to the asio small memory block recycling code. We'll thus disable it to avoid the
# crashes
add_definitions(-DBOOST_ASIO_DISABLE_SMALL_BLOCK_RECYCLING)

include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/external ${PROJECT_BINARY_DIR}/generated)

enable_testing()
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

#Needs to declare a GLOBAL variable to pass plugin list down to the src/main/CMakeFile.
define_property(GLOBAL PROPERTY PLUGIN_LIBS_LIST
        BRIEF_DOCS "Plugins registered."
        FULL_DOCS "Plugins registered.")
set_property(GLOBAL PROPERTY PLUGIN_LIBS_LIST "")


macro(wf_add_plugin _LIB_NAME _SOURCE_FILES)

    add_library(${_LIB_NAME} MODULE ${_SOURCE_FILES})
    target_compile_options(${_LIB_NAME} PRIVATE ${WF_WARNING_FLAGS})

    install(TARGETS ${_LIB_NAME}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/ember/widgets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/ember/widgets)

    set_property(GLOBAL APPEND PROPERTY PLUGIN_LIBS_LIST "${_LIB_NAME}")
    if (EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
        conan_target_link_libraries(${_LIB_NAME})
    endif ()
    message(STATUS "Registered widget plugin ${_LIB_NAME}")

endmacro()

macro(wf_generate_lua_bindings PKG_PATH)
    get_filename_component(PACKAGE_NAME ${PKG_PATH} NAME)
    get_filename_component(PACKAGE_DIR ${PKG_PATH} DIRECTORY)

    file(GLOB_RECURSE LUA_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" *.pkg *.h)

    #I couldn't get the script copying to work well on UNIX, so we use slightly different approach on windows and unix.
    if (WIN32)
        add_custom_command(
                OUTPUT ${PACKAGE_NAME}.cxx
                COMMAND ${CMAKE_COMMAND} -E echo "Building Lua bindings for ${PACKAGE_NAME}.cxx"
                COMMAND ${CMAKE_BINARY_DIR}/scripts/update_lua_bindings.bat ${PACKAGE_NAME} ${PACKAGE_NAME}.pkg ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}.cxx
                COMMAND ${CMAKE_COMMAND} -E echo "Done."
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE_DIR}
                DEPENDS ${LUA_FILES}
        )
    else ()
        add_custom_command(
                OUTPUT ${PACKAGE_NAME}.cxx
                COMMAND ${CMAKE_COMMAND} -E echo "Building Lua bindings for ${PACKAGE_NAME}.cxx"
                COMMAND TOLUAXX=${TOLUA++_APP} ${PROJECT_SOURCE_DIR}/tools/support/update_lua_bindings.sh ${PACKAGE_NAME} ${PACKAGE_NAME}.pkg ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}.cxx
                COMMAND ${CMAKE_COMMAND} -E echo "Done."
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE_DIR}
                DEPENDS ${LUA_FILES}
        )
    endif ()

    add_library(${PACKAGE_NAME}_bindings_lua OBJECT ${PACKAGE_NAME}.cxx ${ARGN})

    #Add include directories of both the current source directory as well as the one containing the bindings. This allows for easy includes in the pkg-files.
    target_include_directories(${PACKAGE_NAME}_bindings_lua PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE_DIR})

endmacro()


set(WF_LIBRARY_DIRS)
set(WF_INCLUDE_DIRS)
set(WF_LIBRARIES)

if (WIN32)
    link_libraries("Shlwapi") #Needed for getting Windows specific directories.
    add_definitions(-D_WIN32_WINNT=0x0601) #target Windows 7
endif ()


if (EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    message(STATUS "Using Conan for dependency resolution.")
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
    #We need to copy all .dll files that Conan installed too.
    #TODO: make sure that Conan only builds static libraries on Windows.
    if (WIN32)
        file(GLOB DLL_FILES "${CMAKE_BINARY_DIR}/bin/*.dll")
        install(FILES ${DLL_FILES} DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})
    endif ()

    if (NOT WIN32)
        #There's an issue with the Conan provided boost libraries in that they will declare library dependencies as "-l" options
        #rather than links to the ".a" files (as we want static libs). To work around it we need to link directly to the
        #boost filesystem lib, otherwise the system one will be used.
        #This should be removed once it's fixed in the Conan recipes.
        link_libraries("${CONAN_LIB_DIRS_BOOST}/libboost_filesystem.a")
    endif ()

    set(TOLUA++_APP ${CONAN_BIN_DIRS_TOLUA++}/tolua++)

    #We need to add some extra Ogre include directories
    include_directories(${CONAN_INCLUDE_DIRS_OGRE}/Terrain)
    include_directories(${CONAN_INCLUDE_DIRS_OGRE}/RTShaderSystem)
    include_directories(${CONAN_INCLUDE_DIRS_OGRE}/Overlay)
    include_directories(${CONAN_INCLUDE_DIRS_OGRE}/MeshLodGenerator)
    include_directories(${CONAN_INCLUDE_DIRS_OGRE}/Paging)
    link_directories(${CONAN_LIB_DIRS_OGRE}/OGRE)
    #These things should really be exported by the Conan Ogre recipe, but for now we'll define them instead.
    #Main issue is that this is a bit brittle.
    link_libraries(Codec_FreeImageStatic Plugin_ParticleFXStatic RenderSystem_GL3PlusStatic)
    if (UNIX)
        message(STATUS "Adding links to X11 and GL libraries on Unix system")
        link_libraries(X11 GL Xrandr)
    endif ()
    if (WIN32)
        set(OGRE_MEDIA_DIR ${CONAN_OGRE_ROOT}/Media)
    else ()
        set(OGRE_MEDIA_DIR ${CONAN_OGRE_ROOT}/share/OGRE/Media)
    endif ()
else ()
    find_package(Boost
            1.60.0
            REQUIRED
            COMPONENTS system filesystem thread)

    link_directories(${Boost_LIBRARY_DIRS})
    link_libraries(${Boost_LIBRARIES})
    include_directories(${Boost_INCLUDE_DIRS})

    #Check for libunwind, which is optional and if present will allow for the StackChecker feature to be enabled.
    #This allows for a developer to get some insight into why some frames take too long.
    find_package(unwind 1.1)
    if (UNWIND_FOUND)
        message(STATUS "Found libunwind which allows for the 'slow frame stack print' feature to be enabled.")
        link_directories(${UNWIND_LIBRARY_DIRS})
        link_libraries(${UNWIND_LIBRARIES})
        include_directories(${UNWIND_INCLUDE_DIR})
        add_definitions(-DUNWIND_ENABLED)
    endif (UNWIND_FOUND)

    set(CEGUI_STATIC "ON")

    find_package(ZLIB REQUIRED)

    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIR})
    link_directories(${SDL2_LIBRARY_DIRS})
    link_libraries(${SDL2_LIBRARIES})

    find_package(SigC++
            2.0
            REQUIRED)
    list(APPEND WF_LIBRARIES ${SIGC++_LIBRARY})
    list(APPEND WF_INCLUDE_DIRS ${SIGC++_INCLUDE_DIR})

    find_package(Atlas
            0.7.0
            REQUIRED)
    list(APPEND WF_LIBRARIES ${Atlas_LIBRARIES})
    list(APPEND WF_INCLUDE_DIRS ${Atlas_INCLUDE_DIR})
    list(APPEND WF_LIBRARY_DIRS ${Atlas_LIBRARY_DIR})

    find_package(wfmath
            1.0.3
            REQUIRED)
    list(APPEND WF_LIBRARIES ${wfmath_LIBRARIES})
    list(APPEND WF_INCLUDE_DIRS ${wfmath_INCLUDE_DIR})
    list(APPEND WF_LIBRARY_DIRS ${wfmath_LIBRARY_DIR})

    find_package(eris
            1.4.0
            REQUIRED)
    list(APPEND WF_LIBRARIES ${eris_LIBRARIES})
    list(APPEND WF_INCLUDE_DIRS ${eris_INCLUDE_DIR})
    list(APPEND WF_LIBRARY_DIRS ${eris_LIBRARY_DIR})

    find_package(varconf
            1.0.3
            REQUIRED)
    list(APPEND WF_LIBRARIES ${varconf_LIBRARIES})
    list(APPEND WF_INCLUDE_DIRS ${varconf_INCLUDE_DIR})
    list(APPEND WF_LIBRARY_DIRS ${varconf_LIBRARY_DIR})

    find_package(mercator
            0.4.0
            REQUIRED)
    list(APPEND WF_LIBRARIES ${mercator_LIBRARIES})
    list(APPEND WF_INCLUDE_DIRS ${mercator_INCLUDE_DIR})
    list(APPEND WF_LIBRARY_DIRS ${mercator_LIBRARY_DIR})

    find_package(wfut
            0.2.4
            REQUIRED)
    list(APPEND WF_LIBRARIES ${wfut_LIBRARIES})
    list(APPEND WF_INCLUDE_DIRS ${wfut_INCLUDE_DIR})
    list(APPEND WF_LIBRARY_DIRS ${wfut_LIBRARY_DIR})

    link_directories(${WF_LIBRARY_DIRS})
    include_directories(${WF_INCLUDE_DIRS})
    link_libraries(${WF_LIBRARIES})


    #TODO: check for binreloc?


    set(OGREVER 1.12.2)
    find_package(OGRE ${OGREVER} REQUIRED)
    link_directories(${OGRE_LIBRARY_DIRS})
    link_libraries(${OGRE_LIBRARIES})
    include_directories(${OGRE_INCLUDE_DIRS})
    if (APPLE)
        if (NOT OGRE_RenderSystem_GL3Plus_FOUND)
            MESSAGE(FATAL_ERROR "Could not find Ogre RenderSystem GL3Plus plugin. Make sure you've built Ogre with RenderSystem GL3Plus support.")
        endif (NOT OGRE_RenderSystem_GL3Plus_FOUND)
    endif (APPLE)

    if (NOT OGRE_Terrain_FOUND)
        MESSAGE(FATAL_ERROR "Could not find Ogre Terrain component. Make sure you've built Ogre with Terrain support.")
    endif (NOT OGRE_Terrain_FOUND)
    link_libraries(${OGRE_Terrain_LIBRARIES})
    include_directories(${OGRE_Terrain_INCLUDE_DIRS})

    if (NOT OGRE_Paging_FOUND)
        MESSAGE(FATAL_ERROR "Could not find Ogre Paging component. Make sure you've built Ogre with Paging support.")
    endif (NOT OGRE_Paging_FOUND)
    link_libraries(${OGRE_Paging_LIBRARIES})
    include_directories(${OGRE_Paging_INCLUDE_DIRS})

    if (NOT OGRE_Overlay_FOUND)
        MESSAGE(FATAL_ERROR "Could not find Ogre Overlay component. Make sure you've built Ogre with Overlay support.")
    endif (NOT OGRE_Overlay_FOUND)
    link_libraries(${OGRE_Overlay_LIBRARIES})
    include_directories(${OGRE_Overlay_INCLUDE_DIRS})

    if (NOT OGRE_MeshLodGenerator_FOUND)
        MESSAGE(FATAL_ERROR "Could not find Ogre MeshLodGenerator component. Make sure you've built Ogre with MeshLodGenerator support.")
    endif (NOT OGRE_MeshLodGenerator_FOUND)
    link_libraries(${OGRE_MeshLodGenerator_LIBRARIES})
    include_directories(${OGRE_MeshLodGenerator_INCLUDE_DIRS})

    if (NOT OGRE_RTShaderSystem_FOUND)
        MESSAGE(FATAL_ERROR "Could not find Ogre RTShaderSystem component. Make sure you've built Ogre with RTShaderSystem support.")
    endif (NOT OGRE_RTShaderSystem_FOUND)
    link_libraries(${OGRE_RTShaderSystem_LIBRARIES})
    include_directories(${OGRE_RTShaderSystem_INCLUDE_DIRS})

    if (NOT OGRE_Codec_FreeImage_FOUND)
        MESSAGE(FATAL_ERROR "Could not find Ogre FreeImage Codec. Make sure you've built Ogre with FreeImage support.")
    endif (NOT OGRE_Codec_FreeImage_FOUND)


    if (EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake" AND OGRE_STATIC)
        #If we're building with Conan we don't want any absolute references, so we need to clear the library links from some OGRE components.
        message(STATUS "Clearing link libraries for OgreOverlay and Codec_FreeImage to remove any hard coded paths.")
        set_target_properties(OgreOverlay PROPERTIES
                INTERFACE_LINK_LIBRARIES "OgreMain"
                )
        set_target_properties(Codec_FreeImage PROPERTIES
                INTERFACE_LINK_LIBRARIES "OgreMain"
                )
    endif ()

    find_package(Tolua++ REQUIRED)
    link_directories(${TOLUA++_LIBRARY_DIRS})
    link_libraries(${TOLUA++_LIBRARY})
    include_directories(${TOLUA++_INCLUDE_DIR})

    #Allow Lua version to be set from outside, since some system might have CEGUI and tolua++ compiled to 5.2 or 5.3
    if (NOT LUA_VERSION)
        set(LUA_VERSION 5.1)
    endif (NOT LUA_VERSION)
    # Lua should be before CEGUI_LuaScriptModule
    find_package(Lua ${LUA_VERSION} EXACT)
    if (NOT LUA_FOUND)
        MESSAGE(FATAL_ERROR "Could not find Lua ${LUA_VERSION}.")
    endif (NOT LUA_FOUND)
    link_directories(${LUA_LIBRARY_DIRS})
    link_libraries(${LUA_LIBRARIES})
    include_directories(${LUA_INCLUDE_DIR})


    find_package(CEGUI 0.8.7 REQUIRED)
    link_directories(${CEGUI_LIBRARY_DIRS})
    link_libraries(${CEGUI_LIBRARIES})
    include_directories(${CEGUI_INCLUDE_DIRS})

    if (NOT CEGUI_LuaScriptModule_FOUND)
        MESSAGE(FATAL_ERROR "Could not find CEGUI Lua component. Make sure you've built CEGUI with Lua support.")
    endif (NOT CEGUI_LuaScriptModule_FOUND)
    link_libraries(${CEGUI_LuaScriptModule_LIBRARIES})
    include_directories(${CEGUI_LuaScriptModule_INCLUDE_DIRS})

    find_package(SDL2 REQUIRED)
    link_libraries(${SDL2_LIBRARY})
    link_libraries(${SDL2_LIBRARIES})
    include_directories(${SDL2_INCLUDE_DIR})

    find_package(ALUT REQUIRED)
    link_directories(${ALUT_LIBRARY_DIRS})
    link_libraries(${ALUT_LIBRARIES})
    include_directories(${ALUT_INCLUDE_DIRS})

    find_package(OpenAL REQUIRED)
    link_libraries(${OPENAL_LIBRARY})
    link_libraries(${OPENAL_LIBRARIES})
    include_directories(${OPENAL_INCLUDE_DIR})

    if (NOT WIN32)
        find_package(XDGBaseDir REQUIRED 1.0.0)
        link_libraries(${XDGBaseDir_LIBRARIES})
        include_directories(${XDGBaseDir_INLUDE_DIR})
    endif ()

    find_package(OpenGL)
    link_directories(${OPENGL_LIBRARY_DIRS})
    link_libraries(${OPENGL_LIBRARIES})
    include_directories(${OPENGL_INCLUDE_DIR})

    find_package(Bullet 2.81 REQUIRED)
    include_directories(${BULLET_INCLUDE_DIRS})

    find_package(cppunit 1.8.0)

    #We'll use xmllint for validating schemas of some of our xml files.
    find_program(XMLLINT xmllint)

endif ()

if (NOT TOLUA++_APP)
    MESSAGE(FATAL_ERROR "Could not find tolua++ executable.")
endif (NOT TOLUA++_APP)

message(STATUS "Copying OGRE media to our own installation directory.")
set(LOCAL_OGRE_MEDIA_DIR ${CMAKE_INSTALL_FULL_DATADIR}/ember/OGRE/Media)
install(DIRECTORY "${OGRE_MEDIA_DIR}/RTShaderLib" DESTINATION "${LOCAL_OGRE_MEDIA_DIR}")
install(DIRECTORY "${OGRE_MEDIA_DIR}/ShadowVolume" DESTINATION "${LOCAL_OGRE_MEDIA_DIR}")

add_definitions(-DOGRE_PLUGINDIR="${OGRE_PLUGIN_DIR}")


add_subdirectory(external)
add_subdirectory(src)
#Only build tests if the "check" target is used.
add_subdirectory(tests EXCLUDE_FROM_ALL)

file(GLOB MODELDEFINITIONS_FILES data/dural/*.modeldef)
foreach (FILE ${MODELDEFINITIONS_FILES})
    add_custom_command(TARGET check
            COMMAND "${XMLLINT}" --schema "${PROJECT_SOURCE_DIR}/data/modeldefinition.xsd" --nonet --noout "${FILE}")
endforeach (FILE MODELDEFINITIONS_FILES)


configure_file(tools/Version.tmpl.h generated/Version.h @ONLY)
configure_file(tools/support/update_lua_bindings.bat.in scripts/update_lua_bindings.bat @ONLY)
configure_file(tools/bintray.tmpl.json bintray.json @ONLY)
message(STATUS "Installing snapcraft.yaml file into build directory. Copy this one to the installation directory if you want to build snaps.")
configure_file(tools/snapcraft.tmpl.yaml snap/snapcraft.yaml @ONLY)
configure_file(tools/ember.tmpl.conf ember.conf @ONLY)
configure_file(tools/ember.tmpl ember @ONLY)
configure_file(tools/Ember.tmpl.nsi Ember.nsi @ONLY)
configure_file(docs/Doxyfile.in Doxyfile @ONLY)
configure_file(tools/support/refresh-amber-media-dev.sh.in scripts/refresh-amber-media-dev.sh @ONLY)
configure_file(tools/support/make_dist_media.py.in scripts/make_dist_media.py @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ember.conf DESTINATION ${CMAKE_INSTALL_FULL_SYSCONFDIR}/ember)
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/ember DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})
install(FILES tools/org.worldforge.ember.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
install(FILES data/media/ember.png DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/64x64/apps)
install(FILES README.md COPYING AUTHORS NEWS DESTINATION ${CMAKE_INSTALL_DATADIR}/doc/ember)
install(FILES tools/org.worldforge.ember.appdata.xml DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo)

install(DIRECTORY data DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/ember)

file(GLOB SOUND_DEFINITION_FILES "${CMAKE_CURRENT_SOURCE_DIR}/sounddefinitions/*.sounddef")
install(FILES ${SOUND_DEFINITION_FILES} DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/ember/sounddefinitions)

# man files
install(FILES docs/man/man1/ember.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)

set(MEDIA_DIR media-${MEDIA_VERSION})
set(MEDIAREPO_DIR ${PROJECT_SOURCE_DIR}/mediarepo)
set(MEDIAREPO_PROCESSED_DIR ${CMAKE_BINARY_DIR}/ember-media-${MEDIA_VERSION})

find_program(SUBVERSION_CMD NAMES svn)
find_program(PYTHON_CMD NAMES python)
find_program(RSYNC_CMD NAMES rsync)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    #On systems with Snapcraft this command will build a snap, otherwise one has to do some extra copying of the "snap" folder.
    add_custom_target(snap-build)
    add_custom_command(
            TARGET snap-build
            COMMAND ${CMAKE_COMMAND} -E echo "I will now copy the snapfile to the installation directory and run 'snapcraft'."
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_INSTALL_PREFIX}/snap
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/snap/snapcraft.yaml ${CMAKE_INSTALL_PREFIX}/snap/snapcraft.yaml
            COMMAND snapcraft
            WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}
    )
endif ()

add_custom_target(media-download)
if (RSYNC_CMD)
    add_custom_command(
            TARGET media-download
            COMMAND ${CMAKE_COMMAND} -E echo "I will now use rsync to install the media from amber.worldforge.org into ${MEDIA_DIR}."
            COMMAND ${CMAKE_COMMAND} -E make_directory ${MEDIA_DIR}
            COMMAND rsync -rtzu --delete amber.worldforge.org::ember-media/ember-media-${MEDIA_VERSION}/ ${MEDIA_DIR}
            COMMAND ${CMAKE_COMMAND} -E echo "Copying ${MEDIA_DIR}/media to ${CMAKE_INSTALL_FULL_DATADIR}/ember/media."
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${MEDIA_DIR}/media ${CMAKE_INSTALL_FULL_DATADIR}/ember/media
            COMMAND ${CMAKE_COMMAND} -E echo "Done."
            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    )
else (RSYNC_CMD)
    add_custom_command(
            TARGET media-download
            COMMAND ${CMAKE_COMMAND} -E echo "Could not find the Rsync command. The target 'media-download' is disabled."
    )
endif (RSYNC_CMD)


add_custom_target(mediarepo-checkout)
if (SUBVERSION_CMD)
    add_custom_command(
            TARGET mediarepo-checkout
            COMMAND ${CMAKE_COMMAND} -E echo "Using Subversion to checkout https://svn.worldforge.org:886/svn/media/trunk to ${MEDIAREPO_DIR}/trunk."
            COMMAND ${CMAKE_COMMAND} -E make_directory ${MEDIAREPO_DIR}
            COMMAND ${SUBVERSION_CMD} co https://svn.worldforge.org:886/svn/media/trunk ${MEDIAREPO_DIR}/trunk
    )
else (SUBVERSION_CMD)
    add_custom_command(
            TARGET mediarepo-checkout
            COMMAND ${CMAKE_COMMAND} -E echo "Could not find the Subversion command 'svn'. The target 'mediarepo-checkout' is disabled."
    )
endif (SUBVERSION_CMD)

add_custom_target(mediarepo-process)
if (PYTHON_CMD)
    add_custom_command(
            TARGET mediarepo-process
            COMMAND ${CMAKE_COMMAND} -E echo "Processing media in ${MEDIAREPO_DIR}/trunk and placing it in ${MEDIAREPO_PROCESSED_DIR}."
            COMMAND ${CMAKE_BINARY_DIR}/scripts/make_dist_media.py ${MEDIAREPO_DIR}/trunk ${MEDIAREPO_PROCESSED_DIR}
    )
else (PYTHON_CMD)
    add_custom_command(
            TARGET mediarepo-process
            COMMAND ${CMAKE_COMMAND} -E echo "Could not find the Python command 'python'. The target 'mediarepo-process' is disabled."
    )
endif (PYTHON_CMD)

# Doxygen support, exports a "dox" target.

find_package(Doxygen)

if (DOXYGEN_FOUND)

    set(DOXYGEN_INPUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    set(DOXYGEN_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/docs)

    add_custom_command(
            OUTPUT ${DOXYGEN_OUTPUT}
            COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
            COMMAND ${CMAKE_COMMAND} -E echo "Done."
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            DEPENDS ${DOXYGEN_INPUT}
    )

    add_custom_target(dox DEPENDS ${DOXYGEN_OUTPUT})

endif (DOXYGEN_FOUND)

add_custom_command(
        OUTPUT ChangeLog
        COMMAND ${CMAKE_SOURCE_DIR}/scripts/generate-ChangeLog.sh ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR} f12012e7616c191a8926432faf866c8e43854062
)
add_custom_target(changelog DEPENDS ChangeLog)


# Packaging

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${DESCRIPTION})
set(CPACK_PACKAGE_VENDOR "Worldforge")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR "${EMBER_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${EMBER_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${EMBER_VERSION_PATCH}")
#set(CPACK_INSTALL_SCRIPT "sh ${CMAKE_SOURCE_DIR}/support/generate-ChangeLog.sh ${CMAKE_SOURCE_DIR} ${CPACK_PACKAGE_INSTALL_DIRECTORY} 8bd480b053190ffde2afe33af66f484953036f5a")

set(CPACK_SOURCE_GENERATOR TBZ2 ZIP)

set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${VERSION}" CACHE INTERNAL "tarball basename")

set(CPACK_SOURCE_IGNORE_FILES
        # no hidden files
        "/\\\\..+$"
        "~$"
        )

include(CPack)

message(STATUS "*********")
message(STATUS "Ember is configured and ready to be built.")
message(STATUS "To run it you also need media. There are two ways of getting the media:")
message(STATUS "1. Simplest is to use the 'media-download' target. This will use 'rsync' to download only the required preprocessed media.")
message(STATUS "2. If you want to also make changes to the raw media you can use the target 'mediarepo-checkout' to use Subversion to check out the raw media repo.")
message(STATUS "   Note that this might take a while, will download ~10GB of data and will consume ~20GB on disk.")
message(STATUS "In addition, if you've downloaded the raw media repo, but want Ember to use processed media instead of the raw media repo,")
message(STATUS "you can use the target 'mediarepo-process' to create 'processed' media. This requires Python and ImageMagick.")
message(STATUS "*********")