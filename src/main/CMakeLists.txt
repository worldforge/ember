if (WIN32)
    set(EMBER_EXECUTABLE_NAME ember)
else ()
    set(EMBER_EXECUTABLE_NAME ember.bin)
endif ()

set(SCRIPT_LIBRARIES
        $<TARGET_OBJECTS:ConnectorDefinitions_bindings_lua>
        $<TARGET_OBJECTS:Domain_bindings_lua>
        $<TARGET_OBJECTS:EmberServices_bindings_lua>
        $<TARGET_OBJECTS:Framework_bindings_lua>
        $<TARGET_OBJECTS:Cegui_bindings_lua>
        $<TARGET_OBJECTS:Lua_bindings_lua>
        $<TARGET_OBJECTS:Eris_bindings_lua>
        $<TARGET_OBJECTS:Atlas_bindings_lua>
        $<TARGET_OBJECTS:Varconf_bindings_lua>
        $<TARGET_OBJECTS:EmberOgre_bindings_lua>
        $<TARGET_OBJECTS:Adapters_bindings_lua>
        $<TARGET_OBJECTS:AtlasAdapters_bindings_lua>
        $<TARGET_OBJECTS:Representations_bindings_lua>
        $<TARGET_OBJECTS:Ogre_bindings_lua>
        $<TARGET_OBJECTS:Helpers_bindings_lua>
        )

set(APP_LIBRARIES
        $<TARGET_OBJECTS:emberogre>
        $<TARGET_OBJECTS:caelum>
        $<TARGET_OBJECTS:meshtree>
        $<TARGET_OBJECTS:pagedgeometry>
        $<TARGET_OBJECTS:widgets>
        $<TARGET_OBJECTS:emberogre_terrain>
        $<TARGET_OBJECTS:terrain>
        $<TARGET_OBJECTS:entitymapping>
        $<TARGET_OBJECTS:lua>
        $<TARGET_OBJECTS:cegui>
        $<TARGET_OBJECTS:navigation>
        $<TARGET_OBJECTS:services>
        $<TARGET_OBJECTS:domain>
        $<TARGET_OBJECTS:framework>
        $<TARGET_OBJECTS:ember_tinyxml>
        $<TARGET_OBJECTS:binreloc>
        $<TARGET_OBJECTS:CEGUIOgreRenderer>
        $<TARGET_OBJECTS:DetourTileCache>
        $<TARGET_OBJECTS:Detour>
        $<TARGET_OBJECTS:Recast>
        )

#Use WIN32 to specify that on Windows it should be a non-console app.
add_executable(${EMBER_EXECUTABLE_NAME} WIN32
        Application.cpp
        ConfigBoundLogObserver.cpp
        Ember.cpp
        ConsoleInputBinder.cpp
        ${SCRIPT_LIBRARIES}
        ${APP_LIBRARIES})

#Need to include CMAKE_DL_LIBS to get the dynamic linker.
target_link_libraries(${EMBER_EXECUTABLE_NAME}
        ${CMAKE_DL_LIBS})

target_link_libraries(${EMBER_EXECUTABLE_NAME} ${BULLET_LIBRARIES})
if (EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    conan_target_link_libraries(${EMBER_EXECUTABLE_NAME})
#    if (UNIX)
#        message(STATUS "Adding links to X11 and GL libraries on Unix system")
#        link_libraries(X11 GL Xrandr)
#    endif ()
endif ()

install(TARGETS ${EMBER_EXECUTABLE_NAME} DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})

get_property(_PLUGIN_LIBS_LIST GLOBAL PROPERTY PLUGIN_LIBS_LIST)

wf_add_plugin(ServerWidget
        ${CMAKE_SOURCE_DIR}/src/components/ogre/widgets/ServerWidget.cpp
        )
wf_add_plugin(Quit
        ${CMAKE_SOURCE_DIR}/src/components/ogre/widgets/Quit.cpp
        )
wf_add_plugin(InspectWidget
        ${CMAKE_SOURCE_DIR}/src/components/ogre/widgets/InspectWidget.cpp
        )

wf_add_plugin(Help
        ${CMAKE_SOURCE_DIR}/src/components/ogre/widgets/Help.cpp
        )

wf_add_plugin(ContainerWidget
        ${CMAKE_SOURCE_DIR}/src/components/ogre/widgets/ContainerWidget.cpp
        )

wf_add_plugin(IngameChatWidget
        ${CMAKE_SOURCE_DIR}/src/components/ogre/widgets/IngameChatWidget.cpp
        )

#add_dependencies(${EMBER_EXECUTABLE_NAME} ${_PLUGIN_LIBS_LIST})
foreach (PLUGIN ${_PLUGIN_LIBS_LIST})
    add_dependencies(${PLUGIN} ${EMBER_EXECUTABLE_NAME})
    target_link_libraries(${PLUGIN} ${EMBER_EXECUTABLE_NAME})
endforeach (PLUGIN)

