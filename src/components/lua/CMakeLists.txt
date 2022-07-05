add_subdirectory(bindings)

set(SCRIPT_LIBRARIES
        $<TARGET_OBJECTS:BindingsDomain>
        $<TARGET_OBJECTS:BindingsServices>
        $<TARGET_OBJECTS:BindingsCEGUI>
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

add_library(lua_component STATIC
        LuaScriptingProvider.cpp
        Connector.cpp
        LuaConsoleObject.cpp
        ${SCRIPT_LIBRARIES}
        )

