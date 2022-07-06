add_subdirectory(atlas)
add_subdirectory(eris)
add_subdirectory(varconf)
add_subdirectory(wfmath)

add_library(BindingsFramework OBJECT
        BindingsFramework.cpp)
target_precompile_headers(BindingsFramework
        REUSE_FROM
        BindingsLua)
if (MSVC)
    #Need to allow for a lot of symbols when compiling the bindings on MSVC.
    target_compile_options(BindingsFramework "/bigobj")
endif ()