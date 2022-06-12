add_subdirectory(bindings)

add_library(lua OBJECT
        LuaScriptingProvider.cpp
        Connector.cpp
        LuaConsoleObject.cpp
        )
