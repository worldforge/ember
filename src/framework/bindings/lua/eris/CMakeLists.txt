add_library(BindingsEris OBJECT
        BindingsEris.cpp)
target_precompile_headers(BindingsEris
        REUSE_FROM
        BindingsLua)