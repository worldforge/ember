add_library(domain
        EmberEntity.cpp
        EntityTalk.cpp)


wf_generate_lua_bindings(bindings/lua/Domain)


