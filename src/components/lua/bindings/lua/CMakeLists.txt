add_library(BindingsLua OBJECT
        BindingsLua.cpp)

target_precompile_headers(BindingsLua
        PUBLIC
        <sol2/sol.hpp>)

if (MSVC)
    #Need to allow for a lot of symbols when compiling the bindings on MSVC.
    target_compile_options(BindingsLua PRIVATE "/bigobj")
endif ()