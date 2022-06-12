add_library(BindingsLua OBJECT
        BindingsLua.cpp)

target_precompile_headers(BindingsLua
        PUBLIC
        <sol2/sol.hpp>)