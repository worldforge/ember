add_library(BindingsWFMath OBJECT
        BindingsWFMath.cpp)
target_precompile_headers(BindingsWFMath
        REUSE_FROM
        BindingsLua)
if (MSVC)
    #Need to allow for a lot of symbols when compiling the bindings on MSVC.
    target_compile_options(BindingsWFMath "/bigobj")
endif ()