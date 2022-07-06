file(GLOB BINDINGS_FILES *.cpp)

add_library(BindingsEris OBJECT
        ${BINDINGS_FILES})

target_precompile_headers(BindingsEris
        REUSE_FROM
        BindingsLua)