file(GLOB BINDINGS_FILES *.cpp)

add_library(BindingsEris OBJECT
        ${BINDINGS_FILES})

target_precompile_headers(BindingsEris
        REUSE_FROM
        BindingsLua)
if (MSVC)
    #Need to allow for a lot of symbols when compiling the bindings on MSVC.
    target_compile_options(BindingsEris "/bigobj")
endif ()