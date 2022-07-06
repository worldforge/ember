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