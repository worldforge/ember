cmake_minimum_required(VERSION 3.18)

#Specifies the version of the media. This should normally be the version after the current version.
set(MEDIA_VERSION "0.8.0")
#If no VERSION is set externally, and we can't figure out the Git version, we'll use this version as fallback.
set(VERSION_FALLBACK "0.8.0-dev")

project(Ember)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/tools/cmake)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# Meta data
set(DESCRIPTION "The Worldforge client.")

include(GNUInstallDirs)
include(FindPkgConfig)
include(CMakeDependentOption)

# Version setup. Figure out the version from 'git describe' if VERSION isn't set externally.
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

option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." FALSE)
if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_compile_options(-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options(-fcolor-diagnostics)
    endif ()
endif ()

# Set compiler flags, but only if not running in a CI server. CI environments would normally set the environment variable CI.
if (DEFINED ENV{CI})
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(WF_WARNING_FLAGS "")
    else ()
        #Turn of diagnostics. Perhaps a bit too harsh?
        set(WF_WARNING_FLAGS "-w")
    endif ()
else ()
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(WF_WARNING_FLAGS /W3)
    else ()
        set(WF_WARNING_FLAGS -Wall -Winit-self -Wwrite-strings -Wundef -Wno-unused-parameter -Wno-missing-field-initializers -Wno-long-long)
    endif ()
endif ()
MESSAGE(STATUS "Setting compiler warnings to '${WF_WARNING_FLAGS}'")


add_compile_definitions("PREFIX=\"${CMAKE_INSTALL_PREFIX}\"")
add_compile_definitions("EMBER_DATADIR=\"${CMAKE_INSTALL_FULL_DATADIR}\"")
add_compile_definitions("EMBER_LIBDIR=\"${CMAKE_INSTALL_FULL_LIBDIR}\"")
add_compile_definitions("EMBER_SYSCONFDIR=\"${CMAKE_INSTALL_FULL_SYSCONFDIR}\"")
if (NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    # EMBER_SOURCEMEDIAREPODIR points to an optional media repo path, i.e. a Subversion checkout of the media repo
    # found at https://svn.worldforge.org:886/svn/media/trunk/.
    add_compile_definitions("EMBER_SOURCEMEDIAREPODIR=\"${PROJECT_SOURCE_DIR}/mediarepo/trunk\"")
    # EMBER_PROCESSEDMEDIAREPODIR points to an optional "processed" media path, i.e. a directory where the source media
    # found at EMBER_SOURCEMEDIAREPODIR has been processed using the "scripts/make_dist_media.py" script.
    add_compile_definitions("EMBER_PROCESSEDMEDIAREPODIR=\"${CMAKE_BINARY_DIR}/ember-media-${MEDIA_VERSION}/media\"")
endif (NOT CMAKE_BUILD_TYPE STREQUAL "Release")

# On Windows there are some headers that define "min" and "max" as preprocessor macros, which messes up compilation. This tells them to stop.
add_compile_definitions("NOMINMAX")

if (APPLE)
    #On Mac we're having trouble with "iconv" not being linked properly, so we'll do it here.
    link_libraries(iconv)
endif ()

include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/external ${PROJECT_BINARY_DIR}/generated)

enable_testing()
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

#On Unix we'll automatically build all widgets as reloadable modules, but not on Windows or OSX (since we couldn't get it to work as we would like).
cmake_dependent_option(WF_USE_WIDGET_PLUGINS "Build widgets as reloadable plugins." 1 "UNIX AND NOT APPLE" 0)

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

    if (WIN32)
        #We need to tell msvc to ignore any problems with resolving symbols since they will be resolved when the plugin is loaded.
        #TODO: make the symbols exported anyway so this isn't needed
        set_target_properties(${_LIB_NAME} PROPERTIES LINK_FLAGS "/FORCE:UNRESOLVED")
    elseif ()
        set_target_properties(${_LIB_NAME} PROPERTIES LINK_FLAGS "-Wl,-undefined -Wl,dynamic_lookup \
    -Wl,-no_pie \
    -Wl,-search_paths_first")
    endif ()

    set_property(GLOBAL APPEND PROPERTY PLUGIN_LIBS_LIST "${_LIB_NAME}")
    message(STATUS "Registered widget plugin ${_LIB_NAME}")

endmacro()


set(WF_LIBRARY_DIRS)
set(WF_INCLUDE_DIRS)
set(WF_LIBRARIES)

if (WIN32)
    link_libraries(Shlwapi) #Needed for getting Windows specific directories.
    add_compile_definitions("_WIN32_WINNT=0x0601") #target Windows 7
    add_compile_definitions("_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING") #CEGUI has a String iterator that inherits from std::iterator, which shouldn't be done. We don't want to see the warning from MSVC all the time.
endif ()


if (EXISTS "${CMAKE_BINARY_DIR}/conan_paths.cmake")
    message(STATUS "Using Conan for dependency resolution.")
    SET(CONAN_FOUND TRUE)
    include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
    #We need to copy all .dll files that Conan installed too.
    #TODO: make sure that Conan only builds static libraries on Windows.
    if (WIN32)
        file(GLOB DLL_FILES "${CMAKE_BINARY_DIR}/bin/*.dll")
        install(FILES ${DLL_FILES} DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})
    endif ()

    #    if (NOT WIN32)
    #        #There's an issue with the Conan provided boost libraries in that they will declare library dependencies as "-l" options
    #        #rather than links to the ".a" files (as we want static libs). To work around it we need to link directly to the
    #        #boost filesystem lib, otherwise the system one will be used.
    #        #This should be removed once it's fixed in the Conan recipes.
    #        list(APPEND WF_LIBRARIES "${CONAN_LIB_DIRS_BOOST}/libboost_filesystem.a")
    #    endif ()

    #Building with Conan we'll use static linking, so we'll disable the widget plugins as they won't work with static code.
    set(WF_USE_WIDGET_PLUGINS 0)

endif ()
find_package(Boost
        1.60.0
        REQUIRED
        COMPONENTS system filesystem thread)

link_libraries(Boost::boost)
link_libraries(Boost::filesystem)

#Check for libunwind, which is optional and if present will allow for the StackChecker feature to be enabled.
#This allows for a developer to get some insight into why some frames take too long.
find_package(unwind 1.1)
if (UNWIND_FOUND)
    message(STATUS "Found libunwind which allows for the 'slow frame stack print' feature to be enabled.")
    link_directories(${UNWIND_LIBRARY_DIRS})
    link_libraries(${UNWIND_LIBRARIES})
    include_directories(SYSTEM ${UNWIND_INCLUDE_DIR})
    add_compile_definitions("UNWIND_ENABLED")
endif (UNWIND_FOUND)


find_package(SDL2 REQUIRED)
include_directories(SYSTEM ${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIBRARY_DIRS})
link_libraries(${SDL2_LIBRARIES})

find_package(sigc++
        2.0
        REQUIRED)
link_libraries(sigc++::sigc++)


find_package(Atlas
        #Needed to make CMake on Windows find the packages. There's probably a better way to package Atlas on Windows
        #that would prevent this though.
        PATHS ${CONAN_ATLAS_ROOT}
        0.7.0
        REQUIRED)
link_directories(Atlas::Atlas)

find_package(wfmath
        1.0.3
        REQUIRED)
link_libraries(wfmath::wfmath)

find_package(eris
        1.4.0
        REQUIRED)
link_libraries(eris::eris)


find_package(varconf
        1.0.3
        REQUIRED)
link_libraries(varconf::varconf)

find_package(mercator
        0.4.0
        REQUIRED)
link_libraries(mercator::mercator)

find_package(wfut
        0.2.4
        REQUIRED)
link_libraries(wfut::wfut)

find_package(OGRE
        PATHS ${CONAN_OGRE_ROOT}
        13.4.2
        REQUIRED)
if (CONAN_FOUND)
    #With Conan there will be a hard coded path to Freetype within the Overlay target. Let's remove that and link to
    # Freetype ourselves.
    find_package(Freetype REQUIRED)
    link_libraries(Freetype::Freetype)
    set_target_properties(OgreOverlay PROPERTIES
            INTERFACE_LINK_LIBRARIES "OgreMain;$<LINK_ONLY:ZLIB::ZLIB>"
            )

    #If we're building with Conan we're statically linking Ogre, and need to link to the RenderSystem.
    link_libraries(RenderSystem_GL3Plus)
endif ()

link_libraries(OgreMain)

if (APPLE)
    if (NOT OGRE_RenderSystem_GL3Plus_FOUND)
        MESSAGE(FATAL_ERROR "Could not find Ogre RenderSystem GL3Plus plugin. Make sure you've built Ogre with RenderSystem GL3Plus support.")
    endif (NOT OGRE_RenderSystem_GL3Plus_FOUND)
endif (APPLE)

if (NOT OGRE_Terrain_FOUND)
    MESSAGE(FATAL_ERROR "Could not find Ogre Terrain component. Make sure you've built Ogre with Terrain support.")
endif (NOT OGRE_Terrain_FOUND)
link_libraries(OgreTerrain)

if (NOT OGRE_Overlay_FOUND)
    MESSAGE(FATAL_ERROR "Could not find Ogre Overlay component. Make sure you've built Ogre with Overlay support.")
endif (NOT OGRE_Overlay_FOUND)
link_libraries(OgreOverlay)

if (NOT OGRE_MeshLodGenerator_FOUND)
    MESSAGE(FATAL_ERROR "Could not find Ogre MeshLodGenerator component. Make sure you've built Ogre with MeshLodGenerator support.")
endif (NOT OGRE_MeshLodGenerator_FOUND)
link_libraries(OgreMeshLodGenerator)

if (NOT OGRE_RTShaderSystem_FOUND)
    MESSAGE(FATAL_ERROR "Could not find Ogre RTShaderSystem component. Make sure you've built Ogre with RTShaderSystem support.")
endif (NOT OGRE_RTShaderSystem_FOUND)
link_libraries(OgreRTShaderSystem)

if (NOT OGRE_Codec_FreeImage_FOUND)
    MESSAGE(FATAL_ERROR "Could not find Ogre FreeImage Codec. Make sure you've built Ogre with FreeImage support.")
endif (NOT OGRE_Codec_FreeImage_FOUND)
link_libraries(Codec_FreeImage)

if (NOT OGRE_Plugin_ParticleFX_FOUND)
    MESSAGE(FATAL_ERROR "Could not find Ogre ParticleFX Plugin. Make sure you've built Ogre with ParticleFX support.")
endif (NOT OGRE_Plugin_ParticleFX_FOUND)
link_libraries(Plugin_ParticleFX)

#I just couldn't get Windows to find Lua when provided by Conan, no matter what I tried. As a fallback we'll instead
# use the auto-generated "findlua.cmake" provided by the Conan "cmake_find_package" generator. Preferably this should
# instead be handled by CMake and the paths provided by the "cmake_paths" generator. But here we are.
#Seems also be true for MacOS, so we'll enable this for all platforms if Conan is used.
if (CONAN_FOUND)
    message(STATUS "Looking for Lua through Conan.")
    find_package(lua 5.3 EXACT REQUIRED)
    link_libraries(lua::lua)
else ()
    find_package(Lua 5.3 EXACT REQUIRED)
    link_directories(${LUA_LIBRARY_DIRS})
    link_libraries(${LUA_LIBRARIES})
    include_directories(SYSTEM ${LUA_INCLUDE_DIR})
endif ()
if (CONAN_FOUND)
    set(CEGUI_STATIC "ON")
endif ()
find_package(CEGUI
        0.8.7
        REQUIRED)
link_directories(${CEGUI_LIBRARY_DIRS})
link_libraries(${CEGUI_LIBRARIES})
include_directories(SYSTEM ${CEGUI_INCLUDE_DIRS})
if (CONAN_FOUND)
    link_libraries(${CEGUI_TinyXMLParser_LIBRARIES})
    link_libraries(${CEGUI_FreeImageImageCodec_LIBRARIES})
    link_libraries(${CEGUI_CoreWindowRendererSet_LIBRARIES})
    find_package(PCRE REQUIRED)
    link_libraries(PCRE::PCRE)
    find_package(TinyXML REQUIRED)
    link_libraries(TinyXML::TinyXML)
endif ()

find_package(SDL2 REQUIRED)
link_libraries(${SDL2_LIBRARY})
link_libraries(${SDL2_LIBRARIES})
include_directories(SYSTEM ${SDL2_INCLUDE_DIR})

find_package(OpenAL REQUIRED)
link_libraries(${OPENAL_LIBRARY} ${OPENAL_LIBRARIES})
include_directories(SYSTEM ${OPENAL_INCLUDE_DIR})
#Target OpenAL::OpenAL not available until cmake 3.25
#    link_libraries(OpenAL::OpenAL)
#If we're building with Conan we're statically linking OpenAL, and need to mark this, at least on Windows.
if (CONAN_FOUND)
    add_compile_definitions("AL_LIBTYPE_STATIC")
endif ()

if (NOT WIN32)
    find_package(XDGBaseDir REQUIRED 1.0.0)
    link_libraries(${XDGBaseDir_LIBRARIES})
    include_directories(SYSTEM ${XDGBaseDir_INLUDE_DIR})
endif ()

find_package(OpenGL)
link_libraries(OpenGL::GL)

find_package(Bullet 2.81 REQUIRED)
include_directories(SYSTEM ${BULLET_INCLUDE_DIRS})
link_libraries(${BULLET_LIBRARIES})

find_package(ZLIB REQUIRED)
link_libraries(ZLIB::ZLIB)

find_package(cppunit 1.8.0)


if (WF_USE_WIDGET_PLUGINS)
    # This is needed to make Widget Plugins work.
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    add_compile_definitions("WF_USE_WIDGET_PLUGINS")
endif ()

#We'll use xmllint for validating schemas of some of our xml files.
find_program(XMLLINT xmllint)


set(LOCAL_OGRE_MEDIA_DIR ${CMAKE_INSTALL_FULL_DATADIR}/ember/OGRE/Media)
message(STATUS "Copying OGRE media (from ${OGRE_MEDIA_DIR}) to our own installation directory (${LOCAL_OGRE_MEDIA_DIR}).")
install(DIRECTORY "${OGRE_MEDIA_DIR}/RTShaderLib" DESTINATION "${LOCAL_OGRE_MEDIA_DIR}")
install(DIRECTORY "${OGRE_MEDIA_DIR}/Main" DESTINATION "${LOCAL_OGRE_MEDIA_DIR}")

add_compile_definitions("OGRE_PLUGINDIR=\"${OGRE_PLUGIN_DIR}\"")

#It would be better if Bullet provided a header which specified how it was built...
OPTION(BT_USE_DOUBLE_PRECISION "Using double precision with Bullet. This should match what Bullet has been compiled with." OFF)

if (BT_USE_DOUBLE_PRECISION)
    message(STATUS "Using double precision with Bullet. Make sure that Bullet is compiled with this too.")
    add_compile_definitions("BT_USE_DOUBLE_PRECISION=1")
endif ()


add_subdirectory(external)
add_subdirectory(src)
#Only build tests if the "check" target is used.
add_subdirectory(tests EXCLUDE_FROM_ALL)

file(GLOB MODELDEFINITIONS_FILES data/dural/*.modeldef)
foreach (FILE ${MODELDEFINITIONS_FILES})
    add_custom_command(TARGET check
            COMMAND "${XMLLINT}" --schema "${PROJECT_SOURCE_DIR}/data/modeldefinition.xsd" --nonet --noout "${FILE}")
endforeach ()

file(GLOB ENTITY_RECIPES_FILES "data/entityrecipes/*.entityrecipe" "data/entityrecipes/*.xsd")
install(FILES ${ENTITY_RECIPES_FILES} DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/ember/entityrecipes)


configure_file(tools/Version.tmpl.h generated/Version.h @ONLY)
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

find_package(Subversion)
find_package(Python3 COMPONENTS Interpreter)
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
if (Subversion_FOUND)
    add_custom_command(
            TARGET mediarepo-checkout
            COMMAND ${CMAKE_COMMAND} -E echo "Using Subversion to checkout https://svn.worldforge.org:886/svn/media/trunk to ${MEDIAREPO_DIR}/trunk."
            COMMAND ${CMAKE_COMMAND} -E make_directory ${MEDIAREPO_DIR}
            COMMAND ${Subversion_SVN_EXECUTABLE} co https://svn.worldforge.org:886/svn/media/trunk ${MEDIAREPO_DIR}/trunk
    )
else (Subversion_FOUND)
    add_custom_command(
            TARGET mediarepo-checkout
            COMMAND ${CMAKE_COMMAND} -E echo "Could not find the Subversion command 'svn'. The target 'mediarepo-checkout' is disabled."
    )
endif (Subversion_FOUND)

add_custom_target(mediarepo-process)
if (Python3_Interpreter_FOUND)
    add_custom_command(
            TARGET mediarepo-process
            COMMAND ${CMAKE_COMMAND} -E echo "Processing media in ${MEDIAREPO_DIR}/trunk and placing it in ${MEDIAREPO_PROCESSED_DIR}."
            COMMAND ${Python3_EXECUTABLE} ${CMAKE_BINARY_DIR}/scripts/make_dist_media.py ${MEDIAREPO_DIR}/trunk ${MEDIAREPO_PROCESSED_DIR}
    )
else (Python3_Interpreter_FOUND)
    add_custom_command(
            TARGET mediarepo-process
            COMMAND ${CMAKE_COMMAND} -E echo "Could not find the Python command 'python'. The target 'mediarepo-process' is disabled."
    )
endif (Python3_Interpreter_FOUND)

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