add_subdirectory(bindings)

add_library(lua_component OBJECT
        LuaScriptingProvider.cpp
        Connector.cpp
        LuaConsoleObject.cpp
        )
