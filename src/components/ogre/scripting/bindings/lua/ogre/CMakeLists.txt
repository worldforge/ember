file(GLOB BINDINGS_FILES *.cpp)

add_library(BindingsOgre OBJECT
        ${BINDINGS_FILES}
        )
target_precompile_headers(BindingsOgre
        REUSE_FROM
        BindingsLua)
target_precompile_headers(BindingsOgre
        REUSE_FROM
        emberogre)

if (MSVC)
    #Need to allow for a lot of symbols when compiling the bindings on MSVC.
    target_compile_options(BindingsOgre PRIVATE "/bigobj")
endif ()