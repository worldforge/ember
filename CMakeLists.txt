cmake_minimum_required(VERSION 3.2)

project(Ember)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 14)

include(GNUInstallDirs)
include(FindPkgConfig)

# Version setup

set(EMBER_VERSION_MAJOR 0)
set(EMBER_VERSION_MINOR 8)
set(EMBER_VERSION_PATCH 0)

set(VERSION ${EMBER_VERSION_MAJOR}.${EMBER_VERSION_MINOR}.${EMBER_VERSION_PATCH})

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
add_definitions(-DVERSION="${VERSION}")
add_definitions(-DPREFIX="${CMAKE_INSTALL_PREFIX}")
add_definitions(-DEMBER_DATADIR="${CMAKE_INSTALL_FULL_DATADIR}")
add_definitions(-DEMBER_SYSCONFDIR="${CMAKE_INSTALL_FULL_SYSCONFDIR}")

# We've been getting issues with memory corruption in boost when building with GCC -O2 flags, and it
# seems to be related to the asio small memory block recycling code. We'll thus disable it to avoid the
# crashes
add_definitions(-DBOOST_ASIO_DISABLE_SMALL_BLOCK_RECYCLING)


include_directories("${PROJECT_SOURCE_DIR}/src")

enable_testing()
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

# Meta data

macro(wf_generate_lua_bindings PKG_PATH)
    get_filename_component(PACKAGE_NAME ${PKG_PATH} NAME)
    get_filename_component(PACKAGE_DIR ${PKG_PATH} DIRECTORY)

    file(GLOB_RECURSE LUA_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" *.pkg)

    add_custom_command(
            OUTPUT ${PACKAGE_NAME}.cxx
            COMMAND ${CMAKE_COMMAND} -E echo_append "Building Lua bindings for ${PACKAGE_NAME}.cxx"
            COMMAND TOLUAXX=${TOLUA++_APP} ${PROJECT_SOURCE_DIR}/scripts/update_lua_bindings.sh ${PACKAGE_NAME} ${PACKAGE_NAME}.pkg ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}.cxx
            COMMAND ${CMAKE_COMMAND} -E echo "Done."
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE_DIR}
            DEPENDS ${LUA_FILES}
    )

    add_library(${PACKAGE_NAME}_bindings_lua ${PACKAGE_NAME}.cxx ${ARGN})

    #Add include directories of both the current source directory as well as the one containing the bindings. This allows for easy includes in the pkg-files.
    target_include_directories(${PACKAGE_NAME}_bindings_lua PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE_DIR})

endmacro()


find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIBRARY_DIRS})
link_libraries(${SDL2_LIBRARIES})


pkg_check_modules(WF REQUIRED sigc++-2.0>=2.0.0 eris-1.4>=1.4.0 varconf-1.0>=0.6.7 mercator-0.4>=0.4.0 atlascpp-0.7>=0.7.0 wfmath-1.0>=1.0.3 libwfut-0.2>=0.2.2)
link_directories(${WF_LIBRARY_DIRS})
include_directories(${WF_INCLUDE_DIRS})
link_libraries(${WF_LIBRARIES})


find_package(Boost
        1.46.0
        REQUIRED
        COMPONENTS thread date_time system filesystem)

link_directories(${Boost_LIBRARY_DIRS})
link_libraries(${Boost_LIBRARIES})
include_directories(${Boost_INCLUDE_DIRS})

#TODO: check for binreloc?


set(OGREVER 1.10.8)
find_package(OGRE ${OGREVER} REQUIRED)
link_directories(${OGRE_LIBRARY_DIRS})
link_libraries(${OGRE_LIBRARIES})
include_directories(${OGRE_INCLUDE_DIRS})
if (APPLE)
    if (NOT OGRE_RenderSystem_GL_FOUND)
        MESSAGE(FATAL_ERROR "Could not find Ogre RenderSystem GL plugin. Make sure you've built Ogre with RenderSystem GL support.")
    endif (NOT OGRE_RenderSystem_GL_FOUND)
endif (APPLE)
# Required to find glew.h header. Backtrace: OgreSetup.cpp ==> OgreGLContext.h ==> OgreGLPrerequisites.h ==> <GL/glew.h>
include_directories("${OGRE_RenderSystem_GL_INCLUDE_DIR}")

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


#On UNIX with PKG_CONFIG we can be more assertive of the paths than the FindOGRE.cmake macro is, so lets
#get the correct paths from pkg-config.
if (UNIX)
    pkg_check_modules(OGREPKGCFG REQUIRED OGRE>=${OGREVER})

    set(OGRE_DEBUGBUILD FALSE)
    add_definitions(-DOGRE_PLUGINDIR="${OGREPKGCFG_LIBDIR}/OGRE")
    #Check if any library ends with "_d", which indicates that OGRE is a debug build.
    foreach (LIBRARY ${OGREPKGCFG_LIBRARIES})
        string(FIND ${LIBRARY} "_d" IS_DEBUG)
        if (NOT IS_DEBUG MATCHES -1)
            set(OGRE_DEBUGBUILD TRUE)
        endif (NOT IS_DEBUG MATCHES -1)
    endforeach ()

    if (OGRE_DEBUGBUILD)
        MESSAGE(STATUS "Ogre was built in debug mode.")
        add_definitions(-DOGRE_DEBUG_BUILD)
    else (OGRE_DEBUGBUILD)
        MESSAGE(STATUS "Ogre was built in release mode.")
    endif (OGRE_DEBUGBUILD)

else (UNIX)
    #On other systems we need to rely on the FindOGRE.cmake macros finding things in correct places.
    if (OGRE_LIBRARY_DBG)
        set(OGRE_LIBRARY ${OGRE_LIBRARY_DBG})
    else (OGRE_LIBRARY_DBG)
        set(OGRE_LIBRARY ${OGRE_LIBRARY_REL})
    endif (OGRE_LIBRARY_DBG)

    #It's not enough to check for the _DBG and _REL keys since they can both be set. We also need to check if the library ends with "_d.so" to
    #determine if it's built for debug or release.
    string(FIND ${OGRE_LIBRARY} "_d.so" OGRE_DEBUG_BUILD)
    if (OGRE_DEBUG_BUILD MATCHES -1)
        MESSAGE(STATUS "Ogre was built in release mode.")
        add_definitions(-DOGRE_PLUGINDIR="${OGRE_PLUGIN_DIR_REL}")
    else (OGRE_DEBUG_BUILD MATCHES -1)
        MESSAGE(STATUS "Ogre was built in debug mode.")
        add_definitions(-DOGRE_PLUGINDIR="${OGRE_PLUGIN_DIR_DBG}")
        add_definitions(-DOGRE_DEBUG_BUILD)
    endif (OGRE_DEBUG_BUILD MATCHES -1)


endif (UNIX)

add_definitions(-DOGRE_MEDIA_DIR="${OGRE_MEDIA_DIR}")


if (APPLE)
    pkg_check_modules(TOLUAPP tolua++)
    if (NOT TOLUAPP_FOUND)
        #On OSX we'll just use the tolua++ which Hammer provides
        set(TOLUAPP_LIBRARIES "-ltolua++")
    else (APPLE)
        link_directories(${TOLUAPP_LIBRARY_DIRS})
        link_libraries(${TOLUAPP_LIBRARIES})
        include_directories(${TOLUAPP_INCLUDE_DIR})
    endif (APPLE)
else (APPLE)
    find_package(Tolua++ REQUIRED)
    link_directories(${TOLUA++_LIBRARY_DIRS})
    link_libraries(${TOLUA++_LIBRARY})
    include_directories(${TOLUA++_INCLUDE_DIR})
endif (APPLE)

#Allow Lua version to be set from outside, since some system might have CEGUI and tolua++ compiled to 5.2 or 5.3
if (NOT LUA_VERSION)
    set(LUA_VERSION 5.1)
endif (NOT LUA_VERSION)
# Lua should be before CEGUI_LuaScriptModule
find_package(Lua ${LUA_VERSION} EXACT)
if (APPLE AND NOT LUA_FOUND) # Try macports version if frameworks are not available
    pkg_check_modules(LUA lua-${LUA_VERSION})
endif (APPLE AND NOT LUA_FOUND)
if (NOT LUA_FOUND)
    MESSAGE(FATAL_ERROR "Could not find Lua ${LUA_VERSION}.")
endif (NOT LUA_FOUND)
link_directories(${LUA_LIBRARY_DIRS})
link_libraries(${LUA_LIBRARIES})
include_directories(${LUA_INCLUDE_DIR})


find_package(CEGUI 0.8.4 REQUIRED)
link_directories(${CEGUI_LIBRARY_DIRS})
link_libraries(${CEGUI_LIBRARIES})
include_directories(${CEGUI_INCLUDE_DIRS})

if (NOT CEGUI_OgreRenderer_FOUND)
    MESSAGE(FATAL_ERROR "Could not find CEGUI Ogre component. Make sure you've built CEGUI with Ogre support.")
endif (NOT CEGUI_OgreRenderer_FOUND)
link_libraries(${CEGUI_OgreRenderer_LIBRARIES})
include_directories(${CEGUI_OgreRenderer_INCLUDE_DIRS})

if (NOT CEGUI_LuaScriptModule_FOUND)
    MESSAGE(FATAL_ERROR "Could not find CEGUI Lua component. Make sure you've built CEGUI with Lua support.")
endif (NOT CEGUI_LuaScriptModule_FOUND)
link_libraries(${CEGUI_LuaScriptModule_LIBRARIES})
include_directories(${CEGUI_LuaScriptModule_INCLUDE_DIRS})


pkg_check_modules(TINYXML QUIET tinyxml)
if (TINYXML_FOUND)
    link_directories(${TINYXML_LIBRARY_DIRS})
    link_libraries(${TINYXML_LIBRARIES})
    include_directories(${TINYXML_INCLUDE_DIRS})
endif (TINYXML_FOUND)

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

pkg_check_modules(XDG REQUIRED libxdg-basedir>=1.0.0)
link_directories(${XDG_LIBRARY_DIRS})
link_libraries(${XDG_LIBRARIES})
include_directories(${XDG_INCLUDE_DIRS})

find_package(OpenGL)
link_directories(${OPENGL_LIBRARY_DIRS})
link_libraries(${OPENGL_LIBRARIES})
include_directories(${OPENGL_INCLUDE_DIR})

pkg_check_modules(CPPUNIT cppunit>=1.8.0)

#We'll use xmllint for validating schemas of some of our xml files.
find_program(XMLLINT xmllint)

add_subdirectory(src/domain)
add_subdirectory(src/bindings/lua)
add_subdirectory(src/services)
add_subdirectory(src/framework)
add_subdirectory(src/components/cegui)
add_subdirectory(src/components/entitymapping)
add_subdirectory(src/components/lua)
add_subdirectory(src/components/navigation)
add_subdirectory(src/components/ogre)
add_subdirectory(src/components/terrain)
#Only build tests if the "check" target is used.
add_subdirectory(tests EXCLUDE_FROM_ALL)
add_subdirectory(src/main)

file(GLOB MODELDEFINITIONS_FILES data/dural/*.modeldef)
foreach (FILE ${MODELDEFINITIONS_FILES})
    add_custom_command(TARGET check
            COMMAND "${XMLLINT}" --schema "${PROJECT_SOURCE_DIR}/data/modeldefinition.xsd" --nonet --noout "${FILE}")
endforeach (FILE MODELDEFINITIONS_FILES)


configure_file(ember.conf.in ember.conf @ONLY)
configure_file(ember.in ember @ONLY)
configure_file(ember.dox.in ember.dox @ONLY)
configure_file(scripts/refresh-amber-media-dev.sh.in scripts/refresh-amber-media-dev.sh @ONLY)
configure_file(scripts/make_dist_media.py.in scripts/make_dist_media.py @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ember.conf DESTINATION ${CMAKE_INSTALL_FULL_SYSCONFDIR}/ember)
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/ember DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})
install(FILES ember.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
install(FILES media/ember.png DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/64x64/apps)
install(FILES README.md COPYING AUTHORS NEWS DESTINATION ${CMAKE_INSTALL_DATADIR}/doc/ember-${VERSION})

install(DIRECTORY data DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/ember)

file(GLOB SOUND_DEFINITION_FILES "${CMAKE_CURRENT_SOURCE_DIR}/sounddefinitions/*.sounddef")
install(FILES ${SOUND_DEFINITION_FILES} DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/ember/sounddefinitions)

# man files
install(FILES man/man1/ember.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)

set(MEDIA_DIR media-${VERSION})
add_custom_target(media-download)
add_custom_command(
        TARGET media-download
        COMMAND ${CMAKE_COMMAND} -E echo_append "I will now use rsync to install the media from amber.worldforge.org into ${MEDIA_DIR}."
        COMMAND ${CMAKE_COMMAND} -E make_directory ${MEDIA_DIR}
        COMMAND rsync -rtvzu amber.worldforge.org::ember-media/ember-media-${VERSION}/ ${MEDIA_DIR}
        COMMAND ${CMAKE_COMMAND} -E echo "Copying ${MEDIA_DIR}/media to ${CMAKE_INSTALL_FULL_DATADIR}/ember/media."
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${MEDIA_DIR}/media ${CMAKE_INSTALL_FULL_DATADIR}/ember/media
        COMMAND ${CMAKE_COMMAND} -E echo "Done."
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)



# Doxygen support, exports a "docs" target.

find_package(Doxygen)

if (DOXYGEN_FOUND)

    set(DOXYGEN_INPUT ${CMAKE_CURRENT_BINARY_DIR}/ember.dox)
    set(DOXYGEN_OUTPUT doc)

    add_custom_command(
            OUTPUT ${DOXYGEN_OUTPUT}
            COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
            COMMAND ${CMAKE_COMMAND} -E echo "Done."
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            DEPENDS ${DOXYGEN_INPUT}
    )

    add_custom_target(docs DEPENDS ${DOXYGEN_OUTPUT})

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

message("*********\nEmber is configured. Use the 'media-download' target to fetch the needed media. I.e. 'make media-download'.\n*********")