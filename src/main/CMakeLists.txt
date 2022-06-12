if (WIN32)
    set(EMBER_EXECUTABLE_NAME ember)
else ()
    set(EMBER_EXECUTABLE_NAME ember.bin)
endif ()

set(SCRIPT_LIBRARIES
        $<TARGET_OBJECTS:BindingsDomain>
        $<TARGET_OBJECTS:BindingsServices>
        $<TARGET_OBJECTS:BindingsCEGUI>
        $<TARGET_OBJECTS:BindingsAdapters>
        $<TARGET_OBJECTS:BindingsAtlasAdapters>
        $<TARGET_OBJECTS:BindingsRepresentations>
        $<TARGET_OBJECTS:BindingsFramework>
        $<TARGET_OBJECTS:BindingsEris>
        $<TARGET_OBJECTS:BindingsVarconf>
        $<TARGET_OBJECTS:BindingsAtlas>
        $<TARGET_OBJECTS:BindingsLua>
        $<TARGET_OBJECTS:BindingsEmberOgre>
        $<TARGET_OBJECTS:BindingsOgre>
        $<TARGET_OBJECTS:BindingsWFMath>
        )

set(APP_LIBRARIES
        $<TARGET_OBJECTS:emberogre>
        $<TARGET_OBJECTS:caelum>
        $<TARGET_OBJECTS:meshtree>
        $<TARGET_OBJECTS:pagedgeometry>
        $<TARGET_OBJECTS:widgets>
        $<TARGET_OBJECTS:emberogre_terrain>
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

#The idea here is to allow symbol exports from the main exe, so that the plugins can import them. Not sure if it works though.
set_property(TARGET ${EMBER_EXECUTABLE_NAME} PROPERTY ENABLE_EXPORTS 1)

#Need to include CMAKE_DL_LIBS to get the dynamic linker.
target_link_libraries(${EMBER_EXECUTABLE_NAME}
        ${WF_LIBRARIES}
        ${CMAKE_DL_LIBS})

target_link_libraries(${EMBER_EXECUTABLE_NAME} ${BULLET_LIBRARIES})
if (EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    conan_target_link_libraries(${EMBER_EXECUTABLE_NAME})
endif ()

install(TARGETS ${EMBER_EXECUTABLE_NAME} DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})

if (WF_USE_WIDGET_PLUGINS)
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

    wf_add_plugin(EntityCreatorWidget
            ${CMAKE_SOURCE_DIR}/src/components/ogre/widgets/EntityCreatorWidget.cpp
            )
    get_property(_PLUGIN_LIBS_LIST GLOBAL PROPERTY PLUGIN_LIBS_LIST)

    #add_dependencies(${EMBER_EXECUTABLE_NAME} ${_PLUGIN_LIBS_LIST})
    foreach (PLUGIN ${_PLUGIN_LIBS_LIST})
        add_dependencies(${PLUGIN} ${EMBER_EXECUTABLE_NAME})
    endforeach (PLUGIN)

endif ()