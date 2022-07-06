add_library(BindingsAtlas OBJECT
        BindingsAtlas.cpp)
target_precompile_headers(BindingsAtlas
        REUSE_FROM
        BindingsLua)
if (MSVC)
    #Need to allow for a lot of symbols when compiling the bindings on MSVC.
    target_compile_options(BindingsAtlas "/bigobj")
endif ()