add_library(BindingsVarconf OBJECT
        BindingsVarconf.cpp)
target_precompile_headers(BindingsVarconf
        REUSE_FROM
        BindingsLua)
if (MSVC)
    #Need to allow for a lot of symbols when compiling the bindings on MSVC.
    target_compile_options(BindingsVarconf PRIVATE "/bigobj")
endif ()