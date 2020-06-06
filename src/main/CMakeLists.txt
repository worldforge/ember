if (WIN32)
	set(EMBER_EXECUTABLE_NAME ember)
else()
	set(EMBER_EXECUTABLE_NAME ember.bin)
endif()

#Use WIN32 to specify that on Windows it should be a non-console app.
add_executable(${EMBER_EXECUTABLE_NAME} WIN32
        Application.cpp
        ConfigBoundLogObserver.cpp
        Ember.cpp
        ConsoleInputBinder.cpp)

set(SCRIPT_LIBRARIES
        ConnectorDefinitions_bindings_lua
        Domain_bindings_lua
        EmberServices_bindings_lua
        Framework_bindings_lua
        Cegui_bindings_lua
        Lua_bindings_lua
        Eris_bindings_lua
        Atlas_bindings_lua
        Varconf_bindings_lua
        EmberOgre_bindings_lua
        Adapters_bindings_lua
        AtlasAdapters_bindings_lua
        Representations_bindings_lua
        Ogre_bindings_lua
        Helpers_bindings_lua
        )

set(APP_LIBRARIES
        emberogre
        caelum
        meshtree
        pagedgeometry
        widgets
        emberogre_terrain
        terrain
        entitymapping
        lua
        cegui
        navigation
        services
        domain
        framework
        ember_tinyxml
        binreloc
        CEGUIOgreRenderer
        DetourTileCache
        Detour
        Recast
        )

target_link_libraries(${EMBER_EXECUTABLE_NAME}
        ${SCRIPT_LIBRARIES}
        ${APP_LIBRARIES})

target_link_libraries(${EMBER_EXECUTABLE_NAME} ${BULLET_LIBRARIES})
if (EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    conan_target_link_libraries(${EMBER_EXECUTABLE_NAME})
endif ()

install(TARGETS ${EMBER_EXECUTABLE_NAME} DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})

get_property(_PLUGIN_LIBS_LIST GLOBAL PROPERTY PLUGIN_LIBS_LIST)

#add_dependencies(${EMBER_EXECUTABLE_NAME} ${_PLUGIN_LIBS_LIST})
foreach(PLUGIN ${_PLUGIN_LIBS_LIST})
    add_dependencies(${PLUGIN} ${EMBER_EXECUTABLE_NAME})
endforeach(PLUGIN)