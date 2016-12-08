add_executable(ember.bin Application.cpp ConfigBoundLogObserver.cpp Ember.cpp)
target_link_libraries(ember.bin
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
        emberogre
        terrain
        entitymapping
        lua
        cegui
        navigation
        services
        domain
        framework)

install(TARGETS ember.bin DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})